
#ifdef LWIP_DEF
	#define LWIP_GLOBE
#else
	#define LWIP_GLOBE		extern
#endif



 
/*Static IP ADDRESS*/
#define DEFAULT_IP_ADDR0   198
#define DEFAULT_IP_ADDR1   1
#define DEFAULT_IP_ADDR2   3
#define DEFAULT_IP_ADDR3   145
   
/*NETMASK*/
#define DEFAULT_NETMASK_ADDR0   255
#define DEFAULT_NETMASK_ADDR1   255
#define DEFAULT_NETMASK_ADDR2   255
#define DEFAULT_NETMASK_ADDR3   0

/*Gateway Address*/
#define DEFAULT_GW_ADDR0   198
#define DEFAULT_GW_ADDR1   1
#define DEFAULT_GW_ADDR2   3
#define DEFAULT_GW_ADDR3   1  

LWIP_GLOBE void LWIP_NetworkConfigChange(void);


