

#ifdef APP_PCCOMM_DEF
	#define APP_PCCOMM_GLOBE
#else
	#define APP_PCCOMM_GLOBE		extern
#endif

#include <devdef.h>

APP_PCCOMM_GLOBE strPCCOMMPro 	PCCOMMRx,PCCOMMTx;


APP_PCCOMM_GLOBE void APP_PCCOMM_Init(void);

APP_PCCOMM_GLOBE int APP_PCCOMM_RxBuffCheck(INT8U *buff, INT16U *len);

APP_PCCOMM_GLOBE int APP_PCCOMM_RxBuffRecv(void*p,INT16U len);

APP_PCCOMM_GLOBE void APP_PCCOMM_TxBuffSend(strPCCOMMPro txbuff);

APP_PCCOMM_GLOBE int APP_PCCOMM_CmdDeal(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_PCCOMM_GLOBE int APP_PCCOMM_Deal(void*p,INT16U len);

APP_PCCOMM_GLOBE void APP_PCCOMM_SetResult_Send(INT16U cmd_id,int status);



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
APP_PCCOMM_GLOBE int APP_PCCOMM_SatParamSet(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_PCCOMM_GLOBE int APP_PCCOMM_AntennaParamSet(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_PCCOMM_GLOBE int APP_PCCOMM_AntennaParamAdjustSet(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_PCCOMM_GLOBE int APP_PCCOMM_LocalParamSet(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_PCCOMM_GLOBE int APP_PCCOMM_LocalParamSrcSelSet(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_PCCOMM_GLOBE int APP_PCCOMM_SysDateTimeSet(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_PCCOMM_GLOBE int APP_PCCOMM_SysDateTimeSrcSelSet(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_PCCOMM_GLOBE int APP_PCCOMM_DelSatSet(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_PCCOMM_GLOBE int APP_PCCOMM_AntennaRecordGoto(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_PCCOMM_GLOBE int APP_PCCOMM_DelAntennaRecord(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_PCCOMM_GLOBE int APP_PCCOMM_BuzzTest(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_PCCOMM_GLOBE int APP_PCCOMM_AimAtSatModeSel(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_PCCOMM_GLOBE int APP_PCCOMM_AntennaRecord(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_PCCOMM_GLOBE int APP_PCCOMM_AntennaSigleStepCtr(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_PCCOMM_GLOBE int APP_PCCOMM_AntennaTurnCtr(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_PCCOMM_GLOBE int APP_PCCOMM_SelSat(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_PCCOMM_GLOBE int APP_PCCOMM_AimAtSatParamSet(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

//APP_PCCOMM_GLOBE int APP_PCCOMM_MotorSoftLimit(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_PCCOMM_GLOBE int APP_PCCOMM_NetWorkSet(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_PCCOMM_GLOBE int APP_PCCOMM_PCAdminSet(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_PCCOMM_GLOBE int APP_PCCOMM_CurrSatParamQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_PCCOMM_GLOBE int APP_PCCOMM_SaveAllSatParamQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_PCCOMM_GLOBE int APP_PCCOMM_AntennaParamQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_PCCOMM_GLOBE int APP_PCCOMM_AntennaAdjustParamQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_PCCOMM_GLOBE int APP_PCCOMM_LocalParamQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_PCCOMM_GLOBE int APP_PCCOMM_DevSysDateTimeQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_PCCOMM_GLOBE int APP_PCCOMM_NetWorkQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_PCCOMM_GLOBE int APP_PCCOMM_AimAtSatDegreeQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_PCCOMM_GLOBE int APP_PCCOMM_AntennaRecordQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_PCCOMM_GLOBE int APP_PCCOMM_MotorSoftLimitQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_PCCOMM_GLOBE int APP_PCCOMM_DevStaQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_PCCOMM_GLOBE int APP_PCCOMM_DevAdminQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_PCCOMM_GLOBE int APP_PCCOMM_DevAlarmQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_PCCOMM_GLOBE int APP_PCCOMM_OptLogQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_PCCOMM_GLOBE int APP_PCCOMM_WarnLogQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_PCCOMM_GLOBE int APP_PCCOMM_VersionQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_PCCOMM_GLOBE int APP_PCCOMM_AimAtSatParamQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_PCCOMM_GLOBE int APP_PCCOMM_AntennaTurnDirAdjustBegin(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_PCCOMM_GLOBE int APP_PCCOMM_AntennaTurnDirAdjustFinish(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_PCCOMM_GLOBE int APP_PCCOMM_AntennaHargWareLimitAdjust(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_PCCOMM_GLOBE int APP_PCCOMM_AGCRecord(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_PCCOMM_GLOBE int APP_PCCOMM_FactoryReset(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);
