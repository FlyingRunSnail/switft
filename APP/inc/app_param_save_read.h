

#ifdef APP_PARAM_DEF
	#define APP_PARAM_GLOBE
#else
	#define APP_PARAM_GLOBE		extern
#endif

#include <devdef.h>


// 天线参数储存地址，12个字节
// 存储数据天线类型（1B）+ 偏焦角（4B）+ 天线口径（1B）+ LNB本振（4B）+ AGC输出范围（1B）+ 码盘数量(1B)
#define		APP_EEPROM_Antenna_Param_ADDR						0x0000


// 历史对星最优电压值储存地址，4个字节
#define		APP_EEPROM_HISTORY_MAX_MV_ADDR						0x000C

// 电子码盘与天线角度映射值存储地址,   24个字节
// 天线对星方位角度值(4B) + 方位码盘角度值(4B) +  码盘计数模式(1B)
// 天线对星俯仰角度值(4B) +俯仰码盘角度值(4B)  + 码盘计数模式(1B)
// 天线对星极化角度值(4B) + 极化码盘角度值(4B) + 码盘计数模式(1B)
#define		APP_EEPROM_ElecCode_Map_AntennaDegree_Addr			0x0010

// 地球站本地信息参数源选择信息存储地址， 1个字节
#define		APP_EEPROM_LocallParamSrc_ADDR						0x0030

// 地球站本地信息参数最近一次手动设置的信息存储地址， 14个字节
// 本地经度标识（1B）+本地经度（4B）+ 本地纬度标识（1B）+ 本地纬度（4B）+ 本地海拔（4B）
#define		APP_EEPROM_LocallParam_ADDR							0x0031

// 系统日期时钟数据源选择信息存储地址， 1个字节
#define		APP_EEPROM_DevSysDateTimeSrc_ADDR						0x0070

// 对星模式存储,  1个字节
#define		APP_EEPROM_AimAtSatMode_ADDR							0x0074

// 对星工作状态,  1个字节
#define		APP_EEPROM_AimAtSatWorkSta_ADDR							0x0075

// 追星参数最低门限存储，4个字节
#define		APP_EEPROM_AimAtSatLimitMv_ADDR							0x0078

// 网络设置存储地址， 16个字节
// IP地址（4B）+ 子网掩码（4B）+网关（4B）+  端口号(4B)
#define		APP_EEPROM_NetWork_ADDR									0x0080

// MAC 存储地址， 6个字节
#define		APP_EEPROM_MAC_ADDR										0x0090

// 方位电机硬限位信息存储地址， 8个字节，左限位码盘数据（4B）+ 右限位码盘数据（4B）
#define		APP_EEPROM_PositionHardLimit_ADDR							0x00A0

// 俯仰电机硬限位信息存储地址， 8个字节，下限位码盘数据（4B）+ 上限位码盘数据（4B）
#define		APP_EEPROM_CabrageHardLimit_ADDR							0x00B0

// 极化电机硬限位信息存储地址， 8个字节，逆时针限位码盘数据（4B）+ 顺时针限位码盘数据（4B）
#define		APP_EEPROM_PolarHardLimit_ADDR							0x00C0

// 天线转动方向与电机转动方向映射关系,6字节
// 天线方位左转动时方位电机转动方向(1B) +天线方位右转动时方位电机转动方向(1B) +
//俯仰下转动时 俯仰电机转动方向(1B)+俯仰上转动时 俯仰电机转动方向(1B)+
// 极化逆时针转动时 极化电机转动方向(1B) + 极化顺时针转动时极化电机转动方向(1B)
#define		APP_EEPROM_AntennaDir_Map_MotorDir_ADDR					0x00D0

// 电子码盘的计数模式
#define		APP_EEPROM_ELECCODER_COUNT_MODE_ADDR					0x00DA


// 当前开机时间暂存( 7个字节)
#define		APP_EEPROM_Dev_PowerOn_SysDateTime_ADDR					0x00E0


// 天线位置记录信息存储总数, 1个字节 
#define		APP_EEPROM_AntennaRecord_TotalNum_ADDR							0x0100

// 天线位置记录信息存储基础地址，(最高只支持10条记录)
// 每一条天线位置记录, 记录电子码盘的实际度数，以及相关的卫星数据，总38字节
// 卫星名称（16B）+ 卫星经度标识（1B）+卫星经度（4B）+ 信号频率（4B）+ 极化方式（1B）+
// 方位电子码盘度数(4B) + 俯仰电子码盘度数(4B) + 极化电子码盘度数(4B)
#define		APP_EEPROM_AntennaRecord_Base_ADDR							0x0110
	#define	APP_EEPROM_AntennaRecord_Offset_ADDR						0x0050



// 设备操作日志总数， 4个字节
#define		APP_EEPROM_OPTLog_TotalNum_ADDR						0x0800

// 当前操作日志编号数，4个字节
#define		APP_EEPROM_OPTLog_CurrID_ADDR							0x0804

// 设备告警日志总数， 4个字节
#define		APP_EEPROM_WarnLog_TotalNum_ADDR						0x0810

// 当前告警日志编号数，4个字节
#define		APP_EEPROM_WarnLog_CurrID_ADDR							0x0814


// 因为以太网冲连接重启标志
#define		APP_EEPROM_Reset_Flag_ADDR							0x0820



// 卫星总数存储地址， 4个字节
#define		APP_EEPROM_Sat_TotalNum_ADDR			0x0A00			

// 当前卫星ID号存储地址，1个字节
#define		APP_EEPROM_Sat_CurrID_ADDR				0x0A08				//  当前选中卫星ID存储地址

// 当前卫星参数存储地址
#define		APP_EEPROM_CurrSatParam_ADDR				0x0A20				//  

// 卫星参数存储起始地址,// 0x1010  ~ 0x3010之间为卫星参数存储的地方,
// 每条卫星参数存储27个字节，
//卫星编号（1B）+ 卫星名称（16B）+ 卫星经度标识（1B）+卫星经度（4B）+ 信号频率（4B）+ 极化方式（1B）
#define		APP_EEPROM_SatParam_Base_ADDR			0x0A40				// 卫星参数存储起始地址
	#define		APP_EEPROM_SatParam_OffSet_ADDR	0x0020				//  偏移地址






APP_PARAM_GLOBE void APP_PARAM_Init(void);

APP_PARAM_GLOBE int APP_PARAM_Save(INT16U addr,	INT8U *savebuff, INT16U len);

APP_PARAM_GLOBE int APP_PARAM_Read(INT16U addr,	INT8U *Readbuff, INT16U len);


APP_PARAM_GLOBE int APP_PARAM_AntennaParam_Save(strAntennaParam *AntennaParam);

APP_PARAM_GLOBE int APP_PARAM_ElecCode_Map_AntennaDegree_Save(strAllElecCoderMapAntenna *AllElecCoderMapAntenna);

APP_PARAM_GLOBE int APP_PARAM_LocallParamSrc_Save(INT8U src_flag);

APP_PARAM_GLOBE int APP_PARAM_LocallHandParam_Save(strHandLocalPositionParam *HandLocalPositionParam);

APP_PARAM_GLOBE int APP_PARAM_DevSysDateTimeSrc_Save(INT8U src_flag);

APP_PARAM_GLOBE int APP_PARAM_AimAtSatMode_Save(INT8U mode);

APP_PARAM_GLOBE int APP_PARAM_NetWork_Save(strNetWork		*NetWork);

APP_PARAM_GLOBE int APP_PARAM_MAC_Save(strNetWork		*NetWork);

APP_PARAM_GLOBE int APP_PARAM_HardWareLimit_Save(INT8U antenna_part,INT8U antenna_limit_part,INT32U limit_data);

#if 0
APP_PARAM_GLOBE int APP_PARAM_PositionSoftLimit_Save(strAntennaParam		*AntennaParam);

APP_PARAM_GLOBE int APP_PARAM_CabrageSoftLimit_Save(strAntennaParam		*AntennaParam);

APP_PARAM_GLOBE int APP_PARAM_PolarSoftLimit_Save(strAntennaParam		*AntennaParam);
#endif

APP_PARAM_GLOBE int APP_PARAM_AntennaRecord_TotalNum_Save(INT32U total_num);

APP_PARAM_GLOBE int APP_PARAM_AntennaRecord_Save(INT8U id,INT8U  *buff, INT16U len);

APP_PARAM_GLOBE int APP_PARAM_AntennaRecord_AllClear(void);

APP_PARAM_GLOBE int APP_PARAM_OPTLog_TotalNum_Save(INT32U total_num);

APP_PARAM_GLOBE int APP_PARAM_OPTLog_CurrID_Save(INT32U id);

APP_PARAM_GLOBE int APP_PARAM_WarnLog_TotalNum_Save(INT32U total_num);

APP_PARAM_GLOBE int APP_PARAM_WarnLog_CurrID_Save(INT32U id);

APP_PARAM_GLOBE int APP_PARAM_SatTotalNum_Save(INT16U total_num);

APP_PARAM_GLOBE int APP_PARAM_SatCurrID_Save(INT8U curr_id);

APP_PARAM_GLOBE int APP_PARAM_SatParam_Save(strSatParam *SatParam);

APP_PARAM_GLOBE int APP_PARAM_Sat_Del(strSatParam *SatParam);

APP_PARAM_GLOBE int APP_PARAM_AimAtSatParamLimitmv_Save(strSeekSatParam  *SeekSatParam);

APP_PARAM_GLOBE int APP_PARAM_HistoryMaxMv_Save(FP32	*history_max_mv);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

APP_PARAM_GLOBE int APP_PARAM_AntennaParam_Read(strAntennaParam *AntennaParam);

APP_PARAM_GLOBE int APP_PARAM_ElecCode_Map_AntennaDegree_Read(strAllElecCoderMapAntenna *AllElecCoderMapAntenna);

APP_PARAM_GLOBE int APP_PARAM_LocallParamSrc_Read(INT8U *src_flag);

APP_PARAM_GLOBE int APP_PARAM_LocallHandParam_Read(strHandLocalPositionParam *HandLocalPositionParam);

APP_PARAM_GLOBE int APP_PARAM_DevSysDateTimeSrc_Read(INT8U *src_flag);

APP_PARAM_GLOBE int APP_PARAM_AimAtSatMode_Read(INT8U *mode);

APP_PARAM_GLOBE int APP_PARAM_NetWork_Read(strNetWork		*NetWork);

APP_PARAM_GLOBE int APP_PARAM_MAC_Read(strNetWork		*NetWork);

APP_PARAM_GLOBE int APP_PARAM_HardWareLimit_Read(INT8U antenna_part,INT8U antenna_limit_part, INT32U *limit_data);

#if 0
APP_PARAM_GLOBE int APP_PARAM_PositionSoftLimit_Read(strAntennaParam		*AntennaParam);

APP_PARAM_GLOBE int APP_PARAM_CabrageSoftLimit_Read(strAntennaParam		*AntennaParam);

APP_PARAM_GLOBE int APP_PARAM_PolarSoftLimit_Read(strAntennaParam		*AntennaParam);
#endif

APP_PARAM_GLOBE int APP_PARAM_AntennaRecord_TotalNum_Read(INT32U *total_num);

APP_PARAM_GLOBE int APP_PARAM_AntennaRecord_Read(INT8U id,	INT8U *p);

APP_PARAM_GLOBE int APP_PARAM_AntennaRecord_Read2Struct(INT8U id,	strSatParam *SatParam, strAntennaParam *AntennaParam, FP32 *degree_AZ,FP32 *degree_EL,FP32 *degree_PL);

APP_PARAM_GLOBE int APP_PARAM_OPTLog_TotalNum_Read(INT32U *total_num);

APP_PARAM_GLOBE int APP_PARAM_OPTLog_CurrID_Read(INT32U *id);

APP_PARAM_GLOBE int APP_PARAM_WarnLog_TotalNum_Read(INT32U *total_num);

APP_PARAM_GLOBE int APP_PARAM_WarnLog_CurrID_Read(INT32U *id);

APP_PARAM_GLOBE int APP_PARAM_SatTotalNum_Read(INT16U *total_num);

APP_PARAM_GLOBE int APP_PARAM_SatCurrID_Read(INT8U *curr_id);

APP_PARAM_GLOBE int APP_PARAM_SatParam_Read(strSatParam *SatParam);

APP_PARAM_GLOBE int APP_PARAM_HistoryMaxMv_Read(FP32 *history_max_mv);

APP_PARAM_GLOBE INT8U  APP_PARAM_EthRelinkResetMCUFlag_Read(void);

APP_PARAM_GLOBE int APP_PARAM_AimAtSatParamLimitmv_Read(strSeekSatParam  *SeekSatParam);

APP_PARAM_GLOBE int APP_PARAM_AntennaDir_Map_MotorDir_Read(strAntennaParam *AntennaParam);

APP_PARAM_GLOBE int APP_PARAM_AntennaDir_Map_MotorDir_Save(INT8U AntennaPart,strAntennaDirMapMotor *AntennaDirMapMotor);

APP_PARAM_GLOBE int APP_PARAM_ElecCode_count_mode_Read(INT8U ElecCode_addr,INT8U *count_mode);

APP_PARAM_GLOBE int APP_PARAM_ElecCode_count_mode_Save(INT8U ElecCode_addr,INT8U count_mode);

APP_PARAM_GLOBE int APP_PARAM_DevPowerOnSysDateTime_Save(strSysDateTime	 *SysDateTime);

APP_PARAM_GLOBE int APP_PARAM_DevPowerOnSysDateTime_Read(strSysDateTime	 *SysDateTime);

