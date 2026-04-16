/*
* Copyright (c) 2026 Vladimir Alemasov
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
#include "sx127x-defs.h"
#include "hal-sx127x.h"

//--------------------------------------------
#define SX127X_XTAL_FREQ                  32000000UL
#define SX127X_PLL_STEP_SHIFT_AMOUNT     (8)
#define SX127X_PLL_STEP_SCALED           (SX127X_XTAL_FREQ >> (19 - SX127X_PLL_STEP_SHIFT_AMOUNT))

//--------------------------------------------
// https://github.com/Lora-net/llcc68_driver/blob/master/src/llcc68.c#L1224
uint32_t sx127x_convert_freq_in_hz_to_pll_step(uint32_t freq_in_hz)
{
	uint32_t steps_int;
	uint32_t steps_frac;

	// Get integer and fractional parts of the frequency computed with a PLL step scaled value
	steps_int  = freq_in_hz / SX127X_PLL_STEP_SCALED;
	steps_frac = freq_in_hz - (steps_int * SX127X_PLL_STEP_SCALED);

	// Apply the scaling factor to retrieve a frequency in Hz (+ ceiling)
	return (steps_int << SX127X_PLL_STEP_SHIFT_AMOUNT) + (((steps_frac << SX127X_PLL_STEP_SHIFT_AMOUNT) + (SX127X_PLL_STEP_SCALED >> 1)) / SX127X_PLL_STEP_SCALED);
}

//--------------------------------------------
void sx127x_write_buf(uint8_t reg_addr, uint8_t *data, uint8_t data_length)
{
	uint8_t cnt;

	hal_sx127x_select();
	hal_sx127x_txrx(reg_addr | 0x80);
	for (cnt = 0; cnt < data_length; cnt++)
	{
		hal_sx127x_txrx(data[cnt]);
	}
	hal_sx127x_release();
}

//--------------------------------------------
void sx127x_read_buf(uint8_t reg_addr, uint8_t *data, uint8_t data_length)
{
	uint8_t cnt;

	hal_sx127x_select();
	hal_sx127x_txrx(reg_addr & 0x7F);
	for (cnt = 0; cnt < data_length; cnt++)
	{
		data[cnt] = hal_sx127x_txrx(0);
	}
	hal_sx127x_release();
}

//--------------------------------------------
void sx127x_write_register(uint8_t reg_addr, uint8_t data)
{
	sx127x_write_buf(reg_addr, &data, sizeof(uint8_t));
}

//--------------------------------------------
uint8_t sx127x_read_register(uint8_t reg_addr)
{
	uint8_t data;
	sx127x_read_buf(reg_addr, &data, sizeof(uint8_t));
	return data;
}

//--------------------------------------------
void sx127x_set_device_mode(uint8_t device_mode)
{
	uint8_t op_mode = sx127x_read_register(SX127X_REG_OPMODE);
	sx127x_write_register(SX127X_REG_OPMODE, (op_mode & ~SX127X_OPMODE_MODE_MASK) | device_mode);
}

//--------------------------------------------
void sx127x_set_lora_sync_word(uint8_t sync_word)
{
	sx127x_write_register(SX127X_REG_SYNC_WORD, sync_word);
}

//--------------------------------------------
void sx127x_set_frequency(uint32_t freq_in_hz)
{
	uint32_t steps = sx127x_convert_freq_in_hz_to_pll_step(freq_in_hz);
	sx127x_write_register(SX127X_REG_FR_MSB, (uint8_t)(steps >> 16));
	sx127x_write_register(SX127X_REG_FR_MID, (uint8_t)(steps >> 8));
	sx127x_write_register(SX127X_REG_FR_LSB, (uint8_t)(steps >> 0));
}

//--------------------------------------------
void sx127x_set_spreading_factor(uint8_t sf)
{
	uint8_t mc2 = sx127x_read_register(SX127X_REG_MODEM_CONFIG2);
	sx127x_write_register(SX127X_REG_MODEM_CONFIG2, (mc2 & ~SX127X_MODEM_CONFIG2_SF_MASK) | sf);
}

//--------------------------------------------
void sx127x_set_detection_optimize(uint8_t dov)
{
	uint8_t reg = sx127x_read_register(SX127X_REG_DETECT_OPTIMIZE);
	sx127x_write_register(SX127X_REG_DETECT_OPTIMIZE, (reg & ~SX127X_DETECT_OPTIMIZE_MASK) | dov);
}

//--------------------------------------------
void sx127x_set_detection_threshold(uint8_t dt)
{
	sx127x_write_register(SX127X_REG_DETECT_THRESHOLD, dt);
}

//--------------------------------------------
void sx127x_set_preamble_length(uint16_t len)
{
	sx127x_write_register(SX127X_REG_PREAMBLE_MSB, (uint8_t)(len >> 8));
	sx127x_write_register(SX127X_REG_PREAMBLE_LSB, (uint8_t)(len));
}
