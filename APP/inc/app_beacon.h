

#ifdef APP_BEACON_DEF
	#define APP_BEACON_GLOBE
#else
	#define APP_BEACON_GLOBE		extern
#endif

#include <hw_beacon.h>

#define 	STATRT_BEEP_MIN_AGC_MV				6000		// AGC  6000mv обоч
#define 	STOP_BEEP_MAX_DVALUE_AGC_MV		100			// AGC  	100mv 
#define 	STOP_BEEP_MAX_DVALUE_AGC_DB		2			// 2 db
#define	STOP_BEEP_TIMEOUT_S					300			//  300 S



APP_BEACON_GLOBE void APP_BEACON_Init(void);

APP_BEACON_GLOBE int APP_BEACON_SetFreq(INT32U freq);

APP_BEACON_GLOBE int APP_BEACON_QueBeaconCheck(void);

APP_BEACON_GLOBE int APP_BEACON_SetAGCOutputRange(INT8U agc_output_flag);

APP_BEACON_GLOBE int APP_BEACON_SetAGCOutputRange(INT8U agc_output_flag);



