
#ifdef MAIN_DEF
	#define MAIN_GLOBE
#else
	#define MAIN_GLOBE		extern
#endif

#include "app_cfg.h"
#include "bsp_os.h"





#define	TASK_START_PRIO       	6

#define	TASK_SHELL_PRIO		7	

#define	TASK_EthRelink_PRIO		9	


#define	TASK_Monitor_PRIO					23	
#define	TASK_Beacon_PRIO					24	
#define	TASK_Position_ElecCoder_PRIO		25	
#define	TASK_Cabrage_ElecCoder_PRIO		26	
#define	TASK_Polar_ElecCoder_PRIO			27	
#define	TASK_SatelliteTrack_PRIO		30	
#define	TASK_AntennaRecordGoto_PRIO		31	
#define	TASK_SatGradientTrack_PRIO		32	


#define  	TASK_EthRelink_STK_SIZE            					TASK_STK_SIZE_256
#define  	TASK_Position_ElecCoder_STK_SIZE            		TASK_STK_SIZE_256
#define  	TASK_Cabrage_ElecCoder_STK_SIZE            		TASK_STK_SIZE_256
#define  	TASK_Polar_ElecCoder_STK_SIZE            			TASK_STK_SIZE_256
#define  	TASK_Beacon_STK_SIZE            					TASK_STK_SIZE_256
#define  	TASK_Monitor_STK_SIZE            					TASK_STK_SIZE_1024
#define  	TASK_SatelliteTrack_STK_SIZE            				TASK_STK_SIZE_512
#define  	TASK_SatGradientTrack_STK_SIZE            				TASK_STK_SIZE_512

#define  	TASK_START_STK_SIZE            		TASK_STK_SIZE_1024 
//#define  	TASK_DevCfg_STK_SIZE            		TASK_STK_SIZE_512
#define  	TASK_SHELL_STK_SIZE            					(TASK_STK_SIZE_512 + TASK_STK_SIZE_256)

#define  	TASK_AntennaRecordGoto_STK_SIZE            			(TASK_STK_SIZE_512 + TASK_STK_SIZE_128)



MAIN_GLOBE OS_TCB	TaskEthRelinkTCB;
MAIN_GLOBE CPU_STK	TaskEthRelinkStk[TASK_EthRelink_STK_SIZE];

MAIN_GLOBE OS_TCB	TaskMonitorTCB;
MAIN_GLOBE CPU_STK	TaskMonitorStk[TASK_Monitor_STK_SIZE];

MAIN_GLOBE OS_TCB	TaskShellTCB;
MAIN_GLOBE CPU_STK	TaskShellStk[TASK_SHELL_STK_SIZE];

MAIN_GLOBE OS_TCB	TaskSatelliteTrackTCB;
MAIN_GLOBE CPU_STK	TaskSatelliteTrackStk[TASK_SatelliteTrack_STK_SIZE];

MAIN_GLOBE OS_TCB	TaskSatGradientTrackTCB;
MAIN_GLOBE CPU_STK	TaskSatGradientTrackStk[TASK_SatGradientTrack_STK_SIZE];

MAIN_GLOBE OS_TCB	TaskAntennaRecordGotoTCB;
MAIN_GLOBE CPU_STK	TaskAntennaRecordGotoStk[TASK_AntennaRecordGoto_STK_SIZE];



MAIN_GLOBE OS_TCB	TaskStartTCB;
MAIN_GLOBE CPU_STK	TaskStartStk[TASK_START_STK_SIZE];


MAIN_GLOBE OS_TCB	TaskPositionElecCoderTCB;
MAIN_GLOBE CPU_STK	TaskPositionElecCoderStk[TASK_Position_ElecCoder_STK_SIZE];


MAIN_GLOBE OS_TCB	TaskCabrageElecCoderTCB;
MAIN_GLOBE CPU_STK	TaskCabrageElecCoderStk[TASK_Cabrage_ElecCoder_STK_SIZE];


MAIN_GLOBE OS_TCB	TaskPolarElecCoderTCB;
MAIN_GLOBE CPU_STK	TaskPolarElecCoderStk[TASK_Polar_ElecCoder_STK_SIZE];

MAIN_GLOBE OS_TCB	TaskBeaconTCB;
MAIN_GLOBE CPU_STK	TaskBeaconStk[TASK_Beacon_STK_SIZE];


//MAIN_GLOBE OS_TCB	TaskDevCfgTCB;
//MAIN_GLOBE CPU_STK	TaskDevCfgStk[TASK_DevCfg_STK_SIZE];

MAIN_GLOBE void  TaskSatelliteTrackDel (void);

MAIN_GLOBE void  TaskSatelliteTrackCreate(void);

MAIN_GLOBE void  TaskAntennaRecordGotoDel (void);

MAIN_GLOBE void  TaskAntennaRecordGotoCreate(void);

MAIN_GLOBE void  TaskSatGradientTrackDel (void);

MAIN_GLOBE void  TaskSatGradientTrackCreate(void);
/*
*********************************************************************************************************
*                                      define
*********************************************************************************************************
*/



/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/




