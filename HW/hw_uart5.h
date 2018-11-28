
#ifdef HW_BEACON_DEF
	#define HW_BEACON_GLOBE
#else
	#define HW_BEACON_GLOBE		extern
#endif

#include <bsp_uart.h>

// UART5
#define		SWIFT_UART5_TX_PIN_PORT		GPIOC
#define		SWIFT_UART5_TX_PIN				GPIO_Pin_12

#define		SWIFT_UART5_RX_PIN_PORT		GPIOD
#define		SWIFT_UART5_RX_PIN				GPIO_Pin_2

#define 	SWIFT_UART5					UART5								
#define		HW_UART5_INT_ID				BSP_INT_ID_USART5			


HW_BEACON_GLOBE OS_SEM SWIFT_UART5_Rev_Sem;


HW_BEACON_GLOBE void SWIFT_UART5_IntHandler(void);

HW_BEACON_GLOBE void SWIFT_UART5_INT_Switch(INT8U switch_set);

HW_BEACON_GLOBE void SWIFT_UART5_Init(INT32U	BaudRate);

HW_BEACON_GLOBE void SWIFT_UART5_SendBuff(INT8U *buff, INT32U bufflen);

HW_BEACON_GLOBE void SWIFT_UART5_RecvBuff(INT8U *buff, INT32U *bufflen);

HW_BEACON_GLOBE void SWIFT_UART5_RecvBuff_Clear(void);

