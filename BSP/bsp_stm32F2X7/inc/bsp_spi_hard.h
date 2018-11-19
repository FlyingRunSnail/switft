
#ifdef BSP_SPI_HARD_DEF
	#define BSP_SPI_HARD_GLOBE
#else
	#define BSP_SPI_HARD_GLOBE		extern
#endif

#include "stm32f2xx.h"
#include  <bsp.h>



typedef struct _str_SPI_GPIO_HARD_
{
	GPIO_TypeDef* GPIOx[4];
	GPIO_InitTypeDef GPIO_InitStruct[4];
	SPI_TypeDef	* SPIx;
	SPI_InitTypeDef SPI_InitStruct;
}str_SPI_GPIO_HARD;


#define SPI_GPIO_SCK_INDEX		0
#define SPI_GPIO_MOSI_INDEX		1
#define SPI_GPIO_MISO_INDEX		2
#define SPI_GPIO_CS_INDEX			3


BSP_SPI_HARD_GLOBE void BSP_SPI_HARD_PortClkSwitch(SPI_TypeDef* SPIx,INT8U switch_set);

BSP_SPI_HARD_GLOBE void BSP_SPI_HARD_Init(str_SPI_GPIO_HARD SPI_GPIO_HARD);

BSP_SPI_HARD_GLOBE void BSP_SPI_HARD_CS_Enable(GPIO_TypeDef* GPIOx, INT16U GPIO_Pin);

BSP_SPI_HARD_GLOBE void BSP_SPI_HARD_CS_Disable(GPIO_TypeDef* GPIOx, INT16U GPIO_Pin);

BSP_SPI_HARD_GLOBE INT8U BSP_SPI_HARD_WriteByte(SPI_TypeDef* SPIx,INT8U data);

BSP_SPI_HARD_GLOBE INT8U BSP_SPI_HARD_ReadByte(SPI_TypeDef* SPIx);


