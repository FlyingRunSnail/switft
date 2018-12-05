
#ifndef __HW_UART6__
#define __HW_UART6__

#ifdef HW_UART6_DEF
	#define HW_UART6_GLOBE
#else
	#define HW_UART6_GLOBE		extern
#endif

#include <bsp_uart.h>


#define		SWIFT_UART6_TX_PIN_PORT		GPIOC
#define		SWIFT_UART6_TX_PIN			GPIO_Pin_6

#define		SWIFT_UART6_RX_PIN_PORT		GPIOC
#define		SWIFT_UART6_RX_PIN			GPIO_Pin_7

#define 	SWIFT_UART6_UARTX			USART6								// 信标机串口号
#define		SWIFT_UART6_INT_ID	BSP_INT_ID_USART6					// 信标机串口中断ID


HW_UART6_GLOBE OS_SEM SWIFT_UART6_Rev_Sem;

HW_UART6_GLOBE void SWIFT_UART6_Init(INT32U	BaudRate);

HW_UART6_GLOBE void SWIFT_UART6_IntHandler(void);

HW_UART6_GLOBE void SWIFT_UART6_INT_Switch(INT8U switch_set);

HW_UART6_GLOBE void SWIFT_UART6_SendBuff(INT8S *buff, INT32U bufflen);

HW_UART6_GLOBE void SWIFT_UART6_RecvBuff(INT8S *buff, INT32U *bufflen);

HW_UART6_GLOBE INT32S SWIFT_USART6_GETC(void);

HW_UART6_GLOBE INT32S SWIFT_USART6_TSTC(void);

#endif

