

#ifdef  PROTOCOL_BSP_DEF
	#define 		PROTOCOL_BSP_GLOBE	
#else
	#define 		PROTOCOL_BSP_GLOBE	extern
#endif


#include <bsp.h>
#include <opt.h>





PROTOCOL_BSP_GLOBE	void PROTOCOL_RevFframe_Get(void*p,INT16U len);

PROTOCOL_BSP_GLOBE	int PROTOCOL_RevFrame_Check(INT8U *buff, INT16U len);

PROTOCOL_BSP_GLOBE 	void PROTOCOL_TxFrame_Get(strPCCOMMPro framebuff);


