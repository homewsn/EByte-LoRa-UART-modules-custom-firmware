/*
* Copyright (c) 2018, 2022 Vladimir Alemasov
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

#ifndef PLATFORM_H_
#define PLATFORM_H_

#include "cpu.h"

//--------------------------------------------
void platform_init(void);
void delay_ms(uint32_t delay_ms);
void delay_us(uint32_t delay_us);
uint32_t get_platform_counter(void);
void system_reset(void);

#endif // PLATFORM_H_
