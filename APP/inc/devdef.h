
#ifndef __DEV_DEF_H__
#define __DEV_DEF_H__


#ifdef	DEV_DEF
	#define	DEV_GLOBE
#else
	#define	DEV_GLOBE	extern
#endif


#include <bsp.h>

//#define	TEST_CODE_USE_FLAG


//  注意:  所有频率的单位以Khz为单位


//////////////////////////////////////////////////////////////////////////////////////////////
//本地宏定义

//设备工作状态
#define		WORK_ON			0
#define		WORK_OFF			1

//通信设备在线状态
#define		COMM_LINK			0
#define		COMM_UNLINK		1


//设备工作状态
#define 		WORKSTA_NORMAL	0
#define 		WORKSTA_ALARM	1

#define 	WARN_CLOSE_ID			0x00
#define 	WARN_BEACON_ID		0x01
	#define	WARN_BEACON_UNLINK			0x0101
	#define	WARN_BEACON_FREQ_SET_FAIL	0x0102
	#define	WARN_BEACON_OUTRANG_SET_FAIL	0x0104
#define 	WARN_GPS_ID			0x02
	#define 	WARN_GPS_UNLINK			0x0201
	#define 	WARN_GPS_NO_LOCATION	0x0202
#define 	WARN_INCL_ID			0x03
	#define 	WARN_INCL_UNLINK			0x0301
#define 	WARN_EEPROM_ID		0x04
	#define 	WARN_EEPROM_UNLINK		0x0401
#define 	WARN_SPIFLASH_ID		0x05
	#define 	WARN_SPIFLASH_UNLINK		0x0501



//////////////////////////////////////////////////////////////////////////////////////////////

//枚举类型定义

//通信状态枚举
typedef enum _enumCOMMSta_
{
	NORMAL=0,		
	SET_FAIL=1,
	QUE_FAIL=2,
	NO_CMD=4,
	COMM_CHECK_ERR=8,
} enumCOMMSta;

// 命令ID
typedef enum _enumCOMMCmdID_
{
	// 设置命令ID
	SatParamSetID = 0x0101,				// 修改/增添卫星信息参数
	AntennaParamSetID,					// 天线信息参数设置
	AntennaParamAdjustID,				// 天线参数校准
	LocalParamSetID,					// 地球站位置信息手动设置
	LocalParamSrcSelSetID,				// 地球站位置信息源选择设置
	
	SysDateTimeSetID,					// 设备系统日期时间手动设置
	SysDateTimeSrcSelSetID,				// 设备系统日期时间来源选择

	SatDelID,							// 删除卫星信息命令
	AimAtSatParamSetID=0x010A,				// 追星参数设置
	AntennaTurnDirAdjustBeginSetID = 0x010B,	// 启动天线方向转向校准
	AntennaTurnDirAdjustFinishSetID,	// 完成天线方向转向校准
	AntennaHardWareLimitSetID  = 0x010E,	// 天线硬限位校准

	// 设备控制命令ID

	AimAtSatModeSelID = 0x0201,			// 对星模式选择
	AntennaRecordID,					// 天线位置记录
	AntennaSigleStepCtrID,				// 天线手动单步控制
	AntennaDegreeCtrID,					// 天线手动角度控制
	AntennaRecordGotoID = 0x0206,					// 前往天线位置记录
	AntennaRecordDelID = 0x0207,					//删除天线记录位置
	AGCRecordID = 0x0209,					//记录AGC值
	SatSelID = 0x0211,							// 选卫星设置
	MotorSoftLimitSetID = 0x0212,					// 电机软限位设置
	NetWorkSetID= 0x0215,				// 网络设置
	PCHighAdminID = 0x021A,				// PC机权限
	
	FactoryResetID = 0x0221,				// 恢复出厂设置

	// 查询命令ID
	CurrSatParamQueID = 0x0801,			// 当前选中目标卫星信息参数查询
	AllSaveSatParamQueID,				// 当前存储卫星信息参数查询
	AntennaParamQueID,				// 当前天线信息参数查询
	AntennaAdjustParamQueID,			// 当前天线校准参数查询
	LocalParamQueID,					// 当前地球站位置信息查询
	DevSysDateTimeQueID,				// 当前设备系统日期时钟查询
	NetWorkQueID,						// 网络参数查询
	
	AimAtSatTheoreticalDegreeQueID = 0x080A,	// 当前设备对星理论角查询
	AntennaRecordQueID = 0x080B,				// 设备天线位置记录查询
	MotorSoftLimitQueID = 0x080C,		//软限位查询
	AimAtSatModeQueID,					// 对星模式查询
	AimAtSatParamQueID,				// 追星参数查询
	DevStaQueID = 0x0810,				// 设备状态信息查询
	DevAdminQueID = 0x0811,			// 设备控制状态查询
	CurrWarnStaQueID = 0x0812,				// 当前设备告警信息查询

	// 设备日志查询
	WarnLogQueID = 0x0901,				// 设备告警日志查询
	OperationLogQueID,					// 操作日志查询

	VersionQueID = 0x0A01,				// 版本查询

}enumCOMMCmdID;


//电机编号枚举
typedef enum _enumMotorIDSta_
{
	motor_position_id =1,		
	motor_cabrage_id,	
	motor_polar_id,	
} enumMotorIDSta;

/////////////////////////////////////////////////////////////////////////////////////////////

//消息的一些结构体

#define	BUZZER_Q_ID			0x01
#define	HORLED_Q_ID			0x02
#define	NIXIELIGHT_Q_ID		0x03

typedef	struct _strBuzzerQ_
{
	INT8U 	id;
	INT8U	len;
	INT8U	on_off;				// 
	INT8U	flick_flag;			// 是否闪烁式鸣叫标记,0为不闪烁叫，1为闪烁叫
	INT16U	period_ms;			// 鸣叫间隔时间
	INT16U	beep_cnt;			// 鸣叫次数
}strBuzzerQ;


//////////////////////////////////////////////////////////////////////////////////////////////


//PC通信状态结构体
typedef struct  _strPCCommStat_
{
	INT8U	link_stat;
	INT8U 	comm_stat;
	INT8U	mcu_busy;
	INT32U 	dst_IP_addr;		// 目标PC IP地址
	INT32U 	dst_IP_port;			// 目标PC 机通信端口号
}strPCCommStat;

//PDU通信状态结构体
typedef struct  _strPDUCommStat_
{
	INT8U	link_stat;
	INT8U 	comm_stat;
	INT8U	mcu_busy;
}strPDUCommStat;

// 设备网络相关结构体
typedef struct  _strNetWork_
{
	INT32U	ip_addr;
	INT32U 	mask;
	INT32U	gate;
	INT8U 	mac[6];
	INT32U 	ip_port;
}strNetWork;


//信标机相关子结构体
//信标机配置信息结构体
typedef struct  _strBeaconConf_
{
	INT8U	comm_interface;			//通信接口:  默认为0，串口通信
	INT32U 	comm_rate;				// 通信速率
	INT32U	lo_freq;					// 本振频率
	INT32U	work_freq;				// 工作频点
	INT8U	agc_output_range;			
}strBeaconConf;

//信标机状态信息结构体
typedef struct  _strBeaconStat_
{
	INT8U	link_stat;				// 连接状态 :  COMM_LINK    COMM_UNLINK
	INT32U 	dev_stat;				// 设备状态  WORKSTA_NORMAL   WORKSTA_ALARM
	INT32U	comm_stat;					// 通信协议状态 enumCOMMSta
}strBeaconStat;

//信标机数据信息结构体
typedef struct  _strBeaconData_
{
	INT32U	data_dbm;				// dbm单位数据
	INT32U 	data_mv;				// 毫伏单位数据
}strBeaconData;


//GPS相关子结构体
//GPS配置信息结构体
typedef struct  _strGPSConf_
{
	INT8U	comm_interface;			//通信接口:  默认为0，串口通信
	INT32U 	comm_rate;				// 通信速率
	
}strGPSConf;

//GPS状态信息结构体
typedef struct  _strGPSStat_
{
	INT8U	link_stat;				// 连接状态 :  COMM_LINK    COMM_UNLINK
	INT32U 	dev_stat;				// 设备状态  WORKSTA_NORMAL   WORKSTA_ALARM
	INT32U	comm_stat;					// 通信协议状态 enumCOMMSta
}strGPSStat;

//GPS数据信息结构体
typedef struct  _strGPSData_
{
	INT16U	year;	
	INT8U	month;	
	INT8U	day;
	INT8U	hour;
	INT8U	min;
	INT8U	sec;
	INT16U   ms;
	INT8U	lat_flag;					// 南北纬标识，'N'北纬，'S'南纬
	INT32U		lat;						//  实际纬度值的1000倍，即精确到0.001度，
	INT8U	lon_flag;					// 东西经标识，'E'东经    'W'西经
	INT32U		lon;						//  实际经度值的1000倍，即精确到0.001度
	INT32U		height;					// 以米为单位
	INT8U valid;
}strGPSData;


//电子码盘相关子结构体
//电子码盘配置信息结构体
typedef struct  _strEleccoderConf_
{
	INT8U	comm_interface;			//通信接口:  默认为0，串口通信
	INT32U 	comm_rate;				// 通信速率
	INT8U	addr;					// 地址
	INT8U 	bit_num;				// 码盘位数
	INT8U	count_mode;			// 计数模式，0xE1表示顺时针加计数，0xE2表示顺时针减计数
}strEleccoderConf;

//电子码盘状态信息结构体
typedef struct  _strInclinometerStat_
{
	INT8U	link_stat;				// 连接状态 :  COMM_LINK    COMM_UNLINK
	INT32U 	dev_stat;				// 设备状态  WORKSTA_NORMAL   WORKSTA_ALARM
	INT32U	comm_stat;					// 通信协议状态 enumCOMMSta
	INT8U	comm_idle;
}strEleccoderStat;

//电子码盘数据信息结构体
typedef struct  _strInclinometerData_
{
	INT32U	data;				// 原始数据
	FP32		degree;				// 原始数据转化后的角度数(0~360度)

}strEleccoderData;


// 电子码盘与实际角度映射关系
typedef struct  _strElecCoderMapAntenna_
{
	INT8U			ElecCoder_count_mode;	// 		当初指定映射关系时的电子码盘的计数模式
	FP32				ElecCoder_degree;		// 		当初指定映射关系时的电子码盘度数
	FP32				Antenna_degree;		// 		当初指定映射关系时的天线实际指向度数
}strElecCoderMapAntenna;

#define	ANTENNA_DIR_LEFT_DOWN_ANTICLOCK	0x01
#define	ANTENNA_DIR_RIGHT_UP_CLOCK			0x02

#define	ANTENNA_DIR_LEFT_DOWN_ANTICLOCK_LIMIT		0x01
#define	ANTENNA_DIR_RIGHT_UP_CLOCK_LIMIT			0x02

#define	MOTOR_DIR_ANTICLOCK					0x01
#define   MOTOR_DIR_CLOCK						0x02

// 电机转动方向与天线转动方向映射关系
typedef struct  _strAntennaDirMapMotor_
{
	INT8U 		antenna_part;		// 0x01表示方位，0x02表示俯仰，0x03表示极化
	INT8U 		motor_dir[2];		// 方位时，组号0表示左转时的电机方向，组号1表示右转时的电机方向
									// 俯仰时，组号0表示下转是的电机方向，组号1表示上转是的电机方向
									// 极化，组号0表示逆时针时的电机方向，组号1表示顺时针时的电机方向
}strAntennaDirMapMotor;

//////////////////////////////////////////////////////////////////////////////////////////////

// ACU的相关三级子结构体

// 信标机结构体
typedef struct  _strBeaconInfo_
{
	strBeaconConf 	BeaconConf;
	strBeaconStat		BeaconStat;
	strBeaconData	BeaconData;
}strBeaconInfo;

// GPS结构体
typedef struct  _strGPSInfo_
{
	strGPSConf 	GPSConf;
	strGPSStat	GPSStat;
	strGPSData	GPSData;
}strGPSInfo;

// 电子码盘结构体
typedef struct  _strEleccoderInfo_
{
	strEleccoderConf 	EleccoderConf;
	strEleccoderStat	EleccoderStat;
	strEleccoderData	EleccoderData;
}strEleccoderInfo;

// EEPROM结构体
typedef struct  _strEEPROMInfo_
{
	INT32U		total_size;
	INT8U		link_stat;
	INT8U		comm_stat;
}strEEPROMInfo;


// SPI FLASH结构体
typedef struct  _strSPIFlashInfo_
{
	INT32U		total_size;
	INT32U		sector_size;
	INT8U		link_stat;
	INT8U		comm_stat;
}strSPIFlashInfo;

// Buzzer结构体
typedef struct  _strBuzzerInfo_
{
	INT8U		buzzer_switch;
	INT8U		on_off_status;
	INT8U 		flick_flag;
	INT16U 		beep_ms;
	INT16U 		period_ms;
	INT32U 		beep_cnt;
}strBuzzerInfo;



//卫星参数
typedef struct  _strSatParam_
{
	INT8U		id;						// 卫星ID
	INT8U 		name[16];				// 卫星名称
	INT8U		lon_flag;				//卫星经度标示  ,'E'  'W'
	FP32			lon;						// 卫星经度
	INT32U		freq;					// 工作波段
	INT8U		polar_type;				//  极化方式
}strSatParam;

//地球站参数
typedef struct  _strLocalPositionParam_
{
	INT8U		lon_flag;				// 本地位置经度标示
	FP32			lon;						// 经度
	INT8U		lat_flag;					// 本地位置纬度标示
	FP32			lat;						// 纬度
	INT32U		height;					// 高度    以米(m)为单位
	INT8U		valid_flag;				// 有效性
	INT8U		data_src_flag;			// 数据源，0x00 GPS  0x01  人工输入
}strLocalPositionParam;

//地球站手动设置参数
typedef struct  _strHandLocalPositionParam_
{
	INT8U		lon_flag;				// 本地位置经度标示
	FP32		lon;						// 经度
	INT8U		lat_flag;					// 本地位置纬度标示
	FP32		lat;						// 纬度
	INT32U		height;					// 高度    以米(m)为单位

	INT8U 		valid_flag;				// 0x01 表示有效，0x00表示无效

}strHandLocalPositionParam;

//天线参数
typedef struct  _strAntennaParam_
{
	INT8U		type;		// 天线型号，  正馈和偏馈正装、偏馈倒装3种  0:正馈  1: 偏馈正装   2: 偏馈倒装
	INT32U		offset_angle;	// 如果是偏馈类型天线，则需要知道偏焦角
	INT8U 		caliber;			// 天线口径，总共5中，从1 到 5
	INT32U		lo_freq;
	INT8U		gain_range_flag;		// 天线增益范围标志

	INT8U		elec_coder_num;		// 电子码盘数量

	FP32			motor_cabrage_start;	// 俯仰电机范围起始角度值
	FP32			motor_cabrage_end;		// 俯仰电机范围结束角度值

	FP32			motor_position_start;	// 方位电机范围起始角度值
	FP32			motor_position_end;		// 方位电机范围结束角度值

	FP32			motor_polar_start;		// 极化电机范围起始角度值
	FP32			motor_polar_end;		// 极化电机范围借宿角度值
	
	FP32			aim_cabrage;			// 目标俯仰角理论值
	FP32			aim_position;			// 目标方位角理论值
	FP32			aim_polar;				// 目标极化角理论值

	FP32			aim_cabrage_hand;			// 目标俯仰角理论值手动模式
	FP32			aim_position_hand;			// 目标方位角理论值手动模式
	FP32			aim_polar_hand;				// 目标极化角理论值手动模式

	strAntennaDirMapMotor AntennaDirMapMotor[3];		
}strAntennaParam;

// 设备告警结构体
typedef struct  _strDevAlarm_
{
	INT8U		alarm_cnt;		// 告警数目
	INT8U		alarm_refresh;		// 告警更新
	INT32U		*alarm_ptr;		// 告警缓存指针
}strDevAlarm;


// 设备管理权限结构体
typedef struct  _strDevAdmin_
{
	INT8U		admin_sta;		// 管理权限状态// 设备管理控制状态，0x00表示ACU主控，0x01表示PC主控，0x02表示PDU主控。
	INT8U		mode_sta;		// 模式状态，分自动模式和手动模式和空闲，0x00空闲，0x01手动，0x02自动
								// PDU模式下，默认就是手动模式；
								// PC 和ACU控制下，可选择是否自动或者手动
	INT8U 		stop_flag;		//  急停标志0x01表示停止一切活动
	INT8U		pc_admin_deal_sta;		// PC机申请权限的处理状态进度
										//0x01 表示PC申请权限等待OLED的用户答复中
										// 0x00, 表示没有申请或者已经得到答复 
	INT32U		pc_admin_ip_addr;		// pc 主控时，当前pc 的IP地址
	INT32U		pc_admin_ask_ip_addr;		//申请pc 控制权限的IP地址
	INT32U		pc_admin_link_cnt;		// pc 主控时，pc 的link 计数
}strDevAdmin;


// 全部的电子码盘与天线各部位的映射关系 
// 电子码盘与实际角度映射关系
typedef struct  _strAllElecCoderMapAntenna_
{
	strElecCoderMapAntenna		ElecCoderMapAntenna[3];
	INT8S	SatPolarType;							// 校准时的卫星极化方式，对应的计划角度加减标志，如水平极化则为0，
																	// 垂直极化若目标角度值加了90度，则为2，减了90度则为1.
}strAllElecCoderMapAntenna;


// 追星参数
typedef struct  _strSeekSatParam_
{
	INT8U	SeekSat_Mode;				// 对星模式，0x01手动，0x02自动，0x00停止，0x03,跟踪模式，0x04前往姿态记录位置
	INT8U	Urgent_Stop;				//  紧急停止，0x01紧急停止，0x00正常。
	INT8U 	ScanRange_param[3];		//  扫描范围参数
	INT32U 	AimAtSat_Limit_mv;			// 启动追星的最低门限值，无符号，
	
}strSeekSatParam;


typedef struct _strLogFlash_
{
	INT8U head;
	INT32U currlog_size;
	INT32U currlog_index;
	INT8U   currlog_date_time[7];
	INT8U   *currlog_buff;
	INT32U beforelog_addr;
	INT32U beforelog_index;
	INT32U beforelog_aize;
	INT8U   checksum8bit;
	INT8U tail;
	
}strLogFlash;

typedef struct _strLogInfo_
{
	INT32U log_sys_addr;		//不需要存储，每次开机后自动初始化
	INT32U log_total_num;
	INT32U currlog_addr;				// 当前存储的log地址
	INT32U currlog_size;
	INT32U currlog_index;
	INT32U beforelog_addr;			// 之前已经存储好的LOG地址
	INT32U beforelog_index;
	INT32U beforelog_size;
}strLogInfo;

//////////////////////////////////////////////////////////////////////////////////////////////

//ACU的相关二级子结构体

//设备状态与配置信息结构体
typedef struct  _strDeviceInfo_
{
	INT8U				dev_work_stat;
	strPCCommStat		PCCommStat;
	strPDUCommStat		PDUommStat;
	strNetWork			NetWork;
	strBeaconInfo			BeaconInfo;
	strGPSInfo			GPSInfo;
	strHandLocalPositionParam	HandLocalPositionParam;
	strEleccoderInfo 		EleccoderInfo[3];
	strEEPROMInfo		EEPROMInfo;
	strSPIFlashInfo		SPIFlashInfo;
	strBuzzerInfo			BuzzerInfo;
	strAllElecCoderMapAntenna	AllElecCoderMapAntenna;
}strDeviceInfo;

//对星参数信息结构体
typedef struct  _strACUSeekSatParam_
{
	strSatParam 			CurrSatParam;
	strLocalPositionParam	LocalPositionParam;
	strAntennaParam		AntennaParam;
	strSeekSatParam		SeekSatParam;
}strACUSeekSatParam;


#define	SAT_MAX_NUM		100
//卫星列表结构体
typedef struct  _strSATList_
{
	INT16U				Sat_Total_num;		// 存储的卫星总数
	INT8U				Sat_ID[SAT_MAX_NUM];
	INT8U 				SatList_Refresh_flag;	//  0x01表示列表有更新，0x00表示列表无更新
}strSATList;


//天线位置记录列表结构体
#define	ANTENNA_RECORD_MAX_NUM		10
typedef struct  _strAntennaRecordList_
{
	INT32U				record_Total_num;		// 存储的卫星总数
	INT8U				record_ID[ANTENNA_RECORD_MAX_NUM];
}strAntennaRecordList;


//acu当前状态结构体
typedef struct  _strSATseekerCurrSta_
{
	FP32 				beacon_rxpower_db;
	FP32 				beacon_rxpower_mv;

	FP32 				history_max_mv;

	

	FP32					antenna_position;		// 天线方位角
	FP32					antenna_cabrage;		// 天线俯仰角
	FP32					antenna_polar;			// 天线极化角
	
	INT8U				antenna_position_sta;			//  	见ACU与PDU通信协议3.3.1
	INT8U				antenna_cabrage_sta;			//  	见ACU与PDU通信协议3.3.2
	INT8U				antenna_polar_sta;				//  	见ACU与PDU通信协议3.3.3
	
	INT8U 				dev_admin_sta;			// 设备管理控制状态，0x00表示ACU主控，0x01表示PC主控，0x02表示PDU主控, 0x11表示申请PC主控权限状态。

	INT8U 				dev_move_sta;			// 设备运动状态
	
	INT8U 				dev_work_sta;			// 设备工作状态
	
	INT8U 				dev_alarm_flag;			// 设备告警标志
	INT8U 				dev_alarm_reflash_flag[2];	//  设备告警更新标志		组号0表用于PC，组号1用于OLED
	INT8U 				dev_alarm_buff[6];
						// bit位置1表示故障告警，0表示正常
						//	组号0:  电子码盘故障， bit0方位码盘，bit1俯仰码盘，bit2计划码盘
						//	组号1:  信标机，bit0信标机通信故障，bit1信标机频率写入失败，bit2增益范围设置失败
						//   组号2:  主控板，bit0EEPROM故障，bit1flash故障
						//  组号3:  PDU通信故障
						// 组号4:	GPS故障，bit0GPS通信故障，bit1GPS定位失败
						// 组号5:	电机告警:	bit0俯仰电机上限告警，bit1俯仰电机下限告警
						//							bit2方位电机左限位告警，bit3方位电机右限位告警
						//							bit4极化电机顺限位告警，bit5极化电极逆限位告警

	INT8U 				dev_param_reflash_flag[2];		//  设备参数更新标志，组号0表用于PC，组号1用于OLED

	strDevAdmin			DevAdmin;
	strDevAlarm			DevAlarm;

	FP32 				AGC_max_mv;				// 当前最优值
	INT8U 				motor_move[3];				// 组号0鍉组号2分别表示  方位电机、俯仰电机以及极化电机；
	INT8U 				motor_limit_sta[3];			// 组号0鍉组号2分别表示  方位电机、俯仰电机以及极化电机；
													//电机运动状态，0x00表示停止运动，ANTENNA_DIR_LEFT_DOWN_ANTICLOCK 和 ANTENNA_DIR_RIGHT_UP_CLOCK
	INT32S					temperature;			//  ACU温度
}strACUCurrSta;


// 版本结构体
typedef struct  _strVersion_
{
	INT8U 			hardver[16];
	INT8U			softver[16];
	INT8U			datetime[6];		
}strVersion;

// 系统时间
typedef struct  _strSysDateTime_
{
	INT16U 			year;
	INT8U			month;
	INT8U			day;
	INT8U 			hour;
	INT8U 			min;
	INT8U 			senc;
	INT16U 			ms;

	INT32U 			systick;

	INT32U 			src_flag;			// 时间源， 0x00表示自动获取， 0x01表示手动设置，0x02表示GPS获取
}strSysDateTime;



//////////////////////////////////////////////////////////////////////////////////////////////
// ACU的相关一级结构体
typedef struct  _strSATseekerDevInfo_
{
	strDeviceInfo  		DeviceInfo;
	strACUSeekSatParam		ACUParam;
	strSATList			SATList;		
	strAntennaRecordList	AntennaRecordList;
	strACUCurrSta		ACUCurrSta;
	strSysDateTime		SysDateTime;
	strVersion			Version[2];				// 数组0表示ACU的版本，数组1表示PDU的版本

}strACUDevInfo;


typedef struct _strPCCOMMPro_
{
	INT8U head;
	INT8U datalen;
	INT8U *databuff;
	INT8U checksum;
	INT8U end;

	INT32U ip_addr;
	INT32U port;	
}strPCCOMMPro;

DEV_GLOBE	strACUDevInfo		ACUDevInfo;
DEV_GLOBE	strLogInfo OptLogInfo, WarnLogInfo;	

DEV_GLOBE INT8U Task_Run_Flag[3];		// 组号0表示对星任务，组号1表示追星任务，组号2表示前往记录位置任务
#endif

