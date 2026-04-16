/*
* Copyright (c) 2018, 2020, 2026 Vladimir Alemasov
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

#include <stddef.h>     /* size_t */
#include "platform.h"
#include "stm8l-hw.h"

#if defined E32_433T20D_32100_V3_0 || defined E32_433T20D_10012_V6_0 || defined E32_433T30D_10010_V6_2 || defined E45_TTL_1W_10024_V2_0
//--------------------------------------------
#define	PORT_TX         GPIO_A   // PA2 --> TX
#define	PIN_TX          2
#define	PORT_RX         GPIO_A   // PA3 <-- RX
#define	PIN_RX          3

#elif
#error "Unknown module"
#endif

//--------------------------------------------
static void (*irq_callback)(uint8_t byte);

//--------------------------------------------
void hal_uart_init(void func(uint8_t byte))
{
	irq_callback = func;

	// Pins
	hw_cfg_pin(PORT_TX, PIN_TX, GPIO_DDR_OUT | GPIO_CR1_OUT_PUSHPULL | GPIO_CR2_OUT_2MHZ);
	hw_cfg_pin(PORT_RX, PIN_RX, GPIO_DDR_INP | GPIO_CR1_INP_PULLUP   | GPIO_CR2_INP_EXTINTDIS);

	// Remap USART1_TX on PA2 and USART1_RX on PA3
	SYSCFG_RMPCR1 |= (1 << SYSCFG_RMPCR1_USART1TR_REMAP0);

	// USART1 peripheral clock enable
	CLK_PCKENR1 |= (1 << CLK_PCKENR1_USART1);

	// UART_DIV = fMASTER/UART_baud_rate = 16000000 / 115200 = 138.88 ~= 139 = 0x8B
	// UART_DIV = 0x008B => BRR1 = 0x08, BRR2 = 0x0B
	// The BRR2 should be programmed before BRR1
	USART1_BRR2 = 0x0B;
	USART1_BRR1 = 0x08;

	// TEN = 1: Transmitter enable
	USART1_CR2 |= (1 << USART1_CR2_TEN);
	// RIEN = 1: Receiver interrupt enable
	// REN = 1: Receiver enable
	USART1_CR2 |= (1 << USART1_CR2_RIEN) | (1 << USART1_CR2_REN);
}

//--------------------------------------------
void hal_uart_tx(const uint8_t *buf, size_t len)
{
	while (len--)
	{
		while (!(USART1_SR & (1 << USART1_SR_TXE)));
		USART1_DR = *buf++;
	}
}

//--------------------------------------------
// USART1 receive (RX full) interruption handler
INTERRUPT_HANDLER(USART1_RXC_ISR_Handler, USART1_RXC_ISR)
{
	if (USART1_SR & (1 << USART1_SR_RXNE))
	{
		uint8_t byte;
		byte = USART1_DR;
		irq_callback(byte);
	}
}
