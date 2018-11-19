

#define BSP_DELAY_DEF

#include "bsp_delay.h"


void BSP_Delay_ms(INT16U ms)
{
	INT16U ms_time;
	OS_ERR    os_err;
	while(ms)
	{
		if(ms > 999)
			ms_time = 999;
		else
			ms_time = ms;
		OSTimeDlyHMSM(0, 0, 0, ms_time,OS_OPT_TIME_HMSM_STRICT, &os_err);
		ms -= ms_time;
	}
}

void BSP_Delay_s(INT8U sec)
{
	INT8U s_time;
	OS_ERR    os_err;
	while(sec)
	{
		if(sec > 59)
			s_time = 59;
		else
			s_time = sec;
		OSTimeDlyHMSM(0, 0, s_time, 0,OS_OPT_TIME_HMSM_STRICT, &os_err);
		sec -= s_time;
	}
}

void BSP_Delay_us(INT32U us)
{
	INT32U i,ii=26;
	for(i=0;i<us;i++)
	{
		while(ii--);
		ii=26;
	}
}






