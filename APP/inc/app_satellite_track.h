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
**describe: 		����׷��������
**input:	    		wokemode ׷��ģʽ���μ�TRACKMODE ö�ٵĶ���
**output:		none
**return: 		ִ�н��
**autor:  		tao
**date:
************************************************************/
//APP_SAT_TRACK_GLOBE int APP_Satellite_Tracking(TRACKMODE workmode);

/***********************************************************
**name:			APP_Sat_GradientTrack
**describe: 		����׷��������
**input:	    pc  ������ɺ�Ĺ���ֵ,��ΪĿ��ֵ
**output:		none
**return: 		ִ�н��
**autor:  		tao
**date:
************************************************************/
APP_SAT_TRACK_GLOBE int APP_Sat_GradientTrack();

APP_SAT_TRACK_GLOBE int logData(FP32 az, FP32 el, FP32 pl, FP32 agc, INT16U pw);

/***********************************************************
**name:			APP_CurrDegree_Get_FP
**describe: 		��ȡָ�����̵�ǰ�Ƕȣ����������ݣ�
**input:	    num:����id
**				degree:���ս����ָ��
**output:		degree:���̵�ǰ�Ƕ�
**return: 		ִ�н��
**autor:  		tao
**date:
************************************************************/
APP_SAT_TRACK_GLOBE int APP_CurrDegree_Get_FP(INT8U num, FP32 *degree);


/***********************************************************
**name:			APP_Satellite_Drift_Check
**describe: 		����Ư�Ƽ��
**input:	    		p0 ��׼ʱ�Ĺ���ֵ��segma ƫ�Χ
**output:		none
**return: 		ִ�н��
**autor:  		tao
**date:
************************************************************/
APP_SAT_TRACK_GLOBE int APP_Satellite_Drift_Check(FP32 *p0,FP32 p_obj, FP32 segma);

/***********************************************************
**name:			APP_Power_AG_Get
**describe: 		��ȡ����ƽ��ֵ
**input:	    		p ��ȡ���ʵ�ָ��
**output:		p ��ȡ�Ĺ���ƽ��ֵ
**return: 		ִ�н��
**autor:  		tao
**date:
************************************************************/
APP_SAT_TRACK_GLOBE int APP_Power_AG_Get(FP32 *p);

APP_SAT_TRACK_GLOBE int APP_Power_Get(FP32 *p);

/***********************************************************
**name:			APP_Satellite_Cap_Scan
**describe: 		ʮ��ɨ��׷��
**input:	    		Azc ��ʼ��λ��ELc ��ʼ������pc ��ʼ����ֵ
**output:		Azc ���Ǻ�ķ�λ��ELc ���Ǻ�ĸ�����pc ���Ǻ�Ĺ���ֵ
**return: 		ִ�н��
**autor:  		tao
**date:
************************************************************/
APP_SAT_TRACK_GLOBE int APP_Satellite_Cap_Scan(FP32 *Azc, FP32 *ELc, FP32 *pc);

/***********************************************************
**name:			APP_Preliminary_Aim
**describe: 		������������
**input:	    none
**output:		none
**return: 		ִ�н��
**autor:  		tao
**date:
************************************************************/
APP_SAT_TRACK_GLOBE int APP_Preliminary_Aim();

/***********************************************************
**name:			APP_Gradient_Track
**describe: 		�ݶ�׷��������
**input:	    none
**output:		none
**return: 		ִ�н��
**autor:  		tao
**date:
************************************************************/
APP_SAT_TRACK_GLOBE int APP_Gradient_Track(FP32 *Azc, FP32 *ELc, FP32 *pc);



/***********************************************************
**name:			APP_Satellite_Track_Start
**describe: 		�����Զ����Ƴ���
**input:		none
**output:		none
**return: 		none
**autor:  		tao
**date:
************************************************************/
APP_SAT_TRACK_GLOBE int APP_Satellite_Track_Start();


APP_SAT_TRACK_GLOBE int APP_Triangle_Track(FP32 *Azc, FP32 *ELc, FP32 *pc);
#endif
