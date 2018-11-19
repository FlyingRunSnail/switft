

#ifdef BSP_ETH_DEF
	#define BSP_ETH_GLOBE
#else
	#define BSP_ETH_GLOBE		extern
#endif

#include "stm32f2xx.h"
#include  "bsp.h"
#include  "bsp_os.h"
#include "stm32f2x7_eth.h"
#include "core_cm3.h"


//#define		MII_MODE
#define		RMII_MODE

#define DP83848_PHY_ADDRESS       0x01 /* Relative to STM322xG-EVAL Board */


/* MAC ADDRESS*/
#define DEFAULT_MAC_ADDR0   02
#define DEFAULT_MAC_ADDR1   00
#define DEFAULT_MAC_ADDR2   00
#define DEFAULT_MAC_ADDR3   00
#define DEFAULT_MAC_ADDR4   00
#define DEFAULT_MAC_ADDR5   00



#define		ETH_MCO_PIN_PORT			GPIOA
#define		ETH_MCO_PIN				GPIO_Pin_8

/* Specific defines for EXTI line, used to manage Ethernet link status */
#define ETH_LINK_EXTI_LINE             EXTI_Line12
#define ETH_LINK_EXTI_PORT_SOURCE      EXTI_PortSourceGPIOG
#define ETH_LINK_EXTI_PIN_SOURCE       EXTI_PinSource12
#define ETH_LINK_EXTI_IRQn             EXTI15_10_IRQn 
/* PB14 */
#define ETH_LINK_PIN                   GPIO_Pin_12
#define ETH_LINK_GPIO_PORT             GPIOG
#define ETH_LINK_GPIO_CLK              RCC_AHB1Periph_GPIOG
/* PHY registers */
#define PHY_MICR                  0x11 /* MII Interrupt Control Register */
#define PHY_MICR_INT_EN           ((uint16_t)0x0002) /* PHY Enable interrupts */
#define PHY_MICR_INT_OE           ((uint16_t)0x0001) /* PHY Enable output interrupt events */
#define PHY_MISR                  0x12 /* MII Interrupt Status and Misc. Control Register */
#define PHY_MISR_LINK_INT_EN      ((uint16_t)0x0020) /* Enable Interrupt on change of link status */
#define PHY_LINK_STATUS           ((uint16_t)0x2000) /* PHY link status interrupt mask */


#define PHY_STS				0x10
#define PHY_LINK_STATUS_LINK           ((uint16_t)0x0001) /* PHY link status   */


BSP_ETH_GLOBE INT8U * BSP_ETH_GetCurrentTxBuffer(void);

BSP_ETH_GLOBE INT32U BSP_ETH_IsRxPktValid(void);

BSP_ETH_GLOBE void  BSP_InitEth(void);

BSP_ETH_GLOBE void ETH_BSP_Config(void);

BSP_ETH_GLOBE void ETH_GPIO_Config(void);

BSP_ETH_GLOBE void ETH_MACDMA_Config(void);

BSP_ETH_GLOBE void ETH_MACaddr_Config(INT8U *mac_addr);

BSP_ETH_GLOBE void ETH_IRQHandler(void);

BSP_ETH_GLOBE void BSP_ETH_ReleaseDescriptor(FrameTypeDef * pframe);

BSP_ETH_GLOBE INT8U BSP_ETH_GetDescriptorStatus(FrameTypeDef * pframe);



