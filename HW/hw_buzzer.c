


#define HW_BUZZER_DEF

#include <hw_buzzer.h>


/***********************************************************
**name:	HW_BUZZER_Init
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void HW_BUZZER_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  
	BSP_GPIO_PortClkSwitch(HW_BUZZER_PIN_PORT,ENABLE);

	GPIO_InitStructure.GPIO_Pin = HW_BUZZER_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(HW_BUZZER_PIN_PORT, &GPIO_InitStructure);

	HW_BUZZER_Off();
	
}

/***********************************************************
**name:	HW_BUZZER_On
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void HW_BUZZER_On(void)
{
	BSP_GPIO_PinWriteHigh(HW_BUZZER_PIN_PORT,HW_BUZZER_PIN);
}

/***********************************************************
**name:	HW_BUZZER_Off
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void HW_BUZZER_Off(void)
{
	BSP_GPIO_PinWriteLow(HW_BUZZER_PIN_PORT,HW_BUZZER_PIN);
}

/***********************************************************
**name:	HW_BUZZER_Flick
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void HW_BUZZER_Flick(void)
{
	GPIO_ToggleBits(HW_BUZZER_PIN_PORT,HW_BUZZER_PIN);
}


