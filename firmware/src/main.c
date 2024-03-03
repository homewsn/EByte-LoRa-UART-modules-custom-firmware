/*
* Copyright (c) 2022, 2023 Vladimir Alemasov
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
#include "hal-io.h"
#include "hal-uart.h"
#include "lora-defs.h"
#include "lora-drv.h"
#include "isoc.h"

//--------------------------------------------
extern const lora_drv_t lora_drv;

//--------------------------------------------
// HOMEWSN_DLS project
#define LORA_SYNC_WORD_HOMEWSN_DLS       LORA_SYNC_WORD_LORAWAN_PRIVATE
#define LORA_PREAMBLE_LEN_HOMEWSN_DLS    8
#define ISOC_AIRPKT_LENGTH_HOMEWSN_DLS   48

//--------------------------------------------
#define LED_INTERVAL_MS         250
#define MAX_INIT_COUNTER        8

//--------------------------------------------
#define QUEUE_TEST              1

//--------------------------------------------
static uint8_t uart_tx_buf[ISOC_BUF_LENGTH];
static size_t uart_tx_len;
static uint8_t uart_rx_buf[ISOC_BUF_LENGTH];
static size_t uart_rx_cnt;
static uint8_t isoc_rx_buf[ISOC_BUF_LENGTH];
static size_t isoc_rx_len;
static uint8_t radio_tx_buf[ISOC_MAX_AIRPKT_LENGTH];
static uint8_t radio_tx_len;
static uint8_t radio_rx_buf[ISOC_MAX_AIRPKT_LENGTH];
static uint8_t radio_rx_len;
static lora_irq_src_t irq_src;
static int16_t rssi;
static int16_t rssi_channel_busy = DEF_CHANNEL_BUSY_RSSI;
static int8_t snr;
static bool uart_rx_flag;
static uint8_t radio_tx_flag;
static uint8_t radio_rxon_flag;
static uint8_t radio_init_flag;
static uint8_t radio_rssi_flag;
static bool radio_modes_flag;
static uint8_t led_state;
static uint32_t led_cnt;
static uint32_t init_cnt;
static uint32_t radio_tx_cnt;
static uint32_t radio_tx_time;

//--------------------------------------------
static uint32_t get_time_interval(uint32_t start)
{
	uint32_t cnt = get_platform_counter();
	uint32_t diff;

	if (cnt < start)
	{
		diff = 0xFFFFFFFF - start + cnt;
	}
	else
	{
		diff = cnt - start;
	}

	return diff;
}

//--------------------------------------------
static void isoc_rx_feed(void)
{
	int len;

	if (!uart_rx_cnt)
	{
		return;
	}
	len = isoc_message_check(uart_rx_buf, uart_rx_cnt);
	if (len < 0)
	{
		uart_rx_cnt = 0;
	}
	if (len > 0 && !isoc_rx_len)
	{
		isoc_rx_len = len;
		memcpy(isoc_rx_buf, uart_rx_buf, isoc_rx_len);
		if (uart_rx_cnt > len)
		{
			memcpy(uart_rx_buf, uart_rx_buf + len, uart_rx_cnt - len);
			uart_rx_cnt -= len;
		}
		else
		{
			uart_rx_cnt = 0;
		}
	}
}

//--------------------------------------------
static void uart_rx_callback(uint8_t byte)
{
	uart_rx_flag = true;
	if (uart_rx_cnt == sizeof(uart_rx_buf))
	{
		uart_rx_cnt = 0;
	}
	uart_rx_buf[uart_rx_cnt++] = byte;
	isoc_rx_feed();
	uart_rx_flag = false;
}

//--------------------------------------------
#define LED_ON   0
#define LED_OFF  1

//--------------------------------------------
void main(void)
{
	lora_modes_t modes =
	{
		LORA_PKT_IMPLICIT,
		ISOC_AIRPKT_LENGTH_HOMEWSN_DLS,
		LORA_DIO_IRQ_ON,
		LORA_CRC_ON,
		LORA_IQ_STANDARD,
		LORA_PREAMBLE_LEN_HOMEWSN_DLS,
		LORA_SYNC_WORD_HOMEWSN_DLS
	};

	platform_init();
	lora_drv.init(&modes);
	radio_modes_flag = true;
	hal_io_init();
	hal_uart_init(uart_rx_callback);

	// start yellow LED blinking (waiting for valid ISOC_SET_LORA_PARAMS_CMD)
	led_state = LED_ON;
	led_cnt = get_platform_counter();
	hal_io_m0(led_state);

	for (;;)
	{
		if (!radio_rxon_flag && !radio_tx_flag && radio_init_flag)
		{
			hal_io_m0(LED_OFF);
			lora_drv.start_continious_rx();
			radio_rxon_flag = 1;
		}
		if (isoc_rx_len)
		{
			switch (isoc_rx_buf[2])
			{
			case ISOC_SET_LORA_PARAMS_CMD:
			{
				lora_params_t params =
				{
					isoc_rx_buf[ISOC_HEADER_LENGTH] |
						((uint32_t)isoc_rx_buf[ISOC_HEADER_LENGTH + 1] << 8) |
						((uint32_t)isoc_rx_buf[ISOC_HEADER_LENGTH + 2] << 16) |
						((uint32_t)isoc_rx_buf[ISOC_HEADER_LENGTH + 3] << 24),
					(lora_bw_t)isoc_rx_buf[ISOC_HEADER_LENGTH + 4],
					(lora_sf_t)isoc_rx_buf[ISOC_HEADER_LENGTH + 5],
					(lora_cr_t)isoc_rx_buf[ISOC_HEADER_LENGTH + 6]
				};
				if (!radio_modes_flag)
				{
					// send message to host - valid ISOC_SET_LORA_MODES_CMD command is needed before ISOC_SET_LORA_PARAMS_CMD
					uart_tx_len = isoc_req_lora_modes_cmd(uart_tx_buf);
					break;
				}
				if (lora_drv.set_params(&params) != LORA_STATUS_OK)
				{
					radio_init_flag = 0;
					// start yellow LED blinking (waiting for valid ISOC_SET_LORA_PARAMS_CMD)
					led_state = LED_ON;
					led_cnt = get_platform_counter();
					hal_io_m0(led_state);
				}
				else
				{
					radio_tx_len = 0;
					radio_init_flag = 1;
					radio_rxon_flag = 0;
					// send message to host - ISOC_REQ_AIR_PKT_CMD command
					uart_tx_len = isoc_req_air_pkt_cmd(uart_tx_buf);
				}
				break;
			}
			case ISOC_AIR_PKT_CMD:
				if (!radio_init_flag)
				{
					if (!radio_modes_flag)
					{
						// send message to host - valid ISOC_SET_LORA_MODES_CMD command is needed before ISOC_AIR_PKT_CMD
						uart_tx_len = isoc_req_lora_modes_cmd(uart_tx_buf);
					}
					else
					{
						// send message to host - ISOC_SET_LORA_PARAMS_CMD command is needed before ISOC_AIR_PKT_CMD
						uart_tx_len = isoc_req_lora_params_cmd(uart_tx_buf);
					}
				}
				else
				{
					if (!radio_tx_len)
					{
						radio_tx_len = (uint8_t)((uint16_t)isoc_rx_buf[3] | (uint16_t)(isoc_rx_buf[4] << 8)) - ISOC_RSSI_SNR_LENGTH;
						memcpy(radio_tx_buf, &isoc_rx_buf[ISOC_HEADER_LENGTH], radio_tx_len);
					}
				}
				break;
			case ISOC_GET_RSSI_CMD:
				if (!radio_init_flag)
				{
					if (!radio_modes_flag)
					{
						// send message to host - valid ISOC_SET_LORA_MODES_CMD command is needed before ISOC_GET_RSSI_CMD
						uart_tx_len = isoc_req_lora_modes_cmd(uart_tx_buf);
					}
					else
					{
						// send message to host - ISOC_SET_LORA_PARAMS_CMD command is needed before ISOC_GET_RSSI_CMD
						uart_tx_len = isoc_req_lora_params_cmd(uart_tx_buf);
					}
				}
				else
				{
					lora_drv.read_rssi(&rssi);
					uart_tx_len = isoc_set_rssi_cmd(uart_tx_buf, rssi);
				}
				break;
			case ISOC_SET_RSSI_CMD:
				rssi_channel_busy = (int16_t)isoc_rx_buf[ISOC_HEADER_LENGTH];
				break;
			case ISOC_GET_MODULE_NAME_CMD:
			{
				const char *name = lora_drv.get_module_name();
				uart_tx_len = isoc_get_module_name_rsp(uart_tx_buf, name);
				break;
			}
			case ISOC_GET_LORACORE_NAME_CMD:
			{
				const char *name = lora_drv.get_core_name();
				uart_tx_len = isoc_get_loracore_name_rsp(uart_tx_buf, name);
				break;
			}
			case ISOC_SET_LORA_MODES_CMD:
			{
				lora_modes_t modes =
				{
					(lora_pkt_len_modes_t)isoc_rx_buf[ISOC_HEADER_LENGTH],
					isoc_rx_buf[ISOC_HEADER_LENGTH + 1],
					(lora_dio_irq_modes_t)isoc_rx_buf[ISOC_HEADER_LENGTH + 2],
					(lora_crc_modes_t)isoc_rx_buf[ISOC_HEADER_LENGTH + 3],
					(lora_iq_modes_t)isoc_rx_buf[ISOC_HEADER_LENGTH + 4],
					((uint16_t)isoc_rx_buf[ISOC_HEADER_LENGTH + 5] | (uint16_t)(isoc_rx_buf[ISOC_HEADER_LENGTH + 6] << 8)),
					isoc_rx_buf[ISOC_HEADER_LENGTH + 7]
				};
				if (lora_drv.init(&modes) != LORA_STATUS_OK)
				{
					radio_modes_flag = false;
					// send message to host - valid ISOC_SET_LORA_MODES_CMD command is needed
					uart_tx_len = isoc_req_lora_modes_cmd(uart_tx_buf);
				}
				else
				{
					radio_modes_flag = true;
				}
				radio_init_flag = 0;
				// start yellow LED blinking (waiting for valid ISOC_SET_LORA_PARAMS_CMD / ISOC_SET_LORA_MODES_CMD)
				led_state = LED_ON;
				led_cnt = get_platform_counter();
				hal_io_m0(led_state);
				break;
			}
			default:
				break;
			}
			isoc_rx_len = 0;
#if QUEUE_TEST
			disableInterrupts();
			if (!uart_rx_flag)
			{
				isoc_rx_feed();
			}
			enableInterrupts();
#endif
		}
		if (radio_init_flag)
		{
			irq_src = lora_drv.irq_poll();
			switch (irq_src)
			{
			case LORA_IRQ_RX_DONE:
				hal_io_m0(LED_ON);
				if (lora_drv.read_rx_packet(radio_rx_buf, &radio_rx_len, &rssi, &snr) != LORA_STATUS_OK)
				{
					radio_rx_len = 0;
				}
				radio_rxon_flag = 0;
				break;
			case LORA_IRQ_TX_DONE:
				radio_tx_flag = 0;
				break;
			}
		}
		if (radio_rx_len)
		{
			if (!uart_tx_len)
			{
				uart_tx_len = isoc_air_pkt_cmd_rssi_snr(uart_tx_buf, radio_rx_buf, radio_rx_len, rssi, snr);
				radio_rx_len = 0;
			}
		}
		if (uart_tx_len)
		{
			hal_uart_tx(uart_tx_buf, uart_tx_len);
			uart_tx_len = 0;
		}
		if (radio_tx_flag)
		{
			uint32_t diff = get_time_interval(radio_tx_cnt);
			if (diff < radio_tx_time * 2)
			{
				continue;
			}
#if 1
			// Radio transmission too long
			// Reset MCU
			system_reset();
#endif
		}
		if (radio_tx_len)
		{
			lora_drv.read_rssi(&rssi);
			if (rssi > rssi_channel_busy)
			{
				// The channel is busy
				// It can be a packet addressed to anyone
				radio_rssi_flag = 1;
				continue;
			}
			if (radio_rssi_flag)
			{
				// The channel is free already
				radio_rssi_flag = 0;
				// Give a chance to complete the reception of the packet if it's for me
				delay_ms(100);
				continue;
			}
			// The channel is ready to transmit
			radio_rxon_flag = 0;
			hal_io_m0(LED_ON);
#if 1
			// Radio transmission
			radio_tx_time = lora_drv.send_tx_packet(radio_tx_buf, radio_tx_len);
			radio_tx_flag = 1;
			radio_tx_cnt = get_platform_counter();
#else
			// Loopback test
			memcpy(radio_rx_buf, radio_tx_buf, radio_tx_len);
			radio_rx_len = radio_tx_len;
#endif
			radio_tx_len = 0;
		}
		if (!radio_init_flag)
		{
			if (get_time_interval(led_cnt) > LED_INTERVAL_MS)
			{
				led_cnt = get_platform_counter();
				led_state = led_state == LED_ON ? LED_OFF : LED_ON;
				hal_io_m0(led_state);
				init_cnt++;
			}
			if (init_cnt > MAX_INIT_COUNTER)
			{
				init_cnt = 0;
				if (!radio_modes_flag)
				{
					// send message to host - valid ISOC_SET_LORA_MODES_CMD command is needed
					uart_tx_len = isoc_req_lora_modes_cmd(uart_tx_buf);
				}
				else
				{
					// send message to host - valid ISOC_SET_LORA_PARAMS_CMD command is needed
					uart_tx_len = isoc_req_lora_params_cmd(uart_tx_buf);
				}
			}
		}
	}
}
