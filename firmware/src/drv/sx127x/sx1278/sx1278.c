/*
* Copyright (c) 2018-2022, 2026 Vladimir Alemasov
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
#include "hal-sx127x.h"
#include "lora-defs.h"
#include "lora.h"
#include "sx127x-defs.h"
#include "sx127x.h"

#define SX1278_LF_MIN_RSSI_LEVEL       (-164)
#define SX1278_HF_MIN_RSSI_LEVEL       (-157)

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
#if defined _DUMMY_SX1277_
const uint8_t sx1278_sf[] =
{
	SX127X_MODEM_CONFIG2_SF6,
	SX127X_MODEM_CONFIG2_SF7,
	SX127X_MODEM_CONFIG2_SF8,
	SX127X_MODEM_CONFIG2_SF9
};
#else
const uint8_t sx1278_sf[] =
{
	SX127X_MODEM_CONFIG2_SF6,
	SX127X_MODEM_CONFIG2_SF7,
	SX127X_MODEM_CONFIG2_SF8,
	SX127X_MODEM_CONFIG2_SF9,
	SX127X_MODEM_CONFIG2_SF10,
	SX127X_MODEM_CONFIG2_SF11,
	SX127X_MODEM_CONFIG2_SF12
};
#endif
const uint8_t sx1278_cr[] =
{
	SX1278_MODEM_CONFIG1_CR_4_5,
	SX1278_MODEM_CONFIG1_CR_4_6,
	SX1278_MODEM_CONFIG1_CR_4_7,
	SX1278_MODEM_CONFIG1_CR_4_8
};

//--------------------------------------------
static lora_modes_t lora_modes;
static lora_core_t lora_core;


//--------------------------------------------
// LoRa core functions

//--------------------------------------------
static void sx1278_set_lora_mode(void)
{
	uint8_t op_mode = SX127X_OPMODE_LORA | SX127X_OPMODE_ACCESS_LORA | SX1278_OPMODE_LFM_ON | SX127X_OPMODE_MODE_SLEEP;
	sx127x_write_register(SX127X_REG_OPMODE, op_mode);
}

//--------------------------------------------
static void sx1278_set_bandwidth(uint8_t bw)
{
	uint8_t mc1 = sx127x_read_register(SX127X_REG_MODEM_CONFIG1);
	sx127x_write_register(SX127X_REG_MODEM_CONFIG1, (mc1 & ~SX1278_MODEM_CONFIG1_BW_MASK) | bw);
}

//--------------------------------------------
static void sx1278_set_coding_rate(uint8_t cr)
{
	uint8_t mc1 = sx127x_read_register(SX127X_REG_MODEM_CONFIG1);
	sx127x_write_register(SX127X_REG_MODEM_CONFIG1, (mc1 & ~SX1278_MODEM_CONFIG1_CR_MASK) | cr);
}

//--------------------------------------------
static void sx1278_set_header_mode(uint8_t hm)
{
	uint8_t mc1 = sx127x_read_register(SX127X_REG_MODEM_CONFIG1);
	sx127x_write_register(SX127X_REG_MODEM_CONFIG1, (mc1 & ~SX1278_MODEM_CONFIG1_HEADER_MASK) | hm);
}

//--------------------------------------------
static void sx1278_set_crc(uint8_t crc)
{
	uint8_t mc2 = sx127x_read_register(SX127X_REG_MODEM_CONFIG2);
	sx127x_write_register(SX127X_REG_MODEM_CONFIG2, (mc2 & ~SX1278_MODEM_CONFIG2_CRC_MASK) | crc);
}

//--------------------------------------------
static void sx1278_set_ldro(bool enable)
{
	uint8_t mc3 = sx127x_read_register(SX1278_REG_MODEM_CONFIG3);
	if (enable)
	{
		sx127x_write_register(SX1278_REG_MODEM_CONFIG3, (mc3 & ~SX1278_MODEM_CONFIG3_LDRO_MASK) | SX1278_MODEM_CONFIG3_LDRO_ON);
	}
	else
	{
		sx127x_write_register(SX1278_REG_MODEM_CONFIG3, (mc3 & ~SX1278_MODEM_CONFIG3_LDRO_MASK) | SX1278_MODEM_CONFIG3_LDRO_OFF);
	}
}

//--------------------------------------------
static void sx1278_set_agc_auto(uint8_t agc_auto)
{
	uint8_t mc3 = sx127x_read_register(SX1278_REG_MODEM_CONFIG3);
	sx127x_write_register(SX1278_REG_MODEM_CONFIG3, (mc3 & ~SX1278_MODEM_CONFIG3_AGCAUTO_MASK) | agc_auto);
}

//--------------------------------------------
static void sx1278_set_tx_power(bool boost_on, int8_t power)
{
	bool pa_dac_20dBm = false;

	if (boost_on)
	{
		if (power > 17)
		{
			pa_dac_20dBm = true;
			sx127x_write_register(SX1278_REG_PA_DAC, SX127X_PA_DAC_20);
		}
		else
		{
			sx127x_write_register(SX1278_REG_PA_DAC, SX127X_PA_DAC_NORMAL);
		}
		if (pa_dac_20dBm)
		{
			if (power < 5)
			{
				power = 5;
			}
			if (power > 20)
			{
				power = 20;
			}
			// Pout = 20 - (15 - OutputPower)
			sx127x_write_register(SX127X_REG_PA_CONFIG, (SX127X_PA_CONFIG_BOOST | ((uint8_t)((uint16_t)(power - 5)))));
		}
		else
		{
			if (power < 2)
			{
				power = 2;
			}
			if (power > 17)
			{
				power = 17;
			}
			// Pout = 17 - (15 - OutputPower)
			sx127x_write_register(SX127X_REG_PA_CONFIG, (SX127X_PA_CONFIG_BOOST | ((uint8_t)((uint16_t)(power - 2)))));
		}
	}
	else
	{
		if (power > 0)
		{
			if (power > 15)
			{
				power = 15;
			}
			// Pout = Pmax - (15 - OutputPower)
			// Pmax = 10.8 + 0.6 * MaxPower
			// MaxPower = 7 => Pmax = 15 => Pout = OutputPower
			sx127x_write_register(SX127X_REG_PA_CONFIG, (SX127X_PA_CONFIG_MAXPOWER_MAX | (uint8_t)power));
        }
		else
		{
			if (power < -4)
			{
				power = -4;
			}
			// Pout = Pmax - (15 - OutputPower)
			// Pmax = 10.8 + 0.6 * MaxPower
			// MaxPower = 0 => Pmax = 10.8 => Pout = OutputPower - 4.2 ~= OutputPower - 4
			sx127x_write_register(SX127X_REG_PA_CONFIG, (SX127X_PA_CONFIG_MAXPOWER_MAX | (uint8_t)(power + 4)));
		}
	}
}

//--------------------------------------------
static void sx1278_tx_start(void)
{
	hal_sx127x_radio_tx();
}

//--------------------------------------------
static void sx1278_rx_start(void)
{
	hal_sx127x_radio_rx();
}

//--------------------------------------------
// SX1276/77/78 Errata Note Revision 1 - Sept 2013
// 2.1 Sensitivity Optimization with a 500 kHz Bandwidth
// For carrier frequencies ranging from 410 to 525 MHz
static void sx1278_sensitivity_optimization_workaround(lora_bw_t bw)
{
#if defined E32_433T20D_32100_V3_0 || defined E32_433T20D_10012_V6_0 || defined E32_433T30D_10010_V6_2
	// LF bands
	if (bw == LORA_BW_500)
	{
		sx127x_write_register(0x36, 0x02);
		sx127x_write_register(0x3A, 0x7F);
	}
#elif defined E45_TTL_1W_10024_V2_0
	// HF bands
	if (bw == LORA_BW_500)
	{
		sx127x_write_register(0x36, 0x02);
		sx127x_write_register(0x3A, 0x64);
	}
#endif
	else
	{
		sx127x_write_register(0x36, 0x03);
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
	reg_0x31 = sx127x_read_register(0x31);
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
		reg_0x2F = sx127x_read_register(0x2F);
		reg_0x30 = sx127x_read_register(0x30);
	}

	if (rx)
	{
		sx127x_set_frequency(freq);
	}
	else
	{
		sx127x_set_frequency(freq_in_hz);
	}
	sx127x_write_register(0x31, reg_0x31);
	sx127x_write_register(0x2F, reg_0x2F);
	sx127x_write_register(0x30, reg_0x30);
}


//--------------------------------------------
// Driver functions

//--------------------------------------------
#if defined E45_TTL_1W_10024_V2_0
static const char core_name[] = "SX1276";
#elif defined _DUMMY_SX1277_
static const char core_name[] = "SX1277";
#elif defined E32_433T20D_32100_V3_0 || defined E32_433T20D_10012_V6_0 || defined E32_433T30D_10010_V6_2
static const char core_name[] = "SX1278";
#elif defined _DUMMY_SX1279_
static const char core_name[] = "SX1279";
#elif
#error "Unknown module"
#endif

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

	hal_sx127x_init();
	memcpy(&lora_modes, modes, sizeof(lora_modes_t));

#if 0
	uint8_t version = sx127x_read_register(SX127X_REG_VERSION);
#endif

	sx1278_set_lora_mode();
	sx127x_set_device_mode(SX127X_OPMODE_MODE_STDBY);
	sx127x_set_lora_sync_word(lora_modes.sync_word);

	sx1278_set_header_mode(lora_modes.pkt_len_mode == LORA_PKT_EXPLICIT ? SX1278_MODEM_CONFIG1_HEADER_EXPL : SX1278_MODEM_CONFIG1_HEADER_IMPL);
	sx1278_set_crc(lora_modes.crc_mode == LORA_CRC_OFF ?  SX1278_MODEM_CONFIG2_CRC_OFF : SX1278_MODEM_CONFIG2_CRC_ON);
	sx127x_set_preamble_length(lora_modes.preamble_len);

	sx1278_set_agc_auto(SX1278_MODEM_CONFIG3_AGCAUTO_OFF);
	sx1278_set_tx_power(true, 20);

	sx127x_write_register(SX127X_REG_FIFO_RX_BASE_ADDR, 0x00);
	sx127x_write_register(SX127X_REG_FIFO_TX_BASE_ADDR, 0x00);

#if defined E32_433T20D_10012_V6_0 || defined E32_433T30D_10010_V6_2 || defined E45_TTL_1W_10024_V2_0
	// TCXO LDO On
	hal_sx127x_ctrl_clock(1);
	delay_ms(1);
#endif

	return LORA_STATUS_OK;
}

//--------------------------------------------
lora_status_t sx1278_set_params(lora_params_t *params)
{
	uint8_t bw;
	uint8_t sf;
	uint8_t cr;

#if defined E32_433T20D_32100_V3_0 || defined E32_433T20D_10012_V6_0 || defined E32_433T30D_10010_V6_2
	// LF bands
	if (params->freq_in_hz < 410000000 || params->freq_in_hz > 525000000)
	{
		return LORA_STATUS_UNSUPPORTED_FEATURE;
	}
#elif defined E45_TTL_1W_10024_V2_0
	// 868 MHz band
	if (params->freq_in_hz < 862000000 || params->freq_in_hz > 893000000)
	{
		return LORA_STATUS_UNSUPPORTED_FEATURE;
	}
#endif
#if defined _DUMMY_SX1277_
	if (params->sf < LORA_SF6 || params->sf > LORA_SF9)
#else
	if (params->sf < LORA_SF6 || params->sf > LORA_SF12)
#endif
	{
		return LORA_STATUS_UNSUPPORTED_FEATURE;
	}
	if (params->cr < LORA_CR_4_5 || params->cr > LORA_CR_4_8)
	{
		return LORA_STATUS_UNSUPPORTED_FEATURE;
	}
	if (params->bw > LORA_BW_500)
	{
		return LORA_STATUS_UNSUPPORTED_FEATURE;
	}

	if (params->sf == LORA_SF6 && lora_modes.pkt_len_mode == LORA_PKT_EXPLICIT)
	{
		return LORA_STATUS_UNSUPPORTED_FEATURE;
	}

	memcpy(&lora_core.params, params, sizeof(lora_params_t));

	bw = sx1278_bw[(int)lora_core.params.bw];
	sf = sx1278_sf[(int)lora_core.params.sf - LORA_SF6];
	cr = sx1278_cr[(int)lora_core.params.cr - LORA_CR_4_5];
	lora_core.bw_khz = sx1278_bw_khz[(int)lora_core.params.bw];
	lora_core.symbol_length_ms = (float)((uint32_t)1 << (sf >> 4)) / lora_core.bw_khz;
	lora_core.ldro = (lora_core.symbol_length_ms >= 16.0) ? true : false;

	sx127x_set_device_mode(SX127X_OPMODE_MODE_STDBY);
	sx1278_set_bandwidth(bw);
	sx1278_set_coding_rate(cr);
	sx127x_set_spreading_factor(sf);
	sx127x_set_frequency(lora_core.params.freq_in_hz);
	sx1278_set_ldro(lora_core.ldro);

	if (lora_core.params.sf == LORA_SF6)
	{
		sx127x_set_detection_optimize(SX127X_DETECT_OPTIMIZE_SF6);
		sx127x_set_detection_threshold(SX127X_DETECT_THRESHOLD_SF6);
	}
	else
	{
		sx127x_set_detection_optimize(SX127X_DETECT_OPTIMIZE_DEF);
		sx127x_set_detection_threshold(SX127X_DETECT_THRESHOLD_DEF);
	}

	sx1278_sensitivity_optimization_workaround(lora_core.params.bw);

	return LORA_STATUS_OK;
}

//--------------------------------------------
void sx1278_start_continious_rx(void)
{
	sx127x_set_device_mode(SX127X_OPMODE_MODE_STDBY);

	if (lora_modes.pkt_len_mode == LORA_PKT_IMPLICIT)
	{
		sx127x_write_register(SX127X_REG_PAYLOAD_LENGTH, lora_modes.pkt_len);
	}
	if (lora_modes.dio_irq_mode == LORA_DIO_IRQ_ON)
	{
		sx127x_write_register(
			SX127X_REG_DIO_MAPPING_1,
			SX127X_MAP_DIO0_LORA_RXDONE | SX127X_MAP_DIO1_LORA_NOP | SX127X_MAP_DIO2_LORA_NOP | SX127X_MAP_DIO3_LORA_NOP
		);
	}
	sx127x_write_register(SX127X_REG_IRQ_FLAGS, 0xFF);
	sx127x_write_register(SX127X_REG_IRQ_FLAGS_MASK, ~(SX127X_IRQ_LORA_RXDONE | SX127X_IRQ_LORA_CRCERROR));
	sx127x_write_register(SX127X_REG_FIFO_ADDR_PTR, 0x00);

	sx1278_receiver_spurious_reception_workaround(lora_core.params.freq_in_hz, lora_core.params.bw, true);

	sx1278_rx_start();
	sx127x_set_device_mode(SX127X_OPMODE_MODE_RXCONTINUOUS);
}

//--------------------------------------------
lora_irq_src_t sx1278_irq_poll(void)
{
	uint8_t irq;

	irq = sx127x_read_register(SX127X_REG_IRQ_FLAGS);

	if (irq == SX127X_IRQ_LORA_NONE)
	{
		return LORA_IRQ_NONE;
	}
	if (irq & SX127X_IRQ_LORA_CAD_DETECTED)
	{
		sx127x_write_register(SX127X_REG_IRQ_FLAGS, irq | SX127X_IRQ_LORA_CAD_DETECTED);
		return LORA_IRQ_CAD_DETECTED;
	}
	if (irq & SX127X_IRQ_LORA_CAD_DONE)
	{
		sx127x_write_register(SX127X_REG_IRQ_FLAGS, irq | SX127X_IRQ_LORA_CAD_DONE);
		return LORA_IRQ_CAD_DONE;
	}
	if (irq & SX127X_IRQ_LORA_RXDONE)
	{
		sx127x_write_register(SX127X_REG_IRQ_FLAGS, irq | SX127X_IRQ_LORA_RXDONE);
		return LORA_IRQ_RX_DONE;
	}
	if (irq & SX127X_IRQ_LORA_TXDONE)
	{
		sx127x_write_register(SX127X_REG_IRQ_FLAGS, irq | SX127X_IRQ_LORA_TXDONE);
		return LORA_IRQ_TX_DONE;
	}
	return LORA_IRQ_NONE;
}

//--------------------------------------------
lora_status_t sx1278_read_rx_packet(uint8_t *buf, uint8_t *len, int16_t *rssi, int8_t *snr)
{
	uint8_t irq;

	sx127x_set_device_mode(SX127X_OPMODE_MODE_STDBY);

	if (lora_modes.pkt_len_mode == LORA_PKT_IMPLICIT)
	{
		*len = lora_modes.pkt_len;
	}
	else
	{
		*len = sx127x_read_register(SX127X_REG_RX_NB_BYTES);
	}

	irq = sx127x_read_register(SX127X_REG_IRQ_FLAGS);
	if (irq & SX127X_IRQ_LORA_CRCERROR)
	{
		return LORA_STATUS_ERROR;
	}

	sx127x_read_buf(SX127X_REG_FIFO, buf, *len);
	*snr = (int8_t)sx127x_read_register(SX127X_REG_PKT_SNR_VALUE) / 4;

#if defined E32_433T20D_32100_V3_0 || defined E32_433T20D_10012_V6_0 || defined E32_433T30D_10010_V6_2
	// LF bands
	if (*snr < 0)
	{
		*rssi = SX1278_LF_MIN_RSSI_LEVEL + (int16_t)sx127x_read_register(SX127X_REG_PKT_RSSI_VALUE) + *snr;
	}
	else
	{
		*rssi = SX1278_LF_MIN_RSSI_LEVEL + ((int16_t)sx127x_read_register(SX127X_REG_PKT_RSSI_VALUE))*(16/15);
	}
#elif defined E45_TTL_1W_10024_V2_0
	// HF bands
	if (*snr < 0)
	{
		*rssi = SX1278_HF_MIN_RSSI_LEVEL + (int16_t)sx127x_read_register(SX127X_REG_PKT_RSSI_VALUE) + *snr;
	}
	else
	{
		*rssi = SX1278_HF_MIN_RSSI_LEVEL + ((int16_t)sx127x_read_register(SX127X_REG_PKT_RSSI_VALUE))*(16/15);
	}
#endif
	return LORA_STATUS_OK;
}

//--------------------------------------------
uint32_t sx1278_send_tx_packet(uint8_t *buf, uint8_t len)
{
	float pkt_in_symb;

	pkt_in_symb = lora_get_packet_length_in_symbols(
		lora_core.params.sf,
		lora_core.params.cr,
		len,
		lora_modes.preamble_len,
		lora_modes.crc_mode == LORA_CRC_ON ? true : false,
		lora_modes.pkt_len_mode == LORA_PKT_IMPLICIT ? true : false,
		lora_core.ldro
	);
	uint32_t tx_time = (uint32_t)(pkt_in_symb * lora_core.symbol_length_ms);

	sx127x_set_device_mode(SX127X_OPMODE_MODE_STDBY);

	if (lora_modes.dio_irq_mode == LORA_DIO_IRQ_ON)
	{
		sx127x_write_register(
			SX127X_REG_DIO_MAPPING_1,
			SX127X_MAP_DIO0_LORA_TXDONE | SX127X_MAP_DIO1_LORA_NOP | SX127X_MAP_DIO2_LORA_NOP | SX127X_MAP_DIO3_LORA_NOP
		);
	}
	sx127x_write_register(SX127X_REG_IRQ_FLAGS, 0xFF);
	sx127x_write_register(SX127X_REG_IRQ_FLAGS_MASK, ~(SX127X_IRQ_LORA_TXDONE));
	sx127x_write_register(SX127X_REG_FIFO_ADDR_PTR, 0x00);
	sx127x_write_register(SX127X_REG_PAYLOAD_LENGTH, len);
	sx127x_write_buf(SX127X_REG_FIFO, buf, len);

	sx1278_receiver_spurious_reception_workaround(lora_core.params.freq_in_hz, lora_core.params.bw, false);

	sx1278_tx_start();
	// start to transmit
	sx127x_set_device_mode(SX127X_OPMODE_MODE_TX);

	return tx_time;
}

//--------------------------------------------
// Checks current RSSI in LORA mode
// This does NOT interrupt the LORA receive mode
void sx1278_read_rssi(int16_t *rssi)
{
#if defined E32_433T20D_32100_V3_0 || defined E32_433T20D_10012_V6_0 || defined E32_433T30D_10010_V6_2
	// LF bands
	*rssi = SX1278_LF_MIN_RSSI_LEVEL + (int16_t)sx127x_read_register(SX127X_REG_RSSI_VALUE);
#elif defined E45_TTL_1W_10024_V2_0
	// HF bands
	*rssi = SX1278_HF_MIN_RSSI_LEVEL + (int16_t)sx127x_read_register(SX127X_REG_RSSI_VALUE);
#endif
}
