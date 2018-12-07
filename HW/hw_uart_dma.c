#include "hw_uart_dma.h"
#include "hw_uart2.h"
#include "exports.h"
#include "kfifo.h"

void DMA1_Stream5_IRQHandler(void);

extern OS_SEM SWIFT_UART2_Rev_Sem;
extern struct kfifo *uart2_rcv_fifo;
unsigned char uart2_dma_buf[USART2_DMA_BUF_LEN] = {0};

void HW_DMA_Tx_Config(DMA_Stream_TypeDef *DMA_Streamx, unsigned char irq_ch, unsigned int chx, unsigned int par, unsigned int mar, unsigned short ndtr)
{
	DMA_InitTypeDef	DMA_InitStructure;
	NVIC_InitTypeDef	NVIC_InitStructure;  

	//得到当前stream是属于DMA2还是DMA1
	if((unsigned int)DMA_Streamx>(unsigned int)DMA2)
	{
		//DMA2时钟使能
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE); 
	}
	else 
	{
		//DMA1时钟使能 
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
	}
	
	DMA_DeInit(DMA_Streamx);

	//等待DMA可配置 
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE)
	{
		;
	}
	
	/* 配置 DMA Stream */
	DMA_InitStructure.DMA_Channel = chx;  //通道选择
	DMA_InitStructure.DMA_PeripheralBaseAddr = par;//DMA外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = mar;//DMA 存储器0地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//存储器到外设模式
	DMA_InitStructure.DMA_BufferSize = ndtr;//数据传输量 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据长度:8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//存储器数据长度:8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// 使用普通模式 
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//中等优先级
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输
	DMA_Init(DMA_Streamx, &DMA_InitStructure);//初始化DMA Stream


#if 0
	//Enable DMA channel IRQ Channel */
	NVIC_InitStructure.NVIC_IRQChannel = irq_ch;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
	NVIC_Init(&NVIC_InitStructure);
#endif
} 

void HW_DMA_Rx_Config(USART_TypeDef *USART, DMA_Stream_TypeDef *DMA_Streamx, unsigned char irq_ch, unsigned int chx, unsigned int par, unsigned int mar, unsigned short ndtr)
{
    DMA_InitTypeDef DMA_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;  

	//得到当前stream是属于DMA2还是DMA1
	if((unsigned int)DMA_Streamx > (unsigned int)DMA2)
	{
		//DMA2时钟使能 
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
	}
	else 
	{
		//DMA1时钟使能 
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
	}
	
	DMA_DeInit(DMA_Streamx);

	//等待DMA可配置
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE)
	{
		;
	}

	/* 配置 DMA Stream */
	DMA_InitStructure.DMA_Channel = chx;  //通道选择
	DMA_InitStructure.DMA_PeripheralBaseAddr = par;//DMA外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr = mar;//DMA 存储器0地址
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//外设到存储器模式
	DMA_InitStructure.DMA_BufferSize = ndtr;//数据传输量 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设非增量模式
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//存储器增量模式
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//外设数据长度:8位
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//存储器数据长度:8位
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// 使用普通模式 
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//中等优先级
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;		   
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//存储器突发单次传输
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//外设突发单次传输	

    /* 3. 配置DMA */
	DMA_Init(DMA_Streamx, &DMA_InitStructure);//初始化DMA Stream

    BSP_IntVectSet(irq_ch, DMA1_Stream5_IRQHandler);
    BSP_IntEn(irq_ch);

    /* 4.设置DMA中断 */
    NVIC_InitStructure.NVIC_IRQChannel = irq_ch;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStructure);	

    /* 5.使能串口的DMA接收 */
    USART_DMACmd(USART, USART_DMAReq_Rx, ENABLE);

    /* 6. Enable transfer complete interrupt */
    DMA_ITConfig(DMA_Streamx, DMA_IT_TC, ENABLE);

    /* 7.使能DMA */
    DMA_Cmd(DMA_Streamx, ENABLE);
}


//开启一次DMA传输
//DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7 
//ndtr:数据传输量  
void HW_DMA_Enable(DMA_Stream_TypeDef *DMA_Streamx, unsigned short ndtr)
{
	//关闭DMA传输 
	DMA_Cmd(DMA_Streamx, DISABLE);

	//确保DMA可以被设置
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE)
	{
		;
	}

	//数据传输量  
	DMA_SetCurrDataCounter(DMA_Streamx,ndtr);

	//使能DMA发送完成中断 
	DMA_ITConfig(DMA_Streamx, DMA_IT_TC, ENABLE);

	//开启DMA传输
	DMA_Cmd(DMA_Streamx, ENABLE);
}

/* COM1发送中断处理函数 */
void DMA2_Stream7_IRQHandler(void)
{

	OSIntEnter();
	if(DMA_GetFlagStatus(DMA2_Stream7, DMA_IT_TCIF7) != RESET)
	{
		DMA_ClearFlag(DMA2_Stream7, DMA_IT_TCIF7);

		//唤醒与之对应的发送任务
		//OSTaskResume(port->task_id);
	}

	OSIntExit();
}

/* COM1接收中断处理函数 */
void DMA2_Stream5_IRQHandler(void) 
{
	OSIntEnter();

	if(DMA_GetITStatus(DMA2_Stream5, DMA_IT_TCIF5) != RESET)
	{
		DMA_ClearFlag(DMA2_Stream5, DMA_IT_TCIF5);

		//拷贝接收缓冲区中的数据

		//DMA_SetCurrDataCounter(DMA2_Stream5, port->rx_dma->buf_size);
		DMA_ITConfig(DMA2_Stream5, DMA_IT_TC, ENABLE);
		DMA_Cmd(DMA2_Stream5, ENABLE);

		// 唤醒mscom处理任务
		//OSTaskResume(15);
		//OSTaskResume(11);
	}

	OSIntExit();
}


/* COM2发送中断处理函数 */
void DMA1_Stream6_IRQHandler(void) 
{

	OSIntEnter();
	if(DMA_GetFlagStatus(DMA1_Stream6, DMA_IT_TCIF6) != RESET)
	{
		DMA_ClearFlag(DMA1_Stream6, DMA_IT_TCIF6);

		//唤醒与之对应的发送任务
		//OSTaskResume(port->task_id);
	}

	OSIntExit();
}

/* COM2接收中断处理函数 */
void DMA1_Stream5_IRQHandler(void) 
{
    unsigned int len;

	if(DMA_GetITStatus(DMA1_Stream5, DMA_IT_TCIF5) != RESET)
	{
		DMA_ClearFlag(DMA1_Stream5, DMA_IT_TCIF5);

		//拷贝接收缓冲区中的数据
        len = USART2_DMA_BUF_LEN - DMA1_Stream5->NDTR;

        kfifo_put(uart2_rcv_fifo, uart2_dma_buf, len);

		DMA_SetCurrDataCounter(DMA1_Stream5, USART2_DMA_BUF_LEN);
		DMA_ITConfig(DMA1_Stream5, DMA_IT_TC, ENABLE);
		DMA_Cmd(DMA1_Stream5, ENABLE);

        BSP_OS_Sem_Post(&SWIFT_UART2_Rev_Sem);
	}
}

/* COM3发送中断处理函数 */
void DMA1_Stream3_IRQHandler(void) 
{
	OSIntEnter();
	if(DMA_GetITStatus(DMA1_Stream3, DMA_IT_TCIF3) != RESET)
	{
		DMA_ClearFlag(DMA1_Stream3, DMA_IT_TCIF3);

		//唤醒与之对应的发送任务
		//OSTaskResume(port->task_id);
	}

	OSIntExit();
}

/* COM3接收中断处理函数 */
void DMA1_Stream1_IRQHandler(void) 
{
	OSIntEnter();

	if(DMA_GetITStatus(DMA1_Stream1, DMA_IT_TCIF1) != RESET)
	{
		DMA_ClearFlag(DMA1_Stream1,DMA_IT_TCIF1);

		//拷贝接收缓冲区中的数据
		//DMA_SetCurrDataCounter(DMA1_Stream1, port->rx_dma->buf_size);
		DMA_ITConfig(DMA1_Stream1, DMA_IT_TC, ENABLE);
		DMA_Cmd(DMA1_Stream1, ENABLE);
	}

	OSIntExit();
}


