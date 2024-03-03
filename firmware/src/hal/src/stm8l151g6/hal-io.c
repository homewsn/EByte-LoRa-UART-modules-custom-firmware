/*
* Copyright (c) 2020 Vladimir Alemasov
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

//--------------------------------------------
#define	PIN_M0          5   // PC5 --> M0
#define	PIN_M1          6   // PC6 --> M1
#define	PIN_AUX         0   // PD0 --> AUX

//--------------------------------------------
void hal_io_init(void)
{
	PC_ODR &= ~(1 << PIN_M0);      // M0 = 0
	PC_DDR |= (1 << PIN_M0);
	PC_CR1 |= (1 << PIN_M0);
	PC_CR2 |= (1 << PIN_M0);

	PC_ODR &= ~(1 << PIN_M1);      // M1 = 0
	PC_DDR |= (1 << PIN_M1);
	PC_CR1 |= (1 << PIN_M1);
	PC_CR2 |= (1 << PIN_M1);

	PD_ODR &= ~(1 << PIN_AUX);     // AUX = 0
	PD_DDR |= (1 << PIN_AUX);
	PD_CR1 |= (1 << PIN_AUX);
	PD_CR2 |= (1 << PIN_AUX);
}

//--------------------------------------------
void hal_io_m0(uint8_t val)
{
	if (!val)
	{
		PC_ODR &= ~(1 << PIN_M0);  // M0 = 0
	}
	else
	{
		PC_ODR |= (1 << PIN_M0);   // M0 = 1
	}
}

//--------------------------------------------
void hal_io_m1(uint8_t val)
{
	if (!val)
	{
		PC_ODR &= ~(1 << PIN_M1);  // M1 = 0
	}
	else
	{
		PC_ODR |= (1 << PIN_M1);   // M1 = 1
	}
}

//--------------------------------------------
void hal_io_aux(uint8_t val)
{
	if (!val)
	{
		PD_ODR &= ~(1 << PIN_AUX); // AUX = 0
	}
	else
	{
		PD_ODR |= (1 << PIN_AUX);  // AUX = 1
	}
}
