

#define HW_EEPROM_DEF

#include <hw_eeprom.h>

#ifndef	HW_EEPROM_I2C_HARD_MODE
	str_I2C_GPIO_SOFT EEPROM_I2C;
#else
	str_I2C_GPIO_HARD EEPROM_I2C;

	DMA_InitTypeDef    sEEDMA_InitStructure; 
	NVIC_InitTypeDef   NVIC_InitStructure;
#endif



#define	EEPROM_AT24CM01	

#ifdef	EEPROM_AT24CM01
	 #define sEE_PAGESIZE           256
#else
	#define sEE_PAGESIZE           8
#endif

#define I2C_SLAVE_ADDRESS7      0xA0



#define sEE_FLAG_TIMEOUT         ((INT32U)0x1000)
#define sEE_LONG_TIMEOUT         ((INT32U)(10 * sEE_FLAG_TIMEOUT))

#define sEE_MAX_TRIALS_NUMBER     300

#define sEE_DIRECTION_TX                 0
#define sEE_DIRECTION_RX                 1  

volatile INT16U	sEEAddress = 0;  
volatile INT32U  sEETimeout = sEE_FLAG_TIMEOUT;   
volatile INT32U* sEEDataReadPointer;   
volatile INT8U*  sEEDataWritePointer;  
volatile INT8U   sEEDataNum;


/***********************************************************
**name:	HW_EEPROM_Init
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void HW_EEPROM_Init(void)
{
	

	#ifndef	HW_EEPROM_I2C_HARD_MODE
		EEPROM_I2C.GPIOx[I2C_PIN_SCK_INDEX] = HW_EEPROM_SCK_PIN_PORT;
		EEPROM_I2C.GPIO_InitStruct[I2C_PIN_SCK_INDEX].GPIO_Pin = HW_EEPROM_SCK_PIN;
		EEPROM_I2C.GPIO_InitStruct[I2C_PIN_SCK_INDEX].GPIO_Mode = GPIO_Mode_OUT;
		EEPROM_I2C.GPIO_InitStruct[I2C_PIN_SCK_INDEX].GPIO_Speed = GPIO_Speed_50MHz;
		EEPROM_I2C.GPIO_InitStruct[I2C_PIN_SCK_INDEX].GPIO_OType = GPIO_OType_PP;
		EEPROM_I2C.GPIO_InitStruct[I2C_PIN_SCK_INDEX].GPIO_PuPd  = GPIO_PuPd_NOPULL;

		EEPROM_I2C.GPIOx[I2C_PIN_DATA_INDEX] = HW_EEPROM_DATA_PIN_PORT;
		EEPROM_I2C.GPIO_InitStruct[I2C_PIN_DATA_INDEX].GPIO_Pin = HW_EEPROM_DATA_PIN;
		EEPROM_I2C.GPIO_InitStruct[I2C_PIN_DATA_INDEX].GPIO_Mode = GPIO_Mode_OUT;
		EEPROM_I2C.GPIO_InitStruct[I2C_PIN_DATA_INDEX].GPIO_Speed = GPIO_Speed_50MHz;
		EEPROM_I2C.GPIO_InitStruct[I2C_PIN_DATA_INDEX].GPIO_OType = GPIO_OType_PP;
		EEPROM_I2C.GPIO_InitStruct[I2C_PIN_DATA_INDEX].GPIO_PuPd  = GPIO_PuPd_NOPULL;
		
		BSP_I2C_SOFT_Init(EEPROM_I2C);
		BSP_I2C_DATA_WriteHigh(EEPROM_I2C);
		BSP_I2C_SCK_WriteHigh(EEPROM_I2C);
	#else
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	
		EEPROM_I2C.GPIOx[I2C_PIN_SCK_INDEX] = HW_EEPROM_SCK_PIN_PORT;
		EEPROM_I2C.GPIO_InitStruct[I2C_PIN_SCK_INDEX].GPIO_Pin = HW_EEPROM_SCK_PIN;
		EEPROM_I2C.GPIO_InitStruct[I2C_PIN_SCK_INDEX].GPIO_Mode = GPIO_Mode_AF;
		EEPROM_I2C.GPIO_InitStruct[I2C_PIN_SCK_INDEX].GPIO_Speed = GPIO_Speed_50MHz;
		EEPROM_I2C.GPIO_InitStruct[I2C_PIN_SCK_INDEX].GPIO_OType = GPIO_OType_OD;
		EEPROM_I2C.GPIO_InitStruct[I2C_PIN_SCK_INDEX].GPIO_PuPd  = GPIO_PuPd_NOPULL;

		EEPROM_I2C.GPIOx[I2C_PIN_DATA_INDEX] = HW_EEPROM_DATA_PIN_PORT;
		EEPROM_I2C.GPIO_InitStruct[I2C_PIN_DATA_INDEX].GPIO_Pin = HW_EEPROM_DATA_PIN;
		EEPROM_I2C.GPIO_InitStruct[I2C_PIN_DATA_INDEX].GPIO_Mode = GPIO_Mode_AF;
		EEPROM_I2C.GPIO_InitStruct[I2C_PIN_DATA_INDEX].GPIO_Speed = GPIO_Speed_50MHz;
		EEPROM_I2C.GPIO_InitStruct[I2C_PIN_DATA_INDEX].GPIO_OType = GPIO_OType_OD;
		EEPROM_I2C.GPIO_InitStruct[I2C_PIN_DATA_INDEX].GPIO_PuPd  = GPIO_PuPd_NOPULL;
	
		EEPROM_I2C.I2Cx = I2C1;
	
		EEPROM_I2C.I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
		EEPROM_I2C.I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
		EEPROM_I2C.I2C_InitStructure.I2C_OwnAddress1 = I2C_SLAVE_ADDRESS7;
		EEPROM_I2C.I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
		EEPROM_I2C.I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
		EEPROM_I2C.I2C_InitStructure.I2C_ClockSpeed = 400000;

		NVIC_InitStructure.NVIC_IRQChannel = HW_EEPROM_TXDMA_INT_ID;
  		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);

		NVIC_InitStructure.NVIC_IRQChannel = HW_EEPROM_RXDMA_INT_ID;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
		NVIC_Init(&NVIC_InitStructure);  

		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);

		DMA_ClearFlag(HW_EEPROM_I2C_DMA_STREAM_TX, DMA_FLAG_FEIF6 | DMA_FLAG_DMEIF6 | DMA_FLAG_TEIF6 | 
                                       DMA_FLAG_HTIF6 | DMA_FLAG_TCIF6);

		DMA_Cmd(HW_EEPROM_I2C_DMA_STREAM_TX, DISABLE);
		DMA_DeInit(HW_EEPROM_I2C_DMA_STREAM_TX);

		sEEDMA_InitStructure.DMA_Channel = DMA_Channel_1;
		sEEDMA_InitStructure.DMA_PeripheralBaseAddr = ((INT32U)0x40005410);
		sEEDMA_InitStructure.DMA_Memory0BaseAddr = (INT32U)0;    /* This parameter will be configured durig communication */;
		sEEDMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral; /* This parameter will be configured durig communication */
		sEEDMA_InitStructure.DMA_BufferSize = 0xFFFF;              /* This parameter will be configured durig communication */
		sEEDMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
		sEEDMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
		sEEDMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
		sEEDMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
		sEEDMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
		sEEDMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
		sEEDMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
		sEEDMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
		sEEDMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
		sEEDMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
		DMA_Init(HW_EEPROM_I2C_DMA_STREAM_TX, &sEEDMA_InitStructure);

		DMA_ClearFlag(HW_EEPROM_I2C_DMA_STREAM_RX, DMA_FLAG_FEIF0 | DMA_FLAG_DMEIF0 | DMA_FLAG_TEIF0 | 
                                       DMA_FLAG_HTIF0 | DMA_FLAG_TCIF0);

		DMA_Cmd(HW_EEPROM_I2C_DMA_STREAM_RX, DISABLE);

		DMA_DeInit(HW_EEPROM_I2C_DMA_STREAM_RX);
		DMA_Init(HW_EEPROM_I2C_DMA_STREAM_RX, &sEEDMA_InitStructure);
  
		DMA_ITConfig(DMA1_Stream0, DMA_IT_TC, ENABLE);
		DMA_ITConfig(DMA1_Stream6, DMA_IT_TC, ENABLE);    
		 
		BSP_I2C_HARD_Init(EEPROM_I2C);
		I2C_DMACmd(EEPROM_I2C.I2Cx, ENABLE);

		BSP_IntVectSet(HW_EEPROM_TXDMA_INT_ID, HW_EEPROM_I2C_DMA_TX_IRQHandler);
		BSP_IntVectSet(HW_EEPROM_RXDMA_INT_ID, HW_EEPROM_I2C_DMA_RX_IRQHandler);

		sEEAddress = I2C_SLAVE_ADDRESS7;
	#endif
	
}



int HW_EEPROM_ReadBuffer(INT32U ReadAddr, INT8U* pBuffer, INT32U *NumByteToRead)
{  

	INT8U dev_addr = sEEAddress;

	#ifdef	EEPROM_AT24CM01
		if( ReadAddr & 0x10000 )
			dev_addr |= 0x02;
		else
			dev_addr &= ~0x02;
	#endif

	sEEDataReadPointer = NumByteToRead;
  
	sEETimeout = sEE_FLAG_TIMEOUT;
	while(I2C_GetFlagStatus(EEPROM_I2C.I2Cx, I2C_FLAG_BUSY))
	{
		if((sEETimeout--) == 0) 
			return FAIL;
	}

  	// 发送一个起始信号
	I2C_GenerateSTART(EEPROM_I2C.I2Cx, ENABLE);
  
	sEETimeout = sEE_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(EEPROM_I2C.I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((sEETimeout--) == 0) 
			return FAIL;
	}
  
	// 发送地址
	I2C_Send7bitAddress(EEPROM_I2C.I2Cx, dev_addr, I2C_Direction_Transmitter);

  	sEETimeout = sEE_FLAG_TIMEOUT;
  	while(!I2C_CheckEvent(EEPROM_I2C.I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  	{
    		if((sEETimeout--) == 0) 
    			return FAIL;
  	} 

	#ifdef	EEPROM_AT24CM01
 		I2C_SendData(EEPROM_I2C.I2Cx, (ReadAddr & 0xFF00)>>8  );  
  		sEETimeout = sEE_FLAG_TIMEOUT;
  		while(!I2C_CheckEvent(EEPROM_I2C.I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
  		{
			if((sEETimeout--) == 0) 
				return FAIL;
  		}
	#endif
	
  	I2C_SendData(EEPROM_I2C.I2Cx, ReadAddr);  
  	sEETimeout = sEE_FLAG_TIMEOUT;
  	while(!I2C_CheckEvent(EEPROM_I2C.I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
  	{
		if((sEETimeout--) == 0) 
			return FAIL;
  	}

  	I2C_GenerateSTART(EEPROM_I2C.I2Cx, ENABLE);
  
  	sEETimeout = sEE_FLAG_TIMEOUT;
  	while(!I2C_CheckEvent(EEPROM_I2C.I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
  	{
   	 	if((sEETimeout--) == 0) 
   	 		return FAIL;
  	} 
  
	I2C_Send7bitAddress(EEPROM_I2C.I2Cx, dev_addr, I2C_Direction_Receiver);  
  
  	if ((*NumByteToRead) < 2)
	{
		sEETimeout = sEE_FLAG_TIMEOUT;
   	 	while(I2C_GetFlagStatus(EEPROM_I2C.I2Cx, I2C_FLAG_ADDR) == RESET)
    		{
      			if((sEETimeout--) == 0) 
      				return FAIL;
    		}     
    
    		I2C_AcknowledgeConfig(EEPROM_I2C.I2Cx, DISABLE);   
    
    		(void)EEPROM_I2C.I2Cx->SR2;
    
    		I2C_GenerateSTOP(EEPROM_I2C.I2Cx, ENABLE);
    
    		sEETimeout = sEE_FLAG_TIMEOUT;
   		 while(I2C_GetFlagStatus(EEPROM_I2C.I2Cx, I2C_FLAG_RXNE) == RESET)
    		{
      			if((sEETimeout--) == 0) 
      				return FAIL;
    		}
    
    		*pBuffer = I2C_ReceiveData(EEPROM_I2C.I2Cx);
    
    		(INT32U)(*NumByteToRead)--;        
    
    		sEETimeout = sEE_FLAG_TIMEOUT;
    		while(EEPROM_I2C.I2Cx->CR1 & I2C_CR1_STOP)
    		{
			if((sEETimeout--) == 0) 
				return FAIL;
		}  
    
    		I2C_AcknowledgeConfig(EEPROM_I2C.I2Cx, ENABLE);    
  	}
  	else
	{   
		sEETimeout = sEE_FLAG_TIMEOUT;
		while(!I2C_CheckEvent(EEPROM_I2C.I2Cx, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))
		{
			if((sEETimeout--) == 0) 
				return FAIL;
		}  
    
		sEEDMA_InitStructure.DMA_Memory0BaseAddr = (INT32U)pBuffer;
    		sEEDMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
    		sEEDMA_InitStructure.DMA_BufferSize = *NumByteToRead;      
    		DMA_Init(DMA1_Stream0, &sEEDMA_InitStructure);    
    
		I2C_DMALastTransferCmd(EEPROM_I2C.I2Cx, ENABLE); 
    
		DMA_Cmd(DMA1_Stream0, ENABLE);    
	}
  
	return SUCC;;
}




int HW_EEPROM_WritePage(INT32U WriteAddr, INT8U* pBuffer, INT8U* NumByteToWrite)
{ 
	INT8U dev_addr = sEEAddress;

	#ifdef	EEPROM_AT24CM01
		if( WriteAddr & 0x10000 ) 
			dev_addr |= 0x02;
		else
			dev_addr &= ~0x02;
	#endif

	sEEDataWritePointer = NumByteToWrite;  
  
	sEETimeout = sEE_LONG_TIMEOUT;
	while(I2C_GetFlagStatus(EEPROM_I2C.I2Cx, I2C_FLAG_BUSY))
	{
		if((sEETimeout--) == 0) 
			return FAIL;
	}
  
	I2C_GenerateSTART(EEPROM_I2C.I2Cx, ENABLE);
  
	sEETimeout = sEE_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(EEPROM_I2C.I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
	{
		if((sEETimeout--) == 0) 
			return FAIL;
	}
  
	sEETimeout = sEE_FLAG_TIMEOUT;
	I2C_Send7bitAddress(EEPROM_I2C.I2Cx, sEEAddress, I2C_Direction_Transmitter);

	sEETimeout = sEE_FLAG_TIMEOUT;
	while(!I2C_CheckEvent(EEPROM_I2C.I2Cx, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
	{
		if((sEETimeout--) == 0) 
			return FAIL;
	}

	#ifdef	EEPROM_AT24CM01
	
 		I2C_SendData(EEPROM_I2C.I2Cx, (WriteAddr & 0xFF00)>>8  );  
  		sEETimeout = sEE_FLAG_TIMEOUT;
  		while(!I2C_CheckEvent(EEPROM_I2C.I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
  		{
			if((sEETimeout--) == 0) 
				return FAIL;
  		}
	#endif

	I2C_SendData(EEPROM_I2C.I2Cx, WriteAddr);

  	sEETimeout = sEE_FLAG_TIMEOUT;  
  	while(!I2C_CheckEvent(EEPROM_I2C.I2Cx, I2C_EVENT_MASTER_BYTE_TRANSMITTING))
  	{
    		if((sEETimeout--) == 0) 
    			return FAIL;
  	}  

	sEEDMA_InitStructure.DMA_Memory0BaseAddr = (INT32U)pBuffer;
	sEEDMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;    
	sEEDMA_InitStructure.DMA_BufferSize = *NumByteToWrite;  
	DMA_Init(DMA1_Stream6, &sEEDMA_InitStructure);  
  
	DMA_Cmd(DMA1_Stream6, ENABLE);
  
	return SUCC;
}



int HW_EEPROM_WriteBuffer(INT32U WriteAddr, INT8U* pBuffer, INT32U NumByteToWrite)
{
	INT8U NumOfPage = 0, NumOfSingle = 0, count = 0;
	INT16U Addr = 0;

	Addr = WriteAddr % sEE_PAGESIZE;
	count = sEE_PAGESIZE - Addr;
	NumOfPage =  NumByteToWrite / sEE_PAGESIZE;
	NumOfSingle = NumByteToWrite % sEE_PAGESIZE;
 
	if(Addr == 0) 
	{
		if(NumOfPage == 0) 
		{
      			sEEDataNum = NumOfSingle;
      			HW_EEPROM_WritePage(WriteAddr,pBuffer, (INT8U*)(&sEEDataNum));
      			sEETimeout = sEE_LONG_TIMEOUT;
      			while (sEEDataNum > 0)
      			{
        			if((sEETimeout--) == 0) 
        				return FAIL;
      			}
      			HW_EEPROM_WaitEepromStandbyState();
		}
		else  
		{
			while(NumOfPage--)
			{
				sEEDataNum = sEE_PAGESIZE;        
				HW_EEPROM_WritePage(WriteAddr,pBuffer, (INT8U*)(&sEEDataNum)); 

				sEETimeout = sEE_LONG_TIMEOUT;
				while (sEEDataNum > 0)
				{
					if((sEETimeout--) == 0) 
						return FAIL;
        			}      
        			HW_EEPROM_WaitEepromStandbyState();
				WriteAddr +=  sEE_PAGESIZE;
				pBuffer += sEE_PAGESIZE;
			}

			if(NumOfSingle!=0)
			{
				sEEDataNum = NumOfSingle;          
				HW_EEPROM_WritePage(WriteAddr,pBuffer,  (INT8U*)(&sEEDataNum));
				sEETimeout = sEE_LONG_TIMEOUT;
				while (sEEDataNum > 0)
				{
					if((sEETimeout--) == 0) 
						return FAIL;
				}    
				HW_EEPROM_WaitEepromStandbyState();
			}
    		}
  	}
	else 
	{
		if(NumOfPage== 0) 
		{
			if (NumByteToWrite > count)
			{
				sEEDataNum = count;        
				HW_EEPROM_WritePage(WriteAddr,pBuffer,  (INT8U*)(&sEEDataNum));
        			sEETimeout = sEE_LONG_TIMEOUT;
       	 		while (sEEDataNum > 0)
        			{
          				if((sEETimeout--) == 0) 
          					return FAIL;
				}          
				HW_EEPROM_WaitEepromStandbyState();      

				sEEDataNum = (NumByteToWrite - count);          
				HW_EEPROM_WritePage((WriteAddr + count),(INT8U*)(pBuffer + count),  (INT8U*)(&sEEDataNum));
				sEETimeout = sEE_LONG_TIMEOUT;
				while (sEEDataNum > 0)
				{
					if((sEETimeout--) == 0) 
						return FAIL;
				}     
				HW_EEPROM_WaitEepromStandbyState();        
			}      
      			else      
      			{
				sEEDataNum = NumOfSingle;         
				HW_EEPROM_WritePage(WriteAddr,pBuffer,  (INT8U*)(&sEEDataNum));
				sEETimeout = sEE_LONG_TIMEOUT;
				while (sEEDataNum > 0)
				{
          				if((sEETimeout--) == 0) 
          					return FAIL;
				}          
				HW_EEPROM_WaitEepromStandbyState();        
			}     
		}
    		else
    		{
			NumByteToWrite -= count;
			NumOfPage =  NumByteToWrite / sEE_PAGESIZE;
			NumOfSingle = NumByteToWrite % sEE_PAGESIZE;
      
			if(count != 0)
			{  
				sEEDataNum = count;         
				HW_EEPROM_WritePage(WriteAddr,pBuffer,  (INT8U*)(&sEEDataNum));
				sEETimeout = sEE_LONG_TIMEOUT;
				while (sEEDataNum > 0)
				{
					if((sEETimeout--) == 0)
						return FAIL;
				}     
				HW_EEPROM_WaitEepromStandbyState();
				WriteAddr += count;
				pBuffer += count;
			} 
      
			while(NumOfPage--)
			{
				sEEDataNum = sEE_PAGESIZE;          
				HW_EEPROM_WritePage(WriteAddr,pBuffer,  (INT8U*)(&sEEDataNum));
				sEETimeout = sEE_LONG_TIMEOUT;
				while (sEEDataNum > 0)
				{
					if((sEETimeout--) == 0) 
						return FAIL;
				}        
				HW_EEPROM_WaitEepromStandbyState();
        			WriteAddr +=  sEE_PAGESIZE;
				pBuffer += sEE_PAGESIZE;  
			}
			if(NumOfSingle != 0)
			{
				sEEDataNum = NumOfSingle;           
				HW_EEPROM_WritePage(WriteAddr,pBuffer,  (INT8U*)(&sEEDataNum)); 
				sEETimeout = sEE_LONG_TIMEOUT;
				while (sEEDataNum > 0)
				{
					if((sEETimeout--) == 0) 
						return FAIL;
				}         
				HW_EEPROM_WaitEepromStandbyState();
			}
		}
	}  
}



int HW_EEPROM_WaitEepromStandbyState(void)      
{
	volatile INT16U tmpSR1 = 0;
	volatile INT32U sEETrials = 0;


	sEETimeout = sEE_LONG_TIMEOUT;
	while(I2C_GetFlagStatus(EEPROM_I2C.I2Cx, I2C_FLAG_BUSY))
	{
		if((sEETimeout--) == 0) 
			return FAIL;
	}

	while (1)
	{
    		I2C_GenerateSTART(EEPROM_I2C.I2Cx, ENABLE);

    		sEETimeout = sEE_FLAG_TIMEOUT;
    		while(!I2C_CheckEvent(EEPROM_I2C.I2Cx, I2C_EVENT_MASTER_MODE_SELECT))
    		{
     			 if((sEETimeout--) == 0) 
     			 return FAIL;
    		}    

    		I2C_Send7bitAddress(EEPROM_I2C.I2Cx, sEEAddress, I2C_Direction_Transmitter);
    
		sEETimeout = sEE_LONG_TIMEOUT;
		do
		{     
			tmpSR1 = EEPROM_I2C.I2Cx->SR1;
      			if((sEETimeout--) == 0) 
      				return FAIL;
    		}

    		while((tmpSR1 & (I2C_SR1_ADDR | I2C_SR1_AF)) == 0);
     
		if (tmpSR1 & I2C_SR1_ADDR)
    		{
			(void)EEPROM_I2C.I2Cx->SR2;
      
      			I2C_GenerateSTOP(EEPROM_I2C.I2Cx, ENABLE);
        
      			return SUCC;
    		}
    		else
		{
      			I2C_ClearFlag(EEPROM_I2C.I2Cx, I2C_FLAG_AF);                  
    		}
    
		if (sEETrials++ == sEE_MAX_TRIALS_NUMBER)
		{
			return FAIL;
		}
	}
}



void HW_EEPROM_I2C_DMA_TX_IRQHandler(void)
{
	if(DMA_GetFlagStatus(HW_EEPROM_I2C_DMA_STREAM_TX, DMA_FLAG_TCIF6) != RESET)
	{  
		DMA_Cmd(HW_EEPROM_I2C_DMA_STREAM_TX, DISABLE);
		DMA_ClearFlag(HW_EEPROM_I2C_DMA_STREAM_TX, DMA_FLAG_TCIF6);

		sEETimeout = sEE_LONG_TIMEOUT;
		while(!I2C_GetFlagStatus(EEPROM_I2C.I2Cx, I2C_FLAG_BTF))
		{
			if((sEETimeout--) == 0)
				return;
		}
  
		I2C_GenerateSTOP(EEPROM_I2C.I2Cx, ENABLE);
    
		*sEEDataWritePointer = 0;  
	}
}



void HW_EEPROM_I2C_DMA_RX_IRQHandler(void)
{
	if(DMA_GetFlagStatus(HW_EEPROM_I2C_DMA_STREAM_RX, DMA_FLAG_TCIF0) != RESET)
	{      
		I2C_GenerateSTOP(EEPROM_I2C.I2Cx, ENABLE);    
    
		DMA_Cmd(HW_EEPROM_I2C_DMA_STREAM_RX, DISABLE);
		DMA_ClearFlag(HW_EEPROM_I2C_DMA_STREAM_RX, DMA_FLAG_TCIF0);
    
		*sEEDataReadPointer = 0;
	}
}





#if 0

/***********************************************************
**name:	HW_EEPROM_WriteByte
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int HW_EEPROM_WriteByte(INT8U dev_addr, INT32U son_addr, INT8U son_addr_size,INT8U data)
{
	INT8U i;
	
	BSP_I2C_SendStart(EEPROM_I2C);

	BSP_I2C_WriteByte(EEPROM_I2C, dev_addr );

	BSP_I2C_DATA_SetInput(&EEPROM_I2C);
	if( BSP_I2C_Check_ACK_NoACK(EEPROM_I2C) != I2C_ACK)
	{
		BSP_I2C_DATA_SetOutput(&EEPROM_I2C);
		return FAIL;
	}
	BSP_I2C_DATA_SetOutput(&EEPROM_I2C);
	

	for(i=son_addr_size;i>0;i--)
	{
		BSP_I2C_WriteByte(EEPROM_I2C,(son_addr & (0xFF<<8*(i-1)))>>8*(i-1));

		BSP_I2C_DATA_SetInput(&EEPROM_I2C);
		if( BSP_I2C_Check_ACK_NoACK(EEPROM_I2C) != I2C_ACK)
		{
			BSP_I2C_DATA_SetOutput(&EEPROM_I2C);
			return FAIL;
		}
		BSP_I2C_DATA_SetOutput(&EEPROM_I2C);
		
	}

	BSP_I2C_WriteByte(EEPROM_I2C,data);

	BSP_I2C_DATA_SetInput(&EEPROM_I2C);
	if( BSP_I2C_Check_ACK_NoACK(EEPROM_I2C) != I2C_ACK)
	{
		BSP_I2C_DATA_SetOutput(&EEPROM_I2C);
		return FAIL;
	}
	BSP_I2C_DATA_SetOutput(&EEPROM_I2C);

	BSP_I2C_SendStop(EEPROM_I2C);

	return SUCC;
	
}

/***********************************************************
**name:	HW_EEPROM_ReadByte
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int HW_EEPROM_ReadByte(INT8U dev_addr, INT32U son_addr, INT8U son_addr_size,INT8U *data)
{
	INT8U i;
	
	BSP_I2C_SendStart(EEPROM_I2C);

	BSP_I2C_WriteByte(EEPROM_I2C, dev_addr );

	BSP_I2C_DATA_SetInput(&EEPROM_I2C);
	if( BSP_I2C_Check_ACK_NoACK(EEPROM_I2C) != I2C_ACK)
	{
		BSP_I2C_DATA_SetOutput(&EEPROM_I2C);
		return FAIL;
	}
	BSP_I2C_DATA_SetOutput(&EEPROM_I2C);

	for(i=son_addr_size;i>0;i--)
	{
		BSP_I2C_WriteByte(EEPROM_I2C,(son_addr & (0xFF<<8*(i-1)))>>8*(i-1));

		BSP_I2C_DATA_SetInput(&EEPROM_I2C);
		if( BSP_I2C_Check_ACK_NoACK(EEPROM_I2C) != I2C_ACK)
		{
			BSP_I2C_DATA_SetOutput(&EEPROM_I2C);
			return FAIL;
		}
		BSP_I2C_DATA_SetOutput(&EEPROM_I2C);
	}

	BSP_I2C_SendStart(EEPROM_I2C);

	BSP_I2C_WriteByte(EEPROM_I2C, dev_addr | 0x01 );

	BSP_I2C_DATA_SetInput(&EEPROM_I2C);
	if( BSP_I2C_Check_ACK_NoACK(EEPROM_I2C) != I2C_ACK)
	{
		BSP_I2C_DATA_SetOutput(&EEPROM_I2C);
		return FAIL;
	}

	*data = BSP_I2C_ReadByte(EEPROM_I2C);
	BSP_I2C_DATA_SetOutput(&EEPROM_I2C);
	BSP_I2C_SendNoACK(EEPROM_I2C);
	BSP_I2C_SendStop(EEPROM_I2C);

	return SUCC;
}


#endif

/***********************************************************
**name:	HW_EEPROM_Test
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int HW_EEPROM_Test(void)
{
	INT8U addr;
	INT8U data;
	int status;
	INT8U buff[10]={0},i,ii=0;
	INT8U sendbuff[10];
	INT32U buffsize=10;
	

	
	status = HW_EEPROM_ReadBuffer(0x00,buff,&buffsize);

	//while(1)
	{
		for(i=0;i<10;i++)
			sendbuff[i] = i+ii;
		buffsize = 10;
		status = HW_EEPROM_WriteBuffer(0x00,sendbuff,buffsize);

		buffsize = 10;
		status = HW_EEPROM_ReadBuffer(0x00,buff,&buffsize);
		ii++;
	}
	return status;
}


