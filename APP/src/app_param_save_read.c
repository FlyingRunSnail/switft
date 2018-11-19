


#define APP_PARAM_DEF

#include <app_param_save_read.h>
#include <devdef.h>
#include <math.h>
#include <hw_eeprom.h>

INT8U EEPROM_COMM_err_cnt = 0;
/***********************************************************
**name:	APP_PARAM_Init
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void APP_PARAM_Init(void)
{
	INT16U i,sat_num=0;
	INT32U beacon_freq;
	INT8U readbuff[100];
	FP32	 	degree;
	INT32U  data;
	INT32S  degree_data;

	strSatParam SatParam;
	strHandLocalPositionParam HandLocalPositionParam;

	

	BSP_Delay_ms(10);

	DATETIME_ReadFromRtcIC(&ACUDevInfo.SysDateTime);

	// 获取电子码盘与天线角度的映射关系
	APP_PARAM_ElecCode_Map_AntennaDegree_Read(&ACUDevInfo.DeviceInfo.AllElecCoderMapAntenna);
	if(	(ACUDevInfo.DeviceInfo.AllElecCoderMapAntenna.SatPolarType != 0x00)&&
		(ACUDevInfo.DeviceInfo.AllElecCoderMapAntenna.SatPolarType != 0x01) && 
		(ACUDevInfo.DeviceInfo.AllElecCoderMapAntenna.SatPolarType != 0x02))
		ACUDevInfo.DeviceInfo.AllElecCoderMapAntenna.SatPolarType = 0x00;

	// 卫星相关数据
	APP_PARAM_SatTotalNum_Read(&ACUDevInfo.SATList.Sat_Total_num);
	if( ACUDevInfo.SATList.Sat_Total_num > SAT_MAX_NUM )
		ACUDevInfo.SATList.Sat_Total_num = 0x00;

	//获取当前卫星参数
	APP_PARAM_CurrSatParam_Read(&ACUDevInfo.ACUParam.CurrSatParam);

	//APP_PARAM_SatCurrID_Read(&ACUDevInfo.ACUParam.CurrSatParam.id);
	//if( ACUDevInfo.ACUParam.CurrSatParam.id >= SAT_MAX_NUM )
	//	ACUDevInfo.ACUParam.CurrSatParam.id = 0x00;
	
	sat_num = ACUDevInfo.SATList.Sat_Total_num;
	for(i=0;i<SAT_MAX_NUM;i++)
	{
		if( sat_num == 0x00 )
			break;

		SatParam.id = i+1;
		if(APP_PARAM_SatParam_Read(&SatParam) == FAIL)
			continue;
		if( ((SatParam.id == ACUDevInfo.ACUParam.CurrSatParam.id ) && ( ACUDevInfo.ACUParam.CurrSatParam.id > 0)) ||( ACUDevInfo.SATList.Sat_Total_num == 1))
		{
			memcpy(&ACUDevInfo.ACUParam.CurrSatParam,&SatParam,sizeof( SatParam ));
		}

		ACUDevInfo.SATList.Sat_ID[i] = SatParam.id;
		if( ACUDevInfo.ACUParam.CurrSatParam.id == 0x00)
		{
			ACUDevInfo.ACUParam.CurrSatParam.id = ACUDevInfo.SATList.Sat_ID[i];
			APP_PARAM_SatCurrID_Save(ACUDevInfo.ACUParam.CurrSatParam.id);
		}
		sat_num--;
	}

	if( sat_num > 0)
	{
		ACUDevInfo.SATList.Sat_Total_num -= sat_num;
		APP_PARAM_SatTotalNum_Save(ACUDevInfo.SATList.Sat_Total_num);
	}

//   	APP_PARAM_SatTotalNum_Save(0x0000);

//  	ACUDevInfo.DeviceInfo.NetWork.ip_addr = 0x3B1001C6;
//  	ACUDevInfo.DeviceInfo.NetWork.mask= 0x00FFFFFF;
//  	ACUDevInfo.DeviceInfo.NetWork.gate = 0x011001C6;
//  	APP_PARAM_NetWork_Save(&ACUDevInfo.DeviceInfo.NetWork);
	// 天线参数
	APP_PARAM_AntennaParam_Read(&ACUDevInfo.ACUParam.AntennaParam);

	APP_PARAM_AimAtSatParamLimitmv_Read(&ACUDevInfo.ACUParam.SeekSatParam);

	if( ACUDevInfo.ACUParam.AntennaParam.lo_freq >= 80000000)
	{
		ACUDevInfo.ACUParam.SeekSatParam.ScanRange_param[0] = 2;
		ACUDevInfo.ACUParam.SeekSatParam.ScanRange_param[1] = 5;
	}
	else
	{
		ACUDevInfo.ACUParam.SeekSatParam.ScanRange_param[0] = 5;
		ACUDevInfo.ACUParam.SeekSatParam.ScanRange_param[1] = 8;
	}

	// 天线 转动方向与电机转动方向
	APP_PARAM_AntennaDir_Map_MotorDir_Read(&ACUDevInfo.ACUParam.AntennaParam);
	

	// 天线软限位
	APP_PARAM_HardWareLimit_Read(0x01, 0x10,&data);
	if( data == 0x00 )
	{
		ACUDevInfo.ACUParam.AntennaParam.motor_position_start = 110;
	}
	else
	{
		APP_ElecEncoder_Data2DegreeFloat(0x01,data,&degree);
		APP_MONITOR_LocallAntennaAdjustDegreeDataGet(0x01,&degree,&ACUDevInfo.ACUParam.AntennaParam.motor_position_start);
		degree_data = ceil(ACUDevInfo.ACUParam.AntennaParam.motor_position_start + 3);
		ACUDevInfo.ACUParam.AntennaParam.motor_position_start = degree_data;

		//ACUDevInfo.ACUParam.AntennaParam.motor_position_start = 110;
	}	
	APP_PARAM_HardWareLimit_Read(0x01, 0x11,&data);
	if( data == 0x00 )
	{
		ACUDevInfo.ACUParam.AntennaParam.motor_position_end = 260;
	}
	else
	{
		APP_ElecEncoder_Data2DegreeFloat(0x01,data,&degree);
		APP_MONITOR_LocallAntennaAdjustDegreeDataGet(0x01,&degree,&ACUDevInfo.ACUParam.AntennaParam.motor_position_end);
		degree_data = floor(ACUDevInfo.ACUParam.AntennaParam.motor_position_end - 3);	
		ACUDevInfo.ACUParam.AntennaParam.motor_position_end = degree_data;
	
		//ACUDevInfo.ACUParam.AntennaParam.motor_position_end = 260;
	}
	
	APP_PARAM_HardWareLimit_Read(0x02, 0x20,&data);
	if( data == 0x00 )
	{
		ACUDevInfo.ACUParam.AntennaParam.motor_cabrage_start = 5;
	}
	else
	{
		APP_ElecEncoder_Data2DegreeFloat(0x02,data,&degree);
		APP_MONITOR_LocallAntennaAdjustDegreeDataGet(0x02,&degree,&ACUDevInfo.ACUParam.AntennaParam.motor_cabrage_start);
		degree_data = ceil(ACUDevInfo.ACUParam.AntennaParam.motor_cabrage_start + 3);
		ACUDevInfo.ACUParam.AntennaParam.motor_cabrage_start	= degree_data;
	
		//ACUDevInfo.ACUParam.AntennaParam.motor_cabrage_start = 5;
	}
	
	APP_PARAM_HardWareLimit_Read(0x02, 0x21,&data);
	if( data == 0x00 )
	{
		ACUDevInfo.ACUParam.AntennaParam.motor_cabrage_end = 85;
	}
	else
	{
		APP_ElecEncoder_Data2DegreeFloat(0x02,data,&degree);
		APP_MONITOR_LocallAntennaAdjustDegreeDataGet(0x02,&degree,&ACUDevInfo.ACUParam.AntennaParam.motor_cabrage_end);
		degree_data = floor(ACUDevInfo.ACUParam.AntennaParam.motor_cabrage_end - 3);	
		ACUDevInfo.ACUParam.AntennaParam.motor_cabrage_end = degree_data;	
	
		//ACUDevInfo.ACUParam.AntennaParam.motor_cabrage_end = 85;
	}
	
	APP_PARAM_HardWareLimit_Read(0x03, 0x30,&data);
	
	if( data == 0x00 )
	{
		ACUDevInfo.ACUParam.AntennaParam.motor_polar_end = 90;
	}
	else
	{
		APP_ElecEncoder_Data2DegreeFloat(0x03,data,&degree);
		APP_MONITOR_LocallAntennaAdjustDegreeDataGet(0x03,&degree,&ACUDevInfo.ACUParam.AntennaParam.motor_polar_end);
		degree_data = floor(ACUDevInfo.ACUParam.AntennaParam.motor_polar_end - 3);	
		ACUDevInfo.ACUParam.AntennaParam.motor_polar_end = degree_data;	
	
		//ACUDevInfo.ACUParam.AntennaParam.motor_polar_end = 90;
	}
	
	APP_PARAM_HardWareLimit_Read(0x03, 0x31,&data);
	if( data == 0x00 )
	{
		ACUDevInfo.ACUParam.AntennaParam.motor_polar_start = -90;
	}
	else
	{
		APP_ElecEncoder_Data2DegreeFloat(0x03,data,&degree);
		APP_MONITOR_LocallAntennaAdjustDegreeDataGet(0x03,&degree,&ACUDevInfo.ACUParam.AntennaParam.motor_polar_start);
		degree_data = ceil(ACUDevInfo.ACUParam.AntennaParam.motor_polar_start + 3);
		ACUDevInfo.ACUParam.AntennaParam.motor_polar_start	= degree_data;
	
		//ACUDevInfo.ACUParam.AntennaParam.motor_polar_start = -90;
	}
// 	if( ACUDevInfo.DeviceInfo.AllElecCoderMapAntenna.SatPolarType != ACUDevInfo.ACUParam.CurrSatParam.polar_type )
// 	{
// 			if(ACUDevInfo.ACUParam.CurrSatParam.polar_type == 'Y')
// 			{
// 				ACUDevInfo.ACUParam.AntennaParam.motor_polar_end += 90;
// 				ACUDevInfo.ACUParam.AntennaParam.motor_polar_start += 90;
// 			}
// 			else
// 			{
// 				ACUDevInfo.ACUParam.AntennaParam.motor_polar_end -= 90;
// 				ACUDevInfo.ACUParam.AntennaParam.motor_polar_start -= 90;
// 			}
// 	}
	
	

	// 本地参数

	//  默认是GPS 数据
	ACUDevInfo.ACUParam.LocalPositionParam.data_src_flag = 0x00;
	//APP_PARAM_LocallParamSrc_Read(&ACUDevInfo.ACUParam.LocalPositionParam.data_src_flag);
	//if( ACUDevInfo.ACUParam.LocalPositionParam.data_src_flag == 0x01 )
	{
		//  但是系统仍旧先从之前存储的数据作为本地参数信息
		APP_PARAM_LocallHandParam_Read(&ACUDevInfo.DeviceInfo.HandLocalPositionParam);
		ACUDevInfo.ACUParam.LocalPositionParam.lon_flag = ACUDevInfo.DeviceInfo.HandLocalPositionParam.lon_flag;
		ACUDevInfo.ACUParam.LocalPositionParam.lon = ACUDevInfo.DeviceInfo.HandLocalPositionParam.lon;
		ACUDevInfo.ACUParam.LocalPositionParam.lat_flag = ACUDevInfo.DeviceInfo.HandLocalPositionParam.lat_flag;
		ACUDevInfo.ACUParam.LocalPositionParam.lat = ACUDevInfo.DeviceInfo.HandLocalPositionParam.lat;
	}

	// 理论角计算
	APP_MONITOR_AimDegree_Get();

	// 信标频率配置
	beacon_freq = ACUDevInfo.ACUParam.CurrSatParam.freq > ACUDevInfo.ACUParam.AntennaParam.lo_freq?
				(ACUDevInfo.ACUParam.CurrSatParam.freq -  ACUDevInfo.ACUParam.AntennaParam.lo_freq):
				( ACUDevInfo.ACUParam.AntennaParam.lo_freq - ACUDevInfo.ACUParam.CurrSatParam.freq);

		//此处设置信标机信标频率
		{
			APP_BEACON_QueAGC();
			BSP_Delay_ms(50);
			APP_BEACON_SetFreq(beacon_freq);

			// 根据以往经验，设置信标机频率，需要至少200毫秒才能设置成功
			//因此此处应该加200毫秒的延时
			BSP_Delay_ms(200);
		}


	
	
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
	

	// 天线记录
	APP_PARAM_AntennaRecord_TotalNum_Read(&ACUDevInfo.AntennaRecordList.record_Total_num);
	sat_num = 0;
	for(i=0;i<ANTENNA_RECORD_MAX_NUM;i++)
	{
		APP_PARAM_AntennaRecord_Read(i+1,readbuff);
		if( readbuff[0] == i+1)
		{
			ACUDevInfo.AntennaRecordList.record_ID[i] = i+1;
			sat_num++;
		}
	}
	if( sat_num != ACUDevInfo.AntennaRecordList.record_Total_num )
	{
		ACUDevInfo.AntennaRecordList.record_Total_num = sat_num;
		APP_PARAM_AntennaRecord_TotalNum_Save(ACUDevInfo.AntennaRecordList.record_Total_num);
	}
	
	APP_PARAM_HistoryMaxMv_Read(&ACUDevInfo.ACUCurrSta.history_max_mv);
	APP_PARAM_AimAtSatParamLimitmv_Read(&ACUDevInfo.ACUParam.SeekSatParam);
	if( (ACUDevInfo.ACUParam.SeekSatParam.AimAtSat_Limit_mv < 1000 ) ||( ACUDevInfo.ACUParam.SeekSatParam.AimAtSat_Limit_mv > 3000 ))
		ACUDevInfo.ACUParam.SeekSatParam.AimAtSat_Limit_mv = 1000;

	
	ACUDevInfo.ACUCurrSta.dev_param_reflash_flag[0] = 1;
	ACUDevInfo.ACUCurrSta.dev_param_reflash_flag[1] = 1;
	
	
	if( APP_PARAM_EthRelinkResetMCUFlag_Read()>0)
	{
		APP_PARAM_AimAtSatMode_Read(&ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode);
		APP_PARAM_AimAtSatWorkSta_Read(&ACUDevInfo.ACUCurrSta.dev_work_sta);
	}
	else
	{
		ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode = 0x00;
		ACUDevInfo.ACUCurrSta.dev_work_sta = 0x00;
	}

	memcpy(ACUDevInfo.Version[0].softver,"EMS V1.01.B030  ",16);
	memcpy(ACUDevInfo.Version[0].hardver,"HW V1.01.B010   ",16);
}

/***********************************************************
**name:	APP_PARAM_Save
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_Save(INT16U addr,	INT8U *savebuff, INT16U len)
{
	INT8U checkbuff[80] = {0};
	INT32U checkbufflen=len,i=0,write_len=len,son_len;
	INT16U write_addr = addr;


	if( HW_EEPROM_WriteBuffer(addr, &savebuff[i], len) == FAIL)
	{
		EEPROM_COMM_err_cnt++;
		if( EEPROM_COMM_err_cnt > 10 )
			ACUDevInfo.DeviceInfo.EEPROMInfo.link_stat = COMM_UNLINK;
		return FAIL;
	}

	BSP_Delay_ms(5);

// 	if( APP_PARAM_Read(addr,	checkbuff, checkbufflen) == FAIL)
	if( HW_EEPROM_ReadBuffer(addr, checkbuff, &checkbufflen) == FAIL)
	{
		EEPROM_COMM_err_cnt++;
		if( EEPROM_COMM_err_cnt > 10 )
			ACUDevInfo.DeviceInfo.EEPROMInfo.link_stat = COMM_UNLINK;
		return FAIL;
	}
	BSP_Delay_ms(2);

	for(i=0;i<len;i++)
	{
		if( savebuff[i] != checkbuff[i] )
		{
			EEPROM_COMM_err_cnt++;
			if( EEPROM_COMM_err_cnt > 10 )
				ACUDevInfo.DeviceInfo.EEPROMInfo.link_stat = COMM_UNLINK;
			return FAIL;
		}
	}

	EEPROM_COMM_err_cnt=0;
	return SUCC;
}

/***********************************************************
**name:	APP_PARAM_Read
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_Read(INT16U addr,	INT8U *Readbuff, INT16U len)
{
	INT32U readlen = len;
	INT32U sun_len1,sun_len2;
	INT32U index = 0;
	INT16U a_addr = addr;

	while( readlen )
	{
		if( readlen > 256 )
		{
			sun_len1 = 256;
			sun_len2 = 256;
		}
		else
		{
			sun_len1 = readlen;
			sun_len2 = readlen;
		}
	
		if( HW_EEPROM_ReadBuffer(a_addr, &Readbuff[index], &sun_len1) == FAIL)
		{
			EEPROM_COMM_err_cnt++;
			if( EEPROM_COMM_err_cnt > 10 )
				ACUDevInfo.DeviceInfo.EEPROMInfo.link_stat = COMM_UNLINK;
			return FAIL;
		}
		a_addr += sun_len2;
		index += sun_len2;
		readlen -= sun_len2;
		BSP_Delay_ms(1);
	}
	EEPROM_COMM_err_cnt=0;
	ACUDevInfo.DeviceInfo.EEPROMInfo.link_stat = COMM_LINK;

	BSP_Delay_ms(2);
	
	return SUCC;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  存储数据


/***********************************************************
**name:	APP_PARAM_DevPowerOnSysDateTime_Save
**describe:   设备开机时间存储
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_DevPowerOnSysDateTime_Save(strSysDateTime	 *SysDateTime)
{
	INT8U savebuff[32] = {0};
	INT16U addr;
	INT8U index = 0;
	INT32U temp;

		
	addr = APP_EEPROM_Dev_PowerOn_SysDateTime_ADDR;

	savebuff[index++] = (SysDateTime->year & 0xFF00)>>8;
	savebuff[index++] = SysDateTime->year & 0x00FF;

	savebuff[index++] = SysDateTime->month;
	savebuff[index++] = SysDateTime->day;
	savebuff[index++] = SysDateTime->hour;
	savebuff[index++] = SysDateTime->min;
	savebuff[index++] = SysDateTime->senc;
	
	return(APP_PARAM_Save(addr,savebuff,7));
	
}


/***********************************************************
**name:	APP_PARAM_AntennaParam_Save
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_AntennaParam_Save(strAntennaParam *AntennaParam)
{
	INT8U savebuff[32] = {0};
	INT16U addr;
	INT8U maxlen =0;
	INT8U checkbuff[32] = {0};
	INT32U checkbufflen;
	INT32U degree_int32u;
	INT8U index = 0,i=0;

		
	addr = APP_EEPROM_Antenna_Param_ADDR;
	
	savebuff[index++] = AntennaParam->type;

	degree_int32u = AntennaParam->offset_angle;
	savebuff[index++] = (degree_int32u & 0xFF000000)>>24;
	savebuff[index++] = (degree_int32u & 0x00FF0000)>>16;
	savebuff[index++] = (degree_int32u & 0x0000FF00)>>8;
	savebuff[index++] = (degree_int32u & 0x000000FF);
	
	savebuff[index++] = AntennaParam->caliber;

	savebuff[index++] = (AntennaParam->lo_freq & 0xFF000000)>>24;
	savebuff[index++] = (AntennaParam->lo_freq & 0x00FF0000)>>16;
	savebuff[index++] = (AntennaParam->lo_freq & 0x0000FF00)>>8;
	savebuff[index++] = (AntennaParam->lo_freq & 0x000000FF);

	savebuff[index++] = AntennaParam->gain_range_flag;

	savebuff[index++] = AntennaParam->elec_coder_num;

	return(APP_PARAM_Save(addr,savebuff,index));
	
}

/***********************************************************
**name:	APP_PARAM_ElecCode_count_mode_Save
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_ElecCode_count_mode_Save(INT8U ElecCode_addr,INT8U count_mode)
{
	INT8U savebuff[32] = {0},index=0;
	INT16U addr;

	addr = APP_EEPROM_ELECCODER_COUNT_MODE_ADDR + ElecCode_addr - 1;
	savebuff[index++]	 = count_mode;
	return(APP_PARAM_Save(addr,savebuff,index));
}


/***********************************************************
**name:	APP_PARAM_ElecCode_Map_AntennaDegree_Save
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_ElecCode_Map_AntennaDegree_Save(strAllElecCoderMapAntenna *AllElecCoderMapAntenna)
{
	INT8U savebuff[32] = {0};
	INT16U addr;
	INT8U maxlen =0;
	INT8U checkbuff[32] = {0};
	INT32U checkbufflen;
	INT32U degree_int32u;
	INT32S degree_int32s;
	INT8U index = 0,i=0;

		
	addr = APP_EEPROM_ElecCode_Map_AntennaDegree_Addr;

	// 相关赋值处理
	// 方位
	degree_int32u = (INT32U)(AllElecCoderMapAntenna->ElecCoderMapAntenna[motor_position_id-1].Antenna_degree * 1000);
	savebuff[index++] = (degree_int32u & 0xFF000000)>>24;
	savebuff[index++] = (degree_int32u & 0x00FF0000)>>16;
	savebuff[index++] = (degree_int32u & 0x0000FF00)>>8;
	savebuff[index++] = (degree_int32u & 0x000000FF);

	degree_int32u = (INT32U)(AllElecCoderMapAntenna->ElecCoderMapAntenna[motor_position_id-1].ElecCoder_degree * 1000);
	savebuff[index++] = (degree_int32u & 0xFF000000)>>24;
	savebuff[index++] = (degree_int32u & 0x00FF0000)>>16;
	savebuff[index++] = (degree_int32u & 0x0000FF00)>>8;
	savebuff[index++] = (degree_int32u & 0x000000FF);

	savebuff[index++] = AllElecCoderMapAntenna->ElecCoderMapAntenna[motor_position_id-1].ElecCoder_count_mode;

	// 俯仰
	degree_int32u = (INT32U)(AllElecCoderMapAntenna->ElecCoderMapAntenna[motor_cabrage_id-1].Antenna_degree * 1000);
	savebuff[index++] = (degree_int32u & 0xFF000000)>>24;
	savebuff[index++] = (degree_int32u & 0x00FF0000)>>16;
	savebuff[index++] = (degree_int32u & 0x0000FF00)>>8;
	savebuff[index++] = (degree_int32u & 0x000000FF);

	degree_int32u = (INT32U)(AllElecCoderMapAntenna->ElecCoderMapAntenna[motor_cabrage_id-1].ElecCoder_degree*1000);
	savebuff[index++] = (degree_int32u & 0xFF000000)>>24;
	savebuff[index++] = (degree_int32u & 0x00FF0000)>>16;
	savebuff[index++] = (degree_int32u & 0x0000FF00)>>8;
	savebuff[index++] = (degree_int32u & 0x000000FF);

	savebuff[index++] = AllElecCoderMapAntenna->ElecCoderMapAntenna[motor_cabrage_id-1].ElecCoder_count_mode;

	// 极化
	degree_int32s  = (INT32S)(AllElecCoderMapAntenna->ElecCoderMapAntenna[motor_polar_id-1].Antenna_degree * 1000);
	savebuff[index++] = (degree_int32s & 0xFF000000)>>24;
	savebuff[index++] = (degree_int32s & 0x00FF0000)>>16;
	savebuff[index++] = (degree_int32s & 0x0000FF00)>>8;
	savebuff[index++] = (degree_int32s & 0x000000FF);

	degree_int32s= (INT32S)(AllElecCoderMapAntenna->ElecCoderMapAntenna[motor_polar_id-1].ElecCoder_degree*1000);
	savebuff[index++] = (degree_int32s & 0xFF000000)>>24;
	savebuff[index++] = (degree_int32s & 0x00FF0000)>>16;
	savebuff[index++] = (degree_int32s & 0x0000FF00)>>8;
	savebuff[index++] = (degree_int32s & 0x000000FF);

	savebuff[index++] = AllElecCoderMapAntenna->ElecCoderMapAntenna[motor_polar_id-1].ElecCoder_count_mode;
	
	savebuff[index++] = AllElecCoderMapAntenna->SatPolarType;
	 

	return(APP_PARAM_Save(addr,savebuff,index));
	
}




/***********************************************************
**name:	APP_PARAM_AntennaDir_Map_MotorDir_Save
**describe:
**input:			AntennaPart: 0x01 天线方位，0x02天线俯仰，0x03天线极化
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_AntennaDir_Map_MotorDir_Save(INT8U AntennaPart,strAntennaDirMapMotor *AntennaDirMapMotor)
{
	INT8U savebuff[32] = {0};
	INT16U addr;
	INT8U index = 0,i=0;

	addr = APP_EEPROM_AntennaDir_Map_MotorDir_ADDR + (AntennaPart-1)*2;

	savebuff[index++] = AntennaDirMapMotor->motor_dir[0];
	savebuff[index++] = AntennaDirMapMotor->motor_dir[1];

	return(APP_PARAM_Save(addr,savebuff,index));
	
}


/***********************************************************
**name:	APP_PARAM_LocallParamSrc_Save
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_LocallParamSrc_Save(INT8U src_flag)
{
	INT8U savebuff[32] = {0};
	INT16U addr;
	INT8U maxlen =0;
	INT8U checkbuff[32] = {0};
	INT32U checkbufflen;
	INT32U degree_int32u;
	INT8U index = 0,i=0;

		
	addr = APP_EEPROM_LocallParamSrc_ADDR;

	// 相关赋值处理
	savebuff[index++] = src_flag;
	

	return(APP_PARAM_Save(addr,savebuff,index));
	
}



/***********************************************************
**name:	APP_PARAM_LocallHandParam_Save
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_LocallHandParam_Save(strHandLocalPositionParam *HandLocalPositionParam)
{
	INT8U savebuff[32] = {0};
	INT16U addr;
	INT8U maxlen =0;
	INT8U checkbuff[32] = {0};
	INT32U checkbufflen;
	INT32U degree_int32u;
	INT8U index = 0,i=0;

	addr = APP_EEPROM_LocallParam_ADDR;

	// 相关赋值处理
	// 经度
	savebuff[index++] = HandLocalPositionParam->lon_flag;
	degree_int32u = (INT32U)(HandLocalPositionParam->lon * 1000);
	savebuff[index++] = (degree_int32u & 0xFF000000)>>24;
	savebuff[index++] = (degree_int32u & 0x00FF0000)>>16;
	savebuff[index++] = (degree_int32u & 0x0000FF00)>>8;
	savebuff[index++] = (degree_int32u & 0x000000FF);

	// 纬度
	savebuff[index++] = HandLocalPositionParam->lat_flag;
	degree_int32u = (INT32U)(HandLocalPositionParam->lat* 1000);
	savebuff[index++] = (degree_int32u & 0xFF000000)>>24;
	savebuff[index++] = (degree_int32u & 0x00FF0000)>>16;
	savebuff[index++] = (degree_int32u & 0x0000FF00)>>8;
	savebuff[index++] = (degree_int32u & 0x000000FF);

	// 高度
	savebuff[index++] = (HandLocalPositionParam->height & 0xFF000000)>>24;
	savebuff[index++] = (HandLocalPositionParam->height & 0x00FF0000)>>16;
	savebuff[index++] = (HandLocalPositionParam->height & 0x0000FF00)>>8;
	savebuff[index++] = (HandLocalPositionParam->height & 0x000000FF);
	
	return(APP_PARAM_Save(addr,savebuff,index));
}




/***********************************************************
**name:	APP_PARAM_DevSysDateTimeSrc_Save
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_DevSysDateTimeSrc_Save(INT8U src_flag)
{
	INT8U savebuff[32] = {0};
	INT16U addr;
	INT8U index = 0;

		
	addr = APP_EEPROM_DevSysDateTimeSrc_ADDR;

	// 相关赋值处理
	savebuff[index++] = src_flag;
	

	return(APP_PARAM_Save(addr,savebuff,index));
	
}


/***********************************************************
**name:	APP_PARAM_AimAtSatMode_Save
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_AimAtSatMode_Save(INT8U mode)
{
	INT8U savebuff[32] = {0};
	INT16U addr;
	INT8U index = 0;

		
	addr = APP_EEPROM_AimAtSatMode_ADDR;

	// 相关赋值处理
	savebuff[index++] = mode;

	
	

	return(APP_PARAM_Save(addr,savebuff,index));
	
}

/***********************************************************
**name:	APP_PARAM_AimAtSatWorkSta_Save
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_AimAtSatWorkSta_Save(INT8U sta)
{
	INT8U savebuff[32] = {0};
	INT16U addr;
	INT8U index = 0;

		
	addr = APP_EEPROM_AimAtSatWorkSta_ADDR;

	// 相关赋值处理
	savebuff[index++] = sta;

	
	

	return(APP_PARAM_Save(addr,savebuff,index));
	
}


/***********************************************************
**name:	APP_PARAM_NetWork_Save
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_NetWork_Save(strNetWork		*NetWork)
{
	INT8U savebuff[32] = {0};
	INT16U addr;
	INT8U index = 0;
	INT32U temp;

		
	addr = APP_EEPROM_NetWork_ADDR;

	// 相关赋值处理
	// ip
	temp = NetWork->ip_addr;
	savebuff[index++] = (temp & 0xFF000000)>>24;
	savebuff[index++] = (temp & 0x00FF0000)>>16;
	savebuff[index++] = (temp & 0x0000FF00)>>8;
	savebuff[index++] = (temp & 0x000000FF);

	// 子网掩码
	temp = NetWork->mask;
	savebuff[index++] = (temp & 0xFF000000)>>24;
	savebuff[index++] = (temp & 0x00FF0000)>>16;
	savebuff[index++] = (temp & 0x0000FF00)>>8;
	savebuff[index++] = (temp & 0x000000FF);

	// 网关
	temp = NetWork->gate;
	savebuff[index++] = (temp & 0xFF000000)>>24;
	savebuff[index++] = (temp & 0x00FF0000)>>16;
	savebuff[index++] = (temp & 0x0000FF00)>>8;
	savebuff[index++] = (temp & 0x000000FF);

	// 端口号
	//temp = NetWork->ip_port;
	//savebuff[index++] = (temp & 0xFF000000)>>24;
	//savebuff[index++] = (temp & 0x00FF0000)>>16;
	//savebuff[index++] = (temp & 0x0000FF00)>>8;
	//savebuff[index++] = (temp & 0x000000FF);
	

	return(APP_PARAM_Save(addr,savebuff,index));
	
}



/***********************************************************
**name:	APP_PARAM_MAC_Save
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_MAC_Save(strNetWork		*NetWork)
{
	INT8U savebuff[32] = {0};
	INT16U addr;
	INT8U index = 0;

		
	addr = APP_EEPROM_MAC_ADDR;

	// 相关赋值处理
	for(index=0; index<6;index++)
		savebuff[index] = NetWork->mac[index];

	return(APP_PARAM_Save(addr,savebuff,index));
	
}

/***********************************************************
**name:	APP_PARAM_PositionSoftLimit_Save
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_HardWareLimit_Save(INT8U antenna_part,INT8U antenna_limit_part,INT32U limit_data)
{
	INT8U savebuff[32] = {0};
	INT16U addr;
	INT8U index = 0;
	INT32U degree_int32u;


	addr = APP_EEPROM_PositionHardLimit_ADDR + (antenna_part-1)*16 + (antenna_limit_part & 0x0F)*4;

	savebuff[index++] =  (limit_data & 0xFF000000)>>24;
	savebuff[index++] =  (limit_data & 0x00FF0000)>>16;
	savebuff[index++] =  (limit_data & 0x0000FF00)>>8;
	savebuff[index++] =  (limit_data & 0x000000FF);

	return(APP_PARAM_Save(addr,savebuff,index));
	
}
#if 0
/***********************************************************
**name:	APP_PARAM_CabrageSoftLimit_Save
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_CabrageSoftLimit_Save(strAntennaParam		*AntennaParam)
{
	INT8U savebuff[32] = {0};
	INT16U addr;
	INT8U index = 0;
	INT32U degree_int32u;

		
	addr = APP_EEPROM_CabrageSoftLimit_ADDR;

	// 相关赋值处理
	// 起始位置
	degree_int32u = AntennaParam->motor_cabrage_start * 1000;
	savebuff[index++] = (degree_int32u & 0xFF000000)>>24;
	savebuff[index++] = (degree_int32u & 0x00FF0000)>>16;
	savebuff[index++] = (degree_int32u & 0x0000FF00)>>8;
	savebuff[index++] = (degree_int32u & 0x000000FF);

	// 结束位置
	degree_int32u = AntennaParam->motor_cabrage_end * 1000;
	savebuff[index++] = (degree_int32u & 0xFF000000)>>24;
	savebuff[index++] = (degree_int32u & 0x00FF0000)>>16;
	savebuff[index++] = (degree_int32u & 0x0000FF00)>>8;
	savebuff[index++] = (degree_int32u & 0x000000FF);

	return(APP_PARAM_Save(addr,savebuff,index));
	
}


/***********************************************************
**name:	APP_PARAM_PolarSoftLimit_Save
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_PolarSoftLimit_Save(strAntennaParam		*AntennaParam)
{
	INT8U savebuff[32] = {0};
	INT16U addr;
	INT8U index = 0;
	INT32S degree_int32s;

		
	addr = APP_EEPROM_PolarSoftLimit_ADDR;

	// 相关赋值处理
	// 起始位置
	degree_int32s = AntennaParam->motor_polar_start * 1000;
	savebuff[index++] = (degree_int32s & 0xFF000000)>>24;
	savebuff[index++] = (degree_int32s & 0x00FF0000)>>16;
	savebuff[index++] = (degree_int32s & 0x0000FF00)>>8;
	savebuff[index++] = (degree_int32s & 0x000000FF);

	// 结束位置
	degree_int32s = AntennaParam->motor_polar_end * 1000;
	savebuff[index++] = (degree_int32s & 0xFF000000)>>24;
	savebuff[index++] = (degree_int32s & 0x00FF0000)>>16;
	savebuff[index++] = (degree_int32s & 0x0000FF00)>>8;
	savebuff[index++] = (degree_int32s & 0x000000FF);

	return(APP_PARAM_Save(addr,savebuff,index));
	
}

#endif
/***********************************************************
**name:	APP_PARAM_AntennaRecord_TotalNum_Save
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_AntennaRecord_TotalNum_Save(INT32U total_num)
{
	INT8U savebuff[32] = {0};
	INT16U addr;
	INT8U index = 0;
	INT32U temp;

		
	addr = APP_EEPROM_AntennaRecord_TotalNum_ADDR;

	// 相关赋值处理
	savebuff[index++] = (total_num & 0xFF000000)>>24;
	savebuff[index++] = (total_num & 0x00FF0000)>>16;
	savebuff[index++] = (total_num & 0x0000FF00)>>8;
	savebuff[index++] = (total_num & 0x000000FF);
	return(APP_PARAM_Save(addr,savebuff,index));
	
}



/***********************************************************
**name:	APP_PARAM_AntennaRecord_Save
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_AntennaRecord_Save(INT8U id,INT8U  *buff, INT16U len)
{
	//INT8U savebuff[64] = {0};
	INT16U addr;
	INT8U index = 0;
	INT32U temp;

		
	addr = APP_EEPROM_AntennaRecord_Base_ADDR + (id-1)*APP_EEPROM_AntennaRecord_Offset_ADDR;

	// 相关赋值处理
	{
		;
	}
	return(APP_PARAM_Save(addr,buff,len));
	
}

/***********************************************************
**name:	APP_PARAM_AntennaRecord_AllClear
**describe:   清除所有的天线姿态记录
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_AntennaRecord_AllClear(void)
{
	INT8U savebuff[64] = {0};
	INT16U addr;
	INT8U index = 0,i;
	INT32U temp;

		
	addr = APP_EEPROM_AntennaRecord_Base_ADDR;

	for(i=0;i<ANTENNA_RECORD_MAX_NUM;i++)
	{
		APP_PARAM_Save(addr,savebuff,64);
		addr += APP_EEPROM_AntennaRecord_Offset_ADDR;
	}

	return SUCC;
}



/***********************************************************
**name:	APP_PARAM_OPTLog_TotalNum_Save
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_OPTLog_TotalNum_Save(INT32U total_num)
{
	INT8U savebuff[32] = {0};
	INT16U addr;
	INT8U index = 0;
	INT32U temp;

		
	addr = APP_EEPROM_OPTLog_TotalNum_ADDR;

	// 相关赋值处理
	savebuff[index++] = (total_num & 0xFF000000)>>24;
	savebuff[index++] = (total_num & 0x00FF0000)>>16;
	savebuff[index++] = (total_num & 0x0000FF00)>>8;
	savebuff[index++] = (total_num & 0x000000FF);
	return(APP_PARAM_Save(addr,savebuff,index));
	
}

/***********************************************************
**name:	APP_PARAM_OPTLog_CurrID_Save
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_OPTLog_CurrID_Save(INT32U id)
{
	INT8U savebuff[32] = {0};
	INT16U addr;
	INT8U index = 0;
	INT32U temp;

		
	addr = APP_EEPROM_OPTLog_CurrID_ADDR;

	// 相关赋值处理
	savebuff[index++] = (id & 0xFF000000)>>24;
	savebuff[index++] = (id & 0x00FF0000)>>16;
	savebuff[index++] = (id & 0x0000FF00)>>8;
	savebuff[index++] = (id & 0x000000FF);
	return(APP_PARAM_Save(addr,savebuff,index));
	
}


/***********************************************************
**name:	APP_PARAM_WarnLog_TotalNum_Save
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_WarnLog_TotalNum_Save(INT32U total_num)
{
	INT8U savebuff[32] = {0};
	INT16U addr;
	INT8U index = 0;
	INT32U temp;

		
	addr = APP_EEPROM_WarnLog_TotalNum_ADDR;

	// 相关赋值处理
	savebuff[index++] = (total_num & 0xFF000000)>>24;
	savebuff[index++] = (total_num & 0x00FF0000)>>16;
	savebuff[index++] = (total_num & 0x0000FF00)>>8;
	savebuff[index++] = (total_num & 0x000000FF);
	return(APP_PARAM_Save(addr,savebuff,index));
	
}

/***********************************************************
**name:	APP_PARAM_WarnLog_CurrID_Save
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_WarnLog_CurrID_Save(INT32U id)
{
	INT8U savebuff[32] = {0};
	INT16U addr;
	INT8U index = 0;
	INT32U temp;

		
	addr = APP_EEPROM_WarnLog_CurrID_ADDR;

	// 相关赋值处理
	savebuff[index++] = (id & 0xFF000000)>>24;
	savebuff[index++] = (id & 0x00FF0000)>>16;
	savebuff[index++] = (id & 0x0000FF00)>>8;
	savebuff[index++] = (id & 0x000000FF);
	return(APP_PARAM_Save(addr,savebuff,index));
	
}



/***********************************************************
**name:	APP_PARAM_SatTotalNum_Save
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_SatTotalNum_Save(INT16U total_num)
{
	INT8U savebuff[32] = {0};
	INT16U addr;
	INT8U index = 0;
	INT32U temp;

		
	addr = APP_EEPROM_Sat_TotalNum_ADDR;

	// 相关赋值处理
	savebuff[index++] = (total_num & 0xFF00)>>8;
	savebuff[index++] = (total_num & 0x00FF);
	return(APP_PARAM_Save(addr,savebuff,index));
	
}

/***********************************************************
**name:	APP_PARAM_SatCurrID_Save
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_SatCurrID_Save(INT8U curr_id)
{
	INT8U savebuff[32] = {0};
	INT16U addr;
	INT8U index = 0;
	INT32U temp;

		
	addr = APP_EEPROM_Sat_CurrID_ADDR;

	// 相关赋值处理
	savebuff[index++] = curr_id;
	return(APP_PARAM_Save(addr,savebuff,index));
	
}

/***********************************************************
**name:	APP_PARAM_SatCurrID_Save
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_CurrSatParam_Save(strSatParam *SatParam)
{
	INT8U savebuff[32] = {0};
	INT16U addr;
	INT8U maxlen =0;
	INT8U checkbuff[32] = {0};
	INT32U checkbufflen;
	INT32U degree_int32u;
	INT8U index = 0,i=0;

//	if( SatParam->id <= 0)
//		return FAIL;
		
	addr = APP_EEPROM_CurrSatParam_ADDR;
	
	savebuff[index++] = 0x00;

	memcpy(&savebuff[index],SatParam->name,16);
	index += 16;
	
	savebuff[index++] = SatParam->lon_flag;

	degree_int32u = (INT32U)(SatParam->lon * 1000);
	savebuff[index++] = (degree_int32u & 0xFF000000)>>24;
	savebuff[index++] = (degree_int32u & 0x00FF0000)>>16;
	savebuff[index++] = (degree_int32u & 0x0000FF00)>>8;
	savebuff[index++] = (degree_int32u & 0x000000FF);

	savebuff[index++] = (SatParam->freq & 0xFF000000)>>24;
	savebuff[index++] = (SatParam->freq & 0x00FF0000)>>16;
	savebuff[index++] = (SatParam->freq & 0x0000FF00)>>8;
	savebuff[index++] = (SatParam->freq & 0x000000FF);

	savebuff[index++] = SatParam->polar_type;

	return(APP_PARAM_Save(addr,savebuff,index));
	
}


/***********************************************************
**name:	APP_PARAM_Sat_Save
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_SatParam_Save(strSatParam *SatParam)
{
	INT8U savebuff[32] = {0};
	INT16U addr;
	INT8U maxlen =0;
	INT8U checkbuff[32] = {0};
	INT32U checkbufflen;
	INT32U degree_int32u;
	INT8U index = 0,i=0;

	if( SatParam->id <= 0)
		return FAIL;
		
	addr = (SatParam->id - 1)*APP_EEPROM_SatParam_OffSet_ADDR + APP_EEPROM_SatParam_Base_ADDR;
	
	savebuff[index++] = SatParam->id;

	memcpy(&savebuff[index],SatParam->name,16);
	index += 16;
	
	savebuff[index++] = SatParam->lon_flag;

	degree_int32u = (INT32U)(SatParam->lon * 1000);
	savebuff[index++] = (degree_int32u & 0xFF000000)>>24;
	savebuff[index++] = (degree_int32u & 0x00FF0000)>>16;
	savebuff[index++] = (degree_int32u & 0x0000FF00)>>8;
	savebuff[index++] = (degree_int32u & 0x000000FF);

	savebuff[index++] = (SatParam->freq & 0xFF000000)>>24;
	savebuff[index++] = (SatParam->freq & 0x00FF0000)>>16;
	savebuff[index++] = (SatParam->freq & 0x0000FF00)>>8;
	savebuff[index++] = (SatParam->freq & 0x000000FF);

	savebuff[index++] = SatParam->polar_type;

	return(APP_PARAM_Save(addr,savebuff,index));
}



/***********************************************************
**name:	APP_PARAM_EthRelinkResetMCUFlagSet_Save
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_EthRelinkResetMCUFlagSet_Save(void)
{
	INT8U savebuff[32] = {0};
	INT16U addr;
	INT8U maxlen =0;
	INT8U checkbuff[32] = {0};
	INT32U checkbufflen;
	INT32U degree_int32u;
	INT8U index = 0,i=0;

	
	addr = APP_EEPROM_Reset_Flag_ADDR;
	
	savebuff[index++] = 1;

	return(APP_PARAM_Save(addr,savebuff,index));
}

/***********************************************************
**name:	APP_PARAM_EthRelinkResetMCUFlag_Del
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_EthRelinkResetMCUFlag_Del(void)
{
	INT8U savebuff[32] = {0};
	INT16U addr;
	INT8U maxlen =0;
	INT8U checkbuff[32] = {0};
	INT32U checkbufflen;
	INT32U degree_int32u;
	INT8U index = 0,i=0;

	
	addr = APP_EEPROM_Reset_Flag_ADDR;
	
	savebuff[index++] = 0;

	return(APP_PARAM_Save(addr,savebuff,index));
}




/***********************************************************
**name:	APP_PARAM_Sat_Del
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_Sat_Del(strSatParam *SatParam)
{
	INT8U savebuff[32] = {0};
	INT16U addr;
	INT8U maxlen =0;
	INT8U checkbuff[32] = {0};
	INT32U checkbufflen;
	INT32U degree_int32u;
	INT8U index = 0,i=0;

	if( SatParam->id <= 0)
		return FAIL;
		
	addr = (SatParam->id - 1)*APP_EEPROM_SatParam_OffSet_ADDR + APP_EEPROM_SatParam_Base_ADDR;

	

	return(APP_PARAM_Save(addr,savebuff,1));
}



/***********************************************************
**name:	APP_PARAM_AimAtSatParamLimitmv_Save
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_AimAtSatParamLimitmv_Save(strSeekSatParam  *SeekSatParam)
{
	INT8U savebuff[32] = {0};
	INT16U addr;
	INT8U maxlen =0;
	INT8U checkbuff[32] = {0};
	INT32U checkbufflen;
	INT32U degree_int32u;
	INT8U index = 0,i=0;

		
	addr = APP_EEPROM_AimAtSatLimitMv_ADDR;

	savebuff[index++] = (SeekSatParam->AimAtSat_Limit_mv & 0xFF000000)>>24;
	savebuff[index++] = (SeekSatParam->AimAtSat_Limit_mv & 0x00FF0000)>>16;
	savebuff[index++] = (SeekSatParam->AimAtSat_Limit_mv & 0x0000FF00)>>8;
	savebuff[index++] = (SeekSatParam->AimAtSat_Limit_mv & 0x000000FF);
	

	return(APP_PARAM_Save(addr,savebuff,index));
}


/***********************************************************
**name:	APP_PARAM_HistoryMaxMv_Save
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_HistoryMaxMv_Save(FP32	*history_max_mv)
{
	INT8U savebuff[32] = {0};
	INT16U addr;
	INT8U maxlen =0;
	INT8U checkbuff[32] = {0};
	INT32U checkbufflen;
	INT32U degree_int32u;
	INT8U index = 0,i=0;

	addr  =	APP_EEPROM_HISTORY_MAX_MV_ADDR;

	 ACUDevInfo.ACUCurrSta.history_max_mv  = *history_max_mv;
	 ACUDevInfo.ACUCurrSta.AGC_max_mv = ACUDevInfo.ACUCurrSta.history_max_mv;
	

	savebuff[index++] = ((INT32U)history_max_mv & 0xFF000000)>>24;
	savebuff[index++] = ((INT32U)history_max_mv & 0x00FF0000)>>16;
	savebuff[index++] = ((INT32U)history_max_mv & 0x0000FF00)>>8;
	savebuff[index++] = ((INT32U)history_max_mv & 0x000000FF);

	return(APP_PARAM_Save(addr,savebuff,index));
}



////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  读取数据


/***********************************************************
**name:	APP_PARAM_AntennaParam_Read
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_AntennaParam_Read(strAntennaParam *AntennaParam)
{
	INT8U readbuff[32] = {0},len=12;
	INT16U addr;
	INT8U maxlen =0;
	INT8U checkbuff[32] = {0};
	INT32U checkbufflen;
	INT32U degree_int32u;
	INT8U index = 0,i=0;

		
	addr = APP_EEPROM_Antenna_Param_ADDR;

	if(APP_PARAM_Read(addr,readbuff,len) == FAIL)
		return FAIL;
	
	AntennaParam->type = readbuff[index++];

	degree_int32u = (readbuff[index] << 24) + (readbuff[index+1] << 16) + (readbuff[index+2] << 8) + readbuff[index+3];
	index += 4;
	AntennaParam->offset_angle = degree_int32u; 
	
	
	AntennaParam->caliber = readbuff[index++];

	AntennaParam->lo_freq = (readbuff[index] << 24) + (readbuff[index+1] << 16) + (readbuff[index+2] << 8) + readbuff[index+3];
	index += 4;

	AntennaParam->gain_range_flag = readbuff[index++];

	AntennaParam->elec_coder_num = readbuff[index++];

	if((AntennaParam->elec_coder_num < 2 ) ||( AntennaParam->elec_coder_num > 3))
		AntennaParam->elec_coder_num = 2;

	return SUCC;
	
}



/***********************************************************
**name:	APP_PARAM_ElecCode_Map_AntennaDegree_Read
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_ElecCode_Map_AntennaDegree_Read(strAllElecCoderMapAntenna *AllElecCoderMapAntenna)
{
	INT8U readbuff[32] = {0},len=28;
	INT16U addr;

	INT32U degree_int32u;
	INT32S degree_int32s;
	INT8U index = 0,i=0;

		
	addr = APP_EEPROM_ElecCode_Map_AntennaDegree_Addr;

	if(APP_PARAM_Read(addr,readbuff,len) == FAIL)
		return FAIL;

	// 相关赋值处理
	// 方位
	degree_int32u = (readbuff[index] << 24) + (readbuff[index+1] << 16) + (readbuff[index+2] << 8) + readbuff[index+3];
	index += 4;
	AllElecCoderMapAntenna->ElecCoderMapAntenna[motor_position_id-1].Antenna_degree = (FP32)degree_int32u  / 1000.0;

	(INT32U)degree_int32u = (readbuff[index] << 24) + (readbuff[index+1] << 16) + (readbuff[index+2] << 8) + readbuff[index+3];
	index += 4;
	AllElecCoderMapAntenna->ElecCoderMapAntenna[motor_position_id-1].ElecCoder_degree = (FP32)degree_int32u  / 1000.0;

	AllElecCoderMapAntenna->ElecCoderMapAntenna[motor_position_id-1].ElecCoder_count_mode = readbuff[index++];


	// 俯仰
	degree_int32u =  (readbuff[index] << 24) + (readbuff[index+1] << 16) + (readbuff[index+2] << 8) + readbuff[index+3];
	index += 4;
	AllElecCoderMapAntenna->ElecCoderMapAntenna[motor_cabrage_id-1].Antenna_degree = (FP32)degree_int32u  / 1000.0;

	(INT32U)degree_int32u =  (readbuff[index] << 24) + (readbuff[index+1] << 16) + (readbuff[index+2] << 8) + readbuff[index+3];
	index += 4;
	AllElecCoderMapAntenna->ElecCoderMapAntenna[motor_cabrage_id-1].ElecCoder_degree = (FP32)degree_int32u  / 1000.0;

	AllElecCoderMapAntenna->ElecCoderMapAntenna[motor_cabrage_id-1].ElecCoder_count_mode = readbuff[index++];

	// 极化
	degree_int32s =  (readbuff[index] << 24) + (readbuff[index+1] << 16) + (readbuff[index+2] << 8) + readbuff[index+3];
	index += 4;
	AllElecCoderMapAntenna->ElecCoderMapAntenna[motor_polar_id-1].Antenna_degree = (FP32)degree_int32s  / 1000.0;

	degree_int32s =  (readbuff[index] << 24) + (readbuff[index+1] << 16) + (readbuff[index+2] << 8) + readbuff[index+3];
	index += 4;
	AllElecCoderMapAntenna->ElecCoderMapAntenna[motor_polar_id-1].ElecCoder_degree = (FP32)degree_int32s  / 1000.0;

	AllElecCoderMapAntenna->ElecCoderMapAntenna[motor_polar_id-1].ElecCoder_count_mode = readbuff[index++];
	
	AllElecCoderMapAntenna->SatPolarType = readbuff[index++];
	

	return SUCC;
	
}


/***********************************************************
**name:	APP_PARAM_LocallParamSrc_Read
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_LocallParamSrc_Read(INT8U *src_flag)
{
	INT8U readbuff[32] = {0},len;
	INT16U addr;

	INT32U degree_int32u;
	INT8U index = 0,i=0;

		
	addr = APP_EEPROM_LocallParamSrc_ADDR;

	if(APP_PARAM_Read(addr,readbuff,1) == FAIL)
		return FAIL;

	// 相关赋值处理
	*src_flag =  readbuff[index++];
	

	return SUCC;
	
}


/***********************************************************
**name:	APP_PARAM_LocallHandParam_Read
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_LocallHandParam_Read(strHandLocalPositionParam *HandLocalPositionParam)
{
	INT8U readbuff[32] = {0},len;
	INT16U addr;

	INT32U degree_int32u;
	INT8U index = 0,i=0;

		
	addr = APP_EEPROM_LocallParam_ADDR;

	if(APP_PARAM_Read(addr,readbuff,14) == FAIL)
		return FAIL;


	// 相关赋值处理
	// 经度
	HandLocalPositionParam->lon_flag = readbuff[index++];
	(INT32U)degree_int32u =  (readbuff[index] << 24) + (readbuff[index+1] << 16) + (readbuff[index+2] << 8) + readbuff[index+3];
	index += 4;
	HandLocalPositionParam->lon = (FP32)degree_int32u / 1000;


	// 纬度
	HandLocalPositionParam->lat_flag = readbuff[index++];
	(INT32U)degree_int32u = (readbuff[index] << 24) + (readbuff[index+1] << 16) + (readbuff[index+2] << 8) + readbuff[index+3];
	index += 4;
	HandLocalPositionParam->lat = (FP32)degree_int32u / 1000;

	// 高度
	HandLocalPositionParam->height = (readbuff[index] << 24) + (readbuff[index+1] << 16) + (readbuff[index+2] << 8) + readbuff[index+3];
	index += 4;
	
	return SUCC;
}


/***********************************************************
**name:	APP_PARAM_DevSysDateTimeSrc_Save
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_DevSysDateTimeSrc_Read(INT8U *src_flag)
{
	INT8U readbuff[32] = {0},len;
	INT16U addr;

	INT32U degree_int32u;
	INT8U index = 0,i=0;

		
	addr = APP_EEPROM_DevSysDateTimeSrc_ADDR;

	if(APP_PARAM_Read(addr,readbuff,1) == FAIL)
		return FAIL;

	// 相关赋值处理
	*src_flag =  readbuff[index++];
	

	return SUCC;
	
}


/***********************************************************
**name:	APP_PARAM_AimAtSatMode_Read
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_AimAtSatMode_Read(INT8U *mode)
{
	INT8U readbuff[32] = {0},len;
	INT16U addr;

	INT32U degree_int32u;
	INT8U index = 0,i=0;

		
	addr = APP_EEPROM_AimAtSatMode_ADDR;

	if(APP_PARAM_Read(addr,readbuff,1) == FAIL)
		return FAIL;

	// 相关赋值处理
	*mode =  readbuff[index++];
	

	return SUCC;
}

/***********************************************************
**name:	APP_PARAM_AimAtSatMode_Read
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_AimAtSatWorkSta_Read(INT8U *sta)
{
	INT8U readbuff[32] = {0},len;
	INT16U addr;

	INT32U degree_int32u;
	INT8U index = 0,i=0;

		
	addr = APP_EEPROM_AimAtSatWorkSta_ADDR;

	if(APP_PARAM_Read(addr,readbuff,1) == FAIL)
		return FAIL;

	// 相关赋值处理
	*sta =  readbuff[index++];
	

	return SUCC;
}


/***********************************************************
**name:	APP_PARAM_NetWork_Read
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_NetWork_Read(strNetWork		*NetWork)
{
	INT8U readbuff[32] = {0},len;
	INT16U addr;

	INT8U index = 0,i=0;

		
	addr = APP_EEPROM_NetWork_ADDR;
	if(APP_PARAM_Read(addr,readbuff,12) == FAIL)
		return FAIL;

	// 相关赋值处理
	// ip
	 NetWork->ip_addr =   (readbuff[index] << 24) + (readbuff[index+1] << 16) + (readbuff[index+2] << 8) + readbuff[index+3];
	 index += 4;

	// 子网掩码
	NetWork->mask =   (readbuff[index] << 24) + (readbuff[index+1] << 16) + (readbuff[index+2] << 8) + readbuff[index+3];
	index += 4;

	// 网关
	 NetWork->gate =  (readbuff[index] << 24) + (readbuff[index+1] << 16) + (readbuff[index+2] << 8) + readbuff[index+3];
	 index += 4;

	// 端口号
	//NetWork->ip_port  = (readbuff[index] << 24) + (readbuff[index+1] << 16) + (readbuff[index+2] << 8) + readbuff[index+3];


	return SUCC;
	
}



/***********************************************************
**name:	APP_PARAM_MAC_Read
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_MAC_Read(strNetWork		*NetWork)
{
	INT8U readbuff[32] = {0},len = 6;
	INT16U addr;

	INT8U index = 0,i=0;

		
	addr = APP_EEPROM_MAC_ADDR;
	if(APP_PARAM_Read(addr,readbuff,len) == FAIL)
		return FAIL;

	// 相关赋值处理
	for(index=0; index<6;index++)
		NetWork->mac[index] = readbuff[index];

	return SUCC;
	
}


/***********************************************************
**name:	APP_PARAM_PositionSoftLimit_Read
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_ElecCode_count_mode_Read(INT8U ElecCode_addr,INT8U *count_mode)
{
	INT8U readbuff[32] = {0},len = 1;
	INT16U addr;

	INT8U index = 0,i=0;
	INT32S degree_int32s;
		
	addr = APP_EEPROM_ELECCODER_COUNT_MODE_ADDR + ElecCode_addr - 1;
	if(APP_PARAM_Read(addr,readbuff,len) == FAIL)
		return FAIL;

	*count_mode = readbuff[index++];
	if( (*count_mode != 0xE1) && ( *count_mode != 0xE2 ))
		*count_mode = 0xE1;

	
	return SUCC;
	
}


/***********************************************************
**name:	APP_PARAM_HardWareLimit_Read
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_HardWareLimit_Read(INT8U antenna_part,INT8U antenna_limit_part, INT32U *limit_data)
{
	INT8U readbuff[32] = {0},len = 4;
	INT16U addr;

	INT8U index = 0,i=0;
	INT32S degree_int32s;
		
	addr = APP_EEPROM_PositionHardLimit_ADDR + (antenna_part-1)*16 + (antenna_limit_part & 0x0F)*4;
	if(APP_PARAM_Read(addr,readbuff,len) == FAIL)
		return FAIL;

	// 相关赋值处理
	// 起始位置
	*limit_data =  (readbuff[index] << 24) + (readbuff[index+1] << 16) + (readbuff[index+2] << 8) + readbuff[index+3];

	return SUCC;
}
#if 0
/***********************************************************
**name:	APP_PARAM_PositionSoftLimit_Read
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_PositionSoftLimit_Read(strAntennaParam		*AntennaParam)
{
	INT8U readbuff[32] = {0},len = 8;
	INT16U addr;

	INT8U index = 0,i=0;
	INT32S degree_int32s;
		
	addr = APP_EEPROM_PositionSoftLimit_ADDR;
	if(APP_PARAM_Read(addr,readbuff,len) == FAIL)
		return FAIL;

	// 相关赋值处理
	// 起始位置
	(INT32S)degree_int32s =  (readbuff[index] << 24) + (readbuff[index+1] << 16) + (readbuff[index+2] << 8) + readbuff[index+3];
	index += 4;
	AntennaParam->motor_position_start = (FP32)degree_int32s / (FP32)1000;


	// 结束位置
	(INT32S)degree_int32s =  (readbuff[index] << 24) + (readbuff[index+1] << 16) + (readbuff[index+2] << 8) + readbuff[index+3];
	index += 4;
	AntennaParam->motor_position_end = (FP32)degree_int32s / (FP32)1000;

	return SUCC;
	
}

/***********************************************************
**name:	APP_PARAM_CabrageSoftLimit_Read
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_CabrageSoftLimit_Read(strAntennaParam		*AntennaParam)
{
	INT8U readbuff[32] = {0},len = 8;
	INT16U addr;

	INT8U index = 0,i=0;
	INT32S degree_int32s;
		
	addr = APP_EEPROM_CabrageSoftLimit_ADDR;
	if(APP_PARAM_Read(addr,readbuff,len) == FAIL)
		return FAIL;

	// 相关赋值处理
	// 起始位置
	(INT32S)degree_int32s =  (readbuff[index] << 24) + (readbuff[index+1] << 16) + (readbuff[index+2] << 8) + readbuff[index+3];
	index += 4;
	AntennaParam->motor_cabrage_start = (FP32)degree_int32s / (FP32)1000;


	// 结束位置
	(INT32S)degree_int32s = (readbuff[index] << 24) + (readbuff[index+1] << 16) + (readbuff[index+2] << 8) + readbuff[index+3];
	index += 4;
	AntennaParam->motor_cabrage_end = (FP32)degree_int32s / (FP32)1000;

	return SUCC;
	
}

/***********************************************************
**name:	APP_PARAM_PolarSoftLimit_Read
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_PolarSoftLimit_Read(strAntennaParam		*AntennaParam)
{
	INT8U readbuff[32] = {0},len = 8;
	INT16U addr;

	INT8U index = 0,i=0;
	INT32S degree_int32s;
		
	addr = APP_EEPROM_PolarSoftLimit_ADDR;
	if(APP_PARAM_Read(addr,readbuff,len) == FAIL)
		return FAIL;

	// 相关赋值处理
	// 起始位置
	(INT32S)degree_int32s =  (readbuff[index] << 24) + (readbuff[index+1] << 16) + (readbuff[index+2] << 8) + readbuff[index+3];
	index += 4;
	AntennaParam->motor_polar_start = (FP32)degree_int32s / (FP32)1000;


	// 结束位置
	(INT32S)degree_int32s =  (readbuff[index] << 24) + (readbuff[index+1] << 16) + (readbuff[index+2] << 8) + readbuff[index+3];
	index += 4;
	AntennaParam->motor_polar_end = (FP32)degree_int32s / (FP32)1000;

	return SUCC;
	
}

#endif
/***********************************************************
**name:	APP_PARAM_AntennaRecord_TotalNum_Read
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_AntennaRecord_TotalNum_Read(INT32U *total_num)
{
	INT8U readbuff[32] = {0},len = 4;
	INT16U addr;

	INT8U index = 0,i=0;
		
	addr = APP_EEPROM_AntennaRecord_TotalNum_ADDR;
	if(APP_PARAM_Read(addr,readbuff,len) == FAIL)
		return FAIL;

	*total_num = (readbuff[index] << 24) + (readbuff[index+1] << 16) + (readbuff[index+2] << 8) + readbuff[index+3];

	
	return SUCC;
	
}



/***********************************************************
**name:	APP_PARAM_AntennaRecord_Read
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_AntennaRecord_Read(INT8U id,	INT8U *p)
{
	INT8U readbuff[100] = {0},len = 80;
	INT16U addr;

	INT8U index = 0,i=0;
		
	addr = APP_EEPROM_AntennaRecord_Base_ADDR + (id -1)*APP_EEPROM_AntennaRecord_Offset_ADDR;
	if(APP_PARAM_Read(addr,p,len) == FAIL)
		return FAIL;

	
	return SUCC;
	
}

/***********************************************************
**name:	APP_PARAM_AntennaRecord_Read2Struct
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_AntennaRecord_Read2Struct(INT8U id,	strSatParam *SatParam, strAntennaParam *AntennaParam, FP32 *degree_AZ,FP32 *degree_EL,FP32 *degree_PL)
{
	INT8U readbuff[100] = {0},len = 80,index=8;
	INT16U addr;
	INT32U data_int32u;
	INT32S data_int32s;

		
	addr = APP_EEPROM_AntennaRecord_Base_ADDR + (id -1)*APP_EEPROM_AntennaRecord_Offset_ADDR;
	if(APP_PARAM_Read(addr,readbuff,len) == FAIL)
		return FAIL;

	// 卫星参数
	memcpy( SatParam->name,&readbuff[index],sizeof(SatParam->name));
	index += sizeof(SatParam->name);
	SatParam->lon_flag = readbuff[index++];
	data_int32u = (readbuff[index] << 24) + (readbuff[index+1] << 16) + (readbuff[index+2] << 8) + readbuff[index+3];
	SatParam->lon = (FP32)data_int32u / 1000.0;
	index += 4;
	SatParam->freq =  (readbuff[index] << 24) + (readbuff[index+1] << 16) + (readbuff[index+2] << 8) + readbuff[index+3];
	index += 4;
	SatParam->polar_type = readbuff[index++];

	// 天线参数
	AntennaParam->type = readbuff[index++];

	data_int32u = (readbuff[index] << 24) + (readbuff[index+1] << 16) + (readbuff[index+2] << 8) + readbuff[index+3];
	index += 4;
	AntennaParam->offset_angle = data_int32u; 
	
	
	AntennaParam->caliber = readbuff[index++];

	AntennaParam->lo_freq = (readbuff[index] << 24) + (readbuff[index+1] << 16) + (readbuff[index+2] << 8) + readbuff[index+3];
	index += 4;

	AntennaParam->gain_range_flag = readbuff[index++];

	AntennaParam->elec_coder_num = readbuff[index++];

	if((AntennaParam->elec_coder_num < 2 ) ||( AntennaParam->elec_coder_num > 3))
		AntennaParam->elec_coder_num = 2;

	// 存储电压值与DBM值的数据，暂时不取
	index += 4;
	index += 4;

	// 位置角度数
	(INT32U)data_int32u = (readbuff[index] << 24) + (readbuff[index+1] << 16) + (readbuff[index+2] << 8) + readbuff[index+3];
// 	AntennaParam->aim_position = ( (FP32)data_int32u) / 1000.0;
	*degree_AZ =( (FP32)data_int32u) / 1000.0;
	index += 4;

	(INT32U)data_int32u = (readbuff[index] << 24) + (readbuff[index+1] << 16) + (readbuff[index+2] << 8) + readbuff[index+3];
	*degree_EL = ( (FP32)data_int32u) / 1000.0;
// 	AntennaParam->aim_cabrage = ( (FP32)data_int32u) / 1000.0;
	index += 4;

	data_int32s = (readbuff[index] << 24) + (readbuff[index+1] << 16) + (readbuff[index+2] << 8) + readbuff[index+3];
	*degree_PL = ((FP32)data_int32s) / 1000.0;
// 	AntennaParam->aim_polar = ( (FP32)data_int32s) / 1000.0;
	index += 4;
	
	return SUCC;
	
}



/***********************************************************
**name:	APP_PARAM_OPTLog_TotalNum_Read
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_OPTLog_TotalNum_Read(INT32U *total_num)
{
	INT8U readbuff[32] = {0},len = 4;
	INT16U addr;

	INT8U index = 0,i=0;
		
	addr = APP_EEPROM_OPTLog_TotalNum_ADDR;
	if(APP_PARAM_Read(addr,readbuff,len) == FAIL)
		return FAIL;

	*total_num =  (readbuff[index] << 24) + (readbuff[index+1] << 16) + (readbuff[index+2] << 8) + readbuff[index+3];

	
	return SUCC;
	
}

/***********************************************************
**name:	APP_PARAM_OPTLog_CurrID_Read
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_OPTLog_CurrID_Read(INT32U *id)
{
	INT8U readbuff[32] = {0},len = 4;
	INT16U addr;

	INT8U index = 0,i=0;
		
	addr = APP_EEPROM_OPTLog_CurrID_ADDR;
	if(APP_PARAM_Read(addr,readbuff,len) == FAIL)
		return FAIL;

	*id =  (readbuff[index] << 24) + (readbuff[index+1] << 16) + (readbuff[index+2] << 8) + readbuff[index+3];

	
	return SUCC;
	
}

/***********************************************************
**name:	APP_PARAM_WarnLog_TotalNum_Read
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_WarnLog_TotalNum_Read(INT32U *total_num)
{
	INT8U readbuff[32] = {0},len = 4;
	INT16U addr;

	INT8U index = 0,i=0;
		
	addr = APP_EEPROM_WarnLog_TotalNum_ADDR;
	if(APP_PARAM_Read(addr,readbuff,len) == FAIL)
		return FAIL;

	*total_num =  (readbuff[index] << 24) + (readbuff[index+1] << 16) + (readbuff[index+2] << 8) + readbuff[index+3];

	
	return SUCC;
	
}

/***********************************************************
**name:	APP_PARAM_WarnLog_CurrID_Read
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_WarnLog_CurrID_Read(INT32U *id)
{
	INT8U readbuff[32] = {0},len = 4;
	INT16U addr;

	INT8U index = 0,i=0;
		
	addr = APP_EEPROM_WarnLog_CurrID_ADDR;
	if(APP_PARAM_Read(addr,readbuff,len) == FAIL)
		return FAIL;

	*id =  (readbuff[index] << 24) + (readbuff[index+1] << 16) + (readbuff[index+2] << 8) + readbuff[index+3];

	
	return SUCC;
	
}



/***********************************************************
**name:	APP_PARAM_SatTotalNum_Read
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_SatTotalNum_Read(INT16U *total_num)
{
	INT8U readbuff[32] = {0},len = 2;
	INT16U addr;

	INT8U index = 0,i=0;
	
	addr = APP_EEPROM_Sat_TotalNum_ADDR;
	if(APP_PARAM_Read(addr,readbuff,len) == FAIL)
		return FAIL;

	*total_num = (readbuff[index] << 8) + readbuff[index+1];

	
	return SUCC;
	
}

/***********************************************************
**name:	APP_PARAM_SatCurrID_Read
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_SatCurrID_Read(INT8U *curr_id)
{
	INT8U readbuff[32] = {0},len = 1;
	INT16U addr;

	INT8U index = 0,i=0;
	
	addr = APP_EEPROM_Sat_CurrID_ADDR;
	if(APP_PARAM_Read(addr,readbuff,len) == FAIL)
		return FAIL;

	*curr_id = readbuff[index];

	
	return SUCC;
	
}

/***********************************************************
**name:	APP_PARAM_CurrSatParam_Read
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_CurrSatParam_Read(strSatParam *SatParam)
{
	INT8U readbuff[32] = {0};
	INT16U addr;
	INT8U maxlen =0;
	INT8U checkbuff[32] = {0};
	INT32U checkbufflen;
	INT32U degree_int32u;
	INT8U index = 0,i=0;

	//if( SatParam->id <= 0)
	//	return FAIL;
		
	addr = APP_EEPROM_CurrSatParam_ADDR;

	if(APP_PARAM_Read(addr,readbuff,27) == FAIL)
		return FAIL;

	//if( SatParam->id != readbuff[index] )
	//	return FAIL;

	SatParam->id = readbuff[index++];

	memcpy(SatParam->name,&readbuff[index],16);
	index += 16;

	SatParam->lon_flag = readbuff[index++];
	degree_int32u = (readbuff[index] << 24) + (readbuff[index+1] << 16) + (readbuff[index+2] << 8) + readbuff[index+3];
	SatParam->lon = (FP32)degree_int32u / 1000;
	index += 4;

	SatParam->freq =  (readbuff[index] << 24) + (readbuff[index+1] << 16) + (readbuff[index+2] << 8) + readbuff[index+3];
	index += 4;

	SatParam->polar_type = readbuff[index];

	return SUCC;
	
}

/***********************************************************
**name:	APP_PARAM_AimAtSatParamLimitmv_Read
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_AimAtSatParamLimitmv_Read(strSeekSatParam  *SeekSatParam)
{
	INT8U readbuff[32] = {0},len = 4;
	INT16U addr;

	INT8U index = 0,i=0;
	
	addr = APP_EEPROM_AimAtSatLimitMv_ADDR;
	if(APP_PARAM_Read(addr,readbuff,len) == FAIL)
		return FAIL;

	SeekSatParam->AimAtSat_Limit_mv = (readbuff[index] << 24) + (readbuff[index+1] << 16) + (readbuff[index+2] << 8) + readbuff[index+3];


	return SUCC;
}

/***********************************************************
**name:	APP_PARAM_AntennaDir_Map_MotorDir_Read
**describe:
**input:			AntennaPart: 0x01 天线方位，0x02天线俯仰，0x03天线极化
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_AntennaDir_Map_MotorDir_Read(strAntennaParam *AntennaParam)
{
	INT8U readbuff[32] = {0};
	INT16U addr;
	INT8U index = 0,i=0;

	
	addr = APP_EEPROM_AntennaDir_Map_MotorDir_ADDR;

	if(APP_PARAM_Read(addr,readbuff,6) == FAIL)
		return FAIL;

	for(i=0;i<3;i++)
	{
		AntennaParam->AntennaDirMapMotor[i].motor_dir[0] = readbuff[index++];
		AntennaParam->AntennaDirMapMotor[i].motor_dir[1] = readbuff[index++];
	}

	return SUCC;
	
}

/***********************************************************
**name:	APP_PARAM_Sat_Read
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_SatParam_Read(strSatParam *SatParam)
{
	INT8U readbuff[32] = {0};
	INT16U addr;
	INT8U maxlen =0;
	INT8U checkbuff[32] = {0};
	INT32U checkbufflen;
	INT32U degree_int32u;
	INT8U index = 0,i=0;

	if( SatParam->id <= 0)
		return FAIL;
		
	addr = (SatParam->id - 1)*APP_EEPROM_SatParam_OffSet_ADDR + APP_EEPROM_SatParam_Base_ADDR;

	if(APP_PARAM_Read(addr,readbuff,27) == FAIL)
		return FAIL;

	if( SatParam->id != readbuff[index] )
		return FAIL;

	SatParam->id = readbuff[index++];

	memcpy(SatParam->name,&readbuff[index],16);
	index += 16;

	SatParam->lon_flag = readbuff[index++];
	degree_int32u = (readbuff[index] << 24) + (readbuff[index+1] << 16) + (readbuff[index+2] << 8) + readbuff[index+3];
	SatParam->lon = (FP32)degree_int32u / 1000;
	index += 4;

	SatParam->freq =  (readbuff[index] << 24) + (readbuff[index+1] << 16) + (readbuff[index+2] << 8) + readbuff[index+3];
	index += 4;

	SatParam->polar_type = readbuff[index];

	return SUCC;
}



/***********************************************************
**name:	APP_PARAM_HistoryMaxMv_Read
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_HistoryMaxMv_Read(FP32 *history_max_mv)
{
	INT8U readbuff[32] = {0};
	INT16U addr;
	INT8U maxlen =0;
	INT8U checkbuff[32] = {0};
	INT32U checkbufflen;
	INT32U degree_int32u;
	INT8U index = 0,i=0;
		
	addr  =	APP_EEPROM_HISTORY_MAX_MV_ADDR;

	if(APP_PARAM_Read(addr,readbuff,4) == FAIL)
		return FAIL;

	*history_max_mv = (FP32)( (readbuff[index] << 24) + (readbuff[index+1] << 16) + (readbuff[index+2] << 8) + readbuff[index+3]);


	return SUCC;
}




/***********************************************************
**name:	APP_PARAM_EthRelinkResetMCUFlag_Read
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
INT8U  APP_PARAM_EthRelinkResetMCUFlag_Read(void)
{
	INT8U readbuff[32] = {0};
	INT16U addr;
	INT8U maxlen =0;
	INT8U checkbuff[32] = {0};
	INT32U checkbufflen;
	INT32U degree_int32u;
	INT8U index = 0,i=0;
		
	addr  =	APP_EEPROM_Reset_Flag_ADDR;

	if(APP_PARAM_Read(addr,readbuff,1) == FAIL)
		return FAIL;

	return readbuff[index];

}





/***********************************************************
**name:	APP_PARAM_HistoryMaxMv_Read
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PARAM_DevPowerOnSysDateTime_Read(strSysDateTime	 *SysDateTime)
{
	INT8U readbuff[32] = {0};
	INT16U addr;
	INT8U maxlen =0;
	INT8U checkbuff[32] = {0};
	INT32U checkbufflen;
	INT32U degree_int32u;
	INT8U index = 0,i=0;
		
	addr  =	APP_EEPROM_Dev_PowerOn_SysDateTime_ADDR;

	if(APP_PARAM_Read(addr,readbuff,7) == FAIL)
		return FAIL;

	SysDateTime->year = (readbuff[index]<<8) + readbuff[index+1];
	index += 2;
	SysDateTime->month = readbuff[index++];
	SysDateTime->day= readbuff[index++];
	SysDateTime->hour= readbuff[index++];
	SysDateTime->min= readbuff[index++];
	SysDateTime->senc= readbuff[index++];


	return SUCC;
}


