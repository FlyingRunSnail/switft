
#define HW_WTD_DEF

#include <hw_watchdog.h>



/***********************************************************
**name:	HW_WTD_Init
**describe:   ø¥√≈π∑≥ı ºªØ
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void HW_WTD_Init(void)
{
	/*
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);	

	WWDG_SetPrescaler(WWDG_Prescaler_8);		// 8∑÷∆µ £¨ 60MHZ   /   8

	WWDG_SetWindowValue(0x60);

	WWDG_SetCounter(0x7f);

	WWDG_Enable(0x7f);

//	POWER_LNB_PowerUp();

//	POWER_BUC_PowerUp();
	*/

	 RCC->CSR |= ((uint32_t)RCC_CSR_LSION);
	
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetReload(0xFFF);			// 4096∫¡√Î
	IWDG_SetPrescaler(IWDG_Prescaler_32);
	//IWDG_WriteAccessCmd(IWDG_WriteAccess_Disable);

	

	IWDG_ReloadCounter();
	IWDG_Enable();
	
	//IWDG_Enable();
}

/***********************************************************
**name:	HW_WTD_Init
**describe:   
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void HW_WTD_Feed(void)
{
	//WWDG_Enable(0x7f);
	//IWDG_SetReload(0x7FF);		// 100∫¡√Î
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetReload(0xFFF);			// 4096∫¡√Î
	//IWDG_WriteAccessCmd(IWDG_WriteAccess_Disable);
	IWDG_ReloadCounter();
	IWDG_Enable();
	
}

