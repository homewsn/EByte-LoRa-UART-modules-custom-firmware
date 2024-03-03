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

#include "platform.h"
#include "lora-defs.h"
#include "lora-drv.h"
#include "sx1278.h"
#include "lora-module-name.h"

//--------------------------------------------
const lora_drv_t lora_drv =
{
	get_module_name,
	sx1278_get_core_name,
	sx1278_init,
	sx1278_set_params,
	sx1278_start_continious_rx,
	sx1278_irq_poll,
	sx1278_read_rx_packet,
	sx1278_send_tx_packet,
	sx1278_read_rssi
};
