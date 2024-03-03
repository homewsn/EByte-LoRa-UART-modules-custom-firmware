/*
* Copyright (c) 2022 Vladimir Alemasov
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

#ifndef LORA_DRV_H_
#define LORA_DRV_H_

//--------------------------------------------
typedef struct lora_drv
{
	const char *(*get_module_name)(void);
	const char *(*get_core_name)(void);
	lora_status_t (*init)(lora_modes_t *modes);
	lora_status_t (*set_params)(lora_params_t *params);
	void (*start_continious_rx)(void);
	lora_irq_src_t (*irq_poll)(void);
	lora_status_t (*read_rx_packet)(uint8_t *buf, uint8_t *len, int16_t *rssi, int8_t *snr);
	uint32_t (*send_tx_packet)(uint8_t *buf, uint8_t len);
	void (*read_rssi)(int16_t *rssi);
} lora_drv_t;

#endif // LORA_DRV_H_
