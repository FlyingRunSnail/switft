


#define APP_PCCOMM_DEF

#include <app_pccomm.h>
#include <app_tool.h>
#include <app_log.h>
#include <math.h>
#include <app_param_save_read.h>
#include <app_beacon.h>
#include <app_monitor.h>
#define		APP_PCCOMM_TIMEOUT_MS		200


extern void APP_UDP_Sendto(INT8U *pbuff,INT32U len);


INT8U PCCOMMRxBuff[50];
INT8U PCCOMMRxBuffLen = 0;

INT8U PCCOMMTxBuff[1500];
INT16U PCCOMMTxBuffLen = 0;

typedef struct _strPCCOMMCmdDeal_
{
	INT16U cmd_id;
	int (* APP_PCCOMM_Func_Deal)(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);
}strPCCOMMCmdDeal;

strPCCOMMCmdDeal PCCOMMCmdDealTbl[]=
{
	{ SatParamSetID	,			APP_PCCOMM_SatParamSet },
	{ AntennaParamSetID	,		APP_PCCOMM_AntennaParamSet },
	{ AntennaParamAdjustID	,	APP_PCCOMM_AntennaParamAdjustSet },
	{ LocalParamSetID	,			APP_PCCOMM_LocalParamSet },
	{ LocalParamSrcSelSetID,		APP_PCCOMM_LocalParamSrcSelSet },
	{ SysDateTimeSetID	,		APP_PCCOMM_SysDateTimeSet },
	{ SysDateTimeSrcSelSetID,		APP_PCCOMM_SysDateTimeSrcSelSet },
	{ SatDelID,					APP_PCCOMM_DelSatSet },
	{AimAtSatParamSetID,			APP_PCCOMM_AimAtSatParamSet},
	{AntennaTurnDirAdjustBeginSetID,APP_PCCOMM_AntennaTurnDirAdjustBegin},
	{AntennaTurnDirAdjustFinishSetID,APP_PCCOMM_AntennaTurnDirAdjustFinish},
	{AntennaHardWareLimitSetID,	APP_PCCOMM_AntennaHargWareLimitAdjust},
	
	{ AimAtSatModeSelID	,		APP_PCCOMM_AimAtSatModeSel },
	{ AntennaRecordID	,		APP_PCCOMM_AntennaRecord},
	{ AntennaSigleStepCtrID	,	APP_PCCOMM_AntennaSigleStepCtr },
	{ AntennaDegreeCtrID	,		APP_PCCOMM_AntennaTurnCtr },
	{AntennaRecordGotoID,			APP_PCCOMM_AntennaRecordGoto},
	{ AntennaRecordDelID,			APP_PCCOMM_DelAntennaRecord},
	{AGCRecordID,				APP_PCCOMM_AGCRecord},
	{ SatSelID	,				APP_PCCOMM_SelSat },
//	{ MotorSoftLimitSetID	,		APP_PCCOMM_MotorSoftLimit },
	{ NetWorkSetID	,			APP_PCCOMM_NetWorkSet },
	{ PCHighAdminID	,			APP_PCCOMM_PCAdminSet },
	{ FactoryResetID,			APP_PCCOMM_FactoryReset},

	
	{ CurrSatParamQueID	,			APP_PCCOMM_CurrSatParamQue },
	{ AllSaveSatParamQueID	,		APP_PCCOMM_SaveAllSatParamQue },
	{ AntennaParamQueID	,			APP_PCCOMM_AntennaParamQue },
	{ AntennaAdjustParamQueID	,	APP_PCCOMM_AntennaAdjustParamQue },
	{ LocalParamQueID	,			APP_PCCOMM_LocalParamQue },
	{ DevSysDateTimeQueID	,		APP_PCCOMM_DevSysDateTimeQue },
	{ NetWorkQueID,					APP_PCCOMM_NetWorkQue},
	{ AimAtSatTheoreticalDegreeQueID,	APP_PCCOMM_AimAtSatDegreeQue },
	{ AntennaRecordQueID	,			APP_PCCOMM_AntennaRecordQue },
	{ MotorSoftLimitQueID	,			APP_PCCOMM_MotorSoftLimitQue },
	{ AimAtSatParamQueID,			APP_PCCOMM_AimAtSatParamQue},
	{ DevStaQueID	,			APP_PCCOMM_DevStaQue },
	{ DevAdminQueID	,			APP_PCCOMM_DevAdminQue },
	{ CurrWarnStaQueID	,			APP_PCCOMM_DevAlarmQue },
	
	{ WarnLogQueID	,			APP_PCCOMM_WarnLogQue },
	{ OperationLogQueID	,			APP_PCCOMM_OptLogQue },
	//{ ParamSetLogQueID ,				},
	{ VersionQueID	,			APP_PCCOMM_VersionQue },
	
	
};

static INT16U PCCOMMCmdDealTbl_Size = sizeof(PCCOMMCmdDealTbl) / sizeof(strPCCOMMCmdDeal);


static INT16U AntennaDir;
/***********************************************************
**name:	APP_PCCOMM_Init
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void APP_PCCOMM_Init(void)
{
	//HW_PCCOMM_Init(115200);
	
	PCCOMMRx.databuff = PCCOMMRxBuff;
	
	PCCOMMTx.head = 0xBC;
	PCCOMMTx.datalen = 0;
	PCCOMMTx.databuff = &PCCOMMTxBuff[2];
	PCCOMMTx.checksum = 0;
	PCCOMMTx.end = 0xCB;

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
**name:	APP_PCCOMM_RxBuffCheck
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_RxBuffCheck(INT8U *buff, INT16U *len)
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
**name:	APP_PCCOMM_RxBuffGet
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_RxBuffRecv(void*p,INT16U len)
{
	INT8U buff[259]={0};

	if( len <= 255)
		memcpy(buff,(INT8U *)p,len);
	else
		return FAIL;

	if(APP_PCCOMM_RxBuffCheck(buff, &len) == FAIL)
		return FAIL;

	PCCOMMRx.datalen = buff[1];
	memcpy( PCCOMMRx.databuff,&buff[2],PCCOMMRx.datalen);

	return SUCC;
}

/***********************************************************
**name:	APP_PCCOMM_TxBuffSend
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void APP_PCCOMM_TxBuffSend(strPCCOMMPro txbuff)
{
	INT16U index=0;

	PCCOMMTxBuff[index++] = txbuff.head;
	PCCOMMTxBuff[index++] = txbuff.datalen;
	//memcpy(&PCCOMMTxBuff[index],txbuff.databuff,txbuff.datalen);
	index += txbuff.datalen;
	PCCOMMTxBuff[index++] = TOOL_CheckSum8BitGet(PCCOMMTxBuff,index);
	PCCOMMTxBuff[index++] = txbuff.end;

	PCCOMMTxBuffLen = index;
	//HW_PCCOMM_UART_SendBuff(buff,index);
	APP_UDP_Sendto(PCCOMMTxBuff,PCCOMMTxBuffLen);
	BSP_Delay_ms(20);
}

/***********************************************************
**name:	APP_PCCOMM_CmdDeal
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_CmdDeal(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U i=0;
	INT16U cmd_id = 0;
	int status = FAIL;

	cmd_id = (rxbuff[0]<<8)+rxbuff[1];
	
	ACUDevInfo.DeviceInfo.PCCommStat.mcu_busy = 1;

	if(((cmd_id & 0x0F00) == 0x0100) ||((cmd_id & 0x0F00) == 0x0200))
	{
		if( cmd_id != PCHighAdminID )
		{
			if( (ACUDevInfo.DeviceInfo.PCCommStat.dst_IP_addr != ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_ip_addr ) ||(ACUDevInfo.ACUCurrSta.DevAdmin.admin_sta != 0x01))
			{
				APP_PCCOMM_SetResult_Send(cmd_id,status);
				return FAIL;
			}
		}
	}
	
	for(i=0;i<PCCOMMCmdDealTbl_Size;i++)
	{
		if( cmd_id == PCCOMMCmdDealTbl[i].cmd_id )
		{
			status = PCCOMMCmdDealTbl[i].APP_PCCOMM_Func_Deal(rxbuff, rxbufflen,txbuff, txbufflen);
			if( ((cmd_id & 0xFF00) == 0x0100) || ((cmd_id & 0xFF00) == 0x0200))
			{
				APP_PCCOMM_SetResult_Send(cmd_id,status);
			}
			else if( ((cmd_id & 0xFF00) == 0x0800) || ((cmd_id & 0xFF00) == 0x0A00) || ((cmd_id & 0xFF00) == 0x0900))
			{
				APP_PCCOMM_TxBuffSend(PCCOMMTx);
			}
			ACUDevInfo.DeviceInfo.PCCommStat.mcu_busy = 0;
			return status;
		}
	}
	ACUDevInfo.DeviceInfo.PCCommStat.mcu_busy = 0;
	return status;
}


/***********************************************************
**name:	APP_PCCOMM_Deal
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_Deal(void*p,INT16U len)
{
	
	
	if( APP_PCCOMM_RxBuffRecv(p,len)  == FAIL)
		return FAIL;

	if(APP_PCCOMM_CmdDeal(PCCOMMRx.databuff, &PCCOMMRx.datalen,PCCOMMTx.databuff, &PCCOMMTx.datalen) == FAIL)
		return FAIL;

	return SUCC;
}

/***********************************************************
**name:	APP_PCCOMM_SetResult_Send
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void APP_PCCOMM_SetResult_Send(INT16U cmd_id,int status)
{
	INT8U index=0;

	PCCOMMTx.head = 0xBC;
	
	PCCOMMTx.databuff[index++] = ((cmd_id | 0x8000) & 0xFF00)>>8;
	PCCOMMTx.databuff[index++] = ((cmd_id | 0x8000) & 0x00ff);

	PCCOMMTx.databuff[index++] = 1;

	if( status == SUCC)
		PCCOMMTx.databuff[index++] = 0;
	else
		PCCOMMTx.databuff[index++] = 1;

	PCCOMMTx.datalen = index;

	PCCOMMTx.end = 0xCB;

	APP_PCCOMM_TxBuffSend(PCCOMMTx);
	BSP_Delay_ms(10);

	
	
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
//具体命令处理函数
/***********************************************************
**name:	APP_PCCOMM_SatParamSet
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_SatParamSet(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id;
	INT16U index = 0,i;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];
//	INT8U refresh_flag = 0;
//	INT32U beacon_freq;
	INT8U sat_reflash_flag = 0;	// 0表示只是修改， 1 表示增加， 2表示删除
	FP32		lon;
	strSatParam 			AddSatParam;


	cmd_id = (rxbuff[0]<<8)+rxbuff[1];

	if( (cmd_id != SatParamSetID ) || (cmdlen !=0x1B ))
		return FAIL;
	

	AddSatParam.id =  pbuff[index++];

	// 表示是增添卫星
	if( AddSatParam.id == 0x00 )
	{
		if( ACUDevInfo.SATList.Sat_Total_num >= SAT_MAX_NUM)
			return FAIL;
			
		// 查找最近的卫星标号
		for(i=0;i<SAT_MAX_NUM;i++)
		{
			if(ACUDevInfo.SATList.Sat_ID[i] == 0)
			{
				AddSatParam.id = i+1;
				sat_reflash_flag = 1;
				break;
			}
		}
	}
	memcpy(AddSatParam.name, &pbuff[index],16); 
	index += 16;
	
	AddSatParam.lon_flag = pbuff[index++]; 
	lon =(FP32)((pbuff[index]<<24) +  (pbuff[index+1]<<16) + (pbuff[index+2]<<8) +pbuff[index+3]);

	AddSatParam.lon = (FP32)lon / (FP32)1000;
	index += 4;
	AddSatParam.freq= (pbuff[index]<<24) +  (pbuff[index+1]<<16) + (pbuff[index+2]<<8) +pbuff[index+3] ;
	index += 4;
	AddSatParam.polar_type = pbuff[index++];

	BSP_Delay_ms(2);

	//  存储到EEPROM中去
	if( APP_PARAM_SatParam_Save(&AddSatParam) == FAIL)
		return FAIL;

	

	// 更新卫星列表
	if( sat_reflash_flag == 1)
	{
		ACUDevInfo.SATList.Sat_Total_num++;
		APP_PARAM_SatTotalNum_Save(ACUDevInfo.SATList.Sat_Total_num);
		ACUDevInfo.SATList.Sat_ID[AddSatParam.id - 1] = AddSatParam.id;
		ACUDevInfo.SATList.SatList_Refresh_flag = 0x01;

		// 存储操作日志到flash中去
		APP_OptLOG_Save_SatOpt(LOG_OPT_ADD_STAR,AddSatParam);
	}
	else if( sat_reflash_flag == 0 )
	{
		ACUDevInfo.SATList.Sat_ID[AddSatParam.id - 1] = AddSatParam.id;
		ACUDevInfo.SATList.SatList_Refresh_flag = 0x01;

		// 存储操作日志到flash中去
		APP_OptLOG_Save_SatOpt(LOG_OPT_REVISE_STAR_PARAM,AddSatParam);
	}


	return SUCC;
}

/***********************************************************
**name:	APP_PCCOMM_AntennaParamSet
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_AntennaParamSet(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id;
	INT8U index = 0;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];
	INT8U gain_range_flag;
	INT8U gain_range;

	strAntennaParam		NewAntennaParam;

	cmd_id = (rxbuff[0]<<8)+rxbuff[1];

	if( cmd_id != AntennaParamSetID )
		return FAIL;
	
	NewAntennaParam.type = pbuff[index++];

	// 偏焦角
	if( NewAntennaParam.type == 0x00 )
		NewAntennaParam.offset_angle = 0x00;
	else
		NewAntennaParam.offset_angle =  (pbuff[index]<<24) +  (pbuff[index+1]<<16) + (pbuff[index+2]<<8) +pbuff[index+3] ;
	index += 4;

	// 口径
	NewAntennaParam.caliber = pbuff[index++];

	// 天线LNB本振
	NewAntennaParam.lo_freq = (pbuff[index]<<24) +  (pbuff[index+1]<<16) + (pbuff[index+2]<<8) +pbuff[index+3] ;
	index += 4;

	// 天线增益输出范围标志 
	NewAntennaParam.gain_range_flag = pbuff[index++];

	// 天线上安装的电子码盘数量
	NewAntennaParam.elec_coder_num = pbuff[index++];

	// 判断增益输出范围是否需要修改
	if(NewAntennaParam.gain_range_flag != ACUDevInfo.DeviceInfo.BeaconInfo.BeaconConf.agc_output_range )
	{
 		if(APP_BEACON_SetAGCOutputRange( NewAntennaParam.gain_range_flag ) == FAIL)
 			return FAIL;
		ACUDevInfo.DeviceInfo.BeaconInfo.BeaconConf.agc_output_range = NewAntennaParam.gain_range_flag;
	}

	if( (ACUDevInfo.ACUParam.AntennaParam.lo_freq >= 8000000) &&(NewAntennaParam.lo_freq < 8000000))
	{
		//需要清除之前的天线姿态记录
		memset(&ACUDevInfo.AntennaRecordList,0x00,sizeof(ACUDevInfo.AntennaRecordList));
		APP_PARAM_AntennaRecord_TotalNum_Save(0x00);
		APP_PARAM_AntennaRecord_AllClear();
	}
	if( (ACUDevInfo.ACUParam.AntennaParam.lo_freq < 8000000) &&(NewAntennaParam.lo_freq >= 8000000))
	{
		//需要清除之前的天线姿态记录
		memset(&ACUDevInfo.AntennaRecordList,0x00,sizeof(ACUDevInfo.AntennaRecordList));
		APP_PARAM_AntennaRecord_TotalNum_Save(0x00);
		APP_PARAM_AntennaRecord_AllClear();
	}
	

	ACUDevInfo.ACUParam.AntennaParam.type = NewAntennaParam.type;
	ACUDevInfo.ACUParam.AntennaParam.offset_angle = NewAntennaParam.offset_angle;
	ACUDevInfo.ACUParam.AntennaParam.caliber = NewAntennaParam.caliber;
	ACUDevInfo.ACUParam.AntennaParam.lo_freq = NewAntennaParam.lo_freq;
	ACUDevInfo.ACUParam.AntennaParam.gain_range_flag = NewAntennaParam.gain_range_flag;
	ACUDevInfo.ACUParam.AntennaParam.elec_coder_num = NewAntennaParam.elec_coder_num;

	BSP_Delay_ms(2);
		
	// 并且重新计算理论倾角
	APP_MONITOR_AimDegree_Get();

 	if( ACUDevInfo.ACUParam.AntennaParam.lo_freq >= 8000000)		//大于8G
 	{
 		//ACUDevInfo.ACUParam.AntennaParam.wave_type = 'K';		// Ku波段
 		ACUDevInfo.ACUParam.SeekSatParam.ScanRange_param[0] = 2;
 		ACUDevInfo.ACUParam.SeekSatParam.ScanRange_param[1] = 5;
 		ACUDevInfo.ACUParam.SeekSatParam.ScanRange_param[2] = 10;
 	}
 	else
 	{
 		//ACUDevInfo.ACUParam.AntennaParam.wave_type = 'C';		// C波段
 		ACUDevInfo.ACUParam.SeekSatParam.ScanRange_param[0] = 5;
 		ACUDevInfo.ACUParam.SeekSatParam.ScanRange_param[1] = 8;
 		ACUDevInfo.ACUParam.SeekSatParam.ScanRange_param[2] = 12;
 	}


	//  存储到EEPROM中去
	if(APP_PARAM_AntennaParam_Save(&ACUDevInfo.ACUParam.AntennaParam) == FAIL)
		return FAIL;
	
	// 存储操作日志到flash中去
	APP_OptLOG_Save_AntennaParamOpt(LOG_OPT_ANTENNA_PARAM_SET_BY_PC,ACUDevInfo.ACUParam.AntennaParam);

	return SUCC;
}



/***********************************************************
**name:	APP_PCCOMM_AntennaParamAdjustSet
**describe:		天线校准
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_AntennaParamAdjustSet(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id;
	INT8U index = 0;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];

	strAllElecCoderMapAntenna AllElecCoderMapAntenna;

	cmd_id = (rxbuff[0]<<8)+rxbuff[1];

	if(cmd_id != AntennaParamAdjustID )
		return FAIL;

	if( ACUDevInfo.DeviceInfo.EleccoderInfo[motor_position_id-1].EleccoderStat.link_stat == COMM_LINK )
	{
		AllElecCoderMapAntenna.ElecCoderMapAntenna[motor_position_id-1].Antenna_degree = ACUDevInfo.ACUParam.AntennaParam.aim_position;
		AllElecCoderMapAntenna.ElecCoderMapAntenna[motor_position_id-1].ElecCoder_degree = ACUDevInfo.DeviceInfo.EleccoderInfo[motor_position_id-1].EleccoderData.degree;
		AllElecCoderMapAntenna.ElecCoderMapAntenna[motor_position_id-1].ElecCoder_count_mode = ACUDevInfo.DeviceInfo.EleccoderInfo[motor_position_id-1].EleccoderConf.count_mode;

		BSP_Delay_ms(1);
	}
	else
		return FAIL;

	if( ACUDevInfo.DeviceInfo.EleccoderInfo[motor_cabrage_id-1].EleccoderStat.link_stat == COMM_LINK )
	{
		AllElecCoderMapAntenna.ElecCoderMapAntenna[motor_cabrage_id-1].Antenna_degree = ACUDevInfo.ACUParam.AntennaParam.aim_cabrage;
		AllElecCoderMapAntenna.ElecCoderMapAntenna[motor_cabrage_id-1].ElecCoder_degree = ACUDevInfo.DeviceInfo.EleccoderInfo[motor_cabrage_id-1].EleccoderData.degree;
		AllElecCoderMapAntenna.ElecCoderMapAntenna[motor_cabrage_id-1].ElecCoder_count_mode = ACUDevInfo.DeviceInfo.EleccoderInfo[motor_cabrage_id-1].EleccoderConf.count_mode;
		BSP_Delay_ms(1);
	}
	else
		return FAIL;

	

	if( ACUDevInfo.ACUParam.AntennaParam.elec_coder_num >= 3 )
	{
		if( ACUDevInfo.DeviceInfo.EleccoderInfo[motor_polar_id-1].EleccoderStat.link_stat == COMM_LINK )
		{
			if( ACUDevInfo.ACUParam.CurrSatParam.polar_type == 'Y')
			{
				if(  ACUDevInfo.ACUParam.AntennaParam.aim_polar < 0)
				{
					AllElecCoderMapAntenna.ElecCoderMapAntenna[motor_polar_id-1].Antenna_degree = ACUDevInfo.ACUParam.AntennaParam.aim_polar + 90;
					AllElecCoderMapAntenna.SatPolarType = 2;
				}
				else
				{
					AllElecCoderMapAntenna.SatPolarType = 1;
					AllElecCoderMapAntenna.ElecCoderMapAntenna[motor_polar_id-1].Antenna_degree = ACUDevInfo.ACUParam.AntennaParam.aim_polar - 90;
				}
			}
			else
			{
				AllElecCoderMapAntenna.SatPolarType = 0;
				AllElecCoderMapAntenna.ElecCoderMapAntenna[motor_polar_id-1].Antenna_degree = ACUDevInfo.ACUParam.AntennaParam.aim_polar;
			}
			AllElecCoderMapAntenna.ElecCoderMapAntenna[motor_polar_id-1].ElecCoder_degree = ACUDevInfo.DeviceInfo.EleccoderInfo[motor_polar_id-1].EleccoderData.degree;
			AllElecCoderMapAntenna.ElecCoderMapAntenna[motor_polar_id-1].ElecCoder_count_mode = ACUDevInfo.DeviceInfo.EleccoderInfo[motor_polar_id-1].EleccoderConf.count_mode;
			BSP_Delay_ms(1);
		}
		else
			return FAIL;
	}
	
	

	if( APP_PARAM_ElecCode_Map_AntennaDegree_Save(&AllElecCoderMapAntenna) == FAIL)
		return FAIL;

	memcpy(&ACUDevInfo.DeviceInfo.AllElecCoderMapAntenna,&AllElecCoderMapAntenna,sizeof(AllElecCoderMapAntenna));

	BSP_Delay_ms(1);

	//校准码盘数据后，需要清除之前的天线姿态记录
	memset(&ACUDevInfo.AntennaRecordList,0x00,sizeof(ACUDevInfo.AntennaRecordList));
	APP_PARAM_AntennaRecord_TotalNum_Save(0x00);
	APP_PARAM_AntennaRecord_AllClear();
	
	// 存储操作日志到flash中去
	{
		APP_OptLOG_Save(LOG_OPT_ANTENNA_CODER_DATA_ADJUST,&rxbuff[3],0);
	}
	
	return SUCC;
}


/***********************************************************
**name:	APP_PCCOMM_LocalParamSet
**describe:  本地位置经纬度设置
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_LocalParamSet(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
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
	CurrLocalPositionParam.lon = (FP32)( (pbuff[index]<<24) +  (pbuff[index+1]<<16) + (pbuff[index+2]<<8) +pbuff[index+3])/(FP32)1000;
	index += 4;

	CurrLocalPositionParam.lat_flag = pbuff[index++];
	CurrLocalPositionParam.lat =   (FP32)((pbuff[index]<<24) +  (pbuff[index+1]<<16) + (pbuff[index+2]<<8) +pbuff[index+3])/(FP32)1000;
	index += 4;

	CurrLocalPositionParam.height =  (pbuff[index]<<24) +  (pbuff[index+1]<<16) + (pbuff[index+2]<<8) +pbuff[index+3];
	index += 4;

	if(	(CurrLocalPositionParam.lon_flag != ACUDevInfo.ACUParam.LocalPositionParam.lon_flag) ||
		(CurrLocalPositionParam.lon != ACUDevInfo.ACUParam.LocalPositionParam.lon) ||
		(CurrLocalPositionParam.lat_flag != ACUDevInfo.ACUParam.LocalPositionParam.lat_flag) ||
		(CurrLocalPositionParam.lat != ACUDevInfo.ACUParam.LocalPositionParam.lat)	)
	{
		ACUDevInfo.ACUParam.LocalPositionParam.lon_flag = CurrLocalPositionParam.lon_flag;
		ACUDevInfo.ACUParam.LocalPositionParam.lon = CurrLocalPositionParam.lon;
		ACUDevInfo.ACUParam.LocalPositionParam.lat_flag = CurrLocalPositionParam.lat_flag;
		ACUDevInfo.ACUParam.LocalPositionParam.lat = CurrLocalPositionParam.lat;

		ACUDevInfo.DeviceInfo.HandLocalPositionParam.lon_flag = CurrLocalPositionParam.lon_flag;
		ACUDevInfo.DeviceInfo.HandLocalPositionParam.lon = CurrLocalPositionParam.lon;
		ACUDevInfo.DeviceInfo.HandLocalPositionParam.lat_flag = CurrLocalPositionParam.lat_flag;
		ACUDevInfo.DeviceInfo.HandLocalPositionParam.lat = CurrLocalPositionParam.lat;

		ACUDevInfo.ACUParam.LocalPositionParam.data_src_flag = 0x01;		//  手动输入
		
		// 并且重新计算理论倾角
		APP_MONITOR_AimDegree_Get();
	
		//  存储到EEPROM中去
		APP_PARAM_LocallHandParam_Save(&CurrLocalPositionParam);

		
	}

	// 存储操作日志到flash中去
	APP_OptLOG_Save_LocallParamOpt(LOG_OPT_SET_LOCALL_PARAM,ACUDevInfo.DeviceInfo.HandLocalPositionParam);

	return SUCC;
}




/***********************************************************
**name:	APP_PCCOMM_LocalParamSrcSelSet
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_LocalParamSrcSelSet(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
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
		ACUDevInfo.ACUParam.LocalPositionParam.lon = (FP32)ACUDevInfo.DeviceInfo.GPSInfo.GPSData.lon / 1000.0;

		ACUDevInfo.ACUParam.LocalPositionParam.lat_flag = ACUDevInfo.DeviceInfo.GPSInfo.GPSData.lat_flag;
		ACUDevInfo.ACUParam.LocalPositionParam.lat =  (FP32)ACUDevInfo.DeviceInfo.GPSInfo.GPSData.lat / 1000.0;

		ACUDevInfo.ACUParam.LocalPositionParam.height = ACUDevInfo.DeviceInfo.GPSInfo.GPSData.height;

		ACUDevInfo.ACUParam.LocalPositionParam.valid_flag  = (ACUDevInfo.DeviceInfo.GPSInfo.GPSData.valid=='A' ? 1:0);

	}
	else if( ACUDevInfo.ACUParam.LocalPositionParam.data_src_flag == 0x01)
	{
		ACUDevInfo.ACUParam.LocalPositionParam.lon_flag = ACUDevInfo.DeviceInfo.HandLocalPositionParam.lon_flag;
		ACUDevInfo.ACUParam.LocalPositionParam.lon = ACUDevInfo.DeviceInfo.HandLocalPositionParam.lon;

		ACUDevInfo.ACUParam.LocalPositionParam.lat_flag = ACUDevInfo.DeviceInfo.HandLocalPositionParam.lat_flag;
		ACUDevInfo.ACUParam.LocalPositionParam.lat = ACUDevInfo.DeviceInfo.HandLocalPositionParam.lat;

		ACUDevInfo.ACUParam.LocalPositionParam.height = ACUDevInfo.DeviceInfo.HandLocalPositionParam.height;

		ACUDevInfo.ACUParam.LocalPositionParam.valid_flag = ACUDevInfo.DeviceInfo.HandLocalPositionParam.valid_flag;

		
	}
	else
	{
		if( ACUDevInfo.DeviceInfo.GPSInfo.GPSData.valid == 'A')
		{
			ACUDevInfo.ACUParam.LocalPositionParam.lon_flag = ACUDevInfo.DeviceInfo.GPSInfo.GPSData.lon_flag;
			ACUDevInfo.ACUParam.LocalPositionParam.lon = (FP32)ACUDevInfo.DeviceInfo.GPSInfo.GPSData.lon / 1000.0;

			ACUDevInfo.ACUParam.LocalPositionParam.lat_flag = ACUDevInfo.DeviceInfo.GPSInfo.GPSData.lat_flag;
			ACUDevInfo.ACUParam.LocalPositionParam.lat = (FP32)ACUDevInfo.DeviceInfo.GPSInfo.GPSData.lat / 1000.0;

			ACUDevInfo.ACUParam.LocalPositionParam.height = ACUDevInfo.DeviceInfo.GPSInfo.GPSData.height;

			ACUDevInfo.ACUParam.LocalPositionParam.valid_flag  = (ACUDevInfo.DeviceInfo.GPSInfo.GPSData.valid=='A' ? 1:0);
		}
		else
		{
			if( ACUDevInfo.DeviceInfo.HandLocalPositionParam.valid_flag > 0 )
			{
				ACUDevInfo.ACUParam.LocalPositionParam.lon_flag = ACUDevInfo.DeviceInfo.HandLocalPositionParam.lon_flag;
				ACUDevInfo.ACUParam.LocalPositionParam.lon = (FP32)ACUDevInfo.DeviceInfo.HandLocalPositionParam.lon /1000.0;

				ACUDevInfo.ACUParam.LocalPositionParam.lat_flag = ACUDevInfo.DeviceInfo.HandLocalPositionParam.lat_flag;
				ACUDevInfo.ACUParam.LocalPositionParam.lat = (FP32)ACUDevInfo.DeviceInfo.HandLocalPositionParam.lat /1000.0;

				ACUDevInfo.ACUParam.LocalPositionParam.height = ACUDevInfo.DeviceInfo.HandLocalPositionParam.height;

				ACUDevInfo.ACUParam.LocalPositionParam.valid_flag  = ACUDevInfo.DeviceInfo.HandLocalPositionParam.valid_flag;
			}
			else
			{
				ACUDevInfo.ACUParam.LocalPositionParam.lon_flag = ACUDevInfo.DeviceInfo.GPSInfo.GPSData.lon_flag;
				ACUDevInfo.ACUParam.LocalPositionParam.lon =  (FP32)ACUDevInfo.DeviceInfo.GPSInfo.GPSData.lon / 1000.0;

				ACUDevInfo.ACUParam.LocalPositionParam.lat_flag = ACUDevInfo.DeviceInfo.GPSInfo.GPSData.lat_flag;
				ACUDevInfo.ACUParam.LocalPositionParam.lat =  (FP32)ACUDevInfo.DeviceInfo.GPSInfo.GPSData.lat / 1000.0;

				ACUDevInfo.ACUParam.LocalPositionParam.height = ACUDevInfo.DeviceInfo.GPSInfo.GPSData.height;

				ACUDevInfo.ACUParam.LocalPositionParam.valid_flag  = (ACUDevInfo.DeviceInfo.GPSInfo.GPSData.valid=='A' ? 1:0);
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
**name:	APP_PCCOMM_SysDateTimeSet
**describe:   系统时间设置
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_SysDateTimeSet(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
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

	//if( ACUDevInfo.SysDateTime.src_flag == 0x01)
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
	if( DATETIME_SavetoRtcIC(&ACUDevInfo.SysDateTime) == FAIL)
		return FAIL;

	// 存储操作日志到flash中去
	{
		;
	}

	return SUCC;
}


/***********************************************************
**name:	APP_PCCOMM_SysDateTimeSrcSelSet
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_SysDateTimeSrcSelSet(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
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
**name:	APP_PCCOMM_DelSatSet
**describe:  删除卫星
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_DelSatSet(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id,i;
	INT8U index = 0;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];
	INT8U del_sat_id = 0;

	static strSatParam 			DelSatParam;

	cmd_id = (rxbuff[0]<<8)+rxbuff[1];
	

	if( cmd_id != SatDelID )
		return FAIL;

	// 查找发送过来的卫星ID是否在已有的卫星参数链表中
	while(cmdlen)
	{
		del_sat_id =  pbuff[index++];
		for( i =0;i<SAT_MAX_NUM;i++)
		{
			if(ACUDevInfo.SATList.Sat_ID[i] == del_sat_id )
			{
				ACUDevInfo.SATList.Sat_ID[i] = 0x00;
				ACUDevInfo.SATList.SatList_Refresh_flag = 0x01;
				if( ACUDevInfo.SATList.Sat_Total_num > 0)
					ACUDevInfo.SATList.Sat_Total_num--;
					
				//在对应的EEPROM地址中 删除相应的卫星参数
				DelSatParam.id = del_sat_id;

				APP_PARAM_SatParam_Read(&DelSatParam);
				APP_PARAM_Sat_Del(&DelSatParam);

				// 存储操作日志到flash中去
				APP_OptLOG_Save_SatOpt(LOG_OPT_DEL_STAR,DelSatParam);
				BSP_Delay_ms(5);
				break;
			}
		}
		cmdlen--;
	}

	APP_PARAM_SatTotalNum_Save(ACUDevInfo.SATList.Sat_Total_num);

	return SUCC;
}



/***********************************************************
**name:	APP_PCCOMM_DelAntennaRecord
**describe:  删除天线记录
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_DelAntennaRecord(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
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

				APP_OptLOG_Save_AntennaPoseOpt(LOG_OPT_ANTENNA_POSE_DEL_BY_PC,SatParam,AntennaParam,degree_AZ,degree_EL,degree_PL);
				
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

	return SUCC;
}

/***********************************************************
**name:	APP_PCCOMM_AGCRecord
**describe: AGC 记录
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_AGCRecord(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
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
**name:	APP_PCCOMM_AimAtSatParamSet
**describe:  追星参数设置
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_AimAtSatParamSet(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id,i;
	INT8U index = 0;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];
	INT32U AimAtSat_Limit_param = 0;

	strSatParam 			DelSatParam;

	cmd_id = (rxbuff[0]<<8)+rxbuff[1];
	

	if(cmd_id != AimAtSatParamSetID )
		return FAIL;

	AimAtSat_Limit_param =  (pbuff[index] << 24) +  (pbuff[index+1] << 16) +  (pbuff[index+2] << 8) +  pbuff[index+3];

	ACUDevInfo.ACUParam.SeekSatParam.AimAtSat_Limit_mv = AimAtSat_Limit_param;


	APP_PARAM_AimAtSatParamLimitmv_Save(&ACUDevInfo.ACUParam.SeekSatParam);

	
	// 存储操作日志到flash中去
	{
		APP_OptLOG_Save(LOG_OPT_SET_FOLLOW_STATR_MIN_LIMIT_PARAM,&rxbuff[3],4);
	}

	return SUCC;
}



/***********************************************************
**name:	APP_PCCOMM_AntennaTurnDirAdjustBegin
**describe:  启动天线转动方向校准
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_AntennaTurnDirAdjustBegin(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id,i;
	INT8U index = 0;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];
	INT8U antenna_dir;
	INT8U antenna_part;
	

	cmd_id = (rxbuff[0]<<8)+rxbuff[1];
	

	if(cmd_id != AntennaTurnDirAdjustBeginSetID )
		return FAIL;

	antenna_part = pbuff[index++];
	
		
	if( ( antenna_part != motor_position_id ) && ( antenna_part != motor_cabrage_id ) && ( antenna_part != motor_polar_id ))
		return FAIL;
	
	antenna_dir = pbuff[index++];

	if( antenna_dir == 0x01)
		antenna_dir = ANTENNA_DIR_RIGHT_UP_CLOCK;
	else if( antenna_dir == 0x02 )
		antenna_dir = ANTENNA_DIR_LEFT_DOWN_ANTICLOCK;
	else
		return FAIL;
	
	if( Motor_DriveStart(antenna_part,MOTOR_DIR_CLOCK) == FAIL)
		return FAIL;

	AntennaDir = (antenna_part << 8) +  MOTOR_DIR_CLOCK;

	ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode = 0x01;

	return SUCC;
}




/***********************************************************
**name:	APP_PCCOMM_AntennaTurnDirAdjustFinish
**describe:  完成天线转动方向校准
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_AntennaTurnDirAdjustFinish(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id,i;
	INT8U index = 0;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];
	INT8U antenna_dir;
	INT8U antenna_part;
	INT8U motor_dir;
	int sta;
	

	cmd_id = (rxbuff[0]<<8)+rxbuff[1];
	

	if(cmd_id != AntennaTurnDirAdjustFinishSetID )
	{
		ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode = 0x00;
		return FAIL;
	}

	antenna_part = pbuff[index++];
	antenna_dir = pbuff[index++];
	if( antenna_dir == 0x01)
		antenna_dir = ANTENNA_DIR_RIGHT_UP_CLOCK;
	else if( antenna_dir == 0x02 )
		antenna_dir = ANTENNA_DIR_LEFT_DOWN_ANTICLOCK;
	else
	{
		ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode = 0x00;
		return FAIL;
	}

	if (( (AntennaDir & 0xFF00) >> 8) != antenna_part)
	{
		Motor_DriveEnd( (AntennaDir & 0xFF00)>>8,AntennaDir & 0x00FF);
		ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode = 0x00;
		return FAIL;
	}
	
	ACUDevInfo.ACUParam.AntennaParam.AntennaDirMapMotor[antenna_part-1].antenna_part = antenna_part;

	if( antenna_dir == (AntennaDir & 0x00FF))
	{
		ACUDevInfo.ACUParam.AntennaParam.AntennaDirMapMotor[antenna_part-1].motor_dir[ANTENNA_DIR_LEFT_DOWN_ANTICLOCK-1] = MOTOR_DIR_ANTICLOCK;
		ACUDevInfo.ACUParam.AntennaParam.AntennaDirMapMotor[antenna_part-1].motor_dir[ANTENNA_DIR_RIGHT_UP_CLOCK-1] = MOTOR_DIR_CLOCK;
	}
	else
	{
		ACUDevInfo.ACUParam.AntennaParam.AntennaDirMapMotor[antenna_part-1].motor_dir[ANTENNA_DIR_LEFT_DOWN_ANTICLOCK-1] = MOTOR_DIR_CLOCK;
		ACUDevInfo.ACUParam.AntennaParam.AntennaDirMapMotor[antenna_part-1].motor_dir[ANTENNA_DIR_RIGHT_UP_CLOCK-1] = MOTOR_DIR_ANTICLOCK;
	}

	// 存储进EEPROM
	if( APP_PARAM_AntennaDir_Map_MotorDir_Save(antenna_part,&ACUDevInfo.ACUParam.AntennaParam.AntennaDirMapMotor[antenna_part-1]) == FAIL)
	{
		ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode = 0x00;
		return FAIL;
	}

	

	BSP_Delay_ms(2);

	// 顺便完成码盘计数模式的校准
	sta = APP_ElecEncoder_DataAddDir_Adjust(antenna_part,antenna_dir) ;

	
	if( Motor_DriveEnd((AntennaDir & 0xFF00)>>8,AntennaDir & 0x00FF) == FAIL)
	{
		ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode = 0x00;
		if( Motor_DriveEnd((AntennaDir & 0xFF00)>>8,AntennaDir & 0x00FF) == FAIL)
			return FAIL;
	}

	// 记录操作日志
	if( antenna_part == motor_position_id )
		APP_OptLOG_SaveCode(LOG_OPT_ANTENNA_AZ_TURN_ADJUST);
	else if( antenna_part == motor_cabrage_id )
		APP_OptLOG_SaveCode(LOG_OPT_ANTENNA_EL_TURN_ADJUST);
	else if( antenna_part == motor_polar_id )
		APP_OptLOG_SaveCode(LOG_OPT_ANTENNA_PL_TURN_ADJUST);
	else
		return FAIL;

	ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode = 0x00;

	return sta;
}



/***********************************************************
**name:	APP_PCCOMM_AntennaHargWareLimitAdjust
**describe:  天线硬限位校准
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_AntennaHargWareLimitAdjust(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id,i;
	INT8U index = 0;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];
	INT8U antenna_dir;
	INT8U antenna_limit_part;
	INT8U	antenna_part;
	INT32U limit_data;
	INT32U log_code;
	FP32		degree;
	INT32S  degree_data;

	

	cmd_id = (rxbuff[0]<<8)+rxbuff[1];
	

	if(cmd_id != AntennaHardWareLimitSetID )
		return FAIL;

	antenna_limit_part = pbuff[index++];

	if(  antenna_limit_part == 0x10)
		log_code = LOG_OPT_ANTENNA_AZ_LEFT_LIMIT_ADJUST;
	else if(  antenna_limit_part == 0x11)
		log_code = LOG_OPT_ANTENNA_AZ_RIGHT_LIMIT_ADJUST;
	else if(  antenna_limit_part == 0x20)
		log_code = LOG_OPT_ANTENNA_EL_DOWN_LIMIT_ADJUST;
	else if(  antenna_limit_part == 0x21)
		log_code = LOG_OPT_ANTENNA_EL_UP_LIMIT_ADJUST;
	else if(  antenna_limit_part == 0x30)
		log_code = LOG_OPT_ANTENNA_PL_CLOCK_LIMIT_ADJUST ;
	else if(  antenna_limit_part == 0x31)
		log_code = LOG_OPT_ANTENNA_PL_ANTICLOCK_LIMIT_ADJUST;
	else
		return FAIL;
		
	antenna_part = (antenna_limit_part & 0xF0)>>4;

	limit_data = ACUDevInfo.DeviceInfo.EleccoderInfo[antenna_part-1].EleccoderData.data;

	BSP_Delay_ms(2);

	if( APP_PARAM_HardWareLimit_Save(antenna_part,antenna_limit_part,limit_data) == FAIL)
		return FAIL;

	if(  antenna_limit_part == 0x10)
	{
		APP_ElecEncoder_Data2DegreeFloat(0x01,limit_data,&degree);
		APP_MONITOR_LocallAntennaAdjustDegreeDataGet(0x01,&degree,&ACUDevInfo.ACUParam.AntennaParam.motor_position_start);
		ACUDevInfo.ACUParam.AntennaParam.motor_position_start += 3;
		ACUDevInfo.ACUParam.AntennaParam.motor_position_start = ceil(ACUDevInfo.ACUParam.AntennaParam.motor_position_start);
	}
	else if(  antenna_limit_part == 0x11)
	{
		APP_ElecEncoder_Data2DegreeFloat(0x01,limit_data,&degree);
		APP_MONITOR_LocallAntennaAdjustDegreeDataGet(0x01,&degree,&ACUDevInfo.ACUParam.AntennaParam.motor_position_end);
		ACUDevInfo.ACUParam.AntennaParam.motor_position_end -= 3;
		ACUDevInfo.ACUParam.AntennaParam.motor_position_end = floor(ACUDevInfo.ACUParam.AntennaParam.motor_position_end);
	}
	else if(  antenna_limit_part == 0x20)
	{
		APP_ElecEncoder_Data2DegreeFloat(0x02,limit_data,&degree);
		APP_MONITOR_LocallAntennaAdjustDegreeDataGet(0x02,&degree,&ACUDevInfo.ACUParam.AntennaParam.motor_cabrage_start);
		degree_data = ACUDevInfo.ACUParam.AntennaParam.motor_cabrage_start += 3;
		ACUDevInfo.ACUParam.AntennaParam.motor_cabrage_start = ceil(ACUDevInfo.ACUParam.AntennaParam.motor_cabrage_start);
	}
	else if(  antenna_limit_part == 0x21)
	{
		APP_ElecEncoder_Data2DegreeFloat(0x02,limit_data,&degree);
		APP_MONITOR_LocallAntennaAdjustDegreeDataGet(0x02,&degree,&ACUDevInfo.ACUParam.AntennaParam.motor_cabrage_end);
		ACUDevInfo.ACUParam.AntennaParam.motor_cabrage_end -= 3;
		ACUDevInfo.ACUParam.AntennaParam.motor_cabrage_end = floor(ACUDevInfo.ACUParam.AntennaParam.motor_cabrage_end);
	}
	else if(  antenna_limit_part == 0x30)
	{
		APP_ElecEncoder_Data2DegreeFloat(0x03,limit_data,&degree);
		APP_MONITOR_LocallAntennaAdjustDegreeDataGet(0x03,&degree,&ACUDevInfo.ACUParam.AntennaParam.motor_polar_end);
		ACUDevInfo.ACUParam.AntennaParam.motor_polar_end -= 3;
		ACUDevInfo.ACUParam.AntennaParam.motor_polar_end = floor(ACUDevInfo.ACUParam.AntennaParam.motor_polar_end);
	}
	else if(  antenna_limit_part == 0x31)
	{
		APP_ElecEncoder_Data2DegreeFloat(0x03,limit_data,&degree);
		APP_MONITOR_LocallAntennaAdjustDegreeDataGet(0x03,&degree,&ACUDevInfo.ACUParam.AntennaParam.motor_polar_start);
		ACUDevInfo.ACUParam.AntennaParam.motor_polar_start += 3;
		ACUDevInfo.ACUParam.AntennaParam.motor_polar_start = ceil(ACUDevInfo.ACUParam.AntennaParam.motor_polar_start);
	}
	else
		return FAIL;

	// 记录操作日志
	APP_OptLOG_SaveCode(log_code);
	


	return SUCC;
}



/***********************************************************
**name:	APP_PCCOMM_LocalParamSet
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_BuzzTest(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{

	return SUCC;
}





/***********************************************************
**name:	APP_PCCOMM_AimAtSatModeSel
**describe: 追星模式选择
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_AimAtSatModeSel(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
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
				
			APP_OptLOG_Save(LOG_OPT_HAND_AIM_STAR_BEGIN_BY_PC,&rxbuff[3],0);
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
			APP_OptLOG_Save(LOG_OPT_AUTO_AIM_STAR_BEGIN_BY_PC,&rxbuff[3],0);
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
			APP_OptLOG_Save(LOG_OPT_FOLLOW_STAR_BEGIN_BY_PC,&rxbuff[3],0);
		}
		
		else if( SeekSat_Mode == 0x00)
		{
			if(  ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode == 0x01 )
			{
				if( Task_Run_Flag[2] > 0 )
					TaskAntennaRecordGotoDel();

				APP_OptLOG_Save(LOG_OPT_HAND_AIM_STAR_END_BY_PC,&rxbuff[3],0);
			}
			if( ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode == 0x02)
			{
				if( Task_Run_Flag[0] > 0 )
					TaskSatelliteTrackDel();

				APP_OptLOG_Save(LOG_OPT_AUTO_AIM_STAR_END_BY_PC,&rxbuff[3],0);
			}
			if( ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode == 0x04)
			{
				if( Task_Run_Flag[2] > 0 )
					TaskAntennaRecordGotoDel();

				APP_OptLOG_Save(LOG_OPT_ANTENNA_POSE_STOP_BY_PC,&rxbuff[3],0);
			}
			if(  ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode == 0x03 )
			{
				if( Task_Run_Flag[1] > 0 )
					TaskSatGradientTrackDel();

				APP_OptLOG_Save(LOG_OPT_FOLLOW_STAR_END_BY_PC,&rxbuff[3],0);
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
**name:	APP_PCCOMM_AntennaRecord
**describe: 天线位置记录
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_AntennaRecord(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
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
	//DATETIME_ReadFromRtcIC(&ACUDevInfo.SysDateTime);
	AntennaRecordBuff[index++] = (ACUDevInfo.SysDateTime.year & 0xFF00)>>8;
	AntennaRecordBuff[index++] = ACUDevInfo.SysDateTime.year & 0xFF;
	AntennaRecordBuff[index++] = ACUDevInfo.SysDateTime.month;
	AntennaRecordBuff[index++] = ACUDevInfo.SysDateTime.day;
	AntennaRecordBuff[index++] = ACUDevInfo.SysDateTime.hour;
	AntennaRecordBuff[index++] = ACUDevInfo.SysDateTime.min;
	AntennaRecordBuff[index++] = ACUDevInfo.SysDateTime.senc;

	BSP_Delay_ms(2);

	// INDEX =8 

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

	//INDEX = 0X22
	// 存储当前的天线参数
	AntennaRecordBuff[index++] = ACUDevInfo.ACUParam.AntennaParam.type;
	degree_int32u = (INT32U)ACUDevInfo.ACUParam.AntennaParam.offset_angle;
 	AntennaRecordBuff[index++] = (degree_int32u & 0xFF000000)>>24;
 	AntennaRecordBuff[index++] = (degree_int32u & 0x00FF0000)>>16;
 	AntennaRecordBuff[index++] = (degree_int32u & 0x0000FF00)>>8;
 	AntennaRecordBuff[index++] = (degree_int32u & 0x000000FF);
	//AntennaRecordBuff[index++] = ACUDevInfo.ACUParam.AntennaParam.offset_angle;
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

	BSP_Delay_ms(2);

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
		APP_OptLOG_Save_AntennaPoseOpt(	LOG_OPT_ANTENNA_POSE_RECORD_BY_PC,
											ACUDevInfo.ACUParam.CurrSatParam,
											ACUDevInfo.ACUParam.AntennaParam,
											ACUDevInfo.ACUCurrSta.antenna_position,
											ACUDevInfo.ACUCurrSta.antenna_cabrage,								
											ACUDevInfo.ACUCurrSta.antenna_polar);
		BSP_Delay_ms(50);								
	}
	
	return SUCC;
}



/***********************************************************
**name:	APP_PCCOMM_AntennaSigleStepCtr
**describe: 天线手动单步控制
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_AntennaSigleStepCtr(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
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
**name:	APP_PCCOMM_AntennaTurnCtr
**describe: 天线手动转动控制
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_AntennaTurnCtr(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
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

	if( ACUDevInfo.ACUParam.AntennaParam.elec_coder_num >= 3 )
	{
		if(	( ACUDevInfo.ACUParam.AntennaParam.aim_polar_hand <= ACUDevInfo.ACUParam.AntennaParam.motor_polar_start ) ||
			( ACUDevInfo.ACUParam.AntennaParam.aim_polar_hand >=  ACUDevInfo.ACUParam.AntennaParam.motor_polar_end))
			return FAIL;
	}
	TaskAntennaRecordGotoCreate();
		
	
	return SUCC;
}


/***********************************************************
**name:	APP_PCCOMM_AntennaRecordGoto
**describe: 调用天线位置记录
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_AntennaRecordGoto(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
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
	RecordAntennaParam.aim_position = (FP32)((AntennaRecordBuff[index]<<24)+ (AntennaRecordBuff[index+1]<<16)+(AntennaRecordBuff[index+2]<<8)+AntennaRecordBuff[index+3]) / (FP32)1000.0;
	
	index += 4;
	RecordAntennaParam.aim_cabrage =  (FP32)((AntennaRecordBuff[index]<<24)+ (AntennaRecordBuff[index+1]<<16)+(AntennaRecordBuff[index+2]<<8)+AntennaRecordBuff[index+3]) /  (FP32)1000.0;
	
	index += 4;
	RecordAntennaParam.aim_polar =  (FP32)((AntennaRecordBuff[index]<<24)+ (AntennaRecordBuff[index+1]<<16)+(AntennaRecordBuff[index+2]<<8)+AntennaRecordBuff[index+3]) / (FP32)1000.0;

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
	

	// 卫星参数
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
		APP_OptLOG_Save_AntennaPoseOpt(	LOG_OPT_ANTENNA_POSE_APP_BY_PC,
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
**name:	APP_PCCOMM_SelSat
**describe: 选择卫星
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_SelSat(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
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
// 	if( sel_sat_id != ACUDevInfo.ACUParam.CurrSatParam.id )
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
		APP_OptLOG_Save_SatOpt(LOG_OPT_SEL_STAR_BY_PC,ACUDevInfo.ACUParam.CurrSatParam);
	}
	
	return SUCC;
}

#if 0
/***********************************************************
**name:	APP_PCCOMM_MotorSoftLimit
**describe: 电机软限位设置
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_MotorSoftLimit(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id;
	INT8U index = 0,i;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];
	INT8U sel_motor_id = 0;
	INT32S	start_degree,end_degree;


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
			if( APP_PARAM_PositionSoftLimit_Save(&ACUDevInfo.ACUParam.AntennaParam) == FAIL)
				return FAIL;
			break;
		case motor_cabrage_id:
			ACUDevInfo.ACUParam.AntennaParam.motor_cabrage_start = (FP32)start_degree / 1000;
			ACUDevInfo.ACUParam.AntennaParam.motor_cabrage_end = (FP32)end_degree / 1000;
			if( APP_PARAM_CabrageSoftLimit_Save(&ACUDevInfo.ACUParam.AntennaParam) == FAIL)
				return FAIL;
			break;
		case motor_polar_id:
			ACUDevInfo.ACUParam.AntennaParam.motor_polar_start = (FP32)start_degree / 1000;
			ACUDevInfo.ACUParam.AntennaParam.motor_polar_end = (FP32)end_degree / 1000;
			if( APP_PARAM_PolarSoftLimit_Save(&ACUDevInfo.ACUParam.AntennaParam) == FAIL)
				return FAIL;
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
**name:	APP_PCCOMM_NetWorkSet
**describe: 设备网络设置
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_NetWorkSet(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id;
	INT8U index = 0,i;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];
	INT32U ip,mask,gate;

	

	cmd_id = (rxbuff[0]<<8)+rxbuff[1];
	

	if( (cmd_id != NetWorkSetID )|| (cmdlen != 12 ))
		return FAIL;

	ip =  pbuff[index] +  (pbuff[index+1]<<8) + (pbuff[index+2]<<16) +(pbuff[index+3]<<24);
	index += 4;
	mask =  pbuff[index] +  (pbuff[index+1]<<8) + (pbuff[index+2]<<16) +(pbuff[index+3]<<24);
	if( (mask & 0xFF000000) == 0xFF000000)
		return FAIL;
	index += 4;
	gate =  pbuff[index] +  (pbuff[index+1]<<8) + (pbuff[index+2]<<16) +(pbuff[index+3]<<24);
	if( (ip & 0x00FFFFFF) != (gate & 0x00FFFFFF))
		return FAIL;
	index += 4;

	if(	( ip != ACUDevInfo.DeviceInfo.NetWork.ip_addr )  || 
		( mask != ACUDevInfo.DeviceInfo.NetWork.mask ) ||
		( gate != ACUDevInfo.DeviceInfo.NetWork.gate ) )
	{

		if( ip != ACUDevInfo.DeviceInfo.NetWork.ip_addr ) 
		{
			ACUDevInfo.DeviceInfo.NetWork.ip_addr = ip;
			APP_OptLOG_Save(LOG_OPT_NETWORK_IP_ADDR_SET_BY_PC,&rxbuff[3],4);
		}
		if( mask != ACUDevInfo.DeviceInfo.NetWork.mask	)
		{
			ACUDevInfo.DeviceInfo.NetWork.mask = mask;
			APP_OptLOG_Save(LOG_OPT_NETWORK_MASK_SET_BY_PC,&rxbuff[7],4);
		}
		if( gate != ACUDevInfo.DeviceInfo.NetWork.gate )
		{
			ACUDevInfo.DeviceInfo.NetWork.gate = gate;
			APP_OptLOG_Save(LOG_OPT_NETWORK_GATE_SET_BY_PC,&rxbuff[11],4);
		}

		// 将数据存入EEPROM中去
		APP_PARAM_NetWork_Save(&ACUDevInfo.DeviceInfo.NetWork);

		// 调用重新修改IP的接口函数
		LWIP_NetworkConfigChange();
	

		// 存储操作日志到flash中去
		{

		}

	}
	
	return SUCC;
}





/***********************************************************
**name:	APP_PCCOMM_PCAdminSet
**describe: PC机权限设置
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_PCAdminSet(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
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

	// 判断当前设备是否已经处于PDU 权限控制
	curr_admin_sta = ACUDevInfo.ACUCurrSta.dev_admin_sta;

	switch( curr_admin_sta )
	{
		// 当前处于PDU控制
		case 0x02:
			if (admin_switch > 0)		// pc 申请管理权限
				return FAIL;
			break;

		// 当前处于PC 控制
		case 0x01:
			if (admin_switch >= 1 )		// pc 申请管理权限
			{
				if( ACUDevInfo.DeviceInfo.PCCommStat.dst_IP_addr == ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_ip_addr)
					return SUCC;
				else
					return FAIL;
			}
			else						// PC 申请取消管理权限
			{
				if( ACUDevInfo.DeviceInfo.PCCommStat.dst_IP_addr == ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_ip_addr)
				{
					ACUDevInfo.ACUCurrSta.dev_admin_sta = 0x00;
					ACUDevInfo.ACUCurrSta.DevAdmin.admin_sta = 0;
					ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_deal_sta = 0;
					ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_ip_addr = 0;
					APP_OptLOG_SaveCode(LOG_OPT_TIMEOUT_PC_CANCEL_PC_ADMIN);
					
				}

				// 存储操作日志到flash中去
				{
					;
				}
			}
			break;

		// // 当前处于ACU 控制
		case 0x00:
			if (admin_switch > 0 )
			{
				ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_deal_sta = 0x01;

				// 已经有PC在申请控制权限
				if( ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_ask_ip_addr != 0 )
					return FAIL;
				ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_ask_ip_addr = ACUDevInfo.DeviceInfo.PCCommStat.dst_IP_addr;

				#if 1
				//APP_OLEDCOMM_AskForPCAdmin();
				ACUDevInfo.ACUCurrSta.dev_admin_sta = 0x11;
				ACUDevInfo.ACUCurrSta.DevAdmin.admin_sta = 0x11;

				APP_OptLOG_SaveCode(LOG_OPT_PC_ASK_FOR_ADMIN);
				#else
				// 此处向OLED发送请求PC控制权限的命令

				// 暂时默认获得PC权限
				{
					ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_ip_addr = ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_ask_ip_addr;
					ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_ask_ip_addr = 0;
					ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_deal_sta = 0x01;
					ACUDevInfo.ACUCurrSta.DevAdmin.admin_sta = 0x01;
					ACUDevInfo.ACUCurrSta.dev_admin_sta = 0x01;
				}
				#endif
			}
			break;

		// // 当前处于PC授权等待中
		case 0x11:
			if (admin_switch >= 1 )		// pc 申请管理权限
			{
				if( ACUDevInfo.DeviceInfo.PCCommStat.dst_IP_addr != ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_ask_ip_addr)
					return FAIL;
			}
			else
			{
				if( ACUDevInfo.DeviceInfo.PCCommStat.dst_IP_addr == ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_ask_ip_addr)
				{
					ACUDevInfo.ACUCurrSta.dev_admin_sta = 0x00;
					ACUDevInfo.ACUCurrSta.DevAdmin.admin_sta = 0;
					ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_deal_sta = 0;
					ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_ip_addr = 0;
					ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_ask_ip_addr = 0;
					APP_OptLOG_SaveCode(LOG_OPT_TIMEOUT_PC_CANCEL_PC_ADMIN_ASK);
				}
			}
			break;
		default:
			break;
	}
		
	return SUCC;
}

/***********************************************************
**name:	APP_PCCOMM_FactoryReset
**describe: 恢复出厂设置
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_FactoryReset(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id;
	INT8U index = 0,i;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];
	INT8U admin_switch = 0;
	INT8U curr_admin_sta;

	cmd_id = (rxbuff[0]<<8)+rxbuff[1];
	

	if( cmd_id != FactoryResetID )
		return FAIL;

	
	APP_PARAM_HardWareLimit_Save(motor_position_id,0x10,0x00);
	APP_PARAM_HardWareLimit_Save(motor_position_id,0x11,0x00);
	APP_PARAM_HardWareLimit_Save(motor_cabrage_id,0x20,0x00);
	APP_PARAM_HardWareLimit_Save(motor_cabrage_id,0x21,0x00);
	APP_PARAM_HardWareLimit_Save(motor_polar_id,0x30,0x00);
	APP_PARAM_HardWareLimit_Save(motor_polar_id,0x31,0x00);
	
	ACUDevInfo.ACUParam.AntennaParam.motor_position_start = 100;
	ACUDevInfo.ACUParam.AntennaParam.motor_position_end = 260;
	ACUDevInfo.ACUParam.AntennaParam.motor_cabrage_start = 5;
	ACUDevInfo.ACUParam.AntennaParam.motor_cabrage_end = 85;
	ACUDevInfo.ACUParam.AntennaParam.motor_polar_start = -90;
	ACUDevInfo.ACUParam.AntennaParam.motor_polar_end = 90;

		
	return SUCC;
}



/***********************************************************
**name:	APP_PCCOMM_CurrSatParamQue
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_CurrSatParamQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
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
**name:	APP_PCCOMM_SaveAllSatParamQue
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_SaveAllSatParamQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
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

		index1 = index;

		PCCOMMTxBuff[index++] = 0xBC;
		PCCOMMTxBuff[index++] = 30;

		PCCOMMTxBuff[index++] = ((cmd_id | 0x8000) & 0xFF00)>>8;
		PCCOMMTxBuff[index++] = ((cmd_id | 0x8000) & 0x00FF);
		PCCOMMTxBuff[index++] = 27;

		PCCOMMTxBuff[index++] = SatParam.id;

		memcpy(&PCCOMMTxBuff[index],SatParam.name,16);
		index += 16;
		
		
		PCCOMMTxBuff[index++] = SatParam.lon_flag;

		degree_int32u = (INT32U)(SatParam.lon * 1000);
		PCCOMMTxBuff[index++] = (degree_int32u & 0xFF000000)>>24;
		PCCOMMTxBuff[index++] = (degree_int32u & 0x00FF0000)>>16;
		PCCOMMTxBuff[index++] = (degree_int32u & 0x0000FF00)>>8;
		PCCOMMTxBuff[index++] = (degree_int32u & 0x000000FF);

		PCCOMMTxBuff[index++] = (SatParam.freq & 0xFF000000)>>24;
		PCCOMMTxBuff[index++] = (SatParam.freq & 0x00FF0000)>>16;
		PCCOMMTxBuff[index++] = (SatParam.freq & 0x0000FF00)>>8;
		PCCOMMTxBuff[index++] = (SatParam.freq & 0x000000FF);

		PCCOMMTxBuff[index++] = SatParam.polar_type;

		check_sum = TOOL_CheckSum8BitGet(&PCCOMMTxBuff[index1],32);
		PCCOMMTxBuff[index++] = check_sum;

		PCCOMMTxBuff[index++] = 0xCB;
	
		sat_num++;
		Sat_Total_num++;
		if( sat_num >= 40 )
		{
			sat_num = 0;
			PCCOMMTxBuffLen = index;
			APP_UDP_Sendto(PCCOMMTxBuff,PCCOMMTxBuffLen);
			index = 0;
		}
		if( Sat_Total_num >= ACUDevInfo.SATList.Sat_Total_num )
			break;
	}

//	if( ACUDevInfo.SATList.Sat_Total_num != Sat_Total_num)
//	{
//		ACUDevInfo.SATList.Sat_Total_num = Sat_Total_num;
//		APP_PARAM_SatTotalNum_Save(ACUDevInfo.SATList.Sat_Total_num);
//	}	

	if((sat_num < 40 ) && (sat_num>0))
	{
		PCCOMMTxBuffLen = index;
		APP_UDP_Sendto(PCCOMMTxBuff,index);
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
**name:	APP_PCCOMM_AntennaParamQue
**describe:		//  天线信息查询
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_AntennaParamQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
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
**name:	APP_PCCOMM_AntennaAdjustParamQue
**describe:	 天线校准参数查询
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_AntennaAdjustParamQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
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
**name:	APP_PCCOMM_LocalParamQue
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_LocalParamQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
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
**name:	APP_PCCOMM_DevSysDateTimeQue
**describe:   设备系统日期时间查询
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_DevSysDateTimeQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
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
**name:	APP_PCCOMM_AimAtSatDegreeQue
**describe:  对星理论角查询
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_AimAtSatDegreeQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
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
**name:	APP_PCCOMM_AntennaRecordQue
**describe:  天线位置记录查询
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_AntennaRecordQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
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

			BSP_Delay_ms(5);
			
			PCCOMMTxBuff[index++] = 0xBC;
			PCCOMMTxBuff[index++] = 0x29;
			PCCOMMTxBuff[index++] = ((cmd_id | 0x8000) & 0xFF00)>>8;
			PCCOMMTxBuff[index++] = ((cmd_id | 0x8000) & 0x00FF);
			PCCOMMTxBuff[index++] = 0x26;
			memcpy(&PCCOMMTxBuff[index],readbuff,34);
			index += 34;
			memcpy(&PCCOMMTxBuff[index],&readbuff[46],4);
			index += 4;
			check_sum = TOOL_CheckSum8BitGet(&PCCOMMTxBuff[index1],43);
			PCCOMMTxBuff[index++] = check_sum;
			PCCOMMTxBuff[index++] = 0xCB;

			index1 = index;
		}
	}

	PCCOMMTxBuffLen = index;
	APP_UDP_Sendto(PCCOMMTxBuff,PCCOMMTxBuffLen);
	BSP_Delay_ms(100);

	index = 0;
	txbuff[index++] = ((cmd_id | 0x8000) & 0xFF00)>>8;
	txbuff[index++] = ((cmd_id | 0x8000) & 0x00FF);
	txbuff[index++] = 0x26;
	memset(readbuff,0xFF,63);
	memcpy(&txbuff[index],readbuff,38);
	index += 38;
	
	*txbufflen = index;
	
	return SUCC;
}

/***********************************************************
**name:	APP_PCCOMM_MotorSoftLimitQue
**describe:  天线位置电机软限位查询
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_MotorSoftLimitQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
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
**name:	APP_PCCOMM_NetWorkQue
**describe:   设备网络相关设置查询
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_NetWorkQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
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
**name:	APP_PCCOMM_DevStaQue
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_DevStaQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
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
	txbuff[index++] = (degree_int32u & 0xFF000000)>>24;
	txbuff[index++] = (degree_int32u & 0x00FF0000)>>16;
	txbuff[index++] = (degree_int32u & 0x0000FF00)>>8;
	txbuff[index++] = (degree_int32u & 0x000000FF);

	degree_int32u = (INT32U)(ACUDevInfo.ACUCurrSta.antenna_cabrage * 1000);
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
	txbuff[index++] = (degree_int32s & 0xFF000000)>>24;
	txbuff[index++] = (degree_int32s & 0x00FF0000)>>16;
	txbuff[index++] = (degree_int32s & 0x0000FF00)>>8;
	txbuff[index++] = (degree_int32s & 0x000000FF);

	txbuff[index++] = ((INT32U)(ACUDevInfo.ACUCurrSta.beacon_rxpower_mv ) & 0xFF000000)>>24;
	txbuff[index++] = ((INT32U)(ACUDevInfo.ACUCurrSta.beacon_rxpower_mv ) & 0x00FF0000)>>16;
	txbuff[index++] = ((INT32U)(ACUDevInfo.ACUCurrSta.beacon_rxpower_mv ) & 0x0000FF00)>>8;
	txbuff[index++] = ((INT32U)(ACUDevInfo.ACUCurrSta.beacon_rxpower_mv ) & 0x000000FF);

	db_int32s = (INT32S)(ACUDevInfo.ACUCurrSta.beacon_rxpower_db *1000);
	txbuff[index++] = (db_int32s & 0xFF000000)>>24;
	txbuff[index++] = (db_int32s & 0x00FF0000)>>16;
	txbuff[index++] = (db_int32s & 0x0000FF00)>>8;
	txbuff[index++] = (db_int32s & 0x000000FF);

	txbuff[index++] = ACUDevInfo.ACUCurrSta.dev_move_sta;

	txbuff[index++] = ACUDevInfo.ACUCurrSta.dev_admin_sta;

	txbuff[index++] = ACUDevInfo.ACUCurrSta.dev_work_sta;

	txbuff[index++] = ACUDevInfo.ACUCurrSta.dev_alarm_flag;

	txbuff[index++] = ACUDevInfo.ACUCurrSta.dev_alarm_reflash_flag[0];

	txbuff[index++] = ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode;

	txbuff[index++] = ACUDevInfo.ACUCurrSta.dev_param_reflash_flag[0];

	db_int32u = (INT32U)ACUDevInfo.ACUCurrSta.AGC_max_mv;
	txbuff[index++] = (db_int32u & 0xFF000000)>>24;
	txbuff[index++] =  (db_int32u & 0x00FF0000)>>16;
	txbuff[index++] =  (db_int32u & 0x0000FF00)>>8;
	txbuff[index++] =  (db_int32u & 0x000000FF);


	*txbufflen = index;

	txbuff[0] = ((cmd_id | 0x8000) & 0xFF00)>>8;
	txbuff[1] = ((cmd_id | 0x8000) & 0x00FF);
	txbuff[2] = *txbufflen - 3;

	ACUDevInfo.ACUCurrSta.dev_alarm_reflash_flag[0] = 0x00;
	ACUDevInfo.ACUCurrSta.dev_param_reflash_flag[0] = 0x00;
	
	return SUCC;
}



/***********************************************************
**name:	APP_PCCOMM_DevAdminQue
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_DevAdminQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
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
**name:	APP_PCCOMM_AimAtSatParamQue
**describe:  追星参数查询
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_AimAtSatParamQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id,i;
	INT8U index = 3;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];
	INT32U AimAtSat_Limit_param = 0;

	strSatParam 			DelSatParam;

	cmd_id = (rxbuff[0]<<8)+rxbuff[1];
	

	if(cmd_id != AimAtSatParamQueID )
		return FAIL;


	txbuff[index++] = (ACUDevInfo.ACUParam.SeekSatParam.AimAtSat_Limit_mv & 0xFF000000)>>24;
	txbuff[index++] = (ACUDevInfo.ACUParam.SeekSatParam.AimAtSat_Limit_mv & 0x00FF0000)>>16;
	txbuff[index++] = (ACUDevInfo.ACUParam.SeekSatParam.AimAtSat_Limit_mv & 0x0000FF00)>>8;
	txbuff[index++] = (ACUDevInfo.ACUParam.SeekSatParam.AimAtSat_Limit_mv & 0x000000FF);

	*txbufflen = index;

	txbuff[0] = ((cmd_id | 0x8000) & 0xFF00)>>8;
	txbuff[1] = ((cmd_id | 0x8000) & 0x00FF);
	txbuff[2] = *txbufflen - 3;

	
	// 存储操作日志到flash中去
	{
		;
	}

	return SUCC;
}

/***********************************************************
**name:	APP_PCCOMM_DevAlarmQue
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_DevAlarmQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id;
	INT8U index = 3;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];

	INT32U degree_int32u;
	INT32U	db_int32u;
	INT32U len = 0;

	cmd_id = (rxbuff[0]<<8)+rxbuff[1];

	if( cmd_id != CurrWarnStaQueID )
		return FAIL;

	APP_MONITOR_AlarmStaCodeDeal(&txbuff[index],&len);

	txbuff[0] = ((cmd_id | 0x8000) & 0xFF00)>>8;
	txbuff[1] = ((cmd_id | 0x8000) & 0x00FF);
	txbuff[2] = len ;

	*txbufflen = len+3;

	ACUDevInfo.ACUCurrSta.dev_alarm_reflash_flag[0] = 0;
	
	return SUCC;
}



/***********************************************************
**name:	APP_PCCOMM_LocalParamQue
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_OptLogQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id;
	INT32U index = 0,index1;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];
	INT32U log_cnt = 0;
	INT32U i;
	INT32U que_log_num;
	INT8U logbuff[256]={0},len,check_sum;
	strLogInfo scanOptLog;

	cmd_id = (rxbuff[0]<<8)+rxbuff[1];

	if( cmd_id !=  OperationLogQueID)
		return FAIL;

	que_log_num = (rxbuff[3]<<24)+(rxbuff[4]<<16)+(rxbuff[5]<<8)+rxbuff[6];

	if( que_log_num > OptLogInfo.log_total_num )
		que_log_num = OptLogInfo.log_total_num;

	memcpy(&scanOptLog,&OptLogInfo,sizeof( OptLogInfo )); 

	for(i=0;i<que_log_num;)
	{
		if( APP_LOG_Read1Log(&scanOptLog,logbuff,&len) == SUCC)
		{
			index1 = index;

			PCCOMMTxBuff[index++] = 0xBC;
			PCCOMMTxBuff[index++] = len-15+3;
			PCCOMMTxBuff[index++] = ((OperationLogQueID | 0x8000) & 0xFF00)>>8;
			PCCOMMTxBuff[index++] = ((OperationLogQueID | 0x8000) & 0x00FF);
			PCCOMMTxBuff[index++] = len-15;
			memcpy(&PCCOMMTxBuff[index],&logbuff[5],len-15);
			index += len-15;
			check_sum = TOOL_CheckSum8BitGet(&PCCOMMTxBuff[index1],index-index1);
			PCCOMMTxBuff[index++] = check_sum;
			PCCOMMTxBuff[index++] = 0xCB;
			i++;

			scanOptLog.currlog_addr = (logbuff[len-10]<<24) + (logbuff[len-9]<<16) +(logbuff[len-8]<<8) +logbuff[len-7];
			scanOptLog.currlog_size = (logbuff[len-6]<<24) + (logbuff[len-5]<<16) +(logbuff[len-4]<<8) +logbuff[len-3];
		}
		else
			break;

		if(index >= 700)
		{
			PCCOMMTxBuffLen = index;
			APP_UDP_Sendto(PCCOMMTxBuff,PCCOMMTxBuffLen);
			index = 0;
		}
	}
	if( index > 0 )
	{
		PCCOMMTxBuffLen = index;
		APP_UDP_Sendto(PCCOMMTxBuff,PCCOMMTxBuffLen);
		index = 0;
	}
	index = 0;
	txbuff[index++] = ((cmd_id | 0x8000) & 0xFF00)>>8;
	txbuff[index++] = ((cmd_id | 0x8000) & 0x00FF);
	txbuff[index++] = 32;

	memset(&txbuff[index],0xFF,32);
	index += 32;

	*txbufflen = index;
	return SUCC;

}

/***********************************************************
**name:	APP_PCCOMM_LocalParamQue
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_WarnLogQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
{
	INT16U cmd_id;
	INT32U index = 0,index1;
	INT8U cmdlen = rxbuff[2];
	INT8U *pbuff=&rxbuff[3];
	INT32U log_cnt = 0;
	INT32U i;
	INT32U que_log_num;
	INT8U logbuff[256]={0},len,check_sum;
	strLogInfo scanWarnLog;

	cmd_id = (rxbuff[0]<<8)+rxbuff[1];

	if( cmd_id !=  WarnLogQueID)
		return FAIL;

	que_log_num = (rxbuff[3]<<24)+(rxbuff[4]<<16)+(rxbuff[5]<<8)+rxbuff[6];

	if( que_log_num > WarnLogInfo.log_total_num )
		que_log_num = WarnLogInfo.log_total_num;

	memcpy(&scanWarnLog,&WarnLogInfo,sizeof( WarnLogInfo )); 

	for(i=0;i<que_log_num;)
	{
		if( APP_LOG_Read1Log(&scanWarnLog,logbuff,&len) == SUCC)
		{
			index1 = index;

			PCCOMMTxBuff[index++] = 0xBC;
			PCCOMMTxBuff[index++] = len-15+3;
			PCCOMMTxBuff[index++] = ((WarnLogQueID | 0x8000) & 0xFF00)>>8;
			PCCOMMTxBuff[index++] = ((WarnLogQueID | 0x8000) & 0x00FF);
			PCCOMMTxBuff[index++] = len-15;
			memcpy(&PCCOMMTxBuff[index],&logbuff[5],len-15);
			index += len-15;
			check_sum = TOOL_CheckSum8BitGet(&PCCOMMTxBuff[index1],index-index1);
			PCCOMMTxBuff[index++] = check_sum;
			PCCOMMTxBuff[index++] = 0xCB;
			i++;

			scanWarnLog.currlog_addr = (logbuff[len-10]<<24) + (logbuff[len-9]<<16) +(logbuff[len-8]<<8) +logbuff[len-7];
			scanWarnLog.currlog_size = (logbuff[len-6]<<24) + (logbuff[len-5]<<16) +(logbuff[len-4]<<8) +logbuff[len-3];
		}
		else
			break;

		if(index >= 700)
		{
			PCCOMMTxBuffLen = index;
			APP_UDP_Sendto(PCCOMMTxBuff,PCCOMMTxBuffLen);
			index = 0;
		}
	}
	if( index > 0 )
	{
		PCCOMMTxBuffLen = index;
		APP_UDP_Sendto(PCCOMMTxBuff,PCCOMMTxBuffLen);
		index = 0;
		
	}
	index = 0;
	txbuff[index++] = ((cmd_id | 0x8000) & 0xFF00)>>8;
	txbuff[index++] = ((cmd_id | 0x8000) & 0x00FF);
	txbuff[index++] = 32;

	memset(&txbuff[index],0xFF,32);
	index += 32;

	*txbufflen = index;
	return SUCC;

}

/***********************************************************
**name:	APP_PCCOMM_VersionQue
**describe:   版本查询
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PCCOMM_VersionQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
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


