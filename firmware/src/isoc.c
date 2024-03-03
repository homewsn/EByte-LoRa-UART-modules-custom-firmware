/*
* Copyright (c) 2020, 2022 Vladimir Alemasov
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

#include <stdint.h>		/* uint8_t */
#include <stddef.h>     /* size_t */
#include <string.h>     /* memcpy */
#include "lora-defs.h"
#include "isoc.h"

//------------------------------------------------------------------
int isoc_message_check(const uint8_t *buf, const size_t size)
{
	size_t len;

	if (!size || buf[0] != ISOC_SYNC_CHAR1)
	{
		return -1;
	}
	if (size < ISOC_HEADER_LENGTH)
	{
		return 0;
	}
	if (buf[1] != ISOC_SYNC_CHAR2)
	{
		return -1;
	}
	len = (uint16_t)buf[3] | ((uint16_t)buf[4]) << 8;
	if (size < ISOC_HEADER_LENGTH + len)
	{
		return 0;
	}
	return (int)(ISOC_HEADER_LENGTH + len);
}

//------------------------------------------------------------------
size_t isoc_set_lora_params_cmd(uint8_t *isoc_buf, lora_params_t *params)
{
	*(isoc_buf++) = ISOC_SYNC_CHAR1;
	*(isoc_buf++) = ISOC_SYNC_CHAR2;
	*(isoc_buf++) = ISOC_SET_LORA_PARAMS_CMD;
	*(isoc_buf++) = 7;
	*(isoc_buf++) = 0;
	*(isoc_buf++) = params->freq_in_hz;
	*(isoc_buf++) = params->freq_in_hz >> 8;
	*(isoc_buf++) = params->freq_in_hz >> 16;
	*(isoc_buf++) = params->freq_in_hz >> 24;
	*(isoc_buf++) = params->bw;
	*(isoc_buf++) = params->sf;
	*(isoc_buf++) = params->cr;
	return (ISOC_HEADER_LENGTH + 7);
}

//------------------------------------------------------------------
size_t isoc_req_lora_params_cmd(uint8_t *isoc_buf)
{
	*(isoc_buf++) = ISOC_SYNC_CHAR1;
	*(isoc_buf++) = ISOC_SYNC_CHAR2;
	*(isoc_buf++) = ISOC_REQ_LORA_PARAMS_CMD;
	*(isoc_buf++) = 0;
	*(isoc_buf++) = 0;
	return (ISOC_HEADER_LENGTH);
}

//------------------------------------------------------------------
size_t isoc_req_air_pkt_cmd(uint8_t *isoc_buf)
{
	*(isoc_buf++) = ISOC_SYNC_CHAR1;
	*(isoc_buf++) = ISOC_SYNC_CHAR2;
	*(isoc_buf++) = ISOC_REQ_AIR_PKT_CMD;
	*(isoc_buf++) = 0;
	*(isoc_buf++) = 0;
	return (ISOC_HEADER_LENGTH);
}

//------------------------------------------------------------------
size_t isoc_get_rssi_cmd(uint8_t *isoc_buf)
{
	*(isoc_buf++) = ISOC_SYNC_CHAR1;
	*(isoc_buf++) = ISOC_SYNC_CHAR2;
	*(isoc_buf++) = ISOC_GET_RSSI_CMD;
	*(isoc_buf++) = 0;
	*(isoc_buf++) = 0;
	return (ISOC_HEADER_LENGTH);
}

//------------------------------------------------------------------
size_t isoc_set_rssi_cmd(uint8_t *isoc_buf, int16_t rssi)
{
	*(isoc_buf++) = ISOC_SYNC_CHAR1;
	*(isoc_buf++) = ISOC_SYNC_CHAR2;
	*(isoc_buf++) = ISOC_SET_RSSI_CMD;
	*(isoc_buf++) = 2;
	*(isoc_buf++) = 0;
	*(isoc_buf++) = (uint8_t)rssi;
	*(isoc_buf++) = rssi >> 8;
	return (ISOC_HEADER_LENGTH + 2);
}

//------------------------------------------------------------------
size_t isoc_get_module_name_cmd(uint8_t *isoc_buf)
{
	*(isoc_buf++) = ISOC_SYNC_CHAR1;
	*(isoc_buf++) = ISOC_SYNC_CHAR2;
	*(isoc_buf++) = ISOC_GET_MODULE_NAME_CMD;
	*(isoc_buf++) = 0;
	*(isoc_buf++) = 0;
	return (ISOC_HEADER_LENGTH);
}

//------------------------------------------------------------------
size_t isoc_get_module_name_rsp(uint8_t *isoc_buf, const char *buf)
{
	size_t len = strlen(buf);
	*(isoc_buf++) = ISOC_SYNC_CHAR1;
	*(isoc_buf++) = ISOC_SYNC_CHAR2;
	*(isoc_buf++) = ISOC_GET_MODULE_NAME_RSP;
	*(isoc_buf++) = (uint8_t)len;
	*(isoc_buf++) = (uint8_t)(len >> 8);
	strncpy((char *)isoc_buf, buf, len);
	return (ISOC_HEADER_LENGTH + len);
}

//------------------------------------------------------------------
size_t isoc_get_loracore_name_cmd(uint8_t *isoc_buf)
{
	*(isoc_buf++) = ISOC_SYNC_CHAR1;
	*(isoc_buf++) = ISOC_SYNC_CHAR2;
	*(isoc_buf++) = ISOC_GET_LORACORE_NAME_CMD;
	*(isoc_buf++) = 0;
	*(isoc_buf++) = 0;
	return (ISOC_HEADER_LENGTH);
}

//------------------------------------------------------------------
size_t isoc_get_loracore_name_rsp(uint8_t *isoc_buf, const char *buf)
{
	size_t len = strlen(buf);
	*(isoc_buf++) = ISOC_SYNC_CHAR1;
	*(isoc_buf++) = ISOC_SYNC_CHAR2;
	*(isoc_buf++) = ISOC_GET_LORACORE_NAME_RSP;
	*(isoc_buf++) = (uint8_t)len;
	*(isoc_buf++) = (uint8_t)(len >> 8);
	strncpy((char *)isoc_buf, buf, len);
	return (ISOC_HEADER_LENGTH + len);
}

//------------------------------------------------------------------
size_t isoc_air_pkt_cmd(uint8_t *isoc_buf, const uint8_t *buf, const uint8_t size)
{
	size_t len = size + ISOC_RSSI_SNR_LENGTH;
	*(isoc_buf++) = ISOC_SYNC_CHAR1;
	*(isoc_buf++) = ISOC_SYNC_CHAR2;
	*(isoc_buf++) = ISOC_AIR_PKT_CMD;
	*(isoc_buf++) = (uint8_t)len;
	*(isoc_buf++) = (uint8_t)(len >> 8);
	memcpy(isoc_buf, buf, size);
	isoc_buf += size;
	*(isoc_buf++) = 0;
	*(isoc_buf++) = 0;
	*(isoc_buf++) = 0;
	return (ISOC_HEADER_LENGTH + len);
}

//------------------------------------------------------------------
size_t isoc_air_pkt_cmd_rssi_snr(uint8_t *isoc_buf, const uint8_t *buf, const uint8_t size, int16_t rssi, uint8_t snr)
{
	size_t len = size + ISOC_RSSI_SNR_LENGTH;
	*(isoc_buf++) = ISOC_SYNC_CHAR1;
	*(isoc_buf++) = ISOC_SYNC_CHAR2;
	*(isoc_buf++) = ISOC_AIR_PKT_CMD;
	*(isoc_buf++) = (uint8_t)len;
	*(isoc_buf++) = (uint8_t)(len >> 8);
	memcpy(isoc_buf, buf, size);
	isoc_buf += size;
	*(isoc_buf++) = (uint8_t)rssi;
	*(isoc_buf++) = (uint8_t)(rssi >> 8);
	*(isoc_buf++) = snr;
	return (ISOC_HEADER_LENGTH + len);
}

//------------------------------------------------------------------
size_t isoc_set_lora_modes_cmd(uint8_t *isoc_buf, lora_modes_t *modes)
{
	*(isoc_buf++) = ISOC_SYNC_CHAR1;
	*(isoc_buf++) = ISOC_SYNC_CHAR2;
	*(isoc_buf++) = ISOC_SET_LORA_MODES_CMD;
	*(isoc_buf++) = 8;
	*(isoc_buf++) = 0;
	*(isoc_buf++) = modes->pkt_len_mode;
	*(isoc_buf++) = modes->pkt_len;
	*(isoc_buf++) = modes->dio_irq_mode;
	*(isoc_buf++) = modes->crc_mode;
	*(isoc_buf++) = modes->iq_mode;
	*(isoc_buf++) = (uint8_t)modes->preamble_len;
	*(isoc_buf++) = (uint8_t)(modes->preamble_len >> 8);
	*(isoc_buf++) = modes->sync_word;
	return (ISOC_HEADER_LENGTH + 8);
}

//------------------------------------------------------------------
size_t isoc_req_lora_modes_cmd(uint8_t *isoc_buf)
{
	*(isoc_buf++) = ISOC_SYNC_CHAR1;
	*(isoc_buf++) = ISOC_SYNC_CHAR2;
	*(isoc_buf++) = ISOC_REQ_LORA_MODES_CMD;
	*(isoc_buf++) = 0;
	*(isoc_buf++) = 0;
	return (ISOC_HEADER_LENGTH);
}
