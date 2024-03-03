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

//--------------------------------------------
// STM8L platform.c file

#include "cpu.h"

static volatile uint32_t counter;

//--------------------------------------------
// TIM4 interruption handler
INTERRUPT_HANDLER(TIM4_ISR_Handler, TIM4_ISR)
{
	counter++;
	TIM4_SR &= ~(1 << TIM4_SR_UIF);
}

//--------------------------------------------
void delay_ms(uint32_t delay_ms)
{
	delay_ms += counter;
	while (delay_ms != counter);
}

//--------------------------------------------
uint32_t get_platform_counter(void)
{
	return counter;
}

//--------------------------------------------
void delay_us(uint32_t delay_us)
{
	// ??
	delay_us *= 3;
	do
	{
		delay_us--;
	}
	while (delay_us);
}

//--------------------------------------------
void system_reset(void)
{
	WWDG_CR = 0x80;
	while (1);
}

//--------------------------------------------
// fHSI    = 16 MHz
// fCPU    = 16 MHz
// fSYSCLK = 16 MHz
static void SetSysClock(void)
{
	// fCPU    = fHSI
	// fSYSCLK = fHSI
	CLK_CKDIVR = 0x00;
}

//--------------------------------------------
// fCK_PSC = fSYSCLK
// fCK_CNT = fCK_PSC / 2^(PSCR[3:0])
static void TIM4Init(void)
{
	// TIM4 peripheral clock enable
	CLK_PCKENR1 |= (1 << CLK_PCKENR1_TIM4);
	// fCK_CNT = 16MHz / 2^7 = 125kHz
	TIM4_PSCR = 0x07;
	// Time base equal to 1 ms
	// ARR = 0.001s * 125000Hz - 1 = 124
	TIM4_ARR = 124;
	// enable update interrupt
	TIM4_IER |= (1 << TIM4_IER_UIE);
	// enable TIM4
	TIM4_CR1 |= (1 << TIM4_CR1_CEN);
}

//--------------------------------------------
void platform_init(void)
{
	SetSysClock();
	TIM4Init();
	// enable interrupts
	enableInterrupts();
}
