#include <stdio.h>
#include "ecat_slv.h"
#include "utypes.h"
#include <bcm2835.h>
#include "esc_hw.h"

/* Application variables */
_Objects    Obj;
_Mbuffer Mb;


void cb_application (void)
{
    /* Check for watchdog expired event */
    if (ESCvar.ALevent & ESCREG_ALEVENT_WD)
    {
        if ((ESC_WDstatus() & 0x01) &&
            ((CC_ATOMIC_GET(ESCvar.App.state) & APPSTATE_OUTPUT) > 0))
        {
            DPRINT("DIG_process watchdog expired\n");
            ESC_ALstatusgotoerror((ESCsafeop | ESCerror), ALERR_WATCHDOG);
        }
    }
}

void cb_state_change (uint8_t * as, uint8_t * an)
{
    if (*as == SAFEOP_TO_OP)
    {
        /* Enable HW watchdog event */
        ESC_ALeventmaskwrite (ESC_ALeventmaskread() | ESCREG_ALEVENT_WD);
        /* Enable SM2 sync manager event */
        ESC_ALeventmaskwrite (ESC_ALeventmaskread() | ESCREG_ALEVENT_SM2);
    }
}

void cb_get_inputs (void)
{

Obj.in.machine_control_word=8;
Obj.in.digital_5=1;

//    Obj.Buttons.Button0=0;
//    Obj.Buttons.Button1=1;
//    Obj.Buttons.Button2=1;
//    Obj.Buttons.Button3=0;
//    Obj.Buttons.Button4=1;
//    Obj.Buttons.Button5=0;

}

void cb_set_outputs (void)
{

    printf("Status word: %u\n", Obj.out.machine_status_word);
    printf("Digital out 0:%u\n", Obj.out.digital_0);

//    printf("LED 0:%u\n", Obj.LEDs.LED0);
//    printf("LED 1:%u\n", Obj.LEDs.LED1);
//    printf("LED 2:%u\n", Obj.LEDs.LED2);
//    printf("LED 3:%u\n", Obj.LEDs.LED3);
//    printf("LED 4:%u\n", Obj.LEDs.LED4);
//    printf("LED 5:%u\n", Obj.LEDs.LED5);


   // Assume Buttons connected to 3.3v
//   Obj.Buttons.Button0 =1;
//    Obj.Buttons.Button1 =0;
//    Obj.Buttons.Button2 =1;
//    Obj.Buttons.Button3 =0;
//    Obj.Buttons.Button4 =1;
//    Obj.Buttons.Button5 =0;
////   Obj.Buttons.Button0 = bcm2835_gpio_lev(GPIO26);
//   Obj.Buttons.Button0 = bcm2835_gpio_lev(GPIO19);
//   Obj.Buttons.Button0 = bcm2835_gpio_lev(GPIO13);
//   Obj.Buttons.Button0 = bcm2835_gpio_lev(GPIO06);
//   Obj.Buttons.Button0 = bcm2835_gpio_lev(GPIO05);
//   Obj.Buttons.Button0 = bcm2835_gpio_lev(GPIO22);
}

void GPIO_init (void)
{
    int rc =0;
    printf("GPIO_init\n");

  rc= bcm2835_init();

  if (rc!=1){
      DPRINT("bcm2835_init failed!\n");
      exit(0);
  }

#if SOES_IRQ == 1
    DPRINT("Enable rising edge interrupt on sync0 pin\n");
    bcm2835_gpio_fsel(SYNC0_PIN, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_ren(SYNC0_PIN);
#endif

}


void ESC_safeoutput_override(void){
// Set safe values for digitals
    //    Obj.out.digital_0 = 0;
}
#define MAX_TIME_WAIT_SYNC0 4000

void ESC_wait_for_sync0(uint8_t pin)
{
uint32_t count = 0;
    bcm2835_gpio_set_eds(pin);//clear the previous event. makes sure that only next event is detected
    while((!bcm2835_gpio_eds(pin)) && count < MAX_TIME_WAIT_SYNC0){
        count++;
        delayMicroseconds(1);
    }//waits until next event/tick

    if (count == MAX_TIME_WAIT_SYNC0){
    printf("timeout waiting sync0: %u\n",count);
        }

}


_Noreturn int main_run (void * arg)
{
   static esc_cfg_t config =
   {
      .user_arg = "rpi4,cs0",
#if SOES_IRQ == 1
      .use_interrupt = 1,
#else
                   .use_interrupt = 0,
#endif
      .watchdog_cnt = 10000,
      .set_defaults_hook = NULL,
      .pre_state_change_hook = NULL,
      .post_state_change_hook = NULL,
      .application_hook = NULL,
//      .safeoutput_override = ESC_safeoutput_override,
      .safeoutput_override = NULL,
      .pre_object_download_hook = NULL,
      .post_object_download_hook = NULL,
      .rxpdo_override = NULL,
      .txpdo_override = NULL,
#if SOES_IRQ == 1
                   .esc_hw_interrupt_enable = ESC_interrupt_enable,
                   .esc_hw_interrupt_disable = ESC_interrupt_disable,
#else

                 .esc_hw_interrupt_enable = NULL,
           .esc_hw_interrupt_disable = NULL,
#endif
      .esc_hw_eep_handler = NULL,
#if SOES_IRQ == 1
      .esc_check_dc_handler = dc_checker,
#endif
   };

   Obj.ErrorSettings.SyncErrorCounterLimit = 3000;

   printf ("Hello Main raspberry_lan9252demo\n");
   GPIO_init();
   ecat_slv_init (&config);


//    ESC_startinput(0x8);
//
//    ESC_startoutput(0x8);
//    ESC_startmbx(0x8);

    while (1)
   {
#if SOES_IRQ == 1

        ESC_wait_for_sync0(SYNC0_PIN);
        printf("sync0!\n");
//        printf("SM2_sml:%u\n", ESCvar.ESC_SM2_sml);
//        printf("SM3_sml:%u\n", ESCvar.ESC_SM3_sml);


printf("synccounter limit %u\n",ESCvar.synccounterlimit );

        /* Subtract the sync counter to check the pace compared to the SM IRQ */
        if((CC_ATOMIC_GET(ESCvar.App.state) & APPSTATE_OUTPUT) > 0)
        {
            CC_ATOMIC_SUB(ESCvar.synccounter, 1);
        }
        /* Check so we're inside the limit */
        if((CC_ATOMIC_GET(ESCvar.synccounter) < -ESCvar.synccounterlimit) ||
           (CC_ATOMIC_GET(ESCvar.synccounter) > ESCvar.synccounterlimit))
        {
            if((CC_ATOMIC_GET(ESCvar.App.state) & APPSTATE_OUTPUT) > 0)
            {
                DPRINT("sync error = %d\n", ESCvar.synccounter);
                ESC_ALstatusgotoerror((ESCsafeop | ESCerror), ALERR_SYNCERROR);
                CC_ATOMIC_SET(ESCvar.synccounter, 0);
            }
        }


ecat_slv();
#else
      printf("SM2_sml:%u\n", ESCvar.ESC_SM2_sml);
      printf("SM3_sml:%u\n", ESCvar.ESC_SM3_sml);
      ecat_slv();
#endif
   }

}

int main (void)
{
   printf ("Hello Main\n");
   main_run (NULL);
   return 0;

}

///**
// * SOES main function. Called to handle watchdog
// */
//void soes_irq (void)
//{
//    DIG_process(DIG_PROCESS_WD);
//
//}
//
//void soes_isr (void)
//{
//
//    do
//    {
//        /* Check the state machine */
//        ESC_state();
//        /* Check the SM activation event */
//        ESC_sm_act_event();
//
//        /* Check mailboxes */
//        /* Check mailboxes */
//        while ((ESC_mbxprocess() > 0) || (ESCvar.txcue > 0))
//        {
//            ESC_coeprocess();
//            ESC_foeprocess();
//            ESC_xoeprocess();
//        }
//
//#if EEP_EMULATION
//        EEP_process ();
//      EEP_hw_process();
//#endif
//        ATOMIC_SET(ESCvar.ALevent,ESC_ALeventread());
//
//    }while(ESCvar.ALevent & (ESCREG_ALEVENT_CONTROL | ESCREG_ALEVENT_SMCHANGE
//                             | ESCREG_ALEVENT_SM0 | ESCREG_ALEVENT_SM1 | ESCREG_ALEVENT_EEP));
//
//    ESC_ALeventmaskwrite(ESC_ALeventmaskread()
//                         | (ESCREG_ALEVENT_CONTROL | ESCREG_ALEVENT_SMCHANGE
//                            | ESCREG_ALEVENT_SM0 | ESCREG_ALEVENT_SM1
//                            | ESCREG_ALEVENT_EEP));
//
//    /* On init restore PDO mappings to default size */
//    if((ESCvar.ALstatus & 0x0f) == ESCinit)
//    {
//        txpdomap = DEFAULTTXPDOMAP;
//        rxpdomap = DEFAULTRXPDOMAP;
//        txpdoitems = DEFAULTTXPDOITEMS;
//        rxpdoitems = DEFAULTTXPDOITEMS;
//        initDefaultOutputSyncMgrPara();
//    }
//}
//
//
//void sync0_isr(void * arg)
//{
//    DIG_process(DIG_PROCESS_APP_HOOK | DIG_PROCESS_INPUTS);
//    read_ack = ecat0->DC_SYNC0_STAT;
//}
//
//void ecat_isr(void * arg)
//{
//
//    ESC_read (ESCREG_LOCALTIME, (void *) &ESCvar.Time, sizeof (ESCvar.Time));
//    ESCvar.Time = etohl (ESCvar.Time);
//    ATOMIC_SET(ESCvar.ALevent, etohl(ecat0->AL_EVENT_REQ));
//
//    if(ESCvar.ALevent & ESCREG_ALEVENT_SM2)
//    {
//
//        if(dc_sync == 0)
//        {
//            DIG_process(DIG_PROCESS_OUTPUTS | DIG_PROCESS_APP_HOOK | DIG_PROCESS_INPUTS);
//        }
//        else
//        {
//            DIG_process(DIG_PROCESS_OUTPUTS);
//        }
//    }
//
//    if(ESCvar.ALevent & (ESCREG_ALEVENT_CONTROL | ESCREG_ALEVENT_SMCHANGE
//                         | ESCREG_ALEVENT_SM0 | ESCREG_ALEVENT_SM1 | ESCREG_ALEVENT_EEP))
//    {
//        /* Mask interrupts while servicing them */
//        ecat0->AL_EVENT_MASK &= ~(ESCREG_ALEVENT_CONTROL | ESCREG_ALEVENT_SMCHANGE
//                                  | ESCREG_ALEVENT_SM0 | ESCREG_ALEVENT_SM1 | ESCREG_ALEVENT_EEP);
//        sem_signal(ecat_isr_sem);
//    }
//
//}
//#endif