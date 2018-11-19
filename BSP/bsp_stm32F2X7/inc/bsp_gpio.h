

#ifdef BSP_GPIO_DEF
	#define BSP_GPIO_GLOBE
#else
	#define BSP_GPIO_GLOBE		extern
#endif

#include "stm32f2xx.h"
#include "stm32f2xx_gpio.h"
#include  <bsp.h>


BSP_GPIO_GLOBE void BSP_GPIO_PortClkSwitch(GPIO_TypeDef* GPIOx,INT8U switch_set);

BSP_GPIO_GLOBE int BSP_GPIO_Init(GPIO_TypeDef* GPIOx,INT16U PINx,INT8U PinDir,INT8U PinOtype,INT8U PinPuPd);

BSP_GPIO_GLOBE void BSP_GPIO_PinWriteHigh(GPIO_TypeDef* GPIOx,INT16U PINx);

BSP_GPIO_GLOBE void BSP_GPIO_PinWriteLow(GPIO_TypeDef* GPIOx,INT16U PINx);

BSP_GPIO_GLOBE INT8U BSP_GPIO_InputPinRead(GPIO_TypeDef* GPIOx,INT16U PINx);

BSP_GPIO_GLOBE INT8U BSP_GPIO_OutputPinRead(GPIO_TypeDef* GPIOx,INT16U PINx);






