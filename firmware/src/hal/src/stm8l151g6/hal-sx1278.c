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

#include "platform.h"

//--------------------------------------------
#define	PIN_SCK         5   // PB5 --> SCK
#define	PIN_MOSI        6   // PB6 --> MOSI
#define	PIN_MISO        7   // PB7 <-- MISO
#define	PIN_NSS         4   // PB4 --> NSS
#define	PIN_NRESET      4   // PC4 --> NRESET
#define	PIN_CTRL_RX     3   // PD3 --> CTRL_RX
#define	PIN_CTRL_TX     2   // PD2 --> CTRL_TX
#define	PIN_CTRL_PA     1   // PD1 --> CTRL_PA
#define	PIN_CTRL_LNA    2   // PB2 --> CTRL_LNA
#define	PIN_CTRL_CLOCK  4   // PD4 --> CTRL_CLOCK
#define	PIN_CTRL_VGS    3   // PC3 --> CTRL_VGS

//--------------------------------------------
// SPI Data Transfer Frequency (10MHz max)
// SPI1_CK = fMASTER/2 = 8MHz
#define SPI_CLK_DIV     0

//--------------------------------------------
void hal_sx1278_init(void)
{
	// SPI1 peripheral clock enable
	CLK_PCKENR1 |= (1 << CLK_PCKENR1_SPI1);

	PB_DDR |= (1 << PIN_SCK) | (1 << PIN_MOSI);
	PB_CR1 |= (1 << PIN_SCK) | (1 << PIN_MOSI);
	PB_CR2 |= (1 << PIN_SCK) | (1 << PIN_MOSI);

	PB_CR2 &= ~(1 << PIN_MISO);
	PB_CR1 |= (1 << PIN_MISO);
	PB_DDR &= ~(1 << PIN_MISO);

	// BDM = 0: 2-line unidirectional data mode selected
	// BDOE = 0: irrelevant
	// SSM = 1: Software slave management disabled
	// SSI = 1: Internal slave select - master mode
	SPI_CR2 = (1 << SPI_CR2_SSM) | (1 << SPI_CR2_SSI);
	// SPE = 1: SPI enable
	// BR[2:0] = SPI_INIT_CLK_DIV
	// MSTR = 1: Master configuration
	SPI_CR1 = (1 << SPI_CR1_SPE) | SPI_CLK_DIV | (1 << SPI_CR1_MSTR);

	// CTRL_RX = 0, CTRL_TX = 0, CTRL_PA = 0, CTRL_CLOCK = 0
	PD_ODR &= ~(1 << PIN_CTRL_RX) & ~(1 << PIN_CTRL_TX) & ~(1 << PIN_CTRL_PA) & ~(1 << PIN_CTRL_CLOCK);
	PD_DDR |= (1 << PIN_CTRL_RX) | (1 << PIN_CTRL_TX) | (1 << PIN_CTRL_PA) | (1 << PIN_CTRL_CLOCK);
	PD_CR1 |= (1 << PIN_CTRL_RX) | (1 << PIN_CTRL_TX) | (1 << PIN_CTRL_PA) | (1 << PIN_CTRL_CLOCK);
	PD_CR2 |= (1 << PIN_CTRL_RX) | (1 << PIN_CTRL_TX) | (1 << PIN_CTRL_PA) | (1 << PIN_CTRL_CLOCK);

	PB_ODR |= (1 << PIN_NSS);        // NSS = 1
	PB_ODR &= ~(1 << PIN_CTRL_LNA);  // CTRL_LNA = 0
	PB_DDR |= (1 << PIN_NSS) | (1 << PIN_CTRL_LNA);
	PB_CR1 |= (1 << PIN_NSS) | (1 << PIN_CTRL_LNA);
	PB_CR2 |= (1 << PIN_NSS) | (1 << PIN_CTRL_LNA);

	PC_ODR |= (1 << PIN_NRESET);     // NRESET = 1
	PC_ODR &= ~(1 << PIN_CTRL_VGS);  // CTRL_VGS = 0
	PC_DDR |= (1 << PIN_NRESET) | (1 << PIN_CTRL_VGS);
	PC_CR1 |= (1 << PIN_NRESET) | (1 << PIN_CTRL_VGS);
	PC_CR2 |= (1 << PIN_NRESET) | (1 << PIN_CTRL_VGS);
	delay_ms(10);

	PC_ODR &= ~(1 << PIN_NRESET);    // NRESET = 0
	delay_ms(1);
	PC_CR2 &= ~(1 << PIN_NRESET);
	PC_CR1 &= ~(1 << PIN_NRESET);
	PC_DDR &= ~(1 << PIN_NRESET);    // NRESET = Z
	delay_ms(10);
}

//--------------------------------------------
void hal_sx1278_select(void)
{
	PB_ODR &= ~(1 << PIN_NSS);       // NSS = 0
}

//--------------------------------------------
void hal_sx1278_release(void)
{
	PB_ODR |= (1 << PIN_NSS);        // NSS = 1
}

//--------------------------------------------
uint8_t hal_sx1278_txrx(uint8_t data)
{
	while (!(SPI_SR & (1 << SPI_SR_TXE)));
	SPI_DR = data;
	while (!(SPI_SR & (1 << SPI_SR_RXNE)));
	return SPI_DR;
}

//--------------------------------------------
void hal_sx1278_ctrl_rx(uint8_t val)
{
	if (!val)
	{
		PD_ODR &= ~(1 << PIN_CTRL_RX); // CTRL_RX = 0
	}
	else
	{
		PD_ODR |= (1 << PIN_CTRL_RX);  // CTRL_RX = 1
	}
}

//--------------------------------------------
void hal_sx1278_ctrl_tx(uint8_t val)
{
	if (!val)
	{
		PD_ODR &= ~(1 << PIN_CTRL_TX); // CTRL_TX = 0
	}
	else
	{
		PD_ODR |= (1 << PIN_CTRL_TX);  // CTRL_TX = 1
	}
}

//--------------------------------------------
void hal_sx1278_ctrl_pa(uint8_t val)
{
	if (!val)
	{
		PD_ODR &= ~(1 << PIN_CTRL_PA); // CTRL_PA = 0
	}
	else
	{
		PD_ODR |= (1 << PIN_CTRL_PA);  // CTRL_PA = 1
	}
}

//--------------------------------------------
void hal_sx1278_ctrl_lna(uint8_t val)
{
	if (!val)
	{
		PB_ODR &= ~(1 << PIN_CTRL_LNA); // CTRL_LNA = 0
	}
	else
	{
		PB_ODR |= (1 << PIN_CTRL_LNA);  // CTRL_LNA = 1
	}
}

//--------------------------------------------
void hal_sx1278_ctrl_clock(uint8_t val)
{
	if (!val)
	{
		PD_ODR &= ~(1 << PIN_CTRL_CLOCK); // CTRL_CLOCK = 0
	}
	else
	{
		PD_ODR |= (1 << PIN_CTRL_CLOCK);  // CTRL_CLOCK = 1
	}
}

//--------------------------------------------
void hal_sx1278_ctrl_vgs(uint8_t val)
{
	if (!val)
	{
		PC_ODR &= ~(1 << PIN_CTRL_VGS); // CTRL_VGS = 0
	}
	else
	{
		PC_ODR |= (1 << PIN_CTRL_VGS);  // CTRL_VGS = 1
	}
}
