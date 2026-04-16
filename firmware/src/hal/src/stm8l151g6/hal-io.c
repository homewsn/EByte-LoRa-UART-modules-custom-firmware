/*
* Copyright (c) 2020, 2026 Vladimir Alemasov
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
#include "stm8l-hw.h"

#if defined E32_433T20D_32100_V3_0 || defined E32_433T20D_10012_V6_0 || defined E32_433T30D_10010_V6_2 || defined E45_TTL_1W_10024_V2_0
//--------------------------------------------
#define	PORT_M0         GPIO_C   // PC5 --> M0
#define	PIN_M0          5
#define	PORT_M1         GPIO_C   // PC5 --> M1
#define	PIN_M1          6
#define	PORT_AUX        GPIO_D   // PD0 --> AUX
#define	PIN_AUX         0

#elif
#error "Unknown module"
#endif

//--------------------------------------------
void hal_io_init(void)
{
	// Pins
	hw_cfg_pin(PORT_M0,  PIN_M0,  GPIO_DDR_OUT | GPIO_CR1_OUT_PUSHPULL | GPIO_CR2_OUT_2MHZ);
	hw_cfg_pin(PORT_M1,  PIN_M1,  GPIO_DDR_OUT | GPIO_CR1_OUT_PUSHPULL | GPIO_CR2_OUT_2MHZ);
	hw_cfg_pin(PORT_AUX, PIN_AUX, GPIO_DDR_OUT | GPIO_CR1_OUT_PUSHPULL | GPIO_CR2_OUT_2MHZ);
}

//--------------------------------------------
void hal_io_m0(uint8_t val)
{
	hw_set_pin(PORT_M0, PIN_M0, val); // M0 = val
}

//--------------------------------------------
void hal_io_m1(uint8_t val)
{
	hw_set_pin(PORT_M1, PIN_M1, val); // M1 = val
}

//--------------------------------------------
void hal_io_aux(uint8_t val)
{
	hw_set_pin(PORT_AUX, PIN_AUX, val); // AUX = val
}
