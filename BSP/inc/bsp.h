/*
*********************************************************************************************************
*                                     MICIRUM BOARD SUPPORT PACKAGE
*
*                             (c) Copyright 2012; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                        BOARD SUPPORT PACKAGE
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                           STM3220G-EVAL
*                                         Evaluation Board
*
* Filename      : bsp.h
* Version       : V1.00
* Programmer(s) : SL
*                 DC
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                 MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
*               BSP present pre-processor macro definition.
*
*           (2) This file and its dependencies requires IAR v6.20 or later to be compiled.
*
*********************************************************************************************************
*/

#ifndef  BSP_PRESENT
#define  BSP_PRESENT


/*
*********************************************************************************************************
*                                                 EXTERNS
*********************************************************************************************************
*/

#ifdef   BSP_MODULE
#define  BSP_EXT
#else
#define  BSP_EXT  extern
#endif

#ifndef SUCC
#define SUCC (0)
#endif

#ifndef FAIL
#define FAIL (-1)
#endif

/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/

#include  <stdio.h>
#include  <stdarg.h>

#include  <cpu.h>
#include  <cpu_core.h>

#include  <lib_def.h>
#include  <lib_ascii.h>

#include  <stm32f2xx_gpio.h>
#include  <stm32f2xx_rcc.h>
#include  <stm32f2xx_fsmc.h>
#include  <stm32f2xx_usart.h>
#include  <stm32f2xx_dma.h>
#include  <stm32f2xx_syscfg.h>
#include  <bsp_eth.h>

#include  <system_stm32f2xx.h>

#include <app_cfg.h>



#include  <bsp_ser.h>
#include  <bsp_os.h>
#include	<bsp_delay.h>
#include  <bsp_lcd.h>
#include	<bsp_delay.h>

#include <stm32f2xx_includes.h>
#include <stm32f2x7_eth_includes.h>

#include <string.h>

#include <core_cm3.h>



#define   WORD    unsigned short
#define   BYTE    unsigned char
#define   DWORD   unsigned long
#define   UINT    unsigned int



/*
*********************************************************************************************************
*                                               CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               INT DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             PERIPH DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                                 MACRO'S
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               INT DEFINES
*********************************************************************************************************
*/

#define  BSP_INT_ID_WWDG                                   0    /* Window WatchDog Interrupt                            */
#define  BSP_INT_ID_PVD                                    1    /* PVD through EXTI Line detection Interrupt            */
#define  BSP_INT_ID_TAMPER                                 2    /* Tamper Interrupt                                     */
#define  BSP_INT_ID_RTC                                    3    /* RTC global Interrupt                                 */
#define  BSP_INT_ID_FLASH                                  4    /* FLASH global Interrupt                               */
#define  BSP_INT_ID_RCC                                    5    /* RCC global Interrupt                                 */
#define  BSP_INT_ID_EXTI0                                  6    /* EXTI Line0 Interrupt                                 */
#define  BSP_INT_ID_EXTI1                                  7    /* EXTI Line1 Interrupt                                 */
#define  BSP_INT_ID_EXTI2                                  8    /* EXTI Line2 Interrupt                                 */
#define  BSP_INT_ID_EXTI3                                  9    /* EXTI Line3 Interrupt                                 */
#define  BSP_INT_ID_EXTI4                                 10    /* EXTI Line4 Interrupt                                 */
#define  BSP_INT_ID_DMA1_CH1                              11    /* DMA1 Channel 1 global Interrupt                      */
#define  BSP_INT_ID_DMA1_CH2                              12    /* DMA1 Channel 2 global Interrupt                      */
#define  BSP_INT_ID_DMA1_CH3                              13    /* DMA1 Channel 3 global Interrupt                      */
#define  BSP_INT_ID_DMA1_CH4                              14    /* DMA1 Channel 4 global Interrupt                      */
#define  BSP_INT_ID_DMA1_CH5                              15    /* DMA1 Channel 5 global Interrupt                      */
#define  BSP_INT_ID_DMA1_CH6                              16    /* DMA1 Channel 6 global Interrupt                      */
#define  BSP_INT_ID_DMA1_CH7                              17    /* DMA1 Channel 7 global Interrupt                      */
#define  BSP_INT_ID_ADC1_2                                18    /* ADC1 et ADC2 global Interrupt                        */
#define  BSP_INT_ID_CAN1_TX                               19    /* CAN1 TX Interrupts                                   */
#define  BSP_INT_ID_CAN1_RX0                              20    /* CAN1 RX0 Interrupts                                  */
#define  BSP_INT_ID_CAN1_RX1                              21    /* CAN1 RX1 Interrupt                                   */
#define  BSP_INT_ID_CAN1_SCE                              22    /* CAN1 SCE Interrupt                                   */
#define  BSP_INT_ID_EXTI9_5                               23    /* External Line[9:5] Interrupts                        */
#define  BSP_INT_ID_TIM1_BRK                              24    /* TIM1 Break Interrupt                                 */
#define  BSP_INT_ID_TIM1_UP                               25    /* TIM1 Update Interrupt                                */
#define  BSP_INT_ID_TIM1_TRG_COM                          26    /* TIM1 Trigger and Commutation Interrupt               */
#define  BSP_INT_ID_TIM1_CC                               27    /* TIM1 Capture Compare Interrupt                       */
#define  BSP_INT_ID_TIM2                                  28    /* TIM2 global Interrupt                                */
#define  BSP_INT_ID_TIM3                                  29    /* TIM3 global Interrupt                                */
#define  BSP_INT_ID_TIM4                                  30    /* TIM4 global Interrupt                                */
#define  BSP_INT_ID_I2C1_EV                               31    /* I2C1 Event Interrupt                                 */
#define  BSP_INT_ID_I2C1_ER                               32    /* I2C1 Error Interrupt                                 */
#define  BSP_INT_ID_I2C2_EV                               33    /* I2C2 Event Interrupt                                 */
#define  BSP_INT_ID_I2C2_ER                               34    /* I2C2 Error Interrupt                                 */
#define  BSP_INT_ID_SPI1                                  35    /* SPI1 global Interrupt                                */
#define  BSP_INT_ID_SPI2                                  36    /* SPI2 global Interrupt                                */
#define  BSP_INT_ID_USART1                                37    /* USART1 global Interrupt                              */
#define  BSP_INT_ID_USART2                                38    /* USART2 global Interrupt                              */
#define  BSP_INT_ID_USART3                                39    /* USART3 global Interrupt                              */
#define  BSP_INT_ID_EXTI15_10                             40    /* External Line[15:10] Interrupts                      */
#define  BSP_INT_ID_RTC_ALARM                             41    /* RTC Alarm through EXTI Line Interrupt                */
#define  BSP_INT_ID_OTG_FS_WKUP                           42    /* USB WakeUp from suspend through EXTI Line Interrupt  */

#define  BSP_INT_ID_TIM8_BRK_TIM12                        43    /* TIM8 Break Interrupt and TIM12 global Interrupt      */
#define  BSP_INT_ID_TIM8_UP_TIM13                         44    /* TIM8 Update Interrupt and TIM13 global Interrupt     */
#define  BSP_INT_ID_TIM8_TRG_COM_TIM14                    45    /* TIM8 Trigger/Commutation and TIM14 global Interrupt  */
#define  BSP_INT_ID_TIM8_CC                               46    /* TIM8 Capture Compare Interrupt                       */
#define  BSP_INT_ID_DMA1_STREAM7                          47    /* DMA1 Stream7 Interrupt                               */
#define  BSP_INT_ID_FSMC                                  48    /* FSMC global Interrupt                                */
#define  BSP_INT_ID_SDIO                                  49    /* SDIO global Interrupt                                */

#define  BSP_INT_ID_TIM5                                  50    /* TIM5 global Interrupt                                */
#define  BSP_INT_ID_SPI3                                  51    /* SPI3 global Interrupt                                */
#define  BSP_INT_ID_USART4                                52    /* USART4 global Interrupt                              */
#define  BSP_INT_ID_USART5                                53    /* USART5 global Interrupt                              */
#define  BSP_INT_ID_TIM6                                  54    /* TIM6 global Interrupt                                */
#define  BSP_INT_ID_TIM7                                  55    /* TIM7 global Interrupt                                */
#define  BSP_INT_ID_DMA2_CH1                              56    /* DMA2 Channel 1 global Interrupt                      */
#define  BSP_INT_ID_DMA2_CH2                              57    /* DMA2 Channel 2 global Interrupt                      */
#define  BSP_INT_ID_DMA2_CH3                              58    /* DMA2 Channel 3 global Interrupt                      */
#define  BSP_INT_ID_DMA2_CH4                              59    /* DMA2 Channel 4 global Interrupt                      */
#define  BSP_INT_ID_DMA2_CH5                              60    /* DMA2 Channel 5 global Interrupt                      */

#define  BSP_INT_ID_ETH                                   61    /* ETH  global Interrupt                                */
#define  BSP_INT_ID_ETH_WKUP                              62    /* ETH  WakeUp from EXTI line interrupt                 */
#define  BSP_INT_ID_CAN2_TX                               63    /* CAN2 TX Interrupts                                   */
#define  BSP_INT_ID_CAN2_RX0                              64    /* CAN2 RX0 Interrupts                                  */
#define  BSP_INT_ID_CAN2_RX1                              65    /* CAN2 RX1 Interrupt                                   */
#define  BSP_INT_ID_CAN2_SCE                              66    /* CAN2 SCE Interrupt                                   */
#define  BSP_INT_ID_OTG_FS                                67    /* OTG global Interrupt                                 */

#define  BSP_INT_ID_DMA2_CH6                              68    /* DMA2 Channel 6 global Interrupt                      */
#define  BSP_INT_ID_DMA2_CH7                              69    /* DMA2 Channel 7 global Interrupt                      */
#define  BSP_INT_ID_DMA2_CH8                              70    /* DMA2 Channel 8 global Interrupt                      */
#define  BSP_INT_ID_USART6                                71    /* USART5 global Interrupt                              */
#define  BSP_INT_ID_I2C3_EV                               72    /* I2C3 Event  Interrupt                                */
#define  BSP_INT_ID_I2C3_ER                               73    /* I2C3 Error  Interrupt                                */
#define  BSP_INT_ID_OTG_HS_EP1_OUT                        74    /* OTG HS EP1 Output Interrupt                          */
#define  BSP_INT_ID_OTG_HS_EP1_IN                         75    /* OTG HS EP1 Input Interrupt                           */
#define  BSP_INT_ID_OTG_HS_WKUP                           76    /* OTG HS Wakeup Interrupt                              */
#define  BSP_INT_ID_OTG_HS                                77    /* OTG HS Interrupt                                     */
#define  BSP_INT_ID_DCMI                                  78    /* DCMI Interrupt                                       */
#define  BSP_INT_ID_CRYP                                  79    /* CRYPT Interrupt                                      */
#define  BSP_INT_ID_HASH_RNG                              80    /* HASH RNG Interrupt                                   */


/*
*********************************************************************************************************
*                                             PERIPH DEFINES
*********************************************************************************************************
*/
/*  RCC_AHB1        offset 0x10     */
#define  BSP_PERIPH_ID_IOPA                               0
#define  BSP_PERIPH_ID_IOPB                               1
#define  BSP_PERIPH_ID_IOPC                               2
#define  BSP_PERIPH_ID_IOPD                               3
#define  BSP_PERIPH_ID_IOPE                               4
#define  BSP_PERIPH_ID_IOPF                               5
#define  BSP_PERIPH_ID_IOPG                               6
#define  BSP_PERIPH_ID_IOPH                               7
#define  BSP_PERIPH_ID_IOPI                               8
#define  BSP_PERIPH_ID_CRC                                12
#define  BSP_PERIPH_ID_BKPSRAM                            18
#define  BSP_PERIPH_ID_DMA1                               21
#define  BSP_PERIPH_ID_DMA2                               22
#define  BSP_PERIPH_ID_ETH                                25
#define  BSP_PERIPH_ID_ETH_TX                             26
#define  BSP_PERIPH_ID_ETH_RX                             27
#define  BSP_PERIPH_ID_ETH_PTP                            28
#define  BSP_PERIPH_ID_OTGHS                              29
#define  BSP_PERIPH_ID_OTGHS_ULPI                         30
/*  RCC_AHB2        offset 0x14     */
#define  RCC_AHB2_OFFSET                                  32
#define  BSP_PERIPH_ID_DCMI                               RCC_AHB2_OFFSET+0
#define  BSP_PERIPH_ID_CRUP                               RCC_AHB2_OFFSET+4
#define  BSP_PERIPH_ID_HASH                               RCC_AHB2_OFFSET+5
#define  BSP_PERIPH_ID_RNG                                RCC_AHB2_OFFSET+6
#define  BSP_PERIPH_ID_OTGFS                              RCC_AHB2_OFFSET+7
/*  RCC_ABH3        offset 0x18     */
#define  RCC_AHB3_OFFSET                                  64
#define  BSP_PERIPH_ID_FSMC                               RCC_AHB3_OFFSET
/*  RCC_APB1        offset 0x20     */
#define  RCC_APB1_OFFSET                                  128
#define  BSP_PERIPH_ID_TIM2                               RCC_APB1_OFFSET+0
#define  BSP_PERIPH_ID_TIM3                               RCC_APB1_OFFSET+1
#define  BSP_PERIPH_ID_TIM4                               RCC_APB1_OFFSET+2
#define  BSP_PERIPH_ID_TIM5                               RCC_APB1_OFFSET+3
#define  BSP_PERIPH_ID_TIM6                               RCC_APB1_OFFSET+4
#define  BSP_PERIPH_ID_TIM7                               RCC_APB1_OFFSET+5
#define  BSP_PERIPH_ID_TIM12                              RCC_APB1_OFFSET+6
#define  BSP_PERIPH_ID_TIM13                              RCC_APB1_OFFSET+7
#define  BSP_PERIPH_ID_TIM14                              RCC_APB1_OFFSET+8
#define  BSP_PERIPH_ID_WWDG                               RCC_APB1_OFFSET+11
#define  BSP_PERIPH_ID_SPI2                               RCC_APB1_OFFSET+14
#define  BSP_PERIPH_ID_SPI3                               RCC_APB1_OFFSET+15
#define  BSP_PERIPH_ID_USART2                             RCC_APB1_OFFSET+17
#define  BSP_PERIPH_ID_USART3                             RCC_APB1_OFFSET+18
#define  BSP_PERIPH_ID_UART4                              RCC_APB1_OFFSET+19
#define  BSP_PERIPH_ID_UART5                              RCC_APB1_OFFSET+20
#define  BSP_PERIPH_ID_I2C1                               RCC_APB1_OFFSET+21
#define  BSP_PERIPH_ID_I2C2                               RCC_APB1_OFFSET+22
#define  BSP_PERIPH_ID_I2C3                               RCC_APB1_OFFSET+23
#define  BSP_PERIPH_ID_CAN1                               RCC_APB1_OFFSET+25
#define  BSP_PERIPH_ID_CAN2                               RCC_APB1_OFFSET+26
#define  BSP_PERIPH_ID_PWR                                RCC_APB1_OFFSET+28
#define  BSP_PERIPH_ID_DAC                                RCC_APB1_OFFSET+29
/*  RCC_APB2        offset 0x24     */
#define  RCC_APB2_OFFSET                                  160
#define  BSP_PERIPH_ID_TIM1                               RCC_APB2_OFFSET+0
#define  BSP_PERIPH_ID_TIM8                               RCC_APB2_OFFSET+1
#define  BSP_PERIPH_ID_USART1                             RCC_APB2_OFFSET+4
#define  BSP_PERIPH_ID_USART6                             RCC_APB2_OFFSET+5
#define  BSP_PERIPH_ID_ADC1                               RCC_APB2_OFFSET+8   /*  Reset All ADC */
#define  BSP_PERIPH_ID_ADC2                               RCC_APB2_OFFSET+9
#define  BSP_PERIPH_ID_ADC3                               RCC_APB2_OFFSET+10
#define  BSP_PERIPH_ID_SDIO                               RCC_APB2_OFFSET+11
#define  BSP_PERIPH_ID_SPI1                               RCC_APB2_OFFSET+12
#define  BSP_PERIPH_ID_SYSCFG                             RCC_APB2_OFFSET+14
#define  BSP_PERIPH_ID_TIM9                               RCC_APB2_OFFSET+16
#define  BSP_PERIPH_ID_TIM10                              RCC_APB2_OFFSET+17
#define  BSP_PERIPH_ID_TIM11                              RCC_APB2_OFFSET+18

/*
*********************************************************************************************************
*                                           FUNCTION PROTOTYPES
*********************************************************************************************************
*/

BSP_EXT void        BSP_Init                (void);
BSP_EXT void        BSP_CPU_TickInit        (void);
BSP_EXT CPU_INT32U  BSP_CPU_ClkFreq         (void);

BSP_EXT void        BSP_IntEnAll            (void);
BSP_EXT void        BSP_IntDisAll           (void);


BSP_EXT void        BSP_IntEn      (CPU_DATA       int_id);
BSP_EXT void        BSP_IntInit    (void);
BSP_EXT void        BSP_IntDis     (CPU_DATA       int_id);
BSP_EXT void        BSP_IntClr     (CPU_DATA       int_id);
BSP_EXT void        BSP_IntVectSet (CPU_DATA       int_id,
                            CPU_FNCT_VOID  isr);

BSP_EXT void        BSP_LED_On     (CPU_INT08U     led);
BSP_EXT void        BSP_LED_Off    (CPU_INT08U     led);
BSP_EXT void        BSP_LED_Toggle (CPU_INT08U     led);

BSP_EXT CPU_INT32U  BSP_RNG_Read   (void);

BSP_EXT void        BSP_IntPrioSet (CPU_DATA       int_id,
                            CPU_INT08U     prio);

BSP_EXT void        BSP_PSRAM_Init (void);
BSP_EXT void        BSP_SRAM_Init  (void);


BSP_EXT void BSP_SoftReset(void);

BSP_EXT void BSP_SysClkConfig(void);

BSP_EXT void BSP_PClkConfig(void);


/*
*********************************************************************************************************
*                                           INTERRUPT SERVICES
*********************************************************************************************************
*/

BSP_EXT void         BSP_IntHandlerWWDG               (void);
BSP_EXT void         BSP_IntHandlerPVD                (void);
BSP_EXT void         BSP_IntHandlerTAMPER             (void);
BSP_EXT void         BSP_IntHandlerRTC                (void);
BSP_EXT void         BSP_IntHandlerFLASH              (void);
BSP_EXT void         BSP_IntHandlerRCC                (void);
BSP_EXT void         BSP_IntHandlerEXTI0              (void);
BSP_EXT void         BSP_IntHandlerEXTI1              (void);
BSP_EXT void         BSP_IntHandlerEXTI2              (void);
BSP_EXT void         BSP_IntHandlerEXTI3              (void);
BSP_EXT void         BSP_IntHandlerEXTI4              (void);
BSP_EXT void         BSP_IntHandlerDMA1_CH1           (void);
BSP_EXT void         BSP_IntHandlerDMA1_CH2           (void);
BSP_EXT void         BSP_IntHandlerDMA1_CH3           (void);
BSP_EXT void         BSP_IntHandlerDMA1_CH4           (void);
BSP_EXT void         BSP_IntHandlerDMA1_CH5           (void);
BSP_EXT void         BSP_IntHandlerDMA1_CH6           (void);
BSP_EXT void         BSP_IntHandlerDMA1_CH7           (void);
BSP_EXT void         BSP_IntHandlerADC1_2             (void);
BSP_EXT void         BSP_IntHandlerCAN1_TX            (void);
BSP_EXT void         BSP_IntHandlerCAN1_RX0           (void);
BSP_EXT void         BSP_IntHandlerCAN1_RX1           (void);
BSP_EXT void         BSP_IntHandlerCAN1_SCE           (void);
BSP_EXT void         BSP_IntHandlerEXTI9_5            (void);
BSP_EXT void         BSP_IntHandlerTIM1_BRK           (void);
BSP_EXT void         BSP_IntHandlerTIM1_UP            (void);
BSP_EXT void         BSP_IntHandlerTIM1_TRG_COM       (void);
BSP_EXT void         BSP_IntHandlerTIM1_CC            (void);
BSP_EXT void         BSP_IntHandlerTIM2               (void);
BSP_EXT void         BSP_IntHandlerTIM3               (void);
BSP_EXT void         BSP_IntHandlerTIM4               (void);
BSP_EXT void         BSP_IntHandlerI2C1_EV            (void);
BSP_EXT void         BSP_IntHandlerI2C1_ER            (void);
BSP_EXT void         BSP_IntHandlerI2C2_EV            (void);
BSP_EXT void         BSP_IntHandlerI2C2_ER            (void);
BSP_EXT void         BSP_IntHandlerSPI1               (void);
BSP_EXT void         BSP_IntHandlerSPI2               (void);
BSP_EXT void         BSP_IntHandlerUSART1             (void);
BSP_EXT void         BSP_IntHandlerUSART2             (void);
BSP_EXT void         BSP_IntHandlerUSART3             (void);
BSP_EXT void         BSP_IntHandlerEXTI15_10          (void);
BSP_EXT void         BSP_IntHandlerRTCAlarm           (void);
BSP_EXT void         BSP_IntHandlerUSBWakeUp          (void);
BSP_EXT void         BSP_IntHandlerTIM8_BRK_TIM12     (void);
BSP_EXT void         BSP_IntHandlerTIM8_UP_TIM13      (void);
BSP_EXT void         BSP_IntHandlerTIM8_TRG_COM_TIM14 (void);
BSP_EXT void         BSP_IntHandlerTIM8_CC            (void);
BSP_EXT void         BSP_IntHandlerDMA1_STREAM7       (void);
BSP_EXT void         BSP_IntHandlerFSMC               (void);
BSP_EXT void         BSP_IntHandlerSDIO               (void);
BSP_EXT void         BSP_IntHandlerTIM5               (void);
BSP_EXT void         BSP_IntHandlerSPI3               (void);
BSP_EXT void         BSP_IntHandlerUSART4             (void);
BSP_EXT void         BSP_IntHandlerUSART5             (void);
BSP_EXT void         BSP_IntHandlerTIM6               (void);
BSP_EXT void         BSP_IntHandlerTIM7               (void);
BSP_EXT void         BSP_IntHandlerDMA2_CH1           (void);
BSP_EXT void         BSP_IntHandlerDMA2_CH2           (void);
BSP_EXT void         BSP_IntHandlerDMA2_CH3           (void);
BSP_EXT void         BSP_IntHandlerDMA2_CH4           (void);
BSP_EXT void         BSP_IntHandlerDMA2_CH5           (void);
BSP_EXT void         BSP_IntHandlerETH                (void);
BSP_EXT void         BSP_IntHandlerETHWakeup          (void);
BSP_EXT void         BSP_IntHandlerCAN2_TX            (void);
BSP_EXT void         BSP_IntHandlerCAN2_RX0           (void);
BSP_EXT void         BSP_IntHandlerCAN2_RX1           (void);
BSP_EXT void         BSP_IntHandlerCAN2_SCE           (void);
BSP_EXT void         BSP_IntHandlerOTG                (void);
BSP_EXT void         BSP_IntHandlerDMA2_CH6           (void);
BSP_EXT void         BSP_IntHandlerDMA2_CH7           (void);
BSP_EXT void         BSP_IntHandlerDMA2_CH8           (void);
BSP_EXT void         BSP_IntHandlerUSART6             (void);
BSP_EXT void         BSP_IntHandlerI2C3_EV            (void);
BSP_EXT void         BSP_IntHandlerI2C3_ER            (void);
BSP_EXT void         BSP_IntHandlerOTG_HS_EP1_OUT     (void);
BSP_EXT void         BSP_IntHandlerOTG_HS_EP1_IN      (void);
BSP_EXT void         BSP_IntHandlerOTG_HS_WKUP        (void);
BSP_EXT void         BSP_IntHandlerOTG_HS             (void);
BSP_EXT void         BSP_IntHandlerDCMI               (void);
BSP_EXT void         BSP_IntHandlerCRYP               (void);
BSP_EXT void         BSP_IntHandlerHASH_RNG           (void);





#endif

