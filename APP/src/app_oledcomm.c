


#define APP_OLEDCOMM_DEF

#include <app_oledcomm.h>
#include <app_tool.h>
#include <app_param_save_read.h>
#include <app_log.h>
#include <app_beacon.h>
#include <app_monitor.h>
#define		APP_OLEDCOMM_TIMEOUT_MS		100


INT8U OLEDCOMMRxBuff[50];
INT8U OLEDCOMMRxBuffLen = 0;

INT8U OLEDCOMMTxBuff[64];
INT16U OLEDCOMMTxBuffLen = 0;

typedef struct _strOLEDCOMMCmdDeal_
{
	INT16U cmd_id;
	int (* APP_OLEDCOMM_Func_Deal)(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);
}strOLEDCOMMCmdDeal;

strOLEDCOMMCmdDeal OLEDCOMMCmdDealTbl[]=
{
	//{ SatParamSetID	,			APP_OLEDCOMM_SatParamSet },
	//{ AntennaParamSetID	,		APP_OLEDCOMM_AntennaParamSet },
	//{ AntennaParamAdjustID	,	APP_OLEDCOMM_AntennaParamAdjustSet },
	{ LocalParamSetID	,			APP_OLEDCOMM_LocalParamSet },
	{ LocalParamSrcSelSetID,		APP_OLEDCOMM_LocalParamSrcSelSet },
	{ SysDateTimeSetID	,		APP_OLEDCOMM_SysDateTimeSet },
	{ SysDateTimeSrcSelSetID,		APP_OLEDCOMM_SysDateTimeSrcSelSet },
	//{ SatDelID,					APP_OLEDCOMM_DelSatSet },
	
	{ AimAtSatModeSelID	,		APP_OLEDCOMM_AimAtSatModeSel },
	{ AntennaRecordID	,		APP_OLEDCOMM_AntennaRecord},
	{ AntennaSigleStepCtrID	,	APP_OLEDCOMM_AntennaSigleStepCtr },
	//{ AntennaDegreeCtrID	,		APP_OLEDCOMM_AntennaTurnCtr },
	{AntennaRecordGotoID,			APP_OLEDCOMM_AntennaRecordGoto},
	{ AntennaRecordDelID,			APP_OLEDCOMM_DelAntennaRecord},
	{AGCRecordID,				APP_OLEDCOMM_AGCRecord},
	{ SatSelID	,				APP_OLEDCOMM_SelSat },
//	{ MotorSoftLimitSetID	,		APP_OLEDCOMM_MotorSoftLimit },
	{ NetWorkSetID	,			APP_OLEDCOMM_NetWorkSet },
	{ PCHighAdminID	,			APP_OLEDCOMM_PCAdminSet },

	
	{ CurrSatParamQueID	,			APP_OLEDCOMM_CurrSatParamQue },
	{ AllSaveSatParamQueID	,		APP_OLEDCOMM_SaveAllSatParamQue },
	{ AntennaParamQueID	,			APP_OLEDCOMM_AntennaParamQue },
	{ AntennaAdjustParamQueID	,	APP_OLEDCOMM_AntennaAdjustParamQue },
	{ LocalParamQueID	,			APP_OLEDCOMM_LocalParamQue },
	{ DevSysDateTimeQueID	,		APP_OLEDCOMM_DevSysDateTimeQue },
	{ NetWorkQueID,					APP_OLEDCOMM_NetWorkQue			},
	{ AimAtSatTheoreticalDegreeQueID,	APP_OLEDCOMM_AimAtSatDegreeQue },
	{ AntennaRecordQueID	,			APP_OLEDCOMM_AntennaRecordQue },
	{ MotorSoftLimitQueID	,			APP_OLEDCOMM_MotorSoftLimitQue },
	{ DevStaQueID	,			APP_OLEDCOMM_DevStaQue },
	{ DevAdminQueID	,			APP_OLEDCOMM_DevAdminQue },
	{ CurrWarnStaQueID	,			APP_OLEDCOMM_DevAlarmQue },
	
	//{ WarnLogQueID	,			APP_OLEDCOMM_WarnLogQue },
	//{ OperationLogQueID	,			APP_OLEDCOMM_OptLogQue },
	//{ ParamSetLogQueID ,				},
	{ VersionQueID	,			APP_OLEDCOMM_VersionQue },
	
	
};

static INT16U OLEDCOMMCmdDealTbl_Size = sizeof(OLEDCOMMCmdDealTbl) / sizeof(strOLEDCOMMCmdDeal);

/***********************************************************
**name:	APP_OLEDCOMM_Init
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void APP_OLEDCOMM_Init(void)
{
	SWIFT_UART3_Init(115200);
	
	OLEDCOMMRx.databuff = OLEDCOMMRxBuff;
	
	OLEDCOMMTx.head = 0xBC;
	OLEDCOMMTx.datalen = 0;
	OLEDCOMMTx.databuff = &OLEDCOMMTxBuff[2];
	OLEDCOMMTx.checksum = 0;
	OLEDCOMMTx.end = 0xCB;

	/*
	ACUDevInfo.ACUParam.CurrSatParam.freq = 41985000;
	ACUDevInfo.ACUParam.CurrSatParam.id = 0x01;
	ACUDevInfo.ACUParam.CurrSatParam.lon_flag = 'E';
	ACUDevInfo.ACUParam.CurrSatParam.lon = 105.500;
	memcpy(ACUDevInfo.ACUParam.CurrSatParam.name , "A4",2);
	ACUDevInfo.ACUParam.CurrSatParam.polar_type = 'X';
	*/
}



/***********************************************************
**name:	APP_OLEDCOMM_RxBuffCheck
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OLEDCOMM_RxBuffCheck(INT8U *buff, INT32U *len)
{
	INT8U datalen;
	INT8U checksum=0;
	
	if( buff[0] != 0xBC)
		return FAIL;
	datalen = buff[1];

	if( *len > datalen + 4 )
		*len = datalen + 4;

	if( buff[*len -1] != 0xCB)
		return FAIL;

	checksum = TOOL_CheckSum8BitGet(buff,*len-2);
	if( checksum != buff[*len -2] )
		return FAIL;

	return SUCC;
}

/***********************************************************
**name:	APP_OLEDCOMM_RxBuffGet
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OLEDCOMM_RxBuffRecv(void)
{
	INT8U buff[259]={0};
	INT32U len = 0;


	SWIFT_UART3_RecvBuff(buff,&len);

	if(APP_OLEDCOMM_RxBuffCheck(buff, &len) == FAIL)
		return FAIL;

	OLEDCOMMRx.datalen = buff[1];
	memcpy( OLEDCOMMRx.databuff,&buff[2],OLEDCOMMRx.datalen);

	return SUCC;
}

/***********************************************************
**name:	APP_OLEDCOMM_TxBuffSend
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void APP_OLEDCOMM_TxBuffSend(strPCCOMMPro txbuff)
{
	INT16U index=0;

	OLEDCOMMTxBuff[index++] = txbuff.head;
	OLEDCOMMTxBuff[index++] = txbuff.datalen;
	//memcpy(&OLEDCOMMTxBuff[index],txbuff.databuff,txbuff.datalen);
	index += txbuff.datalen;
	OLEDCOMMTxBuff[index++] = TOOL_CheckSum8BitGet(OLEDCOMMTxBuff,index);
	OLEDCOMMTxBuff[index++] = txbuff.end;

	OLEDCOMMTxBuffLen = index;
	SWIFT_UART3_SendBuff(OLEDCOMMTxBuff,index);
	//APP_UDP_Sendto(OLEDCOMMTxBuff,OLEDCOMMTxBuffLen);
}

/***********************************************************
**name:	APP_OLEDCOMM_CmdDeal
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OLEDCOMM_CmdDeal(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U i=0;
	INT16U cmd_id = 0;
	int status = FAIL;

	cmd_id = (rxbuff[0]<<8)+rxbuff[1];
	
	ACUDevInfo.DeviceInfo.PCCommStat.mcu_busy = 1;
	
	for(i=0;i<OLEDCOMMCmdDealTbl_Size;i++)
	{
		if( cmd_id == OLEDCOMMCmdDealTbl[i].cmd_id )
		{
			status = OLEDCOMMCmdDealTbl[i].APP_OLEDCOMM_Func_Deal(rxbuff, rxbufflen,txbuff, txbufflen);
			if( ((cmd_id & 0xFF00) == 0x0100) || ((cmd_id & 0xFF00) == 0x0200))
			{
				APP_OLEDCOMM_SetResult_Send(cmd_id,status);
			}
			else if( ((cmd_id & 0xFF00) == 0x0800) || ((cmd_id & 0xFF00) == 0x0A00))
			{
				APP_OLEDCOMM_TxBuffSend(OLEDCOMMTx);
			}
			ACUDevInfo.DeviceInfo.PCCommStat.mcu_busy = 0;
			return status;
		}
	}
	ACUDevInfo.DeviceInfo.PCCommStat.mcu_busy = 0;
	return status;
}


/***********************************************************
**name:	APP_OLEDCOMM_Deal
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OLEDCOMM_Deal(void)
{
	if( APP_OLEDCOMM_RxBuffRecv()  == FAIL)
		return FAIL;

	if(APP_OLEDCOMM_CmdDeal(OLEDCOMMRx.databuff, &OLEDCOMMRx.datalen,OLEDCOMMTx.databuff, &OLEDCOMMTx.datalen) == FAIL)
		return FAIL;

	return SUCC;
}

/***********************************************************
**name:	APP_OLEDCOMM_SetResult_Send
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void APP_OLEDCOMM_SetResult_Send(INT16U cmd_id,int status)
{
	INT8U index=0;

	OLEDCOMMTx.head = 0xBC;
	
	OLEDCOMMTx.databuff[index++] = ((cmd_id | 0x8000) & 0xFF00)>>8;
	OLEDCOMMTx.databuff[index++] = ((cmd_id | 0x8000) & 0x00ff);

	OLEDCOMMTx.databuff[index++] = 1;

	if( status == SUCC)
		OLEDCOMMTx.databuff[index++] = 0;
	else
		OLEDCOMMTx.databuff[index++] = 1;

	OLEDCOMMTx.datalen = index;

	OLEDCOMMTx.end = 0xCB;

	APP_OLEDCOMM_TxBuffSend(OLEDCOMMTx);

	
	
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
//具体命令处理函数


/***********************************************************
**name:	APP_OLEDCOMM_LocalParamSet
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OLEDCOMM_LocalParamSet(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id;
	INT8U index = 0;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];

	strHandLocalPositionParam	CurrLocalPositionParam;

	cmd_id = (rxbuff[0]<<8)+rxbuff[1];

	if( (cmd_id != LocalParamSetID ) || (cmdlen != 0x0E ))
		return FAIL;

	CurrLocalPositionParam.lon_flag = pbuff[index++];
	CurrLocalPositionParam.lon = (FP32)( (pbuff[index]<<24) +  (pbuff[index+1]<<16) + (pbuff[index+2]<<8) +pbuff[index+3])/1000;
	index += 4;

	CurrLocalPositionParam.lat_flag = pbuff[index++];
	CurrLocalPositionParam.lat =   (FP32)((pbuff[index]<<24) +  (pbuff[index+1]<<16) + (pbuff[index+2]<<8) +pbuff[index+3])/1000;
	index += 4;

	CurrLocalPositionParam.height =  (pbuff[index]<<24) +  (pbuff[index+1]<<16) + (pbuff[index+2]<<8) +pbuff[index+3];
	index += 4;

	BSP_Delay_ms(2);

	if(	(CurrLocalPositionParam.lon_flag != ACUDevInfo.ACUParam.LocalPositionParam.lon_flag) ||
		(CurrLocalPositionParam.lon != ACUDevInfo.ACUParam.LocalPositionParam.lon) ||
		(CurrLocalPositionParam.lat_flag != ACUDevInfo.ACUParam.LocalPositionParam.lat_flag) ||
		(CurrLocalPositionParam.lat != ACUDevInfo.ACUParam.LocalPositionParam.lat)	)
	{
		ACUDevInfo.ACUParam.LocalPositionParam.lon_flag = CurrLocalPositionParam.lon_flag;
		ACUDevInfo.ACUParam.LocalPositionParam.lon = CurrLocalPositionParam.lon;
		ACUDevInfo.ACUParam.LocalPositionParam.lat_flag = CurrLocalPositionParam.lat_flag;
		ACUDevInfo.ACUParam.LocalPositionParam.lat = CurrLocalPositionParam.lat;

		ACUDevInfo.ACUParam.LocalPositionParam.data_src_flag = 0x01;		//  手动输入

		BSP_Delay_ms(2);
		
		// 并且重新计算理论倾角
		APP_MONITOR_AimDegree_Get();
	
		//  存储到EEPROM中去
		APP_PARAM_LocallHandParam_Save(&CurrLocalPositionParam);

		ACUDevInfo.ACUCurrSta.dev_param_reflash_flag[0] = 0x01;
		ACUDevInfo.ACUCurrSta.dev_param_reflash_flag[1] = 0x01;
	}

	// 存储操作日志到flash中去
	{
		APP_OptLOG_Save_LocallParamOpt(LOG_OPT_SET_LOCALL_PARAM,CurrLocalPositionParam);
	}


	return SUCC;
}




/***********************************************************
**name:	APP_OLEDCOMM_LocalParamSrcSelSet
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OLEDCOMM_LocalParamSrcSelSet(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id;
	INT8U index = 0;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];

	cmd_id = (rxbuff[0]<<8)+rxbuff[1];

	if( (cmd_id != LocalParamSrcSelSetID ) || (cmdlen != 1 ))
		return FAIL;
	
	ACUDevInfo.ACUParam.LocalPositionParam.data_src_flag = pbuff[index++];

	if( ACUDevInfo.ACUParam.LocalPositionParam.data_src_flag == 0x02)
	{
		ACUDevInfo.ACUParam.LocalPositionParam.lon_flag = ACUDevInfo.DeviceInfo.GPSInfo.GPSData.lon_flag;
		ACUDevInfo.ACUParam.LocalPositionParam.lon = (FP32)ACUDevInfo.DeviceInfo.GPSInfo.GPSData.lon / 100;

		ACUDevInfo.ACUParam.LocalPositionParam.lat_flag = ACUDevInfo.DeviceInfo.GPSInfo.GPSData.lat_flag;
		ACUDevInfo.ACUParam.LocalPositionParam.lat =  (FP32)ACUDevInfo.DeviceInfo.GPSInfo.GPSData.lat / 100;

		ACUDevInfo.ACUParam.LocalPositionParam.height = ACUDevInfo.DeviceInfo.GPSInfo.GPSData.height;

		ACUDevInfo.ACUParam.LocalPositionParam.valid_flag  = (ACUDevInfo.DeviceInfo.GPSInfo.GPSData.valid=='A' ? 1:0);

		BSP_Delay_ms(2);

	}
	else if( ACUDevInfo.ACUParam.LocalPositionParam.data_src_flag == 0x01)
	{
		ACUDevInfo.ACUParam.LocalPositionParam.lon_flag = ACUDevInfo.DeviceInfo.HandLocalPositionParam.lon_flag;
		ACUDevInfo.ACUParam.LocalPositionParam.lon = ACUDevInfo.DeviceInfo.HandLocalPositionParam.lon;

		ACUDevInfo.ACUParam.LocalPositionParam.lat_flag = ACUDevInfo.DeviceInfo.HandLocalPositionParam.lat_flag;
		ACUDevInfo.ACUParam.LocalPositionParam.lat = ACUDevInfo.DeviceInfo.HandLocalPositionParam.lat;

		ACUDevInfo.ACUParam.LocalPositionParam.height = ACUDevInfo.DeviceInfo.HandLocalPositionParam.height;

		ACUDevInfo.ACUParam.LocalPositionParam.valid_flag = ACUDevInfo.DeviceInfo.HandLocalPositionParam.valid_flag;

		BSP_Delay_ms(2);

		
	}
	else
	{
		if( ACUDevInfo.DeviceInfo.GPSInfo.GPSData.valid == 'A')
		{
			ACUDevInfo.ACUParam.LocalPositionParam.lon_flag = ACUDevInfo.DeviceInfo.GPSInfo.GPSData.lon_flag;
			ACUDevInfo.ACUParam.LocalPositionParam.lon = ACUDevInfo.DeviceInfo.GPSInfo.GPSData.lon;

			ACUDevInfo.ACUParam.LocalPositionParam.lat_flag = ACUDevInfo.DeviceInfo.GPSInfo.GPSData.lat_flag;
			ACUDevInfo.ACUParam.LocalPositionParam.lat = ACUDevInfo.DeviceInfo.GPSInfo.GPSData.lat;

			ACUDevInfo.ACUParam.LocalPositionParam.height = ACUDevInfo.DeviceInfo.GPSInfo.GPSData.height;

			ACUDevInfo.ACUParam.LocalPositionParam.valid_flag  = (ACUDevInfo.DeviceInfo.GPSInfo.GPSData.valid=='A' ? 1:0);

			BSP_Delay_ms(2);
		}
		else
		{
			if( ACUDevInfo.DeviceInfo.HandLocalPositionParam.valid_flag > 0 )
			{
				ACUDevInfo.ACUParam.LocalPositionParam.lon_flag = ACUDevInfo.DeviceInfo.HandLocalPositionParam.lon_flag;
				ACUDevInfo.ACUParam.LocalPositionParam.lon = ACUDevInfo.DeviceInfo.HandLocalPositionParam.lon;

				ACUDevInfo.ACUParam.LocalPositionParam.lat_flag = ACUDevInfo.DeviceInfo.HandLocalPositionParam.lat_flag;
				ACUDevInfo.ACUParam.LocalPositionParam.lat = ACUDevInfo.DeviceInfo.HandLocalPositionParam.lat;

				ACUDevInfo.ACUParam.LocalPositionParam.height = ACUDevInfo.DeviceInfo.HandLocalPositionParam.height;

				ACUDevInfo.ACUParam.LocalPositionParam.valid_flag  = ACUDevInfo.DeviceInfo.HandLocalPositionParam.valid_flag;

				BSP_Delay_ms(2);
			}
			else
			{
				ACUDevInfo.ACUParam.LocalPositionParam.lon_flag = ACUDevInfo.DeviceInfo.GPSInfo.GPSData.lon_flag;
				ACUDevInfo.ACUParam.LocalPositionParam.lon =  (FP32)ACUDevInfo.DeviceInfo.GPSInfo.GPSData.lon / 100;

				ACUDevInfo.ACUParam.LocalPositionParam.lat_flag = ACUDevInfo.DeviceInfo.GPSInfo.GPSData.lat_flag;
				ACUDevInfo.ACUParam.LocalPositionParam.lat =  (FP32)ACUDevInfo.DeviceInfo.GPSInfo.GPSData.lat / 100;

				ACUDevInfo.ACUParam.LocalPositionParam.height = ACUDevInfo.DeviceInfo.GPSInfo.GPSData.height;

				ACUDevInfo.ACUParam.LocalPositionParam.valid_flag  = (ACUDevInfo.DeviceInfo.GPSInfo.GPSData.valid=='A' ? 1:0);

				BSP_Delay_ms(2);
			}
		}
	}

	

	// 并且重新计算理论倾角
	APP_MONITOR_AimDegree_Get();

	APP_PARAM_LocallParamSrc_Save(ACUDevInfo.ACUParam.LocalPositionParam.data_src_flag);

	
	// 存储操作日志到flash中去
	{
		;
	}

	return SUCC;
}

/***********************************************************
**name:	APP_OLEDCOMM_SysDateTimeSet
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OLEDCOMM_SysDateTimeSet(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id;
	INT8U index = 0;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];

	cmd_id = (rxbuff[0]<<8)+rxbuff[1];
	

	if( (cmd_id != SysDateTimeSetID )|| (cmdlen != 7 ))
		return FAIL;

	if( ACUDevInfo.DeviceInfo.GPSInfo.GPSData.valid == 'A')
		return SUCC;

	if( ACUDevInfo.SysDateTime.src_flag == 0x01)
	{
	
		ACUDevInfo.SysDateTime.year = (pbuff[index]<<8) + pbuff[index+1];
		index += 2;

		ACUDevInfo.SysDateTime.month = pbuff[index++];

		ACUDevInfo.SysDateTime.day = pbuff[index++];

		ACUDevInfo.SysDateTime.hour = pbuff[index++];

		ACUDevInfo.SysDateTime.min = pbuff[index++];

		ACUDevInfo.SysDateTime.senc = pbuff[index++];

		ACUDevInfo.SysDateTime.ms = 0;

		ACUDevInfo.SysDateTime.systick = TOOL_SysTickGet();
	}

	// 存储时钟芯片中去
	{
		;
	}

	// 存储操作日志到flash中去
	{
		;
	}

	return SUCC;
}


/***********************************************************
**name:	APP_OLEDCOMM_SysDateTimeSrcSelSet
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OLEDCOMM_SysDateTimeSrcSelSet(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id;
	INT8U index = 0;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];

	cmd_id = (rxbuff[0]<<8)+rxbuff[1];

	if( (cmd_id != SysDateTimeSrcSelSetID ) || (cmdlen != 1 ))
		return FAIL;
	
	 ACUDevInfo.SysDateTime.src_flag = pbuff[index++];

	if( ( ACUDevInfo.SysDateTime.src_flag == 0x02) || (ACUDevInfo.SysDateTime.src_flag == 0x00))
	{
		if( ACUDevInfo.DeviceInfo.GPSInfo.GPSData.valid == 'A')
		{
			ACUDevInfo.SysDateTime.year = ACUDevInfo.DeviceInfo.GPSInfo.GPSData.year;
			ACUDevInfo.SysDateTime.month= ACUDevInfo.DeviceInfo.GPSInfo.GPSData.month;
			ACUDevInfo.SysDateTime.day = ACUDevInfo.DeviceInfo.GPSInfo.GPSData.day;
			ACUDevInfo.SysDateTime.hour = ACUDevInfo.DeviceInfo.GPSInfo.GPSData.hour;
			ACUDevInfo.SysDateTime.min = ACUDevInfo.DeviceInfo.GPSInfo.GPSData.min;
			ACUDevInfo.SysDateTime.senc = ACUDevInfo.DeviceInfo.GPSInfo.GPSData.sec;
			ACUDevInfo.SysDateTime.ms = ACUDevInfo.DeviceInfo.GPSInfo.GPSData.ms;
		}
	}

	//  存储到EEPROM中去
		{
			;
		}
	
	// 存储操作日志到flash中去
	{
		;
	}

	return SUCC;
}





/***********************************************************
**name:	APP_OLEDCOMM_AimAtSatModeSel
**describe: 追星模式选择
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OLEDCOMM_AimAtSatModeSel(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id;
	INT8U index = 0;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];
	INT8U SeekSat_Mode=0;


	cmd_id = (rxbuff[0]<<8)+rxbuff[1];
	

	if( (cmd_id != AimAtSatModeSelID )|| (cmdlen != 1 ))
		return FAIL;

	SeekSat_Mode = pbuff[index++];

	//if( ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode != SeekSat_Mode )
	{
		if( SeekSat_Mode == 0x01)
		{
			if( Task_Run_Flag[0] > 0 )
				TaskSatelliteTrackDel();
			if( Task_Run_Flag[1] > 0 )
				TaskSatGradientTrackDel();
			if( Task_Run_Flag[2] > 0 )
				TaskAntennaRecordGotoDel();

			// 安全考虑，停止所有电机的动作
			if( Antenna_AllDriveEnd() == FAIL)
				Antenna_AllDriveEnd();

			APP_OptLOG_Save(LOG_OPT_HAND_AIM_STAR_BEGIN_BY_OLED,&rxbuff[3],0);
			ACUDevInfo.ACUCurrSta.dev_work_sta = 0x11;
		}
		else if( SeekSat_Mode == 0x02)
		{
			if( Task_Run_Flag[0] > 0 )
				TaskSatelliteTrackDel();
			if( Task_Run_Flag[1] > 0 )
				TaskSatGradientTrackDel();
			if( Task_Run_Flag[2] > 0 )
				TaskAntennaRecordGotoDel();

			// 安全考虑，停止所有电机的动作
			if( Antenna_AllDriveEnd() == FAIL)
				Antenna_AllDriveEnd();

			ACUDevInfo.ACUCurrSta.dev_work_sta = 0x20;
			TaskSatelliteTrackCreate();
			APP_OptLOG_Save(LOG_OPT_AUTO_AIM_STAR_BEGIN_BY_OLED,&rxbuff[3],0);
		}
		else if( SeekSat_Mode == 0x03)
		{
			if( Task_Run_Flag[0] > 0 )
				TaskSatelliteTrackDel();
			if( Task_Run_Flag[1] > 0 )
				TaskSatGradientTrackDel();
			if( Task_Run_Flag[2] > 0 )
				TaskAntennaRecordGotoDel();

			// 安全考虑，停止所有电机的动作
			if( Antenna_AllDriveEnd() == FAIL)
				Antenna_AllDriveEnd();
				
			ACUDevInfo.ACUCurrSta.dev_work_sta = 0x30;
			TaskSatGradientTrackCreate();
			APP_OptLOG_Save(LOG_OPT_FOLLOW_STAR_BEGIN_BY_OLED,&rxbuff[3],0);
		}
		else if( SeekSat_Mode == 0x00)
		{
			if(  ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode == 0x01 )
			{
				if( Task_Run_Flag[2] > 0 )
					TaskAntennaRecordGotoDel();

				APP_OptLOG_Save(LOG_OPT_HAND_AIM_STAR_END_BY_OLED,&rxbuff[3],0);
			}
			if( ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode == 0x02)
			{
				if( Task_Run_Flag[0] > 0 )
					TaskSatelliteTrackDel();

				APP_OptLOG_Save(LOG_OPT_AUTO_AIM_STAR_END_BY_OLED,&rxbuff[3],0);
			}
			if( ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode == 0x04)
			{
				if( Task_Run_Flag[2] > 0 )
					TaskAntennaRecordGotoDel();

				APP_OptLOG_Save(LOG_OPT_ANTENNA_POSE_STOP_BY_OLED,&rxbuff[3],0);
			}
			if(  ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode == 0x03 )
			{
				if( Task_Run_Flag[1] > 0 )
					TaskSatGradientTrackDel();

				APP_OptLOG_Save(LOG_OPT_FOLLOW_STAR_END_BY_OLED,&rxbuff[3],0);
			}	

			// 安全考虑，停止所有电机的动作
			if( Antenna_AllDriveEnd() == FAIL)
				Antenna_AllDriveEnd();
			//BSP_Delay_s(1);

			ACUDevInfo.ACUCurrSta.dev_work_sta = 0x00;
			
		}	
		
		ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode = SeekSat_Mode;
	}

	
	return SUCC;
}

/***********************************************************
**name:	APP_OLEDCOMM_AntennaRecord
**describe: 天线位置记录
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OLEDCOMM_AntennaRecord(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id,i;
	INT8U index = 0;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];
	INT8U del_sat_id = 0;
	INT8U AntennaRecordBuff[80]={0};
	INT8U AntennaRecord_id=0;
	INT32U AntennaRecord_num = 0;
	INT32U dbm_int32u = 0,degree_int32u;
	INT32S degree_int32s;
	
	cmd_id = (rxbuff[0]<<8)+rxbuff[1];
	

	if( (cmd_id != AntennaRecordID )|| (cmdlen != 0 ))
		return FAIL;

//	AntennaRecord_id = *pbuff++;

//	if( AntennaRecord_id == 0 )
	{
		if( ACUDevInfo.AntennaRecordList.record_Total_num >= ANTENNA_RECORD_MAX_NUM )
			return FAIL;
		else
		{
			for(i=0;i<ANTENNA_RECORD_MAX_NUM;i++)
			{
				if( ACUDevInfo.AntennaRecordList.record_ID[i] > 0 )
					continue;
				AntennaRecord_id = i+1;
				break;
			}
		}
	}
//	else if( AntennaRecord_id > ANTENNA_RECORD_MAX_NUM )
//		return FAIL;
		
	AntennaRecord_num = ACUDevInfo.AntennaRecordList.record_Total_num;
	
	// 获取天线姿态记录ID
	AntennaRecordBuff[index++] = AntennaRecord_id;

	//获取系统设备时间，以及存储当前的时间
	DATETIME_ReadFromRtcIC(&ACUDevInfo.SysDateTime);
	AntennaRecordBuff[index++] = (ACUDevInfo.SysDateTime.year & 0xFF00)>>8;
	AntennaRecordBuff[index++] = ACUDevInfo.SysDateTime.year & 0xFF;
	AntennaRecordBuff[index++] = ACUDevInfo.SysDateTime.month;
	AntennaRecordBuff[index++] = ACUDevInfo.SysDateTime.day;
	AntennaRecordBuff[index++] = ACUDevInfo.SysDateTime.hour;
	AntennaRecordBuff[index++] = ACUDevInfo.SysDateTime.min;
	AntennaRecordBuff[index++] = ACUDevInfo.SysDateTime.senc;

	BSP_Delay_ms(2);

	// 存储当前的卫星参数
	memcpy(&AntennaRecordBuff[index],ACUDevInfo.ACUParam.CurrSatParam.name,16);
	index += 16;
	AntennaRecordBuff[index++] = ACUDevInfo.ACUParam.CurrSatParam.lon_flag;

	degree_int32u = (INT32U)(ACUDevInfo.ACUParam.CurrSatParam.lon * 1000);
 	AntennaRecordBuff[index++] = (degree_int32u & 0xFF000000)>>24;
 	AntennaRecordBuff[index++] = (degree_int32u & 0x00FF0000)>>16;
 	AntennaRecordBuff[index++] = (degree_int32u & 0x0000FF00)>>8;
 	AntennaRecordBuff[index++] = (degree_int32u & 0x000000FF);

	AntennaRecordBuff[index++] = (ACUDevInfo.ACUParam.CurrSatParam.freq & 0xFF000000)>>24;
	AntennaRecordBuff[index++] = (ACUDevInfo.ACUParam.CurrSatParam.freq & 0x00FF0000)>>16;
	AntennaRecordBuff[index++] = (ACUDevInfo.ACUParam.CurrSatParam.freq & 0x0000FF00)>>8;
	AntennaRecordBuff[index++] = (ACUDevInfo.ACUParam.CurrSatParam.freq & 0x000000FF);

	AntennaRecordBuff[index++] = ACUDevInfo.ACUParam.CurrSatParam.polar_type;

	BSP_Delay_ms(2);

	// 存储当前的天线参数
	AntennaRecordBuff[index++] = ACUDevInfo.ACUParam.AntennaParam.type;
	degree_int32u = (INT32U)ACUDevInfo.ACUParam.AntennaParam.offset_angle;
 	AntennaRecordBuff[index++] = (degree_int32u & 0xFF000000)>>24;
 	AntennaRecordBuff[index++] = (degree_int32u & 0x00FF0000)>>16;
 	AntennaRecordBuff[index++] = (degree_int32u & 0x0000FF00)>>8;
 	AntennaRecordBuff[index++] = (degree_int32u & 0x000000FF);
	AntennaRecordBuff[index++] = ACUDevInfo.ACUParam.AntennaParam.caliber;
	AntennaRecordBuff[index++] = (ACUDevInfo.ACUParam.AntennaParam.lo_freq & 0xFF000000)>>24;
	AntennaRecordBuff[index++] = (ACUDevInfo.ACUParam.AntennaParam.lo_freq & 0x00FF0000)>>16;
	AntennaRecordBuff[index++] = (ACUDevInfo.ACUParam.AntennaParam.lo_freq & 0x0000FF00)>>8;
	AntennaRecordBuff[index++] = (ACUDevInfo.ACUParam.AntennaParam.lo_freq & 0x000000FF);
	AntennaRecordBuff[index++] = ACUDevInfo.ACUParam.AntennaParam.gain_range_flag;
	AntennaRecordBuff[index++] = ACUDevInfo.ACUParam.AntennaParam.elec_coder_num;

	BSP_Delay_ms(2);

	// 存储当前AGC电压值以及DBM值
	degree_int32u = (INT32U)ACUDevInfo.ACUCurrSta.beacon_rxpower_mv;
 	AntennaRecordBuff[index++] = (degree_int32u & 0xFF000000)>>24;
 	AntennaRecordBuff[index++] = (degree_int32u & 0x00FF0000)>>16;
 	AntennaRecordBuff[index++] = (degree_int32u & 0x0000FF00)>>8;
 	AntennaRecordBuff[index++] = (degree_int32u & 0x000000FF);

	dbm_int32u = (INT32U)(ACUDevInfo.ACUCurrSta.beacon_rxpower_db * 1000);
	AntennaRecordBuff[index++] = (dbm_int32u & 0xFF000000)>>24;
	AntennaRecordBuff[index++] = (dbm_int32u & 0x00FF0000)>>16;
	AntennaRecordBuff[index++] = (dbm_int32u & 0x0000FF00)>>8;
	AntennaRecordBuff[index++] = (dbm_int32u & 0x000000FF);

	BSP_Delay_ms(2);

	// 存储当前的天线位置信息
	degree_int32u = ACUDevInfo.ACUCurrSta.antenna_position * 1000;
	AntennaRecordBuff[index++] = (degree_int32u & 0xFF000000)>>24;
	AntennaRecordBuff[index++] = (degree_int32u & 0x00FF0000)>>16;
	AntennaRecordBuff[index++] = (degree_int32u & 0x0000FF00)>>8;
	AntennaRecordBuff[index++] = (degree_int32u & 0x000000FF);
	
	degree_int32u = ACUDevInfo.ACUCurrSta.antenna_cabrage * 1000;
	AntennaRecordBuff[index++] = (degree_int32u & 0xFF000000)>>24;
	AntennaRecordBuff[index++] = (degree_int32u & 0x00FF0000)>>16;
	AntennaRecordBuff[index++] = (degree_int32u & 0x0000FF00)>>8;
	AntennaRecordBuff[index++] = (degree_int32u & 0x000000FF);

	BSP_Delay_ms(2);

	if( ACUDevInfo.ACUParam.AntennaParam.elec_coder_num >= 3 )
	{
		degree_int32s = ACUDevInfo.ACUCurrSta.antenna_polar * 1000;
		AntennaRecordBuff[index++] = (degree_int32s & 0xFF000000)>>24;
		AntennaRecordBuff[index++] = (degree_int32s & 0x00FF0000)>>16;
		AntennaRecordBuff[index++] = (degree_int32s & 0x0000FF00)>>8;
		AntennaRecordBuff[index++] = (degree_int32s & 0x000000FF);
	}
	else
	{
		AntennaRecordBuff[index++] = 0x00;
		AntennaRecordBuff[index++] = 0x00;
		AntennaRecordBuff[index++] = 0x00;
		AntennaRecordBuff[index++] = 0x00;
	}

	

	if( APP_PARAM_AntennaRecord_Save(AntennaRecord_id,AntennaRecordBuff,index) == FAIL)
		return FAIL;

	ACUDevInfo.AntennaRecordList.record_ID[AntennaRecord_id-1] = AntennaRecord_id;
	AntennaRecord_num++;
	if( AntennaRecord_num > ANTENNA_RECORD_MAX_NUM )
		AntennaRecord_num = ANTENNA_RECORD_MAX_NUM;
	if( AntennaRecord_num != ACUDevInfo.AntennaRecordList.record_Total_num )
	{
		APP_PARAM_AntennaRecord_TotalNum_Save(AntennaRecord_num);
		ACUDevInfo.AntennaRecordList.record_Total_num = AntennaRecord_num;
	}

	// 存储操作日志到flash中去
	{
		APP_OptLOG_Save_AntennaPoseOpt(	LOG_OPT_ANTENNA_POSE_RECORD_BY_OLED,
											ACUDevInfo.ACUParam.CurrSatParam,
											ACUDevInfo.ACUParam.AntennaParam,
											ACUDevInfo.ACUCurrSta.antenna_position,
											ACUDevInfo.ACUCurrSta.antenna_cabrage,								
											ACUDevInfo.ACUCurrSta.antenna_polar);
	}
	
	return SUCC;
}



/***********************************************************
**name:	APP_OLEDCOMM_AntennaRecordGoto
**describe: 调用天线位置记录
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OLEDCOMM_AntennaRecordGoto(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id,i;
	INT8U index = 0,goto_flag=0;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];
	INT8U del_sat_id = 0;
	INT8U AntennaRecordBuff[100]={0};
	INT8U AntennaRecord_id=0;
	INT32U beacon_freq;

	strSatParam 			RecordSatParam;
	strSysDateTime		RecordSysDateTime;
	strAntennaParam		RecordAntennaParam;
	INT32U agc_mv,agc_dbm;

	// 安全考虑，停止所有电机的动作

	if( ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode != 0x00)
	{
		if((Task_Run_Flag[2] > 0 ) || (Task_Run_Flag[1] > 0) ||(Task_Run_Flag[0] > 0))
		{
			if(Task_Run_Flag[2] > 0 ) 
				TaskAntennaRecordGotoDel();
			else if(Task_Run_Flag[1] > 0 ) 
				TaskSatGradientTrackDel();
			else if(Task_Run_Flag[0] > 0 ) 
				TaskSatelliteTrackDel();
		}
		ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode = 0x00;
		if( Antenna_AllDriveEnd() == FAIL)
			Antenna_AllDriveEnd();
		BSP_Delay_ms(200);
	}
	
	cmd_id = (rxbuff[0]<<8)+rxbuff[1];
	

	if( cmd_id != AntennaRecordGotoID )
		return FAIL;

	AntennaRecord_id = *pbuff++;

	goto_flag = 0;
	if( (AntennaRecord_id > ANTENNA_RECORD_MAX_NUM ) ||( AntennaRecord_id  == 0))
		return FAIL;
	else 
	{
		for(i=0;i<ANTENNA_RECORD_MAX_NUM;i++)
		{
			if( AntennaRecord_id == ACUDevInfo.AntennaRecordList.record_ID[i] )
			{
				goto_flag = 1;
				break;
			}
		}
	}

	if( goto_flag == 0 )
		return FAIL;

	if( APP_PARAM_AntennaRecord_Read(AntennaRecord_id,AntennaRecordBuff) == FAIL)
		return FAIL;

	// 天线姿态记录ID 比较
	if( AntennaRecord_id != AntennaRecordBuff[index++] )
		return FAIL;

	//存储时间
	RecordSysDateTime.year = (AntennaRecordBuff[index]<<8)+AntennaRecordBuff[index+1];
	index += 2;
	RecordSysDateTime.month = AntennaRecordBuff[index++];
	RecordSysDateTime.day= AntennaRecordBuff[index++];
	RecordSysDateTime.hour= AntennaRecordBuff[index++];
	RecordSysDateTime.min= AntennaRecordBuff[index++];
	RecordSysDateTime.senc= AntennaRecordBuff[index++];

	BSP_Delay_ms(2);


	//取出 存储卫星参数
	memcpy(RecordSatParam.name,&AntennaRecordBuff[index],16);
	index += 16;
	RecordSatParam.lon_flag = AntennaRecordBuff[index++];
	RecordSatParam.lon =(FP32)( (AntennaRecordBuff[index]<<24)+ (AntennaRecordBuff[index+1]<<16)+(AntennaRecordBuff[index+2]<<8)+AntennaRecordBuff[index+3]) /(FP32)1000;
	index += 4;

	RecordSatParam.freq = (AntennaRecordBuff[index]<<24)+ (AntennaRecordBuff[index+1]<<16)+(AntennaRecordBuff[index+2]<<8)+AntennaRecordBuff[index+3];
	index += 4;

	RecordSatParam.polar_type = AntennaRecordBuff[index++];

	BSP_Delay_ms(2);


	//取出 存储当前的天线参数
	RecordAntennaParam.type = AntennaRecordBuff[index++];
	RecordAntennaParam.offset_angle = (AntennaRecordBuff[index]<<24)+ (AntennaRecordBuff[index+1]<<16)+(AntennaRecordBuff[index+2]<<8)+AntennaRecordBuff[index+3];
	index += 4;
	RecordAntennaParam.caliber = AntennaRecordBuff[index++];
	RecordAntennaParam.lo_freq = (AntennaRecordBuff[index]<<24)+ (AntennaRecordBuff[index+1]<<16)+(AntennaRecordBuff[index+2]<<8)+AntennaRecordBuff[index+3];
	index += 4;
	RecordAntennaParam.gain_range_flag = AntennaRecordBuff[index++];
	RecordAntennaParam.elec_coder_num = AntennaRecordBuff[index++];

	BSP_Delay_ms(2);

	//取出存储的最优值得电压值和dbm值
	agc_mv = (AntennaRecordBuff[index]<<24)+ (AntennaRecordBuff[index+1]<<16)+(AntennaRecordBuff[index+2]<<8)+AntennaRecordBuff[index+3];
	index += 4;

	
	
	agc_dbm = (AntennaRecordBuff[index]<<24)+ (AntennaRecordBuff[index+1]<<16)+(AntennaRecordBuff[index+2]<<8)+AntennaRecordBuff[index+3];
	index += 4;
	
	// 取出存储天线位置信息
	// 取出存储天线位置信息
	RecordAntennaParam.aim_position = (FP32)((AntennaRecordBuff[index]<<24)+ (AntennaRecordBuff[index+1]<<16)+(AntennaRecordBuff[index+2]<<8)+AntennaRecordBuff[index+3]) / (FP32)1000.0;
	if( RecordAntennaParam.aim_position <= 0.1 )
	{
		BSP_Delay_ms(2);
		RecordAntennaParam.aim_position = (FP32)((AntennaRecordBuff[index]<<24)+ (AntennaRecordBuff[index+1]<<16)+(AntennaRecordBuff[index+2]<<8)+AntennaRecordBuff[index+3]) / (FP32)1000.0;
	}
	index += 4;
	RecordAntennaParam.aim_cabrage =  (FP32)((AntennaRecordBuff[index]<<24)+ (AntennaRecordBuff[index+1]<<16)+(AntennaRecordBuff[index+2]<<8)+AntennaRecordBuff[index+3]) /  (FP32)1000.0;
	if( RecordAntennaParam.aim_cabrage <= 0.1 )
	{
		BSP_Delay_ms(2);
		RecordAntennaParam.aim_cabrage =  (FP32)((AntennaRecordBuff[index]<<24)+ (AntennaRecordBuff[index+1]<<16)+(AntennaRecordBuff[index+2]<<8)+AntennaRecordBuff[index+3]) /  (FP32)1000.0;
	}
	index += 4;
	RecordAntennaParam.aim_polar =  (FP32)((AntennaRecordBuff[index]<<24)+ (AntennaRecordBuff[index+1]<<16)+(AntennaRecordBuff[index+2]<<8)+AntennaRecordBuff[index+3]) / (FP32)1000.0;
	if( RecordAntennaParam.aim_polar <= 0.1 )
	{
		BSP_Delay_ms(2);
		RecordAntennaParam.aim_polar =  (FP32)((AntennaRecordBuff[index]<<24)+ (AntennaRecordBuff[index+1]<<16)+(AntennaRecordBuff[index+2]<<8)+AntennaRecordBuff[index+3]) / (FP32)1000.0;
	}
	index += 4;

	BSP_Delay_ms(2);

	// 天线参数的比较
	//if(	(RecordAntennaParam.type != ACUDevInfo.ACUParam.AntennaParam.type ) ||
	//	(RecordAntennaParam.offset_angle != ACUDevInfo.ACUParam.AntennaParam.offset_angle ) ||
	//	(RecordAntennaParam.caliber != ACUDevInfo.ACUParam.AntennaParam.caliber ) ||
	//	(RecordAntennaParam.gain_range_flag != ACUDevInfo.ACUParam.AntennaParam.gain_range_flag ) )
	//	return FAIL;
	// 天线参数的比较
	if(	(RecordAntennaParam.type != ACUDevInfo.ACUParam.AntennaParam.type ) ||
		(RecordAntennaParam.offset_angle != ACUDevInfo.ACUParam.AntennaParam.offset_angle ) ||
		(RecordAntennaParam.caliber != ACUDevInfo.ACUParam.AntennaParam.caliber )  )
		return FAIL;
	

	// 卫星参数比较,怎么处理还需要仔细斟酌下
	ACUDevInfo.ACUParam.CurrSatParam.id = 0x00;
	memcpy(ACUDevInfo.ACUParam.CurrSatParam.name,RecordSatParam.name,16);
	ACUDevInfo.ACUParam.CurrSatParam.lon_flag = RecordSatParam.lon_flag;
	ACUDevInfo.ACUParam.CurrSatParam.lon = RecordSatParam.lon;
	ACUDevInfo.ACUParam.CurrSatParam.freq = RecordSatParam.freq;
	ACUDevInfo.ACUParam.CurrSatParam.polar_type = RecordSatParam.polar_type;

	BSP_Delay_ms(2);

	beacon_freq = RecordSatParam.freq > ACUDevInfo.ACUParam.AntennaParam.lo_freq?
				(RecordSatParam.freq -  ACUDevInfo.ACUParam.AntennaParam.lo_freq):
				( ACUDevInfo.ACUParam.AntennaParam.lo_freq - RecordSatParam.freq);

	if( APP_BEACON_SetFreq(beacon_freq)==FAIL)
 		return FAIL;
	// 根据以往经验，设置信标机频率，需要至少200毫秒才能设置成功
	//因此此处应该加200毫秒的延时
	BSP_Delay_ms(200);

	
	ACUDevInfo.ACUCurrSta.history_max_mv = agc_mv;
	ACUDevInfo.ACUCurrSta.AGC_max_mv = agc_mv;
	APP_PARAM_HistoryMaxMv_Save(&ACUDevInfo.ACUCurrSta.history_max_mv);

	//
	ACUDevInfo.ACUParam.AntennaParam.aim_cabrage =RecordAntennaParam.aim_cabrage;
	ACUDevInfo.ACUParam.AntennaParam.aim_position = RecordAntennaParam.aim_position;
	ACUDevInfo.ACUParam.AntennaParam.aim_polar = RecordAntennaParam.aim_polar;

	//启动前往姿态记录位置
	ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode = 0x04;   //  0x04模式在位置到达后，会自动取消，转为空闲模式
	
	BSP_Delay_ms(2);
	
	// 存储操作日志到flash中去
	{
		APP_OptLOG_Save_AntennaPoseOpt(	LOG_OPT_ANTENNA_POSE_APP_BY_OLED,
											ACUDevInfo.ACUParam.CurrSatParam,
											ACUDevInfo.ACUParam.AntennaParam,
											ACUDevInfo.ACUParam.AntennaParam.aim_position,	
											ACUDevInfo.ACUParam.AntennaParam.aim_cabrage,							
											ACUDevInfo.ACUParam.AntennaParam.aim_polar);
	}

	APP_PARAM_CurrSatParam_Save(&ACUDevInfo.ACUParam.CurrSatParam);
	ACUDevInfo.ACUCurrSta.dev_param_reflash_flag[0] = 0x01;
	ACUDevInfo.ACUCurrSta.dev_param_reflash_flag[1] = 0x01;
	

	ACUDevInfo.ACUCurrSta.dev_work_sta = 0x40;
	// 本初启动前往姿态记录位置任务
	TaskAntennaRecordGotoCreate();
	
	return SUCC;
}








/***********************************************************
**name:	APP_OLEDCOMM_DelAntennaRecord
**describe:  删除天线记录
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OLEDCOMM_DelAntennaRecord(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id,i;
	INT8U index = 0;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];
	INT8U del_id = 0;
	INT8U savebuff[64]={0};
	INT8U readbuff[100]={0};

	static strSatParam 			SatParam;
	static strAntennaParam AntennaParam;
	static FP32 degree_AZ,degree_EL,degree_PL;

	cmd_id = (rxbuff[0]<<8)+rxbuff[1];
	

	if( cmd_id != AntennaRecordDelID )
		return FAIL;

	

	// 查找发送过来的记录ID是否在已有的参数链表中
	

	while(cmdlen)
	{
		del_id =  pbuff[index++];
		for( i =0;i<ANTENNA_RECORD_MAX_NUM;i++)
		{
			if(ACUDevInfo.AntennaRecordList.record_ID[i] == del_id )
			{
				ACUDevInfo.AntennaRecordList.record_ID[i] = 0x00;
				if( ACUDevInfo.AntennaRecordList.record_Total_num > 0)
					ACUDevInfo.AntennaRecordList.record_Total_num--;

				APP_PARAM_AntennaRecord_Read2Struct(del_id,&SatParam, &AntennaParam, &degree_AZ,&degree_EL,&degree_PL);

				BSP_Delay_ms(2);

				APP_OptLOG_Save_AntennaPoseOpt(LOG_OPT_ANTENNA_POSE_DEL_BY_OLED,SatParam,AntennaParam,degree_AZ,degree_EL,degree_PL);
				//在对应的EEPROM地址中 删除
				APP_PARAM_AntennaRecord_Save(del_id,savebuff,64);

				memset(&SatParam,0x00,sizeof(SatParam));
				memset(&AntennaParam,0x00,sizeof(AntennaParam));
				degree_AZ = 0.0;
				degree_EL = 0.0;
				degree_PL = 0.0;

				BSP_Delay_ms(3);
				break;
			}
		}
		cmdlen--;
	}

	
	// 存储操作日志到flash中去
	{
		//APP_OptLOG_Save(LOG_OPT_ANTENNA_POSE_DEL_BY_PC,&rxbuff[3],0);
		//APP_OptLOG_Save_AntennaPoseOpt(LOG_OPT_ANTENNA_POSE_DEL_BY_PC,strSatParam 	SatParam,strAntennaParam AntennaParam,FP32 degree_AZ,FP32 degree_EL,FP32 degree_PL)
	}

	return SUCC;
}



/***********************************************************
**name:	APP_OLEDCOMM_AGCRecord
**describe: AGC 记录
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OLEDCOMM_AGCRecord(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id,i;
	INT8U index = 0;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];
	INT8U del_sat_id = 0;
	INT8U AntennaRecordBuff[64]={0};
	INT8U AntennaRecord_id=0;
	INT32U AntennaRecord_num = 0;
	INT32U dbm_int32u = 0,degree_int32u;
	INT32S degree_int32s;
	
	cmd_id = (rxbuff[0]<<8)+rxbuff[1];
	

	if( (cmd_id != AGCRecordID )|| (cmdlen != 0 ))
		return FAIL;

	// 存储当前AGC电压值
	if(APP_PARAM_HistoryMaxMv_Save(&ACUDevInfo.ACUCurrSta.beacon_rxpower_mv) == FAIL)
		return FAIL;

	return SUCC;
}


/***********************************************************
**name:	APP_OLEDCOMM_AntennaSigleStepCtr
**describe: 天线手动单步控制
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OLEDCOMM_AntennaSigleStepCtr(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id;
	INT8U index = 0;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];
	INT8U motor_id = 0,dir=0;
	
	int sta = FAIL;

	if( ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode != 0x01)
		return FAIL;

	
	cmd_id = (rxbuff[0]<<8)+rxbuff[1];
	

	if( (cmd_id != AntennaSigleStepCtrID )|| (cmdlen != 2 ))
		return FAIL;

	motor_id = pbuff[index++];
	dir = pbuff[index++];

	if( dir == 0x01)
		dir = ANTENNA_DIR_RIGHT_UP_CLOCK;
	else if( dir == 0x02 )
		dir = ANTENNA_DIR_LEFT_DOWN_ANTICLOCK;
	else
		return FAIL;

	switch(motor_id)
	{
		case motor_position_id:		
		case motor_cabrage_id:
		case motor_polar_id:
			ACUDevInfo.ACUCurrSta.dev_work_sta = 0x10;
			if( (dir == 0x01) ||(dir == 0x02))
				sta = Motor_TimeConfigDrive(motor_id,dir,250);	
			ACUDevInfo.ACUCurrSta.dev_work_sta = 0x11;
			break;
		default:
		 	return FAIL;
	}

	// 日志记录
	{
		;
	}
	
	return sta;
}




/***********************************************************
**name:	APP_OLEDCOMM_AntennaTurnCtr
**describe: 天线手动转动控制
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OLEDCOMM_AntennaTurnCtr(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id;
	INT8U index = 0;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];
	INT8U motor_id = 0,dir=0;
	

	if( ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode != 0x01)
		return FAIL;

	cmd_id = (rxbuff[0]<<8)+rxbuff[1];
	

	if( cmd_id != AntennaDegreeCtrID )
		return FAIL;

	if(	cmdlen  == 0 )
	{
		if(Task_Run_Flag[2] > 0 )
			TaskAntennaRecordGotoDel();
		return SUCC;
	}

	if( cmdlen != 12 )
		return FAIL;

	ACUDevInfo.ACUParam.AntennaParam.aim_position_hand =  (FP32)((pbuff[index]<<24) +  (pbuff[index+1]<<16) + (pbuff[index+2]<<8) +pbuff[index+3] )/1000.0;
	index += 4;
	ACUDevInfo.ACUParam.AntennaParam.aim_cabrage_hand =  (FP32)((pbuff[index]<<24) +  (pbuff[index+1]<<16) + (pbuff[index+2]<<8) +pbuff[index+3] )/1000.0;
	index += 4;
	ACUDevInfo.ACUParam.AntennaParam.aim_polar_hand =  (FP32)((pbuff[index]<<24) +  (pbuff[index+1]<<16) + (pbuff[index+2]<<8) +pbuff[index+3] )/1000.0;
	index += 4;

	BSP_Delay_ms(2);
	
	if(	( ACUDevInfo.ACUParam.AntennaParam.aim_position_hand <= ACUDevInfo.ACUParam.AntennaParam.motor_position_start ) ||
		( ACUDevInfo.ACUParam.AntennaParam.aim_position_hand >=  ACUDevInfo.ACUParam.AntennaParam.motor_position_end))
		return FAIL;

	if(	( ACUDevInfo.ACUParam.AntennaParam.aim_cabrage_hand <= ACUDevInfo.ACUParam.AntennaParam.motor_cabrage_start ) ||
		( ACUDevInfo.ACUParam.AntennaParam.aim_cabrage_hand >=  ACUDevInfo.ACUParam.AntennaParam.motor_cabrage_end))
		return FAIL;

	if(	( ACUDevInfo.ACUParam.AntennaParam.aim_polar_hand <= ACUDevInfo.ACUParam.AntennaParam.motor_polar_start ) ||
		( ACUDevInfo.ACUParam.AntennaParam.aim_polar_hand >=  ACUDevInfo.ACUParam.AntennaParam.motor_polar_end))
		return FAIL;

	TaskAntennaRecordGotoCreate();
		
	
	return SUCC;
}





/***********************************************************
**name:	APP_OLEDCOMM_SelSat
**describe: 选择卫星
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OLEDCOMM_SelSat(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id;
	INT8U index = 0,i;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];
	INT8U sel_sat_id = 0;

	INT32U beacon_freq;

	strSatParam 			SelSatParam;

	cmd_id = (rxbuff[0]<<8)+rxbuff[1];
	

	if( (cmd_id != SatSelID )|| (cmdlen != 1 ))
		return FAIL;

	sel_sat_id = pbuff[index++];

	// 判断是否该卫星即是已经选中的卫星
	//if( sel_sat_id != ACUDevInfo.ACUParam.CurrSatParam.id )
	{

		// 确定该卫星是设备当前已经存储的卫星
		if( sel_sat_id != ACUDevInfo.SATList.Sat_ID[sel_sat_id-1] )
			return FAIL;

		APP_PARAM_SatCurrID_Save(sel_sat_id);
		
		// 从EEPROM中找出相应的卫星参数信息
		SelSatParam.id = sel_sat_id;
		APP_PARAM_SatParam_Read(&SelSatParam);

		BSP_Delay_ms(2);

		beacon_freq = SelSatParam.freq > ACUDevInfo.ACUParam.AntennaParam.lo_freq?
				(SelSatParam.freq -  ACUDevInfo.ACUParam.AntennaParam.lo_freq):
				( ACUDevInfo.ACUParam.AntennaParam.lo_freq - SelSatParam.freq);

		//此处设置信标机信标频率
		{
 			if( APP_BEACON_SetFreq(beacon_freq)==FAIL)
 				return FAIL;

			// 根据以往经验，设置信标机频率，需要至少200毫秒才能设置成功
			//因此此处应该加200毫秒的延时
			//BSP_Delay_ms(200);
		}

		memcpy(&ACUDevInfo.ACUParam.CurrSatParam,&SelSatParam,sizeof(SelSatParam));
		
		// 结合当前的本地经纬度信息，计算对星理论角
		// 并且重新计算理论倾角
		APP_MONITOR_AimDegree_Get();

		// 清除历史最优值
		APP_PARAM_HistoryMaxMv_Save(0);

		APP_PARAM_CurrSatParam_Save(&SelSatParam);

		ACUDevInfo.ACUCurrSta.dev_param_reflash_flag[0] = 0x01;
		ACUDevInfo.ACUCurrSta.dev_param_reflash_flag[1] = 0x01;

		BSP_Delay_ms(2);
	}

	// 存储操作日志到flash中去
	{
		APP_OptLOG_Save_SatOpt(LOG_OPT_SEL_STAR_BY_OLED,ACUDevInfo.ACUParam.CurrSatParam);
	}
	
	return SUCC;
}

#if 0
/***********************************************************
**name:	APP_OLEDCOMM_MotorSoftLimit
**describe: 电机软限位设置
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OLEDCOMM_MotorSoftLimit(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id;
	INT8U index = 0,i;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];
	INT8U sel_motor_id = 0;
	INT32U	start_degree,end_degree;


	cmd_id = (rxbuff[0]<<8)+rxbuff[1];
	

	if( (cmd_id != MotorSoftLimitSetID )|| (cmdlen != 9 ))
		return FAIL;

	sel_motor_id = pbuff[index++];
	start_degree =  (pbuff[index]<<24) +  (pbuff[index+1]<<16) + (pbuff[index+2]<<8) +pbuff[index+3];
	index += 4;
	end_degree =  (pbuff[index]<<24) +  (pbuff[index+1]<<16) + (pbuff[index+2]<<8) +pbuff[index+3];
	index += 4;
	
	switch( sel_motor_id )
	{
		case motor_position_id:
			ACUDevInfo.ACUParam.AntennaParam.motor_position_start = (FP32)start_degree / 1000;
			ACUDevInfo.ACUParam.AntennaParam.motor_position_end = (FP32)end_degree / 1000;
			APP_PARAM_PositionSoftLimit_Save(&ACUDevInfo.ACUParam.AntennaParam);
			break;
		case motor_cabrage_id:
			ACUDevInfo.ACUParam.AntennaParam.motor_cabrage_start = (FP32)start_degree / 1000;
			ACUDevInfo.ACUParam.AntennaParam.motor_cabrage_end = (FP32)end_degree / 1000;
			APP_PARAM_CabrageSoftLimit_Save(&ACUDevInfo.ACUParam.AntennaParam);
			break;
		case motor_polar_id:
			ACUDevInfo.ACUParam.AntennaParam.motor_polar_start = (FP32)start_degree / 1000;
			ACUDevInfo.ACUParam.AntennaParam.motor_polar_end = (FP32)end_degree / 1000;
			APP_PARAM_PolarSoftLimit_Save(&ACUDevInfo.ACUParam.AntennaParam);
			break;
		default:
			return FAIL;
	}

	

	// 存储操作日志到flash中去
	{
		;
	}
	
	return SUCC;
}
#endif


/***********************************************************
**name:	APP_OLEDCOMM_NetWorkSet
**describe: 设备网络设置
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OLEDCOMM_NetWorkSet(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id;
	INT8U index = 0,i;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];
	INT32U ip,mask,gate;

	

	cmd_id = (rxbuff[0]<<8)+rxbuff[1];
	

	if( cmd_id != NetWorkSetID )
		return FAIL;

	ip =  pbuff[index] +  (pbuff[index+1]<<8) + (pbuff[index+2]<<16) +(pbuff[index+3]<<24);
	index += 4;
	mask =  pbuff[index] +  (pbuff[index+1]<<8) + (pbuff[index+2]<<16) +(pbuff[index+3]<<24);
	if( (mask & 0xFF000000) == 0xFF000000)
		return FAIL;
	index += 4;
	gate =  pbuff[index] +  (pbuff[index+1]<<8) + (pbuff[index+2]<<16) +(pbuff[index+3]<<24);
//	if( (ip & 0x00FFFFFF) != (gate & 0x00FFFFFF))
//		return FAIL;

	if(	( ip != ACUDevInfo.DeviceInfo.NetWork.ip_addr )  || 
		( mask != ACUDevInfo.DeviceInfo.NetWork.mask ) ||
		( gate != ACUDevInfo.DeviceInfo.NetWork.gate ) )
	{
		if( ip != ACUDevInfo.DeviceInfo.NetWork.ip_addr ) 
		{
			ACUDevInfo.DeviceInfo.NetWork.ip_addr = ip;
			APP_OptLOG_Save(LOG_OPT_NETWORK_IP_ADDR_SET_BY_OLED,&rxbuff[3],4);
		}
		if( mask != ACUDevInfo.DeviceInfo.NetWork.mask	)
		{
			ACUDevInfo.DeviceInfo.NetWork.mask = mask;
			APP_OptLOG_Save(LOG_OPT_NETWORK_MASK_SET_BY_OLED,&rxbuff[7],4);
		}
		if( gate != ACUDevInfo.DeviceInfo.NetWork.gate )
		{
			ACUDevInfo.DeviceInfo.NetWork.gate = gate;
			APP_OptLOG_Save(LOG_OPT_NETWORK_GATE_SET_BY_OLED,&rxbuff[11],4);
		}

		// 将数据存入EEPROM中去
		APP_PARAM_NetWork_Save(&ACUDevInfo.DeviceInfo.NetWork);

		// 调用重新修改IP的接口函数
		LWIP_NetworkConfigChange();
	

	}
	
	return SUCC;
}


/***********************************************************
**name:	APP_OLEDCOMM_AskForPCAdmin
**describe: PC机权限向OLED申请
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void APP_OLEDCOMM_AskForPCAdmin(void)
{
	INT8U txbuff[32],index=0;
	INT8U CheckSum8 = 0;

	txbuff[index++] = 0xBC;
	txbuff[index++] = 0x03;
	txbuff[index++] = 0x02;
	txbuff[index++] = 0x1A;
	txbuff[index++] = 0x00;

	CheckSum8 = TOOL_CheckSum8BitGet(txbuff,index);
	txbuff[index++] = CheckSum8;

	txbuff[index++] = 0xCB;

	SWIFT_UART3_SendBuff(OLEDCOMMTxBuff,index);
}

/***********************************************************
**name:	APP_OLEDCOMM_PCAdminSet
**describe: PC机权限设置
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OLEDCOMM_PCAdminSet(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id;
	INT8U index = 0,i;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];
	INT8U admin_switch = 0;
	INT8U curr_admin_sta;

	cmd_id = (rxbuff[0]<<8)+rxbuff[1];
	

	if( (cmd_id != PCHighAdminID )|| (cmdlen != 1 ))
		return FAIL;

	admin_switch = pbuff[index++];

	if( admin_switch > 0 )
	{
		ACUDevInfo.ACUCurrSta.DevAdmin.admin_sta = 0x01;
		ACUDevInfo.ACUCurrSta.dev_admin_sta = 0x01;
		ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_ip_addr = ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_ask_ip_addr;
		ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_ask_ip_addr = 0;

		APP_OptLOG_SaveCode(LOG_OPT_OLED_PERMIN_PC_ADMIN);
	}
	else
	{
		

		if( ACUDevInfo.ACUCurrSta.dev_admin_sta == 0x11)
			APP_OptLOG_SaveCode(LOG_OPT_OLED_REFUSE_PC_ADMIN);
		else if(  ACUDevInfo.ACUCurrSta.dev_admin_sta == 0x01 )
			APP_OptLOG_SaveCode(LOG_OPT_OLED_CANCEL_PC_ADMIN);

		ACUDevInfo.ACUCurrSta.DevAdmin.admin_sta = 0x00;
		ACUDevInfo.ACUCurrSta.dev_admin_sta = 0x00;
		ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_ip_addr = 0x00;
		ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_ask_ip_addr = 0x00;
	}

	txbuff[index++] = 0x00;
	*txbufflen = index;

	txbuff[0] = ((cmd_id | 0x8000) & 0xFF00)>>8;
	txbuff[1] = ((cmd_id | 0x8000) & 0x00FF);
	txbuff[2] = *txbufflen - 3;
		
	return SUCC;
}


/***********************************************************
**name:	APP_OLEDCOMM_CurrSatParamQue
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OLEDCOMM_CurrSatParamQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id;
	INT8U index = 3;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];
	INT32U degree_int32u;

	cmd_id = (rxbuff[0]<<8)+rxbuff[1];
	if( cmd_id != CurrSatParamQueID )
		return FAIL;

	txbuff[index++] = ACUDevInfo.ACUParam.CurrSatParam.id;

	memcpy(&txbuff[index],ACUDevInfo.ACUParam.CurrSatParam.name,16);

	index += 16;

	
	txbuff[index++] = ACUDevInfo.ACUParam.CurrSatParam.lon_flag;

	degree_int32u = (INT32U)(ACUDevInfo.ACUParam.CurrSatParam.lon*1000) ;
	txbuff[index++] = (degree_int32u & 0xFF000000)>>24;
	txbuff[index++] = (degree_int32u & 0x00FF0000)>>16;
	txbuff[index++] = (degree_int32u & 0x0000FF00)>>8;
	txbuff[index++] = (degree_int32u & 0x000000FF);

	txbuff[index++] = (ACUDevInfo.ACUParam.CurrSatParam.freq & 0xFF000000)>>24;
	txbuff[index++] = (ACUDevInfo.ACUParam.CurrSatParam.freq & 0x00FF0000)>>16;
	txbuff[index++] = (ACUDevInfo.ACUParam.CurrSatParam.freq & 0x0000FF00)>>8;
	txbuff[index++] = (ACUDevInfo.ACUParam.CurrSatParam.freq & 0x000000FF);

	txbuff[index++] = ACUDevInfo.ACUParam.CurrSatParam.polar_type;

	*txbufflen = index;

	txbuff[0] = ((cmd_id | 0x8000) & 0xFF00)>>8;
	txbuff[1] = ((cmd_id | 0x8000) & 0x00FF);
	txbuff[2] = *txbufflen - 3;
	
	return SUCC;
}



/***********************************************************
**name:	APP_OLEDCOMM_SaveAllSatParamQue
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OLEDCOMM_SaveAllSatParamQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id,i;
	INT16U index = 0,index1=0;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];
	INT8U sat_num = 0,Sat_Total_num = 0;
	INT8U check_sum=0;

	INT32U degree_int32u;

	static INT8U sat_id_index = 0;
	static INT8U cycle_flag = 0;

	strSatParam  SatParam;

	cmd_id = (rxbuff[0]<<8)+rxbuff[1];
	if( cmd_id != AllSaveSatParamQueID )
		return FAIL;

//	if(cycle_flag == 0 ) 
//		sat_id_index = 0;

	// 检查当前卫星列表中是否存在该卫星
//	if( sat_id_index >= ACUDevInfo.SATList.Sat_Total_num)
//	{
//		cycle_flag = 0;
//		sat_id_index = 0;
//		return 2;
//	}

	for(i=0;i<SAT_MAX_NUM;i++)
	{
		SatParam.id = ACUDevInfo.SATList.Sat_ID[i];
		if( SatParam.id ==  0 )
			continue;

		// 从EEPROM中查找相关的卫星数据，赋值到SatParam 中去
		if( APP_PARAM_SatParam_Read(&SatParam) == FAIL)
			continue;

		OLEDCOMMTxBuff[index++] = 0xBC;
		OLEDCOMMTxBuff[index++] = 30;

		OLEDCOMMTxBuff[index++] = ((cmd_id | 0x8000) & 0xFF00)>>8;
		OLEDCOMMTxBuff[index++] = ((cmd_id | 0x8000) & 0x00FF);
		OLEDCOMMTxBuff[index++] = 27;

		OLEDCOMMTxBuff[index++] = SatParam.id;

		memcpy(&OLEDCOMMTxBuff[index],SatParam.name,16);
		index += 16;
		
		
		OLEDCOMMTxBuff[index++] = SatParam.lon_flag;

		degree_int32u = (INT32U)(SatParam.lon * 1000);
		OLEDCOMMTxBuff[index++] = (degree_int32u & 0xFF000000)>>24;
		OLEDCOMMTxBuff[index++] = (degree_int32u & 0x00FF0000)>>16;
		OLEDCOMMTxBuff[index++] = (degree_int32u & 0x0000FF00)>>8;
		OLEDCOMMTxBuff[index++] = (degree_int32u & 0x000000FF);

		OLEDCOMMTxBuff[index++] = (SatParam.freq & 0xFF000000)>>24;
		OLEDCOMMTxBuff[index++] = (SatParam.freq & 0x00FF0000)>>16;
		OLEDCOMMTxBuff[index++] = (SatParam.freq & 0x0000FF00)>>8;
		OLEDCOMMTxBuff[index++] = (SatParam.freq & 0x000000FF);

		OLEDCOMMTxBuff[index++] = SatParam.polar_type;

		check_sum = TOOL_CheckSum8BitGet(&OLEDCOMMTxBuff[0],index);
		OLEDCOMMTxBuff[index++] = check_sum;

		OLEDCOMMTxBuff[index++] = 0xCB;
	
		sat_num++;
		Sat_Total_num++;

		OLEDCOMMTxBuffLen = index;
		SWIFT_UART3_SendBuff(OLEDCOMMTxBuff,OLEDCOMMTxBuffLen);
		BSP_Delay_ms(50);
		index = 0;
		if( Sat_Total_num >= ACUDevInfo.SATList.Sat_Total_num )
			break;
	}

	index = 0;
	txbuff[index++] = ((cmd_id | 0x8000) & 0xFF00)>>8;
	txbuff[index++] = ((cmd_id | 0x8000) & 0x00FF);
	txbuff[index++] = 27;

	txbuff[index++] = 0xFF;

	memset(&txbuff[index],0xFF,16);
	index += 16;
		
		
	txbuff[index++] = 0xFF;

	degree_int32u = 0xFFFFFFFF;
	txbuff[index++] = (degree_int32u & 0xFF000000)>>24;
	txbuff[index++] = (degree_int32u & 0x00FF0000)>>16;
	txbuff[index++] = (degree_int32u & 0x0000FF00)>>8;
	txbuff[index++] = (degree_int32u & 0x000000FF);

	txbuff[index++] = 0xFF;
	txbuff[index++] = 0xFF;
	txbuff[index++] = 0xFF;
	txbuff[index++] = 0xFF;

	txbuff[index++] = 0xFF;

	*txbufflen = index;
	return SUCC;
}





/***********************************************************
**name:	APP_OLEDCOMM_AntennaParamQue
**describe:		//  天线信息查询
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OLEDCOMM_AntennaParamQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id;
	INT8U index = 3;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];

	cmd_id = (rxbuff[0]<<8)+rxbuff[1];

	if( cmd_id != AntennaParamQueID )
		return FAIL;
	
	txbuff[index++] = ACUDevInfo.ACUParam.AntennaParam.type;

	txbuff[index++] = (ACUDevInfo.ACUParam.AntennaParam.offset_angle & 0xFF000000)>>24;
	txbuff[index++] = (ACUDevInfo.ACUParam.AntennaParam.offset_angle & 0x00FF0000)>>16;
	txbuff[index++] = (ACUDevInfo.ACUParam.AntennaParam.offset_angle & 0x0000FF00)>>8;
	txbuff[index++] = (ACUDevInfo.ACUParam.AntennaParam.offset_angle & 0x000000FF);

	txbuff[index++] = ACUDevInfo.ACUParam.AntennaParam.caliber & 0xFF;
			
	txbuff[index++] = (ACUDevInfo.ACUParam.AntennaParam.lo_freq & 0xFF000000)>>24;
	txbuff[index++] = (ACUDevInfo.ACUParam.AntennaParam.lo_freq & 0x00FF0000)>>16;
	txbuff[index++] = (ACUDevInfo.ACUParam.AntennaParam.lo_freq & 0x0000FF00)>>8;
	txbuff[index++] = (ACUDevInfo.ACUParam.AntennaParam.lo_freq & 0x000000FF);

	txbuff[index++] = ACUDevInfo.ACUParam.AntennaParam.gain_range_flag;

	txbuff[index++] = ACUDevInfo.ACUParam.AntennaParam.elec_coder_num;

	*txbufflen = index;

	txbuff[0] = ((cmd_id | 0x8000) & 0xFF00)>>8;
	txbuff[1] = ((cmd_id | 0x8000) & 0x00FF);
	txbuff[2] = *txbufflen - 3;
	
	return SUCC;
}


/***********************************************************
**name:	APP_OLEDCOMM_AntennaAdjustParamQue
**describe:	 天线校准参数查询
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OLEDCOMM_AntennaAdjustParamQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id;
	INT8U index = 3;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];

	cmd_id = (rxbuff[0]<<8)+rxbuff[1];

	if( cmd_id != AntennaAdjustParamQueID )
		return FAIL;

	return SUCC;
}



/***********************************************************
**name:	APP_OLEDCOMM_LocalParamQue
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OLEDCOMM_LocalParamQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id;
	INT8U index = 3;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];
	INT32U degree_int32u;

	cmd_id = (rxbuff[0]<<8)+rxbuff[1];

	if( cmd_id != LocalParamQueID )
		return FAIL;
	
	txbuff[index++] = ACUDevInfo.ACUParam.LocalPositionParam.lon_flag;

	degree_int32u = (INT32U)(ACUDevInfo.ACUParam.LocalPositionParam.lon * 1000);
	txbuff[index++] = (degree_int32u & 0xFF000000)>>16;
	txbuff[index++] = (degree_int32u & 0x00FF0000)>>16;
	txbuff[index++] = (degree_int32u & 0x0000FF00)>>8;
	txbuff[index++] = (degree_int32u & 0x000000FF);

	txbuff[index++] = ACUDevInfo.ACUParam.LocalPositionParam.lat_flag;
	degree_int32u = (INT32U)(ACUDevInfo.ACUParam.LocalPositionParam.lat * 1000);
	txbuff[index++] = (degree_int32u & 0xFF000000)>>16;
	txbuff[index++] = (degree_int32u & 0x00FF0000)>>16;
	txbuff[index++] = (degree_int32u & 0x0000FF00)>>8;
	txbuff[index++] = (degree_int32u & 0x000000FF);

	txbuff[index++] = (ACUDevInfo.ACUParam.LocalPositionParam.height & 0xFF000000)>>24;
	txbuff[index++] = (ACUDevInfo.ACUParam.LocalPositionParam.height & 0x00FF0000)>>16;
	txbuff[index++] = (ACUDevInfo.ACUParam.LocalPositionParam.height & 0x0000FF00)>>8;
	txbuff[index++] = (ACUDevInfo.ACUParam.LocalPositionParam.height & 0x000000FF);

	txbuff[index++] = ACUDevInfo.ACUParam.LocalPositionParam.data_src_flag;

	*txbufflen = index;

	txbuff[0] = ((cmd_id | 0x8000) & 0xFF00)>>8;
	txbuff[1] = ((cmd_id | 0x8000) & 0x00FF);
	txbuff[2] = *txbufflen - 3;

	return SUCC;
}




/***********************************************************
**name:	APP_OLEDCOMM_DevSysDateTimeQue
**describe:   设备系统日期时间查询
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OLEDCOMM_DevSysDateTimeQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id;
	INT8U index = 3;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];

	cmd_id = (rxbuff[0]<<8)+rxbuff[1];

	if( cmd_id != DevSysDateTimeQueID )
		return FAIL;
	
	txbuff[index++] = (ACUDevInfo.SysDateTime.year & 0xFF00)>>8;
	txbuff[index++] = (ACUDevInfo.SysDateTime.year & 0x00FF);

	txbuff[index++] = ACUDevInfo.SysDateTime.month;

	txbuff[index++] = ACUDevInfo.SysDateTime.day;

	txbuff[index++] = ACUDevInfo.SysDateTime.hour;

	txbuff[index++] = ACUDevInfo.SysDateTime.min;

	txbuff[index++] = ACUDevInfo.SysDateTime.senc;

	txbuff[index++] = ACUDevInfo.SysDateTime.src_flag;
	

	*txbufflen = index;

	txbuff[0] = ((cmd_id | 0x8000) & 0xFF00)>>8;
	txbuff[1] = ((cmd_id | 0x8000) & 0x00FF);
	txbuff[2] = *txbufflen - 3;

	return SUCC;
}




/***********************************************************
**name:	APP_OLEDCOMM_NetWorkQue
**describe:   设备网络相关设置查询
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OLEDCOMM_NetWorkQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id;
	INT8U index = 3;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];

	cmd_id = (rxbuff[0]<<8)+rxbuff[1];

	if( cmd_id != NetWorkQueID )
		return FAIL;

	txbuff[index++] = (ACUDevInfo.DeviceInfo.NetWork.ip_addr & 0x000000FF);
	txbuff[index++] = (ACUDevInfo.DeviceInfo.NetWork.ip_addr & 0x0000FF00)>>8;
	txbuff[index++] = (ACUDevInfo.DeviceInfo.NetWork.ip_addr & 0x00FF0000)>>16;
	txbuff[index++] = (ACUDevInfo.DeviceInfo.NetWork.ip_addr & 0xFF000000)>>24;


	
	txbuff[index++] = (ACUDevInfo.DeviceInfo.NetWork.mask & 0x000000FF);
	txbuff[index++] = (ACUDevInfo.DeviceInfo.NetWork.mask & 0x0000FF00)>>8;
	txbuff[index++] = (ACUDevInfo.DeviceInfo.NetWork.mask & 0x00FF0000)>>16;
	txbuff[index++] = (ACUDevInfo.DeviceInfo.NetWork.mask & 0xFF000000)>>24;

	txbuff[index++] = (ACUDevInfo.DeviceInfo.NetWork.gate & 0x000000FF);
	txbuff[index++] = (ACUDevInfo.DeviceInfo.NetWork.gate & 0x0000FF00)>>8;
	txbuff[index++] = (ACUDevInfo.DeviceInfo.NetWork.gate & 0x00FF0000)>>16;
	txbuff[index++] = (ACUDevInfo.DeviceInfo.NetWork.gate & 0xFF000000)>>24;

	
	*txbufflen = index;

	txbuff[0] = ((cmd_id | 0x8000) & 0xFF00)>>8;
	txbuff[1] = ((cmd_id | 0x8000) & 0x00FF);
	txbuff[2] = *txbufflen - 3;

	return SUCC;
}



/***********************************************************
**name:	APP_OLEDCOMM_AimAtSatDegreeQue
**describe:  对星理论角查询
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OLEDCOMM_AimAtSatDegreeQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id;
	INT8U index = 3;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];

	INT32U degree_Int32u;
	INT32S degree_Int32s;

	cmd_id = (rxbuff[0]<<8)+rxbuff[1];

	if( cmd_id != AimAtSatTheoreticalDegreeQueID )
		return FAIL;
	
	degree_Int32u = 	(INT32U)(ACUDevInfo.ACUParam.AntennaParam.aim_position * 1000);
	txbuff[index++] = (degree_Int32u & 0xFF000000)>>24;
	txbuff[index++] = (degree_Int32u & 0x00FF0000)>>16;
	txbuff[index++] = (degree_Int32u & 0x0000FF00)>>8;
	txbuff[index++] = (degree_Int32u & 0x000000FF);

	degree_Int32u = 	(INT32U)(ACUDevInfo.ACUParam.AntennaParam.aim_cabrage * 1000);
	txbuff[index++] = (degree_Int32u & 0xFF000000)>>24;
	txbuff[index++] = (degree_Int32u & 0x00FF0000)>>16;
	txbuff[index++] = (degree_Int32u & 0x0000FF00)>>8;
	txbuff[index++] = (degree_Int32u & 0x000000FF);

	degree_Int32s = 	(INT32S)(ACUDevInfo.ACUParam.AntennaParam.aim_polar * 1000);
	txbuff[index++] = (degree_Int32s & 0xFF000000)>>24;
	txbuff[index++] = (degree_Int32s & 0x00FF0000)>>16;
	txbuff[index++] = (degree_Int32s & 0x0000FF00)>>8;
	txbuff[index++] = (degree_Int32s & 0x000000FF);
	
	*txbufflen = index;
	
	txbuff[0] = ((cmd_id | 0x8000) & 0xFF00)>>8;
	txbuff[1] = ((cmd_id | 0x8000) & 0x00FF);
	txbuff[2] = *txbufflen - 3;
	
	return SUCC;
}




/***********************************************************
**name:	APP_OLEDCOMM_AntennaRecordQue
**describe:  天线位置记录查询
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OLEDCOMM_AntennaRecordQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id;
	INT32U index = 0;
	INT8U cmdlen = rxbuff[2],i;
	INT8U *pbuff=&rxbuff[3];
	INT8U readbuff[100],readlen,check_sum;

	INT32U degree_Int32u;
	INT32U index1=0;

	strAntennaRecordList AntennaRecordList;

	cmd_id = (rxbuff[0]<<8)+rxbuff[1];

	if( cmd_id != AntennaRecordQueID )
		return FAIL;

	index1 = 0;
	for(i=0;i<ANTENNA_RECORD_MAX_NUM;i++)
	{
		if( (ACUDevInfo.AntennaRecordList.record_ID[i] > 0 ) && (ACUDevInfo.AntennaRecordList.record_ID[i]<= ANTENNA_RECORD_MAX_NUM))
		{
			if( APP_PARAM_AntennaRecord_Read(ACUDevInfo.AntennaRecordList.record_ID[i] , readbuff) == FAIL)
				return FAIL;

			
			OLEDCOMMTxBuff[index++] = 0xBC;
			OLEDCOMMTxBuff[index++] = 0x29;
			OLEDCOMMTxBuff[index++] = ((cmd_id | 0x8000) & 0xFF00)>>8;
			OLEDCOMMTxBuff[index++] = ((cmd_id | 0x8000) & 0x00FF);
			OLEDCOMMTxBuff[index++] = 0x26;
			memcpy(&OLEDCOMMTxBuff[index],readbuff,34);
			index += 34;
			memcpy(&OLEDCOMMTxBuff[index],&readbuff[46],4);
			index += 4;
			check_sum = TOOL_CheckSum8BitGet(&OLEDCOMMTxBuff[index1],43);
			OLEDCOMMTxBuff[index++] = check_sum;
			OLEDCOMMTxBuff[index++] = 0xCB;

			OLEDCOMMTxBuffLen = index;
			SWIFT_UART3_SendBuff(OLEDCOMMTxBuff,OLEDCOMMTxBuffLen);
			BSP_Delay_ms(50);
			index = 0;
			index1 = index;
		}
	}

	//PCCOMMTxBuffLen = index;
	//SWIFT_UART3_SendBuff(PCCOMMTxBuff,PCCOMMTxBuffLen);
	//BSP_Delay_ms(100);

	index = 0;
	txbuff[index++] = ((cmd_id | 0x8000) & 0xFF00)>>8;
	txbuff[index++] = ((cmd_id | 0x8000) & 0x00FF);
	txbuff[index++] = 0x22;
	memset(readbuff,0xFF,63);
	memcpy(&txbuff[index],readbuff,34);
	index += 34;
	
	*txbufflen = index;
	
	return SUCC;
}

/***********************************************************
**name:	APP_OLEDCOMM_MotorSoftLimitQue
**describe:  天线位置电机软限位查询
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OLEDCOMM_MotorSoftLimitQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id;
	INT8U index = 3;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];

	INT32U degree_Int32u;
	INT32S degree_Int32s;

	cmd_id = (rxbuff[0]<<8)+rxbuff[1];

	if( cmd_id != MotorSoftLimitQueID )
		return FAIL;
	
	degree_Int32u =  (INT32U)(ACUDevInfo.ACUParam.AntennaParam.motor_position_start * 1000);
	txbuff[index++] = (degree_Int32u  & 0xFF000000)>>24;
	txbuff[index++] = (degree_Int32u  & 0x00FF0000)>>16;
	txbuff[index++] = (degree_Int32u  & 0x0000FF00)>>8;
	txbuff[index++] = (degree_Int32u  & 0x000000FF);

	degree_Int32u =  (INT32U)(ACUDevInfo.ACUParam.AntennaParam.motor_position_end * 1000);
	txbuff[index++] = (degree_Int32u  & 0xFF000000)>>24;
	txbuff[index++] = (degree_Int32u  & 0x00FF0000)>>16;
	txbuff[index++] = (degree_Int32u  & 0x0000FF00)>>8;
	txbuff[index++] = (degree_Int32u  & 0x000000FF);

	degree_Int32u =  (INT32U)(ACUDevInfo.ACUParam.AntennaParam.motor_cabrage_start * 1000);
	txbuff[index++] = (degree_Int32u  & 0xFF000000)>>24;
	txbuff[index++] = (degree_Int32u  & 0x00FF0000)>>16;
	txbuff[index++] = (degree_Int32u  & 0x0000FF00)>>8;
	txbuff[index++] = (degree_Int32u  & 0x000000FF);

	degree_Int32u =  (INT32U)(ACUDevInfo.ACUParam.AntennaParam.motor_cabrage_end * 1000);
	txbuff[index++] = (degree_Int32u  & 0xFF000000)>>24;
	txbuff[index++] = (degree_Int32u  & 0x00FF0000)>>16;
	txbuff[index++] = (degree_Int32u  & 0x0000FF00)>>8;
	txbuff[index++] = (degree_Int32u  & 0x000000FF);

	degree_Int32s =  (INT32S)(ACUDevInfo.ACUParam.AntennaParam.motor_polar_start * 1000);
	txbuff[index++] = (degree_Int32s  & 0xFF000000)>>24;
	txbuff[index++] = (degree_Int32s  & 0x00FF0000)>>16;
	txbuff[index++] = (degree_Int32s  & 0x0000FF00)>>8;
	txbuff[index++] = (degree_Int32s  & 0x000000FF);

	degree_Int32s =  (INT32S)(ACUDevInfo.ACUParam.AntennaParam.motor_polar_end * 1000);
	txbuff[index++] = (degree_Int32s  & 0xFF000000)>>24;
	txbuff[index++] = (degree_Int32s  & 0x00FF0000)>>16;
	txbuff[index++] = (degree_Int32s  & 0x0000FF00)>>8;
	txbuff[index++] = (degree_Int32s  & 0x000000FF);

	
	*txbufflen = index;
	
	txbuff[0] = ((cmd_id | 0x8000) & 0xFF00)>>8;
	txbuff[1] = ((cmd_id | 0x8000) & 0x00FF);
	txbuff[2] = *txbufflen - 3;
	
	return SUCC;
}



/***********************************************************
**name:	APP_OLEDCOMM_DevStaQue
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OLEDCOMM_DevStaQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id;
	INT8U index = 3;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];

	INT32U degree_int32u;
	INT32S degree_int32s;
	INT32U	db_int32u;
	INT32S	db_int32s;

	cmd_id = (rxbuff[0]<<8)+rxbuff[1];

	if( cmd_id != DevStaQueID )
		return FAIL;

	degree_int32u = (INT32U)(ACUDevInfo.ACUCurrSta.antenna_position * 1000);
//	degree_int32u = 123456;
	txbuff[index++] = (degree_int32u & 0xFF000000)>>24;
	txbuff[index++] = (degree_int32u & 0x00FF0000)>>16;
	txbuff[index++] = (degree_int32u & 0x0000FF00)>>8;
	txbuff[index++] = (degree_int32u & 0x000000FF);

	degree_int32u = (INT32U)(ACUDevInfo.ACUCurrSta.antenna_cabrage * 1000);
//	degree_int32u = 12345;
	txbuff[index++] = (degree_int32u & 0xFF000000)>>24;
	txbuff[index++] = (degree_int32u & 0x00FF0000)>>16;
	txbuff[index++] = (degree_int32u & 0x0000FF00)>>8;
	txbuff[index++] = (degree_int32u & 0x000000FF);

// 	if( ACUDevInfo.DeviceInfo.AllElecCoderMapAntenna.SatPolarType != ACUDevInfo.ACUParam.CurrSatParam.polar_type )
// 	{
// 		if(ACUDevInfo.ACUParam.CurrSatParam.polar_type == 'Y')
// 			degree_int32s = (INT32S)((ACUDevInfo.ACUCurrSta.antenna_polar + 90)*1000);
// 		else
// 			degree_int32s = (INT32S)((ACUDevInfo.ACUCurrSta.antenna_polar - 90)*1000);
// 	}
// 	else
	degree_int32s = (INT32S)(ACUDevInfo.ACUCurrSta.antenna_polar * 1000);
//	degree_int32s = -56789;
	txbuff[index++] = (degree_int32s & 0xFF000000)>>24;
	txbuff[index++] = (degree_int32s & 0x00FF0000)>>16;
	txbuff[index++] = (degree_int32s & 0x0000FF00)>>8;
	txbuff[index++] = (degree_int32s & 0x000000FF);

	txbuff[index++] = ((INT32U)(ACUDevInfo.ACUCurrSta.beacon_rxpower_mv ) & 0xFF000000)>>24;
	txbuff[index++] = ((INT32U)(ACUDevInfo.ACUCurrSta.beacon_rxpower_mv ) & 0x00FF0000)>>16;
	txbuff[index++] = ((INT32U)(ACUDevInfo.ACUCurrSta.beacon_rxpower_mv ) & 0x0000FF00)>>8;
	txbuff[index++] = ((INT32U)(ACUDevInfo.ACUCurrSta.beacon_rxpower_mv ) & 0x000000FF);

	db_int32s = (INT32S)(ACUDevInfo.ACUCurrSta.beacon_rxpower_db*1000);
	txbuff[index++] = (db_int32s & 0xFF000000)>>24;
	txbuff[index++] = (db_int32s & 0x00FF0000)>>16;
	txbuff[index++] = (db_int32s & 0x0000FF00)>>8;
	txbuff[index++] = (db_int32s & 0x000000FF);

	txbuff[index++] = ACUDevInfo.ACUCurrSta.dev_move_sta;

	txbuff[index++] = ACUDevInfo.ACUCurrSta.dev_admin_sta;

	txbuff[index++] = ACUDevInfo.ACUCurrSta.dev_work_sta;

	txbuff[index++] = ACUDevInfo.ACUCurrSta.dev_alarm_flag;

	txbuff[index++] = ACUDevInfo.ACUCurrSta.dev_alarm_reflash_flag[1];

	txbuff[index++] = ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode;


	*txbufflen = index;

	txbuff[0] = ((cmd_id | 0x8000) & 0xFF00)>>8;
	txbuff[1] = ((cmd_id | 0x8000) & 0x00FF);
	txbuff[2] = *txbufflen - 3;
	
	return SUCC;
}



/***********************************************************
**name:	APP_OLEDCOMM_DevAdminQue
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OLEDCOMM_DevAdminQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id;
	INT8U index = 3;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];

	INT32U degree_int32u;
	INT32U	db_int32u;

	cmd_id = (rxbuff[0]<<8)+rxbuff[1];

	if( cmd_id != DevAdminQueID)
		return FAIL;

	txbuff[index++] = ACUDevInfo.ACUCurrSta.dev_admin_sta;

	*txbufflen = index;

	txbuff[0] = ((cmd_id | 0x8000) & 0xFF00)>>8;
	txbuff[1] = ((cmd_id | 0x8000) & 0x00FF);
	txbuff[2] = *txbufflen - 3;
	
	return SUCC;
}



/***********************************************************
**name:	APP_OLEDCOMM_DevAlarmQue
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OLEDCOMM_DevAlarmQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id;
	INT8U index = 3;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];
	INT32U len = 0;

	INT32U degree_int32u;
	INT32U	db_int32u;

	cmd_id = (rxbuff[0]<<8)+rxbuff[1];

	if( cmd_id != CurrWarnStaQueID )
		return FAIL;

	APP_MONITOR_AlarmStaCodeDeal(&txbuff[index],&len);

	txbuff[0] = ((cmd_id | 0x8000) & 0xFF00)>>8;
	txbuff[1] = ((cmd_id | 0x8000) & 0x00FF);
	txbuff[2] = len ;

	*txbufflen = len+3;

	ACUDevInfo.ACUCurrSta.dev_alarm_reflash_flag[1] = 0;
	
	return SUCC;
}



/***********************************************************
**name:	APP_OLEDCOMM_LocalParamQue
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OLEDCOMM_OptLogQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id;
	INT8U index = 0;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];
	INT32U log_cnt = 0;
	INT8U i;

	cmd_id = (rxbuff[0]<<8)+rxbuff[1];

	if( cmd_id !=  OperationLogQueID)
		return FAIL;

	
	
	return SUCC;
}

/***********************************************************
**name:	APP_OLEDCOMM_LocalParamQue
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OLEDCOMM_WarnLogQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id,i;
	INT8U index = 0;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];
	INT32U log_cnt = 0;

	cmd_id = (rxbuff[0]<<8)+rxbuff[1];

	if( cmd_id != WarnLogQueID )
		return FAIL;
	
	return SUCC;
}

/***********************************************************
**name:	APP_OLEDCOMM_VersionQue
**describe:   版本查询
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OLEDCOMM_VersionQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id,i;
	INT8U index = 3;
	INT8U HWverbuff[16],EMBverbuff[16],bufflen;

	cmd_id = (rxbuff[0]<<8)+rxbuff[1];

	if( cmd_id != VersionQueID )
		return FAIL;

	memset(ACUDevInfo.Version[1].softver,0x20,16);
	memset(ACUDevInfo.Version[1].hardver,0x20,16);

//	memcpy(ACUDevInfo.Version[0].softver,"EMS V1.00.B030  ",16);
//	memcpy(ACUDevInfo.Version[0].hardver,"HW V1.00.B010   ",16);
	
	APP_PDUCOMM_VersionQue();

	

	memcpy(&txbuff[index],ACUDevInfo.Version[0].softver,16 );
	index += 16;

	memcpy(&txbuff[index],ACUDevInfo.Version[0].hardver,16 );
	index += 16;

	memcpy(&txbuff[index],ACUDevInfo.Version[1].softver,16 );
	index += 16;

	memcpy(&txbuff[index],ACUDevInfo.Version[1].hardver,16 );
	index += 16;

	*txbufflen = index;

	txbuff[0] = ((cmd_id | 0x8000) & 0xFF00)>>8;
	txbuff[1] = ((cmd_id | 0x8000) & 0x00FF);
	txbuff[2] = *txbufflen - 3;
	
	return SUCC;
	
}


