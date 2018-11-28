#define HW_ELECCODER_DEF

#include <hw_uart5.h>

#define		SWIFTUART5BuffSize		32

//  UART5
static str_UART_GPIO_HARD	SWIFT_UART5_GPIO;
INT8U	SWIFTUART5Buff[SWIFTUART5BuffSize];
str_UART_Buff		SWIFT_UART5_Buff;
OS_SEM SWIFT_UART5_Send_Sem;	
OS_SEM SWIFT_UART5_Rev_Sem;	

INT8U SWIFT_UART5_Predict_RevBytes=0;


/***********************************************************
**name:	SWIFT_UART5_IntHandler
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void SWIFT_UART5_IntHandler(void)
{
	if (USART_GetITStatus(SWIFT_UART5_GPIO.USARTx, USART_IT_RXNE) != RESET)
	{
		
		SWIFT_UART5_Buff.pbuff[SWIFT_UART5_Buff.write_p++] = USART_ReceiveData(SWIFT_UART5_GPIO.USARTx);
		if( SWIFT_UART5_Buff.write_p >= SWIFTUART5BuffSize )
			SWIFT_UART5_Buff.write_p = 0;
		
		if( (SWIFT_UART5_Buff.write_p >= SWIFT_UART5_Predict_RevBytes ) &&
			(SWIFT_UART5_Predict_RevBytes > 0))
			BSP_OS_Sem_Post(&SWIFT_UART5_Rev_Sem);
	}
}



/***********************************************************
**name:	SWIFT_UART5_INT_Switch
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void SWIFT_UART5_INT_Switch(INT8U switch_set)
{
	if( switch_set == ENABLE)
	{
		USART_ITConfig(SWIFT_UART5_GPIO.USARTx, USART_IT_RXNE, ENABLE);
		BSP_IntVectSet(HW_UART5_INT_ID, SWIFT_UART5_IntHandler);
		BSP_IntEn(HW_UART5_INT_ID);
	}
	else
	{
		BSP_IntDis(HW_UART5_INT_ID);
	}
}


/***********************************************************
**name:	SWIFT_UART5_Init
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void SWIFT_UART5_Init(INT32U	BaudRate)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	
	SWIFT_UART5_GPIO.GPIOx[UART_GPIO_TX_INDEX] = SWIFT_UART5_TX_PIN_PORT;
	SWIFT_UART5_GPIO.GPIO_InitStruct[UART_GPIO_TX_INDEX].GPIO_OType = GPIO_OType_PP;
	SWIFT_UART5_GPIO.GPIO_InitStruct[UART_GPIO_TX_INDEX].GPIO_PuPd = GPIO_PuPd_UP;
	SWIFT_UART5_GPIO.GPIO_InitStruct[UART_GPIO_TX_INDEX].GPIO_Mode = GPIO_Mode_AF;
	SWIFT_UART5_GPIO.GPIO_InitStruct[UART_GPIO_TX_INDEX].GPIO_Pin = SWIFT_UART5_TX_PIN;
	SWIFT_UART5_GPIO.GPIO_InitStruct[UART_GPIO_TX_INDEX].GPIO_Speed = GPIO_Speed_50MHz;

	SWIFT_UART5_GPIO.GPIOx[UART_GPIO_RX_INDEX] = SWIFT_UART5_RX_PIN_PORT;
	SWIFT_UART5_GPIO.GPIO_InitStruct[UART_GPIO_RX_INDEX].GPIO_OType = GPIO_OType_PP;
	SWIFT_UART5_GPIO.GPIO_InitStruct[UART_GPIO_RX_INDEX].GPIO_PuPd = GPIO_PuPd_UP;
	SWIFT_UART5_GPIO.GPIO_InitStruct[UART_GPIO_RX_INDEX].GPIO_Mode = GPIO_Mode_AF;
	SWIFT_UART5_GPIO.GPIO_InitStruct[UART_GPIO_RX_INDEX].GPIO_Pin = SWIFT_UART5_RX_PIN;
	SWIFT_UART5_GPIO.GPIO_InitStruct[UART_GPIO_RX_INDEX].GPIO_Speed = GPIO_Speed_50MHz;

	SWIFT_UART5_GPIO.USARTx = SWIFT_UART5;
	SWIFT_UART5_GPIO.USART_InitStruct.USART_BaudRate = BaudRate;
	SWIFT_UART5_GPIO.USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	SWIFT_UART5_GPIO.USART_InitStruct.USART_StopBits = USART_StopBits_1;
	SWIFT_UART5_GPIO.USART_InitStruct.USART_Parity = USART_Parity_No;
	SWIFT_UART5_GPIO.USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	SWIFT_UART5_GPIO.USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	
	BSP_UART_Init(SWIFT_UART5_GPIO);
	SWIFT_UART5_INT_Switch(ENABLE);

	SWIFT_UART5_Buff.pbuff = SWIFTUART5Buff;
	memset(SWIFT_UART5_Buff.pbuff,0,32);
	SWIFT_UART5_Buff.write_p = 0;
	SWIFT_UART5_Buff.read_p  = 0;

	BSP_OS_Sem_Creat(&SWIFT_UART5_Rev_Sem,"Uart5_Rcv_Sem",0);

	BSP_OS_Sem_Creat(&SWIFT_UART5_Send_Sem,"Uart5_Snd_Sem",1);
}

/***********************************************************
**name:	SWIFT_UART5_SendBuff
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void SWIFT_UART5_SendBuff(INT8U *buff, INT32U bufflen)
{
	BSP_OS_Sem_Pend(&SWIFT_UART5_Send_Sem, 0);
	SWIFT_UART5_RecvBuff_Clear();
	BSP_UART_Send(SWIFT_UART5_GPIO.USARTx, buff,bufflen);
	BSP_OS_Sem_Post(&SWIFT_UART5_Send_Sem);
	
}

/***********************************************************
**name:	SWIFT_UART5_RecvBuff
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void SWIFT_UART5_RecvBuff(INT8U *buff, INT32U *bufflen)
{
	*bufflen = SWIFT_UART5_Buff.write_p;
	memcpy(buff,SWIFT_UART5_Buff.pbuff,*bufflen);
	SWIFT_UART5_Buff.write_p = 0;
}

/***********************************************************
**name:	SWIFT_UART5_RecvBuff_Clear
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void SWIFT_UART5_RecvBuff_Clear(void)
{
	memset(SWIFT_UART5_Buff.pbuff,0,SWIFTUART5BuffSize);
	SWIFT_UART5_Buff.write_p = 0;
	SWIFT_UART5_Buff.read_p = 0;
}

