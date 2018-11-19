


#define HW_BEACON_DEF

#include <hw_beacon.h>



static str_UART_GPIO_HARD	SWIFT_UART6_GPIO;

#define		SWIFTUART6BuffSize		128
INT8U	SWIFTUART6Buff[SWIFTUART6BuffSize];
str_UART_Buff		SWIFT_UART6_Buff;

OS_SEM SWIFT_UART6_Send_Sem;
OS_SEM SWIFT_UART6_Rev_Sem;

//接收缓冲,最大USART_REC_LEN个字节
u8 USART_RX_BUF[SWIFTUART6BuffSize];

// 接收状态标记
//bit15，	接收完成标志
//bit14，	接收到0x0d
//bit13~0，	接收到的有效字节数目
u16 USART_RX_STA = 0;

#if 0
static char *pUSARTBufStart = SWIFTUART6Buff;
static char *pUSARTBufIdx = SWIFTUART6Buff;
#endif

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
	while((USART6->SR & 0X40) == 0)
	{
		;
	}

	USART6->DR = (u8) ch;

	return ch;
}

int fgetc(FILE *f)
{
	int ch;
	while (USART_GetFlagStatus(USART6, USART_FLAG_RXNE) == RESET) 
	{
		;
	}

	ch = USART_ReceiveData(USART6);

	return ch;
}

#if 0
int cli_readchar(char c)
{
	 char *p = USART_RX_BUF;
	 char *p_buf = p;
	 int n = 0; 			 /* buffer index	 */
	 int plen = 0;			 /* prompt length	 */
	 int col;				 /* output column cnt	 */
	 char	 c;

	plen = strlen(SHELL_SYS_PROMPT);
	col = plen;

	 /*
	  * Special character handling
	  */
	 switch (c) {
	 case '\r': 		 /* Enter		 */
	 case '\n':
		 *p = '\0';
		 fputs("\r\n");
		 return p - p_buf;
	 
	 case '\0': 		 /* nul 		 */
		 continue;
	 
	 case 0x03: 		 /* ^C - break		 */
		 p_buf[0] = '\0';	 /* discard input */
		 return -1;
	 
	 case 0x15: 		 /* ^U - erase line  */
		 while (col > plen) {
			 fputs(erase_seq);
			 --col;
		 }
		 p = p_buf;
		 n = 0;
		 continue;
	 
	 case 0x17: 		 /* ^W - erase word  */
		 p = delete_char(p_buf, p, &col, &n, plen);
		 while ((n > 0) && (*p != ' '))
			 p = delete_char(p_buf, p, &col, &n, plen);
		 continue;
	 
	 case 0x08: 		 /* ^H	- backspace  */
	 case 0x7F: 		 /* DEL - backspace  */
		 p = delete_char(p_buf, p, &col, &n, plen);
		 continue;
	 
	 default:
		 /*
		  * Must be a normal character then
		  */
		 if (n < SHELL_SYS_CBSIZE-2) {
			 if (c == '\t') {	 /* expand TABs */
				 /*
				  * if auto completion triggered just
				  * continue
				  */
				 *p = '\0';
				 if (cmd_auto_complete(prompt,
							   console_buffer,
							   &n, &col)) {
					 p = p_buf + n;  /* reset */
					 continue;
				 }
				 fputs(tab_seq + (col & 07));
				 col += 8 - (col & 07);
			 } else {
				 
				 char buf[2];
				 
				 /*
				  * Echo input using puts() to force an
				  * LCD flush if we are using an LCD
				  */
				 ++col;
				 buf[0] = c;
				 buf[1] = '\0';
				 fputs(buf);
			 }
			 *p++ = c;
			 ++n;
		 } 
		 else 
		 {			 /* Buffer full */
			 fputc('\a');
		 }
	 }

}
#endif

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
	INT8U test_buff[10]={0};

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

	BSP_OS_Sem_Creat(&Beacon2MCU_Sem,"Beacon2MCU Sem",0);

	BSP_OS_Sem_Creat(&SWIFT_UART6_Send_Sem,"SWIFT_UART6_Send_Sem",1);

    BSP_OS_Sem_Creat(&SWIFT_UART6_Rev_Sem, "SWIFT_SHELL_SEM", 0);
	
}

#if 0
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
						
				BSP_OS_Sem_Post(&Beacon2MCU_Sem);
			}
		}
		else
			SWIFT_UART6_Buff.write_p = 0;
	}
}
#endif

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
		//Res = USART_ReceiveData(SWIFT_UART6_GPIO.USARTx);
		BSP_OS_Sem_Post(&SWIFT_UART6_Rev_Sem);

#if 0
		//读取接收到的数据
		Res = USART_ReceiveData(SWIFT_UART6_GPIO.USARTx);
#endif

#if 0
		switch(Res)
		{
		case '\r':			/* Enter		*/
		case '\n':
			*pUSARTBufIdx = '\0';
			fputs("\r\n");
			break;
		case '\0': 		 /* nul 		 */
			break;
		 case 0x03: 		 /* ^C - break		 */
			pUSARTBufStart[0] = '\0';	 /* discard input */
			break;
		 case 0x15: 		 /* ^U - erase line  */
			 while (col > plen) {
				 fputs(erase_seq);
				 --col;
			 }
			pUSARTBufIdx = pUSARTBufStart;
			n = 0; 
			break;
		case 0x08: 		 /* ^H	- backspace  */
		case 0x7F: 		 /* DEL - backspace  */
			pUSARTBufIdx = delete_char(pUSARTBufStart, pUSARTBufIdx, &col, &n, plen);
			break;
		default:
			/*
			  * Must be a normal character then
			  */
			 if (n < SHELL_SYS_CBSIZE-2) {
				 if (c == '\t') {	 /* expand TABs */
					 /*
					  * if auto completion triggered just
					  * continue
					  */
					 *pUSARTBufIdx = '\0';
					 if (cmd_auto_complete(prompt,
								   console_buffer,
								   &n, &col)) {
						 pUSARTBufIdx = pUSARTBufStart + n;  /* reset */
						 continue;
					 }
					 fputs(tab_seq + (col & 07));
					 col += 8 - (col & 07);
				 } else {
					 
					 char buf[2];
					 
					 /*
					  * Echo input using puts() to force an
					  * LCD flush if we are using an LCD
					  */
					 ++col;
					 buf[0] = c;
					 buf[1] = '\0';
					 fputs(buf);
				 }
				 *pUSARTBufIdx++ = c;
				 ++n;
			} 
			else 
			{			 /* Buffer full */
				 fputc('\a');
			}
		}
#endif
#if 0
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
				memcpy(console_buffer, USART_RX_BUF, SWIFTUART6BuffSize);

				//唤醒shell任务
				BSP_OS_Sem_Post(&SWIFT_UART6_Rev_Sem);
			}

			USART_RX_STA++;

			//接收数据错误,重新开始接收
			if ((USART_RX_STA & 0x3FFF) > (SWIFTUART6BuffSize - 1))
			{
				USART_RX_STA = 0;
			}
		}
#endif
	}
}


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

INT32S SWIFT_USART6_GETC(void)
{
	BSP_OS_Sem_Pend(&SWIFT_UART6_Rev_Sem, 0);
	return BSP_UART_RCV(SWIFT_UART6_GPIO.USARTx);
}

INT32S SWIFT_USART6_TSTC(void)
{
	return BSP_UART_TST(SWIFT_UART6_GPIO.USARTx);
}

