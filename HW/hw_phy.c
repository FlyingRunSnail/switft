
#define HW_PHY_DEF

#include <hw_phy.h>


/***********************************************************
**name:	HW_PHY_Rst_Init
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void HW_PHY_Rst_Init(void)
{
    //Configure PHY Rst (PF9)
    BSP_GPIO_Init(HW_PHY_RST_PIN_PORT, HW_PHY_RST_PIN, GPIO_Mode_OUT, GPIO_OType_PP,GPIO_PuPd_NOPULL);

    // output high default
    BSP_GPIO_PinWriteHigh(HW_PHY_RST_PIN_PORT, HW_PHY_RST_PIN);
}

/***********************************************************
**name:	HW_PHY_Rst
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void HW_PHY_Rst(void)
{
	BSP_GPIO_PinWriteLow(HW_PHY_RST_PIN_PORT, HW_PHY_RST_PIN);
    BSP_Delay_ms(100);
	BSP_GPIO_PinWriteHigh(HW_PHY_RST_PIN_PORT, HW_PHY_RST_PIN);
}

