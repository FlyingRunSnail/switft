
#include "hw_uart1.h"

// UART1
#define		SWIFTUART1BuffSize		32

static str_UART_GPIO_HARD	SWIFT_UART1_GPIO;
INT8U	SWIFTUART1Buff[SWIFTUART1BuffSize];
str_UART_Buff		SWIFT_UART1_Buff;
OS_SEM SWIFT_UART1_Send_Sem;	
OS_SEM SWIFT_UART1_Rev_Sem;	

INT8U SWIFT_UART1_Predict_RevBytes=0;

#if 1
/***********************************************************
**name:	SWIFT_UART1_IntHandler
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void SWIFT_UART1_IntHandler(void)
{
	if (USART_GetITStatus(SWIFT_UART1_GPIO.USARTx, USART_IT_RXNE) != RESET)
	{
		SWIFT_UART1_Buff.pbuff[SWIFT_UART1_Buff.write_p++] = USART_ReceiveData(SWIFT_UART1_GPIO.USARTx);
		if( SWIFT_UART1_Buff.write_p >= SWIFTUART1BuffSize )
			SWIFT_UART1_Buff.write_p = 0;
		if( (SWIFT_UART1_Buff.write_p >= SWIFT_UART1_Predict_RevBytes ) &&(SWIFT_UART1_Predict_RevBytes>0))
			BSP_OS_Sem_Post(&SWIFT_UART1_Rev_Sem);
	}
}
#else

/***********************************************************
**name:	SWIFT_UART1_IntHandler
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void SWIFT_UART1_IntHandler(void)
{
	INT8U Res;

	if (USART_GetITStatus(SWIFT_UART1_GPIO.USARTx, USART_IT_RXNE) != RESET)
	{
		//读取接收到的数据
		Res = USART_ReceiveData(SWIFT_UART1_GPIO.USARTx);

		//退格键
		if (Res == 0x8)
		{
			if ((USART_RX_STA & 0x3FFF) > 0)
			{
				//先发退格键，再发空格，最后再发退格
				fputc(Res, NULL);
				fputc(0x20, NULL);
				fputc(Res, NULL);
				USART_RX_STA--;
			}
		}
		else
		{
			fputc(Res, NULL);
			USART_RX_BUF[USART_RX_STA & 0x3FFF] = Res ;

			//回车键
			if (Res == '\r')
			{
				if ((USART_RX_STA & 0x3FFF) > 1)
				{
					USART_RX_BUF[USART_RX_STA & 0x3FFF] = '\0' ;
				}

				USART_RX_STA |= 0x8000;

				//唤醒shell任务
				BSP_OS_Sem_Post(&SWIFT_UART1_Rev_Sem);
			}

			USART_RX_STA++;

			//接收数据错误,重新开始接收
			if ((USART_RX_STA & 0x3FFF) > (SWIFTUART1BuffSize - 1))
			{
				USART_RX_STA = 0;
			}
		}
	}
}
#endif

/***********************************************************
**name:	SWIFT_UART1_INT_Switch
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void SWIFT_UART1_INT_Switch(INT8U switch_set)
{
	if( switch_set == ENABLE)
	{
		USART_ITConfig(SWIFT_UART1_GPIO.USARTx, USART_IT_RXNE, ENABLE);
		BSP_IntVectSet(HW_UART1_INT_ID, SWIFT_UART1_IntHandler);
		BSP_IntEn(HW_UART1_INT_ID);
	}
	else
	{
		BSP_IntDis(HW_UART1_INT_ID);
	}
}

/***********************************************************
**name:	SWIFT_UART1_Init
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void SWIFT_UART1_Init(INT32U	BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	SWIFT_UART1_GPIO.GPIOx[UART_GPIO_TX_INDEX] = SWIFT_UART1_TX_PIN_PORT;
	SWIFT_UART1_GPIO.GPIO_InitStruct[UART_GPIO_TX_INDEX].GPIO_OType = GPIO_OType_PP;
	SWIFT_UART1_GPIO.GPIO_InitStruct[UART_GPIO_TX_INDEX].GPIO_PuPd = GPIO_PuPd_UP;
	SWIFT_UART1_GPIO.GPIO_InitStruct[UART_GPIO_TX_INDEX].GPIO_Mode = GPIO_Mode_AF;
	SWIFT_UART1_GPIO.GPIO_InitStruct[UART_GPIO_TX_INDEX].GPIO_Pin = SWIFT_UART1_TX_PIN;
	SWIFT_UART1_GPIO.GPIO_InitStruct[UART_GPIO_TX_INDEX].GPIO_Speed = GPIO_Speed_50MHz;

	SWIFT_UART1_GPIO.GPIOx[UART_GPIO_RX_INDEX] = SWIFT_UART1_RX_PIN_PORT;
	SWIFT_UART1_GPIO.GPIO_InitStruct[UART_GPIO_RX_INDEX].GPIO_OType = GPIO_OType_PP;
	SWIFT_UART1_GPIO.GPIO_InitStruct[UART_GPIO_RX_INDEX].GPIO_PuPd = GPIO_PuPd_UP;
	SWIFT_UART1_GPIO.GPIO_InitStruct[UART_GPIO_RX_INDEX].GPIO_Mode = GPIO_Mode_AF;
	SWIFT_UART1_GPIO.GPIO_InitStruct[UART_GPIO_RX_INDEX].GPIO_Pin = SWIFT_UART1_RX_PIN;
	SWIFT_UART1_GPIO.GPIO_InitStruct[UART_GPIO_RX_INDEX].GPIO_Speed = GPIO_Speed_50MHz;

	SWIFT_UART1_GPIO.USARTx = HW_UART1;
	SWIFT_UART1_GPIO.USART_InitStruct.USART_BaudRate = BaudRate;
	SWIFT_UART1_GPIO.USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	SWIFT_UART1_GPIO.USART_InitStruct.USART_StopBits = USART_StopBits_1;
	SWIFT_UART1_GPIO.USART_InitStruct.USART_Parity = USART_Parity_No;
	SWIFT_UART1_GPIO.USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	SWIFT_UART1_GPIO.USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	
	BSP_UART_Init(SWIFT_UART1_GPIO);
	SWIFT_UART1_INT_Switch(ENABLE);

	SWIFT_UART1_Buff.pbuff = SWIFTUART1Buff;
	memset(SWIFT_UART1_Buff.pbuff,0,32);
	SWIFT_UART1_Buff.write_p = 0;
	SWIFT_UART1_Buff.read_p  = 0;

	BSP_OS_Sem_Creat(&SWIFT_UART1_Rev_Sem,"PolarElecCoder rev Sem",0);

	BSP_OS_Sem_Creat(&SWIFT_UART1_Send_Sem,"PolarElecCoder send Sem",1);
}

/***********************************************************
**name:	SWIFT_UART1_SendBuff
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void SWIFT_UART1_SendBuff(INT8U *buff, INT32U bufflen)
{
	BSP_OS_Sem_Pend(&SWIFT_UART1_Send_Sem, 0);
	SWIFT_UART1_RecvBuff_Clear();
	BSP_UART_Send(SWIFT_UART1_GPIO.USARTx, buff,bufflen);
	BSP_OS_Sem_Post(&SWIFT_UART1_Send_Sem);
	
}

/***********************************************************
**name:	SWIFT_UART1_RecvBuff
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void SWIFT_UART1_RecvBuff(INT8U *buff, INT32U *bufflen)
{
	*bufflen = SWIFT_UART1_Buff.write_p;
	memcpy(buff,SWIFT_UART1_Buff.pbuff,*bufflen);
	SWIFT_UART1_Buff.write_p = 0;
}

/***********************************************************
**name:	SWIFT_UART1_RecvBuff_Clear
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void SWIFT_UART1_RecvBuff_Clear(void)
{
	memset(SWIFT_UART1_Buff.pbuff,0,SWIFTUART1BuffSize);
	SWIFT_UART1_Buff.write_p = 0;
	SWIFT_UART1_Buff.read_p = 0;
}

