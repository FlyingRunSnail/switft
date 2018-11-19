

#ifdef HW_BEACON_DEF
	#define HW_BEACON_GLOBE
#else
	#define HW_BEACON_GLOBE		extern
#endif

#include <bsp_uart.h>



//  方位
#define		SWIFT_UART5_TX_PIN_PORT		GPIOC
#define		SWIFT_UART5_TX_PIN				GPIO_Pin_12

#define		SWIFT_UART5_RX_PIN_PORT		GPIOD
#define		SWIFT_UART5_RX_PIN				GPIO_Pin_2

#define 		SWIFT_UART5					UART5								
#define		HW_PositionElecCoder_UART_INT_ID				BSP_INT_ID_USART5			

#define 		HW_Position_RS422_DUPLEX_SWITCH_PIN_PORT_CLK		RCC_AHB1Periph_GPIOE
#define		HW_Position_RS422_DUPLEX_SWITCH_PIN_PORT			GPIOE
#define		HW_Position_RS422_DUPLEX_SWITCH_PIN					GPIO_Pin_4

#define 		HW_Position_RS422_RX_SWITCH_PIN_PORT_CLK			RCC_AHB1Periph_GPIOE
#define		HW_Position_RS422_RX_SWITCH_PIN_PORT				GPIOE
#define		HW_Position_RS422_RX_SWITCH_PIN						GPIO_Pin_3

#define 		HW_Position_RS422_TX_SWITCH_PIN_PORT_CLK			RCC_AHB1Periph_GPIOE
#define		HW_Position_RS422_TX_SWITCH_PIN_PORT				GPIOE
#define		HW_Position_RS422_TX_SWITCH_PIN						GPIO_Pin_1

#define 		HW_Position_RS422_RATE_PIN_PORT_CLK					RCC_AHB1Periph_GPIOE
#define		HW_Position_RS422_RATE_PIN_PORT						GPIOE
#define		HW_Position_RS422_RATE_PIN							GPIO_Pin_0

// 俯仰
#define		SWIFT_UART2_TX_PIN_PORT		GPIOD
#define		SWIFT_UART2_TX_PIN				GPIO_Pin_5

#define		SWIFT_UART2_RX_PIN_PORT		GPIOD
#define		SWIFT_UART2_RX_PIN				GPIO_Pin_6

#define 		SWIFT_UART2					USART2								
#define		HW_CabrageElecCoder_UART_INT_ID				BSP_INT_ID_USART2	

#define 		HW_Cabrage_RS422_DUPLEX_SWITCH_PIN_PORT_CLK		RCC_AHB1Periph_GPIOF
#define		HW_Cabrage_RS422_DUPLEX_SWITCH_PIN_PORT			GPIOF
#define		HW_Cabrage_RS422_DUPLEX_SWITCH_PIN					GPIO_Pin_3

#define 		HW_Cabrage_RS422_RX_SWITCH_PIN_PORT_CLK			RCC_AHB1Periph_GPIOF
#define		HW_Cabrage_RS422_RX_SWITCH_PIN_PORT				GPIOF
#define		HW_Cabrage_RS422_RX_SWITCH_PIN						GPIO_Pin_2

#define 		HW_Cabrage_RS422_TX_SWITCH_PIN_PORT_CLK			RCC_AHB1Periph_GPIOE
#define		HW_Cabrage_RS422_TX_SWITCH_PIN_PORT				GPIOE
#define		HW_Cabrage_RS422_TX_SWITCH_PIN						GPIO_Pin_6

#define 		HW_Cabrage_RS422_RATE_PIN_PORT_CLK					RCC_AHB1Periph_GPIOE
#define		HW_Cabrage_RS422_RATE_PIN_PORT						GPIOE
#define		HW_Cabrage_RS422_RATE_PIN							GPIO_Pin_5

// 极化
#define		SWIFT_UART1_TX_PIN_PORT		GPIOA
#define		SWIFT_UART1_TX_PIN				GPIO_Pin_9

#define		SWIFT_UART1_RX_PIN_PORT		GPIOA
#define		SWIFT_UART1_RX_PIN				GPIO_Pin_10

#define 		HW_PolarElecCoder_UARTX					USART1								
#define		HW_PolarElecCoder_UART_INT_ID				BSP_INT_ID_USART1	

#define 		HW_Polar_RS422_DUPLEX_SWITCH_PIN_PORT_CLK		RCC_AHB1Periph_GPIOG
#define		HW_Polar_RS422_DUPLEX_SWITCH_PIN_PORT			GPIOG
#define		HW_Polar_RS422_DUPLEX_SWITCH_PIN				GPIO_Pin_7

#define 		HW_Polar_RS422_RX_SWITCH_PIN_PORT_CLK			RCC_AHB1Periph_GPIOG
#define		HW_Polar_RS422_RX_SWITCH_PIN_PORT				GPIOG
#define		HW_Polar_RS422_RX_SWITCH_PIN					GPIO_Pin_6

#define 		HW_Polar_RS422_TX_SWITCH_PIN_PORT_CLK			RCC_AHB1Periph_GPIOG
#define		HW_Polar_RS422_TX_SWITCH_PIN_PORT				GPIOG
#define		HW_Polar_RS422_TX_SWITCH_PIN					GPIO_Pin_5

#define 		HW_Polar_RS422_RATE_PIN_PORT_CLK				RCC_AHB1Periph_GPIOG
#define		HW_Polar_RS422_RATE_PIN_PORT					GPIOG
#define		HW_Polar_RS422_RATE_PIN							GPIO_Pin_4



HW_BEACON_GLOBE OS_SEM SWIFT_UART5_Rev_Sem;

HW_BEACON_GLOBE OS_SEM SWIFT_UART2_Rev_Sem;

HW_BEACON_GLOBE OS_SEM SWIFT_UART1_Rev_Sem;



HW_BEACON_GLOBE void SWIFT_UART5_IntHandler(void);

HW_BEACON_GLOBE void SWIFT_UART5_INT_Switch(INT8U switch_set);

HW_BEACON_GLOBE void SWIFT_UART5_Init(INT32U	BaudRate);

HW_BEACON_GLOBE void SWIFT_UART2_IntHandler(void);

HW_BEACON_GLOBE void SWIFT_UART2_INT_Switch(INT8U switch_set);

HW_BEACON_GLOBE void SWIFT_UART2_Init(INT32U	BaudRate);

HW_BEACON_GLOBE void SWIFT_UART1_IntHandler(void);

HW_BEACON_GLOBE void SWIFT_UART1_INT_Switch(INT8U switch_set);

HW_BEACON_GLOBE void SWIFT_UART1_Init(INT32U	BaudRate);

HW_BEACON_GLOBE void SWIFT_UART5_SendBuff(INT8U *buff, INT32U bufflen);

HW_BEACON_GLOBE void SWIFT_UART5_RecvBuff(INT8U *buff, INT32U *bufflen);

HW_BEACON_GLOBE void SWIFT_UART2_SendBuff(INT8U *buff, INT32U bufflen);

HW_BEACON_GLOBE void SWIFT_UART2_RecvBuff(INT8U *buff, INT32U *bufflen);

HW_BEACON_GLOBE void SWIFT_UART1_SendBuff(INT8U *buff, INT32U bufflen);

HW_BEACON_GLOBE void SWIFT_UART1_RecvBuff(INT8U *buff, INT32U *bufflen);

HW_BEACON_GLOBE void SWIFT_UART5_RecvBuff_Clear(void);

HW_BEACON_GLOBE void SWIFT_UART2_RecvBuff_Clear(void);

HW_BEACON_GLOBE void SWIFT_UART1_RecvBuff_Clear(void);


