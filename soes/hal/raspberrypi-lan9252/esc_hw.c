/*
 * Licensed under the GNU General Public License version 2 with exceptions. See
 * LICENSE file in the project root for full license information
 */

/** \file
 * \brief
 * ESC hardware layer functions for LAN9252 through BCM2835 SPI on Raspberry PI.
 *
 * Function to read and write commands to the ESC. Used to read/write ESC
 * registers and memory.
 */
#include "esc.h"
#include "esc_hw.h"
#include <string.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <bcm2835.h>
#include "ecat_options.h"
#include "utypes.h"
#include "log.h"
#include "user_message.h"


#define BIT(x)	(1U << (x))

#define ESC_CMD_SERIAL_WRITE     0x02
#define ESC_CMD_SERIAL_READ      0x03

#define ESC_CMD_RESET_CTL        0x01F8      // reset register
#define ESC_CMD_HW_CFG           0x0074      // hardware configuration register
#define ESC_CMD_BYTE_TEST        0x0064      // byte order test register
#define ESC_CMD_ID_REV           0x0050      // chip ID and revision
#define ESC_CMD_IRQ_CFG          0x0054      // interrupt configuration
#define ESC_CMD_INT_EN           0x005C      // interrupt enable

#define ESC_RESET_DIGITAL        0x00000001
#define ESC_RESET_ETHERCAT       0x00000040
#define ESC_RESET_CTRL_RST       (ESC_RESET_DIGITAL & ESC_RESET_ETHERCAT)
#define ESC_HW_CFG_READY         0x08000000
#define ESC_BYTE_TEST_OK         0x87654321

#define ESC_PRAM_RD_FIFO_REG     0x0000
#define ESC_PRAM_WR_FIFO_REG     0x0020
#define ESC_PRAM_RD_ADDR_LEN_REG 0x0308
#define ESC_PRAM_RD_CMD_REG      0x030C
#define ESC_PRAM_WR_ADDR_LEN_REG 0x0310
#define ESC_PRAM_WR_CMD_REG      0x0314

#define ESC_PRAM_CMD_BUSY        0x80000000
#define ESC_PRAM_CMD_ABORT       0x40000000
#define ESC_PRAM_CMD_AVAIL       0x00000001
#define ESC_PRAM_CMD_CNT(x)      (((x) >> 8) & 0x1F)
#define ESC_PRAM_SIZE(x)         ((x) << 16)
#define ESC_PRAM_ADDR(x)         ((x) << 0)

#define ESC_CSR_DATA_REG         0x0300
#define ESC_CSR_CMD_REG          0x0304

#define ESC_CSR_CMD_BUSY         0x80000000
#define ESC_CSR_CMD_READ         (0x80000000 | 0x40000000)
#define ESC_CSR_CMD_WRITE        0x80000000
#define ESC_CSR_CMD_SIZE(x)      ((x) << 16)

/* bcm2835 spi single write */
static void bcm2835_spi_write_32 (uint16_t address, uint32_t val)
{
   char data[7];

   data[0] = ESC_CMD_SERIAL_WRITE;
   data[1] = ((address >> 8) & 0xFF);
   data[2] = (address & 0xFF);
   data[3] = (val & 0xFF);
   data[4] = ((val >> 8) & 0xFF);
   data[5] = ((val >> 16) & 0xFF);
   data[6] = ((val >> 24) & 0xFF);

   /* Write data */
   bcm2835_spi_transfern(data, 7);
}

/* bcm2835 spi single read */
static uint32_t bcm2835_spi_read_32 (uint16_t address)
{
   char data[7];

   data[0] = ESC_CMD_SERIAL_READ;
   data[1] = ((address >> 8) & 0xFF);
   data[2] = (address & 0xFF);

   /* Read data */
   bcm2835_spi_transfern(data, 7);

   return ((data[6] << 24) |
           (data[5] << 16) |
           (data[4] << 8) |
           data[3]);
}

/* ESC read CSR function */
static void ESC_read_csr (uint16_t address, void *buf, uint16_t len)
{
   uint32_t value;

   value = (ESC_CSR_CMD_READ | ESC_CSR_CMD_SIZE(len) | address);
   bcm2835_spi_write_32(ESC_CSR_CMD_REG, value);

   do
   {
      value = bcm2835_spi_read_32(ESC_CSR_CMD_REG);
   } while(value & ESC_CSR_CMD_BUSY);

   value = bcm2835_spi_read_32(ESC_CSR_DATA_REG);
   memcpy(buf, (uint8_t *)&value, len);
}

/* ESC write CSR function */
static void ESC_write_csr (uint16_t address, void *buf, uint16_t len)
{
   uint32_t value;

   memcpy((uint8_t*)&value, buf,len);
   bcm2835_spi_write_32(ESC_CSR_DATA_REG, value);
   value = (ESC_CSR_CMD_WRITE | ESC_CSR_CMD_SIZE(len) | address);
   bcm2835_spi_write_32(ESC_CSR_CMD_REG, value);

   do
   {
      value = bcm2835_spi_read_32(ESC_CSR_CMD_REG);
   } while(value & ESC_CSR_CMD_BUSY);
}

/* ESC read process data ram function */
static void ESC_read_pram (uint16_t address, void *buf, uint16_t len)
{
   uint32_t value;
   uint8_t * temp_buf = buf;
   uint16_t quotient, remainder, byte_offset = 0;
   uint8_t fifo_cnt, fifo_size, fifo_range, first_byte_position, temp_len;
   uint8_t *buffer = NULL;
   int i, size;

   bcm2835_spi_write_32(ESC_PRAM_RD_CMD_REG, ESC_PRAM_CMD_ABORT);
   do
   {
      value = bcm2835_spi_read_32(ESC_PRAM_RD_CMD_REG);
   }while(value & ESC_PRAM_CMD_BUSY);

   bcm2835_spi_write_32(ESC_PRAM_RD_ADDR_LEN_REG, (ESC_PRAM_SIZE(len) | ESC_PRAM_ADDR(address)));

   bcm2835_spi_write_32(ESC_PRAM_RD_CMD_REG, ESC_PRAM_CMD_BUSY);

   /* Find out first byte position and adjust the copy from that
    * according to LAN9252 datasheet and MicroChip SDK code
    */
   first_byte_position = (address & 0x03);

   /* Transfer data */
   while (len > 0)
   {
      /* Wait for read availabiliy */
      if (byte_offset > 0)
      {
         quotient = len/4;
         remainder = len - quotient*4;
      }
      else
      {
         quotient = (len + first_byte_position)/4;
         remainder = (len + first_byte_position) - quotient*4;
      }
      if (remainder != 0)
      {
         quotient++;
      }
      fifo_range = MIN(quotient,16);
      do
      {
         value = bcm2835_spi_read_32(ESC_PRAM_RD_CMD_REG);
      }while(!(value & ESC_PRAM_CMD_AVAIL) || (ESC_PRAM_CMD_CNT(value) < fifo_range));

      /* Fifo size */
      fifo_size = ESC_PRAM_CMD_CNT(value);

      /* Transfer data size */
      size = 3+4*fifo_size;

      /* Allocate buffer */
      buffer = (uint8_t *)realloc(buffer, size);

      /* Reset fifo count */
      fifo_cnt = fifo_size;

      /* Reset buffer */
      memset(buffer,0,size);
      buffer[0] = ESC_CMD_SERIAL_READ;
      buffer[1] = ((ESC_PRAM_RD_FIFO_REG >>8) & 0xFF);
      buffer[2] = ( ESC_PRAM_RD_FIFO_REG & 0xFF);

      /* Transfer batch of data */
      bcm2835_spi_transfern((char *)buffer, size);

      i = 3;
      while (fifo_cnt > 0 && len > 0)
      {
         value = buffer[i] | (buffer[i+1] << 8) | (buffer[i+2] << 16) | (buffer[i+3] << 24);

         if (byte_offset > 0)
         {
            temp_len = (len > 4) ? 4: len;
            memcpy(temp_buf + byte_offset ,&value, temp_len);
         }
         else
         {
            temp_len = (len > (4 - first_byte_position)) ? (4 - first_byte_position) : len;
            memcpy(temp_buf ,((uint8_t *)&value + first_byte_position), temp_len);
         }

         i += 4;
         fifo_cnt--;
         len -= temp_len;
         byte_offset += temp_len;
      }
   }
   free(buffer);
}

/* ESC write process data ram function */
static void ESC_write_pram (uint16_t address, void *buf, uint16_t len)
{
   uint32_t value;
   uint8_t * temp_buf = buf;
   uint16_t quotient, remainder, byte_offset = 0;
   uint8_t fifo_cnt, fifo_size, fifo_range, first_byte_position, temp_len;
   uint8_t *buffer = NULL;
   int i, size;

   bcm2835_spi_write_32(ESC_PRAM_WR_CMD_REG, ESC_PRAM_CMD_ABORT);
   do
   {
      value = bcm2835_spi_read_32(ESC_PRAM_WR_CMD_REG);
   }while(value & ESC_PRAM_CMD_BUSY);

   bcm2835_spi_write_32(ESC_PRAM_WR_ADDR_LEN_REG, (ESC_PRAM_SIZE(len) | ESC_PRAM_ADDR(address)));

   bcm2835_spi_write_32(ESC_PRAM_WR_CMD_REG, ESC_PRAM_CMD_BUSY);

   /* Find out first byte position and adjust the copy from that
    * according to LAN9252 datasheet and MicroChip SDK code
    */
   first_byte_position = (address & 0x03);

   /* Transfer data */
   while (len > 0)
   {
      /* Wait for write availabiliy */
      if (byte_offset > 0)
      {
         quotient = len/4;
         remainder = len - quotient*4;
      }
      else
      {
         quotient = (len + first_byte_position)/4;
         remainder = (len + first_byte_position) - quotient*4;
      }
      if (remainder != 0)
      {
         quotient++;
      }
      fifo_range = MIN(quotient,16);
      do
      {
         value = bcm2835_spi_read_32(ESC_PRAM_WR_CMD_REG);
      }while(!(value & ESC_PRAM_CMD_AVAIL) || (ESC_PRAM_CMD_CNT(value) < fifo_range));

      /* Fifo size */
      fifo_size = ESC_PRAM_CMD_CNT(value);

      /* Transfer data size */
      size = 3+4*fifo_size;

      /* Allocate buffer */
      buffer = (uint8_t *)realloc(buffer, size);

      /* Reset fifo count */
      fifo_cnt = fifo_size;

      /* Reset buffer */
      memset(buffer,0,size);
      buffer[0] = ESC_CMD_SERIAL_WRITE;
      buffer[1] = ((ESC_PRAM_WR_FIFO_REG >> 8) & 0xFF);
      buffer[2] = (ESC_PRAM_WR_FIFO_REG & 0xFF);

      i = 3;
      while (fifo_cnt > 0 && len > 0)
      {
         value = 0;
         if (byte_offset > 0)
         {
            temp_len = (len > 4) ? 4: len;
            memcpy(&value, (temp_buf + byte_offset), temp_len);
         }
         else
         {
            temp_len = (len > (4 - first_byte_position)) ? (4 - first_byte_position) : len;
            memcpy(((uint8_t *)&value + first_byte_position), temp_buf, temp_len);
         }

         buffer[i] = (value & 0xFF);
         buffer[i+1] = ((value >> 8) & 0xFF);
         buffer[i+2] = ((value >> 16) & 0xFF);
         buffer[i+3] = ((value >> 24) & 0xFF);

         i += 4;
         fifo_cnt--;
         len -= temp_len;
         byte_offset += temp_len;
      }

      /* Transfer batch of data */
      bcm2835_spi_transfern((char *)buffer, size);
   }
   free(buffer);
}

/** ESC read function used by the Slave stack.
 *
 * @param[in]   address     = address of ESC register to read
 * @param[out]  buf         = pointer to buffer to read in
 * @param[in]   len         = number of bytes to read
 */
void ESC_read (uint16_t address, void *buf, uint16_t len)
{
   /* Select Read function depending on address, process data ram or not */
   if (address >= 0x1000)
   {
      ESC_read_pram(address, buf, len);
   }
   else
   {
      uint16_t size;
      uint8_t *temp_buf = (uint8_t *)buf;

      while(len > 0)
      {
         /* We write maximum 4 bytes at the time */
         size = (len > 4) ? 4 : len;
         /* Make size aligned to address according to LAN9252 datasheet
          * Table 12-14 EtherCAT CSR Address VS size and MicroChip SDK code
          */
         /* If we got an odd address size is 1 , 01b 11b is captured */
         if(address & BIT(0))
         {
            size = 1;
         }
         /* If address 1xb and size != 1 and 3 , allow size 2 else size 1 */
         else if (address & BIT(1))
         {
            size = (size & BIT(0)) ? 1 : 2;
         }
         /* size 3 not valid */
         else if (size == 3)
         {
            size = 1;
         }
         /* else size is kept AS IS */
         ESC_read_csr(address, temp_buf, size);

         /* next address */
         len -= size;
         temp_buf += size;
         address += size;
      }
   }
   /* To mimic the ET1100 always providing AlEvent on every read or write */
   ESC_read_csr(ESCREG_ALEVENT,(void *)&ESCvar.ALevent,sizeof(ESCvar.ALevent));
   ESCvar.ALevent = etohs (ESCvar.ALevent);

}

/** ESC write function used by the Slave stack.
 *
 * @param[in]   address     = address of ESC register to write
 * @param[out]  buf         = pointer to buffer to write from
 * @param[in]   len         = number of bytes to write
 */
void ESC_write (uint16_t address, void *buf, uint16_t len)
{
   /* Select Write function depending on address, process data ram or not */
   if (address >= 0x1000)
   {
      ESC_write_pram(address, buf, len);
   }
   else
   {
      uint16_t size;
      uint8_t *temp_buf = (uint8_t *)buf;

      while(len > 0)
      {
         /* We write maximum 4 bytes at the time */
         size = (len > 4) ? 4 : len;
         /* Make size aligned to address according to LAN9252 datasheet
          * Table 12-14 EtherCAT CSR Address VS size  and MicroChip SDK code
          */
         /* If we got an odd address size is 1 , 01b 11b is captured */
         if(address & BIT(0))
         {
            size = 1;
         }
         /* If address 1xb and size != 1 and 3 , allow size 2 else size 1 */
         else if (address & BIT(1))
         {
            size = (size & BIT(0)) ? 1 : 2;
         }
         /* size 3 not valid */
         else if (size == 3)
         {
            size = 1;
         }
         /* else size is kept AS IS */
         ESC_write_csr(address, temp_buf, size);

         /* next address */
         len -= size;
         temp_buf += size;
         address += size;
      }
   }

   /* To mimic the ET1x00 always providing AlEvent on every read or write */
   ESC_read_csr(ESCREG_ALEVENT,(void *)&ESCvar.ALevent,sizeof(ESCvar.ALevent));
   ESCvar.ALevent = etohs (ESCvar.ALevent);
}

/* Un-used due to evb-lan9252-digio not havning any possability to
 * reset except over SPI.
 */
void ESC_reset (void)
{

}



void ESC_init (const esc_cfg_t * config)
{
   bool rpi4 = true, cs1 = false;
   uint32_t value;
   uint32_t counter = 0;
   uint32_t timeout = 1000; // wait 100msec
   const char * user_arg = (char *)config->user_arg;
   size_t arg_len = strlen(user_arg)+1;
   char * arg_str = (char *)calloc(arg_len, sizeof(char));
   strncpy(arg_str,user_arg,arg_len);
   char * delim = " ,.-";
   char * token = strtok(arg_str,delim);

   // parse user arguments
   while (token != NULL)
   {
      if (strncmp(token,"cs1",3) == 0)
      {
         cs1 = true; // select CS1 pin
      }
      else if (strncmp(token,"rpi4",4) == 0)
      {
         rpi4 = true; // select clock divider for raspberry pi 4 or newer
      }
      token = strtok(NULL,delim);
   }
   free(arg_str);

   // start initialization
   if (bcm2835_init())
   {
      if (bcm2835_spi_begin())
      {
         // Set SPI bit order
         bcm2835_spi_setBitOrder(BCM2835_SPI_BIT_ORDER_MSBFIRST);
         // Set SPI data mode BCM2835_SPI_MODE0 = 0, CPOL = 0, CPHA = 0,
         // Clock idle low, data is clocked in on rising edge, output data (change) on falling edge
         bcm2835_spi_setDataMode(BCM2835_SPI_MODE0);
         if (rpi4)
         {
            // Raspberry 4 due to a higher CPU speed this value is to change to: BCM2835_SPI_CLOCK_DIVIDER_32
            bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_32);
             UM_INFO(GBLAN9252_SOES_UM_EN, "GBLAN9252_SOES: bcm2835_spi_setClockDivider set to 32");
         }
         else
         {
            // Set SPI clock speed BCM2835_SPI_CLOCK_DIVIDER_16 = 16, 16 = 64ns = 15.625MHz
            bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_16);
             UM_INFO(GBLAN9252_SOES_UM_EN, "GBLAN9252_SOES: bcm2835_spi_setClockDivider set to 16");
         }
         if (cs1)
         {
            // Enable management of CS1 pin
            bcm2835_spi_chipSelect(BCM2835_SPI_CS1);
            // Enable CS1 and set polarity
            bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS1, LOW);
             UM_INFO(GBLAN9252_SOES_UM_EN, "GBLAN9252_SOES: bcm2835_spi_chipSelect set to CS1");
         }
         else
         {
            // Enable management of CS0 pin
            bcm2835_spi_chipSelect(BCM2835_SPI_CS0);
            // enable CS0 and set polarity
            bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, LOW);
             UM_INFO(GBLAN9252_SOES_UM_EN, "GBLAN9252_SOES: bcm2835_spi_chipSelect set to CS0");

         }

         // Reset the ecat core here due to evb-lan9252-digio not having any GPIO for that purpose.
         bcm2835_spi_write_32(ESC_CMD_RESET_CTL,ESC_RESET_CTRL_RST);

         // Wait until reset command has been executed
         do
         {
            usleep(100);
            counter++;
            value = bcm2835_spi_read_32(ESC_CMD_RESET_CTL);
         } while ((value & ESC_RESET_CTRL_RST) && (counter < timeout));

         // Perform byte test
         do
         {
            usleep(100);
            counter++;
            value = bcm2835_spi_read_32(ESC_CMD_BYTE_TEST);
         } while ((value != ESC_BYTE_TEST_OK) && (counter < timeout));

         // Check hardware is ready
         do
         {
            usleep(100);
            counter++;
            value = bcm2835_spi_read_32(ESC_CMD_HW_CFG);
         } while (!(value & ESC_HW_CFG_READY) && (counter < timeout));

         // Check if timeout occured - cumlative
         if (counter < timeout)
         {
            // Read the chip identification and revision
            value = bcm2835_spi_read_32(ESC_CMD_ID_REV);
             UM_INFO(GBLAN9252_SOES_UM_EN, "GBLAN9252_SOES: Detected chip [0x%x] Rev [%u]", ((value >> 16) & 0xFFFF), (value & 0xFFFF));

            // Set AL event mask
            value = (ESCREG_ALEVENT_CONTROL |
                     ESCREG_ALEVENT_SMCHANGE |
                     ESCREG_ALEVENT_SM0 |
                     ESCREG_ALEVENT_SM1);
            ESC_ALeventmaskwrite(value);
//             value = (ESCREG_ALEVENT_CONTROL |
//                      ESCREG_ALEVENT_SMCHANGE |
//                     ESCREG_ALEVENT_SM0 |
//                     ESCREG_ALEVENT_SM1 |
//                      ESCREG_ALEVENT_SM2 | ESCREG_ALEVENT_SM3 );
//             ESC_ALeventmaskwrite(value);

            if ((ESCvar.use_interrupt != 0) &&
               (ESCvar.esc_hw_interrupt_enable != NULL))
            {
                LL_DEBUG(GBLAN9252_SOES_GEN_LOG_EN, "GBLAN9252_SOES: Hardware interrupts are enabled");
               ESCvar.esc_hw_interrupt_enable (ESCREG_ALEVENT_DC_SYNC0 | ESCREG_ALEVENT_SM2 );
            }
         }
         else
         {
             UM_ERROR(GBLAN9252_SOES_UM_EN, "GBLAN9252_SOES: Timeout during chip reset");
            bcm2835_spi_end();
            bcm2835_close();
         }
      }
      else
      {
          UM_ERROR(GBLAN9252_SOES_UM_EN, "GBLAN9252_SOES: bcm2835_spi_begin failed. Are you running as root?");
         bcm2835_close();
      }
   }
   else
   {
       UM_ERROR(GBLAN9252_SOES_UM_EN, "GBLAN9252_SOES: bcm2835_init failed. Are you running as root?");
   }
}

void ESC_interrupt_enable (uint32_t mask)
{
   if (ESCREG_ALEVENT_DC_SYNC0 & mask)
   {
      // Enable interrupt from SYNC0
      ESC_ALeventmaskwrite(ESC_ALeventmaskread() | ESCREG_ALEVENT_DC_SYNC0);
   }
   if (ESCREG_ALEVENT_SM2 & mask)
   {
      // Enable interrupt from SYNC0
      ESC_ALeventmaskwrite(ESC_ALeventmaskread() | ESCREG_ALEVENT_SM2);
   }


   // Set LAN9252 interrupt pin driver as push-pull active high
   bcm2835_spi_write_32(ESC_CMD_IRQ_CFG, 0x00000111);

   // Enable LAN9252 interrupt
   bcm2835_spi_write_32(ESC_CMD_INT_EN, 0x00000001);

}

void ESC_interrupt_disable (uint32_t mask)
{
   if (ESCREG_ALEVENT_DC_SYNC0 & mask)
   {
      // Disable interrupt from SYNC0
      ESC_ALeventmaskwrite(ESC_ALeventmaskread() & ~(ESCREG_ALEVENT_DC_SYNC0));
   }
   if (ESCREG_ALEVENT_SM2 & mask)
   {
      // Disable interrupt from SM2
      ESC_ALeventmaskwrite(ESC_ALeventmaskread() & ~(ESCREG_ALEVENT_SM2));
   }

   // Disable LAN9252 interrupt
   bcm2835_spi_write_32(ESC_CMD_INT_EN, 0x00000000);
}


/* Setup of DC */
uint16_t dc_checker (void)
{

    ESCvar.synccounterlimit = Obj.ErrorSettings.SyncErrorCounterLimit;
    uint16_t ret = 0;
    uint32_t sync0_cycletime = ESC_SYNC0cycletime();
    uint16_t sync_type_supported1c32 = 0;
    uint32_t mincycletime = 0;
    int8_t sync_act = ESC_SYNCactivation();


    if(COE_getSyncMgrPara(0x1c32, 0x4, &sync_type_supported1c32, DTYPE_UNSIGNED16) == 0)
    {
        ret = ALERR_DCINVALIDSYNCCFG;
        DPRINT("sync type supported==0");
    }
    else if(COE_getSyncMgrPara(0x1c32, 0x5, &mincycletime, DTYPE_UNSIGNED32) == 0)
    {
        ret = ALERR_DCINVALIDSYNCCFG;
        DPRINT("min cycle time==0");
    }
    else if(COE_getSyncMgrPara(0x10F1, 0x2, &ESCvar.synccounterlimit, DTYPE_UNSIGNED16) == 0)
    {
        ret = ALERR_DCINVALIDSYNCCFG;
        DPRINT("sync counter limit==0");
    }
    else if((sync_act & (ESCREG_SYNC_SYNC0_EN | ESCREG_SYNC_SYNC1_EN)) == 0)
    {
        ret = ALERR_DCINVALIDSYNCCFG;
        DPRINT("no sync 0 or sync 1");
    }
        /* Do we support activated signals */
    else if(((sync_type_supported1c32 & SYNCTYPE_SUPPORT_DCSYNC0) == 0) &&
            ((sync_act & ESCREG_SYNC_SYNC0_EN) > 0))
    {
        ret = ALERR_DCINVALIDSYNCCFG;
    }
        /* Do we support activated signals */
    else if(((sync_type_supported1c32 & SYNCTYPE_SUPPORT_DCSYNC1) == 0) &&
            ((sync_act & ESCREG_SYNC_SYNC1_EN) > 0))
    {
        ret = ALERR_DCINVALIDSYNCCFG;
    }
    else if((sync0_cycletime != 0) && (sync0_cycletime < mincycletime))
    {
        ret = ALERR_DCSYNC0CYCLETIME;
    }



    /* Indicate we run DC */
    ESCvar.dcsync = 1;
    /* Fetch the sync counter limit (SDO10F1) */
//    ESCvar.synccounterlimit = Obj.ErrorSettings.SyncErrorCounterLimit;
    return 0;
}



/** Get the value for requested SDO 0x1C32 or 0x1C33 sub index
 *
 * @param[in] index    = value on index of object we want to locate
 * @param[in] subindex = value on subindex of object we want to locate
 * @param[out] buf     = buf to copy value to
 * @param[in] datatype = EtherCAT datatype of buf
 * @return 1 if value was found, else 0.
 */
int COE_getSyncMgrPara (uint16_t index, uint8_t subindex, void * buf, uint16_t datatype)
{
    int result = 0;
    int32_t nidx;
    int32_t snidx;
    const _objd *objd;

    nidx = SDO_findobject(index);

    if(nidx < 0)
    {
        return result;
    }
    else if((index != 0x1c32) && (index != 0x1c33) && (index != 0x10F1))
    {
        return result;
    }

    snidx = SDO_findsubindex(nidx, subindex);

    if(snidx >= 0)
    {
        objd = SDOobjects[nidx].objdesc;

        if((objd[snidx].data != NULL) &&
           (objd[snidx].datatype == datatype))
        {
            memcpy(buf, objd[snidx].data, objd[snidx].bitlength / 8 );
            result = 1;
        }
        else
        {
            if((datatype == DTYPE_UNSIGNED32) &&
               (objd[snidx].datatype == datatype))
            {
                *(uint32_t *)buf = objd[snidx].value;
                result = 1;
            }
            else if((datatype == DTYPE_UNSIGNED16) &&
                    (objd[snidx].datatype == datatype))
            {
                *(uint16_t *)buf = (uint16_t)objd[snidx].value;
                result = 1;

            }
            else if((datatype == DTYPE_UNSIGNED8) &&
                    (objd[snidx].datatype == datatype))
            {
                *(uint8_t *)buf = (uint8_t)objd[snidx].value;
                result = 1;
            }
        }
    }

    return result;
}


/** EEPROM read function
 *
 * @param[in]   addr     = EEPROM byte address
 * @param[out]  data     = pointer to buffer of output data
 * @param[in]   count    = number of bytes to read
 * @return 0 on OK, 1 on error
 */
int8_t EEP_read (uint32_t addr, uint8_t *data, uint16_t count)
{

    return 0;
}

/** EEPROM write function
 *
 * @param[in]   addr     = EEPROM byte address
 * @param[out]  data     = pointer to buffer of input data
 * @param[in]   count    = number of bytes to write
 * @return 0 on OK, 1 on error
 */
int8_t EEP_write (uint32_t addr, uint8_t *data, uint16_t count)
{

    return 0;
}