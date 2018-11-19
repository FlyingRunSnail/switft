



#ifdef 	APP_ELEC_ENCODER_DEF
	#define	APP_ELEC_ENCODER_GLOBE
#else
	#define	APP_ELEC_ENCODER_GLOBE	extern
#endif


#include <hw_eleccoder.h>
#include <devdef.h>



#define APP_ELEC_ENCODER_POSITION			0x01				// 方位电子码盘
#define APP_ELEC_ENCODER_CABRAGE			0x02				// 俯仰电子码盘
#define APP_ELEC_ENCODER_POLARIZATION		0x03				// 极化电子码盘


#define		ELEC_ENCODER_UNLINK_NUM			3

APP_ELEC_ENCODER_GLOBE int APP_ElecEncoder_Init(void);

APP_ELEC_ENCODER_GLOBE INT32U APP_ElecEncoder_DegreeInt2Data(INT8U addr,INT32U degree);

APP_ELEC_ENCODER_GLOBE INT32U APP_ElecEncoder_Data2DegreeInt(INT8U addr,INT32U data);

APP_ELEC_ENCODER_GLOBE INT32U APP_ElecEncoder_DegreeFloat2Data(INT8U addr,FP32 degree);

//APP_ELEC_ENCODER_GLOBE FP32 APP_ElecEncoder_Data2DegreeFloat(INT8U addr,INT32U data);

APP_ELEC_ENCODER_GLOBE void APP_ElecEncoder_Data2DegreeFloat(INT8U addr,INT32U data ,FP32 *degree);

APP_ELEC_ENCODER_GLOBE int APP_ElecEncoder_Position_DataAddDir_Set(INT8U dir);

APP_ELEC_ENCODER_GLOBE int APP_ElecEncoder_Position_DataAddDir_Que(INT8U *dir);

APP_ELEC_ENCODER_GLOBE int APP_ElecEncoder_Cabrage_DataAddDir_Set(INT8U dir);

APP_ELEC_ENCODER_GLOBE int APP_ElecEncoder_Cabrage_DataAddDir_Que(INT8U *dir);

APP_ELEC_ENCODER_GLOBE int APP_ElecEncoder_Polarization_DataAddDir_Set(INT8U dir);

APP_ELEC_ENCODER_GLOBE int APP_ElecEncoder_Polarization_DataAddDir_Que(INT8U *dir);

APP_ELEC_ENCODER_GLOBE int APP_ElecEncoder_Position_MakeZero_Set(void);

APP_ELEC_ENCODER_GLOBE int APP_ElecEncoder_Cabrage_MakeZero_Set(void);

APP_ELEC_ENCODER_GLOBE int APP_ElecEncoder_Polarization_MakeZero_Set(void);

APP_ELEC_ENCODER_GLOBE int APP_ElecEncoder_Position_Addr_Que(INT8U *addr);

APP_ELEC_ENCODER_GLOBE int APP_ElecEncoder_Cabrage_Addr_Que(INT8U *addr);

APP_ELEC_ENCODER_GLOBE int APP_ElecEncoder_Polarization_Addr_Que(INT8U *addr);

APP_ELEC_ENCODER_GLOBE int APP_ElecEncoder_Position_Addr_Set(INT8U addr);

APP_ELEC_ENCODER_GLOBE int APP_ElecEncoder_Cabrage_Addr_Set(INT8U addr);

APP_ELEC_ENCODER_GLOBE int APP_ElecEncoder_Polarization_Addr_Set(INT8U addr);

APP_ELEC_ENCODER_GLOBE int APP_ElecEncoder_Position_CurrData_Get(INT32U *data);

APP_ELEC_ENCODER_GLOBE int APP_ElecEncoder_Cabrage_CurrData_Get(INT32U *data);

APP_ELEC_ENCODER_GLOBE int APP_ElecEncoder_Polarization_CurrData_Get(INT32U *data);


APP_ELEC_ENCODER_GLOBE int APP_ElecEncoder_DataAddDir_Set(INT8U addr,INT8U dir);

APP_ELEC_ENCODER_GLOBE int APP_ElecEncoder_CurrData_Get(INT8U addr,INT32U *data);

APP_ELEC_ENCODER_GLOBE int APP_ElecEncoder_DataAddDir_Get(INT8U addr,INT8U *dir);

