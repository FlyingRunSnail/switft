
#define HW_UART2_DEF

#include <hw_uart2.h>
#include <hw_uart_dma.h>
#include "kfifo.h"

extern INT8U uart2_dma_buf[USART2_DMA_BUF_LEN];
struct kfifo *uart2_rcv_fifo;

#define    SWIFTUART2BuffSize (32)

// UART2
static str_UART_GPIO_HARD	SWIFT_UART2_GPIO;
INT8U	SWIFTUART2Buff[SWIFTUART2BuffSize];
str_UART_Buff		SWIFT_UART2_Buff;
OS_SEM SWIFT_UART2_Send_Sem;	
OS_SEM SWIFT_UART2_Rev_Sem;	

#define UART2_BUF_LEN (256)
INT8U uart2_buf[UART2_BUF_LEN] = {0};

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
    INT16U tmp;

	//if (USART_GetITStatus(SWIFT_UART2_GPIO.USARTx, USART_IT_RXNE) != RESET)
	if (USART_GetITStatus(SWIFT_UART2_GPIO.USARTx, USART_IT_IDLE) != RESET)
    {
        tmp = SWIFT_UART2_GPIO.USARTx->SR;
        tmp = SWIFT_UART2_GPIO.USARTx->DR;
        (void)tmp;

        //USART_ClearITPendingBit(SWIFT_UART2_GPIO.USARTx, USART_IT_RXNE);
        USART_ClearITPendingBit(SWIFT_UART2_GPIO.USARTx, USART_IT_IDLE);

        //关闭DMA,防止处理其间有数据
        DMA_Cmd(DMA1_Stream5, DISABLE);

        //BSP_OS_Sem_Post(&SWIFT_UART2_Rev_Sem);
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
		//USART_ITConfig(SWIFT_UART2_GPIO.USARTx, USART_IT_RXNE, ENABLE);
		USART_ITConfig(SWIFT_UART2_GPIO.USARTx, USART_IT_IDLE, ENABLE);
		BSP_IntVectSet(HW_UART2_INT_ID, SWIFT_UART2_IntHandler);
		BSP_IntEn(HW_UART2_INT_ID);
	}
	else
	{
		BSP_IntDis(HW_UART2_INT_ID);
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
void SWIFT_UART2_Init(INT32U BaudRate)
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
	memset(SWIFT_UART2_Buff.pbuff,0,SWIFTUART2BuffSize);
	SWIFT_UART2_Buff.write_p = 0;
	SWIFT_UART2_Buff.read_p  = 0;

    /* uart2 dma buf */
    memset(uart2_buf, 0, UART2_BUF_LEN);
    uart2_rcv_fifo = kfifo_alloc(uart2_buf, UART2_BUF_LEN);

    HW_DMA_Rx_Config(USART2, DMA1_Stream5,  DMA1_Stream5_IRQn, DMA_Channel_4, &(USART2->DR), uart2_dma_buf, USART2_DMA_BUF_LEN);

	BSP_OS_Sem_Creat(&SWIFT_UART2_Rev_Sem,"Uart2_Rcv_Sem",0);

	BSP_OS_Sem_Creat(&SWIFT_UART2_Send_Sem,"Uart2_SndSem",1);
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

char SWIFT_USART2_GETC(void)
{
    char xch;

    INT32U len = kfifo_len(uart2_rcv_fifo);

    if (len > 0)
    {
        kfifo_get(uart2_rcv_fifo, &xch, 1);
        return xch;
    }

    return 0xff;
}

INT32S SWIFT_USART2_TSTC(void)
{
    //return BSP_UART_TST(SWIFT_UART2_GPIO.USARTx);
    INT32U len = kfifo_len(uart2_rcv_fifo);
    if (len > 0)
    {
        return 1;
    }

    return 0;
}

