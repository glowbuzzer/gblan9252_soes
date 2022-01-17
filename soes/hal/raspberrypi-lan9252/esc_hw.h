/*
 * Licensed under the GNU General Public License version 2 with exceptions. See
 * LICENSE file in the project root for full license information
 */

 /** \file
 * \brief
 * ESC hardware specifoc EEPROM emulation functions.
 */

#ifndef __esc_hw__
#define __esc_hw__

void ESC_interrupt_enable (uint32_t mask);
void ESC_interrupt_disable (uint32_t mask);

uint16_t dc_checker (void);
int COE_getSyncMgrPara (uint16_t index, uint8_t subindex, void * buf, uint16_t datatype);
#endif
