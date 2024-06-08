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

#include "asr6601.h"

//--------------------------------------------
// System Clock Frequency (Core Clock)
uint32_t SystemCoreClock = 24000000;

//--------------------------------------------
// Update SystemCoreClock variable
void SystemCoreClockUpdate(void)
{
	switch (ASR_RCC->CR0_b.SYSCLK_SEL)
	{
	case RCC_CR0_SYSCLK_SEL_RCO48M:
		SystemCoreClock = 48000000;
		break;
	case RCC_CR0_SYSCLK_SEL_RCO32K:
		SystemCoreClock = 32000;
		break;
	case RCC_CR0_SYSCLK_SEL_XO32K:
		SystemCoreClock = 32768;
		break;
	case RCC_CR0_SYSCLK_SEL_XO24M:
		SystemCoreClock = 24000000;
		break;
	case RCC_CR0_SYSCLK_SEL_XO32M:
		SystemCoreClock = 32000000;
		break;
	case RCC_CR0_SYSCLK_SEL_RCO4M:
		SystemCoreClock = 3600000;
		break;
	case RCC_CR0_SYSCLK_SEL_RCO48M_DIV2:
	default:
		SystemCoreClock = 24000000;
		break;
	}
}

//--------------------------------------------
void SystemInit(void)
{
#if (__FPU_PRESENT == 1) && (__FPU_USED == 1)
	// Enabling the FPU
	// Set bits 20-23 to enable CP10 and CP11 coprocessors
	SCB->CPACR |= ((3UL << 10 * 2) | (3UL << 11 * 2));
#endif

	ASR_EFC->TIMING_CFG_b.READ_NUM = 1;
	while (!ASR_EFC->SR_b.READ_NUM_DONE);
}
