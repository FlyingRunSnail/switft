


#define APP_BEACON_DEF

#include <app_beacon.h>
#include <devdef.h>
#include <app_log.h>


#define		APP_BEACON_TIMEOUT_MS		25

static INT8U Beacon_Err_cnt = 0;

OS_SEM APP_Beacon_Send_Sem;

/***********************************************************
**name:	SWIFT_UART6_Init
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void APP_BEACON_Init(void)
{
	SWIFT_UART6_Init(19200);

	BSP_OS_Sem_Creat(&APP_Beacon_Send_Sem,"APP_Beacon_Send_Sem",1);

	APP_BEACON_QueAGC();
}


/***********************************************************
**name:	APP_BEACON_SetFreqCmdSend
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void APP_BEACON_SetFreqCmdSend(INT32U freq_khz)
{
	INT8U cmdbuff[15]={0};
	INT32U freq = freq_khz;

	

	cmdbuff[0] = '<';
	cmdbuff[1] = 'F';
	cmdbuff[2] = freq / 1000000   + 0X30;
	cmdbuff[3] = freq %1000000 / 100000 + 0X30;
	cmdbuff[4] = freq %100000  / 10000 + 0X30;
	cmdbuff[5] = freq %10000   / 1000 + 0x30;

	cmdbuff[6] = '.';

	cmdbuff[7] =  freq % 1000   / 100 + 0X30;
	cmdbuff[8] = freq % 100   / 10 + 0X30;

	cmdbuff[9] = freq % 10 + 0X30;

	cmdbuff[10] =0x0D;
	cmdbuff[11] =0x0A;

	SWIFT_UART6_SendBuff(cmdbuff,12);
}

/***********************************************************
**name:	APP_BEACON_SetFreqCmdResultGet
**describe:  查询设置频点结果
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_BEACON_SetFreqCmdResultGet(INT32U timeout_ms)
{
	INT8U recvbuff[100];
	INT32U recvbufflen=0;
	INT32U index=0;
	INT32U len=0;

	if(BSP_OS_Sem_Pend(&Beacon2MCU_Sem,timeout_ms)== FAIL)
	{
		SWIFT_UART6_RecvBuff(&recvbuff[index],&recvbufflen);
		return FAIL;
	}
	SWIFT_UART6_RecvBuff(&recvbuff[index],&recvbufflen);

	if( recvbufflen != 4 )
		return FAIL;

	if( recvbuff[0] != '>' )
		return FAIL;

	if( recvbuff[1] == 'T')
		return SUCC;
	else
		return FAIL;

	
}

/***********************************************************
**name:	APP_BEACON_QueBeaconCheckCmdSend
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void APP_BEACON_QueBeaconCheckCmdSend(void)
{
	INT8U cmdbuff[4]={0};

	cmdbuff[0] = '{';
	cmdbuff[1] = 'C';
	cmdbuff[2] = 0x0D;
	cmdbuff[3] = 0x0A;
	
	SWIFT_UART6_SendBuff(cmdbuff,4);
}


/***********************************************************
**name:	APP_BEACON_QueBeaconCheckResultGet
**describe:  
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_BEACON_QueBeaconCheckResultGet(INT32U timeout_ms)
{
	INT8U recvbuff[100];
	INT32U recvbufflen=0;
	INT32U index=0;
	INT32U len=0;

	if(BSP_OS_Sem_Pend(&Beacon2MCU_Sem,timeout_ms)== FAIL)
	{
		SWIFT_UART6_RecvBuff(&recvbuff[index],&recvbufflen);
		return FAIL;
	}
	SWIFT_UART6_RecvBuff(&recvbuff[index],&recvbufflen);

	if( recvbufflen != 4 )
		return FAIL;

	if( recvbuff[0] != '>' )
		return FAIL;

	if( recvbuff[1] == 'V')
		return SUCC;
	else
		return WORKSTA_ALARM;

	
}




/***********************************************************
**name:	APP_BEACON_SetAGCOutputRangeCmdSend
**describe:
**input:			agc_output_flag:  0x00 -45~-85dbm; 0x01 -38~-78dbm; 0x02 -30~-70dbm
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void APP_BEACON_SetAGCOutputRangeCmdSend(INT8U agc_output_flag)
{
	INT8U cmdbuff[5]={0};

	cmdbuff[0] = '<';
	cmdbuff[1] = 'D';
	cmdbuff[2] = agc_output_flag % 10 + 0x30;
	cmdbuff[3] = 0x0D;
	cmdbuff[4] = 0x0A;

	SWIFT_UART6_SendBuff(cmdbuff,5);
}



/***********************************************************
**name:	APP_BEACON_SetAGCOutputRangeResultGet
**describe:  
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_BEACON_SetAGCOutputRangeResultGet(INT32U timeout_ms,INT8U agc_output_flag)
{
	INT8U recvbuff[100];
	INT32U recvbufflen=0;
	INT32U index=0;
	INT32U len=0;

	if(BSP_OS_Sem_Pend(&Beacon2MCU_Sem,timeout_ms)== FAIL)
	{
		SWIFT_UART6_RecvBuff(&recvbuff[index],&recvbufflen);
		return FAIL;
	}
	SWIFT_UART6_RecvBuff(&recvbuff[index],&recvbufflen);

	if( recvbufflen != 5 )
		return FAIL;

	if( recvbuff[0] != '>' )
		return FAIL;

	if( (recvbuff[1] == '8' ) &&  (recvbuff[2] == '5' ) && (agc_output_flag == 0))
		return SUCC;
	else if( (recvbuff[1] == '7' ) &&  (recvbuff[2] == '8' ) && (agc_output_flag == 1))
		return SUCC;
	else if( (recvbuff[1] == '7' ) &&  (recvbuff[2] == '0' ) && (agc_output_flag == 2))
		return SUCC;
	else 
		return FAIL;
	
}




/***********************************************************
**name:	APP_BEACON_QueAGCCmdSend
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void APP_BEACON_QueAGCCmdSend(void)
{
	INT8U cmdbuff[6]={0};

	cmdbuff[0] = '<';
	cmdbuff[1] = 'A';
	cmdbuff[2] = 'S';
	cmdbuff[3] = 'K';
	cmdbuff[4] = 0x0D;
	cmdbuff[5] = 0x0A;

	SWIFT_UART6_SendBuff(cmdbuff,6);
}




/***********************************************************
**name:	APP_BEACON_SetAGCOutputRangeResultGet
**describe:  
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_BEACON_QueAGCResultGet(INT32U timeout_ms,INT32U *agc_mv,INT32U *agc_db)
{
	INT8U recvbuff[100];
	INT32U recvbufflen=0;
	INT32U index=0;
	INT32U len=0;

	if(BSP_OS_Sem_Pend(&Beacon2MCU_Sem,timeout_ms)== FAIL)
	{
		SWIFT_UART6_RecvBuff(&recvbuff[index],&recvbufflen);
		return FAIL;
	}
	SWIFT_UART6_RecvBuff(&recvbuff[index],&recvbufflen);

	if( recvbufflen < 10 )
		return FAIL;

	if( recvbuff[0] != '>' )
		return FAIL;

	if( (recvbuff[1]!= 'D' ) &&  (recvbuff[1] != 'E' ))
	{
		*agc_mv = 0;
		*agc_db = 0;
		return FAIL;
	}

	if(recvbuff[1]== 'D' )
	{
		*agc_mv = 0;
		*agc_db = 0;
		return SUCC;
	}

	if( (recvbuff[2] > 0x39 ) || (recvbuff[2] < 0x30 ))
	{
		*agc_mv = 0;
		*agc_db = 0;
		return FAIL;
	}

	*agc_mv = (recvbuff[2] - 0x30) *1000 + (recvbuff[4]-0x30)*100 + (recvbuff[5]-0x30)*10;			//毫伏为单位

	if( recvbufflen == 10)
		*agc_db = (recvbuff[6]-0x30)*1000 + (recvbuff[7]-0x30)*100;
	else
		*agc_db = (recvbuff[6]-0x30)*1000 + (recvbuff[7]-0x30)*100 + (recvbuff[8]-0x30)*10;
	
	return SUCC;
}


/***********************************************************
**name:	APP_BEACON_SetFreq
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_BEACON_SetFreq(INT32U freq)
{
	int status;

	BSP_OS_Sem_Pend(&APP_Beacon_Send_Sem, 0);

	if( (freq < 950000) || (freq > 2150000))
	{
		//APP_WarnLOG_Save2Flash_Type(WARN_BEACON_FREQ_SET_FAIL);
		//ACUDevInfo.DeviceInfo.BeaconInfo.BeaconStat.dev_stat |= 0x01;
		BSP_OS_Sem_Post(&APP_Beacon_Send_Sem);
		return FAIL;
	}
	
	APP_BEACON_SetFreqCmdSend(freq);
	status = APP_BEACON_SetFreqCmdResultGet(160*APP_BEACON_TIMEOUT_MS);

	if( status == SUCC)
	{
		ACUDevInfo.DeviceInfo.BeaconInfo.BeaconConf.work_freq = freq;
		ACUDevInfo.DeviceInfo.BeaconInfo.BeaconStat.link_stat = COMM_LINK;
		Beacon_Err_cnt = 0;
	}
	else
	{
		//APP_WarnLOG_Save2Flash_Type(WARN_BEACON_FREQ_SET_FAIL);
		ACUDevInfo.DeviceInfo.BeaconInfo.BeaconStat.comm_stat = 0x01;

		BSP_OS_Sem_Post(&APP_Beacon_Send_Sem);
		ACUDevInfo.DeviceInfo.BeaconInfo.BeaconStat.dev_stat |= 0x01;
		ACUDevInfo.ACUCurrSta.dev_alarm_buff[1] |= 0x02;
		APP_WarnLOG_SaveCode(LOG_WARN_BEACON_SET_FREQ_FAIL);
		return FAIL;
	}
	ACUDevInfo.DeviceInfo.BeaconInfo.BeaconStat.dev_stat &= 0X01;
	BSP_OS_Sem_Post(&APP_Beacon_Send_Sem);

	ACUDevInfo.ACUCurrSta.dev_alarm_buff[1] &= ~0x02;

	return status;
}

/***********************************************************
**name:	APP_BEACON_QueBeaconCheck
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_BEACON_QueBeaconCheck(void)
{
	int status;

	BSP_OS_Sem_Pend(&APP_Beacon_Send_Sem, 0);
	
	APP_BEACON_QueBeaconCheckCmdSend();
	status = APP_BEACON_QueBeaconCheckResultGet(APP_BEACON_TIMEOUT_MS);

	if( status == SUCC)
	{
		ACUDevInfo.DeviceInfo.BeaconInfo.BeaconStat.link_stat = COMM_LINK;
		//SATseekerDevInfo.DeviceInfo.BeaconInfo.BeaconStat.dev_stat = WORKSTA_NORMAL;
	}
	else if( status == WORKSTA_ALARM )
	{
		ACUDevInfo.DeviceInfo.BeaconInfo.BeaconStat.dev_stat = WORKSTA_ALARM;
		
	}
	
	else
	{
		Beacon_Err_cnt++;
		if(  Beacon_Err_cnt == 10 )
		{
			//APP_WarnLOG_Save2Flash_Type(WARN_BEACON_UNLINK);
			ACUDevInfo.DeviceInfo.BeaconInfo.BeaconStat.comm_stat = 0x01;
		}
		if(  Beacon_Err_cnt > 100 )
			Beacon_Err_cnt = 20;

		BSP_OS_Sem_Post(&APP_Beacon_Send_Sem);
		return FAIL;
	}

	Beacon_Err_cnt = 0;
	ACUDevInfo.DeviceInfo.BeaconInfo.BeaconStat.comm_stat = 0x00;

	BSP_OS_Sem_Post(&APP_Beacon_Send_Sem);

	return status;
}




/***********************************************************
**name:	APP_BEACON_SetAGCOutputRange
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_BEACON_SetAGCOutputRange(INT8U agc_output_flag)
{
	int status;
	INT8U out_flag;

	BSP_OS_Sem_Pend(&APP_Beacon_Send_Sem, 0);

	if( agc_output_flag == 0x85 )
		out_flag = 0x00;
	else if( agc_output_flag == 0x78 )
		out_flag = 0x01;
	else
		out_flag = 0x02;
	APP_BEACON_SetAGCOutputRangeCmdSend(out_flag);
	status = APP_BEACON_SetAGCOutputRangeResultGet(100*APP_BEACON_TIMEOUT_MS,out_flag);

	if( status == SUCC)
	{
		ACUDevInfo.DeviceInfo.BeaconInfo.BeaconConf.agc_output_range = agc_output_flag;
		//SATseekerDevInfo.DeviceInfo.BeaconInfo.BeaconStat.dev_stat = WORKSTA_NORMAL;
		ACUDevInfo.DeviceInfo.BeaconInfo.BeaconStat.link_stat = COMM_LINK;
	}
	else
	{
		//APP_WarnLOG_Save2Flash_Type(WARN_BEACON_OUTRANG_SET_FAIL);
		//SATseekerDevInfo.DeviceInfo.BeaconInfo.BeaconStat.comm_stat = 0x01;

		BSP_OS_Sem_Post(&APP_Beacon_Send_Sem);
		ACUDevInfo.DeviceInfo.BeaconInfo.BeaconStat.dev_stat |= 0x02;

		ACUDevInfo.ACUCurrSta.dev_alarm_buff[1] |= 0x04;

		APP_WarnLOG_SaveCode(LOG_WARN_BEACON_SET_GAIN_FAIL);
		return FAIL;
	}

	Beacon_Err_cnt = 0;
	ACUDevInfo.DeviceInfo.BeaconInfo.BeaconStat.comm_stat = 0x00;

	BSP_OS_Sem_Post(&APP_Beacon_Send_Sem);
	
	ACUDevInfo.DeviceInfo.BeaconInfo.BeaconStat.dev_stat &= ~0x02;

	ACUDevInfo.ACUCurrSta.dev_alarm_buff[1] &= ~0x04;
	return status;
}



/***********************************************************
**name:	APP_BEACON_QueAGC(
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_BEACON_QueAGC(void)
{
	int status;
	INT32U agc_mv;
	INT32S agc_db;

	BSP_OS_Sem_Pend(&APP_Beacon_Send_Sem, 0);
	
	
	APP_BEACON_QueAGCCmdSend();
	status = APP_BEACON_QueAGCResultGet(APP_BEACON_TIMEOUT_MS,&agc_mv,&agc_db);

	

	if( status == SUCC)
	{
		ACUDevInfo.DeviceInfo.BeaconInfo.BeaconData.data_mv = agc_mv;
		//if( ACUDevInfo.DeviceInfo.BeaconInfo.BeaconData.data_mv >= 10 )
			ACUDevInfo.DeviceInfo.BeaconInfo.BeaconData.data_dbm = agc_db;
		//else
		//	ACUDevInfo.DeviceInfo.BeaconInfo.BeaconData.data_dbm = 8600;
		ACUDevInfo.DeviceInfo.BeaconInfo.BeaconStat.dev_stat = WORKSTA_NORMAL;
		ACUDevInfo.DeviceInfo.BeaconInfo.BeaconStat.link_stat = COMM_LINK;
	}
	else
	{
		
		Beacon_Err_cnt++;
		if(  Beacon_Err_cnt >= 10 )
		{
			ACUDevInfo.DeviceInfo.BeaconInfo.BeaconData.data_mv = 0;
			ACUDevInfo.DeviceInfo.BeaconInfo.BeaconData.data_dbm = 8600;
			//APP_WarnLOG_Save2Flash_Type(WARN_BEACON_UNLINK);
			ACUDevInfo.DeviceInfo.BeaconInfo.BeaconStat.comm_stat = COMM_UNLINK;
			ACUDevInfo.DeviceInfo.BeaconInfo.BeaconStat.link_stat = COMM_UNLINK;
		}
		if(  Beacon_Err_cnt > 100 )
			Beacon_Err_cnt = 20;

		BSP_OS_Sem_Post(&APP_Beacon_Send_Sem);
		return FAIL;
	}

	Beacon_Err_cnt = 0;
	ACUDevInfo.DeviceInfo.BeaconInfo.BeaconStat.comm_stat = 0x00;

	ACUDevInfo.ACUCurrSta.beacon_rxpower_mv = ACUDevInfo.DeviceInfo.BeaconInfo.BeaconData.data_mv;
	agc_db = ACUDevInfo.DeviceInfo.BeaconInfo.BeaconData.data_dbm;
	ACUDevInfo.ACUCurrSta.beacon_rxpower_db = (-agc_db)/100;

	BSP_OS_Sem_Post(&APP_Beacon_Send_Sem);
	
	return status;
}


