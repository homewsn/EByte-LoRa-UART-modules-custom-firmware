/*
* Copyright (c) 2022 Vladimir Alemasov
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

#ifndef ASR6601_HW_H_
#define ASR6601_HW_H_

// GPIOCFG macros
#define GPIO_AF_MASK                       0xF
#define GPIO_AF_NONE                       0
#define GPIO_AF_P02_UART0_CTS              1
#define GPIO_AF_P02_I2S_DI                 2
#define GPIO_AF_P02_I2C0_SCL               3
#define GPIO_AF_P02_SSP0_TX                4
#define GPIO_AF_P02_GPTIM0_CH2             6
#define GPIO_AF_P02_GPTIM2_CH0             7
#define GPIO_AF_P03_UART0_RTS              1
#define GPIO_AF_P03_I2S_DO                 2
#define GPIO_AF_P03_I2C0_SDA               3
#define GPIO_AF_P03_SSP0_RX                4
#define GPIO_AF_P03_GPTIM0_CH3             6
#define GPIO_AF_P03_GPTIM2_CH1             7
#define GPIO_AF_P04_UART1_RXD              1
#define GPIO_AF_P04_UART1_TXD              2
#define GPIO_AF_P04_UART1_SIR_OUT          3
#define GPIO_AF_P04_SSP1_CLK               4
#define GPIO_AF_P04_I2S_WS_IN              5
#define GPIO_AF_P04_I2S_WS_OUT             6
#define GPIO_AF_P04_GPTIM3_ETR             7
#define GPIO_AF_P06_UART1_CTS              1
#define GPIO_AF_P06_SWD                    2
#define GPIO_AF_P06_I2C1_SCL               3
#define GPIO_AF_P06_SSP1_TX                4
#define GPIO_AF_P06_I2S_SCLK_IN            6
#define GPIO_AF_P06_I2S_SCLK_OUT           7
#define GPIO_AF_P07_UART1_RTS              1
#define GPIO_AF_P07_SWC                    2
#define GPIO_AF_P07_I2C1_SDA               3
#define GPIO_AF_P07_SSP1_RX                4
#define GPIO_AF_P07_LPTIM_OUT              7
#define GPIO_AF_P08_UART2_RXD              1
#define GPIO_AF_P08_UART2_TXD              2
#define GPIO_AF_P08_UART2_SIR_OUT          3
#define GPIO_AF_P08_SSP2_CLK               4
#define GPIO_AF_P08_GPTIM1_CH0             6
#define GPIO_AF_P08_GPTIM3_CH0             7
#define GPIO_AF_P09_UART2_TXD              1
#define GPIO_AF_P09_UART2_RXD              2
#define GPIO_AF_P09_UART2_SIR_IN           3
#define GPIO_AF_P09_SSP2_NSS               4
#define GPIO_AF_P09_COMP1_OUT              5
#define GPIO_AF_P09_GPTIM1_CH1             6
#define GPIO_AF_P09_GPTIM3_CH1             7
#define GPIO_AF_P10_UART2_CTS              1
#define GPIO_AF_P10_I2C2_SCL               3
#define GPIO_AF_P10_SSP2_TX                4
#define GPIO_AF_P10_GPTIM0_CH0             6
#define GPIO_AF_P10_GPTIM0_ETR             7
#define GPIO_AF_P11_UART2_RTS              1
#define GPIO_AF_P11_I2C2_SDA               3
#define GPIO_AF_P11_SSP2_RX                4
#define GPIO_AF_P11_GPTIM1_CH2             6
#define GPIO_AF_P11_LPTIM_ETR              7
#define GPIO_AF_P14_UART3_CTS              1
#define GPIO_AF_P14_I2C0_SCL               3
#define GPIO_AF_P14_LPTIM1_IN2             4
#define GPIO_AF_P14_COMP0_OUT              5
#define GPIO_AF_P14_GPTIM0_CH1             6
#define GPIO_AF_P15_UART3_RTS              1
#define GPIO_AF_P15_I2C0_SDA               3
#define GPIO_AF_P15_LPTIM1_OUT             4
#define GPIO_AF_P15_COMP1_OUT              5
#define GPIO_AF_P15_GPTIM1_CH0             6
#define GPIO_AF_P15_GPTIM3_CH0             7
#define GPIO_AF_P16_UART0_RXD              1
#define GPIO_AF_P16_UART0_TXD              2
#define GPIO_AF_P16_UART0_SIR_OUT          3
#define GPIO_AF_P16_LPTIM1_ETR             4
#define GPIO_AF_P16_I2S_MCLK               5
#define GPIO_AF_P16_GPTIM1_CH1             6
#define GPIO_AF_P16_GPTIM3_CH1             7
#define GPIO_AF_P17_UART0_TXD              1
#define GPIO_AF_P17_UART0_RXD              2
#define GPIO_AF_P17_UART0_SIR_IN           3
#define GPIO_AF_P17_LPTIM1_IN1             4
#define GPIO_AF_P17_I2S_SCLK_IN            5
#define GPIO_AF_P17_I2S_SCLK_OUT           6
#define GPIO_AF_P17_LPTIM_IN1              7
#define GPIO_AF_P33_UART0_TXD              1
#define GPIO_AF_P33_UART0_RXD              2
#define GPIO_AF_P33_UART0_SIR_IN           3
#define GPIO_AF_P33_QSPI_IO1               4
#define GPIO_AF_P44_UART3_RXD              1
#define GPIO_AF_P44_UART3_TXD              2
#define GPIO_AF_P44_UART3_SIR_OUT          3
#define GPIO_AF_P44_SSP0_CLK               4
#define GPIO_AF_P44_GPTIM1_ETR             6
#define GPIO_AF_P58_RTC_TAMPER             1
#define GPIO_AF_P58_LPTIM_IN1              2
#define GPIO_AF_P58_RTC_IO_OUT             3
#define GPIO_AF_P58_LPTIM1_ETR             4
#define GPIO_AF_P58_LPUART_RX              5
#define GPIO_AF_P58_LORA_RF_SWITCH         6
#define GPIO_AF_P59_RTC_WAKEUP0            1
#define GPIO_AF_P59_LPTIM_ETR              2
#define GPIO_AF_P59_LORA_RF_SWITCH         3
#define GPIO_AF_P59_LPTIM1_IN2             4
#define GPIO_AF_P59_LPUART_RTS             5
#define GPIO_AF_P60_RTC_WAKEUP1            1
#define GPIO_AF_P60_LPUART_RX              2
#define GPIO_AF_P60_LORA_RF_SWITCH         3
#define GPIO_AF_P60_LPTIM1_IN1             4
#define GPIO_AF_P60_LPTIM_IN2              5
#define GPIO_AF_P62_LPTIM_IN2              1
#define GPIO_AF_P62_LPUART_RX              2
#define GPIO_AF_P62_LPTIM_ETR              3
#define GPIO_AF_P62_LPTIM1_IN2             4
#define GPIO_AF_P62_RTC_WAKEUP1            5

#define GPIOCFG_MODE_SHIFT                 4
#define GPIOCFG_MODE_MASK                 (3 << GPIOCFG_MODE_SHIFT)
#define GPIOCFG_MODE_ANA                  (0 << GPIOCFG_MODE_SHIFT)       // Analog mode (reset state)
#define GPIOCFG_MODE_ALT                  (1 << GPIOCFG_MODE_SHIFT)       // Alternate function mode
#define GPIOCFG_MODE_GP                   (2 << GPIOCFG_MODE_SHIFT)       // General purpose mode

#define GPIOCFG_GP_MODE_SHIFT              6
#define GPIOCFG_GP_MODE_MASK              (7 << GPIOCFG_GP_MODE_SHIFT)
#define GPIOCFG_GP_MODE_INPUT_FLOATING    (0 << GPIOCFG_GP_MODE_SHIFT)    // Input floating
#define GPIOCFG_GP_MODE_INPUT_PULL_UP     (1 << GPIOCFG_GP_MODE_SHIFT)    // Input pull-up
#define GPIOCFG_GP_MODE_INPUT_PULL_DOWN   (2 << GPIOCFG_GP_MODE_SHIFT)    // Input pull-down
#define GPIOCFG_GP_MODE_OUTPUT_PP_HIGH    (3 << GPIOCFG_GP_MODE_SHIFT)    // Output push-pull high level
#define GPIOCFG_GP_MODE_OUTPUT_PP_LOW     (4 << GPIOCFG_GP_MODE_SHIFT)    // Output push-pull low level
#define GPIOCFG_GP_MODE_OUTPUT_OD_HIZ     (5 << GPIOCFG_GP_MODE_SHIFT)    // Output open-drain high-impedance
#define GPIOCFG_GP_MODE_OUTPUT_OD_LOW     (6 << GPIOCFG_GP_MODE_SHIFT)    // Output open-drain low level

#define GPIOCFG_DRIVE_SHIFT                9
#define GPIOCFG_DRIVE_MASK                (1 << GPIOCFG_DRIVE_SHIFT)
#define GPIOCFG_DRIVE_4MA                 (0 << GPIOCFG_DRIVE_SHIFT)      // 4 mA (reset state)
#define GPIOCFG_DRIVE_8MA                 (1 << GPIOCFG_DRIVE_SHIFT)      // 8 mA

// IRQ triggers
#define GPIO_IRQ_NONE                      0x00
#define GPIO_IRQ_RISING                    0x01
#define GPIO_IRQ_FALLING                   0x02

//--------------------------------------------
// configure operation mode of GPIO pin
void hw_cfg_pin(uint8_t gpio_pin, uint16_t gpiocfg);
// set state of GPIO output pin
void hw_set_pin(uint8_t gpio_pin, uint8_t state);
// get state of GPIO input pin
uint8_t hw_get_pin(uint8_t gpio_pin);
// configure given line as external interrupt source
void hw_cfg_extirq(uint8_t gpio_pin, uint8_t irqcfg);
// clear external interrupt source flag
void hw_clr_extirq(uint8_t gpio_pin);

#endif // ASR6601_HW_H_
