/**
  ******************************************************************************
  * @file      startup_stm32f2xx.s
  * @author    MCD Application Team
  * @version   V1.1.3
  * @date      05-March-2012
  * @brief     STM32F2xx Devices vector table for RIDE7 toolchain. 
  *            This module performs:
  *                - Set the initial SP
  *                - Set the initial PC == Reset_Handler,
  *                - Set the vector table entries with the exceptions ISR address
  *                - Configure the system clock and the external SRAM mounted on 
  *                  STM322xG-EVAL board to be used as data memory (optional, 
  *                  to be enabled by user)
  *                - Branches to main in the C library (which eventually
  *                  calls main()).
  *            After Reset the Cortex-M3 processor is in Thread mode,
  *            priority is Privileged, and the Stack is set to Main.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
    
  .syntax unified
  .cpu cortex-m3
  .fpu softvfp
  .thumb

.global  g_pfnVectors
.global  Default_Handler

/* start address for the initialization values of the .data section. 
defined in linker script */
.word  _sidata
/* start address for the .data section. defined in linker script */  
.word  _sdata
/* end address for the .data section. defined in linker script */
.word  _edata
/* start address for the .bss section. defined in linker script */
.word  _sbss
/* end address for the .bss section. defined in linker script */
.word  _ebss
/* stack used for SystemInit_ExtMemCtl; always internal RAM used */

/**
 * @brief  This is the code that gets called when the processor first
 *          starts execution following a reset event. Only the absolutely
 *          necessary set is performed, after which the application
 *          supplied main() routine is called. 
 * @param  None
 * @retval : None
*/

    .section  .text.Reset_Handler
  .weak  Reset_Handler
  .type  Reset_Handler, %function
Reset_Handler:  

/* Copy the data segment initializers from flash to SRAM */  
  movs  r1, #0
  b  LoopCopyDataInit

CopyDataInit:
  ldr  r3, =_sidata
  ldr  r3, [r3, r1]
  str  r3, [r0, r1]
  adds  r1, r1, #4
    
LoopCopyDataInit:
  ldr  r0, =_sdata
  ldr  r3, =_edata
  adds  r2, r0, r1
  cmp  r2, r3
  bcc  CopyDataInit
  ldr  r2, =_sbss
  b  LoopFillZerobss
/* Zero fill the bss segment. */  
FillZerobss:
  movs  r3, #0
  str  r3, [r2], #4
    
LoopFillZerobss:
  ldr  r3, = _ebss
  cmp  r2, r3
  bcc  FillZerobss
/* Call the clock system intitialization function.*/
  bl  SystemInit   
/* Call the application's entry point.*/
  bl  main
  bx  lr    
.size  Reset_Handler, .-Reset_Handler

/**
 * @brief  This is the code that gets called when the processor receives an 
 *         unexpected interrupt.  This simply enters an infinite loop, preserving
 *         the system state for examination by a debugger.
 * @param  None     
 * @retval None       
*/
    .section  .text.Default_Handler,"ax",%progbits
Default_Handler:
Infinite_Loop:
  b  Infinite_Loop
  .size  Default_Handler, .-Default_Handler
/******************************************************************************
*
* The minimal vector table for a Cortex M3. Note that the proper constructs
* must be placed on this to ensure that it ends up at physical address
* 0x0000.0000.
* 
*******************************************************************************/
   .section  .isr_vector,"a",%progbits
  .type  g_pfnVectors, %object
  .size  g_pfnVectors, .-g_pfnVectors
    
    
g_pfnVectors:
  .word  _estack
  .word  Reset_Handler
  .word  App_NMI_ISR
  .word  App_Fault_ISR
  .word  App_MemFault_ISR
  .word  App_BusFault_ISR
  .word  App_UsageFault_ISR
  .word  App_Spurious_ISR
  .word  App_Spurious_ISR
  .word  App_Spurious_ISR
  .word  App_Spurious_ISR
  .word  App_Spurious_ISR
  .word  App_Spurious_ISR
  .word  App_Spurious_ISR
  .word  OS_CPU_PendSVHandler
  .word  OS_CPU_SysTickHandler
  
  /* External Interrupts */
  .word     BSP_IntHandlerWWDG                   /* Window WatchDog              */                                        
  .word     BSP_IntHandlerPVD                    /* PVD through EXTI Line detection */                        
  .word     BSP_IntHandlerTAMPER             /* Tamper and TimeStamps through the EXTI line */            
  .word     BSP_IntHandlerRTC               /* RTC Wakeup through the EXTI line */                      
  .word     BSP_IntHandlerFLASH                  /* FLASH                        */                                          
  .word     BSP_IntHandlerRCC                    /* RCC                          */                                            
  .word     BSP_IntHandlerEXTI0                  /* EXTI Line0                   */                        
  .word     BSP_IntHandlerEXTI1                  /* EXTI Line1                   */                          
  .word     BSP_IntHandlerEXTI2                  /* EXTI Line2                   */                          
  .word     BSP_IntHandlerEXTI3                  /* EXTI Line3                   */                          
  .word     BSP_IntHandlerEXTI4                  /* EXTI Line4                   */                          
  .word     BSP_IntHandlerDMA1_CH1           /* DMA1 Stream 0                */                  
  .word     BSP_IntHandlerDMA1_CH2           /* DMA1 Stream 1                */                   
  .word     BSP_IntHandlerDMA1_CH3           /* DMA1 Stream 2                */                   
  .word     BSP_IntHandlerDMA1_CH4           /* DMA1 Stream 3                */                   
  .word     BSP_IntHandlerDMA1_CH5           /* DMA1 Stream 4                */                   
  .word     BSP_IntHandlerDMA1_CH6           /* DMA1 Stream 5                */                   
  .word     BSP_IntHandlerDMA1_CH7           /* DMA1 Stream 6                */                   
  .word     BSP_IntHandlerADC1_2                    /* ADC1, ADC2 and ADC3s         */                   
  .word     BSP_IntHandlerCAN1_TX                /* CAN1 TX                      */                         
  .word     BSP_IntHandlerCAN1_RX0               /* CAN1 RX0                     */                          
  .word     BSP_IntHandlerCAN1_RX1               /* CAN1 RX1                     */                          
  .word     BSP_IntHandlerCAN1_SCE               /* CAN1 SCE                     */                          
  .word     BSP_IntHandlerEXTI9_5                /* External Line[9:5]s          */                          
  .word     BSP_IntHandlerTIM1_BRK          /* TIM1 Break and TIM9          */         
  .word     BSP_IntHandlerTIM1_UP          /* TIM1 Update and TIM10        */         
  .word     BSP_IntHandlerTIM1_TRG_COM     /* TIM1 Trigger and Commutation and TIM11 */
  .word     BSP_IntHandlerTIM1_CC                /* TIM1 Capture Compare         */                          
  .word     BSP_IntHandlerTIM2                   /* TIM2                         */                   
  .word     BSP_IntHandlerTIM3                   /* TIM3                         */                   
  .word     BSP_IntHandlerTIM4                   /* TIM4                         */                   
  .word     BSP_IntHandlerI2C1_EV                /* I2C1 Event                   */                          
  .word     BSP_IntHandlerI2C1_ER                /* I2C1 Error                   */                          
  .word     BSP_IntHandlerI2C2_EV                /* I2C2 Event                   */                          
  .word     BSP_IntHandlerI2C2_ER                /* I2C2 Error                   */                            
  .word     BSP_IntHandlerSPI1                   /* SPI1                         */                   
  .word     BSP_IntHandlerSPI2                   /* SPI2                         */                   
  .word     BSP_IntHandlerUSART1                 /* USART1                       */                   
  .word     BSP_IntHandlerUSART2                 /* USART2                       */                   
  .word     BSP_IntHandlerUSART3                 /* USART3                       */                   
  .word     BSP_IntHandlerEXTI15_10              /* External Line[15:10]s        */                          
  .word     BSP_IntHandlerRTCAlarm              /* RTC Alarm (A and B) through EXTI Line */                 
  .word     BSP_IntHandlerUSBWakeUp            /* USB OTG FS Wakeup through EXTI line */                       
  .word     BSP_IntHandlerTIM8_BRK_TIM12         /* TIM8 Break and TIM12         */         
  .word     BSP_IntHandlerTIM8_UP_TIM13          /* TIM8 Update and TIM13        */         
  .word     BSP_IntHandlerTIM8_TRG_COM_TIM14     /* TIM8 Trigger and Commutation and TIM14 */
  .word     BSP_IntHandlerTIM8_CC                /* TIM8 Capture Compare         */                          
  .word     BSP_IntHandlerDMA1_STREAM7           /* DMA1 Stream7                 */                          
  .word     BSP_IntHandlerFSMC                   /* FSMC                         */                   
  .word     BSP_IntHandlerSDIO                   /* SDIO                         */                   
  .word     BSP_IntHandlerTIM5                   /* TIM5                         */                   
  .word     BSP_IntHandlerSPI3                   /* SPI3                         */                   
  .word     BSP_IntHandlerUSART4                  /* UART4                        */                   
  .word     BSP_IntHandlerUSART5                  /* UART5                        */                   
  .word     BSP_IntHandlerTIM6               /* TIM6 and DAC1&2 underrun errors */                   
  .word     BSP_IntHandlerTIM7                   /* TIM7                         */
  .word     BSP_IntHandlerDMA2_CH1           /* DMA2 Stream 0                */                   
  .word     BSP_IntHandlerDMA2_CH2           /* DMA2 Stream 1                */                   
  .word     BSP_IntHandlerDMA2_CH3           /* DMA2 Stream 2                */                   
  .word     BSP_IntHandlerDMA2_CH4           /* DMA2 Stream 3                */                   
  .word     BSP_IntHandlerDMA2_CH5           /* DMA2 Stream 4                */                   
  .word     BSP_IntHandlerETH                    /* Ethernet                     */                   
  .word     BSP_IntHandlerETHWakeup               /* Ethernet Wakeup through EXTI line */                     
  .word     BSP_IntHandlerCAN2_TX                /* CAN2 TX                      */                          
  .word     BSP_IntHandlerCAN2_RX0               /* CAN2 RX0                     */                          
  .word     BSP_IntHandlerCAN2_RX1               /* CAN2 RX1                     */                          
  .word     BSP_IntHandlerCAN2_SCE               /* CAN2 SCE                     */                          
  .word     BSP_IntHandlerOTG                 /* USB OTG FS                   */                   
  .word     BSP_IntHandlerDMA2_CH6           /* DMA2 Stream 5                */                   
  .word     BSP_IntHandlerDMA2_CH7           /* DMA2 Stream 6                */                   
  .word     BSP_IntHandlerDMA2_CH8           /* DMA2 Stream 7                */                   
  .word     BSP_IntHandlerUSART6                 /* USART6                       */                    
  .word     BSP_IntHandlerI2C3_EV                /* I2C3 event                   */                          
  .word     BSP_IntHandlerI2C3_ER                /* I2C3 error                   */                          
  .word     BSP_IntHandlerOTG_HS_EP1_OUT         /* USB OTG HS End Point 1 Out   */                   
  .word     BSP_IntHandlerOTG_HS_EP1_IN          /* USB OTG HS End Point 1 In    */                   
  .word     BSP_IntHandlerOTG_HS_WKUP            /* USB OTG HS Wakeup through EXTI */                         
  .word     BSP_IntHandlerOTG_HS                 /* USB OTG HS                   */                   
  .word     BSP_IntHandlerDCMI                   /* DCMI                         */                   
  .word     BSP_IntHandlerCRYP                   /* CRYP crypto                  */                   
  .word     BSP_IntHandlerHASH_RNG               /* Hash and Rng                 */
                         
                         
/*******************************************************************************
*
* Provide weak aliases for each Exception handler to the Default_Handler. 
* As they are weak aliases, any function with the same name will override 
* this definition.
* 
*******************************************************************************/
   .weak      App_NMI_ISR
   .thumb_set App_NMI_ISR,Default_Handler
  
   .weak      App_Fault_ISR
   .thumb_set App_Fault_ISR,Default_Handler
  
   .weak      App_MemFault_ISR
   .thumb_set App_MemFault_ISR,Default_Handler
  
   .weak      App_BusFault_ISR
   .thumb_set App_BusFault_ISR,Default_Handler

   .weak      App_UsageFault_ISR
   .thumb_set App_UsageFault_ISR,Default_Handler

   .weak      App_Spurious_ISR
   .thumb_set App_Spurious_ISR,Default_Handler

   .weak      App_Reserved_ISR
   .thumb_set App_Reserved_ISR,Default_Handler

   .weak      OS_CPU_PendSVHandler
   .thumb_set OS_CPU_PendSVHandler,Default_Handler

   .weak      OS_CPU_SysTickHandler
   .thumb_set OS_CPU_SysTickHandler,Default_Handler              
  
   .weak      BSP_IntHandlerWWDG                   
   .thumb_set BSP_IntHandlerWWDG,Default_Handler      
                  
   .weak      BSP_IntHandlerPVD      
   .thumb_set BSP_IntHandlerPVD,Default_Handler
               
   .weak      BSP_IntHandlerTAMPER            
   .thumb_set BSP_IntHandlerTAMPER,Default_Handler
            
   .weak      BSP_IntHandlerRTC                  
   .thumb_set BSP_IntHandlerRTC,Default_Handler
            
   .weak      BSP_IntHandlerFLASH         
   .thumb_set BSP_IntHandlerFLASH,Default_Handler
                  
   .weak      BSP_IntHandlerRCC      
   .thumb_set BSP_IntHandlerRCC,Default_Handler
                  
   .weak      BSP_IntHandlerEXTI0         
   .thumb_set BSP_IntHandlerEXTI0,Default_Handler
                  
   .weak      BSP_IntHandlerEXTI1         
   .thumb_set BSP_IntHandlerEXTI1,Default_Handler
                     
   .weak      BSP_IntHandlerEXTI2         
   .thumb_set BSP_IntHandlerEXTI2,Default_Handler 
                 
   .weak      BSP_IntHandlerEXTI3         
   .thumb_set BSP_IntHandlerEXTI3,Default_Handler
                        
   .weak      BSP_IntHandlerEXTI4         
   .thumb_set BSP_IntHandlerEXTI4,Default_Handler
                  
   .weak      BSP_IntHandlerDMA1_CH1               
   .thumb_set BSP_IntHandlerDMA1_CH1,Default_Handler
         
   .weak      BSP_IntHandlerDMA1_CH2               
   .thumb_set BSP_IntHandlerDMA1_CH2,Default_Handler
                  
   .weak      BSP_IntHandlerDMA1_CH3               
   .thumb_set BSP_IntHandlerDMA1_CH3,Default_Handler
                  
   .weak      BSP_IntHandlerDMA1_CH4               
   .thumb_set BSP_IntHandlerDMA1_CH4,Default_Handler 
                 
   .weak      BSP_IntHandlerDMA1_CH5              
   .thumb_set BSP_IntHandlerDMA1_CH5,Default_Handler
                  
   .weak      BSP_IntHandlerDMA1_CH6               
   .thumb_set BSP_IntHandlerDMA1_CH6,Default_Handler
                  
   .weak      BSP_IntHandlerDMA1_CH7               
   .thumb_set BSP_IntHandlerDMA1_CH7,Default_Handler
                  
   .weak      BSP_IntHandlerADC1_2      
   .thumb_set BSP_IntHandlerADC1_2,Default_Handler
               
   .weak      BSP_IntHandlerCAN1_TX   
   .thumb_set BSP_IntHandlerCAN1_TX,Default_Handler
            
   .weak      BSP_IntHandlerCAN1_RX0                  
   .thumb_set BSP_IntHandlerCAN1_RX0,Default_Handler
                           
   .weak      BSP_IntHandlerCAN1_RX1                  
   .thumb_set BSP_IntHandlerCAN1_RX1,Default_Handler
            
   .weak      BSP_IntHandlerCAN1_SCE                  
   .thumb_set BSP_IntHandlerCAN1_SCE,Default_Handler
            
   .weak      BSP_IntHandlerEXTI9_5   
   .thumb_set BSP_IntHandlerEXTI9_5,Default_Handler
            
   .weak      BSP_IntHandlerTIM1_BRK            
   .thumb_set BSP_IntHandlerTIM1_BRK,Default_Handler
            
   .weak      BSP_IntHandlerTIM1_UP            
   .thumb_set BSP_IntHandlerTIM1_UP,Default_Handler
      
   .weak      BSP_IntHandlerTIM1_TRG_COM      
   .thumb_set BSP_IntHandlerTIM1_TRG_COM,Default_Handler
      
   .weak      BSP_IntHandlerTIM1_CC   
   .thumb_set BSP_IntHandlerTIM1_CC,Default_Handler
                  
   .weak      BSP_IntHandlerTIM2            
   .thumb_set BSP_IntHandlerTIM2,Default_Handler
                  
   .weak      BSP_IntHandlerTIM3            
   .thumb_set BSP_IntHandlerTIM3,Default_Handler
                  
   .weak      BSP_IntHandlerTIM4            
   .thumb_set BSP_IntHandlerTIM4,Default_Handler
                  
   .weak      BSP_IntHandlerI2C1_EV   
   .thumb_set BSP_IntHandlerI2C1_EV,Default_Handler
                     
   .weak      BSP_IntHandlerI2C1_ER   
   .thumb_set BSP_IntHandlerI2C1_ER,Default_Handler
                     
   .weak      BSP_IntHandlerI2C2_EV   
   .thumb_set BSP_IntHandlerI2C2_EV,Default_Handler
                  
   .weak      BSP_IntHandlerI2C2_ER   
   .thumb_set BSP_IntHandlerI2C2_ER,Default_Handler
                           
   .weak      BSP_IntHandlerSPI1            
   .thumb_set BSP_IntHandlerSPI1,Default_Handler
                        
   .weak      BSP_IntHandlerSPI2            
   .thumb_set BSP_IntHandlerSPI2,Default_Handler
                  
   .weak      BSP_IntHandlerUSART1      
   .thumb_set BSP_IntHandlerUSART1,Default_Handler
                     
   .weak      BSP_IntHandlerUSART2      
   .thumb_set BSP_IntHandlerUSART2,Default_Handler
                     
   .weak      BSP_IntHandlerUSART3      
   .thumb_set BSP_IntHandlerUSART3,Default_Handler
                  
   .weak      BSP_IntHandlerEXTI15_10               
   .thumb_set BSP_IntHandlerEXTI15_10,Default_Handler
               
   .weak      BSP_IntHandlerRTCAlarm               
   .thumb_set BSP_IntHandlerRTCAlarm,Default_Handler
            
   .weak      BSP_IntHandlerUSBWakeUp         
   .thumb_set BSP_IntHandlerUSBWakeUp,Default_Handler
            
   .weak      BSP_IntHandlerTIM8_BRK_TIM12         
   .thumb_set BSP_IntHandlerTIM8_BRK_TIM12,Default_Handler
         
   .weak      BSP_IntHandlerTIM8_UP_TIM13            
   .thumb_set BSP_IntHandlerTIM8_UP_TIM13,Default_Handler
         
   .weak      BSP_IntHandlerTIM8_TRG_COM_TIM14      
   .thumb_set BSP_IntHandlerTIM8_TRG_COM_TIM14,Default_Handler
      
   .weak      BSP_IntHandlerTIM8_CC   
   .thumb_set BSP_IntHandlerTIM8_CC,Default_Handler
                  
   .weak      BSP_IntHandlerDMA1_STREAM7               
   .thumb_set BSP_IntHandlerDMA1_STREAM7,Default_Handler
                     
   .weak      BSP_IntHandlerFSMC            
   .thumb_set BSP_IntHandlerFSMC,Default_Handler
                     
   .weak      BSP_IntHandlerSDIO            
   .thumb_set BSP_IntHandlerSDIO,Default_Handler
                     
   .weak      BSP_IntHandlerTIM5            
   .thumb_set BSP_IntHandlerTIM5,Default_Handler
                     
   .weak      BSP_IntHandlerSPI3            
   .thumb_set BSP_IntHandlerSPI3,Default_Handler
                     
   .weak      BSP_IntHandlerUSART4         
   .thumb_set BSP_IntHandlerUSART4,Default_Handler
                  
   .weak      BSP_IntHandlerUSART5         
   .thumb_set BSP_IntHandlerUSART5,Default_Handler
                  
   .weak      BSP_IntHandlerTIM6                  
   .thumb_set BSP_IntHandlerTIM6,Default_Handler
               
   .weak      BSP_IntHandlerTIM7            
   .thumb_set BSP_IntHandlerTIM7,Default_Handler
         
   .weak      BSP_IntHandlerDMA2_CH1               
   .thumb_set BSP_IntHandlerDMA2_CH1,Default_Handler
               
   .weak      BSP_IntHandlerDMA2_CH2               
   .thumb_set BSP_IntHandlerDMA2_CH2,Default_Handler
                  
   .weak      BSP_IntHandlerDMA2_CH3               
   .thumb_set BSP_IntHandlerDMA2_CH3,Default_Handler
            
   .weak      BSP_IntHandlerDMA2_CH4               
   .thumb_set BSP_IntHandlerDMA2_CH4,Default_Handler
            
   .weak      BSP_IntHandlerDMA2_CH5               
   .thumb_set BSP_IntHandlerDMA2_CH5,Default_Handler
            
   .weak      BSP_IntHandlerETH      
   .thumb_set BSP_IntHandlerETH,Default_Handler
                  
   .weak      BSP_IntHandlerETHWakeup                  
   .thumb_set BSP_IntHandlerETHWakeup,Default_Handler
            
   .weak      BSP_IntHandlerCAN2_TX   
   .thumb_set BSP_IntHandlerCAN2_TX,Default_Handler
                           
   .weak      BSP_IntHandlerCAN2_RX0                  
   .thumb_set BSP_IntHandlerCAN2_RX0,Default_Handler
                           
   .weak      BSP_IntHandlerCAN2_RX1                  
   .thumb_set BSP_IntHandlerCAN2_RX1,Default_Handler
                           
   .weak      BSP_IntHandlerCAN2_SCE                  
   .thumb_set BSP_IntHandlerCAN2_SCE,Default_Handler
                           
   .weak      BSP_IntHandlerOTG      
   .thumb_set BSP_IntHandlerOTG,Default_Handler
                     
   .weak      BSP_IntHandlerDMA2_CH6               
   .thumb_set BSP_IntHandlerDMA2_CH6,Default_Handler
                  
   .weak      BSP_IntHandlerDMA2_CH7               
   .thumb_set BSP_IntHandlerDMA2_CH7,Default_Handler
                  
   .weak      BSP_IntHandlerDMA2_CH8               
   .thumb_set BSP_IntHandlerDMA2_CH8,Default_Handler
                  
   .weak      BSP_IntHandlerUSART6      
   .thumb_set BSP_IntHandlerUSART6,Default_Handler
                        
   .weak      BSP_IntHandlerI2C3_EV   
   .thumb_set BSP_IntHandlerI2C3_EV,Default_Handler
                        
   .weak      BSP_IntHandlerI2C3_ER   
   .thumb_set BSP_IntHandlerI2C3_ER,Default_Handler
                        
   .weak      BSP_IntHandlerOTG_HS_EP1_OUT         
   .thumb_set BSP_IntHandlerOTG_HS_EP1_OUT,Default_Handler
               
   .weak      BSP_IntHandlerOTG_HS_EP1_IN            
   .thumb_set BSP_IntHandlerOTG_HS_EP1_IN,Default_Handler
               
   .weak      BSP_IntHandlerOTG_HS_WKUP         
   .thumb_set BSP_IntHandlerOTG_HS_WKUP,Default_Handler
            
   .weak      BSP_IntHandlerOTG_HS      
   .thumb_set BSP_IntHandlerOTG_HS,Default_Handler
                  
   .weak      BSP_IntHandlerDCMI            
   .thumb_set BSP_IntHandlerDCMI,Default_Handler
                     
   .weak      BSP_IntHandlerCRYP            
   .thumb_set BSP_IntHandlerCRYP,Default_Handler
               
   .weak      BSP_IntHandlerHASH_RNG                  
   .thumb_set BSP_IntHandlerHASH_RNG,Default_Handler   


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
