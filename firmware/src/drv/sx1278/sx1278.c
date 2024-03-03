/*
* Copyright (c) 2018-2022 Vladimir Alemasov
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

#include <string.h>
#include <stdbool.h>
#include "platform.h"
#include "hal-sx1278.h"
#include "lora-defs.h"
#include "lora.h"
#include "sx1278-defs.h"
#include "sx1278.h"

//--------------------------------------------
#define SX1278_XTAL_FREQ                  32000000UL
#define SX1278_PLL_STEP_SHIFT_AMOUNT     (8)
#define SX1278_PLL_STEP_SCALED           (SX1278_XTAL_FREQ >> (19 - SX1278_PLL_STEP_SHIFT_AMOUNT))
#define SX1278_LF_MIN_RSSI_LEVEL         (-164)

//--------------------------------------------
const uint8_t sx1278_bw[] =
{
	SX1278_MODEM_CONFIG1_BW_7,
	SX1278_MODEM_CONFIG1_BW_10,
	SX1278_MODEM_CONFIG1_BW_15,
	SX1278_MODEM_CONFIG1_BW_20,
	SX1278_MODEM_CONFIG1_BW_31,
	SX1278_MODEM_CONFIG1_BW_41,
	SX1278_MODEM_CONFIG1_BW_62,
	SX1278_MODEM_CONFIG1_BW_125,
	SX1278_MODEM_CONFIG1_BW_250,
	SX1278_MODEM_CONFIG1_BW_500
};
const float sx1278_bw_khz[] =
{
	7.8,
	10.4,
	15.6,
	20.8,
	31.2,
	41.7,
	62.5,
	125.0,
	250.0,
	500.0
};
const uint8_t sx1278_sf[] =
{
	SX1278_MODEM_CONFIG2_SF6,
	SX1278_MODEM_CONFIG2_SF7,
	SX1278_MODEM_CONFIG2_SF8,
	SX1278_MODEM_CONFIG2_SF9,
	SX1278_MODEM_CONFIG2_SF10,
	SX1278_MODEM_CONFIG2_SF11,
	SX1278_MODEM_CONFIG2_SF12
};
const uint8_t sx1278_cr[] =
{
	SX1278_MODEM_CONFIG1_CR_4_5,
	SX1278_MODEM_CONFIG1_CR_4_6,
	SX1278_MODEM_CONFIG1_CR_4_7,
	SX1278_MODEM_CONFIG1_CR_4_8
};

//--------------------------------------------
static lora_modes_t lora_modes;
static struct
{
	uint32_t freq_in_hz;
	lora_bw_t bw;
	lora_sf_t sf;
	lora_cr_t cr;
	bool ldro;
	float bw_khz;
	float symbol_length_ms;
} lora_params;


//--------------------------------------------
// Private functions

//--------------------------------------------
// https://github.com/Lora-net/llcc68_driver/blob/master/src/llcc68.c#L1224
static uint32_t sx1278_convert_freq_in_hz_to_pll_step(uint32_t freq_in_hz)
{
	uint32_t steps_int;
	uint32_t steps_frac;

	// Get integer and fractional parts of the frequency computed with a PLL step scaled value
	steps_int  = freq_in_hz / SX1278_PLL_STEP_SCALED;
	steps_frac = freq_in_hz - (steps_int * SX1278_PLL_STEP_SCALED);

	// Apply the scaling factor to retrieve a frequency in Hz (+ ceiling)
	return (steps_int << SX1278_PLL_STEP_SHIFT_AMOUNT) + (((steps_frac << SX1278_PLL_STEP_SHIFT_AMOUNT) + (SX1278_PLL_STEP_SCALED >> 1)) / SX1278_PLL_STEP_SCALED);
}

//--------------------------------------------
static void sx1278_write_buf(uint8_t reg_addr, uint8_t *data, uint8_t data_length)
{
	uint8_t cnt;

	hal_sx1278_select();
	hal_sx1278_txrx(reg_addr | 0x80);
	for (cnt = 0; cnt < data_length; cnt++)
	{
		hal_sx1278_txrx(data[cnt]);
	}
	hal_sx1278_release();
}

//--------------------------------------------
static void sx1278_read_buf(uint8_t reg_addr, uint8_t *data, uint8_t data_length)
{
	uint8_t cnt;

	hal_sx1278_select();
	hal_sx1278_txrx(reg_addr & 0x7F);
	for (cnt = 0; cnt < data_length; cnt++)
	{
		data[cnt] = hal_sx1278_txrx(0);
	}
	hal_sx1278_release();
}

//--------------------------------------------
static void sx1278_write_register(uint8_t reg_addr, uint8_t data)
{
	sx1278_write_buf(reg_addr, &data, sizeof(uint8_t));
}

//--------------------------------------------
static uint8_t sx1278_read_register(uint8_t reg_addr)
{
	uint8_t data;
	sx1278_read_buf(reg_addr, &data, sizeof(uint8_t));
	return data;
}


//--------------------------------------------
static void sx1278_set_device_mode(uint8_t device_mode)
{
	uint8_t op_mode = sx1278_read_register(SX1278_REG_OPMODE);
	sx1278_write_register(SX1278_REG_OPMODE, (op_mode & ~SX1278_OPMODE_MODE_MASK) | device_mode);
}

//--------------------------------------------
static void sx1278_set_lora_mode(void)
{
	uint8_t op_mode = SX1278_OPMODE_LORA | SX1278_OPMODE_ACCESS_LORA | /*SX1278_OPMODE_LFM_OFF*/ SX1278_OPMODE_LFM_ON | SX1278_OPMODE_MODE_SLEEP;
	sx1278_write_register(SX1278_REG_OPMODE, op_mode);
}

#if 0
//--------------------------------------------
static void sx1278_set_fsk_mode(void)
{
	uint8_t op_mode = SX1278_OPMODE_FSK | SX1278_OPMODE_ACCESS_FSK | /*SX1278_OPMODE_LFM_OFF*/ SX1278_OPMODE_LFM_ON | SX1278_OPMODE_MODE_SLEEP;
	sx1278_write_register(SX1278_REG_OPMODE, op_mode);
}
#endif

//--------------------------------------------
static void sx1278_set_lora_sync_word(uint8_t sync_word)
{
	sx1278_write_register(SX1278_REG_SYNC_WORD, sync_word);
}


//--------------------------------------------
static void sx1278_set_frequency(uint32_t freq_in_hz)
{
	uint32_t steps = sx1278_convert_freq_in_hz_to_pll_step(freq_in_hz);
	sx1278_write_register(SX1278_REG_FR_MSB, (uint8_t)(steps >> 16));
	sx1278_write_register(SX1278_REG_FR_MID, (uint8_t)(steps >> 8));
	sx1278_write_register(SX1278_REG_FR_LSB, (uint8_t)(steps >> 0));
}

//--------------------------------------------
static void sx1278_set_bandwidth(uint8_t bw)
{
	uint8_t mc1 = sx1278_read_register(SX1278_REG_MODEM_CONFIG1);
	sx1278_write_register(SX1278_REG_MODEM_CONFIG1, (mc1 & ~SX1278_MODEM_CONFIG1_BW_MASK) | bw);
}

//--------------------------------------------
static void sx1278_set_coding_rate(uint8_t cr)
{
	uint8_t mc1 = sx1278_read_register(SX1278_REG_MODEM_CONFIG1);
	sx1278_write_register(SX1278_REG_MODEM_CONFIG1, (mc1 & ~SX1278_MODEM_CONFIG1_CR_MASK) | cr);
}

//--------------------------------------------
static void sx1278_set_header_mode(uint8_t hm)
{
	uint8_t mc1 = sx1278_read_register(SX1278_REG_MODEM_CONFIG1);
	sx1278_write_register(SX1278_REG_MODEM_CONFIG1, (mc1 & ~SX1278_MODEM_CONFIG1_HEADER_MASK) | hm);
}

//--------------------------------------------
static void sx1278_set_spreading_factor(uint8_t sf)
{
	uint8_t mc2 = sx1278_read_register(SX1278_REG_MODEM_CONFIG2);
	sx1278_write_register(SX1278_REG_MODEM_CONFIG2, (mc2 & ~SX1278_MODEM_CONFIG2_SF_MASK) | sf);
}

//--------------------------------------------
static void sx1278_set_detection_optimize(uint8_t dov)
{
	uint8_t reg = sx1278_read_register(SX1278_REG_DETECT_OPTIMIZE);
	sx1278_write_register(SX1278_REG_DETECT_OPTIMIZE, (reg & ~SX1278_DETECT_OPTIMIZE_MASK) | dov);
}

//--------------------------------------------
static void sx1278_set_detection_threshold(uint8_t dt)
{
	sx1278_write_register(SX1278_REG_DETECT_THRESHOLD, dt);
}

//--------------------------------------------
static void sx1278_set_crc(uint8_t crc)
{
	uint8_t mc2 = sx1278_read_register(SX1278_REG_MODEM_CONFIG2);
	sx1278_write_register(SX1278_REG_MODEM_CONFIG2, (mc2 & ~SX1278_MODEM_CONFIG2_CRC_MASK) | crc);
}

//--------------------------------------------
static void sx1278_set_ldro(bool enable)
{
	uint8_t mc3 = sx1278_read_register(SX1278_REG_MODEM_CONFIG3);
	if (enable)
	{
		sx1278_write_register(SX1278_REG_MODEM_CONFIG3, (mc3 & ~SX1278_MODEM_CONFIG3_LDRO_MASK) | SX1278_MODEM_CONFIG3_LDRO_ON);
	}
	else
	{
		sx1278_write_register(SX1278_REG_MODEM_CONFIG3, (mc3 & ~SX1278_MODEM_CONFIG3_LDRO_MASK) | SX1278_MODEM_CONFIG3_LDRO_OFF);
	}
}

//--------------------------------------------
static void sx1278_set_agc_auto(uint8_t agc_auto)
{
	uint8_t mc3 = sx1278_read_register(SX1278_REG_MODEM_CONFIG3);
	sx1278_write_register(SX1278_REG_MODEM_CONFIG3, (mc3 & ~SX1278_MODEM_CONFIG3_AGCAUTO_MASK) | agc_auto);
}

//--------------------------------------------
static void sx1278_set_pa_ramp(uint8_t pr)
{
	uint8_t reg = sx1278_read_register(SX1278_REG_PA_RAMP);
	sx1278_write_register(SX1278_REG_PA_RAMP, (reg & ~SX1278_PA_RAMP_MASK) | pr);
}

//--------------------------------------------
static void sx1278_set_pll_bandwidth(uint8_t bw)
{
	uint8_t reg = sx1278_read_register(SX1278_REG_PLL_LF);
	sx1278_write_register(SX1278_REG_PLL_LF, (reg & ~SX1278_PLL_LF_BANDWIDTH_MASK) | bw);
}

#if 0
//--------------------------------------------
static void sx1278_set_symb_timeout(uint16_t timeout)
{
	uint8_t mc2 = sx1278_read_register(SX1278_REG_MODEM_CONFIG2);
	sx1278_write_register(SX1278_REG_MODEM_CONFIG2, (mc2 & ~SX1278_MODEM_CONFIG2_TO_MASK) | (uint8_t)(timeout >> 8));
	sx1278_write_register(SX1278_REG_SYMB_TIMEOUT_LSB, (uint8_t)(timeout));
}
#endif

//--------------------------------------------
static void sx1278_set_preamble_length(uint16_t len)
{
	sx1278_write_register(SX1278_REG_PREAMBLE_MSB, (uint8_t)(len >> 8));
	sx1278_write_register(SX1278_REG_PREAMBLE_LSB, (uint8_t)(len));
}

//--------------------------------------------
// Check your board wiring (PA_BOOST / RFO pins of sx1278) before using this function
static void sx1278_set_tx_power(bool boost_on, uint8_t power, uint8_t max_power)
{
	if (power == 20 && boost_on)
	{
		// +20 dBm (100 mW)
		sx1278_write_register(SX1278_REG_PA_DAC, SX1278_PA_DAC_20);
	}
	else
	{
		// max +17 dBm (50 mW)
		sx1278_write_register(SX1278_REG_PA_DAC, SX1278_PA_DAC_NORMAL);
	}

	if (power > 15)
	{
		power = 15;
	}
	else if (power < 2)
	{
		power = 2;
	}

	if (boost_on)
	{
		// Enable PA_BOOST pin (27). Output power is limited to +20 dBm (100 mW)
		sx1278_write_register(SX1278_REG_PA_CONFIG, (0x80 | power));
	}
	else
	{
		// Enable RFO_HF pin (22). Output power is limited to +14 dBm (25 mW)
		if (max_power > 4)
		{
			max_power = 4;
		}
		sx1278_write_register(SX1278_REG_PA_CONFIG, ((max_power << 4) | power));
	}
}

//--------------------------------------------
static void sx1278_tx_start(void)
{
	// lna off
	hal_sx1278_ctrl_lna(0);
	// tx on - rx off
	hal_sx1278_ctrl_rx(0);
	hal_sx1278_ctrl_tx(1);
	// pa on
	hal_sx1278_ctrl_pa(1);
	hal_sx1278_ctrl_vgs(1);
	delay_ms(1);
}

//--------------------------------------------
static void sx1278_rx_start(void)
{
	// pa off
	hal_sx1278_ctrl_vgs(0);
	hal_sx1278_ctrl_pa(0);
	delay_ms(1);
	// tx off - rx on
	hal_sx1278_ctrl_rx(1);
	hal_sx1278_ctrl_tx(0);
	// lna on
	hal_sx1278_ctrl_lna(1);
	delay_ms(1);
}

//--------------------------------------------
// SX1276/77/78 Errata Note Revision 1 - Sept 2013
// 2.1 Sensitivity Optimization with a 500 kHz Bandwidth
// For carrier frequencies ranging from 410 to 525 MHz
static void sx1278_sensitivity_optimization_workaround(lora_bw_t bw)
{
	if (bw == LORA_BW_500)
	{
		sx1278_write_register(0x36, 0x02);
		sx1278_write_register(0x3A, 0x7F);
	}
	else
	{
		sx1278_write_register(0x36, 0x03);
	}
}

//--------------------------------------------
// SX1276/77/78 Errata Note Revision 1 - Sept 2013
// 2.3 Receiver Spurious Reception of a LoRa Signal
static void sx1278_receiver_spurious_reception_workaround(uint32_t freq_in_hz, lora_bw_t bw, bool rx)
{
	uint32_t freq;
	uint8_t reg_0x31;
	uint8_t reg_0x2F;
	uint8_t reg_0x30;

	freq = freq_in_hz;
	reg_0x31 = sx1278_read_register(0x31);
	reg_0x31 &= ~0x80;
	reg_0x2F = 0x44;
	reg_0x30 = 0x00;

	switch (bw)
	{
	case LORA_BW_7:
		reg_0x2F = 0x48;
		freq += 7810;
		break;
	case LORA_BW_10:
		freq += 10420;
		break;
	case LORA_BW_15:
		freq += 15620;
		break;
	case LORA_BW_20:
		freq += 20830;
		break;
	case LORA_BW_31:
		freq += 31250;
		break;
	case LORA_BW_41:
		freq += 41670;
		break;
	case LORA_BW_62:
	case LORA_BW_125:
	case LORA_BW_250:
		reg_0x2F = 0x40;
		break;
	case LORA_BW_500:
		reg_0x31 |= 0x80;
		reg_0x2F = sx1278_read_register(0x2F);
		reg_0x30 = sx1278_read_register(0x30);
	}

	if (rx)
	{
		sx1278_set_frequency(freq);
	}
	else
	{
		sx1278_set_frequency(freq_in_hz);
	}
	sx1278_write_register(0x31, reg_0x31);
	sx1278_write_register(0x2F, reg_0x2F);
	sx1278_write_register(0x30, reg_0x30);
}


//--------------------------------------------
// Driver functions

//--------------------------------------------
static const char core_name[] = "SX1278";

//--------------------------------------------
const char* sx1278_get_core_name(void)
{
	return core_name;
}

//--------------------------------------------
lora_status_t sx1278_init(lora_modes_t *modes)
{
	if (modes->pkt_len_mode == LORA_PKT_IMPLICIT && modes->pkt_len == 0)
	{
		return LORA_STATUS_UNSUPPORTED_FEATURE;
	}

	hal_sx1278_init();
	memcpy(&lora_modes, modes, sizeof(lora_modes_t));

#if 0
	uint8_t version = sx1278_read_register(SX1278_REG_VERSION);
#endif

	sx1278_set_lora_mode();
	sx1278_set_device_mode(SX1278_OPMODE_MODE_STDBY);
	sx1278_set_lora_sync_word(lora_modes.sync_word);

	sx1278_set_header_mode(lora_modes.pkt_len_mode == LORA_PKT_EXPLICIT ? SX1278_MODEM_CONFIG1_HEADER_EXPL : SX1278_MODEM_CONFIG1_HEADER_IMPL);
	sx1278_set_crc(lora_modes.crc_mode == LORA_CRC_OFF ?  SX1278_MODEM_CONFIG2_CRC_OFF : SX1278_MODEM_CONFIG2_CRC_ON);
	sx1278_set_preamble_length(lora_modes.preamble_len);

	sx1278_set_agc_auto(SX1278_MODEM_CONFIG3_AGCAUTO_OFF);
	sx1278_set_pll_bandwidth(SX1278_PLL_LF_BANDWIDTH_75);
	sx1278_set_pa_ramp(SX1278_PA_RAMP_50);
	sx1278_set_tx_power(true, 20, 0);

	sx1278_write_register(SX1278_REG_FIFO_RX_BASE_ADDR, 0x00);
	sx1278_write_register(SX1278_REG_FIFO_TX_BASE_ADDR, 0x00);

	// clock on
	hal_sx1278_ctrl_clock(1);
	delay_ms(1);

	return LORA_STATUS_OK;
}

//--------------------------------------------
lora_status_t sx1278_set_params(lora_params_t *params)
{
	uint8_t bw;
	uint8_t sf;
	uint8_t cr;

	if (params->freq_in_hz < 410000000 || params->freq_in_hz > 525000000)
	{
		return LORA_STATUS_UNSUPPORTED_FEATURE;
	}
	if (params->sf < LORA_SF6)
	{
		return LORA_STATUS_UNSUPPORTED_FEATURE;
	}
	if (params->sf == LORA_SF6 && lora_modes.pkt_len_mode == LORA_PKT_EXPLICIT)
	{
		return LORA_STATUS_UNSUPPORTED_FEATURE;
	}

	memcpy(&lora_params, params, sizeof(lora_params_t));

	bw = sx1278_bw[(int)lora_params.bw];
	sf = sx1278_sf[(int)lora_params.sf - LORA_SF6];
	cr = sx1278_cr[(int)lora_params.cr - LORA_CR_4_5];
	lora_params.bw_khz = sx1278_bw_khz[(int)lora_params.bw];
	lora_params.symbol_length_ms = (float)((uint32_t)1 << (sf >> 4)) / lora_params.bw_khz;
	lora_params.ldro = (lora_params.symbol_length_ms >= 16.0) ? true : false;

	sx1278_set_device_mode(SX1278_OPMODE_MODE_STDBY);
	sx1278_set_bandwidth(bw);
	sx1278_set_coding_rate(cr);
	sx1278_set_spreading_factor(sf);
	sx1278_set_frequency(lora_params.freq_in_hz);
	sx1278_set_ldro(lora_params.ldro);

	if (lora_params.sf == LORA_SF6)
	{
		sx1278_set_detection_optimize(SX1278_DETECT_OPTIMIZE_SF6);
		sx1278_set_detection_threshold(SX1278_DETECT_THRESHOLD_SF6);
	}
	else
	{
		sx1278_set_detection_optimize(SX1278_DETECT_OPTIMIZE_DEF);
		sx1278_set_detection_threshold(SX1278_DETECT_THRESHOLD_DEF);
	}

	sx1278_sensitivity_optimization_workaround(lora_params.bw);

	return LORA_STATUS_OK;
}

//--------------------------------------------
void sx1278_start_continious_rx(void)
{
	sx1278_set_device_mode(SX1278_OPMODE_MODE_STDBY);

	if (lora_modes.pkt_len_mode == LORA_PKT_IMPLICIT)
	{
		sx1278_write_register(SX1278_REG_PAYLOAD_LENGTH, lora_modes.pkt_len);
	}
	if (lora_modes.dio_irq_mode == LORA_DIO_IRQ_ON)
	{
		sx1278_write_register(
			SX1278_REG_DIO_MAPPING_1,
			SX1278_MAP_DIO0_LORA_RXDONE | SX1278_MAP_DIO1_LORA_NOP | SX1278_MAP_DIO2_LORA_NOP | SX1278_MAP_DIO3_LORA_NOP
		);
	}
	sx1278_write_register(SX1278_REG_IRQ_FLAGS, 0xFF);
	sx1278_write_register(SX1278_REG_IRQ_FLAGS_MASK, ~(SX1278_IRQ_LORA_RXDONE | SX1278_IRQ_LORA_CRCERROR));
	sx1278_write_register(SX1278_REG_FIFO_ADDR_PTR, 0x00);

	sx1278_receiver_spurious_reception_workaround(lora_params.freq_in_hz, lora_params.bw, true);

	sx1278_rx_start();
	sx1278_set_device_mode(SX1278_OPMODE_MODE_RXCONTINUOUS);
}

//--------------------------------------------
lora_irq_src_t sx1278_irq_poll(void)
{
	uint8_t irq;

	irq = sx1278_read_register(SX1278_REG_IRQ_FLAGS);

	if (irq == SX1278_IRQ_LORA_NONE)
	{
		return LORA_IRQ_NONE;
	}
	if (irq & SX1278_IRQ_LORA_CAD_DETECTED)
	{
		sx1278_write_register(SX1278_REG_IRQ_FLAGS, irq | SX1278_IRQ_LORA_CAD_DETECTED);
		return LORA_IRQ_CAD_DETECTED;
	}
	if (irq & SX1278_IRQ_LORA_CAD_DONE)
	{
		sx1278_write_register(SX1278_REG_IRQ_FLAGS, irq | SX1278_IRQ_LORA_CAD_DONE);
		return LORA_IRQ_CAD_DONE;
	}
	if (irq & SX1278_IRQ_LORA_RXDONE)
	{
		sx1278_write_register(SX1278_REG_IRQ_FLAGS, irq | SX1278_IRQ_LORA_RXDONE);
		return LORA_IRQ_RX_DONE;
	}
	if (irq & SX1278_IRQ_LORA_TXDONE)
	{
		sx1278_write_register(SX1278_REG_IRQ_FLAGS, irq | SX1278_IRQ_LORA_TXDONE);
		return LORA_IRQ_TX_DONE;
	}
	return LORA_IRQ_NONE;
}

//--------------------------------------------
lora_status_t sx1278_read_rx_packet(uint8_t *buf, uint8_t *len, int16_t *rssi, int8_t *snr)
{
	uint8_t irq;

	sx1278_set_device_mode(SX1278_OPMODE_MODE_STDBY);

	if (lora_modes.pkt_len_mode == LORA_PKT_IMPLICIT)
	{
		*len = lora_modes.pkt_len;
	}
	else
	{
		*len = sx1278_read_register(SX1278_REG_RX_NB_BYTES);
	}

	irq = sx1278_read_register(SX1278_REG_IRQ_FLAGS);
	if (irq & SX1278_IRQ_LORA_CRCERROR)
	{
		return LORA_STATUS_ERROR;
	}

	sx1278_read_buf(SX1278_REG_FIFO, buf, *len);
	*snr = (int8_t)sx1278_read_register(SX1278_REG_PKT_SNR_VALUE) / 4;

	if (*snr < 0)
	{
		*rssi = SX1278_LF_MIN_RSSI_LEVEL + (int16_t)sx1278_read_register(SX1278_REG_PKT_RSSI_VALUE) + *snr;
	}
	else
	{
		*rssi = SX1278_LF_MIN_RSSI_LEVEL + ((int16_t)sx1278_read_register(SX1278_REG_PKT_RSSI_VALUE))*(16/15);
	}
	return LORA_STATUS_OK;
}

//--------------------------------------------
uint32_t sx1278_send_tx_packet(uint8_t *buf, uint8_t len)
{
	float pkt_in_symb;

	pkt_in_symb = lora_get_packet_length_in_symbols(
		lora_params.sf,
		lora_params.cr,
		len,
		lora_modes.preamble_len,
		lora_modes.crc_mode == LORA_CRC_ON ? true : false,
		lora_modes.pkt_len_mode == LORA_PKT_IMPLICIT ? true : false,
		lora_params.ldro
	);
	uint32_t tx_time = (uint32_t)(pkt_in_symb * lora_params.symbol_length_ms);

	sx1278_set_device_mode(SX1278_OPMODE_MODE_STDBY);

	if (lora_modes.dio_irq_mode == LORA_DIO_IRQ_ON)
	{
		sx1278_write_register(
			SX1278_REG_DIO_MAPPING_1,
			SX1278_MAP_DIO0_LORA_TXDONE | SX1278_MAP_DIO1_LORA_NOP | SX1278_MAP_DIO2_LORA_NOP | SX1278_MAP_DIO3_LORA_NOP
		);
	}
	sx1278_write_register(SX1278_REG_IRQ_FLAGS, 0xFF);
	sx1278_write_register(SX1278_REG_IRQ_FLAGS_MASK, ~(SX1278_IRQ_LORA_TXDONE));
	sx1278_write_register(SX1278_REG_FIFO_ADDR_PTR, 0x00);
	sx1278_write_register(SX1278_REG_PAYLOAD_LENGTH, len);
	sx1278_write_buf(SX1278_REG_FIFO, buf, len);

	sx1278_receiver_spurious_reception_workaround(lora_params.freq_in_hz, lora_params.bw, false);

	sx1278_tx_start();
	// start to transmit
	sx1278_set_device_mode(SX1278_OPMODE_MODE_TX);

	return tx_time;
}

//--------------------------------------------
// Checks current RSSI in LORA mode
// This does NOT interrupt the LORA receive mode
void sx1278_read_rssi(int16_t *rssi)
{
	*rssi = SX1278_LF_MIN_RSSI_LEVEL + (int16_t)sx1278_read_register(SX1278_REG_RSSI_VALUE);
}
