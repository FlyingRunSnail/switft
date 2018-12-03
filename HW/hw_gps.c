
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
    //Configure AP (PA11 PA12)
    BSP_GPIO_Init(HW_GPS_CTRL_PIN_PORT, HW_GPS_CTRL_PIN | HW_GPS_RST_PIN, GPIO_Mode_OUT, GPIO_OType_PP,GPIO_PuPd_NOPULL);


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
	BSP_GPIO_PinWriteHigh(HW_GPS_CTRL_PIN_PORT, HW_GPS_RST_PIN);
}

