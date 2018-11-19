

#define BSP_TIMER_DEF

#include <bsp_timer.h>




/***********************************************************
**name:	BSP_TIMER_PortClkSwitch
**describe:  定时器的时钟开关函数
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void BSP_TIMER_PortClkSwitch(TIM_TypeDef* TIMx,INT8U switch_set)
{
	switch( (INT32U)TIMx )
	{
		case (INT32U)TIM1:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, switch_set);	
			break;

		case (INT32U)TIM2:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, switch_set);	
			break;

		case (INT32U)TIM3:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, switch_set);	
			break;

		case (INT32U)TIM4:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, switch_set);	
			break;

		case (INT32U)TIM5:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, switch_set);	
			break;

		case (INT32U)TIM6:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, switch_set);	
			break;
		case (INT32U)TIM7:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, switch_set);	
			break;
		case (INT32U)TIM8:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, switch_set);	
			break;
		case (INT32U)TIM9:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9, switch_set);	
			break;
		case (INT32U)TIM10:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, switch_set);	
			break;
		case (INT32U)TIM11:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, switch_set);	
			break;
		case (INT32U)TIM12:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, switch_set);	
			break;
		case (INT32U)TIM13:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, switch_set);	
			break;
		case (INT32U)TIM14:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, switch_set);	
			break;
	}

}



/***********************************************************
**name:	BSP_TIMER_Init
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void BSP_TIMER_Init(TIM_TypeDef* TIMx,INT32U us)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	BSP_TIMER_PortClkSwitch(TIMx,ENABLE);

	TIM_TimeBaseStructure.TIM_Period = (us - 1);
  	TIM_TimeBaseStructure.TIM_Prescaler = 60-1;				//  一个CNT记数表示1微妙,
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);
	TIM_ClearITPendingBit(TIMx, TIM_IT_Update);
	TIM_ITConfig(TIMx, TIM_IT_Update, ENABLE);
}

/***********************************************************
**name:	BSP_TIMER_Switch
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void BSP_TIMER_Switch(TIM_TypeDef* TIMx,INT8U switch_set)
{
	TIM_Cmd(TIMx, switch_set);
}




