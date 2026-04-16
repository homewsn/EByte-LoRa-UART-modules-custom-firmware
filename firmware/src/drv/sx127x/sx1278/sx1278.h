/*
* Copyright (c) 2018 Vladimir Alemasov
* All rights reserved
*
* This program and the accompanying materials are distributed under 
* the terms of GNU General Public License version 2 
* as published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*/

#ifndef SX1278_H_
#define SX1278_H_

//--------------------------------------------
const char* sx1278_get_core_name(void);
lora_status_t sx1278_init(lora_modes_t *modes);
lora_status_t sx1278_set_params(lora_params_t *params);
void sx1278_start_continious_rx(void);
lora_irq_src_t sx1278_irq_poll(void);
lora_status_t sx1278_read_rx_packet(uint8_t *buf, uint8_t *len, int16_t *rssi, int8_t *snr);
uint32_t sx1278_send_tx_packet(uint8_t *buf, uint8_t len);
void sx1278_read_rssi(int16_t *rssi);

#endif // SX1278_H_
