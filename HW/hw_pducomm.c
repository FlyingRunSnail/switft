


#include <hw_pducomm.h>
#include <app_log.h>
#include <devdef.h>

#define	HW_PDU_DEF


static str_UART_GPIO_HARD	SWIFT_UART4_GPIO;

#define		SWIFTUART4BuffSize		64
INT8U	SWIFTUART4Buff[SWIFTUART4BuffSize];
str_UART_Buff		SWIFT_UART4_Buff;

OS_SEM SWIFT_UART4_Send_Sem;

OS_SEM SWIFT_UART4_Rev_Sem;


/***********************************************************
**name:	SWIFT_UART4_UrgentDeal
**describe: 
**input:	
**output:	none
**return:  FAIL:   SUCC
**autor:  andiman
**date:
************************************************************/
void SWIFT_UART4_UrgentDeal(void)
{
	INT8U	rxbuff[32],txbuff[32];
	INT32U len;
	INT16U cmd;

	SWIFT_UART4_RecvBuff(rxbuff,&len);

//	if( rxbuff[1] + 4 != len )
//	{
//		return;
//	}

	cmd = (rxbuff[2] << 8) + rxbuff[3];

	switch( cmd )
	{
		/*
		case 0x0D01:
			if( ACUDevInfo.ACUCurrSta.antenna_position_sta != rxbuff[5])
				ACUDevInfo.ACUCurrSta.antenna_position_sta =  rxbuff[5];
			if( ACUDevInfo.ACUCurrSta.antenna_cabrage_sta != rxbuff[6])
				ACUDevInfo.ACUCurrSta.antenna_cabrage_sta =  rxbuff[6];
			if( ACUDevInfo.ACUCurrSta.antenna_polar_sta != rxbuff[7])
				ACUDevInfo.ACUCurrSta.antenna_polar_sta =  rxbuff[7];
			break;
		
		case 0x0D06:
			
			if(  rxbuff[5] > 0x00 )
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
					APP_OptLOG_SaveCode(LOG_OPT_PDU_ADMIN_DISABLE);
				}
			}
			
			break;
			
		case 0x0D08:
			if(  rxbuff[5] > 0x00 )
			{
				if( ACUDevInfo.ACUCurrSta.DevAdmin.stop_flag != 0x01)
				{
					
					if( ACUDevInfo.ACUCurrSta.dev_admin_sta != 0x02 )
					{
						ACUDevInfo.ACUCurrSta.dev_admin_sta = 0x02;
						ACUDevInfo.ACUCurrSta.DevAdmin.admin_sta = 0x02;
						ACUDevInfo.ACUCurrSta.DevAdmin.mode_sta = 0x01;
						ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_ip_addr = 0x00;
						ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_link_cnt = 0;
						ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_ask_ip_addr = 0;
						APP_OptLOG_SaveCode(LOG_OPT_PDU_ADMIN_ENABLE);
					}
					
					ACUDevInfo.ACUCurrSta.DevAdmin.stop_flag = 0x01;
					APP_OptLOG_SaveCode(LOG_OPT_PDU_STOP_ENABLE);
				}
			}
			else
			{
				if( ACUDevInfo.ACUCurrSta.DevAdmin.stop_flag > 0x00)
				{
					ACUDevInfo.ACUCurrSta.dev_admin_sta = 0x02;
					ACUDevInfo.ACUCurrSta.DevAdmin.admin_sta = 0x02;
					ACUDevInfo.ACUCurrSta.DevAdmin.mode_sta = 0x02;
					ACUDevInfo.ACUCurrSta.DevAdmin.stop_flag = 0x00;
					ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_ip_addr = 0x00;
					ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_link_cnt = 0;
					ACUDevInfo.ACUCurrSta.DevAdmin.pc_admin_ask_ip_addr = 0;
					APP_OptLOG_SaveCode(LOG_OPT_PDU_STOP_DISABLE);
				}
			}
			break;
		*/
		default:
			break;
	}

	ACUDevInfo.DeviceInfo.PDUommStat.link_stat = COMM_LINK;
}


/***********************************************************
**name:	SWITFT_UART4_IntHandler
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void SWITFT_UART4_IntHandler(void)
{
 	if (USART_GetITStatus(SWIFT_UART4_GPIO.USARTx, USART_IT_RXNE) != RESET)
	{
		SWIFT_UART4_Buff.pbuff[SWIFT_UART4_Buff.write_p++] = USART_ReceiveData(SWIFT_UART4_GPIO.USARTx);
		if( SWIFT_UART4_Buff.write_p >= SWIFTUART4BuffSize )
			SWIFT_UART4_Buff.write_p = 0;
		if( SWIFT_UART4_Buff.pbuff[0] == 0xBC ) 
		{
			if( SWIFT_UART4_Buff.write_p >= 2 )
			{
				if( SWIFT_UART4_Buff.write_p >= SWIFT_UART4_Buff.pbuff[1]+4 )
				{
					if( SWIFT_UART4_Buff.pbuff[SWIFT_UART4_Buff.pbuff[1]+3] == 0xCB )
					{
						// 涉及PDU紧急事项，在中断中直接处理 
						if( (SWIFT_UART4_Buff.pbuff[2] & 0x0F) == 0x0D )
						{
							SWIFT_UART4_UrgentDeal();
						}
						else
							BSP_OS_Sem_Post(&SWIFT_UART4_Rev_Sem);

						
					}
					else
						SWIFT_UART4_Buff.write_p = 0;
				}
			}
		}
		else
			SWIFT_UART4_Buff.write_p = 0;
	}
}



/***********************************************************
**name:	SWIFT_UART4_INT_Switch
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void SWIFT_UART4_INT_Switch(INT8U switch_set)
{
	if( switch_set == ENABLE)
	{
		USART_ITConfig(SWIFT_UART4_GPIO.USARTx, USART_IT_RXNE, ENABLE);
		BSP_IntVectSet(SWIFT_UART4_INT_ID, SWITFT_UART4_IntHandler);
		BSP_IntEn(SWIFT_UART4_INT_ID);
	}
	else
	{
		BSP_IntDis(SWIFT_UART4_INT_ID);
	}
}



/***********************************************************
**name:	Motor_Init
**describe: 对电机的初始化
**input:	
**output:	none
**return:  FAIL:   SUCC
**autor:  andiman
**date:
************************************************************/
void SWIFT_UART4_Init(INT32U BaudRate)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	SWIFT_UART4_GPIO.GPIOx[UART_GPIO_TX_INDEX] = SWIFT_UART4_TX_PIN_PORT;
	SWIFT_UART4_GPIO.GPIO_InitStruct[UART_GPIO_TX_INDEX].GPIO_OType = GPIO_OType_PP;
	SWIFT_UART4_GPIO.GPIO_InitStruct[UART_GPIO_TX_INDEX].GPIO_PuPd = GPIO_PuPd_UP;
	SWIFT_UART4_GPIO.GPIO_InitStruct[UART_GPIO_TX_INDEX].GPIO_Mode = GPIO_Mode_AF;
	SWIFT_UART4_GPIO.GPIO_InitStruct[UART_GPIO_TX_INDEX].GPIO_Pin = SWIFT_UART4_TX_PIN;
	SWIFT_UART4_GPIO.GPIO_InitStruct[UART_GPIO_TX_INDEX].GPIO_Speed = GPIO_Speed_50MHz;

	SWIFT_UART4_GPIO.GPIOx[UART_GPIO_RX_INDEX] = SWIFT_UART4_RX_PIN_PORT;
	SWIFT_UART4_GPIO.GPIO_InitStruct[UART_GPIO_RX_INDEX].GPIO_OType = GPIO_OType_PP;
	SWIFT_UART4_GPIO.GPIO_InitStruct[UART_GPIO_RX_INDEX].GPIO_PuPd = GPIO_PuPd_UP;
	SWIFT_UART4_GPIO.GPIO_InitStruct[UART_GPIO_RX_INDEX].GPIO_Mode = GPIO_Mode_AF;
	SWIFT_UART4_GPIO.GPIO_InitStruct[UART_GPIO_RX_INDEX].GPIO_Pin = SWIFT_UART4_RX_PIN;
	SWIFT_UART4_GPIO.GPIO_InitStruct[UART_GPIO_RX_INDEX].GPIO_Speed = GPIO_Speed_50MHz;

	SWIFT_UART4_GPIO.USARTx = SWIFT_UART4;
	SWIFT_UART4_GPIO.USART_InitStruct.USART_BaudRate = BaudRate;
	SWIFT_UART4_GPIO.USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	SWIFT_UART4_GPIO.USART_InitStruct.USART_StopBits = USART_StopBits_1;
	SWIFT_UART4_GPIO.USART_InitStruct.USART_Parity = USART_Parity_No;
	SWIFT_UART4_GPIO.USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	SWIFT_UART4_GPIO.USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	
	BSP_UART_Init(SWIFT_UART4_GPIO);
	SWIFT_UART4_INT_Switch(ENABLE);

	SWIFT_UART4_Buff.pbuff = SWIFTUART4Buff;
	memset(SWIFT_UART4_Buff.pbuff,0,32);
	SWIFT_UART4_Buff.write_p = 0;
	SWIFT_UART4_Buff.read_p  = 0;

	BSP_OS_Sem_Creat(&SWIFT_UART4_Rev_Sem,"SWIFT_UART4_Rev_Sem",0);

	BSP_OS_Sem_Creat(&SWIFT_UART4_Send_Sem,"SWIFT_UART4_Send_Sem",1);

}


/***********************************************************
**name:	SWIFT_UART4_RecvBuff_Clear
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void SWIFT_UART4_RecvBuff_Clear(void)
{
	SWIFT_UART4_Buff.write_p = 0;
	SWIFT_UART4_Buff.read_p= 0;
	memset(SWIFT_UART4_Buff.pbuff,0,SWIFTUART4BuffSize);
}

/***********************************************************
**name:	SWIFT_UART4_SendBuff
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void SWIFT_UART4_SendBuff(INT8U *buff, INT32U bufflen)
{
	BSP_OS_Sem_Pend(&SWIFT_UART4_Send_Sem, 0);
	SWIFT_UART4_RecvBuff_Clear();
	BSP_UART_Send(SWIFT_UART4_GPIO.USARTx, buff,bufflen);
	BSP_OS_Sem_Post(&SWIFT_UART4_Send_Sem);
	
}

/***********************************************************
**name:	SWIFT_UART6_RecvBuff
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void SWIFT_UART4_RecvBuff(INT8U *buff, INT32U *bufflen)
{
	*bufflen = SWIFT_UART4_Buff.write_p;
	memcpy(buff,SWIFT_UART4_Buff.pbuff,*bufflen);
	SWIFT_UART4_Buff.write_p = 0;
}


