/*
* Copyright (c) 2018, 2020 Vladimir Alemasov
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

//--------------------------------------------
#define	PIN_TX          2   // PA2 --> TX
#define	PIN_RX          3   // PA3 <-- RX

//--------------------------------------------
static void (*irq_callback)(uint8_t byte);

//--------------------------------------------
void hal_uart_init(void func(uint8_t byte))
{
	irq_callback = func;

	// Remap USART1_TX on PA2 and USART1_RX on PA3
	SYSCFG_RMPCR1 |= (1 << SYSCFG_RMPCR1_USART1TR_REMAP0);

	// USART1 peripheral clock enable
	CLK_PCKENR1 |= (1 << CLK_PCKENR1_USART1);

	PA_CR2 &= ~(1 << PIN_TX);
	PA_CR1 |= (1 << PIN_TX);
	PA_DDR |= (1 << PIN_TX);

	PA_CR2 &= ~(1 << PIN_RX);
	PA_CR1 |= (1 << PIN_RX);
	PA_DDR &= ~(1 << PIN_RX);

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
