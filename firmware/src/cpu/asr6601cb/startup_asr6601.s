; Copyright (c) 2009-2021 ARM Limited. All rights reserved.
; 
;     SPDX-License-Identifier: Apache-2.0
; 
; Licensed under the Apache License, Version 2.0 (the License); you may
; not use this file except in compliance with the License.
; You may obtain a copy of the License at
; 
;     www.apache.org/licenses/LICENSE-2.0
; 
; Unless required by applicable law or agreed to in writing, software
; distributed under the License is distributed on an AS IS BASIS, WITHOUT
; WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
; See the License for the specific language governing permissions and
; limitations under the License.
; 
; NOTICE: This file has been modified by Vladimir Alemasov.

; The modules in this file are included in the libraries, and may be replaced
; by any user-defined modules that define the PUBLIC symbol _program_start or
; a user defined start symbol.
; To override the cstartup defined in the library, simply add your modified
; version to the workbench project.
;
; The vector table is normally located at address 0x08000000.
; When debugging in RAM, it can be located in RAM, aligned to at least 2^6.
; The name "__vector_table" has special meaning for C-SPY:
; it is where the SP start value is found, and the NVIC vector
; table register (VTOR) is initialized to this address if != 0.

        MODULE  ?cstartup

        ;; Forward declaration of sections
        SECTION CSTACK:DATA:NOROOT(3)

        SECTION .intvec:CODE:NOROOT(2)

        EXTERN  __iar_program_start
        EXTERN  SystemInit
        PUBLIC  __vector_table

        DATA
__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler             ; Reset Handler
        DCD     NMI_Handler               ; NMI Handler
        DCD     HardFault_Handler         ; Hard Fault Handler
        DCD     MemManage_Handler         ; MPU Fault Handler
        DCD     BusFault_Handler          ; Bus Fault Handler
        DCD     UsageFault_Handler        ; Usage Fault Handler
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     0                         ; Reserved
        DCD     SVC_Handler               ; SVCall Handler
        DCD     DebugMon_Handler          ; Debug Monitor Handler
        DCD     0                         ; Reserved
        DCD     PendSV_Handler            ; PendSV Handler
        DCD     SysTick_Handler           ; SysTick Handler

        ;; External Interrupts
        DCD     SEC_IRQHandler
        DCD     RTC_IRQHandler
        DCD     WDG_IRQHandler
        DCD     EFC_IRQHandler
        DCD     UART3_IRQHandler
        DCD     I2C2_IRQHandler
        DCD     UART0_IRQHandler
        DCD     UART1_IRQHandler
        DCD     UART2_IRQHandler
        DCD     LPUART_IRQHandler
        DCD     SSP0_IRQHandler
        DCD     SSP1_IRQHandler
        DCD     QSPI_IRQHandler
        DCD     I2C0_IRQHandler
        DCD     I2C1_IRQHandler
        DCD     SCC_IRQHandler
        DCD     ADC_IRQHandler
        DCD     AFEC_IRQHandler
        DCD     SSP2_IRQHandler
        DCD     DMA1_IRQHandler
        DCD     DAC_IRQHandler
        DCD     LORA_IRQHandler
        DCD     GPIO_IRQHandler
        DCD     TIMER0_IRQHandler
        DCD     TIMER1_IRQHandler
        DCD     TIMER2_IRQHandler
        DCD     TIMER3_IRQHandler
        DCD     BSTIMER0_IRQHandler
        DCD     BSTIMER1_IRQHandler
        DCD     LPTIMER0_IRQHandler
        DCD     SAC_IRQHandler
        DCD     DMA0_IRQHandler
        DCD     I2S_IRQHandler
        DCD     LCD_IRQHandler
        DCD     PWR_IRQHandler
        DCD     LPTIMER1_IRQHandler
        DCD     IWDG_IRQHandler

        ;; Default interrupt handlers
        THUMB
        PUBWEAK Reset_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reset_Handler
        LDR     R0, =SystemInit
        BLX     R0
        LDR     R0, =__iar_program_start
        BX      R0

        ;; Dummy exception handlers

        PUBWEAK NMI_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
NMI_Handler
        B .

        PUBWEAK HardFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
HardFault_Handler
        B .

        PUBWEAK MemManage_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
MemManage_Handler
        B .

        PUBWEAK BusFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
BusFault_Handler
        B .

        PUBWEAK UsageFault_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
UsageFault_Handler
        B .

        PUBWEAK SVC_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SVC_Handler
        B .

        PUBWEAK DebugMon_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
DebugMon_Handler
        B .

        PUBWEAK PendSV_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
PendSV_Handler
        B .

        PUBWEAK SysTick_Handler
        SECTION .text:CODE:REORDER:NOROOT(1)
SysTick_Handler
        B .

        ;; Dummy interrupt handlers

        PUBWEAK SEC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
SEC_IRQHandler
        B .

        PUBWEAK RTC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
RTC_IRQHandler
        B .

        PUBWEAK WDG_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
WDG_IRQHandler
        B .

        PUBWEAK EFC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
EFC_IRQHandler
        B .

        PUBWEAK UART3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART3_IRQHandler
        B .

        PUBWEAK I2C2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2C2_IRQHandler
        B .

        PUBWEAK UART0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART0_IRQHandler
        B .

        PUBWEAK UART1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART1_IRQHandler
        B .

        PUBWEAK UART2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
UART2_IRQHandler
        B .

        PUBWEAK LPUART_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
LPUART_IRQHandler
        B .

        PUBWEAK SSP0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
SSP0_IRQHandler
        B .

        PUBWEAK SSP1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
SSP1_IRQHandler
        B .

        PUBWEAK QSPI_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
QSPI_IRQHandler
        B .

        PUBWEAK I2C0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2C0_IRQHandler
        B .

        PUBWEAK I2C1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2C1_IRQHandler
        B .

        PUBWEAK SCC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
SCC_IRQHandler
        B .

        PUBWEAK ADC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
ADC_IRQHandler
        B .

        PUBWEAK AFEC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
AFEC_IRQHandler
        B .

        PUBWEAK SSP2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
SSP2_IRQHandler
        B .

        PUBWEAK DMA1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
DMA1_IRQHandler
        B .

        PUBWEAK DAC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
DAC_IRQHandler
        B .

        PUBWEAK LORA_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
LORA_IRQHandler
        B .

        PUBWEAK GPIO_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
GPIO_IRQHandler
        B .

        PUBWEAK TIMER0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMER0_IRQHandler
        B .

        PUBWEAK TIMER1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMER1_IRQHandler
        B .

        PUBWEAK TIMER2_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMER2_IRQHandler
        B .

        PUBWEAK TIMER3_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
TIMER3_IRQHandler
        B .

        PUBWEAK BSTIMER0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BSTIMER0_IRQHandler
        B .

        PUBWEAK BSTIMER1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
BSTIMER1_IRQHandler
        B .

        PUBWEAK LPTIMER0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
LPTIMER0_IRQHandler
        B .

        PUBWEAK SAC_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
SAC_IRQHandler
        B .

        PUBWEAK DMA0_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
DMA0_IRQHandler
        B .

        PUBWEAK I2S_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
I2S_IRQHandler
        B .

        PUBWEAK LCD_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
LCD_IRQHandler
        B .

        PUBWEAK PWR_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
PWR_IRQHandler
        B .

        PUBWEAK LPTIMER1_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
LPTIMER1_IRQHandler
        B .

        PUBWEAK IWDG_IRQHandler
        SECTION .text:CODE:REORDER:NOROOT(1)
IWDG_IRQHandler
        B .

        END
