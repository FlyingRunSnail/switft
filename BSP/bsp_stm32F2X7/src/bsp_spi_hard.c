

#define BSP_SPI_HARD_DEF

#include <bsp_spi_hard.h>


/***********************************************************
**name:	BSP_SPI_HARD_PortClkSwitch
**describe:  SPI口组的时钟开关函数
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void BSP_SPI_HARD_PortClkSwitch(SPI_TypeDef* SPIx,INT8U switch_set)
{
	switch( (INT32U)SPIx )
	{
		case (INT32U)SPI1:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, switch_set);	
			break;

		case (INT32U)SPI2:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, switch_set);	
			break;

		case (INT32U)SPI3:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, switch_set);	
			break;
	}

}




/***********************************************************
**name:	BSP_SPI_HARD_Init
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void BSP_SPI_HARD_Init(str_SPI_GPIO_HARD SPI_GPIO_HARD)
{
	INT8U i,ii;
	INT16U	GPIO_PinSource;
	INT8U	GPIO_AF;

	for(i=0;i<3;i++)
	{
		BSP_GPIO_PortClkSwitch(SPI_GPIO_HARD.GPIOx[i],ENABLE);
		GPIO_Init(SPI_GPIO_HARD.GPIOx[i],&SPI_GPIO_HARD.GPIO_InitStruct[i]);

		for(ii=0;ii<16;ii++)
		{
			if(SPI_GPIO_HARD.GPIO_InitStruct[i].GPIO_Pin & (1<<ii))
			{
				GPIO_PinSource = ii;
				break;
			}
		}
		if(SPI_GPIO_HARD.SPIx == SPI1)
			GPIO_AF = GPIO_AF_SPI1;
		else if(SPI_GPIO_HARD.SPIx == SPI2)
			GPIO_AF = GPIO_AF_SPI2;
		else if(SPI_GPIO_HARD.SPIx == SPI3)
			GPIO_AF = GPIO_AF_SPI3;
		else
			continue;

		GPIO_PinAFConfig(SPI_GPIO_HARD.GPIOx[i], GPIO_PinSource, GPIO_AF);
	}
	if(SPI_GPIO_HARD.SPIx == SPI1)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,  ENABLE);	
	else if(SPI_GPIO_HARD.SPIx == SPI2)
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,  ENABLE);	
	else if(SPI_GPIO_HARD.SPIx == SPI3)
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3,  ENABLE);	

	SPI_Init(SPI_GPIO_HARD.SPIx, &SPI_GPIO_HARD.SPI_InitStruct);
	SPI_Cmd(SPI_GPIO_HARD.SPIx, ENABLE);
	
}


/***********************************************************
**name:	BSP_SPI_HARD_CS_Enable
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void BSP_SPI_HARD_CS_Enable(GPIO_TypeDef* GPIOx, INT16U GPIO_Pin)
{
	GPIO_WriteBit(GPIOx, GPIO_Pin, Bit_RESET);		
}

/***********************************************************
**name:	BSP_SPI_HARD_CS_Disable
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void BSP_SPI_HARD_CS_Disable(GPIO_TypeDef* GPIOx, INT16U GPIO_Pin)
{
	GPIO_WriteBit(GPIOx, GPIO_Pin, Bit_SET);		
}


/***********************************************************
**name:	BSP_SPI_HARD_WriteByte
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
INT8U BSP_SPI_HARD_WriteByte(SPI_TypeDef* SPIx,INT8U data)
{
	while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);

	SPI_I2S_SendData(SPIx, data);
	
	while(SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET);

	return SPI_I2S_ReceiveData(SPIx);
	
}

/***********************************************************
**name:	BSP_SPI_HARD_ReadByte
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
INT8U BSP_SPI_HARD_ReadByte(SPI_TypeDef* SPIx)
{
	return BSP_SPI_HARD_WriteByte(SPIx,0xFF);
}



