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

#ifndef ISOC_H_
#define ISOC_H_

//------------------------------------------------------------------
#define ISOC_SYNC_CHAR1               0x40
#define ISOC_SYNC_CHAR2               0x53
#define ISOC_SET_LORA_PARAMS_CMD      0x01
#define ISOC_REQ_LORA_PARAMS_CMD      0x02
#define ISOC_REQ_AIR_PKT_CMD          0x03
#define ISOC_GET_RSSI_CMD             0x04
#define ISOC_SET_RSSI_CMD             0x05
#define ISOC_GET_MODULE_NAME_CMD      0x06
#define ISOC_GET_MODULE_NAME_RSP      0x07
#define ISOC_GET_LORACORE_NAME_CMD    0x08
#define ISOC_GET_LORACORE_NAME_RSP    0x09
#define ISOC_SET_LORA_MODES_CMD       0x0A
#define ISOC_REQ_LORA_MODES_CMD       0x0B
#define ISOC_AIR_PKT_CMD              0x80
#define ISOC_HEADER_LENGTH            5
#define ISOC_MAX_AIRPKT_LENGTH        255
#define ISOC_RSSI_SNR_LENGTH          3
#define ISOC_MAX_MSG_LENGTH          (ISOC_HEADER_LENGTH + ISOC_MAX_AIRPKT_LENGTH + ISOC_RSSI_SNR_LENGTH)
#define ISOC_BUF_LENGTH              (ISOC_MAX_MSG_LENGTH)

//------------------------------------------------------------------
int isoc_message_check(const uint8_t *buf, const size_t size);
size_t isoc_set_lora_params_cmd(uint8_t *isoc_buf, lora_params_t *params);
size_t isoc_req_lora_params_cmd(uint8_t *isoc_buf);
size_t isoc_req_air_pkt_cmd(uint8_t *isoc_buf);
size_t isoc_get_rssi_cmd(uint8_t *isoc_buf);
size_t isoc_set_rssi_cmd(uint8_t *isoc_buf, int16_t rssi);
size_t isoc_get_module_name_cmd(uint8_t *isoc_buf);
size_t isoc_get_module_name_rsp(uint8_t *isoc_buf, const char *buf);
size_t isoc_get_loracore_name_cmd(uint8_t *isoc_buf);
size_t isoc_get_loracore_name_rsp(uint8_t *isoc_buf, const char *buf);
size_t isoc_air_pkt_cmd_rssi_snr(uint8_t *isoc_buf, const uint8_t *buf, const uint8_t size, int16_t rssi, uint8_t snr);
size_t isoc_air_pkt_cmd(uint8_t *isoc_buf, const uint8_t *buf, const uint8_t size);
size_t isoc_set_lora_modes_cmd(uint8_t *isoc_buf, lora_modes_t *modes);
size_t isoc_req_lora_modes_cmd(uint8_t *isoc_buf);

#endif // ISOC_H_
