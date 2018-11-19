


#define BSP_ETH_DEF

#include <bsp_eth.h>



volatile	static	INT8U 	EthLinkStatus = 0;

extern void LwIP_SendSem(void);

extern ETH_DMADESCTypeDef  *DMATxDescToSet;
extern ETH_DMADESCTypeDef  *DMARxDescToGet;
extern ETH_DMADESCTypeDef  DMARxDscrTab[ETH_RXBUFNB]; 
extern ETH_DMADESCTypeDef  DMATxDscrTab[ETH_TXBUFNB];

extern BYTE Rx_Buff[ETH_RXBUFNB][ETH_RX_BUF_SIZE], Tx_Buff[ETH_TXBUFNB][ETH_TX_BUF_SIZE];
extern ETH_DMA_Rx_Frame_infos *DMA_RX_FRAME_infos;
void  (*BSP_ETH_IRQHandler)(void) = NULL;


INT8U ETH_MAC_ADDR[6] = {DEFAULT_MAC_ADDR0,	DEFAULT_MAC_ADDR1,		DEFAULT_MAC_ADDR2,
							DEFAULT_MAC_ADDR3	,	DEFAULT_MAC_ADDR4	,	DEFAULT_MAC_ADDR5};

/*******************************************************************************
* Function Name  : BSP_ETH_GetCurrentTxBuffer
* Description    : Return the address of the buffer pointed by the current descritor.
* Input          : None
* Output         : None
* Return         : Buffer address
*******************************************************************************/
INT8U * BSP_ETH_GetCurrentTxBuffer(void)
{
	INT8U* tx_addr = (BYTE*)DMATxDescToSet->Buffer1Addr;
	/* Return Buffer address */
	return tx_addr;   
}


/*******************************************************************************
* Function Name  : BSP_ETH_IsRxPktValid
* Description    : 
* Input          : None
* Output         : None
* Return         : 
*******************************************************************************/
INT32U BSP_ETH_IsRxPktValid(void)
{
	return ETH_GetRxPktSize(DMARxDescToGet); 
}

/**
  * @brief  Configure the PHY to generate an interrupt on change of link status.
  * @param PHYAddress: external PHY address  
  * @retval None
  */
INT32U Eth_Link_PHYITConfig(INT16U PHYAddress)
{
  INT32U tmpreg = 0;

  /* Read MICR register */
  tmpreg = ETH_ReadPHYRegister(PHYAddress, PHY_MICR);

  /* Enable output interrupt events to signal via the INT pin */
  tmpreg |= (uint32_t)PHY_MICR_INT_EN | PHY_MICR_INT_OE;
  if(!(ETH_WritePHYRegister(PHYAddress, PHY_MICR, tmpreg)))
  {
    /* Return ERROR in case of write timeout */
    return ETH_ERROR;
  }

  /* Read MISR register */
  tmpreg = ETH_ReadPHYRegister(PHYAddress, PHY_MISR);

  /* Enable Interrupt on change of link status */
  tmpreg |= (uint32_t)PHY_MISR_LINK_INT_EN;
  if(!(ETH_WritePHYRegister(PHYAddress, PHY_MISR, tmpreg)))
  {
    /* Return ERROR in case of write timeout */
    return ETH_ERROR;
  }
  /* Return SUCCESS */
  return ETH_SUCCESS;   
}

/**
  * @brief  EXTI configuration for Ethernet link status.
  * @param PHYAddress: external PHY address  
  * @retval None
  */
void Eth_Link_EXTIConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the INT (PG12) Clock */
  RCC_AHB1PeriphClockCmd(ETH_LINK_GPIO_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  /* Configure INT pin as input */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Pin = ETH_LINK_PIN;
  GPIO_Init(ETH_LINK_GPIO_PORT, &GPIO_InitStructure);

  /* Connect EXTI Line to INT Pin */
  SYSCFG_EXTILineConfig(ETH_LINK_EXTI_PORT_SOURCE, ETH_LINK_EXTI_PIN_SOURCE);

  /* Configure EXTI line */
  EXTI_InitStructure.EXTI_Line = ETH_LINK_EXTI_LINE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;  
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

	#if 0
  /* Enable and set the EXTI interrupt to the highest priority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
  NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	#endif
 	
}

/**
  * @brief  This function handles Ethernet link status.
  * @param  None
  * @retval None
  */
void Eth_Link_ITHandler(INT16U PHYAddress)
{
  /* Check whether the link interrupt has occurred or not */
  if(((ETH_ReadPHYRegister(PHYAddress, PHY_MISR)) & PHY_LINK_STATUS) != 0)
  {
  	 if(((ETH_ReadPHYRegister(PHYAddress, PHY_STS)) & PHY_LINK_STATUS_LINK) != 0)
	{
// 		ETH_BSP_Config();
		LWIP_Init();
  	 }
  }
}

/***********************************************************
**name:	BSP_ETH_Init
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void  BSP_InitEth(void)
{
	INT32S	i;
	

	CPU_SR_ALLOC();  
	CPU_CRITICAL_ENTER();

	//ETH_BSP_Config();

	

	
	ETH_MACaddr_Config(ETH_MAC_ADDR);

 	ETH_DMATxDescChainInit(DMATxDscrTab, &Tx_Buff[0][0], ETH_TXBUFNB);

	ETH_DMARxDescChainInit(DMARxDscrTab, &Rx_Buff[0][0], ETH_RXBUFNB);

	
 
	    

	for(i=0; i<ETH_RXBUFNB; ++i)
		ETH_DMARxDescReceiveITConfig(&DMARxDscrTab[i], ENABLE);

	#ifdef CHECKSUM_BY_HARDWARE
	// Enable the checksum insertion for the Tx frames 
	for(i=0; i<ETH_TXBUFNB; i++)
		ETH_DMATxDescChecksumInsertionConfig(&DMATxDscrTab[i], ETH_DMATxDesc_ChecksumTCPUDPICMPFull);
	#endif

	CPU_CRITICAL_EXIT();

	
	//Enable MAC and DMA transmission and reception 
	ETH_Start();  
}

/***********************************************************
**name:	ETH_BSP_Config
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void ETH_BSP_Config(void)
{
	ETH_GPIO_Config();

	BSP_ETH_IRQHandler = LwIP_SendSem;
	
	BSP_IntVectSet(BSP_INT_ID_ETH, ETH_IRQHandler);
	BSP_IntEn(BSP_INT_ID_ETH);

	ETH_MACDMA_Config();

	
}


/***********************************************************
**name:	ETH_GPIO_Config
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void ETH_GPIO_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  // Enable GPIOs clocks 
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOB |
                         RCC_AHB1Periph_GPIOC | RCC_AHB1Periph_GPIOI |
                         RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOH |
                         RCC_AHB1Periph_GPIOF, ENABLE);

  // Enable SYSCFG clock
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
 
  //Configure MCO (PA8)
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  
  /* MII/RMII Media interface selection --------------------------------------*/
#ifdef MII_MODE /* Mode MII with STM322xG-EVAL  */
 #ifdef PHY_CLOCK_MCO

  /* Output HSE clock (25MHz) on MCO pin (PA8) to clock the PHY */
  RCC_MCO1Config(RCC_MCO1Source_HSE, RCC_MCO1Div_1);
 #endif /* PHY_CLOCK_MCO */

  SYSCFG_ETH_MediaInterfaceConfig(SYSCFG_ETH_MediaInterface_MII);
#elif defined RMII_MODE  /* Mode RMII with STM322xG-EVAL */

  SYSCFG_ETH_MediaInterfaceConfig(SYSCFG_ETH_MediaInterface_RMII);
#endif

#ifdef MII_MODE  
/* Ethernet pins configuration ************************************************/
   /*
        ETH_MDIO -------------------------> PA2
        ETH_MDC --------------------------> PC1
        ETH_PPS_OUT ----------------------> PB5
        ETH_MII_CRS ----------------------> PH2
        ETH_MII_COL ----------------------> PH3
        ETH_MII_RX_ER --------------------> PI10
        ETH_MII_RXD2 ---------------------> PH6
        ETH_MII_RXD3 ---------------------> PH7
        ETH_MII_TX_CLK -------------------> PC3
        ETH_MII_TXD2 ---------------------> PC2
        ETH_MII_TXD3 ---------------------> PB8
        ETH_MII_RX_CLK/ETH_RMII_REF_CLK---> PA1
        ETH_MII_RX_DV/ETH_RMII_CRS_DV ----> PA7
        ETH_MII_RXD0/ETH_RMII_RXD0 -------> PC4
        ETH_MII_RXD1/ETH_RMII_RXD1 -------> PC5
        ETH_MII_TX_EN/ETH_RMII_TX_EN -----> PG11
        ETH_MII_TXD0/ETH_RMII_TXD0 -------> PG13
        ETH_MII_TXD1/ETH_RMII_TXD1 -------> PG14
                                                  */

  /* Configure PA1, PA2 and PA7 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_7;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_ETH);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_ETH);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_ETH);

  /* Configure PB5 and PB8 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_8;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_ETH);	
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_ETH);

  /* Configure PC1, PC2, PC3, PC4 and PC5 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource1, GPIO_AF_ETH);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource2, GPIO_AF_ETH);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource3, GPIO_AF_ETH);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource4, GPIO_AF_ETH);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource5, GPIO_AF_ETH);
                                
  /* Configure PG11, PG14 and PG13 */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource11, GPIO_AF_ETH);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource13, GPIO_AF_ETH);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource14, GPIO_AF_ETH);

  /* Configure PH2, PH3, PH6, PH7 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6 | GPIO_Pin_7;
  GPIO_Init(GPIOH, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource2, GPIO_AF_ETH);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource3, GPIO_AF_ETH);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource6, GPIO_AF_ETH);
  GPIO_PinAFConfig(GPIOH, GPIO_PinSource7, GPIO_AF_ETH);

  /* Configure PI10 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_Init(GPIOI, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOI, GPIO_PinSource10, GPIO_AF_ETH);
#elif defined RMII_MODE
/* Ethernet pins configuration ************************************************/
   /*
        ETH_MDIO -------------------------> PA2
        ETH_MDC --------------------------> PC1


        ETH_MII_CRS ----------------------> PH2
        ETH_MII_COL ----------------------> PH3
        ETH_MII_RX_ER --------------------> PI10
        ETH_MII_RXD2 ---------------------> PH6
        ETH_MII_RXD3 ---------------------> PH7



        ETH_MII_RX_CLK/ETH_RMII_REF_CLK---> PA1
        ETH_MII_RX_DV/ETH_RMII_CRS_DV ----> PA7
        ETH_MII_RXD0/ETH_RMII_RXD0 -------> PC4
        ETH_MII_RXD1/ETH_RMII_RXD1 -------> PC5
        ETH_MII_TX_EN/ETH_RMII_TX_EN -----> PG11
        ETH_MII_TXD0/ETH_RMII_TXD0 -------> PG13
        ETH_MII_TXD1/ETH_RMII_TXD1 -------> PG14
                                                  */

  /* Configure PA1, PA2 and PA7 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_7;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_ETH);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_ETH);
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_ETH);

  /* Configure PC1, PC4 and PC5 */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_4 | GPIO_Pin_5;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource1, GPIO_AF_ETH);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource4, GPIO_AF_ETH);
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource5, GPIO_AF_ETH);
                                
  /* Configure PG11, PG14 and PG13 */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_14;
  GPIO_Init(GPIOG, &GPIO_InitStructure);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource11, GPIO_AF_ETH);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource13, GPIO_AF_ETH);
  GPIO_PinAFConfig(GPIOG, GPIO_PinSource14, GPIO_AF_ETH);
#endif

	
}


/***********************************************************
**name:	ETH_MACDMA_Config
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void ETH_MACDMA_Config(void)
{
	INT32U timeout = 0;
	ETH_InitTypeDef ETH_InitStructure;

	// Enable ETHERNET clock  
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_ETH_MAC | RCC_AHB1Periph_ETH_MAC_Tx |
                         RCC_AHB1Periph_ETH_MAC_Rx, ENABLE);                                             

	//Reset ETHERNET on AHB Bus 
	ETH_DeInit();

	//Software reset 
	ETH_SoftwareReset();

	//Wait for software reset
	while (ETH_GetSoftwareResetStatus() == SET)
	{
		BSP_Delay_ms(1);
		if( timeout++ > 10000)
			return;
	}

	//ETHERNET Configuration
	//Call ETH_StructInit if you don't like to configure all ETH_InitStructure parameter 
	ETH_StructInit(&ETH_InitStructure);

	//Fill ETH_InitStructure parametrs 
	//------------------------   MAC   -----------------------------------
	ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Enable;
	//ETH_InitStructure.ETH_AutoNegotiation = ETH_AutoNegotiation_Disable; 
	//  ETH_InitStructure.ETH_Speed = ETH_Speed_10M;
	//  ETH_InitStructure.ETH_Mode = ETH_Mode_FullDuplex;   

	ETH_InitStructure.ETH_LoopbackMode = ETH_LoopbackMode_Disable;
	ETH_InitStructure.ETH_RetryTransmission = ETH_RetryTransmission_Disable;
	ETH_InitStructure.ETH_AutomaticPadCRCStrip = ETH_AutomaticPadCRCStrip_Disable;
	ETH_InitStructure.ETH_ReceiveAll = ETH_ReceiveAll_Disable;
	ETH_InitStructure.ETH_BroadcastFramesReception = ETH_BroadcastFramesReception_Enable;
	ETH_InitStructure.ETH_PromiscuousMode = ETH_PromiscuousMode_Disable;
	ETH_InitStructure.ETH_MulticastFramesFilter = ETH_MulticastFramesFilter_Perfect;
	ETH_InitStructure.ETH_UnicastFramesFilter = ETH_UnicastFramesFilter_Perfect;
	#ifdef CHECKSUM_BY_HARDWARE
		ETH_InitStructure.ETH_ChecksumOffload = ETH_ChecksumOffload_Enable;
	#endif

	//------------------------   DMA   -----------------------------------  
  
	//When we use the Checksum offload feature, we need to enable the Store and Forward mode: 
	//the store and forward guarantee that a whole frame is stored in the FIFO, so the MAC can insert/verify the checksum, 
	//if the checksum is OK the DMA can handle the frame otherwise the frame is dropped 
	ETH_InitStructure.ETH_DropTCPIPChecksumErrorFrame = ETH_DropTCPIPChecksumErrorFrame_Enable; 
	ETH_InitStructure.ETH_ReceiveStoreForward = ETH_ReceiveStoreForward_Enable;         
	ETH_InitStructure.ETH_TransmitStoreForward = ETH_TransmitStoreForward_Enable;     
 
	ETH_InitStructure.ETH_ForwardErrorFrames = ETH_ForwardErrorFrames_Disable;       
	ETH_InitStructure.ETH_ForwardUndersizedGoodFrames = ETH_ForwardUndersizedGoodFrames_Disable;   
	ETH_InitStructure.ETH_SecondFrameOperate = ETH_SecondFrameOperate_Enable;
	ETH_InitStructure.ETH_AddressAlignedBeats = ETH_AddressAlignedBeats_Enable;      
	ETH_InitStructure.ETH_FixedBurst = ETH_FixedBurst_Enable;                
	ETH_InitStructure.ETH_RxDMABurstLength = ETH_RxDMABurstLength_32Beat;          
	ETH_InitStructure.ETH_TxDMABurstLength = ETH_TxDMABurstLength_32Beat;
	ETH_InitStructure.ETH_DMAArbitration = ETH_DMAArbitration_RoundRobin_RxTx_2_1;

	//Configure Ethernet 
	ETH_Init(&ETH_InitStructure, DP83848_PHY_ADDRESS);

	//Enable the Ethernet Rx Interrupt 
	ETH_DMAITConfig(ETH_DMA_IT_NIS | ETH_DMA_IT_R, ENABLE);
}

/***********************************************************
**name:	ETH_MACaddr_Config
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void ETH_MACaddr_Config(INT8U *mac_addr)
{
	ETH_MACAddressConfig(ETH_MAC_Address0, mac_addr); 
}



/***********************************************************
**name:	ETH_IRQHandler
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void ETH_IRQHandler(void)
{

	if ( ETH_GetDMAFlagStatus(ETH_DMA_FLAG_R) == SET)
	{
	    	// 此处发送一个信号量
	    	if(BSP_ETH_IRQHandler)
      			BSP_ETH_IRQHandler();
	}
	
	//Clear the interrupt flags. 
	//Clear the Eth DMA Rx IT pending bits 
	ETH_DMAClearITPendingBit(ETH_DMA_IT_R);
	ETH_DMAClearITPendingBit(ETH_DMA_IT_NIS);
}


/***********************************************************
**name:	BSP_ETH_ReleaseDescriptor
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void BSP_ETH_ReleaseDescriptor(FrameTypeDef * pframe)
{
	INT32S i;
	volatile ETH_DMADESCTypeDef *DMARxNextDesc;
	//Release descriptors to DMA 
	//Check if received frame with multiple DMA buffer segments 
	if (DMA_RX_FRAME_infos->Seg_Count > 1)
		DMARxNextDesc = DMA_RX_FRAME_infos->FS_Rx_Desc;
	else
		DMARxNextDesc = pframe->descriptor;
  
	//Set Own bit in Rx descriptors: gives the buffers back to DMA 
	for (i=0; i<DMA_RX_FRAME_infos->Seg_Count; i++)
	{  
		DMARxNextDesc->Status = ETH_DMARxDesc_OWN;
		DMARxNextDesc = (ETH_DMADESCTypeDef *)(DMARxNextDesc->Buffer2NextDescAddr);
	}
  
	//Clear Segment_Count 
	DMA_RX_FRAME_infos->Seg_Count =0;
  
  
	//When Rx Buffer unavailable flag is set: clear it and resume reception 
	if ((ETH->DMASR & ETH_DMASR_RBUS) != RESET)
	{
		//Clear RBUS ETHERNET DMA flag 
		ETH->DMASR = ETH_DMASR_RBUS;
      
		//Resume DMA reception 
		ETH->DMARPDR = 0;
	}
}


/***********************************************************
**name:	BSP_ETH_ReleaseDescriptor
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
INT8U BSP_ETH_GetDescriptorStatus(FrameTypeDef * pframe)
{
	return ((pframe->descriptor->Status & ETH_DMARxDesc_ES) == RESET)?true:false;
}

