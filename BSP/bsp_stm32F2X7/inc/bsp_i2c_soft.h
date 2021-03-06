

#ifdef		BSP_I2C_SOFT_DEF
	#define		BSP_I2C_GLOBE
#else
	#define		BSP_I2C_GLOBE		extern
#endif	


#include "stm32f2xx.h"
#include  <bsp.h>

#define BSP_I2C_SOFT_DELAY_US		15	


#define	I2C_SOFT_PIN_SCK_INDEX				0
#define	I2C_SOFT_PIN_DATA_INDEX			1


#define	I2C_ACK		0
#define	I2C_NOACK	1

typedef struct _str_I2C_GPIO_SOFT_
{
	GPIO_TypeDef* GPIOx[2];
	GPIO_InitTypeDef GPIO_InitStruct[2];
}str_I2C_GPIO_SOFT;





BSP_I2C_GLOBE void BSP_I2C_SOFT_Init(str_I2C_GPIO_SOFT *I2C_GPIO_SOFT);

BSP_I2C_GLOBE void BSP_I2C_DATA_SetInput(str_I2C_GPIO_SOFT *I2C_GPIO_SOFT);

BSP_I2C_GLOBE void BSP_I2C_DATA_SetOutput(str_I2C_GPIO_SOFT *I2C_GPIO_SOFT);

BSP_I2C_GLOBE void BSP_I2C_SCK_WriteHigh(str_I2C_GPIO_SOFT *I2C_GPIO_SOFT);

BSP_I2C_GLOBE void BSP_I2C_SCK_WriteLow(str_I2C_GPIO_SOFT *I2C_GPIO_SOFT);

BSP_I2C_GLOBE void BSP_I2C_DATA_WriteHigh(str_I2C_GPIO_SOFT *I2C_GPIO_SOFT);

BSP_I2C_GLOBE void BSP_I2C_DATA_WriteLow(str_I2C_GPIO_SOFT *I2C_GPIO_SOFT);

BSP_I2C_GLOBE INT8U BSP_I2C_DATA_ReadBit(str_I2C_GPIO_SOFT *I2C_GPIO_SOFT);

BSP_I2C_GLOBE void BSP_I2C_WriteByte(str_I2C_GPIO_SOFT *I2C_GPIO_SOFT,INT8U data);

BSP_I2C_GLOBE INT8U BSP_I2C_ReadByte(str_I2C_GPIO_SOFT *I2C_GPIO_SOFT);

BSP_I2C_GLOBE void BSP_I2C_SendStart(str_I2C_GPIO_SOFT *I2C_GPIO_SOFT);

BSP_I2C_GLOBE void BSP_I2C_SendStop(str_I2C_GPIO_SOFT *I2C_GPIO_SOFT);

BSP_I2C_GLOBE void BSP_I2C_SendACK(str_I2C_GPIO_SOFT *I2C_GPIO_SOFT);

BSP_I2C_GLOBE void BSP_I2C_SendNoACK(str_I2C_GPIO_SOFT *I2C_GPIO_SOFT);

BSP_I2C_GLOBE INT8U BSP_I2C_Check_ACK_NoACK(str_I2C_GPIO_SOFT *I2C_GPIO_SOFT);

BSP_I2C_GLOBE void BSP_I2C_Reset(str_I2C_GPIO_SOFT *I2C_GPIO_SOFT);

