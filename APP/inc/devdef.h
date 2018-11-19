
#ifndef __DEV_DEF_H__
#define __DEV_DEF_H__


#ifdef	DEV_DEF
	#define	DEV_GLOBE
#else
	#define	DEV_GLOBE	extern
#endif


#include <bsp.h>

//#define	TEST_CODE_USE_FLAG


//  ע��:  ����Ƶ�ʵĵ�λ��KhzΪ��λ


//////////////////////////////////////////////////////////////////////////////////////////////
//���غ궨��

//�豸����״̬
#define		WORK_ON			0
#define		WORK_OFF			1

//ͨ���豸����״̬
#define		COMM_LINK			0
#define		COMM_UNLINK		1


//�豸����״̬
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

//ö�����Ͷ���

//ͨ��״̬ö��
typedef enum _enumCOMMSta_
{
	NORMAL=0,		
	SET_FAIL=1,
	QUE_FAIL=2,
	NO_CMD=4,
	COMM_CHECK_ERR=8,
} enumCOMMSta;

// ����ID
typedef enum _enumCOMMCmdID_
{
	// ��������ID
	SatParamSetID = 0x0101,				// �޸�/����������Ϣ����
	AntennaParamSetID,					// ������Ϣ��������
	AntennaParamAdjustID,				// ���߲���У׼
	LocalParamSetID,					// ����վλ����Ϣ�ֶ�����
	LocalParamSrcSelSetID,				// ����վλ����ϢԴѡ������
	
	SysDateTimeSetID,					// �豸ϵͳ����ʱ���ֶ�����
	SysDateTimeSrcSelSetID,				// �豸ϵͳ����ʱ����Դѡ��

	SatDelID,							// ɾ��������Ϣ����
	AimAtSatParamSetID=0x010A,				// ׷�ǲ�������
	AntennaTurnDirAdjustBeginSetID = 0x010B,	// �������߷���ת��У׼
	AntennaTurnDirAdjustFinishSetID,	// ������߷���ת��У׼
	AntennaHardWareLimitSetID  = 0x010E,	// ����Ӳ��λУ׼

	// �豸��������ID

	AimAtSatModeSelID = 0x0201,			// ����ģʽѡ��
	AntennaRecordID,					// ����λ�ü�¼
	AntennaSigleStepCtrID,				// �����ֶ���������
	AntennaDegreeCtrID,					// �����ֶ��Ƕȿ���
	AntennaRecordGotoID = 0x0206,					// ǰ������λ�ü�¼
	AntennaRecordDelID = 0x0207,					//ɾ�����߼�¼λ��
	AGCRecordID = 0x0209,					//��¼AGCֵ
	SatSelID = 0x0211,							// ѡ��������
	MotorSoftLimitSetID = 0x0212,					// �������λ����
	NetWorkSetID= 0x0215,				// ��������
	PCHighAdminID = 0x021A,				// PC��Ȩ��
	
	FactoryResetID = 0x0221,				// �ָ���������

	// ��ѯ����ID
	CurrSatParamQueID = 0x0801,			// ��ǰѡ��Ŀ��������Ϣ������ѯ
	AllSaveSatParamQueID,				// ��ǰ�洢������Ϣ������ѯ
	AntennaParamQueID,				// ��ǰ������Ϣ������ѯ
	AntennaAdjustParamQueID,			// ��ǰ����У׼������ѯ
	LocalParamQueID,					// ��ǰ����վλ����Ϣ��ѯ
	DevSysDateTimeQueID,				// ��ǰ�豸ϵͳ����ʱ�Ӳ�ѯ
	NetWorkQueID,						// ���������ѯ
	
	AimAtSatTheoreticalDegreeQueID = 0x080A,	// ��ǰ�豸�������۽ǲ�ѯ
	AntennaRecordQueID = 0x080B,				// �豸����λ�ü�¼��ѯ
	MotorSoftLimitQueID = 0x080C,		//����λ��ѯ
	AimAtSatModeQueID,					// ����ģʽ��ѯ
	AimAtSatParamQueID,				// ׷�ǲ�����ѯ
	DevStaQueID = 0x0810,				// �豸״̬��Ϣ��ѯ
	DevAdminQueID = 0x0811,			// �豸����״̬��ѯ
	CurrWarnStaQueID = 0x0812,				// ��ǰ�豸�澯��Ϣ��ѯ

	// �豸��־��ѯ
	WarnLogQueID = 0x0901,				// �豸�澯��־��ѯ
	OperationLogQueID,					// ������־��ѯ

	VersionQueID = 0x0A01,				// �汾��ѯ

}enumCOMMCmdID;


//������ö��
typedef enum _enumMotorIDSta_
{
	motor_position_id =1,		
	motor_cabrage_id,	
	motor_polar_id,	
} enumMotorIDSta;

/////////////////////////////////////////////////////////////////////////////////////////////

//��Ϣ��һЩ�ṹ��

#define	BUZZER_Q_ID			0x01
#define	HORLED_Q_ID			0x02
#define	NIXIELIGHT_Q_ID		0x03

typedef	struct _strBuzzerQ_
{
	INT8U 	id;
	INT8U	len;
	INT8U	on_off;				// 
	INT8U	flick_flag;			// �Ƿ���˸ʽ���б��,0Ϊ����˸�У�1Ϊ��˸��
	INT16U	period_ms;			// ���м��ʱ��
	INT16U	beep_cnt;			// ���д���
}strBuzzerQ;


//////////////////////////////////////////////////////////////////////////////////////////////


//PCͨ��״̬�ṹ��
typedef struct  _strPCCommStat_
{
	INT8U	link_stat;
	INT8U 	comm_stat;
	INT8U	mcu_busy;
	INT32U 	dst_IP_addr;		// Ŀ��PC IP��ַ
	INT32U 	dst_IP_port;			// Ŀ��PC ��ͨ�Ŷ˿ں�
}strPCCommStat;

//PDUͨ��״̬�ṹ��
typedef struct  _strPDUCommStat_
{
	INT8U	link_stat;
	INT8U 	comm_stat;
	INT8U	mcu_busy;
}strPDUCommStat;

// �豸������ؽṹ��
typedef struct  _strNetWork_
{
	INT32U	ip_addr;
	INT32U 	mask;
	INT32U	gate;
	INT8U 	mac[6];
	INT32U 	ip_port;
}strNetWork;


//�ű������ӽṹ��
//�ű��������Ϣ�ṹ��
typedef struct  _strBeaconConf_
{
	INT8U	comm_interface;			//ͨ�Žӿ�:  Ĭ��Ϊ0������ͨ��
	INT32U 	comm_rate;				// ͨ������
	INT32U	lo_freq;					// ����Ƶ��
	INT32U	work_freq;				// ����Ƶ��
	INT8U	agc_output_range;			
}strBeaconConf;

//�ű��״̬��Ϣ�ṹ��
typedef struct  _strBeaconStat_
{
	INT8U	link_stat;				// ����״̬ :  COMM_LINK    COMM_UNLINK
	INT32U 	dev_stat;				// �豸״̬  WORKSTA_NORMAL   WORKSTA_ALARM
	INT32U	comm_stat;					// ͨ��Э��״̬ enumCOMMSta
}strBeaconStat;

//�ű��������Ϣ�ṹ��
typedef struct  _strBeaconData_
{
	INT32U	data_dbm;				// dbm��λ����
	INT32U 	data_mv;				// ������λ����
}strBeaconData;


//GPS����ӽṹ��
//GPS������Ϣ�ṹ��
typedef struct  _strGPSConf_
{
	INT8U	comm_interface;			//ͨ�Žӿ�:  Ĭ��Ϊ0������ͨ��
	INT32U 	comm_rate;				// ͨ������
	
}strGPSConf;

//GPS״̬��Ϣ�ṹ��
typedef struct  _strGPSStat_
{
	INT8U	link_stat;				// ����״̬ :  COMM_LINK    COMM_UNLINK
	INT32U 	dev_stat;				// �豸״̬  WORKSTA_NORMAL   WORKSTA_ALARM
	INT32U	comm_stat;					// ͨ��Э��״̬ enumCOMMSta
}strGPSStat;

//GPS������Ϣ�ṹ��
typedef struct  _strGPSData_
{
	INT16U	year;	
	INT8U	month;	
	INT8U	day;
	INT8U	hour;
	INT8U	min;
	INT8U	sec;
	INT16U   ms;
	INT8U	lat_flag;					// �ϱ�γ��ʶ��'N'��γ��'S'��γ
	INT32U		lat;						//  ʵ��γ��ֵ��1000��������ȷ��0.001�ȣ�
	INT8U	lon_flag;					// ��������ʶ��'E'����    'W'����
	INT32U		lon;						//  ʵ�ʾ���ֵ��1000��������ȷ��0.001��
	INT32U		height;					// ����Ϊ��λ
	INT8U valid;
}strGPSData;


//������������ӽṹ��
//��������������Ϣ�ṹ��
typedef struct  _strEleccoderConf_
{
	INT8U	comm_interface;			//ͨ�Žӿ�:  Ĭ��Ϊ0������ͨ��
	INT32U 	comm_rate;				// ͨ������
	INT8U	addr;					// ��ַ
	INT8U 	bit_num;				// ����λ��
	INT8U	count_mode;			// ����ģʽ��0xE1��ʾ˳ʱ��Ӽ�����0xE2��ʾ˳ʱ�������
}strEleccoderConf;

//��������״̬��Ϣ�ṹ��
typedef struct  _strInclinometerStat_
{
	INT8U	link_stat;				// ����״̬ :  COMM_LINK    COMM_UNLINK
	INT32U 	dev_stat;				// �豸״̬  WORKSTA_NORMAL   WORKSTA_ALARM
	INT32U	comm_stat;					// ͨ��Э��״̬ enumCOMMSta
	INT8U	comm_idle;
}strEleccoderStat;

//��������������Ϣ�ṹ��
typedef struct  _strInclinometerData_
{
	INT32U	data;				// ԭʼ����
	FP32		degree;				// ԭʼ����ת����ĽǶ���(0~360��)

}strEleccoderData;


// ����������ʵ�ʽǶ�ӳ���ϵ
typedef struct  _strElecCoderMapAntenna_
{
	INT8U			ElecCoder_count_mode;	// 		����ָ��ӳ���ϵʱ�ĵ������̵ļ���ģʽ
	FP32				ElecCoder_degree;		// 		����ָ��ӳ���ϵʱ�ĵ������̶���
	FP32				Antenna_degree;		// 		����ָ��ӳ���ϵʱ������ʵ��ָ�����
}strElecCoderMapAntenna;

#define	ANTENNA_DIR_LEFT_DOWN_ANTICLOCK	0x01
#define	ANTENNA_DIR_RIGHT_UP_CLOCK			0x02

#define	ANTENNA_DIR_LEFT_DOWN_ANTICLOCK_LIMIT		0x01
#define	ANTENNA_DIR_RIGHT_UP_CLOCK_LIMIT			0x02

#define	MOTOR_DIR_ANTICLOCK					0x01
#define   MOTOR_DIR_CLOCK						0x02

// ���ת������������ת������ӳ���ϵ
typedef struct  _strAntennaDirMapMotor_
{
	INT8U 		antenna_part;		// 0x01��ʾ��λ��0x02��ʾ������0x03��ʾ����
	INT8U 		motor_dir[2];		// ��λʱ�����0��ʾ��תʱ�ĵ���������1��ʾ��תʱ�ĵ������
									// ����ʱ�����0��ʾ��ת�ǵĵ���������1��ʾ��ת�ǵĵ������
									// ���������0��ʾ��ʱ��ʱ�ĵ���������1��ʾ˳ʱ��ʱ�ĵ������
}strAntennaDirMapMotor;

//////////////////////////////////////////////////////////////////////////////////////////////

// ACU����������ӽṹ��

// �ű���ṹ��
typedef struct  _strBeaconInfo_
{
	strBeaconConf 	BeaconConf;
	strBeaconStat		BeaconStat;
	strBeaconData	BeaconData;
}strBeaconInfo;

// GPS�ṹ��
typedef struct  _strGPSInfo_
{
	strGPSConf 	GPSConf;
	strGPSStat	GPSStat;
	strGPSData	GPSData;
}strGPSInfo;

// �������̽ṹ��
typedef struct  _strEleccoderInfo_
{
	strEleccoderConf 	EleccoderConf;
	strEleccoderStat	EleccoderStat;
	strEleccoderData	EleccoderData;
}strEleccoderInfo;

// EEPROM�ṹ��
typedef struct  _strEEPROMInfo_
{
	INT32U		total_size;
	INT8U		link_stat;
	INT8U		comm_stat;
}strEEPROMInfo;


// SPI FLASH�ṹ��
typedef struct  _strSPIFlashInfo_
{
	INT32U		total_size;
	INT32U		sector_size;
	INT8U		link_stat;
	INT8U		comm_stat;
}strSPIFlashInfo;

// Buzzer�ṹ��
typedef struct  _strBuzzerInfo_
{
	INT8U		buzzer_switch;
	INT8U		on_off_status;
	INT8U 		flick_flag;
	INT16U 		beep_ms;
	INT16U 		period_ms;
	INT32U 		beep_cnt;
}strBuzzerInfo;



//���ǲ���
typedef struct  _strSatParam_
{
	INT8U		id;						// ����ID
	INT8U 		name[16];				// ��������
	INT8U		lon_flag;				//���Ǿ��ȱ�ʾ  ,'E'  'W'
	FP32			lon;						// ���Ǿ���
	INT32U		freq;					// ��������
	INT8U		polar_type;				//  ������ʽ
}strSatParam;

//����վ����
typedef struct  _strLocalPositionParam_
{
	INT8U		lon_flag;				// ����λ�þ��ȱ�ʾ
	FP32			lon;						// ����
	INT8U		lat_flag;					// ����λ��γ�ȱ�ʾ
	FP32			lat;						// γ��
	INT32U		height;					// �߶�    ����(m)Ϊ��λ
	INT8U		valid_flag;				// ��Ч��
	INT8U		data_src_flag;			// ����Դ��0x00 GPS  0x01  �˹�����
}strLocalPositionParam;

//����վ�ֶ����ò���
typedef struct  _strHandLocalPositionParam_
{
	INT8U		lon_flag;				// ����λ�þ��ȱ�ʾ
	FP32		lon;						// ����
	INT8U		lat_flag;					// ����λ��γ�ȱ�ʾ
	FP32		lat;						// γ��
	INT32U		height;					// �߶�    ����(m)Ϊ��λ

	INT8U 		valid_flag;				// 0x01 ��ʾ��Ч��0x00��ʾ��Ч

}strHandLocalPositionParam;

//���߲���
typedef struct  _strAntennaParam_
{
	INT8U		type;		// �����ͺţ�  ������ƫ����װ��ƫ����װ3��  0:����  1: ƫ����װ   2: ƫ����װ
	INT32U		offset_angle;	// �����ƫ���������ߣ�����Ҫ֪��ƫ����
	INT8U 		caliber;			// ���߿ھ����ܹ�5�У���1 �� 5
	INT32U		lo_freq;
	INT8U		gain_range_flag;		// �������淶Χ��־

	INT8U		elec_coder_num;		// ������������

	FP32			motor_cabrage_start;	// ���������Χ��ʼ�Ƕ�ֵ
	FP32			motor_cabrage_end;		// ���������Χ�����Ƕ�ֵ

	FP32			motor_position_start;	// ��λ�����Χ��ʼ�Ƕ�ֵ
	FP32			motor_position_end;		// ��λ�����Χ�����Ƕ�ֵ

	FP32			motor_polar_start;		// ���������Χ��ʼ�Ƕ�ֵ
	FP32			motor_polar_end;		// ���������Χ���޽Ƕ�ֵ
	
	FP32			aim_cabrage;			// Ŀ�긩��������ֵ
	FP32			aim_position;			// Ŀ�귽λ������ֵ
	FP32			aim_polar;				// Ŀ�꼫��������ֵ

	FP32			aim_cabrage_hand;			// Ŀ�긩��������ֵ�ֶ�ģʽ
	FP32			aim_position_hand;			// Ŀ�귽λ������ֵ�ֶ�ģʽ
	FP32			aim_polar_hand;				// Ŀ�꼫��������ֵ�ֶ�ģʽ

	strAntennaDirMapMotor AntennaDirMapMotor[3];		
}strAntennaParam;

// �豸�澯�ṹ��
typedef struct  _strDevAlarm_
{
	INT8U		alarm_cnt;		// �澯��Ŀ
	INT8U		alarm_refresh;		// �澯����
	INT32U		*alarm_ptr;		// �澯����ָ��
}strDevAlarm;


// �豸����Ȩ�޽ṹ��
typedef struct  _strDevAdmin_
{
	INT8U		admin_sta;		// ����Ȩ��״̬// �豸�������״̬��0x00��ʾACU���أ�0x01��ʾPC���أ�0x02��ʾPDU���ء�
	INT8U		mode_sta;		// ģʽ״̬�����Զ�ģʽ���ֶ�ģʽ�Ϳ��У�0x00���У�0x01�ֶ���0x02�Զ�
								// PDUģʽ�£�Ĭ�Ͼ����ֶ�ģʽ��
								// PC ��ACU�����£���ѡ���Ƿ��Զ������ֶ�
	INT8U 		stop_flag;		//  ��ͣ��־0x01��ʾֹͣһ�л
	INT8U		pc_admin_deal_sta;		// PC������Ȩ�޵Ĵ���״̬����
										//0x01 ��ʾPC����Ȩ�޵ȴ�OLED���û�����
										// 0x00, ��ʾû����������Ѿ��õ��� 
	INT32U		pc_admin_ip_addr;		// pc ����ʱ����ǰpc ��IP��ַ
	INT32U		pc_admin_ask_ip_addr;		//����pc ����Ȩ�޵�IP��ַ
	INT32U		pc_admin_link_cnt;		// pc ����ʱ��pc ��link ����
}strDevAdmin;


// ȫ���ĵ������������߸���λ��ӳ���ϵ 
// ����������ʵ�ʽǶ�ӳ���ϵ
typedef struct  _strAllElecCoderMapAntenna_
{
	strElecCoderMapAntenna		ElecCoderMapAntenna[3];
	INT8S	SatPolarType;							// У׼ʱ�����Ǽ�����ʽ����Ӧ�ļƻ��ǶȼӼ���־����ˮƽ������Ϊ0��
																	// ��ֱ������Ŀ��Ƕ�ֵ����90�ȣ���Ϊ2������90����Ϊ1.
}strAllElecCoderMapAntenna;


// ׷�ǲ���
typedef struct  _strSeekSatParam_
{
	INT8U	SeekSat_Mode;				// ����ģʽ��0x01�ֶ���0x02�Զ���0x00ֹͣ��0x03,����ģʽ��0x04ǰ����̬��¼λ��
	INT8U	Urgent_Stop;				//  ����ֹͣ��0x01����ֹͣ��0x00������
	INT8U 	ScanRange_param[3];		//  ɨ�跶Χ����
	INT32U 	AimAtSat_Limit_mv;			// ����׷�ǵ��������ֵ���޷��ţ�
	
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
	INT32U log_sys_addr;		//����Ҫ�洢��ÿ�ο������Զ���ʼ��
	INT32U log_total_num;
	INT32U currlog_addr;				// ��ǰ�洢��log��ַ
	INT32U currlog_size;
	INT32U currlog_index;
	INT32U beforelog_addr;			// ֮ǰ�Ѿ��洢�õ�LOG��ַ
	INT32U beforelog_index;
	INT32U beforelog_size;
}strLogInfo;

//////////////////////////////////////////////////////////////////////////////////////////////

//ACU����ض����ӽṹ��

//�豸״̬��������Ϣ�ṹ��
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

//���ǲ�����Ϣ�ṹ��
typedef struct  _strACUSeekSatParam_
{
	strSatParam 			CurrSatParam;
	strLocalPositionParam	LocalPositionParam;
	strAntennaParam		AntennaParam;
	strSeekSatParam		SeekSatParam;
}strACUSeekSatParam;


#define	SAT_MAX_NUM		100
//�����б�ṹ��
typedef struct  _strSATList_
{
	INT16U				Sat_Total_num;		// �洢����������
	INT8U				Sat_ID[SAT_MAX_NUM];
	INT8U 				SatList_Refresh_flag;	//  0x01��ʾ�б��и��£�0x00��ʾ�б��޸���
}strSATList;


//����λ�ü�¼�б�ṹ��
#define	ANTENNA_RECORD_MAX_NUM		10
typedef struct  _strAntennaRecordList_
{
	INT32U				record_Total_num;		// �洢����������
	INT8U				record_ID[ANTENNA_RECORD_MAX_NUM];
}strAntennaRecordList;


//acu��ǰ״̬�ṹ��
typedef struct  _strSATseekerCurrSta_
{
	FP32 				beacon_rxpower_db;
	FP32 				beacon_rxpower_mv;

	FP32 				history_max_mv;

	

	FP32					antenna_position;		// ���߷�λ��
	FP32					antenna_cabrage;		// ���߸�����
	FP32					antenna_polar;			// ���߼�����
	
	INT8U				antenna_position_sta;			//  	��ACU��PDUͨ��Э��3.3.1
	INT8U				antenna_cabrage_sta;			//  	��ACU��PDUͨ��Э��3.3.2
	INT8U				antenna_polar_sta;				//  	��ACU��PDUͨ��Э��3.3.3
	
	INT8U 				dev_admin_sta;			// �豸�������״̬��0x00��ʾACU���أ�0x01��ʾPC���أ�0x02��ʾPDU����, 0x11��ʾ����PC����Ȩ��״̬��

	INT8U 				dev_move_sta;			// �豸�˶�״̬
	
	INT8U 				dev_work_sta;			// �豸����״̬
	
	INT8U 				dev_alarm_flag;			// �豸�澯��־
	INT8U 				dev_alarm_reflash_flag[2];	//  �豸�澯���±�־		���0������PC�����1����OLED
	INT8U 				dev_alarm_buff[6];
						// bitλ��1��ʾ���ϸ澯��0��ʾ����
						//	���0:  �������̹��ϣ� bit0��λ���̣�bit1�������̣�bit2�ƻ�����
						//	���1:  �ű����bit0�ű��ͨ�Ź��ϣ�bit1�ű��Ƶ��д��ʧ�ܣ�bit2���淶Χ����ʧ��
						//   ���2:  ���ذ壬bit0EEPROM���ϣ�bit1flash����
						//  ���3:  PDUͨ�Ź���
						// ���4:	GPS���ϣ�bit0GPSͨ�Ź��ϣ�bit1GPS��λʧ��
						// ���5:	����澯:	bit0����������޸澯��bit1����������޸澯
						//							bit2��λ�������λ�澯��bit3��λ�������λ�澯
						//							bit4�������˳��λ�澯��bit5�����缫����λ�澯

	INT8U 				dev_param_reflash_flag[2];		//  �豸�������±�־�����0������PC�����1����OLED

	strDevAdmin			DevAdmin;
	strDevAlarm			DevAlarm;

	FP32 				AGC_max_mv;				// ��ǰ����ֵ
	INT8U 				motor_move[3];				// ���0�~���2�ֱ��ʾ  ��λ�������������Լ����������
	INT8U 				motor_limit_sta[3];			// ���0�~���2�ֱ��ʾ  ��λ�������������Լ����������
													//����˶�״̬��0x00��ʾֹͣ�˶���ANTENNA_DIR_LEFT_DOWN_ANTICLOCK �� ANTENNA_DIR_RIGHT_UP_CLOCK
	INT32S					temperature;			//  ACU�¶�
}strACUCurrSta;


// �汾�ṹ��
typedef struct  _strVersion_
{
	INT8U 			hardver[16];
	INT8U			softver[16];
	INT8U			datetime[6];		
}strVersion;

// ϵͳʱ��
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

	INT32U 			src_flag;			// ʱ��Դ�� 0x00��ʾ�Զ���ȡ�� 0x01��ʾ�ֶ����ã�0x02��ʾGPS��ȡ
}strSysDateTime;



//////////////////////////////////////////////////////////////////////////////////////////////
// ACU�����һ���ṹ��
typedef struct  _strSATseekerDevInfo_
{
	strDeviceInfo  		DeviceInfo;
	strACUSeekSatParam		ACUParam;
	strSATList			SATList;		
	strAntennaRecordList	AntennaRecordList;
	strACUCurrSta		ACUCurrSta;
	strSysDateTime		SysDateTime;
	strVersion			Version[2];				// ����0��ʾACU�İ汾������1��ʾPDU�İ汾

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

DEV_GLOBE INT8U Task_Run_Flag[3];		// ���0��ʾ�����������1��ʾ׷���������2��ʾǰ����¼λ������
#endif

