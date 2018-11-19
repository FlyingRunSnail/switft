

#ifdef APP_PARAM_DEF
	#define APP_PARAM_GLOBE
#else
	#define APP_PARAM_GLOBE		extern
#endif

#include <devdef.h>


// ���߲��������ַ��12���ֽ�
// �洢�����������ͣ�1B��+ ƫ���ǣ�4B��+ ���߿ھ���1B��+ LNB����4B��+ AGC�����Χ��1B��+ ��������(1B)
#define		APP_EEPROM_Antenna_Param_ADDR						0x0000


// ��ʷ�������ŵ�ѹֵ�����ַ��4���ֽ�
#define		APP_EEPROM_HISTORY_MAX_MV_ADDR						0x000C

// �������������߽Ƕ�ӳ��ֵ�洢��ַ,   24���ֽ�
// ���߶��Ƿ�λ�Ƕ�ֵ(4B) + ��λ���̽Ƕ�ֵ(4B) +  ���̼���ģʽ(1B)
// ���߶��Ǹ����Ƕ�ֵ(4B) +�������̽Ƕ�ֵ(4B)  + ���̼���ģʽ(1B)
// ���߶��Ǽ����Ƕ�ֵ(4B) + �������̽Ƕ�ֵ(4B) + ���̼���ģʽ(1B)
#define		APP_EEPROM_ElecCode_Map_AntennaDegree_Addr			0x0010

// ����վ������Ϣ����Դѡ����Ϣ�洢��ַ�� 1���ֽ�
#define		APP_EEPROM_LocallParamSrc_ADDR						0x0030

// ����վ������Ϣ�������һ���ֶ����õ���Ϣ�洢��ַ�� 14���ֽ�
// ���ؾ��ȱ�ʶ��1B��+���ؾ��ȣ�4B��+ ����γ�ȱ�ʶ��1B��+ ����γ�ȣ�4B��+ ���غ��Σ�4B��
#define		APP_EEPROM_LocallParam_ADDR							0x0031

// ϵͳ����ʱ������Դѡ����Ϣ�洢��ַ�� 1���ֽ�
#define		APP_EEPROM_DevSysDateTimeSrc_ADDR						0x0070

// ����ģʽ�洢,  1���ֽ�
#define		APP_EEPROM_AimAtSatMode_ADDR							0x0074

// ���ǹ���״̬,  1���ֽ�
#define		APP_EEPROM_AimAtSatWorkSta_ADDR							0x0075

// ׷�ǲ���������޴洢��4���ֽ�
#define		APP_EEPROM_AimAtSatLimitMv_ADDR							0x0078

// �������ô洢��ַ�� 16���ֽ�
// IP��ַ��4B��+ �������루4B��+���أ�4B��+  �˿ں�(4B)
#define		APP_EEPROM_NetWork_ADDR									0x0080

// MAC �洢��ַ�� 6���ֽ�
#define		APP_EEPROM_MAC_ADDR										0x0090

// ��λ���Ӳ��λ��Ϣ�洢��ַ�� 8���ֽڣ�����λ�������ݣ�4B��+ ����λ�������ݣ�4B��
#define		APP_EEPROM_PositionHardLimit_ADDR							0x00A0

// �������Ӳ��λ��Ϣ�洢��ַ�� 8���ֽڣ�����λ�������ݣ�4B��+ ����λ�������ݣ�4B��
#define		APP_EEPROM_CabrageHardLimit_ADDR							0x00B0

// �������Ӳ��λ��Ϣ�洢��ַ�� 8���ֽڣ���ʱ����λ�������ݣ�4B��+ ˳ʱ����λ�������ݣ�4B��
#define		APP_EEPROM_PolarHardLimit_ADDR							0x00C0

// ����ת����������ת������ӳ���ϵ,6�ֽ�
// ���߷�λ��ת��ʱ��λ���ת������(1B) +���߷�λ��ת��ʱ��λ���ת������(1B) +
//������ת��ʱ �������ת������(1B)+������ת��ʱ �������ת������(1B)+
// ������ʱ��ת��ʱ �������ת������(1B) + ����˳ʱ��ת��ʱ�������ת������(1B)
#define		APP_EEPROM_AntennaDir_Map_MotorDir_ADDR					0x00D0

// �������̵ļ���ģʽ
#define		APP_EEPROM_ELECCODER_COUNT_MODE_ADDR					0x00DA


// ��ǰ����ʱ���ݴ�( 7���ֽ�)
#define		APP_EEPROM_Dev_PowerOn_SysDateTime_ADDR					0x00E0


// ����λ�ü�¼��Ϣ�洢����, 1���ֽ� 
#define		APP_EEPROM_AntennaRecord_TotalNum_ADDR							0x0100

// ����λ�ü�¼��Ϣ�洢������ַ��(���ֻ֧��10����¼)
// ÿһ������λ�ü�¼, ��¼�������̵�ʵ�ʶ������Լ���ص��������ݣ���38�ֽ�
// �������ƣ�16B��+ ���Ǿ��ȱ�ʶ��1B��+���Ǿ��ȣ�4B��+ �ź�Ƶ�ʣ�4B��+ ������ʽ��1B��+
// ��λ�������̶���(4B) + �����������̶���(4B) + �����������̶���(4B)
#define		APP_EEPROM_AntennaRecord_Base_ADDR							0x0110
	#define	APP_EEPROM_AntennaRecord_Offset_ADDR						0x0050



// �豸������־������ 4���ֽ�
#define		APP_EEPROM_OPTLog_TotalNum_ADDR						0x0800

// ��ǰ������־�������4���ֽ�
#define		APP_EEPROM_OPTLog_CurrID_ADDR							0x0804

// �豸�澯��־������ 4���ֽ�
#define		APP_EEPROM_WarnLog_TotalNum_ADDR						0x0810

// ��ǰ�澯��־�������4���ֽ�
#define		APP_EEPROM_WarnLog_CurrID_ADDR							0x0814


// ��Ϊ��̫��������������־
#define		APP_EEPROM_Reset_Flag_ADDR							0x0820



// ���������洢��ַ�� 4���ֽ�
#define		APP_EEPROM_Sat_TotalNum_ADDR			0x0A00			

// ��ǰ����ID�Ŵ洢��ַ��1���ֽ�
#define		APP_EEPROM_Sat_CurrID_ADDR				0x0A08				//  ��ǰѡ������ID�洢��ַ

// ��ǰ���ǲ����洢��ַ
#define		APP_EEPROM_CurrSatParam_ADDR				0x0A20				//  

// ���ǲ����洢��ʼ��ַ,// 0x1010  ~ 0x3010֮��Ϊ���ǲ����洢�ĵط�,
// ÿ�����ǲ����洢27���ֽڣ�
//���Ǳ�ţ�1B��+ �������ƣ�16B��+ ���Ǿ��ȱ�ʶ��1B��+���Ǿ��ȣ�4B��+ �ź�Ƶ�ʣ�4B��+ ������ʽ��1B��
#define		APP_EEPROM_SatParam_Base_ADDR			0x0A40				// ���ǲ����洢��ʼ��ַ
	#define		APP_EEPROM_SatParam_OffSet_ADDR	0x0020				//  ƫ�Ƶ�ַ






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

