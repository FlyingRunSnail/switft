#define APP_SATELLITE_TRACKE_DEF

#include <app_satellite_track.h>
//#include <app_flash.h>
#include <bsp_delay.h>
#include <app_motor.h>
#include <devdef.h>
#include <app_log.h>
#include <math.h>

#define LOG_PURS_START 0xB1B1B1
#define LOG_PURS_END 0xB1B3B3
#define LOG_PURS_WORK 0xB1B2B2

#define ACCEPTENCE_DOMAIN 0.01f // 电机运动的基本接收范围，单位 度
//#define ACCEPTENCE_DOMAIN_P 1.0f // AGC 接受范围 1V
#define DRIFTCHK_D_P 1.25f // AGC 启动门限差值 1.25V

#ifdef _SIMULATION_
	#define MAX_POWER 10
	#define BAND_KU 0
	#define BAND_C 1
	int scan_rs[][3] = { { 2, 5, 10 }, { 5, 8, 12 } };
	INT8U waveBand = BAND_KU;
#endif

#define PI 3.1415926f
#define SIN(v) sin(v/180.0f*PI)
#define COS(v) cos(v/180.0f*PI)
#define TAN(v) tan(v/180.0f*PI)
#define ATAN(v) atan(v)/PI*180.0f
#define DEF_GRD_STEP_AZ 0.03f;
#define DEF_GRD_STEP_EL 0.02f;

#ifndef _POSITION
	#define ANG_POSITION			0x01				// 方位电子码盘
#endif
#ifndef _CABRAGE
	#define ANG_CABRAGE			0x02				// 俯仰电子码盘
#endif
#ifndef _POLARIZATION
	#define ANG_POLARIZATION		0x03				// 极化电子码盘
#endif

INT8U rainFlag = 0;

int APP_GE(double a, double b, double segma)
{
//#ifdef _SIMULATION_
	if ((a - b) >= segma)
		return TRUE;
	else
		return FALSE;
//#else
//	if ((a - b) <= segma)
//		return TRUE;
//	else
//		return FALSE;
//#endif
}

int APP_LE(double a, double b, double segma)
{
//#ifdef _SIMULATION_
	if ((a - b) <= segma)
		return TRUE;
	else
		return FALSE;
//#else
//	if ((a - b) >= segma)
//		return TRUE;
//	else
//		return FALSE;
//#endif
}

int APP_NB(double a, double b, double segma)
{
	if (fabs(a - b) <= segma)
		return TRUE;
	else
		return FALSE;
}

int APP_BT(double a, double b, double segma0,double segma1)
{
	double d = fabs(a - b);
	if (d >= segma0&&d <= segma1)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

#if 0
/***********************************************************
**name:			logData
**describe:		记录日志
**autor:		tao
************************************************************/
int logData(FP32 az, FP32 el, FP32 pl, FP32 agc, INT32S pw)
{
	INT32U datas[4];
	datas[0] = az;
	datas[1] = el;
	datas[2] = pl;
	datas[3] = (INT32U)(((INT32U)agc << 16) | (INT32U)(-pw)); // ??????? agc要改
	return APP_Flash_TestSaveData(datas, 4);
}
#endif

int getScanRadius(FP32 *radius,FP32 agc0,FP32 agc_opt)
{
	if (APP_BT(agc0, agc_opt, 2.5, 4.0)==TRUE)
	{
#ifdef _SIMULATION_
		*radius = (FP32)scan_rs[waveBand][0];
#else
		*radius = ACUDevInfo.ACUParam.SeekSatParam.ScanRange_param[0];
#endif
	}
	else if (APP_NB(agc0, agc_opt, 4.0)==FALSE)
	{
#ifdef _SIMULATION_
		*radius = (FP32)scan_rs[waveBand][1];
#else
		*radius = ACUDevInfo.ACUParam.SeekSatParam.ScanRange_param[1];
#endif
	}
	return TRUE;
}

int APP_MotorStart(INT8U obj,INT8U dir)
{
	if (Antenna_DriveStart(obj, dir) == FAIL)
	{
		if (Antenna_DriveStart(obj, dir) == FAIL)
		{
			return FAIL;
		}
	}
	return SUCC;
}

int APP_MotorEnd(INT8U obj, INT8U dir)
{
	if (Antenna_DriveEnd(obj, dir) == FAIL)
	{
		if (Antenna_DriveEnd(obj, dir) == FAIL)
		{
			return FAIL;
		}
	}
	return SUCC;
}

int APP_LogFollowSatRecord(INT32U code)
{
	if (APP_LOG_FollowSatRecord(code) == FAIL)
	{
		return APP_LOG_FollowSatRecord(code);
	}
	return SUCC;
}

/***********************************************************
**name:			adapt_target_angle
**describe:		调整天线姿态到指定状态
**autor:		tao
************************************************************/
int adapt_target_angle(FP32 tar_Az, FP32 tar_El, FP32 tar_Pol, INT8U stop_f_Az, INT8U stop_f_El, INT8U stop_f_Pol)
{
	FP32 Az_obj = tar_Az;
	FP32 El_obj = tar_El;
	FP32 Pol_obj = tar_Pol;
	INT8U Az_Motor_Stop_Flag = stop_f_Az;
	INT8U El_Motor_Stop_Flag = stop_f_El;
	INT8U Pol_Motor_Stop_Flag = stop_f_Pol;
	FP32 Az = 0.0;
	FP32 El = 0.0;
	FP32 Pol = 0.0;
	INT8U Az_dir = 0;
	INT8U El_dir = 0;
	INT8U Pol_dir = 0;
	FP32 Az_lBound = ACUDevInfo.ACUParam.AntennaParam.motor_position_start;   //?????? 方位扫描下限
	FP32 Az_uBound = ACUDevInfo.ACUParam.AntennaParam.motor_position_end; //?????? 方位扫描上限
	FP32 EL_lBound = ACUDevInfo.ACUParam.AntennaParam.motor_cabrage_start;   //?????? 俯仰扫描下限
	FP32 EL_uBound = ACUDevInfo.ACUParam.AntennaParam.motor_cabrage_end;  //?????? 俯仰扫描上限
	FP32 PL_lBound = ACUDevInfo.ACUParam.AntennaParam.motor_polar_start;
	FP32 PL_uBound = ACUDevInfo.ACUParam.AntennaParam.motor_polar_end;
	
	if(stop_f_Pol == 0)
	{
		if(tar_Pol<PL_lBound || tar_Pol>PL_uBound)
		{
			return FAIL;
		}
	}
	if(tar_Az<Az_lBound || tar_Az>Az_uBound)
	{
		return FAIL;
	}
	
	if(tar_El<EL_lBound||tar_El>EL_uBound)
	{
		return FAIL;
	}

	if (APP_CurrDegree_Get_FP(ANG_POSITION, &Az) == FAIL) return FAIL;
	if (APP_GE(Az, Az_obj, 0) == TRUE)
	{
		Az_dir = Drec_anticlockwise_down;
	}
	else
	{
		Az_dir = Drec_clockwise_up;
	}
	if (APP_CurrDegree_Get_FP(ANG_CABRAGE, &El) == FAIL) return FAIL;
	if (APP_GE(El, El_obj, 0) == TRUE)
	{
		El_dir = Drec_anticlockwise_down;
	}
	else
	{
		El_dir = Drec_clockwise_up;
	}
	if (APP_CurrDegree_Get_FP(ANG_POLARIZATION, &Pol) == FAIL) return FAIL;
	if (APP_GE(Pol, Pol_obj, 0) == TRUE)
	{
		Pol_dir = Drec_anticlockwise_down;
	}
	else
	{
		Pol_dir = Drec_clockwise_up;
	}

	if (Az_Motor_Stop_Flag == 0)
	{
		if(APP_MotorStart(motor_position_id, Az_dir)==FAIL) return FAIL;
	}
	if (El_Motor_Stop_Flag == 0)
	{
		if (APP_MotorStart(motor_cabrage_id, El_dir) == FAIL) return FAIL;
	}
	if (Pol_Motor_Stop_Flag == 0)
	{
		if(APP_MotorStart(motor_polar_id, Pol_dir)==FAIL) return FAIL;
	}

	while (Az_Motor_Stop_Flag == 0 || El_Motor_Stop_Flag == 0 || Pol_Motor_Stop_Flag == 0)
	{
		if (Az_Motor_Stop_Flag == 0)
		{
			if (APP_CurrDegree_Get_FP(ANG_POSITION, &Az) == FAIL) return FAIL;
			if (Az_dir == Drec_anticlockwise_down)
			{
				if (APP_LE(Az, Az_obj, ACCEPTENCE_DOMAIN) == TRUE)
				{
					if(APP_MotorEnd(motor_position_id, Az_dir)==FAIL) return FAIL;
					Az_Motor_Stop_Flag = 1;
				}
			}
			else
			{
				if (APP_LE(Az_obj, Az, ACCEPTENCE_DOMAIN) == TRUE)
				{
					if(APP_MotorEnd(motor_position_id, Az_dir)==FAIL) return FAIL;
					Az_Motor_Stop_Flag = 1;
				}
			}
		}

		if (El_Motor_Stop_Flag == 0)
		{
			if (APP_CurrDegree_Get_FP(ANG_CABRAGE, &El) == FAIL) return FAIL;
			if (El_dir == Drec_anticlockwise_down)
			{
				if (APP_LE(El, El_obj, ACCEPTENCE_DOMAIN) == TRUE)
				{
					if(APP_MotorEnd(motor_cabrage_id, El_dir)==FAIL) return FAIL;
					El_Motor_Stop_Flag = 1;
				}
			}
			else
			{
				if (APP_LE(El_obj, El, ACCEPTENCE_DOMAIN) == TRUE)
				{
					if(APP_MotorEnd(motor_cabrage_id, El_dir)==FAIL) return FAIL;
					El_Motor_Stop_Flag = 1;
				}
			}
		}

		if (Pol_Motor_Stop_Flag == 0)
		{
			if (APP_CurrDegree_Get_FP(ANG_POLARIZATION, &Pol) == FAIL) return FAIL;
			if (Pol_dir == Drec_anticlockwise_down)
			{
				if (APP_LE(Pol, Pol_obj, ACCEPTENCE_DOMAIN) == TRUE)
				{
					if(APP_MotorEnd(motor_polar_id, Pol_dir)==FAIL) return FAIL;
					Pol_Motor_Stop_Flag = 1;
				}
			}
			else
			{
				if (APP_LE(Pol_obj, Pol, ACCEPTENCE_DOMAIN) == TRUE)
				{
					if(APP_MotorEnd(motor_polar_id, Pol_dir)==FAIL) return FAIL;
					Pol_Motor_Stop_Flag = 1;
				}
			}
		}
	}

	// 电机电机保护延时
#ifdef _SIMULATION_
	Sleep(3000);
#else 
	BSP_Delay_ms(3000);
#endif

	return SUCC;
}

/***********************************************************
**name:			cross_scan
**describe:		十字扫描
**autor:		tao
************************************************************/
int cross_scan(FP32 delta_Az, FP32 delta_El)
{
	// 计算扫描范围
	FP32 Az0 = 0;
	FP32 EL0 = 0;
	FP32 Az_min = 0; 
	FP32 Az_max = 0; 
	FP32 EL_min = 0;
	FP32 EL_max = 0; 
	FP32 Az_lBound = ACUDevInfo.ACUParam.AntennaParam.motor_position_start+1;   //?????? 方位扫描下限
	FP32 Az_uBound = ACUDevInfo.ACUParam.AntennaParam.motor_position_end-1; //?????? 方位扫描上限
	FP32 EL_lBound = ACUDevInfo.ACUParam.AntennaParam.motor_cabrage_start+1;   //?????? 俯仰扫描下限
	FP32 EL_uBound = ACUDevInfo.ACUParam.AntennaParam.motor_cabrage_end-1;  //?????? 俯仰扫描上限
	FP32 p0 = 0;
	//INT16U agc0 = 0;
	FP32 p = -1;
	FP32 Az = 0; 
	FP32 EL = 0; 

	if (APP_CurrDegree_Get_FP(ANG_POSITION, &Az0) == FAIL)
	{
		return FAIL;
	}
	if (APP_CurrDegree_Get_FP(ANG_CABRAGE, &EL0) == FAIL)
	{
		return FAIL;
	}
	if (APP_Power_Get(&p0) == FAIL)
	{
		return FAIL;
	}
	Az_min = Az0 - delta_Az;
	if (Az_min < Az_lBound) Az_min = Az_lBound;
	Az_max = Az0 + delta_Az;
	if (Az_max > Az_uBound) Az_max = Az_uBound;
	EL_min = EL0 - delta_El;
	if (EL_min < EL_lBound) EL_min = EL_lBound;
	EL_max = EL0 + delta_El;
	if (EL_max > EL_uBound) EL_max = EL_uBound;

	Az = Az0;
	EL = EL0;

	// 驱动俯仰电机转动到EL_min
	if(APP_MotorStart(motor_cabrage_id, Drec_anticlockwise_down)==FAIL) return FAIL; // 向下驱动俯仰电机
	do{
		//BSP_Dealy_ms(2);
		if (APP_CurrDegree_Get_FP(ANG_CABRAGE, &EL0) == FAIL)
		{
			APP_MotorEnd(motor_cabrage_id, Drec_anticlockwise_down);
			return FAIL;
		}
	} while (APP_GE((double)EL0, (double)EL_min, ACCEPTENCE_DOMAIN) == TRUE); // 如果接近目标就退出循环，停止电机
	if(APP_MotorEnd(motor_cabrage_id, Drec_anticlockwise_down)==FAIL) return FAIL;


	// 电机电机保护延时
#ifdef _SIMULATION_
	Sleep(1);
#else 
	BSP_Delay_ms(3000);
#endif

	// 驱动俯仰电机转动到El_max，扫描俯仰功率最大值
	if(APP_MotorStart(motor_cabrage_id, Drec_clockwise_up)==FAIL) return FAIL;
	do{
		//BSP_Dealy_ms(5);
		if (APP_CurrDegree_Get_FP(ANG_CABRAGE, &EL0) == FAIL)
		{
			APP_MotorEnd(motor_cabrage_id, Drec_clockwise_up);
			return FAIL;
		}
		if (APP_Power_Get(&p0) == FAIL)
		{
			APP_MotorEnd(motor_cabrage_id, Drec_clockwise_up);
			return FAIL;
		}
		//p0 >>= 8;
		if (APP_GE(p0, p, 0) == TRUE)
		{
			p = p0;
			EL = EL0;
		}
	} while (APP_GE((double)EL_max, (double)EL0, ACCEPTENCE_DOMAIN) == TRUE);
	if(APP_MotorEnd(motor_cabrage_id, Drec_clockwise_up)==FAIL) return FAIL;

	// 电机电机保护延时
#ifdef _SIMULATION_
	Sleep(1);
#else 
	BSP_Delay_ms(3000);
#endif

	// 驱动俯仰电机转动到俯仰最佳位置
	if (APP_GE((double)EL, (double)EL0, 0) != TRUE)
	{
		if(APP_MotorStart(motor_cabrage_id, Drec_anticlockwise_down)==FAIL) return FAIL; // 向下驱动俯仰电机
		do{
			//BSP_Dealy_ms(2);
			if (APP_CurrDegree_Get_FP(ANG_CABRAGE, &EL0) == FAIL)
			{
				APP_MotorEnd(motor_cabrage_id, Drec_anticlockwise_down);
				return FAIL;
			}
		} while (APP_GE((double)EL0, (double)EL, ACCEPTENCE_DOMAIN) == TRUE); // 如果接近目标就退出循环，停止电机
		if(APP_MotorEnd(motor_cabrage_id, Drec_anticlockwise_down)== FAIL) return FAIL;
	}
	//else
	//{
	//	if(APP_MotorStart(motor_cabrage_id, Drec_clockwise_up)==FAIL) return FAIL;
	//	do{
	//		//BSP_Dealy_ms(2);
	//		if (APP_CurrDegree_Get_FP(ANG_CABRAGE, &EL0) == FAIL)
	//		{
	//			APP_MotorEnd(motor_cabrage_id, Drec_clockwise_up);
	//			return FAIL;
	//		}
	//	} while (APP_GE((double)EL, (double)EL0, ACCEPTENCE_DOMAIN) == TRUE);
	//	if(APP_MotorEnd(motor_cabrage_id, Drec_clockwise_up)==FAIL) return FAIL;
	//}


/*	// 电机电机保护延时
#ifdef _SIMULATION_
	Sleep(3000);
#else 
	BSP_Dealy_ms(3000);
#endif*/

	// 驱动方位电机到Az_min
	if(APP_MotorStart(motor_position_id, Drec_anticlockwise_down)==FAIL) return FAIL; // 逆时针驱动方位电机
	do{
		//BSP_Dealy_ms(2);
		if (APP_CurrDegree_Get_FP(ANG_POSITION, &Az0) == FAIL)
		{
			APP_MotorEnd(motor_position_id, Drec_anticlockwise_down);
			return FAIL;
		}
	} while (APP_GE((double)Az0, (double)Az_min, ACCEPTENCE_DOMAIN) == TRUE); // 如果接近目标就退出循环，停止电机
	if(APP_MotorEnd(motor_position_id, Drec_anticlockwise_down)==FAIL) return FAIL;

	// 电机电机保护延时
#ifdef _SIMULATION_
	Sleep(1);
#else 
	BSP_Delay_ms(3000);
#endif

	// 驱动方位电机转动到Az_max, 扫描方位功率最大值
	if(APP_MotorStart(motor_position_id, Drec_clockwise_up)==FAIL) return FAIL;
	do{
		//BSP_Dealy_ms(5);
		if (APP_CurrDegree_Get_FP(ANG_POSITION, &Az0) == FAIL)
		{
			APP_MotorEnd(motor_position_id, Drec_clockwise_up);
			return FAIL;
		}
		if (APP_Power_Get(&p0) == FAIL)
		{
			APP_MotorEnd(motor_position_id, Drec_clockwise_up);
			return FAIL;
		}
		//p0 >>= 8;
		if (APP_GE(p0, p, 0) == TRUE)
		{
			p = p0;
			Az = Az0;
		}
	} while (APP_GE((double)Az_max, (double)Az0, ACCEPTENCE_DOMAIN) == TRUE);
	if(APP_MotorEnd(motor_position_id, Drec_clockwise_up)==FAIL) return FAIL;

	// 电机电机保护延时
#ifdef _SIMULATION_
	Sleep(1);
#else 
	BSP_Delay_ms(3000);
#endif

	// 驱动方位电机转动到方位最佳位置
	if (APP_GE((double)Az, (double)Az0, 0) != TRUE)
	{
		if(APP_MotorStart(motor_position_id, Drec_anticlockwise_down)==FAIL) return FAIL; // 逆时针驱动方位电机
		do{
			//BSP_Dealy_ms(2);
			if (APP_CurrDegree_Get_FP(ANG_POSITION, &Az0) == FAIL)
			{
				APP_MotorEnd(motor_position_id, Drec_anticlockwise_down);
				return FAIL;
			}
		} while (APP_GE((double)Az0, (double)Az, ACCEPTENCE_DOMAIN) == TRUE); // 如果接近目标就退出循环，停止电机
		if(APP_MotorEnd(motor_position_id, Drec_anticlockwise_down)==FAIL) return FAIL;
	}
	//else
	//{
	//	if(APP_MotorStart(motor_position_id, Drec_clockwise_up)==FAIL) return FAIL;
	//	do{
	//		//BSP_Dealy_ms(2);
	//		if (APP_CurrDegree_Get_FP(ANG_POSITION, &Az0) == FAIL)
	//		{
	//			APP_MotorEnd(motor_position_id, Drec_clockwise_up);
	//			return FAIL;
	//		}
	//	} while (APP_GE((double)Az, (double)Az0, ACCEPTENCE_DOMAIN) == TRUE);
	//	if(APP_MotorEnd(motor_position_id, Drec_clockwise_up)==FAIL) return FAIL;
	//}

/*	// 电机电机保护延时
#ifdef _SIMULATION_
	Sleep(3000);
#else 
	BSP_Dealy_ms(3000);
#endif*/

	return SUCC;
}

/***********************************************************
**name:			pl_scan
**describe:		极化闭环
**autor:		tao
************************************************************/
int pl_scan()
{
	// 计算扫描范围
	FP32 PL0 = 0;
	FP32 PL = 0;
	FP32 PL_lBound = -20; //ACUDevInfo.ACUParam.AntennaParam.motor_polar_start + 1;   //?????? 俯仰扫描下限
	FP32 PL_uBound = 20; //ACUDevInfo.ACUParam.AntennaParam.motor_polar_end - 1;  //?????? 俯仰扫描上限
	FP32 p0 = 0;
	FP32 p = -1;

	if (APP_CurrDegree_Get_FP(ANG_POLARIZATION, &PL0) == FAIL)
	{
		return FAIL;
	}

	PL_uBound += PL0;
	if(PL_uBound>=ACUDevInfo.ACUParam.AntennaParam.motor_polar_end - 1)
	{
		PL_uBound = ACUDevInfo.ACUParam.AntennaParam.motor_polar_end - 1;
	}
	
	PL_lBound += PL0;
	if(PL_lBound<=ACUDevInfo.ACUParam.AntennaParam.motor_polar_start + 1)
	{
		PL_lBound = ACUDevInfo.ACUParam.AntennaParam.motor_polar_start + 1;
	}
	
	if (APP_Power_AG_Get(&p0) == FAIL)
	{
		return FAIL;
	}

	// 驱动极化电机转动到EL_min
	if(APP_MotorStart(motor_polar_id, Drec_anticlockwise_down)== FAIL) return FAIL; // 逆时针驱动极化电机
	do{
		//BSP_Dealy_ms(2);
		if (APP_CurrDegree_Get_FP(ANG_POLARIZATION, &PL0) == FAIL)
		{
			APP_MotorEnd(motor_polar_id, Drec_anticlockwise_down);
			return FAIL;
		}
	} while (APP_GE((double)PL0, (double)PL_lBound, ACCEPTENCE_DOMAIN) == TRUE); // 如果接近目标就退出循环，停止电机
	if(APP_MotorEnd(motor_polar_id, Drec_anticlockwise_down)==FAIL) return FAIL;


	// 电机电机保护延时
#ifdef _SIMULATION_
	Sleep(1);
#else 
	BSP_Delay_ms(3000);
#endif

	// 驱动极化电机转动到El_max，扫描极化功率最大值
	if(APP_MotorStart(motor_polar_id, Drec_clockwise_up)==FAIL) return FAIL;
	do{
		//BSP_Dealy_ms(5);
		if (APP_CurrDegree_Get_FP(ANG_POLARIZATION, &PL0) == FAIL)
		{
			APP_MotorEnd(motor_polar_id, Drec_clockwise_up);
			return FAIL;
		}
		if (APP_Power_Get(&p0) == FAIL)
		{
			APP_MotorEnd(motor_polar_id, Drec_clockwise_up);
			return FAIL;
		}
		if (APP_GE(p0, p, 0) == TRUE)
		{
			p = p0;
			PL = PL0;
		}
	} while (APP_GE((double)PL_uBound, (double)PL0, ACCEPTENCE_DOMAIN) == TRUE);
	if(APP_MotorEnd(motor_polar_id, Drec_clockwise_up)==FAIL) return FAIL;

	// 电机电机保护延时
#ifdef _SIMULATION_
	Sleep(1);
#else 
	BSP_Delay_ms(3000);
#endif

	// 逆时针驱动极化电机转动到极化最佳位置
	if(APP_MotorStart(motor_polar_id, Drec_anticlockwise_down)==FAIL) return FAIL; // 向下驱动俯仰电机
	do{
		//BSP_Dealy_ms(2);
		if (APP_CurrDegree_Get_FP(ANG_POLARIZATION, &PL0) == FAIL)
		{
			APP_MotorEnd(motor_polar_id, Drec_anticlockwise_down);
			return FAIL;
		}
	} while (APP_GE((double)PL0, (double)PL, ACCEPTENCE_DOMAIN) == TRUE); // 如果接近目标就退出循环，停止电机
	if(APP_MotorEnd(motor_polar_id, Drec_anticlockwise_down)==FAIL) return FAIL;

	/*	// 电机电机保护延时
	#ifdef _SIMULATION_
	Sleep(3000);
	#else
	BSP_Dealy_ms(3000);
	#endif*/

	return SUCC;
}

/***********************************************************
**name:			APP_CurrDegree_Get
**describe: 		获取指定码盘当前角度
**input:	    num:码盘id
**				degree:接收结果的指针
**output:		degree:码盘当前角度
**return: 		执行结果
**autor:  		tao
**date:
************************************************************/
int APP_CurrDegree_Get_FP(INT8U num, FP32 *degree)
{
	switch (num)
	{
	case ANG_POSITION:
		*degree = ACUDevInfo.ACUCurrSta.antenna_position;
		break;
	case ANG_CABRAGE:
		*degree = ACUDevInfo.ACUCurrSta.antenna_cabrage;
		break;
	case ANG_POLARIZATION:
		*degree = ACUDevInfo.ACUCurrSta.antenna_polar;
		break;
	}

	return SUCC;
}

/***********************************************************
**name:			APP_Satellite_Drift_Check
**describe: 		卫星漂移检测
**input:	    p0 对准时的功率值，segma 偏差范围
**output:		none
**return: 		执行结果
**autor:  		tao
**date:
************************************************************/
int APP_Satellite_Drift_Check(FP32 *p0, FP32 p_obj, FP32 segma)
{	
	*p0 = 0;
	if(APP_Power_AG_Get(p0)==FAIL) 			
	{
		return APP_SAT_TRACK_ERR;
	}
	if(APP_GE(p_obj,*p0,segma)==TRUE)
	{
		return UNDERPOWER;
	}
	return FULLPOWER;
}

/***********************************************************
**name:			APP_Power_AG_Get
**describe: 		获取功率平均值
**input:	    p 获取功率的指针
**output:		p 获取的功率平均值
**return: 		执行结果
**autor:  		tao
**date:
************************************************************/
int APP_Power_AG_Get(FP32 *p)
{
	INT32U samp_cycle = 200; // ???????
	INT32U samp_count = 30;
	FP32 power_ag = 0;
	INT32U i = 0;

	for(i;i<samp_count;i++)
	{
		power_ag += (ACUDevInfo.ACUCurrSta.beacon_rxpower_mv / 1000.0f / samp_count);
		BSP_Delay_ms(samp_cycle);
	}
	*p = power_ag;
	return SUCC;
}

/***********************************************************
**name:			APP_Power_AG_Get_4_PURS
**describe: 		获取功率平均值
**input:	    p 获取功率的指针
**output:		p 获取的功率平均值
**return: 		执行结果
**autor:  		tao
**date:
************************************************************/
int APP_Power_AG_Get_4_PURS(FP32 *p)
{
	INT32U samp_cycle = 100; // ???????
	INT32U samp_count = 20;
	FP32 power_ag = 0;
	INT32U i = 0;

	for(i;i<samp_count;i++)
	{
		power_ag += (ACUDevInfo.ACUCurrSta.beacon_rxpower_mv / 1000.0f / samp_count);
		BSP_Delay_ms(samp_cycle);
	}
	*p = power_ag;
	return SUCC;
}

int APP_Power_Deviation(FP32 *p)
{
	INT32U samp_cycle = 100; // ???????
	INT32U samp_count = 50;
	FP32 power_ag = 0;
	FP32 power_cur = 0;
	INT32U i = 0;

	for (i; i<samp_count; i++)
	{
		power_cur = ACUDevInfo.ACUCurrSta.beacon_rxpower_mv / 1000.0f;
		power_ag += (((power_cur - *p)*(power_cur - *p)) / samp_count);
		BSP_Delay_ms(samp_cycle);
	}
	*p = sqrt(power_ag);
	return SUCC;
}

int APP_Power_Get(FP32 *p)
{
	*p = ACUDevInfo.ACUCurrSta.beacon_rxpower_mv/1000.0f;
	return SUCC;
}

/***********************************************************
**name:			APP_Preliminary_Aim
**describe: 		粗对星
**input:	    none
**output:		none
**return: 		执行结果
**autor:  		tao
**date:
************************************************************/
int APP_Preliminary_Aim()
{
	/*FP32 lgt_sat = 0;
	FP32 lgt_ert = 0;
	FP32 lat_ert = 0;
	FP32 R = 6378136.49f;			// 地球半径
	FP32 hi_sat = 0;				// 卫星高度
	FP32 Az_cur = 0;
	FP32 El_cur = 0;
	FP32 Pol_cur = 0;
	FP32 Az_obj = 0;				// ??????
	FP32 El_obj = 0;				// ??????
	FP32 Pol_obj = 0;				// ??????

	// 获取地理信息
	// ??????
	lgt_sat = ACUDevInfo.ACUParam.CurrSatParam.lon;
	if (ACUDevInfo.ACUParam.CurrSatParam.lon_flag=='W')
	{
		lgt_sat = -lgt_sat;
	}
	lgt_ert = ACUDevInfo.ACUParam.LocalPositionParam.lon;
	if (ACUDevInfo.ACUParam.LocalPositionParam.lon_flag=='W')
	{
		lgt_ert = -lgt_ert;
	}
	lat_ert = ACUDevInfo.ACUParam.LocalPositionParam.lat;
	if (ACUDevInfo.ACUParam.LocalPositionParam.lat_flag=='S')
	{
		lat_ert = -lat_ert;
	}
	hi_sat = (FP32)ACUDevInfo.ACUParam.LocalPositionParam.height;

	// 获取当前天线姿态
	if (APP_CurrDegree_Get_FP(ANG_POSITION, &Az_cur) == FAIL)
	{
		return FAIL;
	}
	if (APP_CurrDegree_Get_FP(ANG_CABRAGE, &El_cur) == FAIL)
	{
		return FAIL;
	}
	if (APP_CurrDegree_Get_FP(ANG_POLARIZATION, &Pol_cur) == FAIL)
	{
		return FAIL;
	}

	// 计算目标姿态
	if (sin(lat_ert) != 0) {
		Az_obj = 180 - (FP32)ATAN(TAN(lgt_sat - lgt_ert) / SIN(lat_ert));
	}
	else{
		// 设置 Az_obj = 当前值
		Az_obj = Az_cur;
	}
	if ((1 - pow((COS(lgt_sat - lgt_ert)*COS(lat_ert)), 2.0)) != 0){
		El_obj = (FP32)ATAN((COS(lgt_sat - lgt_ert)*COS(lat_ert) - (R / R + hi_sat))
				/ (1 - pow((COS(lgt_sat - lgt_ert)*COS(lat_ert)), 2.0)));
	}
	else{
		// 设置 El_obj = 当前值
		El_obj = El_cur;
	}
	if (tan(lat_ert) != 0){
		Pol_obj = (FP32)ATAN(SIN(lgt_ert - lgt_sat) / TAN(lat_ert));
	}
	else{
		// 设置 Pol_obj = 当前值
		Pol_obj = Pol_cur;
	}*/
	INT8U PolFlag = 1;
	if (ACUDevInfo.ACUParam.AntennaParam.elec_coder_num >= 3)
	{
		PolFlag = 0;
	}
	
	return adapt_target_angle(
		ACUDevInfo.ACUParam.AntennaParam.aim_position,
		ACUDevInfo.ACUParam.AntennaParam.aim_cabrage, 
		ACUDevInfo.ACUParam.AntennaParam.aim_polar,
		0, 0, PolFlag);
}

/***********************************************************
**name:			APP_Satellite_Cap_Scan
**describe: 		精对星（十字扫描对星）
**input:	    none
**output:		Azc 对星后的方位，ELc 对星后的俯仰，pc 对星后的功率值
**return: 		执行结果
**autor:  		tao
**date:
************************************************************/
int APP_Satellite_Cap_Scan(FP32 *Azc, FP32 *ELc, FP32 *pc)
{
	// 计算扫描范围
	FP32 delta_Az = 0;
	FP32 delta_El = 0;

#ifdef _SIMULATION_
	delta_Az = (FP32)scan_rs[waveBand][0];
#else
	delta_Az = ACUDevInfo.ACUParam.SeekSatParam.ScanRange_param[0];
#endif

	delta_El = delta_Az;

	if (cross_scan(delta_Az, delta_El) == FAIL) return FAIL;

	//if (cross_scan(0.5f, 0.5f) == FAIL) return FAIL;

	// 更新pc，更新Azc，更新ELc
	if (APP_CurrDegree_Get_FP(ANG_CABRAGE, ELc) == FAIL)
	{
		return FAIL;
	}
	if (APP_CurrDegree_Get_FP(ANG_POLARIZATION, Azc) == FAIL)
	{
		return FAIL;
	}
	if (APP_Power_AG_Get(pc) == FAIL)
	{
		return FAIL;
	}
	
	BSP_Delay_ms(3000);
	
	if(APP_Triangle_Track(Azc,ELc,pc)==FAIL) return FAIL;

	return SUCC;
}

int APP_Triangle_Track(FP32 *Azc, FP32 *ELc, FP32 *pc)
{
	int i = 1;
	
	// 初始步骤
	FP32 Az_step0 = -0.03; // 等边三角形初始化 // 0.04; //等腰直角三角形初始化	//   
	FP32 El_step0 = 0.03*1.7320508f; 								 //0;								//   
	FP32 Az_step1 = 0.06;
	FP32 El_step1 = 0;								 //-0.04;							//
	// 基点坐标
	FP32 beta_Az = *Azc;
	FP32 beta_El = *ELc;
	// 基点重置计数
	INT8U C0 = 0;
	// 三个检测点
	FP32 tar_Az_H = 0;
	FP32 tar_El_H = 0;
	FP32 tar_Az_M = 0;
	FP32 tar_El_M = 0;
	FP32 tar_Az_L = 0;
	FP32 tar_El_L = 0;
	FP32 P_H = 0;
	FP32 P_M = 0;
	FP32 P_L = 0;
	FP32 tar_Az = 0;
	FP32 tar_El = 0;
	// 当前值
	FP32 Az = 0;
	FP32 El = 0;
	FP32 P = 0;
	// 接受功率差,结束条件
	FP32 delta_P = 0.3f;

	FP32 temp_Az = 0;
	FP32 temp_El = 0;
	FP32 temp_P = 0;
	
	FP32 eg_h = 0;
	FP32 eg_t = 0;
	FP32 eg_m = 0;
	FP32 cosBeta = 60;

	APP_CurrDegree_Get_FP(ANG_POSITION, &Az);
	APP_CurrDegree_Get_FP(ANG_CABRAGE, &El);
	APP_Power_Get(&P);

	tar_Az_H = Az;
	tar_El_H = El;
	P_H = P;
	beta_Az = Az;
	beta_El = El;

	if (adapt_target_angle(Az + Az_step0, El + El_step0, 0, 0, 0, 1) == FAIL)
	{
#ifdef _SIMULATION_
		printf("Gradient track end in FAIL!\n\n");
#endif
		return FAIL;
	}
	BSP_Delay_ms(3000);
	//ACUDevInfo.ACUCurrSta.antenna_position += Az_step0;
	//ACUDevInfo.ACUCurrSta.antenna_cabrage += El_step0;

	APP_CurrDegree_Get_FP(ANG_POSITION, &Az);
	APP_CurrDegree_Get_FP(ANG_CABRAGE, &El);
	APP_Power_Get(&P);

	if (P >= P_H)
	{
		tar_Az_M = tar_Az_H;
		tar_El_M = tar_El_H;
		P_M = P_H;
		tar_Az_H = Az;
		tar_El_H = El;
		P_H = P;
	}
	else
	{
		tar_Az_M = Az;
		tar_El_M = El;
		P_M = P;
	}
	if(*pc<P_H);
	{
		*pc = P_H;
		*Azc = tar_Az_H;
		*ELc = tar_El_H;
	}

	if (adapt_target_angle(Az + Az_step1, El + El_step1, 0, 0, 0, 1) == FAIL)
	{
#ifdef _SIMULATION_
		printf("Gradient track end in FAIL!\n\n");
#endif
		return FAIL;
	}
	BSP_Delay_ms(3000);
	//ACUDevInfo.ACUCurrSta.antenna_position += Az_step1;
	//ACUDevInfo.ACUCurrSta.antenna_cabrage += El_step1;

	APP_CurrDegree_Get_FP(ANG_POSITION, &Az);
	APP_CurrDegree_Get_FP(ANG_CABRAGE, &El);
	APP_Power_Get(&P);

	if (P >= P_H)
	{
		tar_Az_L = tar_Az_M;
		tar_El_L = tar_El_M;
		P_L = P_M;
		tar_Az_M = tar_Az_H;
		tar_El_M = tar_El_H;
		P_M = P_H;
		tar_Az_H = Az;
		tar_El_H = El;
		P_H = P;
	}
	else if (P >= P_M)
	{
		tar_Az_L = tar_Az_M;
		tar_El_L = tar_El_M;
		P_L = P_M;
		tar_Az_M = Az;
		tar_El_M = El;
		P_M = P;
	}
	else
	{
		tar_Az_L = Az;
		tar_El_L = El;
		P_L = P;
	}
	if(*pc<P_H);
	{
		*pc = P_H;
		*Azc = tar_Az_H;
		*ELc = tar_El_H;
	}
	i = 1;

	while ((APP_NB(P_L, P_M, delta_P) == FALSE
		|| APP_NB(P_M, P_H, delta_P) == FALSE
		|| APP_NB(P_H, P_L, delta_P) == FALSE) && C0 != 6)
	{
		tar_Az = tar_Az_H - tar_Az_L + tar_Az_M;
		tar_El = tar_El_H - tar_El_L + tar_El_M;

		if (APP_NB(tar_Az, beta_Az, 0.05f) == TRUE
			&& APP_NB(tar_Az, beta_Az, 0.05f) == TRUE)
		{
			temp_Az = tar_Az_M;
			temp_El = tar_El_M;
			temp_P = P_M;

			tar_Az_M = tar_Az_L;
			tar_El_M = tar_El_L;
			P_M = P_L;
			tar_Az_L = temp_Az;
			tar_El_L = temp_El;
			P_L = temp_P;

			tar_Az = tar_Az_H - tar_Az_L + tar_Az_M;
			tar_El = tar_El_H - tar_El_L + tar_El_M;

			C0 += 1;
		}

		beta_Az = tar_Az_L;
		beta_El = tar_El_L;
		
		eg_t = (tar_Az-beta_Az)*(tar_Az-beta_Az)+(tar_El-beta_El)*(tar_El-beta_El);
		eg_h = (Az-beta_Az)*(Az-beta_Az)+(El-beta_El)*(El-beta_El);
		eg_m = (tar_Az-Az)*(tar_Az-Az)+(tar_El-El)*(tar_El-El);
		cosBeta = (eg_t+eg_h-eg_m)/(2*(FP32)sqrt(eg_t*eg_h));
		
		if(cosBeta>0.98)
		{
			break;
		}

#ifdef _SIMULATION_
		printf("Tri track step=%d:\r\naz_mainlobe:%f,Az=%f\r\nel_mainlobe:%f,EL=%f\r\npower=%f\r\nc0=%d\r\n\n", i,
			ACUDevInfo.ACUParam.Old.az_opt_mainlobe, tar_Az-Az, ACUDevInfo.ACUParam.Old.el_opt_mainlobe, tar_El-El, P, C0);
		i++;
#endif

		//if (fabs(tar_Az-Az) < 0.001373f) delta_Az = DEF_GRD_STEP_AZ;
		//if (fabs(tar_El-El) < 0.001373f) delta_El = DEF_GRD_STEP_EL;
		if (adapt_target_angle(tar_Az, tar_El, 0, 0, 0, 1) == FAIL)
		{
#ifdef _SIMULATION_
			printf("tri track end in FAIL!\n\n");
#endif
			return FAIL;
		}
		BSP_Delay_ms(3000);
		//ACUDevInfo.ACUCurrSta.antenna_position += tar_Az - Az;
		//ACUDevInfo.ACUCurrSta.antenna_cabrage += tar_El - El;

		APP_CurrDegree_Get_FP(ANG_POSITION, &Az);
		APP_CurrDegree_Get_FP(ANG_CABRAGE, &El);
		APP_Power_AG_Get_4_PURS(&P);

		if (P >= P_H)
		{
			tar_Az_L = tar_Az_M;
			tar_El_L = tar_El_M;
			P_L = P_M;
			tar_Az_M = tar_Az_H;
			tar_El_M = tar_El_H;
			P_M = P_H;
			tar_Az_H = Az;
			tar_El_H = El;
			P_H = P;
			C0 = 0;
		}
		else if (P >= P_M)
		{
			tar_Az_L = tar_Az_M;
			tar_El_L = tar_El_M;
			P_L = P_M;
			tar_Az_M = Az;
			tar_El_M = El;
			P_M = P;
		}
		else
		{
			tar_Az_L = Az;
			tar_El_L = El;
			P_L = P;
		}
		
		if(*pc<P_H);
		{
			*pc = P_H;
			*Azc = tar_Az_H;
			*ELc = tar_El_H;
		}
	}
	
	if(*pc>=P_H);
	{
		tar_Az_H = *Azc;
		tar_El_H = *ELc;
	}

	if (adapt_target_angle(tar_Az_H, tar_El_H, 0, 0, 0, 1) == FAIL)
	{
#ifdef _SIMULATION_
		printf("tri track end in FAIL!\n\n");
#endif
		return FAIL;
	}
	BSP_Delay_ms(3000);
	//ACUDevInfo.ACUCurrSta.antenna_position += tar_Az_H - Az;
	//ACUDevInfo.ACUCurrSta.antenna_cabrage += tar_El_H - El;

	APP_CurrDegree_Get_FP(ANG_POSITION, Azc);
	APP_CurrDegree_Get_FP(ANG_CABRAGE, ELc);
	APP_Power_AG_Get_4_PURS(pc);

	return SUCC;
}

/***********************************************************
**name:			APP_Gradient_Track
**describe: 		梯度追星主程序
**input:	    none
**output:		Azc 对星后的方位，ELc 对星后的俯仰，pc 对星后的功率值
**return: 		执行结果
**autor:  		tao
**date:
************************************************************/
int APP_Gradient_Track(FP32 *Azc, FP32 *ELc, FP32 *pc){
	FP32 delta_r = 0.08; // 0.475f; // 		// 导引步进固定为0.1度
	FP32 segma_stop = 0.5f;	// 停止门限固定为0.1
	FP32 Az_old_I = 0.0f;
	FP32 Az_old = 0.0f;
	FP32 Az_new = 0.0f;
	FP32 El_old_I = 0.0f;
	FP32 El_old = 0.0f;
	FP32 El_new = 0.0f;
	FP32 p_old_I = 0;
	FP32 p_old = 0;
	FP32 p_new = 0;
	FP32 delta_Az_old = 0.0f;
	FP32 delta_El_old = 0.0f;
	FP32 delta_p_old = 0;
	FP32 delta_Az_new = 0.0f;
	FP32 delta_El_new = 0.0f;
	FP32 delta_p_new = 0;
	FP32 delta_Az = 0.0f;
	FP32 delta_El = 0.0f;
	FP32 segma0 = 0.01f;		// 角度差补偿值
	FP32 d_Az = 0.0f;
	FP32 d_El = 0.0f;
	FP32 d_Az_n = 0.0f;
	FP32 d_El_n = 0.0f;
	FP32 d_Az_El = 0.0f;
	INT8U Az_Motor_Stop_Flag = 0;
	INT8U El_Motor_Stop_Flag = 0;
	INT8U step = 0;

#ifdef _SIMULATION_
	printf("Starting gradient track!\n");
#endif

	// 采集第一组姿态信号数据
	APP_CurrDegree_Get_FP(ANG_POSITION, &Az_old_I);
	APP_CurrDegree_Get_FP(ANG_CABRAGE, &El_old_I);
	APP_Power_Get(&p_old_I);

	// 驱动电机作3s,5s

	if(APP_MotorStart(motor_position_id, Drec_anticlockwise_down)==FAIL) return FAIL;
	if(APP_MotorStart(motor_cabrage_id, Drec_clockwise_up)==FAIL)return FAIL;

#ifdef _SIMULATION_
	Sleep(300);
#else
	BSP_Delay_ms(1000);
#endif

	APP_MotorEnd(motor_position_id, Drec_anticlockwise_down);

#ifdef _SIMULATION_
	Sleep(200);
#else
	BSP_Delay_ms(500);
#endif

	APP_MotorEnd(motor_cabrage_id, Drec_clockwise_up);

#ifdef _SIMULATION_
	Sleep(1000);
#else
	BSP_Delay_ms(3000);
#endif

	// 采集第二组姿态信号数据
	APP_CurrDegree_Get_FP(ANG_POSITION, &Az_old);
	APP_CurrDegree_Get_FP(ANG_CABRAGE, &El_old);
	APP_Power_AG_Get_4_PURS(&p_old);

#ifdef _SIMULATION_
	printf("Gradient track:Az_old=%f,El_old=%f\n", Az_old, El_old);
#endif

	// 驱动电机作5s,3s

	if(APP_MotorStart(motor_position_id, Drec_clockwise_up)==FAIL) return FAIL;
	if(APP_MotorStart(motor_cabrage_id, Drec_anticlockwise_down)==FAIL) return FAIL;

#ifdef _SIMULATION_
	Sleep(300);
#else
	BSP_Delay_ms(1000);
#endif

	APP_MotorEnd(motor_cabrage_id, Drec_anticlockwise_down);

#ifdef _SIMULATION_
	Sleep(200);
#else
	BSP_Delay_ms(500);
#endif

	APP_MotorEnd(motor_position_id, Drec_clockwise_up);

#ifdef _SIMULATION_
	Sleep(1000);
#else
	BSP_Delay_ms(3000);
#endif

	// 采集第三组姿态信号数据
	APP_CurrDegree_Get_FP(ANG_POSITION, &Az_new);
	APP_CurrDegree_Get_FP(ANG_CABRAGE, &El_new);
	APP_Power_AG_Get_4_PURS(&p_new);

#ifdef _SIMULATION_
	printf("Gradient track:Az_new=%f,El_new=%f\n\n", Az_new, El_new);
#endif

	// 计算两组差值
	delta_Az_new = Az_new - Az_old + segma0;
	delta_El_new = El_new - El_old;
	delta_p_new = p_new - p_old;
	delta_Az_old = Az_old - Az_old_I;
	delta_El_old = El_old - El_old_I + segma0;
	delta_p_old = p_old - p_old_I;

	// 求导 （这里的 /256.0 需要修改，这个是过去的功率换算方式）???????
	d_Az_El = delta_Az_new*delta_El_old - delta_El_new*delta_Az_old;
	d_Az = (delta_p_new*delta_El_old - delta_p_old*delta_El_new) / d_Az_El;
	d_El = (delta_p_old*delta_Az_new - delta_p_new*delta_Az_old) / d_Az_El;
	// 归一化
	d_Az_n = d_Az / (FP32)sqrt(pow(d_Az, 2.0f) + pow(d_El, 2.0f));
	d_El_n = d_El / (FP32)sqrt(pow(d_Az, 2.0f) + pow(d_El, 2.0f));

#ifdef _SIMULATION_
	int i = 1;
#endif

	while (pow(d_Az, 2.0) + pow(d_El, 2.0) > segma_stop) // 判定模是否已到达停止门限
	{
		delta_Az = d_Az_n*delta_r;
		delta_El = d_El_n*delta_r;
		//delta_r = (FP32)sqrt(pow(delta_Az, 2.0f) + pow(delta_El, 2.0f));
		if (fabs(delta_Az) < 0.001373f) delta_Az = DEF_GRD_STEP_AZ;
		if (fabs(delta_El) < 0.001373f) delta_El = DEF_GRD_STEP_EL;
		if (adapt_target_angle(Az_new + delta_Az, El_new + delta_El, 0, 0, 0, 1) == FAIL)
		{
#ifdef _SIMULATION_
			printf("Gradient track end in FAIL!\n\n");
#endif
			return FAIL;
		}

		Az_old = Az_new;
		El_old = El_new;
		p_old = p_new;

		// 采集姿态信号数据
		if (APP_CurrDegree_Get_FP(ANG_POSITION, &Az_new) == FAIL
			|| APP_CurrDegree_Get_FP(ANG_CABRAGE, &El_new) == FAIL
			|| APP_Power_AG_Get_4_PURS(&p_new) == FAIL)
		{
			return FAIL;
		}
		
		if(step++>30)
		{
			return SUCC;
		}

		// 计算差值
		delta_Az_old = delta_Az_new;
		delta_El_old = delta_El_new + segma0;
		delta_p_old = delta_p_new;
		delta_Az_new = Az_new - Az_old + segma0;
		delta_El_new = El_new - El_old;
		delta_p_new = p_new - p_old;

		// 求导 （这里的 /256.0 需要修改，这个是过去的功率换算方式）???????
		d_Az_El = delta_Az_new*delta_El_old - delta_El_new*delta_Az_old;
		d_Az = (delta_p_new*delta_El_old - delta_p_old*delta_El_new) / d_Az_El;
		d_El = (delta_p_old*delta_Az_new - delta_p_new*delta_Az_old) / d_Az_El;
		// 归一化
		d_Az_n = d_Az / (FP32)sqrt(pow(d_Az, 2.0f) + pow(d_El, 2.0f));
		d_El_n = d_El / (FP32)sqrt(pow(d_Az, 2.0f) + pow(d_El, 2.0f));
#ifdef _SIMULATION_
		printf("Gradient track step=%d:\r\naz_mainlobe:%f,Az=%f\r\nel_mainlobe:%f,EL=%f\r\npower=%f\r\nmo=%f\r\n\n", i,
			ACUDevInfo.ACUParam.Old.az_opt_mainlobe, Az_new, ACUDevInfo.ACUParam.Old.el_opt_mainlobe, El_new, p_new, (float)(pow(d_Az, 2.0) + pow(d_El, 2.0)));
		i++;
#endif
	}

	// 更新pc，更新Azc，更新ELc
	if (APP_CurrDegree_Get_FP(ANG_CABRAGE, ELc) == FAIL)
	{
#ifdef _SIMULATION_
		printf("Gradient track end in FAIL!\n\n");
#endif
		return FAIL;
	}
	if (APP_CurrDegree_Get_FP(ANG_POSITION, Azc) == FAIL)
	{
#ifdef _SIMULATION_
		printf("Gradient track end in FAIL!\n\n");
#endif
		return FAIL;
	}
	if (APP_Power_AG_Get_4_PURS(pc) == FAIL)
	{
#ifdef _SIMULATION_
		printf("Gradient track end in FAIL!\n\n");
#endif
		return FAIL;
	}

#ifdef _SIMULATION_
	printf("Gradient track end!\n\n");
#endif
	return SUCC;
}

/***********************************************************
**name:			APP_DScale_Cross_Scan_Track
**describe: 		可变范围扫描追星
**input:	    none
**output:		Azc 对星后的方位，ELc 对星后的俯仰，pc 对星后的功率值
**return: 		执行结果
**autor:  		tao
**date:
************************************************************/
int APP_DScale_Cross_Scan_Track(FP32 *Azc, FP32 *ELc, FP32 *pc, FP32 pMax)
{
	FP32 delta_Az = 0.0;
	FP32 delta_EL = 0.0;
	FP32 Az0 = 0.0;
	FP32 EL0 = 0.0;
	FP32 limit = (FP32)ACUDevInfo.ACUParam.SeekSatParam.AimAtSat_Limit_mv/1000.0;

#ifdef _SIMULATION_
	printf("Starting scaled cross scan track!\n");
	int i = 1;
#endif

	// 初始化扫描范围
	getScanRadius(&delta_Az, *pc, pMax);
	delta_EL = delta_Az;

	if (cross_scan(delta_Az, delta_EL) == FAIL) return FAIL;

	if (APP_CurrDegree_Get_FP(ANG_POSITION, Azc) == FAIL)
	{
		return FAIL;
	}
	if (APP_CurrDegree_Get_FP(ANG_CABRAGE, ELc) == FAIL)
	{
		return FAIL;
	}
	if (APP_Power_AG_Get(pc) == FAIL)
	{
		return FAIL;
	}

#ifdef _SIMULATION_
	printf("Scaled cross scan track:step=%d\r\nAz_Scale=%f,EL_Scale=%f\r\naz_mainlobe=%f,Az=%f\r\nel_mainlobe=%f,EL=%f\r\nPower=%f!\r\n\n", i,
		delta_Az, delta_EL, ACUDevInfo.ACUParam.Old.az_opt_mainlobe, *Azc, ACUDevInfo.ACUParam.Old.el_opt_mainlobe, *ELc, *pc);
	i++;
#endif
	
	if(APP_Triangle_Track(Azc, ELc, pc)==FAIL) return FAIL;

	if (APP_NB(*pc, pMax, limit) == FALSE)
	{
		APP_Preliminary_Aim();
		return FAIL;
		/*if (APP_Preliminary_Aim() == FAIL)
		{
			return FAIL;
		}
		if (APP_Satellite_Cap_Scan(Azc, ELc, pc) == FAIL)
		{
			return FAIL;
		}
		if (APP_NB(*pc, pMax, limit) == FALSE)
		{
			return FAIL;
		}*/

#ifdef _SIMULATION_
		printf("Scaled cross scan track:step=%d\r\nAz_Scale=%f,EL_Scale=%f\r\naz_mainlobe=%f,Az=%f\r\nel_mainlobe=%f,EL=%f\r\nPower=%f!\r\n\n", i, 
			delta_Az, delta_EL, ACUDevInfo.ACUParam.Old.az_opt_mainlobe, *Azc, ACUDevInfo.ACUParam.Old.el_opt_mainlobe, *ELc, *pc);
		i++;
#endif
	}

#ifdef _SIMULATION_
	printf("Scaled cross scan track end!\r\n\n");
#endif

	return SUCC;
}

/***********************************************************
**name:			APP_Sat_GradientTrack
**describe: 		卫星追星主程序
**input:	    pc  对星完成后的功率值,作为目标值
**output:		none
**return: 		执行结果
**autor:  		tao
**date:
************************************************************/
int APP_Sat_GradientTrack()
{
	FP32 Az0 = 0;
	FP32 EL0 = 0;

	FP32 p0 = 0;
	FP32 p_dev = 0;
	FP32 p_opt = ACUDevInfo.ACUCurrSta.history_max_mv/1000.0;
	FP32 limit = (FP32)ACUDevInfo.ACUParam.SeekSatParam.AimAtSat_Limit_mv/1000.0;
	//FP32 p_save = 0;
	int i = 0;

	int chk_res = UNDERPOWER;
	
	ACUDevInfo.ACUCurrSta.AGC_max_mv = p_opt*1000.0;

#ifdef _SIMULATION_
	printf("Starting track!\n");
#endif

	rainFlag = 0;

	// 初始化Az0
	if (APP_CurrDegree_Get_FP(ANG_POSITION, &Az0) == FAIL)
	{
		return FAIL;
	}

	// 初始化EL0
	if (APP_CurrDegree_Get_FP(ANG_CABRAGE, &EL0) == FAIL)
	{
		return FAIL;
	}

	while (1) // ?????? ACUParam.Antennae.auto_follow_switch
	{
		if (rainFlag == 0)
		{
			BSP_Delay_s(1);
		}
		else
		{
			for(i=0;i<30;i++)
				BSP_Delay_s(30);
		}

		// 执行功率检测
		chk_res = APP_Satellite_Drift_Check(&p0, p_opt, limit); // 启动门限 由 系统最优AGC减去5dbm （1.25V）确定

		if (chk_res != UNDERPOWER)
		{
			/*if (APP_GE(p0, p_opt, 0.01f) == TRUE)
			{
				p_opt = p0;
				ACUDevInfo.ACUCurrSta.AGC_max_mv = p_opt*1000;
#ifdef _SIMULATION_
				printf("best agc : %f v\n",p_opt);
#endif
				if (p_opt > ACUDevInfo.ACUCurrSta.history_max_mv/1000.0)
				{
					p_save = p_opt*1000;
					APP_PARAM_HistoryMaxMv_Save(&p_save);
				}
			}
#ifdef _SIMULATION_
			else
			{
				printf("current agc : %f v\tbest agc : %f v\n", p0,p_opt);
			}
#endif*/
			rainFlag = 0;
			ACUDevInfo.ACUCurrSta.dev_work_sta = 0x31;
			continue;
		}

		/*抖动方差*/
		p_dev = p0;
		APP_Power_Deviation(&p_dev);
		if (p_dev > 0.5f)
		{
			BSP_Delay_s(30);
			continue;
		}

		ACUDevInfo.ACUCurrSta.dev_work_sta = 0x30;

		APP_LogFollowSatRecord(LOG_PURS_START);
		if (APP_NB(p0,p_opt,3.0)==TRUE)
		{
			// 在梯度追星处理范围内时使用梯度追星
			// if (APP_Triangle_Track(&Az0, &EL0, &p0) == FAIL) // APP_Triangle_Track APP_Gradient_Track
			if (APP_Gradient_Track(&Az0, &EL0, &p0) == FAIL) // APP_Triangle_Track APP_Gradient_Track
			{
				return FAIL;
			}
			if (APP_NB(p0, p_opt, limit) == TRUE)
			{
				rainFlag = 0;
			}
			else if (APP_GE(p_opt, p0, limit) == TRUE&&APP_LE(p_opt, p0, 2.5) == TRUE)
			{
				rainFlag = 1;
			}
			else
			{
#ifdef _SIMULATION_
				printf("Not CAP: power=%f\r\n\n", p0);
#endif
				if (APP_DScale_Cross_Scan_Track(&Az0, &EL0, &p0, p_opt) == FAIL)
					return FAIL;
				
				//if(APP_Triangle_Track(&Az0, &EL0, &p0)==FAIL) return FAIL;
			}
		}
		else
		{
			if (APP_DScale_Cross_Scan_Track(&Az0, &EL0, &p0, p_opt) == FAIL)
				return FAIL;
			//if(APP_Triangle_Track(&Az0, &EL0, &p0)==FAIL) return FAIL;
			/*仅测试
			//if (APP_Triangle_Track(&Az0, &EL0, &p0) == FAIL) // APP_Triangle_Track APP_Gradient_Track
			if (APP_Gradient_Track(&Az0, &EL0, &p0) == FAIL) // APP_Triangle_Track APP_Gradient_Track
			{
				return FAIL;
			} */
		}
		
		ACUDevInfo.ACUCurrSta.dev_work_sta = 0x31;
		/*if (APP_GE(p0, p_opt, 0.01f) == TRUE)
		{
			//p_opt = p0;
			//ACUDevInfo.ACUCurrSta.AGC_max_mv = p_opt*1000;
#ifdef _SIMULATION_
			printf("best agc : %f v\n", p0);
#endif
			if (p0 > ACUDevInfo.ACUCurrSta.history_max_mv/1000.0)
			{
				p_save = p0*1000;
				APP_PARAM_HistoryMaxMv_Save(&p_save);
			}
		}
#ifdef _SIMULATION_
		else
		{
			printf("current agc : %f v\tbest agc : %f v\n", p0, p_opt);
		}
#endif*/
		/*if (p0 > ACUDevInfo.ACUCurrSta.history_max_mv/1000.0)
		{
			p_save = p0*1000;
			APP_PARAM_HistoryMaxMv_Save(&p_save);
		}*/
		
		//p_opt = p0;
		//ACUDevInfo.ACUCurrSta.AGC_max_mv = p_opt*1000;
		APP_LogFollowSatRecord(LOG_PURS_END);
	}

	return SUCC;
}

/***********************************************************
**name:			APP_Satellite_Track_Start
**describe: 		启动自动控制程序
**input:		none
**output:		none
**return: 		none
**autor:  		tao
**date:
************************************************************/
int APP_Satellite_Track_Start()
{
	FP32 agc_max = 10.0f;
	FP32 agc_old_max = 0;
	FP32 agc_cur = 0;
	FP32 az = 0;
	FP32 el = 0;
	FP32 pl = 0;
	FP32 p_save = 0;

	ACUDevInfo.ACUCurrSta.dev_work_sta = 0x20;

	if (APP_Power_AG_Get(&agc_cur) == FAIL)
	{
		// 这里可能以后要记录日志
		return FAIL;
	}

	agc_old_max = ACUDevInfo.ACUCurrSta.history_max_mv/1000.0;

	if (APP_GE(agc_old_max, 0.0f ,2.0f) == TRUE)
	{
		agc_max = agc_old_max;
	}

	if (APP_NB(agc_max, agc_cur, 0.2) == FALSE)
	{
 		if (APP_NB(agc_max, agc_cur, 1.25) == FALSE)
		{
			if (APP_Preliminary_Aim() == FAIL) return FAIL;
			if (APP_Satellite_Cap_Scan(&az, &el, &agc_cur) == FAIL) return FAIL;
		}

		//APP_Gradient_Track(&az, &el, &agc_cur);

		if (ACUDevInfo.ACUParam.AntennaParam.elec_coder_num >= 3)
		{
			if (pl_scan() == FAIL) return FAIL;
		}
	}

	agc_max = agc_cur;
	if (agc_max > agc_old_max)
	{
		p_save = agc_max*1000;
		APP_PARAM_HistoryMaxMv_Save(&p_save);
	}
	ACUDevInfo.ACUCurrSta.AGC_max_mv = agc_max*1000;

	ACUDevInfo.ACUCurrSta.dev_work_sta = 0x21;

	//if (APP_Sat_GradientTrack(agc_max) == FAIL)
	//{
	//	// 这里可能以后要记录日志
	//	;
	//}
	return SUCC;
}
