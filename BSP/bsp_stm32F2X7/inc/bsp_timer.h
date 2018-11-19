
#ifdef BSP_TIMER_DEF
	#define BSP_TIMER_GLOBE
#else
	#define BSP_TIMER_GLOBE		extern
#endif

#include "stm32f2xx.h"
#include  <bsp.h>


BSP_TIMER_GLOBE void BSP_TIMER_PortClkSwitch(TIM_TypeDef* TIMx,INT8U switch_set);


BSP_TIMER_GLOBE void BSP_TIMER_Init(TIM_TypeDef* TIMx,INT32U us);


BSP_TIMER_GLOBE void BSP_TIMER_Switch(TIM_TypeDef* TIMx,INT8U switch_set);

