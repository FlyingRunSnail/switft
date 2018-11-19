


#include <app_monitor.h>
#include <devdef.h>
#include <app_tool.h>
#include <math.h>
#include <app_log.h>

#define	APP_MONITOR_DEF

#define M_PI      ( (FP32)(3.14159265358979323846))

/***********************************************************
**name:	APP_MONITOR_LocallAntennaRealDegreeDataGet
**describe:   获取本地天线的实时转向角度数据
**input:	
**output:	none
**return:  
**autor:  andiman
**date:
************************************************************/
void APP_MONITOR_LocallAntennaRealDegreeDataGet(void)
{
	FP32 eleccoder_position_degree=0,  eleccoder_cabrage_degree=0, eleccoder_polar_degree=0;
	FP32 antenna_position_degree,   antenna_cabrage_degree,  antenna_polar_degree;
	FP32 d_value = 0;

	
	
	if( ACUDevInfo.DeviceInfo.EleccoderInfo[motor_position_id-1].EleccoderStat.link_stat == COMM_LINK)
	{
		// 计算方位,  天线方位角度都是正北0度，正东90度，正南180度，正西270度
		eleccoder_position_degree = ACUDevInfo.DeviceInfo.EleccoderInfo[0].EleccoderData.degree;
		d_value = eleccoder_position_degree - ACUDevInfo.DeviceInfo.AllElecCoderMapAntenna.ElecCoderMapAntenna[motor_position_id-1].ElecCoder_degree;
		if( ACUDevInfo.DeviceInfo.AllElecCoderMapAntenna.ElecCoderMapAntenna[motor_position_id-1].ElecCoder_count_mode == 
			ACUDevInfo.DeviceInfo.EleccoderInfo[motor_position_id-1].EleccoderConf.count_mode)
			antenna_position_degree = ACUDevInfo.DeviceInfo.AllElecCoderMapAntenna.ElecCoderMapAntenna[motor_position_id-1].Antenna_degree + d_value;
		else
			antenna_position_degree = ACUDevInfo.DeviceInfo.AllElecCoderMapAntenna.ElecCoderMapAntenna[motor_position_id-1].Antenna_degree - d_value;

		if( antenna_position_degree < 0 )
			antenna_position_degree += 360;
		else if( antenna_position_degree >= 360 )
			antenna_position_degree -= 360;
	}
	ACUDevInfo.ACUCurrSta.antenna_position = antenna_position_degree;

	if( ACUDevInfo.DeviceInfo.EleccoderInfo[motor_cabrage_id-1].EleccoderStat.link_stat == COMM_LINK)
	{
		// 计算俯仰角,水平面是0度，最高是90度
		eleccoder_cabrage_degree = ACUDevInfo.DeviceInfo.EleccoderInfo[1].EleccoderData.degree;
		d_value = eleccoder_cabrage_degree - ACUDevInfo.DeviceInfo.AllElecCoderMapAntenna.ElecCoderMapAntenna[motor_cabrage_id-1].ElecCoder_degree;
		if( ACUDevInfo.DeviceInfo.AllElecCoderMapAntenna.ElecCoderMapAntenna[motor_cabrage_id-1].ElecCoder_count_mode == 
			ACUDevInfo.DeviceInfo.EleccoderInfo[motor_cabrage_id-1].EleccoderConf.count_mode)
			antenna_cabrage_degree = ACUDevInfo.DeviceInfo.AllElecCoderMapAntenna.ElecCoderMapAntenna[motor_cabrage_id-1].Antenna_degree + d_value;
		else	
			antenna_position_degree = ACUDevInfo.DeviceInfo.AllElecCoderMapAntenna.ElecCoderMapAntenna[motor_cabrage_id-1].Antenna_degree - d_value;

		if( antenna_cabrage_degree < 0 )
			antenna_cabrage_degree = 0;
		else if( antenna_cabrage_degree >= 360 )
			antenna_cabrage_degree -= 360;
	}
	
	ACUDevInfo.ACUCurrSta.antenna_cabrage = antenna_cabrage_degree;		


	if( (ACUDevInfo.ACUParam.AntennaParam.elec_coder_num == 3) && ( ACUDevInfo.DeviceInfo.EleccoderInfo[motor_polar_id-1].EleccoderStat.link_stat == COMM_LINK ))
	{

		// 计算极化,  极化角度都是正北0度，正东90度，正南180度，正西270度
		eleccoder_polar_degree = ACUDevInfo.DeviceInfo.EleccoderInfo[2].EleccoderData.degree;
		d_value = eleccoder_polar_degree - ACUDevInfo.DeviceInfo.AllElecCoderMapAntenna.ElecCoderMapAntenna[motor_polar_id-1].ElecCoder_degree;

		if( ACUDevInfo.DeviceInfo.AllElecCoderMapAntenna.ElecCoderMapAntenna[motor_polar_id-1].ElecCoder_count_mode == 
			ACUDevInfo.DeviceInfo.EleccoderInfo[motor_polar_id-1].EleccoderConf.count_mode)
			antenna_polar_degree = ACUDevInfo.DeviceInfo.AllElecCoderMapAntenna.ElecCoderMapAntenna[motor_polar_id-1].Antenna_degree + d_value;
		else	
			antenna_polar_degree = ACUDevInfo.DeviceInfo.AllElecCoderMapAntenna.ElecCoderMapAntenna[motor_polar_id-1].Antenna_degree - d_value;

		

		
		if( antenna_polar_degree <= -180 )
			antenna_polar_degree += 360;
		else if( antenna_polar_degree >= 180 )	// 因为极化角只有-90~+90，因此认为270~360为-90到0
			antenna_polar_degree -= 360;
		
		
		/*
		if( antenna_polar_degree < 0 )
			antenna_polar_degree = 0;
		else if( antenna_polar_degree >= 360 )
			antenna_polar_degree -= 360;+9
		*/
	}
	ACUDevInfo.ACUCurrSta.antenna_polar = antenna_polar_degree;
	
// 	if( ACUDevInfo.DeviceInfo.AllElecCoderMapAntenna.SatPolarType != ACUDevInfo.ACUParam.CurrSatParam.polar_type )
	{
		if(ACUDevInfo.DeviceInfo.AllElecCoderMapAntenna.SatPolarType == 2)
			ACUDevInfo.ACUCurrSta.antenna_polar -= 90;
		else if(ACUDevInfo.DeviceInfo.AllElecCoderMapAntenna.SatPolarType == 1)
			ACUDevInfo.ACUCurrSta.antenna_polar += 90;
	}
	
}


/***********************************************************
**name:	APP_MONITOR_LocallAntennaAdjustDegreeDataGet
**describe:   获取本地天线的校准角度数据
**input:	
**output:	none
**return:  
**autor:  andiman
**date:
************************************************************/
void APP_MONITOR_LocallAntennaAdjustDegreeDataGet(INT8U part_id,FP32 *degree_input,FP32 *degree_output)
{
	FP32 eleccoder_position_degree=0,  eleccoder_cabrage_degree=0, eleccoder_polar_degree=0;
	FP32 antenna_position_degree,   antenna_cabrage_degree,  antenna_polar_degree;
	FP32 d_value = 0;

	
	
	if( (ACUDevInfo.DeviceInfo.EleccoderInfo[motor_position_id-1].EleccoderStat.link_stat == COMM_LINK)&&(part_id == motor_position_id))
	{
		// 计算方位,  天线方位角度都是正北0度，正东90度，正南180度，正西270度
		eleccoder_position_degree = *degree_input;
		d_value = eleccoder_position_degree - ACUDevInfo.DeviceInfo.AllElecCoderMapAntenna.ElecCoderMapAntenna[motor_position_id-1].ElecCoder_degree;
		if( ACUDevInfo.DeviceInfo.AllElecCoderMapAntenna.ElecCoderMapAntenna[motor_position_id-1].ElecCoder_count_mode == 
			ACUDevInfo.DeviceInfo.EleccoderInfo[motor_position_id-1].EleccoderConf.count_mode)
			antenna_position_degree = ACUDevInfo.DeviceInfo.AllElecCoderMapAntenna.ElecCoderMapAntenna[motor_position_id-1].Antenna_degree + d_value;
		else
			antenna_position_degree = ACUDevInfo.DeviceInfo.AllElecCoderMapAntenna.ElecCoderMapAntenna[motor_position_id-1].Antenna_degree - d_value;

		if( antenna_position_degree < 0 )
			antenna_position_degree += 360;
		else if( antenna_position_degree >= 360 )
			antenna_position_degree -= 360;

		*degree_output = antenna_position_degree;
	}

	if( (ACUDevInfo.DeviceInfo.EleccoderInfo[motor_cabrage_id-1].EleccoderStat.link_stat == COMM_LINK)&&(part_id == motor_cabrage_id))
	{
		// 计算俯仰角,水平面是0度，最高是90度
		eleccoder_cabrage_degree = *degree_input;
		d_value = eleccoder_cabrage_degree - ACUDevInfo.DeviceInfo.AllElecCoderMapAntenna.ElecCoderMapAntenna[motor_cabrage_id-1].ElecCoder_degree;
		if( ACUDevInfo.DeviceInfo.AllElecCoderMapAntenna.ElecCoderMapAntenna[motor_cabrage_id-1].ElecCoder_count_mode == 
			ACUDevInfo.DeviceInfo.EleccoderInfo[motor_cabrage_id-1].EleccoderConf.count_mode)
			antenna_cabrage_degree = ACUDevInfo.DeviceInfo.AllElecCoderMapAntenna.ElecCoderMapAntenna[motor_cabrage_id-1].Antenna_degree + d_value;
		else	
			antenna_cabrage_degree = ACUDevInfo.DeviceInfo.AllElecCoderMapAntenna.ElecCoderMapAntenna[motor_cabrage_id-1].Antenna_degree - d_value;

		if( antenna_cabrage_degree < 0 )
			antenna_cabrage_degree = 0;
		else if( antenna_cabrage_degree >= 360 )
			antenna_cabrage_degree -= 360;

		*degree_output = antenna_cabrage_degree;
	}
		

	if( (ACUDevInfo.ACUParam.AntennaParam.elec_coder_num == 3) && ( ACUDevInfo.DeviceInfo.EleccoderInfo[motor_polar_id-1].EleccoderStat.link_stat == COMM_LINK )&&(part_id == motor_polar_id))
	{

		// 计算极化,  极化角度都是正北0度，正东90度，正南180度，正西270度
		eleccoder_polar_degree = *degree_input;
		d_value = eleccoder_polar_degree - ACUDevInfo.DeviceInfo.AllElecCoderMapAntenna.ElecCoderMapAntenna[motor_polar_id-1].ElecCoder_degree;

		if( ACUDevInfo.DeviceInfo.AllElecCoderMapAntenna.ElecCoderMapAntenna[motor_polar_id-1].ElecCoder_count_mode == 
			ACUDevInfo.DeviceInfo.EleccoderInfo[motor_polar_id-1].EleccoderConf.count_mode)
			antenna_polar_degree = ACUDevInfo.DeviceInfo.AllElecCoderMapAntenna.ElecCoderMapAntenna[motor_polar_id-1].Antenna_degree + d_value;
		else	
			antenna_polar_degree = ACUDevInfo.DeviceInfo.AllElecCoderMapAntenna.ElecCoderMapAntenna[motor_polar_id-1].Antenna_degree - d_value;

		
		if( antenna_polar_degree <= -180 )
			antenna_polar_degree += 360;
		else if( antenna_polar_degree >= 180 )	// 因为极化角只有-90~+90，因此认为270~360为-90到0
			antenna_polar_degree -= 360;
		/*
		if( antenna_polar_degree < 0 )
			antenna_polar_degree = 0;
		else if( antenna_polar_degree >= 360 )
			antenna_polar_degree -= 360;
		*/

		
		
		//if( ACUDevInfo.DeviceInfo.AllElecCoderMapAntenna.SatPolarType != ACUDevInfo.ACUParam.CurrSatParam.polar_type )
		{
			if(ACUDevInfo.DeviceInfo.AllElecCoderMapAntenna.SatPolarType == 2)
				antenna_polar_degree -= 90;
			else if(ACUDevInfo.DeviceInfo.AllElecCoderMapAntenna.SatPolarType == 1)
				antenna_polar_degree += 90;
		}
		
		*degree_output = antenna_polar_degree;
	}
}



/***********************************************************
**name:	APP_AIMDEGREE_Cabrage_Get
**describe:  获取目标俯仰角
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
FP32 APP_AIMDEGREE_Cabrage_Get(INT8U antenna_type, FP32 sat_lon,FP32 local_lon,FP32 local_lat, FP32 corr_val)
{
	FP32 cabrage;
	FP32 sat_lon_angl,local_lon_angl,local_lat_angl;
	
	sat_lon_angl = sat_lon *M_PI/180;
	local_lon_angl = local_lon *M_PI/180;
	local_lat_angl = local_lat *M_PI/180;

	cabrage = atan2((cos(sat_lon_angl-local_lon_angl)*cos(local_lat_angl) - 0.151266) ,( sqrt(1-pow(cos(sat_lon_angl-local_lon_angl)*cos(local_lat_angl),2)))); 
 	cabrage *= 180 / M_PI;

 	if( antenna_type == 1)		// 	天线为偏馈正装
 		cabrage -= corr_val;
 	else if( antenna_type == 2)	// 天线为偏馈倒装
 	{
 		cabrage += corr_val;
 	}
 	else
 		cabrage -= 0;

	return cabrage;
}

/***********************************************************
**name:	APP_AIMDEGREE_Position_Get
**describe:  获取目标方位角
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
FP32 APP_AIMDEGREE_Position_Get(FP32 sat_lon,FP32 local_lon,FP32 local_lat)
{
	FP32 position;
	FP32 sat_lon_angl,local_lon_angl,local_lat_angl;

	sat_lon_angl = sat_lon *M_PI/180;
	local_lon_angl = local_lon *M_PI/180;
	local_lat_angl = local_lat *M_PI/180;

	/*
	temp1 = tan(sat_lon_angl-local_lon_angl);
	temp2 = sin(local_lat_angl);
	*/
	
	position = atan2(tan(sat_lon_angl-local_lon_angl),sin(local_lat_angl));
	//position = atan2(temp1,temp2);
	
	position *= 180 / M_PI;

	
	position = 180 - position;
	return position;
}

/***********************************************************
**name:	APP_AIMDEGREE_Polar_Get
**describe:  获取目标极化角
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
FP32 APP_AIMDEGREE_Polar_Get(FP32 sat_lon,FP32 local_lon,FP32 local_lat)
{
	FP32 polar;
	FP32 sat_lon_angl,local_lon_angl,local_lat_angl;

	sat_lon_angl = sat_lon *M_PI/180;
	local_lon_angl = local_lon *M_PI/180;
	local_lat_angl = local_lat *M_PI/180;
	
	//temp1 = sin(sat_lon_angl-local_lon_angl);
	//temp2 = tan(local_lat_angl);
	
	polar = atan2(sin(sat_lon_angl-local_lon_angl),tan(local_lat_angl));

	//polar = atan2(temp1,temp2);
	polar *= 180 / M_PI;
	return polar;
}




/***********************************************************
**name:	APP_MONITOR_AimDegree_Get
**describe:  获取目标角度
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void APP_MONITOR_AimDegree_Get(void)
{
	
	FP32 aim_cabrage,aim_position,aim_polar;
	FP32 sat_lon;
	FP32 local_lon,local_lat,corr_val;

	sat_lon = ACUDevInfo.ACUParam.CurrSatParam.lon;
	local_lon = ACUDevInfo.ACUParam.LocalPositionParam.lon ;
	local_lat = ACUDevInfo.ACUParam.LocalPositionParam.lat ;
	corr_val = ((FP32)ACUDevInfo.ACUParam.AntennaParam.offset_angle) / 1000;

	aim_cabrage = APP_AIMDEGREE_Cabrage_Get(  ACUDevInfo.ACUParam.AntennaParam.type, sat_lon, local_lon, local_lat,  corr_val);
	aim_position = APP_AIMDEGREE_Position_Get( sat_lon, local_lon, local_lat);
	aim_polar = APP_AIMDEGREE_Polar_Get( sat_lon,  local_lon,  local_lat);

	ACUDevInfo.ACUParam.AntennaParam.aim_cabrage =aim_cabrage;
	ACUDevInfo.ACUParam.AntennaParam.aim_position = aim_position;
	ACUDevInfo.ACUParam.AntennaParam.aim_polar = aim_polar;

	if( ACUDevInfo.ACUParam.CurrSatParam.polar_type == 'Y' )
	{
		ACUDevInfo.ACUParam.AntennaParam.aim_polar = ACUDevInfo.ACUParam.AntennaParam.aim_polar<0?
													(ACUDevInfo.ACUParam.AntennaParam.aim_polar+90):
													(ACUDevInfo.ACUParam.AntennaParam.aim_polar-90);
	}

}


/***********************************************************
**name:	APP_MONITOR_Temperature_Get
**describe:  获取ACU 温度
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void APP_MONITOR_Temperature_Get(void)
{
	static INT8U temp_cnt = 0;
	static INT32S temp = 0;
	
	temp += AD7814_TempGet();
	temp_cnt++;
	if( temp_cnt >= 10 )
	{
		ACUDevInfo.ACUCurrSta.temperature = temp / temp_cnt;
		temp_cnt = 0;
		temp = 0;
	}
}



/***********************************************************
**name:	APP_MONITOR_ETH_LinkSta_Get
**describe:  获取以太网link状态
**input:			
**output:	none
**return:	连接返回1，未连接返回0
**autor:  andiman
**date:
************************************************************/
INT8U APP_MONITOR_ETH_LinkSta_Get(void)
{
	if(((ETH_ReadPHYRegister(DP83848_PHY_ADDRESS, PHY_STS)) & PHY_LINK_STATUS_LINK) != 0)
	{
		
		return 1;
	}
	return 0;
}

/***********************************************************
**name:	APP_MONITOR_ETH_ReLink_Deal
**describe:  获取以太网link状态
**input:			
**output:	none
**return:	连接返回1，未连接返回0
**autor:  andiman
**date:
************************************************************/
void APP_MONITOR_ETH_ReLink_Deal(INT8U curr_eth_link)
{
	
	APP_PARAM_EthRelinkResetMCUFlagSet_Save();
	
	APP_PARAM_AimAtSatMode_Save(ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode);
	APP_PARAM_AimAtSatWorkSta_Save(ACUDevInfo.ACUCurrSta.dev_work_sta);
	// 暂时的处理是重启MCU
	BSP_SoftReset();
	//LWIP_Init();
	
}


/***********************************************************
**name:	APP_MONITOR_ETH_AdminUnlinkTimeout_Get
**describe:  获取以太网管理权限超时计数，当具有管理权限的PC机有超时未发数据信息时，则取消他的管理权限
**input:			
**output:	none
**return:		返回次数
**autor:  andiman
**date:
************************************************************/
INT32U APP_MONITOR_ETH_AdminUnlinkTimeout_Get(void)
{
	static INT32U PC_admin_old_link_cnt;
	static INT32U pc_admin_unlink_cnt = 0;

	if( ACUDevInfo.ACUCurrSta.DevAdmin.admin_sta == 0x01)
	{
		if( PC_admin_old_link_cnt != ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_link_cnt )
		{
			PC_admin_old_link_cnt = ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_link_cnt;
			pc_admin_unlink_cnt = 0;
		}
		else
		{
			pc_admin_unlink_cnt++;
		}
	}
	else
	{
		PC_admin_old_link_cnt = ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_link_cnt - 1;
		pc_admin_unlink_cnt = 0;
	}

	return pc_admin_unlink_cnt;
}



/***********************************************************
**name:	APP_MONITOR_PC_Admin_Timeout_Deal
**describe:  
**output:	none
**return:		返回次数
**autor:  andiman
**date:
************************************************************/
void APP_MONITOR_PC_Admin_Timeout_Deal(INT32U period_ms)
{
	static INT32U pc_admin_wait_timeout_cnt = 0;
	static INT32U PC_admin_old_link_cnt;
	static INT32U pc_admin_unlink_cnt = 0;
	
	if( ACUDevInfo.ACUCurrSta.dev_admin_sta == 0x01)
	{
		if( PC_admin_old_link_cnt != ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_link_cnt )
		{
			pc_admin_unlink_cnt = 0;
			PC_admin_old_link_cnt = ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_link_cnt;
		}
		else
		{
			pc_admin_unlink_cnt++;
			if( pc_admin_unlink_cnt * period_ms > 15000)		// 超过15秒钟未连接清除管理权限
			{
				pc_admin_unlink_cnt = 0;
				ACUDevInfo.ACUCurrSta.dev_admin_sta = 0x00;
				ACUDevInfo.ACUCurrSta.DevAdmin.admin_sta = 0x00;
				ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_link_cnt = 0;
				ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_deal_sta = 0;
				ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_ip_addr = 0;
				ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_ask_ip_addr = 0;

				APP_OptLOG_SaveCode(LOG_OPT_TIMEOUT_ACU_CANCEL_PC_ADMIN);
			}
		}
	}
	else 
	{
		pc_admin_unlink_cnt = 0;
		PC_admin_old_link_cnt = 0;
		if( ACUDevInfo.ACUCurrSta.dev_admin_sta == 0x11)
		{
			pc_admin_wait_timeout_cnt++;
			if( pc_admin_wait_timeout_cnt  * period_ms  > 300000)  // 超过5分钟清除管理权限申请
			{
				pc_admin_wait_timeout_cnt = 0;
				//if(ACUDevInfo.ACUCurrSta.DevAdmin.admin_sta != 0x02)
				{
					ACUDevInfo.ACUCurrSta.dev_admin_sta = 0x00;
					ACUDevInfo.ACUCurrSta.DevAdmin.admin_sta = 0x00;
					ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_link_cnt = 0;
					ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_deal_sta = 0;
					ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_ip_addr = 0;
					ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_ask_ip_addr = 0;

					APP_OptLOG_SaveCode(LOG_OPT_TIMEOUT_ACU_REFUS_PC_ADMIN);
				}
			
			}
		}
		else
			pc_admin_wait_timeout_cnt = 0;
	}
}

/***********************************************************
**name:	APP_MONITOR_AlarmStaCodeDeal
**describe:  设备告警状态编码转换，方便传给PC和oled
**input:			
**output:	none
**return:		返回次数
**autor:  andiman
**date:
************************************************************/
void APP_MONITOR_AlarmStaCodeDeal(INT8U *AlarmCodebuff,INT32U *buffsize)
{
	INT8U index = 0;

	// 方位码盘
	if( ACUDevInfo.ACUCurrSta.dev_alarm_buff[0] & 0x01 )
	{
		AlarmCodebuff[index++] = 0x01;
		AlarmCodebuff[index++] = 0x01;
		AlarmCodebuff[index++] = 0x01;
	}
	// 俯仰码盘
	if( ACUDevInfo.ACUCurrSta.dev_alarm_buff[0] & 0x02 )
	{
		AlarmCodebuff[index++] = 0x01;
		AlarmCodebuff[index++] = 0x01;
		AlarmCodebuff[index++] = 0x02;
	}
	// 极化码盘
	if( ACUDevInfo.ACUCurrSta.dev_alarm_buff[0] & 0x04 )
	{
		AlarmCodebuff[index++] = 0x01;
		AlarmCodebuff[index++] = 0x01;
		AlarmCodebuff[index++] = 0x03;
	}

	//信标机通信故障
	if( ACUDevInfo.ACUCurrSta.dev_alarm_buff[1] & 0x01 )
	{
		AlarmCodebuff[index++] = 0x01;
		AlarmCodebuff[index++] = 0x02;
		AlarmCodebuff[index++] = 0x01;
	}
	// 信标机频率设置失败
	if( ACUDevInfo.ACUCurrSta.dev_alarm_buff[1] & 0x02 )
	{
		AlarmCodebuff[index++] = 0x01;
		AlarmCodebuff[index++] = 0x02;
		AlarmCodebuff[index++] = 0x02;
	}
	// 信标机增益范围设置失败
	if( ACUDevInfo.ACUCurrSta.dev_alarm_buff[1] & 0x04 )
	{
		AlarmCodebuff[index++] = 0x01;
		AlarmCodebuff[index++] = 0x02;
		AlarmCodebuff[index++] = 0x03;
	}

	// 主控板EEPROM故障
	if( ACUDevInfo.ACUCurrSta.dev_alarm_buff[2] & 0x01 )
	{
		AlarmCodebuff[index++] = 0x01;
		AlarmCodebuff[index++] = 0x03;
		AlarmCodebuff[index++] = 0x01;
	}
	// 主控板flash故障
	if( ACUDevInfo.ACUCurrSta.dev_alarm_buff[2] & 0x02 )
	{
		AlarmCodebuff[index++] = 0x01;
		AlarmCodebuff[index++] = 0x03;
		AlarmCodebuff[index++] = 0x02;
	}

	// PDU单元通信失败
	if( ACUDevInfo.ACUCurrSta.dev_alarm_buff[3] & 0x01 )
	{
		AlarmCodebuff[index++] = 0x01;
		AlarmCodebuff[index++] = 0x04;
		AlarmCodebuff[index++] = 0x01;
	}
	//GPS定位失败
	if( ACUDevInfo.ACUCurrSta.dev_alarm_buff[4] & 0x02 )
	{
		AlarmCodebuff[index++] = 0x01;
		AlarmCodebuff[index++] = 0x04;
		AlarmCodebuff[index++] = 0x02;
	}
	//俯仰上限告警
	if( ACUDevInfo.ACUCurrSta.dev_alarm_buff[5] & 0x01)
	{
		AlarmCodebuff[index++] = 0x01;
		AlarmCodebuff[index++] = 0x04;
		AlarmCodebuff[index++] = 0x03;
	}
	//俯仰下线告警
	if( ACUDevInfo.ACUCurrSta.dev_alarm_buff[5] & 0x02)
	{
		AlarmCodebuff[index++] = 0x01;
		AlarmCodebuff[index++] = 0x04;
		AlarmCodebuff[index++] = 0x04;
	}
	//方位左限告警
	if( ACUDevInfo.ACUCurrSta.dev_alarm_buff[5] & 0x04)
	{
		AlarmCodebuff[index++] = 0x01;
		AlarmCodebuff[index++] = 0x04;
		AlarmCodebuff[index++] = 0x05;
	}
	//方位右限位告警
	if( ACUDevInfo.ACUCurrSta.dev_alarm_buff[5] & 0x08)
	{
		AlarmCodebuff[index++] = 0x01;
		AlarmCodebuff[index++] = 0x04;
		AlarmCodebuff[index++] = 0x06;
	}
	//极化顺时针限位告警
	if( ACUDevInfo.ACUCurrSta.dev_alarm_buff[5] & 0x10)
	{
		AlarmCodebuff[index++] = 0x01;
		AlarmCodebuff[index++] = 0x04;
		AlarmCodebuff[index++] = 0x07;
	}
	// 极化逆时针限位告警
	if( ACUDevInfo.ACUCurrSta.dev_alarm_buff[5] & 0x20)
	{
		AlarmCodebuff[index++] = 0x01;
		AlarmCodebuff[index++] = 0x04;
		AlarmCodebuff[index++] = 0x08;
	}

	*buffsize = index;

}

/***********************************************************
**name:	APP_MONITOR_AlarmStaWrite2Flash
**describe:  将设备告警状态编码写入FLASH
**input:			
**output:	none
**return:		返回次数
**autor:  andiman
**date:
************************************************************/
void APP_MONITOR_AlarmStaWrite2Flash(INT8U *old_ararmbuff, INT8U *new_alarmbuff)
{
	INT8U index = 0;
	INT8U AlarmCodebuff[3] = {0};

	// 方位码盘
	if(( (old_ararmbuff[0] & 0x01 ) != (new_alarmbuff[0] & 0x01 )) && ((new_alarmbuff[0] & 0x01 ) > 0))
	{
		AlarmCodebuff[index++] = 0x01;
		AlarmCodebuff[index++] = 0x01;
		AlarmCodebuff[index++] = 0x01;

		// 写入flash


		index = 0;
	}
	// 俯仰码盘
	if(( (old_ararmbuff[0] & 0x02 ) != (new_alarmbuff[0] & 0x02 )) && ((new_alarmbuff[0] & 0x02 ) > 0))
	{
		AlarmCodebuff[index++] = 0x01;
		AlarmCodebuff[index++] = 0x01;
		AlarmCodebuff[index++] = 0x02;

		// 写入flash


		index = 0;
	}
	// 极化码盘
	if(( (old_ararmbuff[0] & 0x04 ) != (new_alarmbuff[0] & 0x04 )) && ((new_alarmbuff[0] & 0x04 ) > 0))
	{
		AlarmCodebuff[index++] = 0x01;
		AlarmCodebuff[index++] = 0x01;
		AlarmCodebuff[index++] = 0x03;

		// 写入flash


		index = 0;
	}

	//信标机通信故障
	if(( (old_ararmbuff[1] & 0x01 ) != (new_alarmbuff[1] & 0x01 )) && ((new_alarmbuff[1] & 0x01 ) > 0))
	{
		AlarmCodebuff[index++] = 0x01;
		AlarmCodebuff[index++] = 0x02;
		AlarmCodebuff[index++] = 0x01;

		// 写入flash


		index = 0;
	}
	// 信标机频率设置失败
	if(( (old_ararmbuff[1] & 0x02 ) != (new_alarmbuff[1] & 0x02)) && ((new_alarmbuff[1] & 0x02 ) > 0))
	{
		AlarmCodebuff[index++] = 0x01;
		AlarmCodebuff[index++] = 0x02;
		AlarmCodebuff[index++] = 0x02;

		// 写入flash


		index = 0;
	}
	// 信标机增益范围设置失败
	if(( (old_ararmbuff[1] & 0x04 ) != (new_alarmbuff[1] & 0x04 )) && ((new_alarmbuff[1] & 0x04 ) > 0))
	{
		AlarmCodebuff[index++] = 0x01;
		AlarmCodebuff[index++] = 0x02;
		AlarmCodebuff[index++] = 0x03;

		// 写入flash


		index = 0;
	}

	// 主控板EEPROM故障
	if(( (old_ararmbuff[2] & 0x01 ) != (new_alarmbuff[2] & 0x01 )) && ((new_alarmbuff[2] & 0x01 ) > 0))
	{
		AlarmCodebuff[index++] = 0x01;
		AlarmCodebuff[index++] = 0x03;
		AlarmCodebuff[index++] = 0x01;

		// 写入flash


		index = 0;
	}
	// 主控板flash故障
	if(( (old_ararmbuff[2] & 0x02 ) != (new_alarmbuff[2] & 0x02 )) && ((new_alarmbuff[2] & 0x02 ) > 0))
	{
		AlarmCodebuff[index++] = 0x01;
		AlarmCodebuff[index++] = 0x03;
		AlarmCodebuff[index++] = 0x02;
	}

	// PDU单元通信失败
	if(( (old_ararmbuff[3] & 0x01 ) != (new_alarmbuff[3] & 0x01 )) && ((new_alarmbuff[3] & 0x01 ) > 0))
	{
		AlarmCodebuff[index++] = 0x01;
		AlarmCodebuff[index++] = 0x04;
		AlarmCodebuff[index++] = 0x01;

		// 写入flash


		index = 0;
	}
	//GPS定位失败
	if(( (old_ararmbuff[4] & 0x02 ) != (new_alarmbuff[4] & 0x02 )) && ((new_alarmbuff[4] & 0x02 ) > 0))
	{
		AlarmCodebuff[index++] = 0x01;
		AlarmCodebuff[index++] = 0x04;
		AlarmCodebuff[index++] = 0x02;

		// 写入flash


		index = 0;
	}
	//俯仰上限告警
	if(( (old_ararmbuff[5] & 0x01 ) != (new_alarmbuff[5] & 0x01 )) && ((new_alarmbuff[5] & 0x01 ) > 0))
	{
		AlarmCodebuff[index++] = 0x01;
		AlarmCodebuff[index++] = 0x04;
		AlarmCodebuff[index++] = 0x03;

		// 写入flash


		index = 0;
	}
	//俯仰下线告警
	if(( (old_ararmbuff[5] & 0x02 ) != (new_alarmbuff[5] & 0x02)) && ((new_alarmbuff[5] & 0x02 ) > 0))
	{
		AlarmCodebuff[index++] = 0x01;
		AlarmCodebuff[index++] = 0x04;
		AlarmCodebuff[index++] = 0x04;

		// 写入flash


		index = 0;
	}
	//方位左限告警
	if(( (old_ararmbuff[5] & 0x04 ) != (new_alarmbuff[5] & 0x04 )) && ((new_alarmbuff[5] & 0x04 ) > 0))
	{
		AlarmCodebuff[index++] = 0x01;
		AlarmCodebuff[index++] = 0x04;
		AlarmCodebuff[index++] = 0x05;

		// 写入flash


		index = 0;
	}
	//方位右限位告警
	if(( (old_ararmbuff[5] & 0x08 ) != (new_alarmbuff[5] & 0x08 )) && ((new_alarmbuff[5] & 0x08 ) > 0))
	{
		AlarmCodebuff[index++] = 0x01;
		AlarmCodebuff[index++] = 0x04;
		AlarmCodebuff[index++] = 0x06;

		// 写入flash


		index = 0;
	}
	//极化顺时针限位告警
	if(( (old_ararmbuff[5] & 0x10 ) != (new_alarmbuff[5] & 0x10 )) && ((new_alarmbuff[5] & 0x10 ) > 0))
	{
		AlarmCodebuff[index++] = 0x01;
		AlarmCodebuff[index++] = 0x04;
		AlarmCodebuff[index++] = 0x07;

		// 写入flash


		index = 0;
	}
	// 极化逆时针限位告警
	if(( (old_ararmbuff[5] & 0x10 ) != (new_alarmbuff[5] & 0x10 )) && ((new_alarmbuff[5] & 0x10 ) > 0))
	{
		AlarmCodebuff[index++] = 0x01;
		AlarmCodebuff[index++] = 0x04;
		AlarmCodebuff[index++] = 0x08;

		// 写入flash


		index = 0;
	}

}

/***********************************************************
**name:	APP_MONITOR_DevHWAlarmSta
**describe:   需要实时轮询的 设备硬件告警状态监视
**output:	none
**return:		
**autor:  andiman
**date:
************************************************************/
void APP_MONITOR_DevHWSta(INT32U period_ms)
{
	INT8U i=0,dev_alarm_flag;
	static INT32U gps_timeout_cnt = 0;
	static INT8U dev_alarm_buff[6]={0};
	INT8U alarm_high_flag = 0;
	static INT8U alarm_buzzer_refresh = 0;
	static INT8U gps_alarm_flag = 0;
	static INT8U limit_alarm_cnt[3]={0};
	
// 查询电子码盘告警信息
	// 方位
	if( ACUDevInfo.DeviceInfo.EleccoderInfo[0].EleccoderStat.link_stat == COMM_UNLINK )
	{
		dev_alarm_buff[0] |= 0x01;
		if(  (ACUDevInfo.ACUCurrSta.dev_alarm_buff[0] & 0x01) != (dev_alarm_buff[0] & 0x01))
			APP_WarnLOG_SaveCode(LOG_WARN_CODER_AZ);
		alarm_high_flag = 1;
	}
	else
		dev_alarm_buff[0] &= ~0x01;

	// 俯仰
	if( ACUDevInfo.DeviceInfo.EleccoderInfo[1].EleccoderStat.link_stat == COMM_UNLINK )
	{
		dev_alarm_buff[0] |= 0x02;
		if(  (ACUDevInfo.ACUCurrSta.dev_alarm_buff[0] & 0x02) != (dev_alarm_buff[0] & 0x02))
			APP_WarnLOG_SaveCode(LOG_WARN_CODER_EL);
		alarm_high_flag = 1;
	}
	else
		dev_alarm_buff[0] &= ~0x02;

	// 极化
	if( ACUDevInfo.ACUParam.AntennaParam.elec_coder_num == 0x03)
	{
		if( ACUDevInfo.DeviceInfo.EleccoderInfo[2].EleccoderStat.link_stat == COMM_UNLINK )
		{
			dev_alarm_buff[0] |= 0x04;
			if(  (ACUDevInfo.ACUCurrSta.dev_alarm_buff[0] & 0x04) != (dev_alarm_buff[0] & 0x04))
				APP_WarnLOG_SaveCode(LOG_WARN_CODER_PL);
			alarm_high_flag = 1;
		}
		else
			dev_alarm_buff[0] &= ~0x04;
	}
	else
		dev_alarm_buff[0] &= ~0x04;

// 信标机相关查询
	if( ACUDevInfo.DeviceInfo.BeaconInfo.BeaconStat.link_stat == COMM_UNLINK)
	{
		dev_alarm_buff[1] |= 0x01;
		if(  (ACUDevInfo.ACUCurrSta.dev_alarm_buff[1] & 0x01) != (dev_alarm_buff[1] & 0x01))
			APP_WarnLOG_SaveCode(LOG_WARN_BEACON_ERR);
		alarm_high_flag = 1;
	}
	else
		dev_alarm_buff[1] &= ~0x01;

//主控板相关查询
	// EEPROM
	if( ACUDevInfo.DeviceInfo.EEPROMInfo.link_stat == COMM_UNLINK)
	{
		dev_alarm_buff[2] |= 0x01;
		if(  (ACUDevInfo.ACUCurrSta.dev_alarm_buff[2] & 0x01) != (dev_alarm_buff[2] & 0x01))
			APP_WarnLOG_SaveCode(LOG_WARN_ACU_EEPROM_ERR);
	}
	else
		dev_alarm_buff[2] &= ~0x01;

	// FLASH
	if( ACUDevInfo.DeviceInfo.SPIFlashInfo.link_stat == COMM_UNLINK)
		dev_alarm_buff[2] |= 0x02;
	else
		dev_alarm_buff[2] &= ~0x02;

	// PDU相关通信
	if( ACUDevInfo.DeviceInfo.PDUommStat.link_stat == COMM_UNLINK)
	{
		dev_alarm_buff[3] |= 0x01;
		alarm_high_flag = 1;
		if(  (ACUDevInfo.ACUCurrSta.dev_alarm_buff[3] & 0x01) != (dev_alarm_buff[3] & 0x01))
			APP_WarnLOG_SaveCode(LOG_WARN_PDU_ERR);

		if( (ACUDevInfo.ACUCurrSta.dev_admin_sta == 0x02) || ( ACUDevInfo.ACUCurrSta.DevAdmin.admin_sta == 0x02 ))
		{
			ACUDevInfo.ACUCurrSta.dev_admin_sta = 0x00;
			ACUDevInfo.ACUCurrSta.DevAdmin.admin_sta = 0x00;
			ACUDevInfo.ACUCurrSta.DevAdmin.mode_sta = 0x00;
		}

	
	}
	else
	{
		dev_alarm_buff[3] &= ~0x01;
		if( ACUDevInfo.DeviceInfo.EleccoderInfo[motor_position_id-1].EleccoderStat.link_stat == COMM_LINK )
		{
			// 软限位告警以及处理
			if( ACUDevInfo.ACUCurrSta.antenna_position <= ACUDevInfo.ACUParam.AntennaParam.motor_position_start )
			{
				limit_alarm_cnt[motor_position_id-1 ]++;
				if( limit_alarm_cnt[motor_position_id-1 ] >= 3 )
				{
					if( ACUDevInfo.ACUCurrSta.motor_limit_sta[motor_position_id-1] != ANTENNA_DIR_LEFT_DOWN_ANTICLOCK_LIMIT)
					{
						dev_alarm_buff[5] |= 0x04;
						ACUDevInfo.ACUCurrSta.motor_limit_sta[motor_position_id-1] = ANTENNA_DIR_LEFT_DOWN_ANTICLOCK_LIMIT;
						if( ACUDevInfo.ACUCurrSta.motor_move[motor_position_id-1] == ANTENNA_DIR_LEFT_DOWN_ANTICLOCK )
							Antenna_DriveEnd(motor_position_id,ANTENNA_DIR_LEFT_DOWN_ANTICLOCK);

						APP_WarnLOG_SaveCode(LOG_WARN_PDU_MOTOR_AZ_LEFT_LIMIT);

						alarm_high_flag = 1;
					}
					limit_alarm_cnt[motor_position_id-1 ] = 0;
				}
			}
			else if(ACUDevInfo.ACUCurrSta.antenna_position >= ACUDevInfo.ACUParam.AntennaParam.motor_position_end)	
			{
				limit_alarm_cnt[motor_position_id-1 ]++;
				if( limit_alarm_cnt[motor_position_id-1 ] >= 3 )
				{
					if(  ACUDevInfo.ACUCurrSta.motor_limit_sta[motor_position_id-1] != ANTENNA_DIR_RIGHT_UP_CLOCK_LIMIT)
					{
						dev_alarm_buff[5] |= 0x08;
						ACUDevInfo.ACUCurrSta.motor_limit_sta[motor_position_id-1] = ANTENNA_DIR_RIGHT_UP_CLOCK_LIMIT;
						if( ACUDevInfo.ACUCurrSta.motor_move[motor_position_id-1] == ANTENNA_DIR_RIGHT_UP_CLOCK )
							Antenna_DriveEnd(motor_position_id,ANTENNA_DIR_RIGHT_UP_CLOCK);

						APP_WarnLOG_SaveCode(LOG_WARN_PDU_MOTOR_AZ_RIGHT_LIMIT);

						alarm_high_flag = 1;
					}
					limit_alarm_cnt[motor_position_id-1 ]=0;
				}
				
			}
			else
			{
				ACUDevInfo.ACUCurrSta.motor_limit_sta[motor_position_id-1] = 0x00;
				dev_alarm_buff[5] &= ~0x0C;
				limit_alarm_cnt[motor_position_id-1 ]=0;
			}
		}
		else
		{
			ACUDevInfo.ACUCurrSta.motor_limit_sta[motor_position_id-1] = 0x00;
			dev_alarm_buff[5] &= ~0x0C;
			limit_alarm_cnt[motor_position_id-1 ]=0;
		}

		if( ACUDevInfo.DeviceInfo.EleccoderInfo[motor_cabrage_id-1].EleccoderStat.link_stat == COMM_LINK )
		{
			
			if(ACUDevInfo.ACUCurrSta.antenna_cabrage <= ACUDevInfo.ACUParam.AntennaParam.motor_cabrage_start )
			{
				limit_alarm_cnt[motor_cabrage_id-1 ]++;
				if( limit_alarm_cnt[motor_cabrage_id-1 ] >= 3 )
				{
					if( ACUDevInfo.ACUCurrSta.motor_limit_sta[motor_cabrage_id-1] != ANTENNA_DIR_LEFT_DOWN_ANTICLOCK_LIMIT)
					{
						dev_alarm_buff[5] |= 0x02;
						ACUDevInfo.ACUCurrSta.motor_limit_sta[motor_cabrage_id-1] = ANTENNA_DIR_LEFT_DOWN_ANTICLOCK_LIMIT;
						if( ACUDevInfo.ACUCurrSta.motor_move[motor_cabrage_id-1] == ANTENNA_DIR_LEFT_DOWN_ANTICLOCK )
							Antenna_DriveEnd(motor_cabrage_id,ANTENNA_DIR_LEFT_DOWN_ANTICLOCK);
				
						APP_WarnLOG_SaveCode(LOG_WARN_PDU_MOTOR_EL_DOWN_LIMIT);

						alarm_high_flag = 1;
					}
					limit_alarm_cnt[motor_cabrage_id-1 ] = 0;
				}
				
				
			}
			else if(ACUDevInfo.ACUCurrSta.antenna_cabrage >= ACUDevInfo.ACUParam.AntennaParam.motor_cabrage_end )
			{
				limit_alarm_cnt[motor_cabrage_id-1 ]++;
				if( limit_alarm_cnt[motor_cabrage_id-1 ] >= 3 )
				{
					if(  ACUDevInfo.ACUCurrSta.motor_limit_sta[motor_cabrage_id-1] != ANTENNA_DIR_RIGHT_UP_CLOCK_LIMIT)
					{
						dev_alarm_buff[5] |= 0x01;
						ACUDevInfo.ACUCurrSta.motor_limit_sta[motor_cabrage_id-1] = ANTENNA_DIR_RIGHT_UP_CLOCK_LIMIT;
						if( ACUDevInfo.ACUCurrSta.motor_move[motor_cabrage_id-1] == ANTENNA_DIR_RIGHT_UP_CLOCK )
							Antenna_DriveEnd(motor_cabrage_id,ANTENNA_DIR_RIGHT_UP_CLOCK);

						APP_WarnLOG_SaveCode(LOG_WARN_PDU_MOTOR_EL_UP_LIMIT);

						alarm_high_flag = 1;
					}
					limit_alarm_cnt[motor_cabrage_id-1 ] = 0;
				}
				
			}
			else
			{
				dev_alarm_buff[5] &= ~0x03;
				ACUDevInfo.ACUCurrSta.motor_limit_sta[motor_cabrage_id-1] = 0x00;
				limit_alarm_cnt[motor_cabrage_id-1 ] = 0;
			}
		}
		else
		{
			dev_alarm_buff[5] &= ~0x03;
			ACUDevInfo.ACUCurrSta.motor_limit_sta[motor_cabrage_id-1] = 0x00;
			limit_alarm_cnt[motor_cabrage_id-1 ] = 0;
		}
	
		if( (ACUDevInfo.ACUParam.AntennaParam.elec_coder_num == 0x03) && ( ACUDevInfo.DeviceInfo.EleccoderInfo[motor_polar_id-1].EleccoderStat.link_stat == COMM_LINK ))
		{
			if(ACUDevInfo.ACUCurrSta.antenna_polar <= ACUDevInfo.ACUParam.AntennaParam.motor_polar_start )
			{
				limit_alarm_cnt[motor_polar_id-1 ]++;
				if( limit_alarm_cnt[motor_polar_id-1 ] >= 3 )
				{
					if( ACUDevInfo.ACUCurrSta.motor_limit_sta[motor_polar_id-1] != ANTENNA_DIR_LEFT_DOWN_ANTICLOCK_LIMIT)
					{
						dev_alarm_buff[5] |= 0x20;
						ACUDevInfo.ACUCurrSta.motor_limit_sta[motor_polar_id-1] = ANTENNA_DIR_LEFT_DOWN_ANTICLOCK_LIMIT;
						if( ACUDevInfo.ACUCurrSta.motor_move[motor_polar_id-1] == ANTENNA_DIR_LEFT_DOWN_ANTICLOCK )
							Antenna_DriveEnd(motor_polar_id,ANTENNA_DIR_LEFT_DOWN_ANTICLOCK);

						APP_WarnLOG_SaveCode(LOG_WARN_PDU_MOTOR_PL_DOWN_LIMIT);

						alarm_high_flag = 1;
					}
					limit_alarm_cnt[motor_polar_id-1 ] = 0;
				}
			}
			else if( ACUDevInfo.ACUCurrSta.antenna_polar >= ACUDevInfo.ACUParam.AntennaParam.motor_polar_end )
			{
				limit_alarm_cnt[motor_polar_id-1 ]++;
				if( limit_alarm_cnt[motor_polar_id-1 ] >= 3 )
				{
					if( ACUDevInfo.ACUCurrSta.motor_limit_sta[motor_polar_id-1] != ANTENNA_DIR_RIGHT_UP_CLOCK_LIMIT)
					{
						dev_alarm_buff[5] |= 0x10;
						ACUDevInfo.ACUCurrSta.motor_limit_sta[motor_polar_id-1] = ANTENNA_DIR_RIGHT_UP_CLOCK_LIMIT;
						if( ACUDevInfo.ACUCurrSta.motor_move[motor_polar_id-1] == ANTENNA_DIR_RIGHT_UP_CLOCK )
							Antenna_DriveEnd(motor_polar_id,ANTENNA_DIR_RIGHT_UP_CLOCK);
						APP_WarnLOG_SaveCode(LOG_WARN_PDU_MOTOR_PL_UP_LIMIT);

						alarm_high_flag = 1;
					}
					limit_alarm_cnt[motor_polar_id-1 ] = 0;
				}
			}
			else
			{
				dev_alarm_buff[5] &= ~0x30;
				ACUDevInfo.ACUCurrSta.motor_limit_sta[motor_polar_id-1] = 0x00;
				limit_alarm_cnt[motor_polar_id-1 ] = 0;
			}
		}
		else
		{
	
			dev_alarm_buff[5] &= ~0x30;
			ACUDevInfo.ACUCurrSta.motor_limit_sta[motor_polar_id-1] = 0x00;
			limit_alarm_cnt[motor_polar_id-1 ] = 0;
		}
	}
	// GPS
	if( ACUDevInfo.ACUParam.LocalPositionParam.data_src_flag == 0x00)
	{
		if( ACUDevInfo.DeviceInfo.GPSInfo.GPSData.valid == 'A')
		{
			gps_timeout_cnt = 0;
			ACUDevInfo.DeviceInfo.GPSInfo.GPSStat.dev_stat = WORKSTA_NORMAL;
			dev_alarm_buff[4] &= ~0x02;
			gps_alarm_flag = 0;
		}
		else
		{
			gps_timeout_cnt++;
			if( gps_timeout_cnt * period_ms / 1000 > 20 )		// 20秒钟
			{
				ACUDevInfo.DeviceInfo.GPSInfo.GPSStat.dev_stat = WORKSTA_ALARM;
				//gps_alarm_flag = 1;
				dev_alarm_buff[4] |= 0x02;
				if(  (ACUDevInfo.ACUCurrSta.dev_alarm_buff[4] & 0x02) != (dev_alarm_buff[4] & 0x02))
					APP_WarnLOG_SaveCode(LOG_WARN_PDU_GPS_POSITIONING_FAIL);
				/*
				if( (ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode == 0x01) ||(ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode == 0x02))
				{
					if(gps_alarm_flag > 0)
					{
						dev_alarm_buff[4] |= 0x02;
						if(  (ACUDevInfo.ACUCurrSta.dev_alarm_buff[4] & 0x02) != (dev_alarm_buff[4] & 0x02))
							APP_WarnLOG_SaveCode(LOG_WARN_PDU_GPS_POSITIONING_FAIL);
					}
				}
				*/
				
			}
		}
	}
	else
	{
		if( (dev_alarm_buff[4] & 0x02) > 0 )
		{
			dev_alarm_buff[4] &= ~0x02;
			gps_alarm_flag = 0;
		}
		
	}

	



	for(i=0;i<sizeof(dev_alarm_buff);i++)
	{
		if( dev_alarm_buff[i] == 0 )
		{
			dev_alarm_flag = 0;
			continue;
		}
		else
		{
			dev_alarm_flag = 1;
			break;
		}
	}
	if( ACUDevInfo.ACUCurrSta.dev_alarm_flag != dev_alarm_flag)
	{
		ACUDevInfo.ACUCurrSta.dev_alarm_flag = dev_alarm_flag;
		ACUDevInfo.ACUCurrSta.dev_alarm_reflash_flag[0] =1;
		ACUDevInfo.ACUCurrSta.dev_alarm_reflash_flag[1] =1;
		alarm_buzzer_refresh = 1;
	}

	if( ACUDevInfo.ACUCurrSta.dev_alarm_flag > 0)
	{
		if( memcmp(dev_alarm_buff,ACUDevInfo.ACUCurrSta.dev_alarm_buff,sizeof(dev_alarm_buff)) != 0)
		{
			ACUDevInfo.ACUCurrSta.dev_alarm_reflash_flag[0] =1;
			ACUDevInfo.ACUCurrSta.dev_alarm_reflash_flag[1] =1;
			alarm_buzzer_refresh = 1;
		}


		if( ACUDevInfo.ACUCurrSta.dev_alarm_reflash_flag > 0 )
		{
			memcpy( ACUDevInfo.ACUCurrSta.dev_alarm_buff,dev_alarm_buff ,sizeof(dev_alarm_buff));
		}

		if( alarm_high_flag == 1)
		{
			//if( alarm_high_flag_old == 0)
			{
				if( Task_Run_Flag[0] > 0 )
				{
					if( Antenna_AllDriveEnd() == FAIL)
						Antenna_AllDriveEnd();
					TaskSatelliteTrackDel();
				}
				if( Task_Run_Flag[1] > 0 )
				{
					if( Antenna_AllDriveEnd() == FAIL)
						Antenna_AllDriveEnd();
					TaskSatGradientTrackDel();
				}
				if( Task_Run_Flag[2] > 0 )
				{
					if( Antenna_AllDriveEnd() == FAIL)
						Antenna_AllDriveEnd();
					TaskAntennaRecordGotoDel();
				}
				

				if(ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode >= 0x01) 
				{
					ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode = 0x00;
					ACUDevInfo.ACUCurrSta.dev_work_sta = 0x00;

					APP_OptLOG_SaveCode(LOG_OPT_STANDBY_MODE_BY_ACU);
				}
				

				if( alarm_buzzer_refresh > 0)
				{
					APP_BUZZER_Switch(ENABLE);
					APP_BUZZER_BeepOpt(50,500,1,10000);
					alarm_buzzer_refresh = 0;
				}
			}
			
		}
		else
		{
			if( alarm_buzzer_refresh > 0)
			{
				APP_BUZZER_Switch(ENABLE);
				APP_BUZZER_BeepOpt(50,500,1,10000);
				alarm_buzzer_refresh = 0;
			}
		}

	}
	else
	{
		memset(ACUDevInfo.ACUCurrSta.dev_alarm_buff,0,sizeof(dev_alarm_buff));
		if( alarm_buzzer_refresh > 0)
		{
			APP_BUZZER_Switch(DISABLE);
			APP_BUZZER_BeepOpt(0,0,0,0);
			alarm_buzzer_refresh = 0;
		}
	}

	if( (ACUDevInfo.ACUCurrSta.dev_work_sta > 0) &&( ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode == 0x00))
		ACUDevInfo.ACUCurrSta.dev_work_sta = 0;
	

}


/***********************************************************
**name:	APP_MONITOR_DevMotorMonitor
**describe:   电机监控
**output:	none
**return:		
**autor:  andiman
**date:
************************************************************/
void APP_MONITOR_DevMotorMonitor(INT32U period_ms)
{
	INT8U i;
	static FP32 old_degree[3]={0};
	static INT32U scan_cnt=0;
	static INT8U err_cnt[3] = {0};

	if( ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode == 0x00)
	{
		scan_cnt++;
		if( period_ms * scan_cnt>= 1000 )
		{
			scan_cnt = 0;
			for(i=0;i<2;i++)
			{	
				if(	(old_degree[i] > ACUDevInfo.DeviceInfo.EleccoderInfo[i].EleccoderData.degree + 0.008 ) ||
					(old_degree[i] < ACUDevInfo.DeviceInfo.EleccoderInfo[i].EleccoderData.degree - 0.008 ) )
				{
					old_degree[i] = ACUDevInfo.DeviceInfo.EleccoderInfo[i].EleccoderData.degree;
					err_cnt[i]++;
					if( err_cnt[i] >= 10 )
					{
						err_cnt[i] = 0;
						Antenna_DriveEnd(i+1,0x00);
					}
				}
				else
				{
					err_cnt[i] = 0;
				}

			}

			if( ACUDevInfo.ACUParam.AntennaParam.elec_coder_num >= 3 )
			{
				if(	(old_degree[2] > ACUDevInfo.DeviceInfo.EleccoderInfo[2].EleccoderData.degree + 0.2 ) ||
					(old_degree[2] < ACUDevInfo.DeviceInfo.EleccoderInfo[2].EleccoderData.degree - 0.2 ) )
				{
					old_degree[2] = ACUDevInfo.DeviceInfo.EleccoderInfo[2].EleccoderData.degree;
					err_cnt[2]++;
					if( err_cnt[2] >= 10 )
					{
						err_cnt[2] = 0;
						Antenna_DriveEnd(3,0x00);
					}
				}
				else
				{
					err_cnt[2] = 0;
				}
			
			}
		}
	}

}



/***********************************************************
**name:	APP_ElecEncoder_DataAddDir_Adjust
**describe: 
**input:		
**output:			data:  获取到的实时数据
**return:  FAIL:   SUCC
**autor:  andiman
**date:
************************************************************/
int APP_ElecEncoder_DataAddDir_Adjust( INT8U antenna_part, INT8U antenna_dir)
{
	INT32U databuff[9]={0},i;
	INT8U   add_dir_will,add_flag[3],add_flag_now,index=0;
	int sta = SUCC;
	INT8U count_mode;

//	if( Antenna_DriveStart(antenna_part,antenna_dir) == FAIL)
//		return FAIL;

	if( antenna_dir == ANTENNA_DIR_RIGHT_UP_CLOCK )
		add_dir_will = 1;
	else
		add_dir_will = 0;
		
	for( i=0;i<4;i++)
	{
		databuff[i] = ACUDevInfo.DeviceInfo.EleccoderInfo[antenna_part-1].EleccoderData.data;
		if( antenna_part == motor_polar_id)
			BSP_Delay_ms(800);
		else
			BSP_Delay_ms(500);
	}

	if( (( databuff[0]   == databuff[1]   ) &&  ( databuff[0]   == databuff[ 2]  ))  || 
		(( databuff[1]   == databuff[ 2]  ) &&( databuff[1]   == databuff[ 3]  )))
		return FAIL;

	if( databuff[0] < databuff[1] )
		add_flag[index++] = 1;
	else
		add_flag[index++] = 0;

	if( databuff[1] < databuff[2] )
		add_flag[index++] = 1;
	else
		add_flag[index++] = 0;

	if( databuff[2] < databuff[3] )
		add_flag[index++] = 1;
	else
		add_flag[index++] = 0;
	

	if(add_flag[0] == add_flag[1] ) 
		add_flag_now = add_flag[0];
	else
	{
		if( add_flag[0] == add_flag[2] ) 
			add_flag_now = databuff[0];
		else
			add_flag_now = databuff[1];
	}

	BSP_Delay_ms(2);

//	if( Antenna_DriveEnd(antenna_part,antenna_dir) == FAIL)
//		return FAIL;

	APP_ElecEncoder_DataAddDir_Get(antenna_part,&count_mode);
	BSP_Delay_ms(10);
	if( add_dir_will != add_flag_now )
	{
		
		if( count_mode == 0xE1 )
		{
			sta = APP_ElecEncoder_DataAddDir_Set(antenna_part,0xE2);
			count_mode = 0xE2;
			if(sta == FAIL)
			{
				BSP_Delay_ms(10);
				sta = APP_ElecEncoder_DataAddDir_Set(antenna_part,0xE2);
				if(sta == FAIL)
					return FAIL;
			}
		}
		else if( count_mode == 0xE2 )
		{
			sta = APP_ElecEncoder_DataAddDir_Set(antenna_part,0xE1);
			count_mode = 0xE1;
			if(sta == FAIL)
			{	
				BSP_Delay_ms(10);
				sta = APP_ElecEncoder_DataAddDir_Set(antenna_part,0xE1);
				if(sta == FAIL)
					return FAIL;
			}
		}
		else
		{
			sta = APP_ElecEncoder_DataAddDir_Set(antenna_part,0xE1);
			count_mode = 0xE1;
			if(sta == FAIL)
			{
				BSP_Delay_ms(10);
				sta = APP_ElecEncoder_DataAddDir_Set(antenna_part,0xE1);
				if(sta == FAIL)
					return FAIL;
			}
		}

		BSP_Delay_ms(100);
	}

	if( APP_PARAM_ElecCode_count_mode_Save(antenna_part,count_mode) ==FAIL)
	{
		BSP_Delay_ms(10);
		APP_PARAM_ElecCode_count_mode_Save(antenna_part,count_mode);
	}

	ACUDevInfo.DeviceInfo.EleccoderInfo[antenna_part-1].EleccoderConf.count_mode = count_mode;

	return sta;
}

#if 1
INT32U APP_Beer(INT32U money,INT32U price, INT32U CapsTo1Beer, INT32U BottlesTo1Beer)
{
	INT32U beer,cap,bottle;
	INT32U beer_total_num=0;

	beer = money / price;
	cap = beer;
	bottle = beer;
	beer_total_num += beer;
	beer = 0;

	while(1)
	{
		beer = cap / CapsTo1Beer;
		cap = cap % CapsTo1Beer;

		beer += bottle / BottlesTo1Beer;
		bottle = bottle % BottlesTo1Beer;

		beer_total_num += beer;

		cap += beer;
		bottle += beer;

		if( (cap < CapsTo1Beer ) && (bottle < BottlesTo1Beer))
			break;
	}

	return beer_total_num;
}
#else
INT32U APP_Beer(INT32U money,INT32U price, INT32U CapsTo1Beer, INT32U BottlesTo1Beer,INT8U borrow_flag)
{
	INT32U beer,cap,bottle;
	INT32U beer_total_num=0;
	
	INT32U borrow_num=0,i;
	INT32U beer_new,cap_new,bottle_new;

	INT32U temp_beer_total_num = 0;
	

	beer = money / price;
	cap = beer;
	bottle = beer;
	beer_total_num += beer;
	beer = 0;

	while(1)
	{
		beer = cap / CapsTo1Beer;
		cap = cap % CapsTo1Beer;

		beer += bottle / BottlesTo1Beer;
		bottle = bottle % BottlesTo1Beer;

		beer_total_num += beer;

		cap += beer;
		bottle += beer;

		if( (cap < CapsTo1Beer ) && (bottle < BottlesTo1Beer))
		{
			if( borrow_flag > 1 )
			{
				for(i=1;i<money/price;i++)
				{
					borrow_num = i;
					cap_new = cap + borrow_num;
					bottle_new = bottle + borrow_num;
					temp_beer_total_num = beer_total_num;
					while(1)
					{
						beer_new =  cap_new / CapsTo1Beer;
						cap_new = cap_new % CapsTo1Beer;

						beer_new += bottle_new / BottlesTo1Beer;
						bottle_new = bottle_new % BottlesTo1Beer;

						temp_beer_total_num += beer_new;

						if( beer_new > borrow_num ) 
						{
							if(( beer_new - borrow_num +  cap_new < CapsTo1Beer ) && ( beer_new - borrow_num +  bottle_new < BottlesTo1Beer ))
							{
								beer_total_num = 
							}
						}
					}
				}
			}
		}
	}

	return beer_total_num;
}
#endif 
