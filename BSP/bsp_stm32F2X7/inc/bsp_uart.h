#ifndef __BSP_UART__
#define __BSP_UART__

#ifdef BSP_UART_DEF
	#define BSP_UART_GLOBE
#else
	#define BSP_UART_GLOBE		extern
#endif

#include "stm32f2xx.h"
#include  <bsp.h>
//#include  <devdef.h>

#define UART_GPIO_TX_INDEX		0
#define UART_GPIO_RX_INDEX		1

// 串口的一些结构体
typedef struct _str_UART_GPIO_HARD_
{
	GPIO_TypeDef* GPIOx[2];
	GPIO_InitTypeDef GPIO_InitStruct[2];
	USART_TypeDef	* USARTx;
	USART_InitTypeDef	USART_InitStruct;
}str_UART_GPIO_HARD;


typedef struct _str_UART_Buff_
{
	INT8U *pbuff;
	INT32U write_p;
	INT32U read_p;
}str_UART_Buff;


BSP_UART_GLOBE void BSP_UART_PortClkSwitch(USART_TypeDef* USARTx,INT8U switch_set);

BSP_UART_GLOBE void BSP_UART_Init(str_UART_GPIO_HARD UART_GPIO_HARD);

BSP_UART_GLOBE void BSP_UART_Send(USART_TypeDef* USARTx, INT8U *sendbuff,	INT32U len);

BSP_UART_GLOBE INT16U BSP_UART_RCV(USART_TypeDef * USARTx);

BSP_UART_GLOBE INT16U BSP_UART_TST(USART_TypeDef * USARTx);

#endif
