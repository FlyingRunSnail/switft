

#ifdef APP_TOOL_DEF
	#define APP_TOOL_GLOBE
#else
	#define APP_TOOL_GLOBE		extern
#endif

#include <bsp.h>
#include <devdef.h>

#define IP4_ADDR(ipaddr, a,b,c,d) \
        (ipaddr)->addr = htonl(((INT32U)((a) & 0xff) << 24) | \
                               ((INT32U)((b) & 0xff) << 16) | \
                               ((INT32U)((c) & 0xff) << 8) | \
                                (INT32U)((d) & 0xff))


APP_TOOL_GLOBE INT8U TOOL_CheckSum8BitGet(INT8U *buff, INT32U len);

APP_TOOL_GLOBE INT32U TOOL_SysTickGet(void);

APP_TOOL_GLOBE void TOOL_INT32Data2Bytebuff(INT32U data, INT8U *buff, INT8U save_mode);

APP_TOOL_GLOBE void TOOL_INT16Data2Bytebuff(INT16U data, INT8U *buff, INT8U save_mode);

APP_TOOL_GLOBE INT32U TOOL_Bytebuff2INT32Data(INT8U *buff,INT8U save_mode);

APP_TOOL_GLOBE INT16U TOOL_Bytebuff2INT16Data(INT8U *buff,INT8U save_mode);

APP_TOOL_GLOBE void TOOL_GetCPI_ID(INT32U *IDbuff)  ;

