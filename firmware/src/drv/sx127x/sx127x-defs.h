#ifndef SX127X_DEFS_H_
#define SX127X_DEFS_H_

// LoRa registers
#define SX127X_REG_FIFO                    0x00    // FIFO read/write access
#define SX127X_REG_OPMODE                  0x01    // Operating mode & LoRa / FSK selection
#define SX127X_REG_FR_MSB                  0x06    // RF Carrier Frequency, Most Significant Bits
#define SX127X_REG_FR_MID                  0x07    // RF Carrier Frequency, Intermediate Bits
#define SX127X_REG_FR_LSB                  0x08    // RF Carrier Frequency, Least Significant Bits
#define SX127X_REG_PA_CONFIG               0x09    // PA selection and Output Power control
#define SX127X_REG_FIFO_ADDR_PTR           0x0D    // FIFO SPI pointer
#define SX127X_REG_FIFO_TX_BASE_ADDR       0x0E    // Start Tx data
#define SX127X_REG_FIFO_RX_BASE_ADDR       0x0F    // Start Rx data
#define SX127X_REG_IRQ_FLAGS_MASK          0x11    // Optional IRQ flag mask
#define SX127X_REG_IRQ_FLAGS               0x12    // IRQ flags
#define SX127X_REG_RX_NB_BYTES             0x13    // Number of received bytes
#define SX127X_REG_PKT_SNR_VALUE           0x19    // Espimation of last packet SNR
#define SX127X_REG_PKT_RSSI_VALUE          0x1A    // RSSI of last packet
#define SX127X_REG_RSSI_VALUE              0x1B    // Current RSSI
#define SX127X_REG_MODEM_CONFIG1           0x1D    // Modem PHY config 1
#define SX127X_REG_MODEM_CONFIG2           0x1E    // Modem PHY config 2
#define SX127X_REG_PREAMBLE_MSB            0x20    // Size of preamble, Most Significant Bits
#define SX127X_REG_PREAMBLE_LSB            0x21    // Size of preamble, Least Significant Bits
#define SX127X_REG_PAYLOAD_LENGTH          0x22    // LoRa payload length
#define SX1278_REG_MODEM_CONFIG3           0x26    // Modem PHY config 3
#define SX127X_REG_DETECT_OPTIMIZE         0x31    // LoRa detection Optimize for SF6
#define SX127X_REG_DETECT_THRESHOLD        0x37    // LoRa detection threshold for SF6
#define SX127X_REG_SYNC_WORD               0x39    // LoRa Sync Word
#define SX127X_REG_DIO_MAPPING_1           0x40    // Mapping of pins DIO0 to DIO3
#define SX127X_REG_VERSION                 0x42    // Semtech ID relating the silicon revision
#define SX1278_REG_PA_DAC                  0x4D    // Higher power settings of the PA
#define SX1272_REG_PA_DAC                  0x5A    // Higher power settings of the PA

#define SX127X_OPMODE_FSK                  0x00    // FSK/OOK Mode
#define SX127X_OPMODE_LORA                 0x80    // LoRa Mode

#define SX127X_OPMODE_ACCESS_LORA          0x00    // Access LoRa registers page 0x0D: 0x3F
#define SX127X_OPMODE_ACCESS_FSK           0x40    // Access FSK registers page (in mode LoRa) 0x0D: 0x3F

#define SX1278_OPMODE_LFM_ON               0x08    // Low Frequency Mode (access to LF test registers)
#define SX1278_OPMODE_LFM_OFF              0x00    // High Frequency Mode (access to HF test registers)

#define SX127X_OPMODE_MODE_MASK            0x07    // Device modes:
#define SX127X_OPMODE_MODE_SLEEP           0x00    // Sleep
#define SX127X_OPMODE_MODE_STDBY           0x01    // Standby
#define SX127X_OPMODE_MODE_FSTX            0x02    // Frequency synthesis TX
#define SX127X_OPMODE_MODE_TX              0x03    // Transmit
#define SX127X_OPMODE_MODE_FSRX            0x04    // Frequency synthesis RX
#define SX127X_OPMODE_MODE_RXCONTINUOUS    0x05    // Receive continuous
#define SX127X_OPMODE_MODE_RXSINGLE        0x06    // Receive single
#define SX127X_OPMODE_MODE_CAD             0x07    // Channel activity detection

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

#define SX1272_MODEM_CONFIG1_BW_MASK       0xC0    // Signal bandwidth:
#define SX1272_MODEM_CONFIG1_BW_125        0x00    // 125 kHz
#define SX1272_MODEM_CONFIG1_BW_250        0x40    // 250 kHz
#define SX1272_MODEM_CONFIG1_BW_500        0x80    // 500 kHz
#define SX1272_MODEM_CONFIG1_CR_MASK       0x38    // Error coding rate:
#define SX1272_MODEM_CONFIG1_CR_4_5        0x08    // 4/5
#define SX1272_MODEM_CONFIG1_CR_4_6        0x10    // 4/6
#define SX1272_MODEM_CONFIG1_CR_4_7        0x18    // 4/7
#define SX1272_MODEM_CONFIG1_CR_4_8        0x20    // 4/8
#define SX1272_MODEM_CONFIG1_HEADER_MASK   0x04    // Header mode:
#define SX1272_MODEM_CONFIG1_HEADER_EXPL   0x00    // Explicit Header
#define SX1272_MODEM_CONFIG1_HEADER_IMPL   0x04    // Implicit Header
#define SX1272_MODEM_CONFIG1_CRC_MASK      0x02    // Enable CRC generation and check on payload:
#define SX1272_MODEM_CONFIG1_CRC_OFF       0x00    // CRC disable
#define SX1272_MODEM_CONFIG1_CRC_ON        0x02    // CRC enable
#define SX1272_MODEM_CONFIG1_LDRO_MASK     0x01    // LowDataRateOptimize:
#define SX1272_MODEM_CONFIG1_LDRO_OFF      0x00    // Disabled
#define SX1272_MODEM_CONFIG1_LDRO_ON       0x01    // Enabled; mandated for SF11 and SF12 with BW = 125 kHz

#define SX127X_MODEM_CONFIG2_SF_MASK       0xF0    // SF rate:
#define SX127X_MODEM_CONFIG2_SF6           0x60    // 64 chips / symbol
#define SX127X_MODEM_CONFIG2_SF7           0x70    // 128 chips / symbol
#define SX127X_MODEM_CONFIG2_SF8           0x80    // 256 chips / symbol
#define SX127X_MODEM_CONFIG2_SF9           0x90    // 512 chips / symbol
#define SX127X_MODEM_CONFIG2_SF10          0xA0    // 1024 chips / symbol
#define SX127X_MODEM_CONFIG2_SF11          0xB0    // 2048 chips / symbol
#define SX127X_MODEM_CONFIG2_SF12          0xC0    // 4096 chips / symbol

#define SX1278_MODEM_CONFIG2_CRC_MASK      0x04    // Enable CRC generation and check on payload:
#define SX1278_MODEM_CONFIG2_CRC_OFF       0x00    // CRC disable
#define SX1278_MODEM_CONFIG2_CRC_ON        0x04    // CRC enable

#define SX1272_MODEM_CONFIG2_AGCAUTO_MASK  0x04    // AgcAutoOn:
#define SX1272_MODEM_CONFIG2_AGCAUTO_OFF   0x00    // LNA gain set by register LnaGain
#define SX1272_MODEM_CONFIG2_AGCAUTO_ON    0x04    // LNA gain set by the internal AGC loop

#define SX1278_MODEM_CONFIG3_LDRO_MASK     0x08    // LowDataRateOptimize:
#define SX1278_MODEM_CONFIG3_LDRO_OFF      0x00    // Disabled
#define SX1278_MODEM_CONFIG3_LDRO_ON       0x08    // Enabled; mandated for when the symbol length exceeds 16ms
#define SX1278_MODEM_CONFIG3_AGCAUTO_MASK  0x04    // AgcAutoOn:
#define SX1278_MODEM_CONFIG3_AGCAUTO_OFF   0x00    // LNA gain set by register LnaGain
#define SX1278_MODEM_CONFIG3_AGCAUTO_ON    0x04    // LNA gain set by the internal AGC loop

#define SX127X_PA_DAC_20                   0x87    // High Power +20 dBm (100 mW) Operation 
#define SX127X_PA_DAC_NORMAL               0x84    // Default value PA_HF/LF or +17 dBm

#define SX127X_PA_CONFIG_RFO               0x00
#define SX127X_PA_CONFIG_BOOST             0x80
#define SX127X_PA_CONFIG_MAXPOWER_MIN      0x00
#define SX127X_PA_CONFIG_MAXPOWER_MAX      0x70

// DIO function mappings                              D0D1D2D3
#define SX127X_MAP_DIO0_LORA_RXDONE        0x00    // 00------
#define SX127X_MAP_DIO0_LORA_TXDONE        0x40    // 01------
#define SX127X_MAP_DIO1_LORA_NOP           0x30    // --11----
#define SX127X_MAP_DIO2_LORA_NOP           0x0C    // ----11--
#define SX127X_MAP_DIO3_LORA_NOP           0x0C    // ------11

#define SX127X_IRQ_LORA_NONE               0x00
#define SX127X_IRQ_LORA_CAD_DETECTED       0x01    // CAD detected interrupt mask
#define SX127X_IRQ_LORA_CAD_DONE           0x04    // CAD complete interrupt mask
#define SX127X_IRQ_LORA_TXDONE             0x08    // FIFO Payload transmission complete interrupt mask
#define SX127X_IRQ_LORA_VALIDHEADER        0x10    // Valid header received interrupt mask
#define SX127X_IRQ_LORA_CRCERROR           0x20    // Payload CRC error interrupt mask
#define SX127X_IRQ_LORA_RXDONE             0x40    // Packet reception complete interrupt mask

#define SX127X_DETECT_OPTIMIZE_MASK        0x07    // Detection optimize:
#define SX127X_DETECT_OPTIMIZE_SF6         0x05    // SF6
#define SX127X_DETECT_OPTIMIZE_DEF         0x03    // SF7 - SF12

#define SX127X_DETECT_THRESHOLD_SF6        0x0C    // SF6
#define SX127X_DETECT_THRESHOLD_DEF        0x0A    // SF7 - SF12

#endif // SX127X_DEFS_H_