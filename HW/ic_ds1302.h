

#ifdef IC_DS1302_DEF
	#define IC_DS1302_GLOBE
#else
	#define IC_DS1302_GLOBE	extern
#endif

#ifndef SUCC
	#define SUCC 0
#endif
#ifndef	FAIL
	#define FAIL (-1)
#endif


#include "stm32f2xx.h"
#include "stm32f2xx_gpio.h"
#include <bsp_gpio.h>



#define		DS1302_SCK_PIN_PORT			GPIOC
#define		DS1302_SCK_PIN					GPIO_Pin_0

#define		DS1302_DATA_PIN_PORT			GPIOC
#define		DS1302_DATA_PIN					GPIO_Pin_2

#define		DS1302_CS_PIN_PORT				GPIOC
#define		DS1302_CS_PIN					GPIO_Pin_3





IC_DS1302_GLOBE INT8U  DS1302_ReadSecond(void);

IC_DS1302_GLOBE int  DS1302_WriteSecond(INT8U sec);

IC_DS1302_GLOBE INT8U  DS1302_ReadMinute(void);

IC_DS1302_GLOBE int  DS1302_WriteMinute(INT8U min);

IC_DS1302_GLOBE INT8U  DS1302_ReadHour(void);

IC_DS1302_GLOBE int  DS1302_WriteHour(INT8U hour);

IC_DS1302_GLOBE INT8U  DS1302_ReadDay(void);

IC_DS1302_GLOBE int  DS1302_WriteDay(INT8U day);

IC_DS1302_GLOBE INT8U  DS1302_ReadMonth(void);

IC_DS1302_GLOBE int  DS1302_WriteMonth(INT8U month);

IC_DS1302_GLOBE INT8U  DS1302_ReadYear(void);

IC_DS1302_GLOBE int  DS1302_WriteYear(INT8U year);

IC_DS1302_GLOBE INT8U  DS1302_ReadWeekday(void);

IC_DS1302_GLOBE int  DS1302_WriteWeekday(INT8U weekday);

IC_DS1302_GLOBE void DS1302_Init(void);
















