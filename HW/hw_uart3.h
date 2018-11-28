
#include <bsp.h>

#ifdef 	HW_OLED_DEF
	#define	HW_OLED_GLOBE
#else
	#define	HW_OLED_GLOBE	extern
#endif

#define		SWIFT_UART3_TX_PIN_PORT		GPIOD
#define		SWIFT_UART3_TX_PIN			GPIO_Pin_8

#define		SWIFT_UART3_RX_PIN_PORT		GPIOD
#define		SWIFT_UART3_RX_PIN			GPIO_Pin_9

#define 	SWIFT_UART3					USART3								
#define		SWIFT_UART3_INT_ID			BSP_INT_ID_USART3					




HW_OLED_GLOBE OS_SEM SWIFT_UART3_Rev_Sem;

HW_OLED_GLOBE void SWIFT_UART3_IntHandler(void);

HW_OLED_GLOBE void SWIFT_UART3_INT_Switch(INT8U switch_set);

HW_OLED_GLOBE void SWIFT_UART3_Init(INT32U BaudRate);

HW_OLED_GLOBE void SWIFT_UART3_SendBuff(INT8U *buff, INT32U bufflen);

HW_OLED_GLOBE void SWIFT_UART3_RecvBuff(INT8U *buff, INT32U *bufflen);

