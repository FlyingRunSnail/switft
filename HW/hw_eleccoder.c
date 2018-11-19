


#define HW_ELECCODER_DEF

#include <hw_eleccoder.h>


//  UART5
static str_UART_GPIO_HARD	SWIFT_UART5_GPIO;
#define		SWIFTUART5BuffSize		32
INT8U	SWIFTUART5Buff[SWIFTUART5BuffSize];
str_UART_Buff		SWIFT_UART5_Buff;
OS_SEM SWIFT_UART5_Send_Sem;	
OS_SEM SWIFT_UART5_Rev_Sem;	

INT8U SWIFT_UART5_Predict_RevBytes=0;

// UART2
static str_UART_GPIO_HARD	SWIFT_UART2_GPIO;
#define		SWIFTUART2BuffSize		32
INT8U	SWIFTUART2Buff[SWIFTUART2BuffSize];
str_UART_Buff		SWIFT_UART2_Buff;
OS_SEM SWIFT_UART2_Send_Sem;	
OS_SEM SWIFT_UART2_Rev_Sem;	

INT8U SWIFT_UART2_Predict_RevBytes=0;

// UART1
static str_UART_GPIO_HARD	SWIFT_UART1_GPIO;
#define		SWIFTUART1BuffSize		32
INT8U	SWIFTUART1Buff[SWIFTUART1BuffSize];
str_UART_Buff		SWIFT_UART1_Buff;
OS_SEM SWIFT_UART1_Send_Sem;	
OS_SEM SWIFT_UART1_Rev_Sem;	

INT8U SWIFT_UART1_Predict_RevBytes=0;

#if 0
//接收缓冲,最大USART_REC_LEN个字节
u8 USART_RX_BUF[SWIFTUART1BuffSize];

// 接收状态标记
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA = 0;

#pragma import(__use_no_semihosting)

//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
}

//重定义fputc函数 
int fputc(int ch, FILE *f)
{
	//循环发送,直到发送完毕   
	while((USART1->SR & 0X40) == 0)
	{
		;
	}

	USART1->DR = (u8) ch;

	return ch;
}
#endif

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
		BSP_IntVectSet(HW_PositionElecCoder_UART_INT_ID, SWIFT_UART5_IntHandler);
		BSP_IntEn(HW_PositionElecCoder_UART_INT_ID);
	}
	else
	{
		BSP_IntDis(HW_PositionElecCoder_UART_INT_ID);
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

	BSP_OS_Sem_Creat(&SWIFT_UART5_Rev_Sem,"PositionElecCoder rev Sem",0);

	BSP_OS_Sem_Creat(&SWIFT_UART5_Send_Sem,"PositionElecCoder send Sem",1);
}


/***********************************************************
**name:	SWIFT_UART2_IntHandler
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void SWIFT_UART2_IntHandler(void)
{
	if (USART_GetITStatus(SWIFT_UART2_GPIO.USARTx, USART_IT_RXNE) != RESET)
	{
		SWIFT_UART2_Buff.pbuff[SWIFT_UART2_Buff.write_p++] = USART_ReceiveData(SWIFT_UART2_GPIO.USARTx);
		if( SWIFT_UART2_Buff.write_p >= SWIFTUART2BuffSize )
			SWIFT_UART2_Buff.write_p = 0;
		if( (SWIFT_UART2_Buff.write_p >= SWIFT_UART2_Predict_RevBytes ) &&
			(	SWIFT_UART2_Predict_RevBytes > 0))
			BSP_OS_Sem_Post(&SWIFT_UART2_Rev_Sem);
	}
}



/***********************************************************
**name:	SWIFT_UART2_INT_Switch
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void SWIFT_UART2_INT_Switch(INT8U switch_set)
{
	if( switch_set == ENABLE)
	{
		USART_ITConfig(SWIFT_UART2_GPIO.USARTx, USART_IT_RXNE, ENABLE);
		BSP_IntVectSet(HW_CabrageElecCoder_UART_INT_ID, SWIFT_UART2_IntHandler);
		BSP_IntEn(HW_CabrageElecCoder_UART_INT_ID);
	}
	else
	{
		BSP_IntDis(HW_CabrageElecCoder_UART_INT_ID);
	}
}
/***********************************************************
**name:	SWIFT_UART2_Init
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void SWIFT_UART2_Init(INT32U	BaudRate)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	
	SWIFT_UART2_GPIO.GPIOx[UART_GPIO_TX_INDEX] = SWIFT_UART2_TX_PIN_PORT;
	SWIFT_UART2_GPIO.GPIO_InitStruct[UART_GPIO_TX_INDEX].GPIO_OType = GPIO_OType_PP;
	SWIFT_UART2_GPIO.GPIO_InitStruct[UART_GPIO_TX_INDEX].GPIO_PuPd = GPIO_PuPd_UP;
	SWIFT_UART2_GPIO.GPIO_InitStruct[UART_GPIO_TX_INDEX].GPIO_Mode = GPIO_Mode_AF;
	SWIFT_UART2_GPIO.GPIO_InitStruct[UART_GPIO_TX_INDEX].GPIO_Pin = SWIFT_UART2_TX_PIN;
	SWIFT_UART2_GPIO.GPIO_InitStruct[UART_GPIO_TX_INDEX].GPIO_Speed = GPIO_Speed_50MHz;

	SWIFT_UART2_GPIO.GPIOx[UART_GPIO_RX_INDEX] = SWIFT_UART2_RX_PIN_PORT;
	SWIFT_UART2_GPIO.GPIO_InitStruct[UART_GPIO_RX_INDEX].GPIO_OType = GPIO_OType_PP;
	SWIFT_UART2_GPIO.GPIO_InitStruct[UART_GPIO_RX_INDEX].GPIO_PuPd = GPIO_PuPd_UP;
	SWIFT_UART2_GPIO.GPIO_InitStruct[UART_GPIO_RX_INDEX].GPIO_Mode = GPIO_Mode_AF;
	SWIFT_UART2_GPIO.GPIO_InitStruct[UART_GPIO_RX_INDEX].GPIO_Pin = SWIFT_UART2_RX_PIN;
	SWIFT_UART2_GPIO.GPIO_InitStruct[UART_GPIO_RX_INDEX].GPIO_Speed = GPIO_Speed_50MHz;

	SWIFT_UART2_GPIO.USARTx = SWIFT_UART2;
	SWIFT_UART2_GPIO.USART_InitStruct.USART_BaudRate = BaudRate;
	SWIFT_UART2_GPIO.USART_InitStruct.USART_WordLength = USART_WordLength_8b;
	SWIFT_UART2_GPIO.USART_InitStruct.USART_StopBits = USART_StopBits_1;
	SWIFT_UART2_GPIO.USART_InitStruct.USART_Parity = USART_Parity_No;
	SWIFT_UART2_GPIO.USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	SWIFT_UART2_GPIO.USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	
	BSP_UART_Init(SWIFT_UART2_GPIO);
	SWIFT_UART2_INT_Switch(ENABLE);

	SWIFT_UART2_Buff.pbuff = SWIFTUART2Buff;
	memset(SWIFT_UART5_Buff.pbuff,0,32);
	SWIFT_UART2_Buff.write_p = 0;
	SWIFT_UART2_Buff.read_p  = 0;

	BSP_OS_Sem_Creat(&SWIFT_UART2_Rev_Sem,"CabrageElecCoder rev Sem",0);

	BSP_OS_Sem_Creat(&SWIFT_UART2_Send_Sem,"CabrageElecCoder send Sem",1);

}

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
		if( SWIFT_UART1_Buff.write_p >= SWIFTUART2BuffSize )
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
		BSP_IntVectSet(HW_PolarElecCoder_UART_INT_ID, SWIFT_UART1_IntHandler);
		BSP_IntEn(HW_PolarElecCoder_UART_INT_ID);
	}
	else
	{
		BSP_IntDis(HW_PolarElecCoder_UART_INT_ID);
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

	SWIFT_UART1_GPIO.USARTx = HW_PolarElecCoder_UARTX;
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

/***********************************************************
**name:	SWIFT_UART2_SendBuff
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void SWIFT_UART2_SendBuff(INT8U *buff, INT32U bufflen)
{
	BSP_OS_Sem_Pend(&SWIFT_UART2_Send_Sem, 0);
	SWIFT_UART2_RecvBuff_Clear();
	BSP_UART_Send(SWIFT_UART2_GPIO.USARTx, buff,bufflen);
	BSP_OS_Sem_Post(&SWIFT_UART2_Send_Sem);
	
}

/***********************************************************
**name:	SWIFT_UART2_RecvBuff
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void SWIFT_UART2_RecvBuff(INT8U *buff, INT32U *bufflen)
{
	*bufflen = SWIFT_UART2_Buff.write_p;
	memcpy(buff,SWIFT_UART2_Buff.pbuff,*bufflen);
	SWIFT_UART2_Buff.write_p = 0;
}

/***********************************************************
**name:	SWIFT_UART2_RecvBuff_Clear
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void SWIFT_UART2_RecvBuff_Clear(void)
{
	memset(SWIFT_UART2_Buff.pbuff,0,SWIFTUART2BuffSize);
	SWIFT_UART2_Buff.write_p = 0;
	SWIFT_UART2_Buff.read_p = 0;
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

