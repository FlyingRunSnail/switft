
#define HW_POWER_DEF

#include <hw_power.h>


static void HW_12V_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  
	BSP_GPIO_PortClkSwitch(HW_12V_PIN_PORT,ENABLE);

    GPIO_InitStructure.GPIO_Pin = HW_12V_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(HW_12V_PIN_PORT, &GPIO_InitStructure);

    // output high default
    BSP_GPIO_PinWriteHigh(HW_12V_PIN_PORT, HW_12V_PIN);
}

static void HW_BAT_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  
	BSP_GPIO_PortClkSwitch(HW_BAT_PIN_PORT,ENABLE);

    GPIO_InitStructure.GPIO_Pin = HW_BAT_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(HW_BAT_PIN_PORT, &GPIO_InitStructure);

    // output low default
    BSP_GPIO_PinWriteLow(HW_BAT_PIN_PORT, HW_BAT_PIN);
}

static void HW_AP_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  
	BSP_GPIO_PortClkSwitch(HW_AP_PIN_PORT,ENABLE);
  
    //Configure AP (PA3)
    GPIO_InitStructure.GPIO_Pin = HW_AP_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init(HW_AP_PIN_PORT, &GPIO_InitStructure);

    // output high default
    BSP_GPIO_PinWriteLow(HW_AP_PIN_PORT, HW_AP_PIN);
}

/***********************************************************
**name:	HW_4G_Init
**describe: init 4g io control as output
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
static void HW_4G_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  
	BSP_GPIO_PortClkSwitch(HW_4G_PIN_PORT,ENABLE);

	GPIO_InitStructure.GPIO_Pin = HW_4G_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(HW_4G_PIN_PORT, &GPIO_InitStructure);

    // output high default
	BSP_GPIO_PinWriteHigh(HW_4G_PIN_PORT,HW_4G_PIN);
}

/***********************************************************
**name:	HW_MESH_Init
**describe: init mesh io control as output
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
static void HW_mesh_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
  
	BSP_GPIO_PortClkSwitch(HW_MESH_PIN_PORT,ENABLE);

	GPIO_InitStructure.GPIO_Pin = HW_MESH_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(HW_MESH_PIN_PORT, &GPIO_InitStructure);

    // output low default
	BSP_GPIO_PinWriteLow(HW_MESH_PIN_PORT, HW_MESH_PIN);
}

/***********************************************************
**name:	HW_POWER_Init
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void HW_POWER_Init(void)
{
    HW_12V_Init();
    HW_BAT_Init();
    HW_AP_Init();
    HW_4G_Init();
    HW_MESH_Init();
}

/***********************************************************
**name:	HW_12V_On
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void HW_12V_On(void)
{
	BSP_GPIO_PinWriteHigh(HW_12V_PIN_PORT,HW_12V_PIN);
}

/***********************************************************
**name:	HW_12V_Off
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void HW_12V_Off(void)
{
	BSP_GPIO_PinWriteLow(HW_12V_PIN_PORT,HW_12V_PIN);
}

/***********************************************************
**name:	HW_BAT_On
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void HW_BAT_On(void)
{
	BSP_GPIO_PinWriteHigh(HW_BAT_PIN_PORT,HW_BAT_PIN);
}

/***********************************************************
**name:	HW_BAT_Off
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void HW_BAT_Off(void)
{
	BSP_GPIO_PinWriteLow(HW_BAT_PIN_PORT,HW_BAT_PIN);
}

/***********************************************************
**name:	HW_AP_On
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void HW_AP_On(void)
{
	BSP_GPIO_PinWriteLow(HW_AP_PIN_PORT,HW_AP_PIN);
}

/***********************************************************
**name:	HW_AP_Off
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void HW_AP_Off(void)
{
	BSP_GPIO_PinWriteHigh(HW_AP_PIN_PORT,HW_AP_PIN);
}

/***********************************************************
**name:	HW_4G_On
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void HW_4G_On(void)
{
	BSP_GPIO_PinWriteLow(HW_4G_PIN_PORT,HW_4G_PIN);
}

/***********************************************************
**name:	HW_4G_Off
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void HW_4G_Off(void)
{
	BSP_GPIO_PinWriteHigh(HW_4G_PIN_PORT,HW_4G_PIN);
}

/***********************************************************
**name:	HW_MESH_On
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void HW_MESH_On(void)
{
	BSP_GPIO_PinWriteHigh(HW_MESH_PIN_PORT, HW_MESH_PIN);
}

/***********************************************************
**name:	HW_MESH_Off
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void HW_MESH_Off(void)
{
	BSP_GPIO_PinWriteLow(HW_MESH_PIN_PORT, HW_MESH_PIN);
}

