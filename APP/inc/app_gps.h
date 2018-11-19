
#include <hw_pducomm.h>

#ifdef 	APP_GPS_DEF
	#define	APP_GPS_GLOBE
#else
	#define	APP_GPS_GLOBE	extern
#endif



APP_GPS_GLOBE int APP_GPS_DataGet(void);



