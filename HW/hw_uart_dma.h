#ifndef __HW_DMA_H
#define	__HW_DMA_H	   

#include "stm32f2xx_dma.h" 

#define USART2_DMA_BUF_LEN (256)

// DMA Tx Config
void HW_DMA_Tx_Config(DMA_Stream_TypeDef *DMA_Streamx, unsigned char irq_ch, unsigned int chx,unsigned int par,unsigned int mar,unsigned short ndtr);

// DMA Rx Config
void HW_DMA_Rx_Config(USART_TypeDef *USART, DMA_Stream_TypeDef *DMA_Streamx, unsigned char irq_ch, unsigned int chx,unsigned int par,unsigned int mar,unsigned short ndtr);

// DMA Enable
void HW_DMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr);

#endif

