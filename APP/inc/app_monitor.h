

#include <bsp.h>

#ifdef 	APP_MONITOR_DEF
	#define	APP_MONITOR
#else
	#define	APP_MONITOR	extern
#endif

APP_MONITOR	void APP_MONITOR_LocallAntennaRealDegreeDataGet(void);

APP_MONITOR void APP_MONITOR_LocallAntennaAdjustDegreeDataGet(INT8U part_id,FP32 *degree_input,FP32 *degree_output);

APP_MONITOR 	void APP_MONITOR_AimDegree_Get(void);

APP_MONITOR	void	APP_MONITOR_Temperature_Get(void);

APP_MONITOR INT8U APP_MONITOR_ETH_LinkSta_Get(void);

APP_MONITOR void APP_MONITOR_ETH_ReLink_Deal(INT8U curr_eth_link);

APP_MONITOR void APP_MONITOR_PC_Admin_Timeout_Deal(INT32U period_ms);

APP_MONITOR void APP_MONITOR_AlarmStaCodeDeal(INT8U *AlarmCodebuff,INT32U *buffsize);

APP_MONITOR void APP_MONITOR_DevHWSta(INT32U period_ms);

APP_MONITOR int APP_ElecEncoder_DataAddDir_Adjust( INT8U antenna_part, INT8U antenna_dir);

APP_MONITOR void APP_MONITOR_DevMotorMonitor(INT32U period_ms);

