


#define APP_BUZZER_DEF

#include <app_buzzer.h>
#include <main.h>


//strBuzzerQ	BuzzerQ;

/***********************************************************
**name:	APP_BUZZER_Init
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void APP_BUZZER_Init(void)
{
	HW_BUZZER_Init();
	ACUDevInfo.DeviceInfo.BuzzerInfo.buzzer_switch = ENABLE;
}

/***********************************************************
**name:	APP_BUZZER_On
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void APP_BUZZER_On(void)
{
	HW_BUZZER_On();
}

/***********************************************************
**name:	APP_BUZZER_Off
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void APP_BUZZER_Off(void)
{
	HW_BUZZER_Off();
}

/***********************************************************
**name:	APP_BUZZER_Flick
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void APP_BUZZER_Flick(void)
{
	HW_BUZZER_Flick();
}

/***********************************************************
**name:	APP_BUZZER_BeepOpt
**describe: 蜂鸣器鸣叫次数操作控制,  
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void APP_BUZZER_BeepOpt(INT16U beep_ms,INT16U period_ms,INT8U flick_flag,INT32U cnt)
{
	ACUDevInfo.DeviceInfo.BuzzerInfo.flick_flag = flick_flag;
	ACUDevInfo.DeviceInfo.BuzzerInfo.beep_ms = beep_ms;
	ACUDevInfo.DeviceInfo.BuzzerInfo.period_ms = period_ms;
	ACUDevInfo.DeviceInfo.BuzzerInfo.beep_cnt = cnt;


}


/***********************************************************
**name:	APP_BUZZER_Switch
**describe: 蜂鸣器开关
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void APP_BUZZER_Switch(INT8U switch_set)
{
	ACUDevInfo.DeviceInfo.BuzzerInfo.buzzer_switch = switch_set > 0 ? ENABLE : DISABLE;
}

/***********************************************************
**name:	APP_BUZZER_SingleBeep
**describe:
**input:			beep_ms: 响的持续时间
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void APP_BUZZER_SingleBeep(INT16U beep_ms)
{
	//APP_BUZZER_On();
	BSP_Delay_ms(beep_ms);
	//APP_BUZZER_Off();
}

/***********************************************************
**name:	APP_BUZZER_ContinualBeep
**describe:
**input:			period_ms: 响的持续时间
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void APP_BUZZER_ContinualBeep(INT16U beep_ms,INT16U period_ms,INT32U beep_cnt)
{
	INT32U i=0;
	for(i=0;i<beep_cnt;i++)
	{
		APP_BUZZER_SingleBeep(beep_ms);
		BSP_Delay_ms(period_ms);
	}
}


/***********************************************************
**name:	APP_BUZZER_Deal
**describe:
**input:			period_ms: 响的持续时间
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void APP_BUZZER_Deal(INT32U period_ms,INT16U *delay_ms)
{
	static INT32U ms_cnt = 0;
	if( ACUDevInfo.DeviceInfo.BuzzerInfo.buzzer_switch == ENABLE )
	{
		
		if(( ms_cnt * period_ms >= ACUDevInfo.DeviceInfo.BuzzerInfo.period_ms ) &&( ACUDevInfo.DeviceInfo.BuzzerInfo.flick_flag > 0))
		{
			APP_BUZZER_SingleBeep(ACUDevInfo.DeviceInfo.BuzzerInfo.beep_ms);
			*delay_ms = ACUDevInfo.DeviceInfo.BuzzerInfo.beep_ms;
			ms_cnt = 0;
		}
		else
			*delay_ms = 0;
		ms_cnt++;
	}
	else
		*delay_ms = 0;
}


