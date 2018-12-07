

#ifdef HW_UART2_DEF
	#define HW_UART2_GLOBE
#else
	#define HW_UART2_GLOBE		extern
#endif

#include <bsp_uart.h>

// UART2
#define		SWIFT_UART2_TX_PIN_PORT		GPIOD
#define		SWIFT_UART2_TX_PIN			GPIO_Pin_5

#define		SWIFT_UART2_RX_PIN_PORT		GPIOD
#define		SWIFT_UART2_RX_PIN			GPIO_Pin_6

#define 	SWIFT_UART2					USART2								
#define		HW_UART2_INT_ID				BSP_INT_ID_USART2	

HW_UART2_GLOBE OS_SEM SWIFT_UART2_Rev_Sem;

HW_UART2_GLOBE void SWIFT_UART2_IntHandler(void);

HW_UART2_GLOBE void SWIFT_UART2_INT_Switch(INT8U switch_set);

HW_UART2_GLOBE void SWIFT_UART2_Init(INT32U	BaudRate);

HW_UART2_GLOBE void SWIFT_UART2_SendBuff(INT8U *buff, INT32U bufflen);

HW_UART2_GLOBE void SWIFT_UART2_RecvBuff(INT8U *buff, INT32U *bufflen);

HW_UART2_GLOBE void SWIFT_UART2_RecvBuff_Clear(void);

HW_UART2_GLOBE char SWIFT_USART2_GETC(void);
 
HW_UART2_GLOBE INT32S SWIFT_USART2_TSTC(void);

