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

#include "platform.h"
#include "stm8l-hw.h"

#if defined E32_433T20D_32100_V3_0
//--------------------------------------------
#define	PORT_SCK          GPIO_B   // PB5 --> SCK
#define	PIN_SCK           5
#define	PORT_MOSI         GPIO_B   // PB6 --> MOSI
#define	PIN_MOSI          6
#define	PORT_MISO         GPIO_B   // PB7 <-- MISO
#define	PIN_MISO          7
#define	PORT_NSS          GPIO_B   // PB4 --> NSS
#define	PIN_NSS           4
#define	PORT_NRESET       GPIO_C   // PC4 --> NRESET
#define	PIN_NRESET        4
#define	PORT_CTRL_RX      GPIO_D   // PD2 --> CTRL_RX
#define	PIN_CTRL_RX       2
#define	PORT_CTRL_TX      GPIO_D   // PD3 --> CTRL_TX
#define	PIN_CTRL_TX       3

#elif defined E32_433T20D_10012_V6_0
//--------------------------------------------
#define	PORT_SCK          GPIO_B   // PB5 --> SCK
#define	PIN_SCK           5
#define	PORT_MOSI         GPIO_B   // PB6 --> MOSI
#define	PIN_MOSI          6
#define	PORT_MISO         GPIO_B   // PB7 <-- MISO
#define	PIN_MISO          7
#define	PORT_NSS          GPIO_B   // PB4 --> NSS
#define	PIN_NSS           4
#define	PORT_NRESET       GPIO_C   // PC4 --> NRESET
#define	PIN_NRESET        4
#define	PORT_CTRL_RX      GPIO_D   // PD2 --> CTRL_RX
#define	PIN_CTRL_RX       2
#define	PORT_CTRL_TX      GPIO_D   // PD3 --> CTRL_TX
#define	PIN_CTRL_TX       3
#define	PORT_CTRL_CLOCK   GPIO_D   // PD4 --> CTRL_CLOCK
#define	PIN_CTRL_CLOCK    4

#elif defined E32_433T30D_10010_V6_2
//--------------------------------------------
#define	PORT_SCK          GPIO_B   // PB5 --> SCK
#define	PIN_SCK           5
#define	PORT_MOSI         GPIO_B   // PB6 --> MOSI
#define	PIN_MOSI          6
#define	PORT_MISO         GPIO_B   // PB7 <-- MISO
#define	PIN_MISO          7
#define	PORT_NSS          GPIO_B   // PB4 --> NSS
#define	PIN_NSS           4
#define	PORT_NRESET       GPIO_C   // PC4 --> NRESET
#define	PIN_NRESET        4
#define	PORT_CTRL_RX      GPIO_D   // PD3 --> CTRL_RX
#define	PIN_CTRL_RX       3
#define	PORT_CTRL_TX      GPIO_D   // PD2 --> CTRL_TX
#define	PIN_CTRL_TX       2
#define	PORT_CTRL_CLOCK   GPIO_D   // PD4 --> CTRL_CLOCK
#define	PIN_CTRL_CLOCK    4
#define	PORT_CTRL_PA      GPIO_D   // PD1 --> CTRL_PA
#define	PIN_CTRL_PA       1
#define	PORT_CTRL_LNA     GPIO_B   // PB2 --> CTRL_LNA
#define	PIN_CTRL_LNA      2
#define	PORT_CTRL_VGS     GPIO_C   // PC3 --> CTRL_VGS
#define	PIN_CTRL_VGS      3

#elif defined E45_TTL_1W_10024_V2_0
//--------------------------------------------
#define	PORT_SCK          GPIO_B   // PB5 --> SCK
#define	PIN_SCK           5
#define	PORT_MOSI         GPIO_B   // PB6 --> MOSI
#define	PIN_MOSI          6
#define	PORT_MISO         GPIO_B   // PB7 <-- MISO
#define	PIN_MISO          7
#define	PORT_NSS          GPIO_B   // PB4 --> NSS
#define	PIN_NSS           4
#define	PORT_NRESET       GPIO_C   // PC3 --> NRESET
#define	PIN_NRESET        3
#define	PORT_CTRL_RX      GPIO_D   // PD2 --> CTRL_RX
#define	PIN_CTRL_RX       2
#define	PORT_CTRL_TX      GPIO_D   // PD3 --> CTRL_TX
#define	PIN_CTRL_TX       3
#define	PORT_CTRL_CLOCK   GPIO_C   // PC4 --> CTRL_CLOCK
#define	PIN_CTRL_CLOCK    4

#elif
#error "Unknown module"
#endif

//--------------------------------------------
// SPI Data Transfer Frequency (10MHz max)
// SPI1_CK = fMASTER/2 = 8MHz
#define SPI_CLK_DIV     0

//--------------------------------------------
void hal_sx127x_init(void)
{
	// Pins
	hw_cfg_pin(PORT_SCK,        PIN_SCK,        GPIO_DDR_OUT | GPIO_CR1_OUT_PUSHPULL | GPIO_CR2_OUT_10MHZ);
	hw_cfg_pin(PORT_MOSI,       PIN_MOSI,       GPIO_DDR_OUT | GPIO_CR1_OUT_PUSHPULL | GPIO_CR2_OUT_10MHZ);
	hw_cfg_pin(PORT_MISO,       PIN_MISO,       GPIO_DDR_INP | GPIO_CR1_INP_PULLUP   | GPIO_CR2_INP_EXTINTDIS);
	hw_cfg_pin(PORT_NSS,        PIN_NSS,        GPIO_DDR_OUT | GPIO_CR1_OUT_PUSHPULL | GPIO_CR2_OUT_10MHZ);
	hw_cfg_pin(PORT_NRESET,     PIN_NRESET,     GPIO_DDR_OUT | GPIO_CR1_OUT_PUSHPULL | GPIO_CR2_OUT_2MHZ);
	hw_cfg_pin(PORT_CTRL_RX,    PIN_CTRL_RX,    GPIO_DDR_OUT | GPIO_CR1_OUT_PUSHPULL | GPIO_CR2_OUT_2MHZ);
	hw_cfg_pin(PORT_CTRL_TX,    PIN_CTRL_TX,    GPIO_DDR_OUT | GPIO_CR1_OUT_PUSHPULL | GPIO_CR2_OUT_2MHZ);
#if defined E32_433T20D_10012_V6_0 || defined E32_433T30D_10010_V6_2 || defined E45_TTL_1W_10024_V2_0
	// TCXO LDO control pin
	hw_cfg_pin(PORT_CTRL_CLOCK, PIN_CTRL_CLOCK, GPIO_DDR_OUT | GPIO_CR1_OUT_PUSHPULL | GPIO_CR2_OUT_2MHZ);
#endif
#if defined E32_433T30D_10010_V6_2
	// Separate control pins for PA, LNA and PA mosfet Gate-Source offset
	hw_cfg_pin(PORT_CTRL_PA,    PIN_CTRL_PA,    GPIO_DDR_OUT | GPIO_CR1_OUT_PUSHPULL | GPIO_CR2_OUT_2MHZ);
	hw_cfg_pin(PORT_CTRL_LNA,   PIN_CTRL_LNA,   GPIO_DDR_OUT | GPIO_CR1_OUT_PUSHPULL | GPIO_CR2_OUT_2MHZ);
	hw_cfg_pin(PORT_CTRL_VGS,   PIN_CTRL_VGS,   GPIO_DDR_OUT | GPIO_CR1_OUT_PUSHPULL | GPIO_CR2_OUT_2MHZ);
#endif

	// SPI1 peripheral clock enable
	CLK_PCKENR1 |= (1 << CLK_PCKENR1_SPI1);

	// BDM = 0: 2-line unidirectional data mode selected
	// BDOE = 0: irrelevant
	// SSM = 1: Software slave management disabled
	// SSI = 1: Internal slave select - master mode
	SPI_CR2 = (1 << SPI_CR2_SSM) | (1 << SPI_CR2_SSI);
	// SPE = 1: SPI enable
	// BR[2:0] = SPI_INIT_CLK_DIV
	// MSTR = 1: Master configuration
	SPI_CR1 = (1 << SPI_CR1_SPE) | SPI_CLK_DIV | (1 << SPI_CR1_MSTR);

	hw_set_pin(PORT_NRESET, PIN_NRESET, 1); // NRESET = 1
	delay_ms(10);

	hw_set_pin(PORT_NRESET, PIN_NRESET, 0); // NRESET = 0
	delay_ms(1);

	hw_set_pin(PORT_NRESET, PIN_NRESET, 1); // NRESET = 1
	delay_ms(10);
}

//--------------------------------------------
void hal_sx127x_select(void)
{
	hw_set_pin(PORT_NSS, PIN_NSS, 0); // NSS = 0
}

//--------------------------------------------
void hal_sx127x_release(void)
{
	hw_set_pin(PORT_NSS, PIN_NSS, 1); // NSS = 1
}

//--------------------------------------------
uint8_t hal_sx127x_txrx(uint8_t data)
{
	while (!(SPI_SR & (1 << SPI_SR_TXE)));
	SPI_DR = data;
	while (!(SPI_SR & (1 << SPI_SR_RXNE)));
	return SPI_DR;
}

//--------------------------------------------
static void hal_sx127x_ctrl_rx(uint8_t val)
{
	hw_set_pin(PORT_CTRL_RX, PIN_CTRL_RX, val); // CTRL_RX = val
}

//--------------------------------------------
static void hal_sx127x_ctrl_tx(uint8_t val)
{
	hw_set_pin(PORT_CTRL_TX, PIN_CTRL_TX, val); // CTRL_TX = val
}

#if defined E32_433T20D_10012_V6_0 || defined E32_433T30D_10010_V6_2 || defined E45_TTL_1W_10024_V2_0
//--------------------------------------------
void hal_sx127x_ctrl_clock(uint8_t val)
{
	hw_set_pin(PORT_CTRL_CLOCK, PIN_CTRL_CLOCK, val); // CTRL_CLOCK = val
}
#endif

#if defined E32_433T30D_10010_V6_2
//--------------------------------------------
static void hal_sx127x_ctrl_pa(uint8_t val)
{
	hw_set_pin(PORT_CTRL_PA, PIN_CTRL_PA, val); // CTRL_PA = val
}

//--------------------------------------------
static void hal_sx127x_ctrl_lna(uint8_t val)
{
	hw_set_pin(PORT_CTRL_LNA, PIN_CTRL_LNA, val); // CTRL_LNA = val
}

//--------------------------------------------
static void hal_sx127x_ctrl_vgs(uint8_t val)
{
	hw_set_pin(PORT_CTRL_VGS, PIN_CTRL_VGS, val); // CTRL_VGS = val
}
#else
//--------------------------------------------
#define hal_sx127x_ctrl_pa(val)
#define hal_sx127x_ctrl_lna(val)
#define hal_sx127x_ctrl_vgs(val)
#endif

//--------------------------------------------
void hal_sx127x_radio_rx(void)
{
	// PA off
	hal_sx127x_ctrl_vgs(0);
	hal_sx127x_ctrl_pa(0);
	delay_ms(1);
	// RF switch: tx off, rx on
	hal_sx127x_ctrl_rx(1);
	hal_sx127x_ctrl_tx(0);
	// LNA on
	hal_sx127x_ctrl_lna(1);
	delay_ms(1);
}

//--------------------------------------------
void hal_sx127x_radio_tx(void)
{
	// LNA off
	hal_sx127x_ctrl_lna(0);
	// RF switch: tx on, rx off
	hal_sx127x_ctrl_rx(0);
	hal_sx127x_ctrl_tx(1);
	// PA on
	hal_sx127x_ctrl_pa(1);
	hal_sx127x_ctrl_vgs(1);
	delay_ms(1);
}
