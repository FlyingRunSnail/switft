
#ifndef __HW_BEACON__
#define __HW_BEACON__

#ifdef HW_BEACON_DEF
	#define HW_BEACON_GLOBE
#else
	#define HW_BEACON_GLOBE		extern
#endif

#include <bsp_uart.h>




#define		HW_BEACON_UART_TX_PIN_PORT		GPIOC
#define		HW_BEACON_UART_TX_PIN			GPIO_Pin_6

#define		HW_BEACON_UART_RX_PIN_PORT		GPIOC
#define		HW_BEACON_UART_RX_PIN			GPIO_Pin_7

#define 		HW_BEACON_UARTX			USART6								// 信标机串口号
#define		SWIFT_UART6_INT_ID	BSP_INT_ID_USART6					// 信标机串口中断ID


HW_BEACON_GLOBE OS_SEM Beacon2MCU_Sem;

HW_BEACON_GLOBE OS_SEM SWIFT_UART6_Rev_Sem;

HW_BEACON_GLOBE void SWIFT_UART6_Init(INT32U	BaudRate);

HW_BEACON_GLOBE void SWIFT_UART6_IntHandler(void);

HW_BEACON_GLOBE void SWIFT_UART6_INT_Switch(INT8U switch_set);

HW_BEACON_GLOBE void SWIFT_UART6_SendBuff(INT8S *buff, INT32U bufflen);

HW_BEACON_GLOBE void SWIFT_UART6_RecvBuff(INT8S *buff, INT32U *bufflen);

HW_BEACON_GLOBE INT32S SWIFT_USART6_GETC(void);

HW_BEACON_GLOBE INT32S SWIFT_USART6_TSTC(void);

#endif

