


#include <app_gps.h>
#include <devdef.h>
#include <app_tool.h>

#define	APP_GPS_DEF





/***********************************************************
**name:	APP_GPS_CheckSumTest
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void  APP_GPS_UTC2BeijingTime(strGPSData *strGPSData,	strSysDateTime *SysDateTime)
{
	SysDateTime->year = strGPSData->year > 100 ? strGPSData->year : ( 2000 + strGPSData->year );
	SysDateTime->month = strGPSData->month;
	SysDateTime->day = strGPSData->day;
	SysDateTime->hour = strGPSData->hour + 8;
	SysDateTime->min = strGPSData->min;
	SysDateTime->senc = strGPSData->sec;
	SysDateTime->ms = strGPSData->ms;

	if( SysDateTime->hour >= 24 )
	{
		SysDateTime->hour -= 24;
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


/***********************************************************
**name:	APP_GPS_DataGet
**describe:   获取GPS数据
**input:	
**output:	none
**return:  
**autor:  andiman
**date:
************************************************************/
int APP_GPS_DataGet(void)
{
	INT8U sendbuff[32],revbuff[32];
	INT32U len;
	INT8U index = 0;
	INT8U checksum = 0;

	// 帧头
	sendbuff[index++] = 0xBC;

	// 帧数据长度 
	sendbuff[index++] = 0x03;

	// 命令字  0x0401
	sendbuff[index++] = 0x0C;
	sendbuff[index++] = 0x01;

	// 长度
	sendbuff[index++] = 0x00;

	// 校验和
	checksum = TOOL_CheckSum8BitGet(sendbuff,index);
	sendbuff[index++] = checksum;

	// 帧尾
	sendbuff[index++] = 0xCB;

	SWIFT_UART4_SendBuff(sendbuff, index);

	// pdu 收到命令后需要马上回复，表示收到
	if( BSP_OS_Sem_Pend(&SWIFT_UART4_Rev_Sem,300) == FAIL)
	{
		ACUDevInfo.DeviceInfo.PDUommStat.link_stat = COMM_UNLINK;
		//ACUDevInfo.ACUParam.LocalPositionParam.data_src_flag = 1;
		return FAIL;
	}

	
	SWIFT_UART4_RecvBuff(revbuff,&len);

	ACUDevInfo.DeviceInfo.PDUommStat.link_stat = COMM_LINK;

	if(( revbuff[2] != 0x8C ) && ( revbuff[3] != 0x01) &&( revbuff[4] != 0x16 ))
		return FAIL;

	index = 5;
	ACUDevInfo.DeviceInfo.GPSInfo.GPSData.valid = revbuff[index++];

	if( ACUDevInfo.DeviceInfo.GPSInfo.GPSData.valid != 'A' )
	{
// 		ACUDevInfo.ACUParam.LocalPositionParam.data_src_flag = 1;
		return FAIL;
	}

	ACUDevInfo.DeviceInfo.GPSInfo.GPSData.lon_flag = revbuff[index++];
	ACUDevInfo.DeviceInfo.GPSInfo.GPSData.lon = (revbuff[index]<<24) + (revbuff[index+1]<<16) + (revbuff[index+2]<<8) + revbuff[index+3];
	index += 4;

	ACUDevInfo.DeviceInfo.GPSInfo.GPSData.lat_flag = revbuff[index++];
	ACUDevInfo.DeviceInfo.GPSInfo.GPSData.lat = (revbuff[index]<<24) + (revbuff[index+1]<<16) + (revbuff[index+2]<<8) + revbuff[index+3];
	index += 4;

	ACUDevInfo.DeviceInfo.GPSInfo.GPSData.height = (revbuff[index]<<24) + (revbuff[index+1]<<16) + (revbuff[index+2]<<8) + revbuff[index+3];
	index += 4;

	ACUDevInfo.DeviceInfo.GPSInfo.GPSData.year =  (revbuff[index]<<8) + revbuff[index+1];
	index += 2;

	ACUDevInfo.DeviceInfo.GPSInfo.GPSData.month = revbuff[index++];

	ACUDevInfo.DeviceInfo.GPSInfo.GPSData.day = revbuff[index++];

	ACUDevInfo.DeviceInfo.GPSInfo.GPSData.hour = revbuff[index++];

	ACUDevInfo.DeviceInfo.GPSInfo.GPSData.min = revbuff[index++];

	ACUDevInfo.DeviceInfo.GPSInfo.GPSData.sec = revbuff[index++] + 1;

	ACUDevInfo.ACUParam.LocalPositionParam.data_src_flag = 0x00;
	ACUDevInfo.ACUParam.LocalPositionParam.valid_flag = 0x01;
	ACUDevInfo.ACUParam.LocalPositionParam.lon_flag = ACUDevInfo.DeviceInfo.GPSInfo.GPSData.lon_flag;
	ACUDevInfo.ACUParam.LocalPositionParam.lon = (FP32)ACUDevInfo.DeviceInfo.GPSInfo.GPSData.lon / 1000.0;
	ACUDevInfo.ACUParam.LocalPositionParam.lat_flag = ACUDevInfo.DeviceInfo.GPSInfo.GPSData.lat_flag;
	ACUDevInfo.ACUParam.LocalPositionParam.lat = (FP32)ACUDevInfo.DeviceInfo.GPSInfo.GPSData.lat / 1000.0;
	ACUDevInfo.ACUParam.LocalPositionParam.height = (FP32)ACUDevInfo.DeviceInfo.GPSInfo.GPSData.height;

	
	
	return SUCC;
}

