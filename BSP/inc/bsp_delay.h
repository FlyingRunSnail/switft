

#ifdef BSP_DELAY_DEF
	#define BSP_DELAY_GLOBE
#else
	#define BSP_DELAY_GLOBE		extern
#endif

#include <os.h>						//ͷ�ļ�·������ʵ�ʽ����޸�


BSP_DELAY_GLOBE void BSP_Delay_ms(INT16U ms);

BSP_DELAY_GLOBE void BSP_Delay_s(INT8U sec);

BSP_DELAY_GLOBE void BSP_Delay_us(INT32U us);


