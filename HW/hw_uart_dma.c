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

	//�õ���ǰstream������DMA2����DMA1
	if((unsigned int)DMA_Streamx>(unsigned int)DMA2)
	{
		//DMA2ʱ��ʹ��
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE); 
	}
	else 
	{
		//DMA1ʱ��ʹ�� 
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
	}
	
	DMA_DeInit(DMA_Streamx);

	//�ȴ�DMA������ 
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE)
	{
		;
	}
	
	/* ���� DMA Stream */
	DMA_InitStructure.DMA_Channel = chx;  //ͨ��ѡ��
	DMA_InitStructure.DMA_PeripheralBaseAddr = par;//DMA�����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = mar;//DMA �洢��0��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;//�洢��������ģʽ
	DMA_InitStructure.DMA_BufferSize = ndtr;//���ݴ����� 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//���������ģʽ
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�洢������ģʽ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//�������ݳ���:8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//�洢�����ݳ���:8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// ʹ����ͨģʽ 
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//�е����ȼ�
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//�洢��ͻ�����δ���
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//����ͻ�����δ���
	DMA_Init(DMA_Streamx, &DMA_InitStructure);//��ʼ��DMA Stream


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

	//�õ���ǰstream������DMA2����DMA1
	if((unsigned int)DMA_Streamx > (unsigned int)DMA2)
	{
		//DMA2ʱ��ʹ�� 
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);
	}
	else 
	{
		//DMA1ʱ��ʹ�� 
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);
	}
	
	DMA_DeInit(DMA_Streamx);

	//�ȴ�DMA������
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE)
	{
		;
	}

	/* ���� DMA Stream */
	DMA_InitStructure.DMA_Channel = chx;  //ͨ��ѡ��
	DMA_InitStructure.DMA_PeripheralBaseAddr = par;//DMA�����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr = mar;//DMA �洢��0��ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;//���赽�洢��ģʽ
	DMA_InitStructure.DMA_BufferSize = ndtr;//���ݴ����� 
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//���������ģʽ
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�洢������ģʽ
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;//�������ݳ���:8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;//�洢�����ݳ���:8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;// ʹ����ͨģʽ 
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;//�е����ȼ�
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;		   
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;//�洢��ͻ�����δ���
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;//����ͻ�����δ���	

    /* 3. ����DMA */
	DMA_Init(DMA_Streamx, &DMA_InitStructure);//��ʼ��DMA Stream

    BSP_IntVectSet(irq_ch, DMA1_Stream5_IRQHandler);
    BSP_IntEn(irq_ch);

    /* 4.����DMA�ж� */
    NVIC_InitStructure.NVIC_IRQChannel = irq_ch;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStructure);	

    /* 5.ʹ�ܴ��ڵ�DMA���� */
    USART_DMACmd(USART, USART_DMAReq_Rx, ENABLE);

    /* 6. Enable transfer complete interrupt */
    DMA_ITConfig(DMA_Streamx, DMA_IT_TC, ENABLE);

    /* 7.ʹ��DMA */
    DMA_Cmd(DMA_Streamx, ENABLE);
}


//����һ��DMA����
//DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7 
//ndtr:���ݴ�����  
void HW_DMA_Enable(DMA_Stream_TypeDef *DMA_Streamx, unsigned short ndtr)
{
	//�ر�DMA���� 
	DMA_Cmd(DMA_Streamx, DISABLE);

	//ȷ��DMA���Ա�����
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE)
	{
		;
	}

	//���ݴ�����  
	DMA_SetCurrDataCounter(DMA_Streamx,ndtr);

	//ʹ��DMA��������ж� 
	DMA_ITConfig(DMA_Streamx, DMA_IT_TC, ENABLE);

	//����DMA����
	DMA_Cmd(DMA_Streamx, ENABLE);
}

/* COM1�����жϴ����� */
void DMA2_Stream7_IRQHandler(void)
{

	OSIntEnter();
	if(DMA_GetFlagStatus(DMA2_Stream7, DMA_IT_TCIF7) != RESET)
	{
		DMA_ClearFlag(DMA2_Stream7, DMA_IT_TCIF7);

		//������֮��Ӧ�ķ�������
		//OSTaskResume(port->task_id);
	}

	OSIntExit();
}

/* COM1�����жϴ����� */
void DMA2_Stream5_IRQHandler(void) 
{
	OSIntEnter();

	if(DMA_GetITStatus(DMA2_Stream5, DMA_IT_TCIF5) != RESET)
	{
		DMA_ClearFlag(DMA2_Stream5, DMA_IT_TCIF5);

		//�������ջ������е�����

		//DMA_SetCurrDataCounter(DMA2_Stream5, port->rx_dma->buf_size);
		DMA_ITConfig(DMA2_Stream5, DMA_IT_TC, ENABLE);
		DMA_Cmd(DMA2_Stream5, ENABLE);

		// ����mscom��������
		//OSTaskResume(15);
		//OSTaskResume(11);
	}

	OSIntExit();
}


/* COM2�����жϴ����� */
void DMA1_Stream6_IRQHandler(void) 
{

	OSIntEnter();
	if(DMA_GetFlagStatus(DMA1_Stream6, DMA_IT_TCIF6) != RESET)
	{
		DMA_ClearFlag(DMA1_Stream6, DMA_IT_TCIF6);

		//������֮��Ӧ�ķ�������
		//OSTaskResume(port->task_id);
	}

	OSIntExit();
}

/* COM2�����жϴ����� */
void DMA1_Stream5_IRQHandler(void) 
{
    unsigned int len;

	if(DMA_GetITStatus(DMA1_Stream5, DMA_IT_TCIF5) != RESET)
	{
		DMA_ClearFlag(DMA1_Stream5, DMA_IT_TCIF5);

		//�������ջ������е�����
        len = USART2_DMA_BUF_LEN - DMA1_Stream5->NDTR;

        kfifo_put(uart2_rcv_fifo, uart2_dma_buf, len);

		DMA_SetCurrDataCounter(DMA1_Stream5, USART2_DMA_BUF_LEN);
		DMA_ITConfig(DMA1_Stream5, DMA_IT_TC, ENABLE);
		DMA_Cmd(DMA1_Stream5, ENABLE);

        BSP_OS_Sem_Post(&SWIFT_UART2_Rev_Sem);
	}
}

/* COM3�����жϴ����� */
void DMA1_Stream3_IRQHandler(void) 
{
	OSIntEnter();
	if(DMA_GetITStatus(DMA1_Stream3, DMA_IT_TCIF3) != RESET)
	{
		DMA_ClearFlag(DMA1_Stream3, DMA_IT_TCIF3);

		//������֮��Ӧ�ķ�������
		//OSTaskResume(port->task_id);
	}

	OSIntExit();
}

/* COM3�����жϴ����� */
void DMA1_Stream1_IRQHandler(void) 
{
	OSIntEnter();

	if(DMA_GetITStatus(DMA1_Stream1, DMA_IT_TCIF1) != RESET)
	{
		DMA_ClearFlag(DMA1_Stream1,DMA_IT_TCIF1);

		//�������ջ������е�����
		//DMA_SetCurrDataCounter(DMA1_Stream1, port->rx_dma->buf_size);
		DMA_ITConfig(DMA1_Stream1, DMA_IT_TC, ENABLE);
		DMA_Cmd(DMA1_Stream1, ENABLE);
	}

	OSIntExit();
}


