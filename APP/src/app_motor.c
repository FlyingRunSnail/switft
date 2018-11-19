


#include <app_motor.h>
#include <app_tool.h>
#include <devdef.h>


#define	APP_MOTOR_DEF


extern OS_SEM APP_PDU_Send_Sem;

/***********************************************************
**name:	Motor_DriveStart
**describe: 对电机启动
**input:	Motor_num: 电机编号
**output:	none
**return:  
**autor:  andiman
**date:
************************************************************/
int Motor_DriveStart(INT8U Motor_num,INT8U direction)
{
	INT8U sendbuff[32],revbuff[32];
	INT32U len;
	INT8U index = 0;
	INT8U checksum = 0;

	// 帧头
	sendbuff[index++] = 0xBC;

	// 帧数据长度 
	sendbuff[index++] = 0x05;

	// 命令字  0x0401
	sendbuff[index++] = 0x04;
	sendbuff[index++] = 0x01;

	// 长度
	sendbuff[index++] = 0x02;

	// 电机编号
	sendbuff[index++] = Motor_num;

	// 转动方向
	sendbuff[index++] = direction;

	// 校验和
	checksum = TOOL_CheckSum8BitGet(sendbuff,index);
	sendbuff[index++] = checksum;

	// 帧尾
	sendbuff[index++] = 0xCB;

	SWIFT_UART4_SendBuff(sendbuff, index);

	// pdu 收到命令后需要马上回复，表示收到
	if( BSP_OS_Sem_Pend(&SWIFT_UART4_Rev_Sem,200) == FAIL)
	{
		ACUDevInfo.DeviceInfo.PDUommStat.link_stat = COMM_UNLINK;
		return FAIL;
	}

	ACUDevInfo.DeviceInfo.PDUommStat.link_stat = COMM_LINK;

	SWIFT_UART4_RecvBuff(revbuff,&len);

	if(( revbuff[2] == 0x84 ) && ( revbuff[3] == 0x01) )
		return SUCC;


	return FAIL;

}

/***********************************************************
**name:	Motor_DriveEnd
**describe: 对电机停止驱动
**input:	Motor_num: 电机编号
**output:	none
**return:  
**autor:  andiman
**date:
************************************************************/
int  Motor_DriveEnd(INT8U Motor_num,INT8U direction)
{
	INT8U sendbuff[32],revbuff[32];
	INT32U len;
	INT8U index = 0;
	INT8U checksum = 0;

	// 帧头
	sendbuff[index++] = 0xBC;

	// 帧数据长度 
	sendbuff[index++] = 0x04;

	// 命令字  0x0401
	sendbuff[index++] = 0x04;
	sendbuff[index++] = 0x02;

	// 长度
	sendbuff[index++] = 0x01;

	// 电机编号
	sendbuff[index++] = Motor_num;

	// 校验和
	checksum = TOOL_CheckSum8BitGet(sendbuff,index);
	sendbuff[index++] = checksum;

	// 帧尾
	sendbuff[index++] = 0xCB;

	SWIFT_UART4_SendBuff(sendbuff, index);

	// pdu 收到命令后需要马上回复，表示收到
	if( BSP_OS_Sem_Pend(&SWIFT_UART4_Rev_Sem,200) == FAIL)
	{
		ACUDevInfo.DeviceInfo.PDUommStat.link_stat = COMM_UNLINK;
		return FAIL;
	}

	SWIFT_UART4_RecvBuff(revbuff,&len);

	ACUDevInfo.DeviceInfo.PDUommStat.link_stat = COMM_LINK;

	if(( revbuff[2] == 0x84 ) && ( revbuff[3] == 0x02) )
		return SUCC;

	return FAIL;

}


/***********************************************************
**name:	Motor_TimeConfigDrive
**describe: 时间配置对电机驱动
**input:	Motor_num: 电机编号
**output:	none
**return:  
**autor:  andiman
**date:
************************************************************/
int Motor_TimeConfigDrive(INT8U Motor_num,INT8U drection,INT16U ms)
{
	if( Antenna_DriveStart(Motor_num,drection) == FAIL)
		return FAIL;
		
	BSP_Delay_ms(ms);

	if( Antenna_DriveEnd(Motor_num,drection) ==FAIL)
		return FAIL;

	return SUCC;
}



/***********************************************************
**name:	Antenna_DriveStart
**describe: 天线驱动启动
**input:	antenna_part: 天线部位编号
**output:	none
**return:  
**autor:  andiman
**date:
************************************************************/
int Antenna_DriveStart(INT8U antenna_part,INT8U direction)
{
	INT8U Motor_num,motor_dir;
	int sta;

	

	if( (antenna_part != motor_position_id ) && ( antenna_part != motor_cabrage_id )&&( antenna_part != motor_polar_id ))
		return FAIL;

	if( ( direction != ANTENNA_DIR_LEFT_DOWN_ANTICLOCK ) && ( direction != ANTENNA_DIR_RIGHT_UP_CLOCK ))
		return FAIL;

	// 判断当前角度值是否已经超出范围
	if( antenna_part == motor_position_id )
	{
		if(ACUDevInfo.ACUCurrSta.antenna_position <= ACUDevInfo.ACUParam.AntennaParam.motor_position_start )
		{
			if( direction == ANTENNA_DIR_LEFT_DOWN_ANTICLOCK )
				return FAIL;
		}
		else if(ACUDevInfo.ACUCurrSta.antenna_position >= ACUDevInfo.ACUParam.AntennaParam.motor_position_end)
		{
			if( direction == ANTENNA_DIR_RIGHT_UP_CLOCK )
				return FAIL;
		}
	}
	else if( antenna_part == motor_cabrage_id )
	{
		if(ACUDevInfo.ACUCurrSta.antenna_cabrage <= ACUDevInfo.ACUParam.AntennaParam.motor_cabrage_start )
		{
			if( direction == ANTENNA_DIR_LEFT_DOWN_ANTICLOCK )
				return FAIL;
		}
		else if(ACUDevInfo.ACUCurrSta.antenna_cabrage >= ACUDevInfo.ACUParam.AntennaParam.motor_cabrage_end)
		{
			if( direction == ANTENNA_DIR_RIGHT_UP_CLOCK )
				return FAIL;
		}
	}
	if( antenna_part == motor_polar_id )
	{
		if(ACUDevInfo.ACUCurrSta.antenna_polar <= ACUDevInfo.ACUParam.AntennaParam.motor_polar_start )
		{
			if( direction == ANTENNA_DIR_LEFT_DOWN_ANTICLOCK )
				return FAIL;
		}
		else if(ACUDevInfo.ACUCurrSta.antenna_polar >= ACUDevInfo.ACUParam.AntennaParam.motor_polar_end)
		{
			if( direction == ANTENNA_DIR_RIGHT_UP_CLOCK )
				return FAIL;
		}
	}

	

	Motor_num = antenna_part;
	motor_dir = ACUDevInfo.ACUParam.AntennaParam.AntennaDirMapMotor[antenna_part-1].motor_dir[direction-1];

	BSP_OS_Sem_Pend(&APP_PDU_Send_Sem, 0);

	sta = Motor_DriveStart(Motor_num,motor_dir);

	if(sta ==SUCC)
		ACUDevInfo.ACUCurrSta.motor_move[antenna_part-1] = direction;

	BSP_OS_Sem_Post(&APP_PDU_Send_Sem);

	return sta;
}

/***********************************************************
**name:	Antenna_DriveEnd
**describe: 天线停止驱动
**input:	antenna_part: 天线部位编号
**output:	none
**return:  
**autor:  andiman
**date:
************************************************************/
int Antenna_DriveEnd(INT8U antenna_part,INT8U direction)
{
	INT8U Motor_num,motor_dir = ANTENNA_DIR_LEFT_DOWN_ANTICLOCK;
	int sta;

	BSP_OS_Sem_Pend(&APP_PDU_Send_Sem, 0);

	if( (antenna_part != motor_position_id ) && ( antenna_part != motor_cabrage_id )&&( antenna_part != motor_polar_id ))
	{
		BSP_OS_Sem_Post(&APP_PDU_Send_Sem);
		return FAIL;
	}

	//if( ( direction != ANTENNA_DIR_LEFT_DOWN_ANTICLOCK ) && ( direction != ANTENNA_DIR_RIGHT_UP_CLOCK ))
	//	return FAIL;

	Motor_num = antenna_part;
	//motor_dir = ACUDevInfo.ACUParam.AntennaParam.AntennaDirMapMotor[antenna_part-1].motor_dir[direction-1];

	sta = Motor_DriveEnd(Motor_num,motor_dir);

	if(sta ==SUCC)
		ACUDevInfo.ACUCurrSta.motor_move[antenna_part-1] = 0x00;


	BSP_OS_Sem_Post(&APP_PDU_Send_Sem);

	return sta;
}

/***********************************************************
**name:	Antenna_AllDriveEnd
**describe: 天线全部停止驱动
**input:	
**output:	none
**return:  
**autor:  andiman
**date:
************************************************************/
int Antenna_AllDriveEnd(void)
{
	int sta = SUCC;
	
	if( Antenna_DriveEnd(motor_position_id,0) == FAIL)
	{
		BSP_Delay_ms(50);
		if( Antenna_DriveEnd(motor_position_id,0) == FAIL)
			sta |= FAIL;
		
	}

	BSP_Delay_ms(50);

	if( Antenna_DriveEnd(motor_cabrage_id,0) == FAIL)
	{
		BSP_Delay_ms(50);
		if( Antenna_DriveEnd(motor_cabrage_id,0) == FAIL)
			sta |= FAIL;
		
	}

	BSP_Delay_ms(50);

	if( Antenna_DriveEnd(motor_polar_id,0) == FAIL)
	{
		BSP_Delay_ms(50);
		if( Antenna_DriveEnd(motor_polar_id,0) == FAIL)
			sta |= FAIL;
		
	}

	return sta;
}

