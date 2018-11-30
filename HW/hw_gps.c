
#define HW_GPS_DEF

#include <hw_gps.h>


/***********************************************************
**name:	HW_POWER_Init
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void HW_GPS_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  
	BSP_GPIO_PortClkSwitch(HW_GPS_CTRL_PIN_PORT,ENABLE);
  
    //Configure AP (PA11 PA12)
    GPIO_InitStructure.GPIO_Pin = HW_GPS_CTRL_PIN | HW_GPS_RST_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init(HW_GPS_CTRL_PIN_PORT, &GPIO_InitStructure);

    // output high default
    BSP_GPIO_PinWriteHigh(HW_GPS_CTRL_PIN_PORT, HW_GPS_CTRL_PIN);
    BSP_GPIO_PinWriteHigh(HW_GPS_CTRL_PIN_PORT, HW_GPS_RST_PIN);
}

/***********************************************************
**name:	HW_GPS_On
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void HW_GPS_On(void)
{
	BSP_GPIO_PinWriteHigh(HW_GPS_CTRL_PIN_PORT, HW_GPS_CTRL_PIN);
}

/***********************************************************
**name:	HW_GPS_Off
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void HW_GPS_Off(void)
{
	BSP_GPIO_PinWriteLow(HW_GPS_CTRL_PIN_PORT, HW_GPS_CTRL_PIN);
}

/***********************************************************
**name:	HW_GPS_Rst
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void HW_GPS_Rst(void)
{
	BSP_GPIO_PinWriteLow(HW_GPS_CTRL_PIN_PORT, HW_GPS_RST_PIN);
    BSP_Delay_ms(100);
}

