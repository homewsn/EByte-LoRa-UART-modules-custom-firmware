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

#ifndef HAL_SX1278_H_
#define HAL_SX1278_H_

void hal_sx1278_init(void);
void hal_sx1278_select(void);
void hal_sx1278_release(void);
uint8_t hal_sx1278_txrx(uint8_t data);
void hal_sx1278_ctrl_rx(uint8_t val);
void hal_sx1278_ctrl_tx(uint8_t val);
void hal_sx1278_ctrl_pa(uint8_t val);
void hal_sx1278_ctrl_lna(uint8_t val);
void hal_sx1278_ctrl_clock(uint8_t val);
void hal_sx1278_ctrl_vgs(uint8_t val);

#endif // HAL_SX1278_H_
