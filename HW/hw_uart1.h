

#ifdef HW_BEACON_DEF
	#define HW_BEACON_GLOBE
#else
	#define HW_BEACON_GLOBE		extern
#endif

#include <bsp_uart.h>


// UART1
#define		SWIFT_UART1_TX_PIN_PORT		GPIOA
#define		SWIFT_UART1_TX_PIN			GPIO_Pin_9

#define		SWIFT_UART1_RX_PIN_PORT		GPIOA
#define		SWIFT_UART1_RX_PIN			GPIO_Pin_10

#define 	HW_UART1					USART1								
#define		HW_UART1_INT_ID				BSP_INT_ID_USART1	


HW_BEACON_GLOBE OS_SEM SWIFT_UART1_Rev_Sem;


HW_BEACON_GLOBE void SWIFT_UART1_IntHandler(void);

HW_BEACON_GLOBE void SWIFT_UART1_INT_Switch(INT8U switch_set);

HW_BEACON_GLOBE void SWIFT_UART1_Init(INT32U	BaudRate);

HW_BEACON_GLOBE void SWIFT_UART1_SendBuff(INT8U *buff, INT32U bufflen);

HW_BEACON_GLOBE void SWIFT_UART1_RecvBuff(INT8U *buff, INT32U *bufflen);

HW_BEACON_GLOBE void SWIFT_UART1_RecvBuff_Clear(void);


