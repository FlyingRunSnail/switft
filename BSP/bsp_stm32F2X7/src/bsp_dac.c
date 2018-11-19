


#define BSP_DAC_DEF

#include <bsp_dac.h>






/***********************************************************
**name:	BSP_DAC_Init
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void BSP_DAC_Init(GPIO_TypeDef* GPIOx, INT16U PINx)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	DAC_InitTypeDef DAC_InitType;   

	
	BSP_GPIO_PortClkSwitch(GPIOx,ENABLE);
	GPIO_InitStructure.GPIO_Pin = PINx;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOx, &GPIO_InitStructure);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE );   //使能DAC通道时钟

	DAC_InitType.DAC_Trigger=DAC_Trigger_None; //不使用触发功能 TEN1=0  
	DAC_InitType.DAC_WaveGeneration=DAC_WaveGeneration_None;//不使用波形发生 
	DAC_InitType.DAC_LFSRUnmask_TriangleAmplitude=DAC_LFSRUnmask_Bit0; 
	DAC_InitType.DAC_OutputBuffer=DAC_OutputBuffer_Enable;  //DAC1输出缓存关闭  
	DAC_Init(DAC_Channel_1,&DAC_InitType);  //初始化DAC通道1
	DAC_Cmd(DAC_Channel_1, ENABLE);
}


/***********************************************************
**name:	BSP_DAC_Config
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int BSP_DAC_Config(INT32U mv_value)
{
	INT16U temp_data,READDATA;
	//先对数值进行校验
	
	if(mv_value > TXLINK_DACAGC_VALUE_MAX)
		temp_data = TXLINK_DACAGC_VALUE_MAX * TXLINK_DACAGC_PARA / TXLINK_DACAGC_VALUE_REF ;
	else if(mv_value <= TXLINK_DACAGC_VALUE_MIN)
		temp_data = TXLINK_DACAGC_VALUE_MIN  * TXLINK_DACAGC_PARA / TXLINK_DACAGC_VALUE_REF;
	else
		temp_data = mv_value * TXLINK_DACAGC_PARA / TXLINK_DACAGC_VALUE_REF;
	
	DAC_SetChannel1Data(DAC_Align_12b_R, (INT16U)temp_data); 
	READDATA = DAC_GetDataOutputValue(DAC_Channel_1);
	if( READDATA != temp_data)
		return FAIL;
	return SUCC;
}



