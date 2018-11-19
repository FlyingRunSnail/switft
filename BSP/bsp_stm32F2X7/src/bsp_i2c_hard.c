

#define		BSP_I2C_HARD_DEF

#include <bsp_i2c_hard.h>



/***********************************************************
**name:	BSP_I2C_HARD_Init
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void BSP_I2C_HARD_Init(str_I2C_GPIO_HARD  I2C_GPIO_HARD)
{
	INT8U i,ii;
	INT16U	GPIO_PinSource;
	INT8U	GPIO_AF;
	INT32U RCC_I2C;

	

	for(i=0;i<2;i++)
	{
		BSP_GPIO_PortClkSwitch(I2C_GPIO_HARD.GPIOx[i],ENABLE);
		GPIO_Init(I2C_GPIO_HARD.GPIOx[i],&I2C_GPIO_HARD.GPIO_InitStruct[i]);

		for(ii=0;ii<16;ii++)
		{
			if(I2C_GPIO_HARD.GPIO_InitStruct[i].GPIO_Pin & (1<<ii))
			{
				GPIO_PinSource = ii;
				break;
			}
		}

		if(I2C_GPIO_HARD.I2Cx == I2C1)
		{
			GPIO_AF = GPIO_AF_I2C1;
			RCC_I2C = RCC_APB1Periph_I2C1;
		}
		else if(I2C_GPIO_HARD.I2Cx == I2C2)
		{
			GPIO_AF = GPIO_AF_I2C2;
			RCC_I2C = RCC_APB1Periph_I2C2;
		}
		else if(I2C_GPIO_HARD.I2Cx == I2C3)
		{
			GPIO_AF = GPIO_AF_I2C3;
			RCC_I2C = RCC_APB1Periph_I2C3;
		}
		else
			continue;

		GPIO_PinAFConfig(I2C_GPIO_HARD.GPIOx[i], GPIO_PinSource, GPIO_AF);
	}
	
	RCC_APB1PeriphClockCmd(RCC_I2C, ENABLE);
	I2C_Init(I2C_GPIO_HARD.I2Cx, &I2C_GPIO_HARD.I2C_InitStructure);
	
	I2C_Cmd(I2C_GPIO_HARD.I2Cx, ENABLE);
	
}




