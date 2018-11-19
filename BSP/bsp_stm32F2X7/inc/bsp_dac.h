

#ifdef BSP_DAC_DEF
	#define BSP_DAC_GLOBE
#else
	#define BSP_DAC_GLOBE		extern
#endif

#include "stm32f2xx.h"
#include  <bsp.h>


#define TXLINK_DACAGC_VALUE_REF			((INT32U)3300)
#define TXLINK_DACAGC_VALUE_MAX			((INT32U)3300)
#define TXLINK_DACAGC_VALUE_MIN			((INT32U)0)

#define TXLINK_DACAGC_PARA		4095




BSP_DAC_GLOBE void BSP_DAC_Init(GPIO_TypeDef* GPIOx, INT16U PINx);


BSP_DAC_GLOBE int BSP_DAC_Config(INT32U mv_value);

