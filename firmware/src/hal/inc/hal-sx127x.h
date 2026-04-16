/*
* Copyright (c) 2018, 2026 Vladimir Alemasov
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

#ifndef HAL_SX127X_H_
#define HAL_SX127X_H_

//--------------------------------------------
void hal_sx127x_init(void);
void hal_sx127x_select(void);
void hal_sx127x_release(void);
uint8_t hal_sx127x_txrx(uint8_t data);
void hal_sx127x_ctrl_clock(uint8_t val);
void hal_sx127x_radio_rx(void);
void hal_sx127x_radio_tx(void);

#endif // HAL_SX127X_H_
