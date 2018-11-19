


#define BSP_UART_DEF

#include <bsp_uart.h>


/***********************************************************
**name:	BSP_UART_PortClkSwitch
**describe:  UART口组的时钟开关函数
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void BSP_UART_PortClkSwitch(USART_TypeDef* USARTx,INT8U switch_set)
{
	switch( (INT32U)USARTx )
	{
		case (INT32U)USART1:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, switch_set);	
			break;

		case (INT32U)USART2:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, switch_set);	
			break;

		case (INT32U)USART3:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, switch_set);	
			break;

		case (INT32U)UART4:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, switch_set);	
			break;

		case (INT32U)UART5:
			RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, switch_set);	
			break;

		case (INT32U)USART6:
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, switch_set);	
			break;

	}

}



/***********************************************************
**name:	BSP_UART_Init
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void BSP_UART_Init(str_UART_GPIO_HARD UART_GPIO_HARD)
{
	INT16U	GPIO_PinSource;
	INT8U i,ii;
	INT8U	GPIO_AF;
	
	BSP_GPIO_PortClkSwitch(UART_GPIO_HARD.GPIOx[UART_GPIO_TX_INDEX],ENABLE);
	BSP_GPIO_PortClkSwitch(UART_GPIO_HARD.GPIOx[UART_GPIO_RX_INDEX],ENABLE);
	GPIO_Init(UART_GPIO_HARD.GPIOx[UART_GPIO_TX_INDEX],&UART_GPIO_HARD.GPIO_InitStruct[UART_GPIO_TX_INDEX]);
	GPIO_Init(UART_GPIO_HARD.GPIOx[UART_GPIO_RX_INDEX],&UART_GPIO_HARD.GPIO_InitStruct[UART_GPIO_RX_INDEX]);

	BSP_UART_PortClkSwitch(UART_GPIO_HARD.USARTx,ENABLE);

	for(i=0;i<2;i++)
	{
		for(ii=0;ii<16;ii++)
		{
			if(UART_GPIO_HARD.GPIO_InitStruct[i].GPIO_Pin & (1<<ii))
			{
				GPIO_PinSource = ii;
				break;
			}
		}

		if(UART_GPIO_HARD.USARTx == USART1)
		{
			GPIO_AF = GPIO_AF_USART1;
		}
		else if(UART_GPIO_HARD.USARTx == USART2)
		{
			GPIO_AF = GPIO_AF_USART2;
		}
		else if(UART_GPIO_HARD.USARTx == USART3)
		{
			GPIO_AF = GPIO_AF_USART3;
		}
		else if(UART_GPIO_HARD.USARTx == UART4)
		{
			GPIO_AF = GPIO_AF_UART4;
		}
		else if(UART_GPIO_HARD.USARTx == UART5)
		{
			GPIO_AF = GPIO_AF_UART5;
		}
		else if(UART_GPIO_HARD.USARTx == USART6)
		{
			GPIO_AF = GPIO_AF_USART6;
		}
		else
			continue;
		
		GPIO_PinAFConfig(UART_GPIO_HARD.GPIOx[i], GPIO_PinSource, GPIO_AF);
	}

	
	USART_Init(UART_GPIO_HARD.USARTx,&UART_GPIO_HARD.USART_InitStruct);

	//USART_ITConfig(UART_GPIO_HARD.USARTx, USART_IT_RXNE, ENABLE);
	USART_Cmd(UART_GPIO_HARD.USARTx, ENABLE);
}


/***********************************************************
**name:	BSP_UART_Send
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void BSP_UART_Send(USART_TypeDef* USARTx, INT8U *sendbuff,	INT32U len)
{
	while(len--)
	{
		USART_SendData(USARTx,*sendbuff++);
		while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
	}
}

INT16U BSP_UART_RCV(USART_TypeDef * USARTx)
{
	//while (USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET);
	return USART_ReceiveData(USARTx);
}

INT16U BSP_UART_TST(USART_TypeDef * USARTx)
{
	if (USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET)
	{
		return 0;
	}

	return 1;
}



