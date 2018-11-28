#define HW_LED_DEF

#include <hw_led.h>


/***********************************************************
**name:	HW_BEACON_Init
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void HW_LED_Init(void)
{
	BSP_GPIO_Init(HW_LED1_PIN_PORT,	HW_LED1_PIN, GPIO_Mode_OUT,	GPIO_OType_PP,GPIO_PuPd_NOPULL);
	BSP_GPIO_Init(HW_LED2_PIN_PORT,	HW_LED2_PIN, GPIO_Mode_OUT,	GPIO_OType_PP,GPIO_PuPd_NOPULL);
}


/***********************************************************
**name:	HW_LED1_OFF
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void HW_LED1_OFF(void)
{
	BSP_GPIO_PinWriteHigh(HW_LED1_PIN_PORT,HW_LED1_PIN);
}

/***********************************************************
**name:	HW_LED2_OFF
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void HW_LED2_OFF(void)
{
	BSP_GPIO_PinWriteHigh(HW_LED2_PIN_PORT,HW_LED2_PIN);
}

/***********************************************************
**name:	HW_LED1_ON
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void HW_LED1_ON(void)
{
	BSP_GPIO_PinWriteLow(HW_LED1_PIN_PORT,HW_LED1_PIN);
}

/***********************************************************
**name:	HW_LED2_OFF
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void HW_LED2_ON(void)
{
	BSP_GPIO_PinWriteLow(HW_LED2_PIN_PORT,HW_LED2_PIN);
}

/***********************************************************
**name:	HW_LED1_Flick
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void HW_LED1_Flick(void)
{
	GPIO_ToggleBits(HW_LED1_PIN_PORT,HW_LED1_PIN);
}

/***********************************************************
**name:	HW_LED1_Flick
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void HW_LED2_Flick(void)
{
	GPIO_ToggleBits(HW_LED2_PIN_PORT,HW_LED2_PIN);
}

