/*
* This file is a compilation stm8s.h from
* "Tiny peripheral library for STM8S" by lujji
* https://github.com/lujji/stm8-bare-min/blob/master/stm8/stm8s.h
* and stm8as.h from
* "STM8 Libraries and Examples with Arduino look&feel" by Georg Icking-Konert
* date 2017-02-11, version 0.3, copyright MIT License (MIT)
* https://github.com/gicking/STM8_templates/blob/master/Library/Base/inc/stm8as.h
*
* Many thanks to both of them for their excellent work.
*/
#ifndef STM8L_H
#define STM8L_H

/* Supported STM8 devices. Define respective device in Makefile
 STM8L151G6   ultra-low-power line medium density device
*/

// check if device is supported
#if !defined (STM8L151G6)
  #error "select supported STM8L device. For a list, see file 'stm8l.h'"
#endif

#define _MEM_(mem_addr)         (*(volatile uint8_t *)(mem_addr))
#define _SFR_(mem_addr)         (*(volatile uint8_t *)(0x5000 + (mem_addr)))

/* PORT A */
#define PA_ODR                  _SFR_(0x00)
#define PA_IDR                  _SFR_(0x01)
#define PA_DDR                  _SFR_(0x02)
#define PA_CR1                  _SFR_(0x03)
#define PA_CR2                  _SFR_(0x04)
/* PORT B */
#define PB_ODR                  _SFR_(0x05)
#define PB_IDR                  _SFR_(0x06)
#define PB_DDR                  _SFR_(0x07)
#define PB_CR1                  _SFR_(0x08)
#define PB_CR2                  _SFR_(0x09)
/* PORT C */
#define PC_ODR                  _SFR_(0x0A)
#define PC_IDR                  _SFR_(0x0B)
#define PC_DDR                  _SFR_(0x0C)
#define PC_CR1                  _SFR_(0x0D)
#define PC_CR2                  _SFR_(0x0E)
/* PORT D */
#define PD_ODR                  _SFR_(0x0F)
#define PD_IDR                  _SFR_(0x10)
#define PD_DDR                  _SFR_(0x11)
#define PD_CR1                  _SFR_(0x12)
#define PD_CR2                  _SFR_(0x13)
/* PORT E */
#define PE_ODR                  _SFR_(0x14)
#define PE_IDR                  _SFR_(0x15)
#define PE_DDR                  _SFR_(0x16)
#define PE_CR1                  _SFR_(0x17)
#define PE_CR2                  _SFR_(0x18)
/* PORT F */
#define PF_ODR                  _SFR_(0x19)
#define PF_IDR                  _SFR_(0x1A)
#define PF_DDR                  _SFR_(0x1B)
#define PF_CR1                  _SFR_(0x1C)
#define PF_CR2                  _SFR_(0x1D)

/* Flash */
#define FLASH_CR1               _SFR_(0x50)
#define FLASH_CR2               _SFR_(0x51)
#define FLASH_PUKR              _SFR_(0x52)
#define FLASH_DUKR              _SFR_(0x53)
#define FLASH_IAPSR             _SFR_(0x54)

/* DMA1 */

/* SYSCFG */
#define SYSCFG_RMPCR1           _SFR_(0x9E)
#define SYSCFG_RMPCR1_USART1TR_REMAP0 4
#define SYSCFG_RMPCR1_USART1TR_REMAP1 5

/* ITC - EXTI */

/* WFE */

/* RST */

/* PWR */

/* CLK */
#define CLK_CKDIVR              _SFR_(0xC0)
#define CLK_CRTCR               _SFR_(0xC1)
#define CLK_ICKR                _SFR_(0xC2)
#define CLK_PCKENR1             _SFR_(0xC3)
#define CLK_PCKENR1_DAC         7
#define CLK_PCKENR1_BEEP        6
#define CLK_PCKENR1_USART1      5
#define CLK_PCKENR1_SPI1        4
#define CLK_PCKENR1_I2C1        3
#define CLK_PCKENR1_TIM4        2
#define CLK_PCKENR1_TIM3        1
#define CLK_PCKENR1_TIM2        0
#define CLK_PCKENR2             _SFR_(0xC4)
#define CLK_PCKENR2_BOOTROM     7
#define CLK_PCKENR2_COMP12      5
#define CLK_PCKENR2_DMA1        4
#define CLK_PCKENR2_LCD         3
#define CLK_PCKENR2_RTC         2
#define CLK_PCKENR2_TIM1        1
#define CLK_PCKENR2_ADC1        0
#define CLK_CCOR                _SFR_(0xC5)
#define CLK_ECKR                _SFR_(0xC6)
#define CLK_SCSR                _SFR_(0xC7)
#define CLK_SWR                 _SFR_(0xC8)
#define CLK_SWCR                _SFR_(0xC9)
#define CLK_CSSR                _SFR_(0xCA)
#define CLK_CBEEPR              _SFR_(0xCB)
#define CLK_HSICALR             _SFR_(0xCC)
#define CLK_HSITRIMR            _SFR_(0xCD)
#define CLK_HSIUNLCKR           _SFR_(0xCE)
#define CLK_REGCSR              _SFR_(0xCF)

/* WWDG */
#define WWDG_CR                 _SFR_(0xD3)
#define WWDG_WR                 _SFR_(0xD4)

/* IWDG */
#define IWDG_KR                 _SFR_(0xE0)
#define IWDG_PR                 _SFR_(0xE1)
#define IWDG_RLR                _SFR_(0xE2)

/* BEEP */

/* RTC */

/* SPI1 */
#define SPI_CR1                 _SFR_(0x200)
#define SPI_CR1_LSBFIRST        7
#define SPI_CR1_SPE             6
#define SPI_CR1_BR2             5
#define SPI_CR1_BR1             4
#define SPI_CR1_BR0             3
#define SPI_CR1_MSTR            2
#define SPI_CR1_CPOL            1
#define SPI_CR1_CPHA            0
#define SPI_CR2                 _SFR_(0x201)
#define SPI_CR2_BDM             7
#define SPI_CR2_BDOE            6
#define SPI_CR2_CRCEN           5
#define SPI_CR2_CRCNEXT         4
#define SPI_CR2_RXONLY          2
#define SPI_CR2_SSM             1
#define SPI_CR2_SSI             0
#define SPI_ICR                 _SFR_(0x202)
#define SPI_SR                  _SFR_(0x203)
#define SPI_SR_BSY              7
#define SPI_SR_OVR              6
#define SPI_SR_MODF             5
#define SPI_SR_CRCERR           4
#define SPI_SR_WKUP             3
#define SPI_SR_TXE              1
#define SPI_SR_RXNE             0
#define SPI_DR                  _SFR_(0x204)
#define SPI_CRCPR               _SFR_(0x205)
#define SPI_RXCRCR              _SFR_(0x206)
#define SPI_TXCRCR              _SFR_(0x207)

/* I2C1 */
#define I2C1_CR1                _SFR_(0x210)
#define I2C1_CR2                _SFR_(0x211)
#define I2C1_FREQR              _SFR_(0x212)
#define I2C1_OARL               _SFR_(0x213)
#define I2C1_OARH               _SFR_(0x214)
#define I2C1_DR                 _SFR_(0x216)
#define I2C1_SR1                _SFR_(0x217)
#define I2C1_SR2                _SFR_(0x218)
#define I2C1_SR3                _SFR_(0x219)
#define I2C1_ITR                _SFR_(0x21A)
#define I2C1_CCRL               _SFR_(0x21B)
#define I2C1_CCRH               _SFR_(0x21C)
#define I2C1_TRISER             _SFR_(0x21D)
#define I2C1_PECR               _SFR_(0x21E)

/* USART1 */
#define USART1_SR               _SFR_(0x230)
#define USART1_SR_TXE           7
#define USART1_SR_RXNE          5
#define USART1_DR               _SFR_(0x231)
#define USART1_BRR1             _SFR_(0x232)
#define USART1_BRR2             _SFR_(0x233)
#define USART1_CR1              _SFR_(0x234)
#define USART1_CR2              _SFR_(0x235)
#define USART1_CR2_RIEN         5
#define USART1_CR2_TEN          3
#define USART1_CR2_REN          2
#define USART1_CR3              _SFR_(0x236)
#define USART1_CR4              _SFR_(0x237)
#define USART1_CR5              _SFR_(0x238)
#define USART1_GTR              _SFR_(0x239)
#define USART1_PSCR             _SFR_(0x23A)

/* TIM2 */
#define TIM2_CR1                _SFR_(0x250)
#define TIM2_CR2                _SFR_(0x251)
#define TIM2_SMCR               _SFR_(0x252)
#define TIM2_ETR                _SFR_(0x253)
#define TIM2_DER                _SFR_(0x254)
#define TIM2_IER                _SFR_(0x255)
#define TIM2_SR1                _SFR_(0x256)
#define TIM2_SR2                _SFR_(0x257)
#define TIM2_EGR                _SFR_(0x258)
#define TIM2_CCMR1              _SFR_(0x259)
#define TIM2_CCMR2              _SFR_(0x25A)
#define TIM2_CCER1              _SFR_(0x25B)
#define TIM2_CNTRH              _SFR_(0x25C)
#define TIM2_CNTRL              _SFR_(0x25D)
#define TIM2_PSCR               _SFR_(0x25E)
#define TIM2_ARRH               _SFR_(0x25F)
#define TIM2_ARRL               _SFR_(0x260)
#define TIM2_CCR1H              _SFR_(0x261)
#define TIM2_CCR1L              _SFR_(0x262)
#define TIM2_CCR2H              _SFR_(0x263)
#define TIM2_CCR2L              _SFR_(0x264)
#define TIM2_BKR                _SFR_(0x265)
#define TIM2_OISR               _SFR_(0x266)

/* TIM3 */
#define TIM3_CR1                _SFR_(0x280)
#define TIM3_CR2                _SFR_(0x281)
#define TIM3_SMCR               _SFR_(0x282)
#define TIM3_ETR                _SFR_(0x283)
#define TIM3_DER                _SFR_(0x284)
#define TIM3_IER                _SFR_(0x285)
#define TIM3_SR1                _SFR_(0x286)
#define TIM3_SR2                _SFR_(0x287)
#define TIM3_EGR                _SFR_(0x288)
#define TIM3_CCMR1              _SFR_(0x289)
#define TIM3_CCMR2              _SFR_(0x28A)
#define TIM3_CCER1              _SFR_(0x28B)
#define TIM3_CNTRH              _SFR_(0x28C)
#define TIM3_CNTRL              _SFR_(0x28D)
#define TIM3_PSCR               _SFR_(0x28E)
#define TIM3_ARRH               _SFR_(0x28F)
#define TIM3_ARRL               _SFR_(0x290)
#define TIM3_CCR1H              _SFR_(0x291)
#define TIM3_CCR1L              _SFR_(0x292)
#define TIM3_CCR2H              _SFR_(0x293)
#define TIM3_CCR2L              _SFR_(0x294)
#define TIM3_BKR                _SFR_(0x295)
#define TIM3_OISR               _SFR_(0x296)

/* TIM1 */
#define TIM1_CR1                _SFR_(0x2B0)
#define TIM1_CR2                _SFR_(0x2B1)
#define TIM1_SMCR               _SFR_(0x2B2)
#define TIM1_ETR                _SFR_(0x2B3)
#define TIM1_DER                _SFR_(0x2B4)
#define TIM1_IER                _SFR_(0x2B5)
#define TIM1_SR1                _SFR_(0x2B6)
#define TIM1_SR2                _SFR_(0x2B7)
#define TIM1_EGR                _SFR_(0x2B8)
#define TIM1_CCMR1              _SFR_(0x2B9)
#define TIM1_CCMR2              _SFR_(0x2BA)
#define TIM1_CCMR3              _SFR_(0x2BB)
#define TIM1_CCMR4              _SFR_(0x2BC)
#define TIM1_CCER1              _SFR_(0x2BD)
#define TIM1_CCER2              _SFR_(0x2BE)
#define TIM1_CNTRH              _SFR_(0x2BF)
#define TIM1_CNTRL              _SFR_(0x2C0)
#define TIM1_PSCRH              _SFR_(0x2C1)
#define TIM1_PSCRL              _SFR_(0x2C2)
#define TIM1_ARRH               _SFR_(0x2C3)
#define TIM1_ARRL               _SFR_(0x2C4)
#define TIM1_RCR                _SFR_(0x2C5)
#define TIM1_CCR1H              _SFR_(0x2C6)
#define TIM1_CCR1L              _SFR_(0x2C7)
#define TIM1_CCR2H              _SFR_(0x2C8)
#define TIM1_CCR2L              _SFR_(0x2C9)
#define TIM1_CCR3H              _SFR_(0x2CA)
#define TIM1_CCR3L              _SFR_(0x2CB)
#define TIM1_CCR4H              _SFR_(0x2CC)
#define TIM1_CCR4L              _SFR_(0x2CD)
#define TIM1_BKR                _SFR_(0x2CE)
#define TIM1_DTR                _SFR_(0x2CF)
#define TIM1_OISR               _SFR_(0x2D0)
#define TIM1_DCR1               _SFR_(0x2D1)
#define TIM1_DCR2               _SFR_(0x2D2)
#define TIM1_DMA1R              _SFR_(0x2D3)

/* TIM4 */
#define TIM4_CR1                _SFR_(0x2E0)
#define TIM4_CR1_CEN            0
#define TIM4_CR2                _SFR_(0x2E1)
#define TIM4_SMCR               _SFR_(0x2E2)
#define TIM4_DER                _SFR_(0x2E3)
#define TIM4_IER                _SFR_(0x2E4)
#define TIM4_IER_TIE            6
#define TIM4_IER_UIE            0
#define TIM4_SR                 _SFR_(0x2E5)
#define TIM4_SR_UIF             0
#define TIM4_EGR                _SFR_(0x2E6)
#define TIM4_CNTR               _SFR_(0x2E7)
#define TIM4_PSCR               _SFR_(0x2E8)
#define TIM4_ARR                _SFR_(0x2E9)

/* IRTIM */
#define IR_CR                   _SFR_(0x2FF)

/* ADC1 */
#define ADC1_CR1                _SFR_(0x340)
#define ADC1_CR2                _SFR_(0x341)
#define ADC1_CR3                _SFR_(0x342)
#define ADC1_SR                 _SFR_(0x343)
#define ADC1_DRH                _SFR_(0x344)
#define ADC1_DRL                _SFR_(0x345)
#define ADC1_HTRH               _SFR_(0x346)
#define ADC1_HTRL               _SFR_(0x347)
#define ADC1_LTRH               _SFR_(0x348)
#define ADC1_LTRL               _SFR_(0x349)
#define ADC1_SQR1               _SFR_(0x34A)
#define ADC1_SQR2               _SFR_(0x34B)
#define ADC1_SQR3               _SFR_(0x34C)
#define ADC1_SQR4               _SFR_(0x34D)
#define ADC1_TRIGR1             _SFR_(0x34E)
#define ADC1_TRIGR2             _SFR_(0x34F)
#define ADC1_TRIGR3             _SFR_(0x350)
#define ADC1_TRIGR4             _SFR_(0x351)

/* DAC */

/* ... */


/* Interrupts */

// trap - Software interrupt (call TRIGGER_TRAP to trip)
#define TLI_ISR                 0

// irq1 - reserved

// irq2 - flash interrupt
#define FLASH_ISR               2

// irq25 - TIM4 or TIM6 Update/Overflow interrupt
#define TIM4_ISR                25

// irq27 - USART1 send (TX empty) interrupt
#define USART1_TXC_ISR          27

// irq28 - USART1 receive (RX full) interrupt
#define USART1_RXC_ISR          28

#endif /* STM8L_H */
