

#ifdef APP_LOG_DEF
	#define APP_LOG_GLOBE
#else
	#define APP_LOG_GLOBE		extern
#endif

#include <ic_n25q128.h>
#include <hw_eeprom.h>
#include <app_pccomm.h>

////////////////////////////////////////////////////////////////////////////////////////////////
// 告警日志宏定义

// 编码器类告警
#define	LOG_WARN_CODER_AZ			0x010101				// 方位编码器故障
#define	LOG_WARN_CODER_EL			0x010102				// 俯仰编码器故障
#define	LOG_WARN_CODER_PL			0x010103				// 极化编码器故障

// 信标机告警
#define	LOG_WARN_BEACON_ERR						0x010201				// 信标机故障
#define	LOG_WARN_BEACON_SET_FREQ_FAIL			0x010202				// 信标机频率设置失败
#define	LOG_WARN_BEACON_SET_GAIN_FAIL			0x010203				// 信标机增益范围设置失败

// 主控板告警
#define	LOG_WARN_ACU_EEPROM_ERR				0x010301	// 主控板易擦除可读写存储器故障		
#define	LOG_WARN_ACU_FLASH_ERR					0x010302	// 主控板闪存故障

// PDU告警
#define	LOG_WARN_PDU_ERR							0x010401	// 功率驱动单元故障

#define	LOG_WARN_PDU_MOTOR_AZ_ERR				0x010411	// 天线方位电机故障
#define	LOG_WARN_PDU_MOTOR_AZ_RIGHT_LIMIT		0x010412	// 天线右限位告警
#define	LOG_WARN_PDU_MOTOR_AZ_LEFT_LIMIT		0x010413	// 天线左限位告警

#define	LOG_WARN_PDU_MOTOR_EL_ERR				0x010421	// 天线俯仰电机故障
#define	LOG_WARN_PDU_MOTOR_EL_UP_LIMIT			0x010422	// 天线上限位告警
#define	LOG_WARN_PDU_MOTOR_EL_DOWN_LIMIT		0x010423	// 天线下限位告警

#define	LOG_WARN_PDU_MOTOR_PL_ERR				0x010431	// 天线极化电机故障
#define	LOG_WARN_PDU_MOTOR_PL_UP_LIMIT			0x010432	// 天线顺时针限位告警
#define	LOG_WARN_PDU_MOTOR_PL_DOWN_LIMIT		0x010433	// 天线逆时针限位告警

#define	LOG_WARN_PDU_GPS_ERR					0x010491	// GPS故障
#define	LOG_WARN_PDU_GPS_POSITIONING_FAIL		0x010492	// GPS定位失败

////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
// 操作日志宏定义

// 自动对星
#define	LOG_OPT_AUTO_AIM_STAR_BEGIN_BY_PC				0x020111	// PC机启动自动对星
#define	LOG_OPT_AUTO_AIM_STAR_END_BY_PC				0x020112	// PC机停止自动对星
#define	LOG_OPT_AUTO_AIM_STAR_BEGIN_BY_OLED			0x020121	// OLED机启动自动对星
#define	LOG_OPT_AUTO_AIM_STAR_END_BY_OLED				0x020122	// OLED机停止自动对星
#define	LOG_OPT_AUTO_AIM_STAR_END_BY_ACU_LOCK			0x020101	// 完成自动对星流程，锁定卫星
#define	LOG_OPT_AUTO_AIM_STAR_END_BY_ACU_UNLOCK		0x020102	// 完成自动对星流程，未锁定卫星

// 手动对星
#define	LOG_OPT_HAND_AIM_STAR_BEGIN_BY_PC				0x020211	// PC机启动手动对星
#define	LOG_OPT_HAND_AIM_STAR_END_BY_PC				0x020212	// PC机停止手动对星
#define	LOG_OPT_HAND_AIM_STAR_BEGIN_BY_OLED			0x020221	// OLED机启动手动对星
#define	LOG_OPT_HAND_AIM_STAR_END_BY_OLED				0x020222	// OLED机停止手动对星

// 姿态记录及回归
#define	LOG_OPT_ANTENNA_POSE_RECORD_BY_PC				0x020301	// PC机记录天线当前姿态以及相关对星参数
#define	LOG_OPT_ANTENNA_POSE_QUE_BY_PC					0x020302	// PC机查阅天线姿态历史记录
#define	LOG_OPT_ANTENNA_POSE_DEL_BY_PC					0x020303	// PC机删除天线姿态历史记录
#define	LOG_OPT_ANTENNA_POSE_APP_BY_PC					0x020304	// PC机应用天线姿态历史记录
#define	LOG_OPT_ANTENNA_POSE_STOP_BY_PC				0x020305	// PC机停止天线姿态历史记录

#define	LOG_OPT_ANTENNA_POSE_RECORD_BY_OLED			0x020311	// OLED机记录天线当前姿态以及相关对星参数
#define	LOG_OPT_ANTENNA_POSE_QUE_BY_OLED				0x020312	// OLED机查阅天线姿态历史记录
#define	LOG_OPT_ANTENNA_POSE_DEL_BY_OLED				0x020313	// OLED机删除天线姿态历史记录
#define	LOG_OPT_ANTENNA_POSE_APP_BY_OLED				0x020314	// OLED机应用天线姿态历史记录
#define	LOG_OPT_ANTENNA_POSE_STOP_BY_OLED				0x020315	// OLED机停止天线姿态历史记录

#define	LOG_OPT_ANTENNA_POSE_FINISH						0x020320	// 完成姿态回归

// 跟踪模式
#define	LOG_OPT_FOLLOW_STAR_BEGIN_BY_PC				0x020411	// PC机启动跟踪模式
#define	LOG_OPT_FOLLOW_STAR_END_BY_PC					0x020412	// PC机停止跟踪模式
#define	LOG_OPT_FOLLOW_STAR_BEGIN_BY_OLED				0x020421	// OLED机启动跟踪模式
#define	LOG_OPT_FOLLOW_STAR_END_BY_OLED				0x020422	// OLED机停止跟踪模式
#define	LOG_OPT_FOLLOW_STAR_BEGIN_BY_ACU				0x020401	// ACU 启动跟踪模式
#define	LOG_OPT_FOLLOW_STAR_END_BY_ACU					0x020402	// ACU 停止跟踪模式

//待机模式
#define	LOG_OPT_STANDBY_MODE_BY_ACU					0x020501	// 因有告警故障，ACU 自动进入待机空闲模式

// 天线参数配置
#define	LOG_OPT_ANTENNA_PARAM_SET_BY_PC				0x021101	// PC机设置天线参数

// 参数校准
#define	LOG_OPT_ANTENNA_AZ_TURN_ADJUST					0x021201	// 天线方位转向校准
#define	LOG_OPT_ANTENNA_EL_TURN_ADJUST					0x021202	// 天线俯仰转向校准
#define	LOG_OPT_ANTENNA_PL_TURN_ADJUST					0x021203	// 天线极化转向校准

#define	LOG_OPT_ANTENNA_AZ_RIGHT_LIMIT_ADJUST			0x021211	// 天线方位右限位校准
#define	LOG_OPT_ANTENNA_AZ_LEFT_LIMIT_ADJUST			0x021212	// 天线方位左限位校准
#define	LOG_OPT_ANTENNA_EL_UP_LIMIT_ADJUST				0x021213	// 天线俯仰上限位校准
#define	LOG_OPT_ANTENNA_EL_DOWN_LIMIT_ADJUST			0x021214	// 天线俯仰下限位校准
#define	LOG_OPT_ANTENNA_PL_CLOCK_LIMIT_ADJUST			0x021215	// 天线极化顺时针限位校准
#define	LOG_OPT_ANTENNA_PL_ANTICLOCK_LIMIT_ADJUST		0x021216	// 天线极化逆时针限位校准

#define	LOG_OPT_ANTENNA_CODER_DATA_ADJUST				0x021221	// 天线码盘数据校准

// 卫星相关操作
#define	LOG_OPT_ADD_STAR									0x021301	// 增添卫星
#define	LOG_OPT_REVISE_STAR_PARAM						0x021302	// 修改卫星参数
#define	LOG_OPT_DEL_STAR									0x021303	// 删除卫星
#define	LOG_OPT_SEL_STAR_BY_PC							0x021304	// PC机选择卫星
#define	LOG_OPT_SEL_STAR_BY_OLED							0x021314	// OLED选择卫星

// 地球站参数
#define	LOG_OPT_SET_LOCALL_PARAM						0x021401	// 设置本地位置信息

// 追星参数设置
#define	LOG_OPT_SET_FOLLOW_STATR_MIN_LIMIT_PARAM		0x021501	// 设置追星最低门限值

// 网络设置
#define	LOG_OPT_NETWORK_IP_ADDR_SET_BY_PC				0x021601	//PC设置机本地IP
#define	LOG_OPT_NETWORK_MASK_SET_BY_PC					0x021602	//PC机设置本地子网掩码
#define	LOG_OPT_NETWORK_GATE_SET_BY_PC					0x021603	//PC机设置本地网关

#define	LOG_OPT_NETWORK_IP_ADDR_SET_BY_OLED			0x021611	//OLED设置机本地IP
#define	LOG_OPT_NETWORK_MASK_SET_BY_OLED				0x021612	//OLED设置本地子网掩码
#define	LOG_OPT_NETWORK_GATE_SET_BY_OLED				0x021613	//OLED设置本地网关

// 设备操作
#define	LOG_OPT_DEV_POWER_ON							0x022101	// 开机操作
#define	LOG_OPT_DEV_FACTORY_RESET						0x022102	// 恢复出厂设置

#define	LOG_OPT_PDU_STOP_ENABLE							0x022111	// 启用急停操作
#define	LOG_OPT_PDU_STOP_DISABLE						0x022112	// 取消急停操作
#define	LOG_OPT_PDU_ADMIN_ENABLE						0x022121	// 启用PDU本地控制
#define	LOG_OPT_PDU_ADMIN_DISABLE						0x022122	// 取消PDU本地控制

#define	LOG_OPT_PC_ASK_FOR_ADMIN						0x022131	// PC机申请管理权限
#define	LOG_OPT_OLED_PERMIN_PC_ADMIN					0x022132	// OLED允许PC获得管理权限
#define	LOG_OPT_OLED_REFUSE_PC_ADMIN					0x022133	// OLED 拒绝PC获得管理权限
#define	LOG_OPT_OLED_CANCEL_PC_ADMIN					0x022134	// OLED 取消PC的管理权限

#define	LOG_OPT_TIMEOUT_ACU_REFUS_PC_ADMIN			0x022135	// PC机管理权限申请超时未处理/PC机超时未通信，ACU取消PC机管理权限申请状态
#define	LOG_OPT_TIMEOUT_ACU_CANCEL_PC_ADMIN			0x022136	// PC机超时未通信，ACU自动取消PC机管理权限

#define	LOG_OPT_TIMEOUT_PC_CANCEL_PC_ADMIN			0x022137	// PC机退出管理权限
#define	LOG_OPT_TIMEOUT_PC_CANCEL_PC_ADMIN_ASK		0x022138	// PC机取消申请管理权限

////////////////////////////////////////////////////////////////////////////////////////////////



#define	LOG_SYS_RECORD_SIZE		32


APP_LOG_GLOBE int APP_LOG_Init(void);

APP_LOG_GLOBE int APP_OptLOG_Save2Flash(INT8U *buff , INT8U len);

APP_LOG_GLOBE int APP_OptLOG_Save(INT32U code,INT8U *buff,INT8U len);

APP_LOG_GLOBE int APP_OptLOG_SaveCode(INT32U code);

APP_LOG_GLOBE int APP_OptLOG_Save_PowerOn(void);

APP_LOG_GLOBE int APP_WarnLOG_Save2Flash(INT8U *buff , INT8U len);

APP_LOG_GLOBE int APP_WarnLOG_Save(INT32U code,INT8U *buff,INT8U len);

APP_LOG_GLOBE int APP_WarnLOG_SaveCode(INT32U code);

APP_LOG_GLOBE int APP_LOG_Read1Log(strLogInfo *CurrLogInfo,INT8U *logbuff,INT8U *len);

APP_LOG_GLOBE void APP_LOG_SatParam2Logbuff(strSatParam 	SatParam,INT8U *logbuff,INT8U *len);

APP_LOG_GLOBE void APP_LOG_LocallParam2Logbuff(strHandLocalPositionParam 	LocalPositionParam,INT8U *logbuff,INT8U *len);

APP_LOG_GLOBE void APP_LOG_AntennaParam2Logbuff(strAntennaParam 	AntennaParam,INT8U *logbuff,INT8U *len);

APP_LOG_GLOBE void APP_LOG_AntennaPoseData2Logbuff(FP32 degree_AZ,FP32 degree_EL,FP32 degree_PL,INT8U *logbuff,INT8U *len);


APP_LOG_GLOBE int APP_OptLOG_Save_SatOpt(INT32U code,strSatParam 	SatParam);

APP_LOG_GLOBE int APP_OptLOG_Save_LocallParamOpt(INT32U code,strHandLocalPositionParam 	LocalPositionParam);

APP_LOG_GLOBE int APP_OptLOG_Save_AntennaParamOpt(INT32U code,strAntennaParam 	AntennaParam);

APP_LOG_GLOBE int APP_OptLOG_Save_AntennaPoseOpt(INT32U code,strSatParam 	SatParam,strAntennaParam AntennaParam,FP32 degree_AZ,FP32 degree_EL,FP32 degree_PL);

APP_LOG_GLOBE int APP_LOG_FollowSatRecord(INT32U code);

APP_LOG_GLOBE int APP_LOG_RecordSatDrift(INT32U period_ms);

