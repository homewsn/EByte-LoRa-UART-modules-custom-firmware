#ifndef SX1278_DEFS_H_
#define SX1278_DEFS_H_

// LoRa registers
#define SX1278_REG_FIFO                    0x00    // LoRa base-band FIFO data input/output. FIFO is cleared an not accessible when device is in SLEEP mode
#define SX1278_REG_OPMODE                  0x01    // Opmodes
#define SX1278_REG_FR_MSB                  0x06    // Frf(23:16) MSB of RF carrier frequency
#define SX1278_REG_FR_MID                  0x07    // Frf(15:8) MSB of RF carrier frequency
#define SX1278_REG_FR_LSB                  0x08    // Frf(7:0) LSB of RF carrier frequency
#define SX1278_REG_PA_CONFIG               0x09    // 
#define SX1278_REG_PA_RAMP                 0x0A    // Rise/Fall time of ramp up/down in FSK
#define SX1278_REG_LNA                     0x0C    // Low-noise amplifier
#define SX1278_REG_FIFO_ADDR_PTR           0x0D    // SPI interface address pointer in FIFO data buffer
#define SX1278_REG_FIFO_TX_BASE_ADDR       0x0E    // Start Tx data
#define SX1278_REG_FIFO_RX_BASE_ADDR       0x0F    // Start Rx data
#define SX1278_REG_FIFO_RX_CURRENT_ADDR    0x10    // Start address (in data buffer) of last packet received
#define SX1278_REG_IRQ_FLAGS_MASK          0x11    // 
#define SX1278_REG_IRQ_FLAGS               0x12    // 
#define SX1278_REG_RX_NB_BYTES             0x13    // Number of payload bytes of latest packet received
#define SX1278_REG_PKT_SNR_VALUE           0x19    // Estimation of SNR on last packet received
#define SX1278_REG_PKT_RSSI_VALUE          0x1A    // RSSI of the latest packet received (dBm)
#define SX1278_REG_RSSI_VALUE              0x1B    // Current RSSI value (dBm)
#define SX1278_REG_HOP_CHANNEL             0x1C    // 
#define SX1278_REG_MODEM_CONFIG1           0x1D    // 
#define SX1278_REG_MODEM_CONFIG2           0x1E    // 
#define SX1278_REG_SYMB_TIMEOUT_LSB        0x1F    // 
#define SX1278_REG_PREAMBLE_MSB            0x20    // Preamble length MSB
#define SX1278_REG_PREAMBLE_LSB            0x21    // Preamble Length LSB
#define SX1278_REG_PAYLOAD_LENGTH          0x22    // Payload length in bytes. The register needs to be set in implicit header mode for the expected packet length. A 0 value is not permitted
#define SX1278_REG_MAX_PAYLOAD_LENGTH      0x23    // Maximum payload length; if header payload length exceeds value a header CRC error is generated. Allows filtering of packet with a bad size.
#define SX1278_REG_MODEM_CONFIG3           0x26    // 
#define SX1278_REG_DETECT_OPTIMIZE         0x31    // 
#define SX1278_REG_DETECT_THRESHOLD        0x37    // 
#define SX1278_REG_SYNC_WORD               0x39    // LoRa Sync Word. Value 0x34 is reserved for LoRaWAN networks
#define SX1278_REG_DIO_MAPPING_1           0x40    // Mapping of pins DIO0 to DIO3
#define SX1278_REG_VERSION                 0x42    // Version code of the chip
#define SX1278_REG_PA_DAC                  0x4D    // High Power +20 dBm (100 mW) Operation
#define SX1278_REG_PLL_LF                  0x70    // LF PLL bandwidth

// FSK/OOK registers
#define SX1278_REG_RSSIVALUE               0x11    // Absolute value of the RSSI in dBm, 0.5dB steps. RSSI = - RssiValue/2 [dBm]
#define SX1278_REG_RXBW                    0x12    // Channel filter bandwidth control
#define SX1278_REG_AFCBW                   0x13    // Channel filter bandwidth control used during the AFC

#define SX1278_OPMODE_LORA                 0x80    // LoRa Mode
#define SX1278_OPMODE_FSK                  0x00    // FSK/OOK Mode

#define SX1278_OPMODE_ACCESS_LORA          0x00    // Access LoRa registers page 0x0D: 0x3F
#define SX1278_OPMODE_ACCESS_FSK           0x40    // Access FSK registers page (in mode LoRa) 0x0D: 0x3F

#define SX1278_OPMODE_LFM_ON               0x08    // Low Frequency Mode (access to LF test registers)
#define SX1278_OPMODE_LFM_OFF              0x00    // High Frequency Mode (access to HF test registers)

#define SX1278_OPMODE_MODE_MASK            0x07    // Device modes:
#define SX1278_OPMODE_MODE_SLEEP           0x00    // Sleep
#define SX1278_OPMODE_MODE_STDBY           0x01    // Standby
#define SX1278_OPMODE_MODE_FSTX            0x02    // Frequency synthesis TX
#define SX1278_OPMODE_MODE_TX              0x03    // Transmit
#define SX1278_OPMODE_MODE_FSRX            0x04    // Frequency synthesis RX
#define SX1278_OPMODE_MODE_RXCONTINUOUS    0x05    // Receive continuous
#define SX1278_OPMODE_MODE_RXSINGLE        0x06    // Receive single
#define SX1278_OPMODE_MODE_CAD             0x07    // Channel activity detection

#define SX1278_HOP_CHANNEL_CRC             0x40    // CRC Information extracted from the received packet header (Explicit header mode only)

#define SX1278_MODEM_CONFIG1_BW_MASK       0xF0    // Signal bandwidth:
#define SX1278_MODEM_CONFIG1_BW_7          0x00    // 7.8 kHz
#define SX1278_MODEM_CONFIG1_BW_10         0x10    // 10.4 kHz
#define SX1278_MODEM_CONFIG1_BW_15         0x20    // 15.6 kHz
#define SX1278_MODEM_CONFIG1_BW_20         0x30    // 20.8 kHz
#define SX1278_MODEM_CONFIG1_BW_31         0x40    // 31.25 kHz
#define SX1278_MODEM_CONFIG1_BW_41         0x50    // 41.7 kHz
#define SX1278_MODEM_CONFIG1_BW_62         0x60    // 62.5 kHz
#define SX1278_MODEM_CONFIG1_BW_125        0x70    // 125 kHz
#define SX1278_MODEM_CONFIG1_BW_250        0x80    // 250 kHz
#define SX1278_MODEM_CONFIG1_BW_500        0x90    // 500 kHz
#define SX1278_MODEM_CONFIG1_CR_MASK       0x0E    // Error coding rate:
#define SX1278_MODEM_CONFIG1_CR_4_5        0x02    // 4/5
#define SX1278_MODEM_CONFIG1_CR_4_6        0x04    // 4/6
#define SX1278_MODEM_CONFIG1_CR_4_7        0x06    // 4/7
#define SX1278_MODEM_CONFIG1_CR_4_8        0x08    // 4/8
#define SX1278_MODEM_CONFIG1_HEADER_MASK   0x01    // Header mode:
#define SX1278_MODEM_CONFIG1_HEADER_EXPL   0x00    // Explicit Header
#define SX1278_MODEM_CONFIG1_HEADER_IMPL   0x01    // Implicit Header

#define SX1278_MODEM_CONFIG2_SF_MASK       0xF0    // SF rate:
#define SX1278_MODEM_CONFIG2_SF6           0x60    // 64 chips / symbol
#define SX1278_MODEM_CONFIG2_SF7           0x70    // 128 chips / symbol
#define SX1278_MODEM_CONFIG2_SF8           0x80    // 256 chips / symbol
#define SX1278_MODEM_CONFIG2_SF9           0x90    // 512 chips / symbol
#define SX1278_MODEM_CONFIG2_SF10          0xA0    // 1024 chips / symbol
#define SX1278_MODEM_CONFIG2_SF11          0xB0    // 2048 chips / symbol
#define SX1278_MODEM_CONFIG2_SF12          0xC0    // 4096 chips / symbol
#define SX1278_MODEM_CONFIG2_CRC_MASK      0x04    // Enable CRC generation and check on payload:
#define SX1278_MODEM_CONFIG2_CRC_OFF       0x00    // CRC disable
#define SX1278_MODEM_CONFIG2_CRC_ON        0x04    // CRC enable
#define SX1278_MODEM_CONFIG2_TO_MASK       0x03    // RX Time-Out MSB

#define SX1278_MODEM_CONFIG3_LDRO_MASK     0x08    // LowDataRateOptimize:
#define SX1278_MODEM_CONFIG3_LDRO_OFF      0x00    // Disabled
#define SX1278_MODEM_CONFIG3_LDRO_ON       0x08    // Enabled; mandated for when the symbol length exceeds 16ms
#define SX1278_MODEM_CONFIG3_AGCAUTO_MASK  0x04    // AgcAutoOn:
#define SX1278_MODEM_CONFIG3_AGCAUTO_OFF   0x00    // LNA gain set by register LnaGain
#define SX1278_MODEM_CONFIG3_AGCAUTO_ON    0x04    // LNA gain set by the internal AGC loop

#define SX1278_LNA_MAX_GAIN_BOOST_LF       0x20    // Maximum gain (Default settings)
#define SX1278_LNA_MAX_GAIN_BOOST_HF       0x20 | 0x03 // Maximum gain + Boost on, 150% LNA current

#define SX1278_PA_RAMP_MASK                0x0F    // Rise/Fall time of ramp up/down in FSK:
#define SX1278_PA_RAMP_3400                0x00    // 3.4 ms
#define SX1278_PA_RAMP_2000                0x01    // 2 ms
#define SX1278_PA_RAMP_1000                0x02    // 1 ms
#define SX1278_PA_RAMP_500                 0x03    // 500 us
#define SX1278_PA_RAMP_250                 0x04    // 250 us
#define SX1278_PA_RAMP_125                 0x05    // 125 us
#define SX1278_PA_RAMP_100                 0x06    // 100 us
#define SX1278_PA_RAMP_62                  0x07    // 62 us
#define SX1278_PA_RAMP_50                  0x08    // 50 us
#define SX1278_PA_RAMP_40                  0x09    // 40 us
#define SX1278_PA_RAMP_31                  0x0A    // 31 us
#define SX1278_PA_RAMP_25                  0x0B    // 25 us
#define SX1278_PA_RAMP_20                  0x0C    // 20 us
#define SX1278_PA_RAMP_15                  0x0D    // 15 us
#define SX1278_PA_RAMP_12                  0x0E    // 12 us
#define SX1278_PA_RAMP_10                  0x0F    // 10 us

#define SX1278_PA_DAC_20                   0x87    // High Power +20 dBm (100 mW) Operation 
#define SX1278_PA_DAC_NORMAL               0x84    // Default value PA_HF/LF or +17 dBm

// DIO function mappings                              D0D1D2D3
#define SX1278_MAP_DIO0_LORA_RXDONE        0x00    // 00------
#define SX1278_MAP_DIO0_LORA_TXDONE        0x40    // 01------
#define SX1278_MAP_DIO1_LORA_NOP           0x30    // --11----
#define SX1278_MAP_DIO2_LORA_NOP           0x0C    // ----11--
#define SX1278_MAP_DIO3_LORA_NOP           0x0C    // ------11

#define SX1278_IRQ_LORA_NONE               0x00
#define SX1278_IRQ_LORA_CAD_DETECTED       0x01    // CAD detected interrupt mask
#define SX1278_IRQ_LORA_CAD_DONE           0x04    // CAD complete interrupt mask
#define SX1278_IRQ_LORA_TXDONE             0x08    // FIFO Payload transmission complete interrupt mask
#define SX1278_IRQ_LORA_VALIDHEADER        0x10    // Valid header received interrupt mask
#define SX1278_IRQ_LORA_CRCERROR           0x20    // Payload CRC error interrupt mask
#define SX1278_IRQ_LORA_RXDONE             0x40    // Packet reception complete interrupt mask

#define SX1278_PLL_LF_BANDWIDTH_MASK       0xC0    // LF PLL bandwidth:
#define SX1278_PLL_LF_BANDWIDTH_75         0x00    // 75 kHz
#define SX1278_PLL_LF_BANDWIDTH_150        0x40    // 150 kHz
#define SX1278_PLL_LF_BANDWIDTH_225        0x80    // 225 kHz
#define SX1278_PLL_LF_BANDWIDTH_300        0xC0    // 300 kHz

#define SX1278_DETECT_OPTIMIZE_MASK        0x07    // Detection optimize:
#define SX1278_DETECT_OPTIMIZE_SF6         0x05    // SF6
#define SX1278_DETECT_OPTIMIZE_DEF         0x03    // SF7 - SF12

#define SX1278_DETECT_THRESHOLD_SF6        0x0C    // SF6
#define SX1278_DETECT_THRESHOLD_DEF        0x0A    // SF7 - SF12

#endif // SX1278_DEFS_H_