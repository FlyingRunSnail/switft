#ifndef _APP_SATELLITE_TRACE_H_
#define _APP_SATELLITE_TRACE_H_

#include <includes.h>

#ifdef 	APP_SATELLITE_TRACKE_DEF
	#define	APP_SAT_TRACK_GLOBE
#else
	#define	APP_SAT_TRACK_GLOBE	extern
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef UNDERPOWER 
	#define UNDERPOWER (-1)
#endif
#ifndef FULLPOWER 
	#define FULLPOWER (1)
#endif
#ifndef APP_SAT_TRACK_ERR
	#define APP_SAT_TRACK_ERR (0)
#endif
//#ifndef TRACKMODE
//	typedef enum _TRACKMODE_{
//		SCAN = 0
//	}TRACKMODE;
//#endif

#ifndef MOTORDIR
	typedef enum _MOTODIR_
	{
		Drec_anticlockwise_down = 0x01,
		Drec_clockwise_up = 0x02
	}MOTORDIR;
#endif

/***********************************************************
**name:			APP_Satellite_Tracking
**describe: 		卫星追星主程序
**input:	    		wokemode 追星模式，参见TRACKMODE 枚举的定义
**output:		none
**return: 		执行结果
**autor:  		tao
**date:
************************************************************/
//APP_SAT_TRACK_GLOBE int APP_Satellite_Tracking(TRACKMODE workmode);

/***********************************************************
**name:			APP_Sat_GradientTrack
**describe: 		卫星追星主程序
**input:	    pc  对星完成后的功率值,作为目标值
**output:		none
**return: 		执行结果
**autor:  		tao
**date:
************************************************************/
APP_SAT_TRACK_GLOBE int APP_Sat_GradientTrack();

APP_SAT_TRACK_GLOBE int logData(FP32 az, FP32 el, FP32 pl, FP32 agc, INT16U pw);

/***********************************************************
**name:			APP_CurrDegree_Get_FP
**describe: 		获取指定码盘当前角度（浮点型数据）
**input:	    num:码盘id
**				degree:接收结果的指针
**output:		degree:码盘当前角度
**return: 		执行结果
**autor:  		tao
**date:
************************************************************/
APP_SAT_TRACK_GLOBE int APP_CurrDegree_Get_FP(INT8U num, FP32 *degree);


/***********************************************************
**name:			APP_Satellite_Drift_Check
**describe: 		卫星漂移检测
**input:	    		p0 对准时的功率值，segma 偏差范围
**output:		none
**return: 		执行结果
**autor:  		tao
**date:
************************************************************/
APP_SAT_TRACK_GLOBE int APP_Satellite_Drift_Check(FP32 *p0,FP32 p_obj, FP32 segma);

/***********************************************************
**name:			APP_Power_AG_Get
**describe: 		获取功率平均值
**input:	    		p 获取功率的指针
**output:		p 获取的功率平均值
**return: 		执行结果
**autor:  		tao
**date:
************************************************************/
APP_SAT_TRACK_GLOBE int APP_Power_AG_Get(FP32 *p);

APP_SAT_TRACK_GLOBE int APP_Power_Get(FP32 *p);

/***********************************************************
**name:			APP_Satellite_Cap_Scan
**describe: 		十字扫描追星
**input:	    		Azc 初始方位，ELc 初始俯仰，pc 初始功率值
**output:		Azc 对星后的方位，ELc 对星后的俯仰，pc 对星后的功率值
**return: 		执行结果
**autor:  		tao
**date:
************************************************************/
APP_SAT_TRACK_GLOBE int APP_Satellite_Cap_Scan(FP32 *Azc, FP32 *ELc, FP32 *pc);

/***********************************************************
**name:			APP_Preliminary_Aim
**describe: 		初对星主程序
**input:	    none
**output:		none
**return: 		执行结果
**autor:  		tao
**date:
************************************************************/
APP_SAT_TRACK_GLOBE int APP_Preliminary_Aim();

/***********************************************************
**name:			APP_Gradient_Track
**describe: 		梯度追星主程序
**input:	    none
**output:		none
**return: 		执行结果
**autor:  		tao
**date:
************************************************************/
APP_SAT_TRACK_GLOBE int APP_Gradient_Track(FP32 *Azc, FP32 *ELc, FP32 *pc);



/***********************************************************
**name:			APP_Satellite_Track_Start
**describe: 		启动自动控制程序
**input:		none
**output:		none
**return: 		none
**autor:  		tao
**date:
************************************************************/
APP_SAT_TRACK_GLOBE int APP_Satellite_Track_Start();


APP_SAT_TRACK_GLOBE int APP_Triangle_Track(FP32 *Azc, FP32 *ELc, FP32 *pc);
#endif
