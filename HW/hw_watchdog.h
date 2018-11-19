
#ifdef HW_WTD_DEF
	#define HW_WTD_GLOBE
#else
	#define HW_WTD_GLOBE	extern
#endif



#include <bsp.h>


HW_WTD_GLOBE void HW_WTD_Init(void);

HW_WTD_GLOBE void HW_WTD_Feed(void);

