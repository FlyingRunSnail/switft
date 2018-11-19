

#ifdef IC_AD7814_DEF
	#define IC_AD7814_GLOBE
#else
	#define IC_AD7814_GLOBE	extern
#endif

#ifndef SUCC
	#define SUCC 0
#endif
#ifndef	FAIL
	#define FAIL (-1)
#endif


#include "stm32f2xx.h"
#include "stm32f2xx_gpio.h"
#include <bsp_spi_hard.h>



#define		AD7814_SCK_PIN_PORT			GPIOA
#define		AD7814_SCK_PIN					GPIO_Pin_5

#define		AD7814_MOSI_PIN_PORT			GPIOB
#define		AD7814_MOSI_PIN					GPIO_Pin_5

#define		AD7814_MISO_PIN_PORT			GPIOA
#define		AD7814_MISO_PIN					GPIO_Pin_6

#define		AD7814_CS_PIN_PORT				GPIOA
#define		AD7814_CS_PIN					GPIO_Pin_4

#define		AD7814_SPI_PORT					SPI1



IC_AD7814_GLOBE  INT32S AD7814_TempGet(void);



















