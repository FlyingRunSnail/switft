

#ifdef APP_LOG_DEF
	#define APP_LOG_GLOBE
#else
	#define APP_LOG_GLOBE		extern
#endif

#include <ic_n25q128.h>
#include <hw_eeprom.h>
#include <app_pccomm.h>

////////////////////////////////////////////////////////////////////////////////////////////////
// �澯��־�궨��

// ��������澯
#define	LOG_WARN_CODER_AZ			0x010101				// ��λ����������
#define	LOG_WARN_CODER_EL			0x010102				// ��������������
#define	LOG_WARN_CODER_PL			0x010103				// ��������������

// �ű���澯
#define	LOG_WARN_BEACON_ERR						0x010201				// �ű������
#define	LOG_WARN_BEACON_SET_FREQ_FAIL			0x010202				// �ű��Ƶ������ʧ��
#define	LOG_WARN_BEACON_SET_GAIN_FAIL			0x010203				// �ű�����淶Χ����ʧ��

// ���ذ�澯
#define	LOG_WARN_ACU_EEPROM_ERR				0x010301	// ���ذ��ײ����ɶ�д�洢������		
#define	LOG_WARN_ACU_FLASH_ERR					0x010302	// ���ذ��������

// PDU�澯
#define	LOG_WARN_PDU_ERR							0x010401	// ����������Ԫ����

#define	LOG_WARN_PDU_MOTOR_AZ_ERR				0x010411	// ���߷�λ�������
#define	LOG_WARN_PDU_MOTOR_AZ_RIGHT_LIMIT		0x010412	// ��������λ�澯
#define	LOG_WARN_PDU_MOTOR_AZ_LEFT_LIMIT		0x010413	// ��������λ�澯

#define	LOG_WARN_PDU_MOTOR_EL_ERR				0x010421	// ���߸����������
#define	LOG_WARN_PDU_MOTOR_EL_UP_LIMIT			0x010422	// ��������λ�澯
#define	LOG_WARN_PDU_MOTOR_EL_DOWN_LIMIT		0x010423	// ��������λ�澯

#define	LOG_WARN_PDU_MOTOR_PL_ERR				0x010431	// ���߼����������
#define	LOG_WARN_PDU_MOTOR_PL_UP_LIMIT			0x010432	// ����˳ʱ����λ�澯
#define	LOG_WARN_PDU_MOTOR_PL_DOWN_LIMIT		0x010433	// ������ʱ����λ�澯

#define	LOG_WARN_PDU_GPS_ERR					0x010491	// GPS����
#define	LOG_WARN_PDU_GPS_POSITIONING_FAIL		0x010492	// GPS��λʧ��

////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////////
// ������־�궨��

// �Զ�����
#define	LOG_OPT_AUTO_AIM_STAR_BEGIN_BY_PC				0x020111	// PC�������Զ�����
#define	LOG_OPT_AUTO_AIM_STAR_END_BY_PC				0x020112	// PC��ֹͣ�Զ�����
#define	LOG_OPT_AUTO_AIM_STAR_BEGIN_BY_OLED			0x020121	// OLED�������Զ�����
#define	LOG_OPT_AUTO_AIM_STAR_END_BY_OLED				0x020122	// OLED��ֹͣ�Զ�����
#define	LOG_OPT_AUTO_AIM_STAR_END_BY_ACU_LOCK			0x020101	// ����Զ��������̣���������
#define	LOG_OPT_AUTO_AIM_STAR_END_BY_ACU_UNLOCK		0x020102	// ����Զ��������̣�δ��������

// �ֶ�����
#define	LOG_OPT_HAND_AIM_STAR_BEGIN_BY_PC				0x020211	// PC�������ֶ�����
#define	LOG_OPT_HAND_AIM_STAR_END_BY_PC				0x020212	// PC��ֹͣ�ֶ�����
#define	LOG_OPT_HAND_AIM_STAR_BEGIN_BY_OLED			0x020221	// OLED�������ֶ�����
#define	LOG_OPT_HAND_AIM_STAR_END_BY_OLED				0x020222	// OLED��ֹͣ�ֶ�����

// ��̬��¼���ع�
#define	LOG_OPT_ANTENNA_POSE_RECORD_BY_PC				0x020301	// PC����¼���ߵ�ǰ��̬�Լ���ض��ǲ���
#define	LOG_OPT_ANTENNA_POSE_QUE_BY_PC					0x020302	// PC������������̬��ʷ��¼
#define	LOG_OPT_ANTENNA_POSE_DEL_BY_PC					0x020303	// PC��ɾ��������̬��ʷ��¼
#define	LOG_OPT_ANTENNA_POSE_APP_BY_PC					0x020304	// PC��Ӧ��������̬��ʷ��¼
#define	LOG_OPT_ANTENNA_POSE_STOP_BY_PC				0x020305	// PC��ֹͣ������̬��ʷ��¼

#define	LOG_OPT_ANTENNA_POSE_RECORD_BY_OLED			0x020311	// OLED����¼���ߵ�ǰ��̬�Լ���ض��ǲ���
#define	LOG_OPT_ANTENNA_POSE_QUE_BY_OLED				0x020312	// OLED������������̬��ʷ��¼
#define	LOG_OPT_ANTENNA_POSE_DEL_BY_OLED				0x020313	// OLED��ɾ��������̬��ʷ��¼
#define	LOG_OPT_ANTENNA_POSE_APP_BY_OLED				0x020314	// OLED��Ӧ��������̬��ʷ��¼
#define	LOG_OPT_ANTENNA_POSE_STOP_BY_OLED				0x020315	// OLED��ֹͣ������̬��ʷ��¼

#define	LOG_OPT_ANTENNA_POSE_FINISH						0x020320	// �����̬�ع�

// ����ģʽ
#define	LOG_OPT_FOLLOW_STAR_BEGIN_BY_PC				0x020411	// PC����������ģʽ
#define	LOG_OPT_FOLLOW_STAR_END_BY_PC					0x020412	// PC��ֹͣ����ģʽ
#define	LOG_OPT_FOLLOW_STAR_BEGIN_BY_OLED				0x020421	// OLED����������ģʽ
#define	LOG_OPT_FOLLOW_STAR_END_BY_OLED				0x020422	// OLED��ֹͣ����ģʽ
#define	LOG_OPT_FOLLOW_STAR_BEGIN_BY_ACU				0x020401	// ACU ��������ģʽ
#define	LOG_OPT_FOLLOW_STAR_END_BY_ACU					0x020402	// ACU ֹͣ����ģʽ

//����ģʽ
#define	LOG_OPT_STANDBY_MODE_BY_ACU					0x020501	// ���и澯���ϣ�ACU �Զ������������ģʽ

// ���߲�������
#define	LOG_OPT_ANTENNA_PARAM_SET_BY_PC				0x021101	// PC���������߲���

// ����У׼
#define	LOG_OPT_ANTENNA_AZ_TURN_ADJUST					0x021201	// ���߷�λת��У׼
#define	LOG_OPT_ANTENNA_EL_TURN_ADJUST					0x021202	// ���߸���ת��У׼
#define	LOG_OPT_ANTENNA_PL_TURN_ADJUST					0x021203	// ���߼���ת��У׼

#define	LOG_OPT_ANTENNA_AZ_RIGHT_LIMIT_ADJUST			0x021211	// ���߷�λ����λУ׼
#define	LOG_OPT_ANTENNA_AZ_LEFT_LIMIT_ADJUST			0x021212	// ���߷�λ����λУ׼
#define	LOG_OPT_ANTENNA_EL_UP_LIMIT_ADJUST				0x021213	// ���߸�������λУ׼
#define	LOG_OPT_ANTENNA_EL_DOWN_LIMIT_ADJUST			0x021214	// ���߸�������λУ׼
#define	LOG_OPT_ANTENNA_PL_CLOCK_LIMIT_ADJUST			0x021215	// ���߼���˳ʱ����λУ׼
#define	LOG_OPT_ANTENNA_PL_ANTICLOCK_LIMIT_ADJUST		0x021216	// ���߼�����ʱ����λУ׼

#define	LOG_OPT_ANTENNA_CODER_DATA_ADJUST				0x021221	// ������������У׼

// ������ز���
#define	LOG_OPT_ADD_STAR									0x021301	// ��������
#define	LOG_OPT_REVISE_STAR_PARAM						0x021302	// �޸����ǲ���
#define	LOG_OPT_DEL_STAR									0x021303	// ɾ������
#define	LOG_OPT_SEL_STAR_BY_PC							0x021304	// PC��ѡ������
#define	LOG_OPT_SEL_STAR_BY_OLED							0x021314	// OLEDѡ������

// ����վ����
#define	LOG_OPT_SET_LOCALL_PARAM						0x021401	// ���ñ���λ����Ϣ

// ׷�ǲ�������
#define	LOG_OPT_SET_FOLLOW_STATR_MIN_LIMIT_PARAM		0x021501	// ����׷���������ֵ

// ��������
#define	LOG_OPT_NETWORK_IP_ADDR_SET_BY_PC				0x021601	//PC���û�����IP
#define	LOG_OPT_NETWORK_MASK_SET_BY_PC					0x021602	//PC�����ñ�����������
#define	LOG_OPT_NETWORK_GATE_SET_BY_PC					0x021603	//PC�����ñ�������

#define	LOG_OPT_NETWORK_IP_ADDR_SET_BY_OLED			0x021611	//OLED���û�����IP
#define	LOG_OPT_NETWORK_MASK_SET_BY_OLED				0x021612	//OLED���ñ�����������
#define	LOG_OPT_NETWORK_GATE_SET_BY_OLED				0x021613	//OLED���ñ�������

// �豸����
#define	LOG_OPT_DEV_POWER_ON							0x022101	// ��������
#define	LOG_OPT_DEV_FACTORY_RESET						0x022102	// �ָ���������

#define	LOG_OPT_PDU_STOP_ENABLE							0x022111	// ���ü�ͣ����
#define	LOG_OPT_PDU_STOP_DISABLE						0x022112	// ȡ����ͣ����
#define	LOG_OPT_PDU_ADMIN_ENABLE						0x022121	// ����PDU���ؿ���
#define	LOG_OPT_PDU_ADMIN_DISABLE						0x022122	// ȡ��PDU���ؿ���

#define	LOG_OPT_PC_ASK_FOR_ADMIN						0x022131	// PC���������Ȩ��
#define	LOG_OPT_OLED_PERMIN_PC_ADMIN					0x022132	// OLED����PC��ù���Ȩ��
#define	LOG_OPT_OLED_REFUSE_PC_ADMIN					0x022133	// OLED �ܾ�PC��ù���Ȩ��
#define	LOG_OPT_OLED_CANCEL_PC_ADMIN					0x022134	// OLED ȡ��PC�Ĺ���Ȩ��

#define	LOG_OPT_TIMEOUT_ACU_REFUS_PC_ADMIN			0x022135	// PC������Ȩ�����볬ʱδ����/PC����ʱδͨ�ţ�ACUȡ��PC������Ȩ������״̬
#define	LOG_OPT_TIMEOUT_ACU_CANCEL_PC_ADMIN			0x022136	// PC����ʱδͨ�ţ�ACU�Զ�ȡ��PC������Ȩ��

#define	LOG_OPT_TIMEOUT_PC_CANCEL_PC_ADMIN			0x022137	// PC���˳�����Ȩ��
#define	LOG_OPT_TIMEOUT_PC_CANCEL_PC_ADMIN_ASK		0x022138	// PC��ȡ���������Ȩ��

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

