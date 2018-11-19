

#include <ic_ds1302.h>
#include <devdef.h>

#ifdef 	APP_DATETIME_DEF
	#define	APP_DATETIME
#else
	#define	APP_DATETIME	extern
#endif

APP_DATETIME int DATETIME_SavetoRtcIC(strSysDateTime *SysDateTime);

APP_DATETIME void DATETIME_ReadFromRtcIC(strSysDateTime *SysDateTime);

APP_DATETIME int DATETIME_RefreshBySec(strSysDateTime *SysDateTime);

