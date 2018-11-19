

#ifdef APP_OLEDCOMM_DEF
	#define APP_OLEDCOMM_GLOBE
#else
	#define APP_OLEDCOMM_GLOBE		extern
#endif

#include <devdef.h>
#include <hw_oledcomm.h>

APP_OLEDCOMM_GLOBE strPCCOMMPro 	OLEDCOMMRx,OLEDCOMMTx;


APP_OLEDCOMM_GLOBE void APP_OLEDCOMM_Init(void);

APP_OLEDCOMM_GLOBE int APP_OLEDCOMM_RxBuffCheck(INT8U *buff, INT32U *len);

APP_OLEDCOMM_GLOBE int APP_OLEDCOMM_RxBuffRecv(void);

APP_OLEDCOMM_GLOBE void APP_OLEDCOMM_TxBuffSend(strPCCOMMPro txbuff);

APP_OLEDCOMM_GLOBE int APP_OLEDCOMM_CmdDeal(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_OLEDCOMM_GLOBE int APP_OLEDCOMM_Deal(void);

APP_OLEDCOMM_GLOBE void APP_OLEDCOMM_SetResult_Send(INT16U cmd_id,int status);



///////////////////////////////////////////////////////////////////////////////////////////////////////////////
APP_OLEDCOMM_GLOBE int APP_OLEDCOMM_SatParamSet(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_OLEDCOMM_GLOBE int APP_OLEDCOMM_AntennaParamSet(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_OLEDCOMM_GLOBE int APP_OLEDCOMM_AntennaParamAdjustSet(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_OLEDCOMM_GLOBE int APP_OLEDCOMM_LocalParamSet(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_OLEDCOMM_GLOBE int APP_OLEDCOMM_LocalParamSrcSelSet(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_OLEDCOMM_GLOBE int APP_OLEDCOMM_SysDateTimeSet(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_OLEDCOMM_GLOBE int APP_OLEDCOMM_SysDateTimeSrcSelSet(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_OLEDCOMM_GLOBE int APP_OLEDCOMM_DelSatSet(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_OLEDCOMM_GLOBE int APP_OLEDCOMM_BuzzTest(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_OLEDCOMM_GLOBE int APP_OLEDCOMM_AimAtSatModeSel(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_OLEDCOMM_GLOBE int APP_OLEDCOMM_AntennaRecord(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_OLEDCOMM_GLOBE int APP_OLEDCOMM_AntennaSigleStepCtr(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_OLEDCOMM_GLOBE int APP_OLEDCOMM_AntennaTurnCtr(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_OLEDCOMM_GLOBE int APP_OLEDCOMM_SelSat(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

//APP_OLEDCOMM_GLOBE int APP_OLEDCOMM_MotorSoftLimit(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_OLEDCOMM_GLOBE int APP_OLEDCOMM_NetWorkSet(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_OLEDCOMM_GLOBE int APP_OLEDCOMM_PCAdminSet(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_OLEDCOMM_GLOBE int APP_OLEDCOMM_CurrSatParamQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_OLEDCOMM_GLOBE int APP_OLEDCOMM_SaveAllSatParamQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_OLEDCOMM_GLOBE int APP_OLEDCOMM_AntennaParamQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_OLEDCOMM_GLOBE int APP_OLEDCOMM_AntennaAdjustParamQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_OLEDCOMM_GLOBE int APP_OLEDCOMM_LocalParamQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_OLEDCOMM_GLOBE int APP_OLEDCOMM_DevSysDateTimeQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_OLEDCOMM_GLOBE int APP_OLEDCOMM_NetWorkQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_OLEDCOMM_GLOBE int APP_OLEDCOMM_AimAtSatDegreeQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_OLEDCOMM_GLOBE int APP_OLEDCOMM_AntennaRecordQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_OLEDCOMM_GLOBE int APP_OLEDCOMM_MotorSoftLimitQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_OLEDCOMM_GLOBE int APP_OLEDCOMM_DevStaQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_OLEDCOMM_GLOBE int APP_OLEDCOMM_DevAdminQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_OLEDCOMM_GLOBE int APP_OLEDCOMM_DevAlarmQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_OLEDCOMM_GLOBE int APP_OLEDCOMM_OptLogQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_OLEDCOMM_GLOBE int APP_OLEDCOMM_WarnLogQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_OLEDCOMM_GLOBE int APP_OLEDCOMM_VersionQue(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_OLEDCOMM_GLOBE int APP_OLEDCOMM_AntennaRecordGoto(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);


APP_OLEDCOMM_GLOBE void APP_OLEDCOMM_AskForPCAdmin(void);

APP_OLEDCOMM_GLOBE int APP_OLEDCOMM_AGCRecord(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

APP_OLEDCOMM_GLOBE int APP_OLEDCOMM_DelAntennaRecord(INT8U *rxbuff, INT8U *rxbufflen,INT8U *txbuff, INT8U *txbufflen);

