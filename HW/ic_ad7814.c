

#define IC_AD7814_DEF

#include "ic_ad7814.h"

str_SPI_GPIO_HARD AD7814_Hard;


OS_SEM AD7814_Sem;

/***********************************************************
**name:	AD7814_SPI_CS_Enable
**describe: 
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void AD7814_SPI_CS_Enable(void)
{
	BSP_OS_Sem_Pend(&AD7814_Sem,100);
	GPIO_WriteBit(AD7814_CS_PIN_PORT,AD7814_CS_PIN, Bit_RESET);	
}

/***********************************************************
**name:	AD7814_SPI_CS_Disable
**describe: 
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void AD7814_SPI_CS_Disable(void)
{
	GPIO_WriteBit(AD7814_CS_PIN_PORT, AD7814_CS_PIN, Bit_SET);	
	BSP_OS_Sem_Post(&AD7814_Sem);
}



/***********************************************************
**name:	AD7814_SPI_WriteReg8bit
**describe: 
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
INT16U  AD7814_SPI_Write(INT16U cmd)
{
	INT8U i=0;
	INT8U tempbit;
	INT8U readbit=0;
	INT16U read_data = 0,write_data;

	write_data = cmd;
	AD7814_SPI_CS_Enable();

	for( i = 0;i<2;i++)
	{

		read_data = (read_data << 8) +BSP_SPI_HARD_WriteByte(AD7814_Hard.SPIx, (write_data & 0xFF00)>>8);
		write_data = write_data << 8;
	}
	AD7814_SPI_CS_Disable();

	return read_data;
}

/***********************************************************
**name:	AD7814_SPI_ReadReg
**describe: 
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
INT16U AD7814_SPI_Read(void)
{
	return (AD7814_SPI_Write(0x0000));
}


/***********************************************************
**name:	AD7814_WriteReg
**describe: 
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
INT32S AD7814_TempGet(void)
{
	INT16U data;
	INT8S temp;
	INT8U positive_flag = 0;

	data = AD7814_SPI_Read();

	if( data & 0x4000 )
		positive_flag = 0;		// 负数
	else	
		positive_flag = 1;		// 正数

	data &= ~0x4000;
	data = data >> 5;

	temp = data * 25 / 100;
	if( positive_flag == 0 )
		temp = 0-temp;

	return temp;
}


/***********************************************************
**name:	AD7814_Init
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void AD7814_Init(void)
{
	AD7814_Hard.GPIOx[SPI_GPIO_SCK_INDEX] = AD7814_SCK_PIN_PORT;
	AD7814_Hard.GPIO_InitStruct[SPI_GPIO_SCK_INDEX].GPIO_Pin = AD7814_SCK_PIN;
	AD7814_Hard.GPIO_InitStruct[SPI_GPIO_SCK_INDEX].GPIO_Mode = GPIO_Mode_AF;
	AD7814_Hard.GPIO_InitStruct[SPI_GPIO_SCK_INDEX].GPIO_Speed = GPIO_Speed_50MHz;
	AD7814_Hard.GPIO_InitStruct[SPI_GPIO_SCK_INDEX].GPIO_OType = GPIO_OType_PP;
	AD7814_Hard.GPIO_InitStruct[SPI_GPIO_SCK_INDEX].GPIO_PuPd  = GPIO_PuPd_NOPULL;

	AD7814_Hard.GPIOx[SPI_GPIO_MOSI_INDEX] = AD7814_MOSI_PIN_PORT;
	AD7814_Hard.GPIO_InitStruct[SPI_GPIO_MOSI_INDEX].GPIO_Pin = AD7814_MOSI_PIN;
	AD7814_Hard.GPIO_InitStruct[SPI_GPIO_MOSI_INDEX].GPIO_Mode = GPIO_Mode_AF;
	AD7814_Hard.GPIO_InitStruct[SPI_GPIO_MOSI_INDEX].GPIO_Speed = GPIO_Speed_50MHz;
	AD7814_Hard.GPIO_InitStruct[SPI_GPIO_MOSI_INDEX].GPIO_OType = GPIO_OType_PP;
	AD7814_Hard.GPIO_InitStruct[SPI_GPIO_MOSI_INDEX].GPIO_PuPd  = GPIO_PuPd_NOPULL;

	AD7814_Hard.GPIOx[SPI_GPIO_MISO_INDEX] = AD7814_MISO_PIN_PORT;
	AD7814_Hard.GPIO_InitStruct[SPI_GPIO_MISO_INDEX].GPIO_Pin = AD7814_MISO_PIN;
	AD7814_Hard.GPIO_InitStruct[SPI_GPIO_MISO_INDEX].GPIO_Mode = GPIO_Mode_AF;
	AD7814_Hard.GPIO_InitStruct[SPI_GPIO_MISO_INDEX].GPIO_Speed = GPIO_Speed_50MHz;
	AD7814_Hard.GPIO_InitStruct[SPI_GPIO_MISO_INDEX].GPIO_OType = GPIO_OType_PP;
	AD7814_Hard.GPIO_InitStruct[SPI_GPIO_MISO_INDEX].GPIO_PuPd  = GPIO_PuPd_NOPULL;

	AD7814_Hard.SPIx = AD7814_SPI_PORT;
	AD7814_Hard.SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	AD7814_Hard.SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	AD7814_Hard.SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	AD7814_Hard.SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
	AD7814_Hard.SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	AD7814_Hard.SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	AD7814_Hard.SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	AD7814_Hard.SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	AD7814_Hard.SPI_InitStruct.SPI_CRCPolynomial = 0x07;
	BSP_SPI_HARD_Init(AD7814_Hard);
	
	AD7814_Hard.GPIOx[SPI_GPIO_CS_INDEX] = AD7814_CS_PIN_PORT;
	AD7814_Hard.GPIO_InitStruct[SPI_GPIO_CS_INDEX].GPIO_Pin = AD7814_CS_PIN;
	AD7814_Hard.GPIO_InitStruct[SPI_GPIO_CS_INDEX].GPIO_Mode = GPIO_Mode_OUT;
	AD7814_Hard.GPIO_InitStruct[SPI_GPIO_CS_INDEX].GPIO_Speed = GPIO_Speed_50MHz;
	AD7814_Hard.GPIO_InitStruct[SPI_GPIO_CS_INDEX].GPIO_OType = GPIO_OType_PP;
	AD7814_Hard.GPIO_InitStruct[SPI_GPIO_CS_INDEX].GPIO_PuPd  = GPIO_PuPd_NOPULL;	
	BSP_GPIO_PortClkSwitch(AD7814_Hard.GPIOx[SPI_GPIO_CS_INDEX],ENABLE);
	GPIO_Init(AD7814_Hard.GPIOx[SPI_GPIO_CS_INDEX],&AD7814_Hard.GPIO_InitStruct[SPI_GPIO_CS_INDEX]);

	BSP_OS_Sem_Creat(&AD7814_Sem,"SPIFLASH Sem",1);
	
	AD7814_SPI_CS_Disable();
}


