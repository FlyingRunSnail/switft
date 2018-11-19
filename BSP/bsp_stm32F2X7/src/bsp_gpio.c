


#define BSP_GPIO_DEF

#include <bsp_gpio.h>

/***********************************************************
**name:	BSP_GPIO_PortClkSwitch
**describe:  GPIO口组的时钟开关函数
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void BSP_GPIO_PortClkSwitch(GPIO_TypeDef* GPIOx,INT8U switch_set)
{
	INT32U PortClk;

	INT8U index;

	index = ((INT32U)GPIOx & 0xFF00)>>8;

	PortClk = 1<<(index /4);

	switch_set = switch_set>0?ENABLE : DISABLE;

	RCC_AHB1PeriphClockCmd(PortClk, switch_set);	
}

/***********************************************************
**name:	BSP_GPIO_Init
**describe:  GPIO口组的初始化函数,仅仅针对普通IO口
				支持多个引脚初始化
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int BSP_GPIO_Init(GPIO_TypeDef* GPIOx,INT16U PINx,INT8U PinDir,INT8U PinOtype,INT8U PinPuPd)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	INT8U i=0;


	 BSP_GPIO_PortClkSwitch(GPIOx,ENABLE);
	
	GPIO_InitStruct.GPIO_Pin = PINx;
	GPIO_InitStruct.GPIO_Mode = PinDir>0?GPIO_Mode_OUT : GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType = PinOtype;
	GPIO_InitStruct.GPIO_PuPd  = PinPuPd;

	for(i=0;i<16;i++)
	{
		if( PINx & (1<<i))
		{
			GPIO_InitStruct.GPIO_Pin = PINx & (1<<i);
			GPIO_Init(GPIOx, &GPIO_InitStruct);
		}
	}

	return SUCC;
}

/***********************************************************
**name:	BSP_GPIO_PinWriteHigh
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void BSP_GPIO_PinWriteHigh(GPIO_TypeDef* GPIOx,INT16U PINx)
{
	GPIO_SetBits(GPIOx, PINx);
}

/***********************************************************
**name:	BSP_GPIO_PinWriteLow
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void BSP_GPIO_PinWriteLow(GPIO_TypeDef* GPIOx,INT16U PINx)
{
	GPIO_ResetBits(GPIOx, PINx);
}

/***********************************************************
**name:	BSP_GPIO_InputPinRead
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
INT8U BSP_GPIO_InputPinRead(GPIO_TypeDef* GPIOx,INT16U PINx)
{
	return ( GPIO_ReadInputDataBit(GPIOx, PINx));
}

/***********************************************************
**name:	BSP_GPIO_OutputPinRead
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
INT8U BSP_GPIO_OutputPinRead(GPIO_TypeDef* GPIOx,INT16U PINx)
{
	return ( GPIO_ReadOutputDataBit(GPIOx, PINx));
}




