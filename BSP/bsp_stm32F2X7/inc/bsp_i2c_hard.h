
#ifdef		BSP_I2C_HARD_DEF
	#define		BSP_I2C_HARD_GLOBE
#else
	#define		BSP_I2C_HARD_GLOBE		extern
#endif	


#include "stm32f2xx.h"
#include  <bsp.h>


#define	I2C_HARD_PIN_SCK_INDEX				0
#define	I2C_HARD_PIN_DATA_INDEX				1


typedef struct _str_I2C_GPIO_HARD_
{
	GPIO_TypeDef* GPIOx[2];
	GPIO_InitTypeDef GPIO_InitStruct[2];

	I2C_TypeDef* I2Cx;
	I2C_InitTypeDef I2C_InitStructure;
}str_I2C_GPIO_HARD;





