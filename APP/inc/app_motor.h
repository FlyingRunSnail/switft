
#include <hw_pducomm.h>

#ifdef 	APP_MOTOR_DEF
	#define	MOTOR_GLOBE
#else
	#define	MOTOR_GLOBE	extern
#endif

#define MCU_LED_NUM0_PIN_PORT			GPIOF
#define MCU_LED_NUM0_PIN_PORT_CLK		RCC_AHB1Periph_GPIOF
#define MCU_LED_NUM0_PIN					GPIO_Pin_10

#define MCU_LED_LIGHT_ON		0
#define MCU_LED_LIGHT_OFF		1


#define MOTOR_POSITION_CLKWISE_PIN_PORT					GPIOA
#define MOTOR_POSITION_CLKWISE_PIN_PORT_CLK			RCC_AHB1Periph_GPIOA
#define MOTOR_POSITION_CLKWISE_PIN						GPIO_Pin_4

#define MOTOR_POSITION_ANTICLKWISE_PIN_PORT			GPIOA
#define MOTOR_POSITION_ANTICLKWISE_PIN_PORT_CLK		RCC_AHB1Periph_GPIOA
#define MOTOR_POSITION_ANTICLKWISE_PIN					GPIO_Pin_5

#define MOTOR_CABRAGE_UP_PIN_PORT						GPIOB
#define MOTOR_CABRAGE_UP_PIN_PORT_CLK					RCC_AHB1Periph_GPIOB
#define MOTOR_CABRAGE_UP_PIN								GPIO_Pin_0

#define MOTOR_CABRAGE_DOWN_PIN_PORT					GPIOB
#define MOTOR_CABRAGE_DOWN_PIN_PORT_CLK				RCC_AHB1Periph_GPIOB
#define MOTOR_CABRAGE_DOWN_PIN							GPIO_Pin_1

#define MOTOR_POLARIZATION_CLKWISE_PIN_PORT				GPIOE
#define MOTOR_POLARIZATION_CLKWISE_PIN_PORT_CLK			RCC_AHB1Periph_GPIOE
#define MOTOR_POLARIZATION_CLKWISE_PIN						GPIO_Pin_4

#define MOTOR_POLARIZATION_ANTICLKWISE_PIN_PORT			GPIOE
#define MOTOR_POLARIZATION_ANTICLKWISE_PIN_PORT_CLK		RCC_AHB1Periph_GPIOE
#define MOTOR_POLARIZATION_ANTICLKWISE_PIN					GPIO_Pin_6



MOTOR_GLOBE int Motor_Init(void);


/***********************************************************
**name:	Motor_DriveStart
**describe: 对电机启动
**input:	Motor_num: 电机编号
**output:	none
**return:  
**autor:  andiman
**date:
************************************************************/
MOTOR_GLOBE int Motor_DriveStart(INT8U Motor_num,INT8U direction);




/***********************************************************
**name:	Motor_DriveEnd
**describe: 对电机停止驱动
**input:	Motor_num: 电机编号
**output:	none
**return:  
**autor:  andiman
**date:
************************************************************/
MOTOR_GLOBE int  Motor_DriveEnd(INT8U Motor_num,INT8U direction);

MOTOR_GLOBE int Motor_TimeConfigDrive(INT8U Motor_num,INT8U drection,INT16U ms);

MOTOR_GLOBE int Antenna_DriveStart(INT8U antenna_part,INT8U direction);

MOTOR_GLOBE int Antenna_DriveEnd(INT8U antenna_part,INT8U direction);

MOTOR_GLOBE int Antenna_AllDriveEnd(void);

