
#include <bsp_uart.h>

#ifdef 	HW_PDU_DEF
	#define	HW_PDU_GLOBE
#else
	#define	HW_PDU_GLOBE	extern
#endif

#define		SWIFT_UART4_TX_PIN_PORT		GPIOC
#define		SWIFT_UART4_TX_PIN			GPIO_Pin_10

#define		SWIFT_UART4_RX_PIN_PORT		GPIOC
#define		SWIFT_UART4_RX_PIN			GPIO_Pin_11

#define 	SWIFT_UART4					UART4								
#define		SWIFT_UART4_INT_ID			BSP_INT_ID_USART4					


HW_PDU_GLOBE OS_SEM SWIFT_UART4_Rev_Sem;

HW_PDU_GLOBE void SWITFT_UART4_IntHandler(void);

HW_PDU_GLOBE void SWIFT_UART4_INT_Switch(INT8U switch_set);

HW_PDU_GLOBE void SWIFT_UART4_Init(INT32U BaudRate);

HW_PDU_GLOBE void SWIFT_UART4_SendBuff(INT8U *buff, INT32U bufflen);

HW_PDU_GLOBE void SWIFT_UART4_RecvBuff(INT8U *buff, INT32U *bufflen);

