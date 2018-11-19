

#define		BSP_I2C_SOFT_DEF

#include <bsp_i2c_soft.h>






/***********************************************************
**name:	BSP_I2C_SOFT_Init
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void BSP_I2C_SOFT_Init(str_I2C_GPIO_SOFT *I2C_GPIO_SOFT)
{
	INT8U i;

	for(i=0;i<2;i++)
	{
		BSP_GPIO_PortClkSwitch(I2C_GPIO_SOFT->GPIOx[i],ENABLE);
		GPIO_Init(I2C_GPIO_SOFT->GPIOx[i],&I2C_GPIO_SOFT->GPIO_InitStruct[i]);
	}
}

/***********************************************************
**name:	BSP_I2C_DATA_SetInput
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void BSP_I2C_DATA_SetInput(str_I2C_GPIO_SOFT *I2C_GPIO_SOFT)
{
	I2C_GPIO_SOFT->GPIO_InitStruct[I2C_SOFT_PIN_DATA_INDEX].GPIO_Mode = GPIO_Mode_IN;
	GPIO_Init(I2C_GPIO_SOFT->GPIOx[I2C_SOFT_PIN_DATA_INDEX],&I2C_GPIO_SOFT->GPIO_InitStruct[I2C_SOFT_PIN_DATA_INDEX]);
}

/***********************************************************
**name:	BSP_I2C_DATA_SetOutput
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void BSP_I2C_DATA_SetOutput(str_I2C_GPIO_SOFT *I2C_GPIO_SOFT)
{
	I2C_GPIO_SOFT->GPIO_InitStruct[I2C_SOFT_PIN_DATA_INDEX].GPIO_Mode = GPIO_Mode_OUT;
	GPIO_Init(I2C_GPIO_SOFT->GPIOx[I2C_SOFT_PIN_DATA_INDEX],&I2C_GPIO_SOFT->GPIO_InitStruct[I2C_SOFT_PIN_DATA_INDEX]);
}

/***********************************************************
**name:	BSP_I2C_SCK_WriteHigh
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void BSP_I2C_SCK_WriteHigh(str_I2C_GPIO_SOFT *I2C_GPIO_SOFT)
{
	GPIO_WriteBit(I2C_GPIO_SOFT->GPIOx[I2C_SOFT_PIN_SCK_INDEX], I2C_GPIO_SOFT->GPIO_InitStruct[I2C_SOFT_PIN_SCK_INDEX].GPIO_Pin, Bit_SET);	
}

/***********************************************************
**name:	BSP_I2C_SCK_WriteLow
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void BSP_I2C_SCK_WriteLow(str_I2C_GPIO_SOFT *I2C_GPIO_SOFT)
{
	GPIO_WriteBit(I2C_GPIO_SOFT->GPIOx[I2C_SOFT_PIN_SCK_INDEX], I2C_GPIO_SOFT->GPIO_InitStruct[I2C_SOFT_PIN_SCK_INDEX].GPIO_Pin, Bit_RESET);	
}

/***********************************************************
**name:	BSP_I2C_DATA_WriteHigh
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void BSP_I2C_DATA_WriteHigh(str_I2C_GPIO_SOFT *I2C_GPIO_SOFT)
{
	GPIO_WriteBit(I2C_GPIO_SOFT->GPIOx[I2C_SOFT_PIN_DATA_INDEX], I2C_GPIO_SOFT->GPIO_InitStruct[I2C_SOFT_PIN_DATA_INDEX].GPIO_Pin, Bit_SET);	
}

/***********************************************************
**name:	BSP_I2C_SCK_High
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void BSP_I2C_DATA_WriteLow(str_I2C_GPIO_SOFT *I2C_GPIO_SOFT)
{
	GPIO_WriteBit(I2C_GPIO_SOFT->GPIOx[I2C_SOFT_PIN_DATA_INDEX], I2C_GPIO_SOFT->GPIO_InitStruct[I2C_SOFT_PIN_DATA_INDEX].GPIO_Pin, Bit_RESET);	
}


/***********************************************************
**name:	BSP_I2C_DATA_ReadBit
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
INT8U BSP_I2C_DATA_ReadBit(str_I2C_GPIO_SOFT *I2C_GPIO_SOFT)
{
	return (GPIO_ReadInputDataBit(I2C_GPIO_SOFT->GPIOx[I2C_SOFT_PIN_DATA_INDEX], I2C_GPIO_SOFT->GPIO_InitStruct[I2C_SOFT_PIN_DATA_INDEX].GPIO_Pin));	
}


/***********************************************************
**name:	BSP_I2C_WriteByte
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void BSP_I2C_WriteByte(str_I2C_GPIO_SOFT *I2C_GPIO_SOFT,INT8U data)
{
	INT8U i;
	
	for(i=0;i<8;i++)
	{
		BSP_I2C_SCK_WriteLow(I2C_GPIO_SOFT);
		if( data & 0x80)
			BSP_I2C_DATA_WriteHigh(I2C_GPIO_SOFT);
		else
			BSP_I2C_DATA_WriteLow(I2C_GPIO_SOFT);
		BSP_Delay_us(BSP_I2C_SOFT_DELAY_US);

		BSP_I2C_SCK_WriteHigh(I2C_GPIO_SOFT);
		BSP_Delay_us(BSP_I2C_SOFT_DELAY_US-1);

		data = data<<1;
	}
}

/***********************************************************
**name:	BSP_I2C_ReadByte
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
INT8U BSP_I2C_ReadByte(str_I2C_GPIO_SOFT *I2C_GPIO_SOFT)
{
	INT8U i;
	INT8U data=0;

	BSP_I2C_DATA_SetInput(I2C_GPIO_SOFT);
	for(i=0;i<8;i++)
	{
		BSP_I2C_SCK_WriteLow(I2C_GPIO_SOFT);
		BSP_Delay_us(10);
		
		BSP_I2C_SCK_WriteHigh(I2C_GPIO_SOFT);
		BSP_Delay_us(5);
		data = (data << 1) + BSP_I2C_DATA_ReadBit(I2C_GPIO_SOFT);

	}

	return data;
}

/***********************************************************
**name:	BSP_I2C_SendStart
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void BSP_I2C_SendStart(str_I2C_GPIO_SOFT *I2C_GPIO_SOFT)
{

	// 确保发送起始信号前，SCK和DATA均为高
	BSP_I2C_DATA_WriteHigh(I2C_GPIO_SOFT);
	BSP_I2C_SCK_WriteHigh(I2C_GPIO_SOFT);
	BSP_Delay_us(BSP_I2C_SOFT_DELAY_US);

	// DATA先拉低表示起始
	BSP_I2C_DATA_WriteLow(I2C_GPIO_SOFT);
	BSP_Delay_us(BSP_I2C_SOFT_DELAY_US);
	
	BSP_I2C_SCK_WriteLow(I2C_GPIO_SOFT);
	BSP_Delay_us(BSP_I2C_SOFT_DELAY_US/2);
}

/***********************************************************
**name:	BSP_I2C_SendStop
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void BSP_I2C_SendStop(str_I2C_GPIO_SOFT *I2C_GPIO_SOFT)
{
	BSP_I2C_SCK_WriteLow(I2C_GPIO_SOFT);
	// 确保发送停止信号前，DAT为低
	BSP_I2C_DATA_WriteLow(I2C_GPIO_SOFT);
	BSP_Delay_us(BSP_I2C_SOFT_DELAY_US);

	// SCK先拉高，数据后拉高表示停止
	BSP_I2C_SCK_WriteHigh(I2C_GPIO_SOFT);
	BSP_Delay_us(BSP_I2C_SOFT_DELAY_US/2);

	
	BSP_I2C_DATA_WriteHigh(I2C_GPIO_SOFT);
	BSP_Delay_us(50);

	//BSP_Delay_us(50);
}


/***********************************************************
**name:	BSP_I2C_SendACK
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void BSP_I2C_SendACK(str_I2C_GPIO_SOFT *I2C_GPIO_SOFT)
{
	BSP_I2C_SCK_WriteLow(I2C_GPIO_SOFT);
	BSP_I2C_DATA_WriteLow(I2C_GPIO_SOFT);
	BSP_Delay_us(BSP_I2C_SOFT_DELAY_US);

	BSP_I2C_SCK_WriteHigh(I2C_GPIO_SOFT);
	BSP_Delay_us(BSP_I2C_SOFT_DELAY_US);
	BSP_I2C_SCK_WriteLow(I2C_GPIO_SOFT);
}

/***********************************************************
**name:	BSP_I2C_SendNoACK
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void BSP_I2C_SendNoACK(str_I2C_GPIO_SOFT *I2C_GPIO_SOFT)
{
	BSP_I2C_SCK_WriteLow(I2C_GPIO_SOFT);
	BSP_I2C_DATA_WriteHigh(I2C_GPIO_SOFT);
	BSP_Delay_us(BSP_I2C_SOFT_DELAY_US);

	BSP_I2C_SCK_WriteHigh(I2C_GPIO_SOFT);
	BSP_Delay_us(BSP_I2C_SOFT_DELAY_US);
	BSP_I2C_SCK_WriteLow(I2C_GPIO_SOFT);
}


/***********************************************************
**name:	BSP_I2C_Check_ACK_NoACK
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
INT8U BSP_I2C_Check_ACK_NoACK(str_I2C_GPIO_SOFT *I2C_GPIO_SOFT)
{
	INT8U data=0;

	BSP_I2C_DATA_SetInput(I2C_GPIO_SOFT);
	
	BSP_I2C_SCK_WriteLow(I2C_GPIO_SOFT);
	BSP_Delay_us(BSP_I2C_SOFT_DELAY_US);

	BSP_I2C_SCK_WriteHigh(I2C_GPIO_SOFT);
	BSP_Delay_us(BSP_I2C_SOFT_DELAY_US-1);
	data = BSP_I2C_DATA_ReadBit(I2C_GPIO_SOFT);

	BSP_I2C_SCK_WriteLow(I2C_GPIO_SOFT);

	BSP_I2C_DATA_WriteHigh(I2C_GPIO_SOFT);
	BSP_I2C_DATA_SetOutput(I2C_GPIO_SOFT);

	return data;
}

/***********************************************************
**name:	BSP_I2C_Reset
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void BSP_I2C_Reset(str_I2C_GPIO_SOFT *I2C_GPIO_SOFT)
{
	INT8U i;
	
	BSP_I2C_DATA_SetInput(I2C_GPIO_SOFT);

	for(i=0;i<200;i++)
	{
		BSP_I2C_SCK_WriteLow(I2C_GPIO_SOFT);
		BSP_Delay_us(BSP_I2C_SOFT_DELAY_US);

		BSP_I2C_SCK_WriteHigh(I2C_GPIO_SOFT);
		
		BSP_I2C_DATA_SetInput(I2C_GPIO_SOFT);
		if(BSP_I2C_DATA_ReadBit(I2C_GPIO_SOFT) == Bit_SET)
			break;
	}

	BSP_I2C_DATA_SetOutput(I2C_GPIO_SOFT);
}

