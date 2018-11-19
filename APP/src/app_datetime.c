


#include <app_datetime.h>
#include <app_tool.h>



#define	APP_DATETIME_DEF


/***********************************************************
**name:	DATETIME_SavetoRtcIC
**describe:  ��ϵͳ����ʱ��洢��ʱ��оƬ֮��
**input:	SysDateTime    ϵͳ����ʱ��ṹ��
**output:	none
**return:  
**autor:  andiman
**date:
************************************************************/
int DATETIME_SavetoRtcIC(strSysDateTime *SysDateTime)
{
	if( DS1302_WriteYear( SysDateTime->year % 100) == FAIL)
		return FAIL;

	if( DS1302_WriteMonth( SysDateTime->month) == FAIL)
		return FAIL;

	if( DS1302_WriteDay( SysDateTime->day) == FAIL)
		return FAIL;

	if( DS1302_WriteHour( SysDateTime->hour) == FAIL)
		return FAIL;

	if( DS1302_WriteMinute( SysDateTime->min) == FAIL)
		return FAIL;

	if( DS1302_WriteSecond( SysDateTime->senc ) == FAIL)
		return FAIL;

	return SUCC;
}

/***********************************************************
**name:	DATETIME_ReadFromRtcIC
**describe:  ��ϵͳ����ʱ��ʱ��оƬ֮�ж�ȡ����
**input:	
**output:	SysDateTime    ϵͳ����ʱ��ṹ��
**return:  
**autor:  andiman
**date:
************************************************************/
void DATETIME_ReadFromRtcIC(strSysDateTime *SysDateTime)
{
	SysDateTime->year = DS1302_ReadYear() + 2000;
	SysDateTime->month = DS1302_ReadMonth();
	SysDateTime->day = DS1302_ReadDay();
	SysDateTime->hour = DS1302_ReadHour();
	SysDateTime->min = DS1302_ReadMinute() % 60;
	SysDateTime->senc = DS1302_ReadSecond() % 60;
}

/***********************************************************
**name:	DATETIME_SavetoRtcIC
**describe:  ���߸���ϵͳʱ��
**input:	SysDateTime    ϵͳ����ʱ��ṹ��
**output:	none
**return:  
**autor:  andiman
**date:
************************************************************/
int DATETIME_RefreshBySec(strSysDateTime *SysDateTime)
{
	SysDateTime->senc++;
	if( SysDateTime->senc >= 60 )
	{
		SysDateTime->senc = 0;
		SysDateTime->min++;
	}

	if( SysDateTime->min >= 60)
	{
		SysDateTime->min= 0;
		SysDateTime->hour++;
	}

	if( SysDateTime->hour >= 24)
	{
		SysDateTime->hour = 0;
		SysDateTime->day++;
	}

	if( 	( SysDateTime->month == 1 ) || ( SysDateTime->month == 3 ) || ( SysDateTime->month == 5 ) || ( SysDateTime->month == 7 ) || 
		( SysDateTime->month == 8 ) || ( SysDateTime->month == 10 ) || ( SysDateTime->month == 12 )  )
	{
		if(  SysDateTime->day > 31 )
		{
			SysDateTime->day = 1;
			SysDateTime->month ++;
		}
	}
	else if( ( SysDateTime->month == 4 ) || ( SysDateTime->month == 6 ) || ( SysDateTime->month == 9 ) || ( SysDateTime->month == 11 ) )
	{
		if(  SysDateTime->day > 30 )
		{
			SysDateTime->day = 1;
			SysDateTime->month ++;
		}
	}
	else if( SysDateTime->month == 2 )
	{
		if (((0==SysDateTime->year % 4) && (0!=SysDateTime->year%100)) || (0==SysDateTime->year%400))  
		{  
			if (SysDateTime->day > 29)  
			{  
				SysDateTime->day =1;  
				SysDateTime->month++;  
			}  
		}  
		else  
		{  
			if (SysDateTime->day>28)  
			{  
				SysDateTime->day=1;  
				SysDateTime->month++;  
			}  
		} 
	}

	if( SysDateTime->month > 12 )
	{
		SysDateTime->month = 1;
		SysDateTime->year++;
	}
	
}


