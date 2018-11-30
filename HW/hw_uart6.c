
#define HW_UART6_DEF

#include "hw_uart6.h"



static str_UART_GPIO_HARD	SWIFT_UART6_GPIO;

#define		SWIFTUART6BuffSize		128
INT8U	SWIFTUART6Buff[SWIFTUART6BuffSize];
str_UART_Buff		SWIFT_UART6_Buff;

OS_SEM SWIFT_UART6_Send_Sem;
OS_SEM SWIFT_UART6_Rev_Sem;

/***********************************************************
**name:	SWIFT_UART6_Init
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void SWIFT_UART6_Init(INT32U	BaudRate)
{
	SWIFT_UART6_GPIO.GPIOx[UART_GPIO_TX_INDEX] = HW_BEACON_UART_TX_PIN_PORT;
	SWIFT_UART6_GPIO.GPIO_InitStruct[UART_GPIO_TX_INDEX].GPIO_OType = GPIO_OType_PP;
	SWIFT_UART6_GPIO.GPIO_InitStruct[UART_GPIO_TX_INDEX].GPIO_PuPd = GPIO_PuPd_UP;
	SWIFT_UART6_GPIO.GPIO_InitStruct[UART_GPIO_TX_INDEX].GPIO_Mode = GPIO_Mode_AF;
	SWIFT_UART6_GPIO.GPIO_InitStruct[UART_GPIO_TX_INDEX].GPIO_Pin = HW_BEACON_UART_TX_PIN;
	SWIFT_UART6_GPIO.GPIO_InitStruct[UART_GPIO_TX_INDEX].GPIO_Speed = GPIO_Speed_50MHz;

	SWIFT_UART6_GPIO.GPIOx[UART_GPIO_RX_INDEX] = HW_BEACON_UART_RX_PIN_PORT;
	SWIFT_UART6_GPIO.GPIO_InitStruct[UART_GPIO_RX_INDEX].GPIO_OType = GPIO_OType_PP;
	SWIFT_UART6_GPIO.GPIO_InitStruct[UART_GPIO_RX_INDEX].GPIO_PuPd = GPIO_PuPd_UP;
	SWIFT_UART6_GPIO.GPIO_InitStruct[UART_GPIO_RX_INDEX].GPIO_Mode = GPIO_Mode_AF;
	SWIFT_UART6_GPIO.GPIO_InitStruct[UART_GPIO_RX_INDEX].GPIO_Pin = HW_BEACON_UART_RX_PIN;
	SWIFT_UART6_GPIO.GPIO_InitStruct[UART_GPIO_RX_INDEX].GPIO_Speed = GPIO_Speed_50MHz;

	SWIFT_UART6_GPIO.USARTx = HW_BEACON_UARTX;
	SWIFT_UART6_GPIO.USART_InitStruct.USART_BaudRate = BaudRate;
	SWIFT_UART6_GPIO.USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	SWIFT_UART6_GPIO.USART_InitStruct.USART_StopBits = USART_StopBits_1;
	SWIFT_UART6_GPIO.USART_InitStruct.USART_Parity = USART_Parity_No;
	SWIFT_UART6_GPIO.USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	SWIFT_UART6_GPIO.USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	
	BSP_UART_Init(SWIFT_UART6_GPIO);
	SWIFT_UART6_INT_Switch(ENABLE);

	SWIFT_UART6_Buff.pbuff = SWIFTUART6Buff;
	memset(SWIFT_UART6_Buff.pbuff,0,32);
	SWIFT_UART6_Buff.write_p = 0;
	SWIFT_UART6_Buff.read_p  = 0;

	BSP_OS_Sem_Creat(&SWIFT_UART6_Send_Sem,"SWIFT_UART6_Send_Sem",1);

    BSP_OS_Sem_Creat(&SWIFT_UART6_Rev_Sem, "SWIFT_SHELL_SEM", 0);
	
}

#if 1
/***********************************************************
**name:	SWIFT_UART6_IntHandler
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void SWIFT_UART6_IntHandler(void)
{
	if (USART_GetITStatus(SWIFT_UART6_GPIO.USARTx, USART_IT_RXNE) != RESET)
	{
		SWIFT_UART6_Buff.pbuff[SWIFT_UART6_Buff.write_p++] = USART_ReceiveData(SWIFT_UART6_GPIO.USARTx);
		if( SWIFT_UART6_Buff.write_p >= SWIFTUART6BuffSize )
			SWIFT_UART6_Buff.write_p = 0;
		if( SWIFT_UART6_Buff.pbuff[0] == '>')
		{
			if( SWIFT_UART6_Buff.pbuff[SWIFT_UART6_Buff.write_p-1] == 0x0A)
			{
				if( SWIFT_UART6_Buff.pbuff[SWIFT_UART6_Buff.write_p-2] == 0x0D)
						
				//BSP_OS_Sem_Post(&Beacon2MCU_Sem);
			}
		}
		else
			SWIFT_UART6_Buff.write_p = 0;
	}
}
#else

/***********************************************************
**name:	SWIFT_UART6_IntHandler
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void SWIFT_UART6_IntHandler(void)
{
	INT8U Res;

	if (USART_GetITStatus(SWIFT_UART6_GPIO.USARTx, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(SWIFT_UART6_GPIO.USARTx, USART_IT_RXNE);
		BSP_OS_Sem_Post(&SWIFT_UART6_Rev_Sem);
	}
}
#endif

/***********************************************************
**name:	SWIFT_UART6_INT_Switch
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void SWIFT_UART6_INT_Switch(INT8U switch_set)
{
	if( switch_set == ENABLE)
	{
		USART_ITConfig(SWIFT_UART6_GPIO.USARTx, USART_IT_RXNE, ENABLE);
		BSP_IntVectSet(SWIFT_UART6_INT_ID, SWIFT_UART6_IntHandler);
		BSP_IntEn(SWIFT_UART6_INT_ID);
	}
	else
	{
		BSP_IntDis(SWIFT_UART6_INT_ID);
	}
}


/***********************************************************
**name:	SWIFT_UART6_SendBuff
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void SWIFT_UART6_SendBuff(INT8S *buff, INT32U bufflen)
{
	BSP_OS_Sem_Pend(&SWIFT_UART6_Send_Sem, 0);
	BSP_UART_Send(SWIFT_UART6_GPIO.USARTx, buff,bufflen);
	BSP_OS_Sem_Post(&SWIFT_UART6_Send_Sem);
	
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
void SWIFT_UART6_RecvBuff(INT8S *buff, INT32U *bufflen)
{
	*bufflen = SWIFT_UART6_Buff.write_p;
	memcpy(buff,SWIFT_UART6_Buff.pbuff,*bufflen);
	SWIFT_UART6_Buff.write_p = 0;
}

