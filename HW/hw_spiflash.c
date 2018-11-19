


#define HW_SPIFLASH_DEF

#include <hw_spiflash.h>


str_SPI_GPIO_HARD SPI_Flash_Hard;


OS_SEM SPIFLASH_Sem;

/***********************************************************
**name:	HW_SPIFLASH_Init
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void HW_SPIFLASH_Init(void)
{
	SPI_Flash_Hard.GPIOx[SPI_GPIO_SCK_INDEX] = HW_SPIFLASH_SCK_PIN_PORT;
	SPI_Flash_Hard.GPIO_InitStruct[SPI_GPIO_SCK_INDEX].GPIO_Pin = HW_SPIFLASH_SCK_PIN;
	SPI_Flash_Hard.GPIO_InitStruct[SPI_GPIO_SCK_INDEX].GPIO_Mode = GPIO_Mode_AF;
	SPI_Flash_Hard.GPIO_InitStruct[SPI_GPIO_SCK_INDEX].GPIO_Speed = GPIO_Speed_50MHz;
	SPI_Flash_Hard.GPIO_InitStruct[SPI_GPIO_SCK_INDEX].GPIO_OType = GPIO_OType_PP;
	SPI_Flash_Hard.GPIO_InitStruct[SPI_GPIO_SCK_INDEX].GPIO_PuPd  = GPIO_PuPd_NOPULL;

	SPI_Flash_Hard.GPIOx[SPI_GPIO_MOSI_INDEX] = HW_SPIFLASH_MOSI_PIN_PORT;
	SPI_Flash_Hard.GPIO_InitStruct[SPI_GPIO_MOSI_INDEX].GPIO_Pin = HW_SPIFLASH_MOSI_PIN;
	SPI_Flash_Hard.GPIO_InitStruct[SPI_GPIO_MOSI_INDEX].GPIO_Mode = GPIO_Mode_AF;
	SPI_Flash_Hard.GPIO_InitStruct[SPI_GPIO_MOSI_INDEX].GPIO_Speed = GPIO_Speed_50MHz;
	SPI_Flash_Hard.GPIO_InitStruct[SPI_GPIO_MOSI_INDEX].GPIO_OType = GPIO_OType_PP;
	SPI_Flash_Hard.GPIO_InitStruct[SPI_GPIO_MOSI_INDEX].GPIO_PuPd  = GPIO_PuPd_NOPULL;

	SPI_Flash_Hard.GPIOx[SPI_GPIO_MISO_INDEX] = HW_SPIFLASH_MISO_PIN_PORT;
	SPI_Flash_Hard.GPIO_InitStruct[SPI_GPIO_MISO_INDEX].GPIO_Pin = HW_SPIFLASH_MISO_PIN;
	SPI_Flash_Hard.GPIO_InitStruct[SPI_GPIO_MISO_INDEX].GPIO_Mode = GPIO_Mode_AF;
	SPI_Flash_Hard.GPIO_InitStruct[SPI_GPIO_MISO_INDEX].GPIO_Speed = GPIO_Speed_50MHz;
	SPI_Flash_Hard.GPIO_InitStruct[SPI_GPIO_MISO_INDEX].GPIO_OType = GPIO_OType_PP;
	SPI_Flash_Hard.GPIO_InitStruct[SPI_GPIO_MISO_INDEX].GPIO_PuPd  = GPIO_PuPd_NOPULL;

	SPI_Flash_Hard.SPIx = HW_SPIFLASH_SPI_PORT;
	SPI_Flash_Hard.SPI_InitStruct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_Flash_Hard.SPI_InitStruct.SPI_Mode = SPI_Mode_Master;
	SPI_Flash_Hard.SPI_InitStruct.SPI_DataSize = SPI_DataSize_8b;
	SPI_Flash_Hard.SPI_InitStruct.SPI_CPOL = SPI_CPOL_Low;
	SPI_Flash_Hard.SPI_InitStruct.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_Flash_Hard.SPI_InitStruct.SPI_NSS = SPI_NSS_Soft;
	SPI_Flash_Hard.SPI_InitStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_Flash_Hard.SPI_InitStruct.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_Flash_Hard.SPI_InitStruct.SPI_CRCPolynomial = 0x07;
	BSP_SPI_HARD_Init(SPI_Flash_Hard);
	
	SPI_Flash_Hard.GPIOx[SPI_GPIO_CS_INDEX] = HW_SPIFLASH_CS_PIN_PORT;
	SPI_Flash_Hard.GPIO_InitStruct[SPI_GPIO_CS_INDEX].GPIO_Pin = HW_SPIFLASH_CS_PIN;
	SPI_Flash_Hard.GPIO_InitStruct[SPI_GPIO_CS_INDEX].GPIO_Mode = GPIO_Mode_OUT;
	SPI_Flash_Hard.GPIO_InitStruct[SPI_GPIO_CS_INDEX].GPIO_Speed = GPIO_Speed_50MHz;
	SPI_Flash_Hard.GPIO_InitStruct[SPI_GPIO_CS_INDEX].GPIO_OType = GPIO_OType_PP;
	SPI_Flash_Hard.GPIO_InitStruct[SPI_GPIO_CS_INDEX].GPIO_PuPd  = GPIO_PuPd_NOPULL;	
	BSP_GPIO_PortClkSwitch(SPI_Flash_Hard.GPIOx[SPI_GPIO_CS_INDEX],ENABLE);
	GPIO_Init(SPI_Flash_Hard.GPIOx[SPI_GPIO_CS_INDEX],&SPI_Flash_Hard.GPIO_InitStruct[SPI_GPIO_CS_INDEX]);

	BSP_OS_Sem_Creat(&SPIFLASH_Sem,"SPIFLASH Sem",1);
	
	HW_SPIFLASH_CsDisable();
}

/***********************************************************
**name:	HW_SPIFLASH_CsEnable
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void HW_SPIFLASH_CsEnable(void)
{
	BSP_OS_Sem_Pend(&SPIFLASH_Sem,100);
	GPIO_WriteBit(HW_SPIFLASH_CS_PIN_PORT, HW_SPIFLASH_CS_PIN, Bit_RESET);	
}

/***********************************************************
**name:	HW_SPIFLASH_CsDisable
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void HW_SPIFLASH_CsDisable(void)
{
	GPIO_WriteBit(HW_SPIFLASH_CS_PIN_PORT, HW_SPIFLASH_CS_PIN, Bit_SET);	
	BSP_OS_Sem_Post(&SPIFLASH_Sem);
}


/***********************************************************
**name:	HW_SPIFLASH_WriteByte
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
INT8U HW_SPIFLASH_WriteByte(INT8U data)
{
	return (BSP_SPI_HARD_WriteByte(SPI_Flash_Hard.SPIx,data));
}

/***********************************************************
**name:	HW_SPIFLASH_WriteData
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
INT8U HW_SPIFLASH_ReadByte(void)
{
	return BSP_SPI_HARD_ReadByte(SPI_Flash_Hard.SPIx);
}



