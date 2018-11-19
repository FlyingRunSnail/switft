

#define LWIP_DEF

#include "lwip.h"
#include "tcpip.h"
#include "ethernetif.h"
#include "bsp_eth.h"
// #include "netconf.h"
#include "devdef.h"
#include "devdef.h"

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
struct netif xnetif; /* network interface structure */

//extern void  (*BSP_ETH_IRQHandler)(void);   


void LwIP_SendSem(void)
{
    // sys_sem_signal(&lwip_eth_sem);
    while(BSP_ETH_IsRxPktValid()) 
        ethernetif_input(&xnetif);
}

/***********************************************************
**name: LwIP_Init
**describe:
**input:            
**output:   none
**return:
**autor:  andiman
**date:
************************************************************/
void LWIP_Init(void)
{
    struct ip_addr ipaddr;
    struct ip_addr netmask;
    struct ip_addr gw;

    ETH_BSP_Config();

//  BSP_InitEth();

//  if( APP_MONITOR_ETH_LinkSta_Get() == 0 )
//      return;

    //Eth_Link_PHYITConfig(DP83848_PHY_ADDRESS);
    //Eth_Link_EXTIConfig();

     tcpip_init(NULL, NULL);

//  APP_PARAM_NetWork_Save(&ACUDevInfo.DeviceInfo.NetWork);
    //if( APP_PARAM_NetWork_Read(&ACUDevInfo.DeviceInfo.NetWork) == FAIL)
    if(1)
    {
        ipaddr.addr = 0;
        netmask.addr = 0;
        gw.addr = 0;

        IP4_ADDR(&ipaddr, DEFAULT_IP_ADDR0, DEFAULT_IP_ADDR1, DEFAULT_IP_ADDR2, DEFAULT_IP_ADDR3);
        IP4_ADDR(&netmask, DEFAULT_NETMASK_ADDR0, DEFAULT_NETMASK_ADDR1 , DEFAULT_NETMASK_ADDR2, DEFAULT_NETMASK_ADDR3);
        IP4_ADDR(&gw, DEFAULT_GW_ADDR0, DEFAULT_GW_ADDR1, DEFAULT_GW_ADDR2, DEFAULT_GW_ADDR3);
    }
    else
    {

        if( ((ACUDevInfo.DeviceInfo.NetWork.ip_addr == 0x00000000) || (ACUDevInfo.DeviceInfo.NetWork.ip_addr== 0xFFFFFFFF)) ||
            (( ACUDevInfo.DeviceInfo.NetWork.mask & 0xFF000000) == 0xFF))
    
        {
            ipaddr.addr = 0;
            netmask.addr = 0;
            gw.addr = 0;
            
            IP4_ADDR(&ipaddr, DEFAULT_IP_ADDR0, DEFAULT_IP_ADDR1, DEFAULT_IP_ADDR2, DEFAULT_IP_ADDR3);
            IP4_ADDR(&netmask, DEFAULT_NETMASK_ADDR0, DEFAULT_NETMASK_ADDR1 , DEFAULT_NETMASK_ADDR2, DEFAULT_NETMASK_ADDR3);
            IP4_ADDR(&gw, DEFAULT_GW_ADDR0, DEFAULT_GW_ADDR1, DEFAULT_GW_ADDR2, DEFAULT_GW_ADDR3);

            ACUDevInfo.DeviceInfo.NetWork.ip_addr = ipaddr.addr;
            ACUDevInfo.DeviceInfo.NetWork.mask = netmask.addr;
            ACUDevInfo.DeviceInfo.NetWork.gate = gw.addr;
        }
        else
        {
            IP4_ADDR(&ipaddr,       (ACUDevInfo.DeviceInfo.NetWork.ip_addr & 0x000000FF),       (ACUDevInfo.DeviceInfo.NetWork.ip_addr & 0x0000FF00)>>8,
                                    (ACUDevInfo.DeviceInfo.NetWork.ip_addr & 0x00FF0000)>>16,   (ACUDevInfo.DeviceInfo.NetWork.ip_addr & 0xFF000000)>>24);
            IP4_ADDR(&netmask,      (ACUDevInfo.DeviceInfo.NetWork.mask & 0x000000FF),          (ACUDevInfo.DeviceInfo.NetWork.mask & 0x0000FF00)>>8,
                                    (ACUDevInfo.DeviceInfo.NetWork.mask & 0x00FF0000)>>16,      (ACUDevInfo.DeviceInfo.NetWork.mask & 0xFF000000)>>24 );
            IP4_ADDR(&gw,           (ACUDevInfo.DeviceInfo.NetWork.gate & 0x000000FF),          (ACUDevInfo.DeviceInfo.NetWork.gate & 0x0000FF00)>>8,
                                    (ACUDevInfo.DeviceInfo.NetWork.gate & 0x00FF0000)>>16,      (ACUDevInfo.DeviceInfo.NetWork.gate & 0xFF000000)>>24 );
        }
    }

    netif_add(&xnetif, &ipaddr, &netmask, &gw, NULL, &ethernetif_init, &tcpip_input);
    //Registers the default network interface. 
    netif_set_default(&xnetif);

    //BSP_ETH_IRQHandler = LwIP_SendSem;

    

    //When the netif is fully configured this function must be called
    netif_set_up(&xnetif); 

}


/***********************************************************
**name: LWIP_NetworkConfigChange
**describe:   ip地址等修改
**input:            
**output:   none
**return:
**autor:  andiman
**date:
************************************************************/
void LWIP_NetworkConfigChange(void)
{
    
    struct ip_addr ipaddr;
    struct ip_addr netmask;
    struct ip_addr gw;
    INT8U IPbuff[4],Maskbuff[4],Gatebuff[4];

    IP4_ADDR(&ipaddr,       (ACUDevInfo.DeviceInfo.NetWork.ip_addr & 0x000000FF),       (ACUDevInfo.DeviceInfo.NetWork.ip_addr & 0x0000FF00)>>8,
                            (ACUDevInfo.DeviceInfo.NetWork.ip_addr & 0x00FF0000)>>16,   (ACUDevInfo.DeviceInfo.NetWork.ip_addr & 0xFF000000)>>24);
    IP4_ADDR(&netmask,      (ACUDevInfo.DeviceInfo.NetWork.mask & 0x000000FF),          (ACUDevInfo.DeviceInfo.NetWork.mask & 0x0000FF00)>>8,
                            (ACUDevInfo.DeviceInfo.NetWork.mask & 0x00FF0000)>>16,      (ACUDevInfo.DeviceInfo.NetWork.mask & 0xFF000000)>>24 );
    IP4_ADDR(&gw,           (ACUDevInfo.DeviceInfo.NetWork.gate & 0x000000FF),          (ACUDevInfo.DeviceInfo.NetWork.gate & 0x0000FF00)>>8,
                            (ACUDevInfo.DeviceInfo.NetWork.gate & 0x00FF0000)>>16,      (ACUDevInfo.DeviceInfo.NetWork.gate & 0xFF000000)>>24 );
    netif_set_addr(&xnetif, &ipaddr,&netmask,&gw);
    
}



/***********************************************************
**name: LWIP_MACConfigChange
**describe:  mac地址修改
**input:            
**output:   none
**return:
**autor:  andiman
**date:
************************************************************/
void LWIP_MACConfigChange(void)
{
    memcpy(xnetif.hwaddr,ACUDevInfo.DeviceInfo.NetWork.mac,6);
    ETH_MACaddr_Config(ACUDevInfo.DeviceInfo.NetWork.mac);
}


