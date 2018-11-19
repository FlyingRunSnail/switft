


#define APP_PDUCOMM_DEF

#include <app_pducomm.h>
#include <app_tool.h>
#include <hw_pducomm.h>
#include <app_log.h>


#define		APP_PDUCOMM_TIMEOUT_MS		200

OS_SEM APP_PDU_Send_Sem;


/***********************************************************
**name:	APP_PDUCOMM_Init
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void APP_PDUCOMM_Init(void)
{
	INT8U sendbuff[2]={0};
	SWIFT_UART4_Init(115200);

	BSP_Delay_ms(100);

	SWIFT_UART4_SendBuff(sendbuff, 2);

	BSP_OS_Sem_Creat(&APP_PDU_Send_Sem,"APP_PDU_Send_Sem",1);
}


/***********************************************************
**name:	APP_PDUCOMM_VersionQue
**describe:   版本查询
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
//int APP_PDUCOMM_VersionQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen)
int APP_PDUCOMM_VersionQue(void)
{
	
	INT8U sendbuff[32],revbuff[50];
	INT32U len;
	INT8U index = 0;
	INT8U checksum = 0;

	BSP_OS_Sem_Pend(&APP_PDU_Send_Sem, 0);

	// 帧头
	sendbuff[index++] = 0xBC;

	// 帧数据长度 
	sendbuff[index++] = 0x03;

	// 命令字  0x0C0A
	sendbuff[index++] = 0x0C;
	sendbuff[index++] = 0x0A;

	// 长度
	sendbuff[index++] = 0x00;

	// 校验和
	checksum = TOOL_CheckSum8BitGet(sendbuff,index);
	sendbuff[index++] = checksum;

	// 帧尾
	sendbuff[index++] = 0xCB;

	SWIFT_UART4_SendBuff(sendbuff, index);

	// pdu 收到命令后需要马上回复，表示收到
	if( BSP_OS_Sem_Pend(&SWIFT_UART4_Rev_Sem,100) == FAIL)
	{
		BSP_OS_Sem_Post(&APP_PDU_Send_Sem);
		return FAIL;
	}

	SWIFT_UART4_RecvBuff(revbuff,&len);

	if(( revbuff[2] != 0x8C ) || ( revbuff[3] != 0x0A) ||  (revbuff[4] != 32))
		return FAIL;

	memcpy(ACUDevInfo.Version[1].softver,&revbuff[5],16);
	memcpy(ACUDevInfo.Version[1].hardver,&revbuff[21],16);

	BSP_OS_Sem_Post(&APP_PDU_Send_Sem);

	return SUCC;
	
}

/***********************************************************
**name:	APP_PDUCOMM_PDUDevStaQue
**describe:  
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_PDUCOMM_PDUDevStaQue(void)
{
	
	INT8U sendbuff[32],revbuff[50];
	INT32U len;
	INT8U index = 0;
	INT8U checksum = 0;

	BSP_OS_Sem_Pend(&APP_PDU_Send_Sem, 0);

	// 帧头
	sendbuff[index++] = 0xBC;

	// 帧数据长度 
	sendbuff[index++] = 0x03;

	// 命令字  0x0C0A
	sendbuff[index++] = 0x0C;
	sendbuff[index++] = 0x02;

	// 长度
	sendbuff[index++] = 0x00;

	// 校验和
	checksum = TOOL_CheckSum8BitGet(sendbuff,index);
	sendbuff[index++] = checksum;

	// 帧尾
	sendbuff[index++] = 0xCB;

	SWIFT_UART4_SendBuff(sendbuff, index);

	// pdu 收到命令后需要马上回复，表示收到
	if( BSP_OS_Sem_Pend(&SWIFT_UART4_Rev_Sem,100) == FAIL)
	{
		ACUDevInfo.DeviceInfo.PDUommStat.comm_stat = COMM_UNLINK;
		ACUDevInfo.DeviceInfo.PDUommStat.link_stat = COMM_UNLINK;

		BSP_OS_Sem_Post(&APP_PDU_Send_Sem);
		return FAIL;
	}

	ACUDevInfo.DeviceInfo.PDUommStat.comm_stat = COMM_LINK;
	ACUDevInfo.DeviceInfo.PDUommStat.link_stat = COMM_LINK;

	SWIFT_UART4_RecvBuff(revbuff,&len);

	if(( revbuff[2] != 0x8C ) || ( revbuff[3] != 0x02))
	{
		BSP_OS_Sem_Post(&APP_PDU_Send_Sem);
		return FAIL;
	}

	if(  revbuff[5] > 0x00 )
	{
		if( ACUDevInfo.ACUCurrSta.dev_admin_sta != 0x02)
		{
			ACUDevInfo.ACUCurrSta.dev_admin_sta = 0x02;
			ACUDevInfo.ACUCurrSta.DevAdmin.admin_sta = 0x02;
			ACUDevInfo.ACUCurrSta.DevAdmin.mode_sta = 0x02;
			ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode = 0x01;
			ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_ip_addr = 0x00;
			ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_link_cnt = 0;
			ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_ask_ip_addr = 0;
			APP_OptLOG_SaveCode(LOG_OPT_PDU_ADMIN_ENABLE);
			
		}
	}
	else
	{
		if( ACUDevInfo.ACUCurrSta.dev_admin_sta == 0x02)
		{
			ACUDevInfo.ACUCurrSta.dev_admin_sta = 0x00;
			ACUDevInfo.ACUCurrSta.DevAdmin.admin_sta = 0x00;
			ACUDevInfo.ACUCurrSta.DevAdmin.mode_sta = 0x00;
			ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode = 0x00;
			ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_ip_addr = 0x00;
			ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_link_cnt = 0;
			ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_ask_ip_addr = 0;
			ACUDevInfo.ACUCurrSta.dev_work_sta = 0x00;

			APP_OptLOG_SaveCode(LOG_OPT_PDU_ADMIN_DISABLE);
		}
	}

	ACUDevInfo.ACUCurrSta.DevAdmin.stop_flag = revbuff[6];

	ACUDevInfo.ACUCurrSta.antenna_position_sta =  revbuff[7];
	ACUDevInfo.ACUCurrSta.antenna_cabrage_sta =  revbuff[8];
	ACUDevInfo.ACUCurrSta.antenna_polar_sta =  revbuff[9];

	BSP_OS_Sem_Post(&APP_PDU_Send_Sem);

	return SUCC;
	
}




