
#include <devdef.h>
#include <bsp_uart.h>

#define	HW_UART3_DEF

#include "hw_uart3.h"


static str_UART_GPIO_HARD	SWIFT_UART3_GPIO;

#define		SWIFTUART3BuffSize		34
INT8U	SWIFTUART3Buff[SWIFTUART3BuffSize];
str_UART_Buff		SWIFT_UART3_Buff;

OS_SEM SWIFT_UART3_Send_Sem;

OS_SEM SWIFT_UART3_Rev_Sem;


/***********************************************************
**name:	SWIFT_UART3_IntHandler
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void SWIFT_UART3_IntHandler(void)
{
 	if (USART_GetITStatus(SWIFT_UART3_GPIO.USARTx, USART_IT_RXNE) != RESET)
	{
		SWIFT_UART3_Buff.pbuff[SWIFT_UART3_Buff.write_p++] = USART_ReceiveData(SWIFT_UART3_GPIO.USARTx);
		if( SWIFT_UART3_Buff.write_p >= SWIFTUART3BuffSize )
			SWIFT_UART3_Buff.write_p = 0;
		if( SWIFT_UART3_Buff.pbuff[0] == 0xDD ) 
		{
			if( SWIFT_UART3_Buff.write_p >= 0x03 )
			{
				if( SWIFT_UART3_Buff.write_p >= SWIFT_UART3_Buff.pbuff[3] + 0x06 )
				{
					if( SWIFT_UART3_Buff.pbuff[SWIFT_UART3_Buff.pbuff[3] + 0x06] == 0x77 )
					{
						BSP_OS_Sem_Post(&SWIFT_UART3_Rev_Sem);
					}
					else
						SWIFT_UART3_Buff.write_p = 0;
				}
			}
		}
		else
			SWIFT_UART3_Buff.write_p = 0;
	}
}

/***********************************************************
**name:	SWIFT_UART3_INT_Switch
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void SWIFT_UART3_INT_Switch(INT8U switch_set)
{
	if( switch_set == ENABLE)
	{
		USART_ITConfig(SWIFT_UART3_GPIO.USARTx, USART_IT_RXNE, ENABLE);
		BSP_IntVectSet(SWIFT_UART3_INT_ID, SWIFT_UART3_IntHandler);
		BSP_IntEn(SWIFT_UART3_INT_ID);
	}
	else
	{
		BSP_IntDis(SWIFT_UART3_INT_ID);
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
void SWIFT_UART3_Init(INT32U BaudRate)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	
	SWIFT_UART3_GPIO.GPIOx[UART_GPIO_TX_INDEX] = SWIFT_UART3_TX_PIN_PORT;
	SWIFT_UART3_GPIO.GPIO_InitStruct[UART_GPIO_TX_INDEX].GPIO_OType = GPIO_OType_PP;
	SWIFT_UART3_GPIO.GPIO_InitStruct[UART_GPIO_TX_INDEX].GPIO_PuPd = GPIO_PuPd_UP;
	SWIFT_UART3_GPIO.GPIO_InitStruct[UART_GPIO_TX_INDEX].GPIO_Mode = GPIO_Mode_AF;
	SWIFT_UART3_GPIO.GPIO_InitStruct[UART_GPIO_TX_INDEX].GPIO_Pin = SWIFT_UART3_TX_PIN;
	SWIFT_UART3_GPIO.GPIO_InitStruct[UART_GPIO_TX_INDEX].GPIO_Speed = GPIO_Speed_50MHz;

	SWIFT_UART3_GPIO.GPIOx[UART_GPIO_RX_INDEX] = SWIFT_UART3_RX_PIN_PORT;
	SWIFT_UART3_GPIO.GPIO_InitStruct[UART_GPIO_RX_INDEX].GPIO_OType = GPIO_OType_PP;
	SWIFT_UART3_GPIO.GPIO_InitStruct[UART_GPIO_RX_INDEX].GPIO_PuPd = GPIO_PuPd_UP;
	SWIFT_UART3_GPIO.GPIO_InitStruct[UART_GPIO_RX_INDEX].GPIO_Mode = GPIO_Mode_AF;
	SWIFT_UART3_GPIO.GPIO_InitStruct[UART_GPIO_RX_INDEX].GPIO_Pin = SWIFT_UART3_RX_PIN;
	SWIFT_UART3_GPIO.GPIO_InitStruct[UART_GPIO_RX_INDEX].GPIO_Speed = GPIO_Speed_50MHz;

	SWIFT_UART3_GPIO.USARTx = SWIFT_UART3;
	SWIFT_UART3_GPIO.USART_InitStruct.USART_BaudRate = BaudRate;
	SWIFT_UART3_GPIO.USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	SWIFT_UART3_GPIO.USART_InitStruct.USART_StopBits = USART_StopBits_1;
	SWIFT_UART3_GPIO.USART_InitStruct.USART_Parity = USART_Parity_No;
	SWIFT_UART3_GPIO.USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	SWIFT_UART3_GPIO.USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	
	BSP_UART_Init(SWIFT_UART3_GPIO);
	SWIFT_UART3_INT_Switch(ENABLE);

	SWIFT_UART3_Buff.pbuff = SWIFTUART3Buff;
	memset(SWIFT_UART3_Buff.pbuff,0,32);
	SWIFT_UART3_Buff.write_p = 0;
	SWIFT_UART3_Buff.read_p  = 0;

	BSP_OS_Sem_Creat(&SWIFT_UART3_Rev_Sem,"SWIFT_UART3_Rev_Sem",0);

	BSP_OS_Sem_Creat(&SWIFT_UART3_Send_Sem,"SWIFT_UART3_Send_Sem",1);

	
}

/***********************************************************
**name:	SWIFT_UART3_RecvBuff_Clear
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void SWIFT_UART3_RecvBuff_Clear(void)
{
	SWIFT_UART3_Buff.write_p = 0;
	SWIFT_UART3_Buff.read_p= 0;
	memset(SWIFT_UART3_Buff.pbuff,0,SWIFTUART3BuffSize);
}

/***********************************************************
**name:	SWIFT_UART3_SendBuff
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void SWIFT_UART3_SendBuff(INT8U *buff, INT32U bufflen)
{
	BSP_OS_Sem_Pend(&SWIFT_UART3_Send_Sem, 0);
	SWIFT_UART3_RecvBuff_Clear();
	BSP_UART_Send(SWIFT_UART3_GPIO.USARTx, buff,bufflen);
	BSP_OS_Sem_Post(&SWIFT_UART3_Send_Sem);
	
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
void SWIFT_UART3_RecvBuff(INT8U *buff, INT32U *bufflen)
{
    BSP_OS_Sem_Pend(&SWIFT_UART3_Rev_Sem, 0);
	*bufflen = SWIFT_UART3_Buff.write_p;
	memcpy(buff,SWIFT_UART3_Buff.pbuff,*bufflen);
	SWIFT_UART3_Buff.write_p = 0;
	BSP_OS_Sem_Post(&SWIFT_UART3_Rev_Sem);
}


