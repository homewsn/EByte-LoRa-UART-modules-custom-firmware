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

//--------------------------------------------
// ASR6601 platform.c file

#include "cpu.h"

static volatile uint32_t counter;

//--------------------------------------------
// SysTick timer interruption handler
void SysTick_Handler(void)
{
	counter++;
}

//--------------------------------------------
void delay_ms(uint32_t delay_ms)
{
	uint32_t start;
	start = counter;
	while ((counter - start) < delay_ms);
}

//--------------------------------------------
uint32_t get_platform_counter(void)
{
	return counter;
}

//--------------------------------------------
void delay_us(uint32_t delay_us)
{
	uint32_t start;
	start = DWT->CYCCNT;
	delay_us *= (SystemCoreClock / 1000000);
	while ((DWT->CYCCNT - start) < delay_us);
}

//--------------------------------------------
void system_reset(void)
{
	NVIC_SystemReset();
}

//--------------------------------------------
// fRCH = 24 MHz (RCH)
// fSYSCLK = 24 MHz (SYSCLK)
// fHCLK = 24 MHz (AHB)
// fPCLK0 = 24 MHz (APB0)
// fPCLK1 = 24 MHz (APB1)
static void SetSysClock(void)
{
}

//--------------------------------------------
static void DWTInit(void)
{
	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
	DWT->CYCCNT = 0;
	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk; 
}

//--------------------------------------------
void platform_init(void)
{
#if 1
	// Number of group priorities: 16
	// Number of sub priorities: none
	// All the priority bits are the preempt priority bits only
	NVIC_SetPriorityGrouping(3);
#endif
	SetSysClock();
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 1000);
	// Changing the SysTick_IRQn priority level in the new group
	NVIC_SetPriority(SysTick_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
	DWTInit();
}
