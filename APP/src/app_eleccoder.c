
#define APP_ELEC_ENCODER_DEF

#include <app_eleccoder.h>
#include <math.h>

#define ELEC_ENCODER_DELAY_TIMEOUT		100			//25毫秒延时

#define APP_ELEC_ENCODER_CCBC_ID			0x01			//  长春博辰厂家ID

typedef struct _App_ElecEncoder_RecvDataBuff_Str_
{
	INT8U databuff[32];
	INT16U len;
}App_ElecEncoder_DataBuff;

App_ElecEncoder_DataBuff 	ElecEncoder_DataBuff;

extern INT8U SWIFT_UART5_Predict_RevBytes;
extern INT8U SWIFT_UART2_Predict_RevBytes;
extern INT8U SWIFT_UART1_Predict_RevBytes;


static INT8U Elec_coder_UlinkCnt[3]={0};

OS_SEM APP_PositionElecCoder_Sem;	
OS_SEM APP_CabrageElecCoder_Sem;	
OS_SEM APP_PolarElecCoder_Sem;	

/***********************************************************
**name:	ElecEncoder_Init
**describe: 对电子码盘的初始化
**input:	
**output:	none
**return:  FAIL:   SUCC
**autor:  andiman
**date:
************************************************************/
int APP_ElecEncoder_Init(void)
{
	
	// 方位电子码盘初始化
	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POSITION-1].EleccoderConf.addr = APP_ELEC_ENCODER_POSITION; // 电子码盘地址
	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POSITION-1].EleccoderConf.comm_interface = 0;
	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POSITION-1].EleccoderConf.comm_rate = 9600;		// 波特率		

	APP_PARAM_ElecCode_count_mode_Read(APP_ELEC_ENCODER_POSITION,&ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POSITION-1].EleccoderConf.count_mode);
	//ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POSITION-1].EleccoderConf.count_mode = 0xE1;	// 顺时针加计数
	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POSITION-1].EleccoderConf.bit_num = 17;			//  17位精度						
	// 对应的串口驱动初始化
	SWIFT_UART5_Init(ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POSITION-1].EleccoderConf.comm_rate);



	// 俯仰电子码盘初始化
	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_CABRAGE-1].EleccoderConf.addr = APP_ELEC_ENCODER_CABRAGE; // 电子码盘地址
	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_CABRAGE-1].EleccoderConf.comm_interface = 0;
	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_CABRAGE-1].EleccoderConf.comm_rate = 9600;		// 波特率	
	APP_PARAM_ElecCode_count_mode_Read(APP_ELEC_ENCODER_CABRAGE,&ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_CABRAGE-1].EleccoderConf.count_mode);
	//ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_CABRAGE-1].EleccoderConf.count_mode = 0xE1;	// 顺时针加计数
	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_CABRAGE-1].EleccoderConf.bit_num = 17;			//  17位精度						
	// 对应的串口驱动初始化
	SWIFT_UART2_Init(ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_CABRAGE-1].EleccoderConf.comm_rate);

	// 极化电子码盘初始化
	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POLARIZATION-1].EleccoderConf.addr = APP_ELEC_ENCODER_POLARIZATION; // 电子码盘地址
	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POLARIZATION-1].EleccoderConf.comm_interface = 0;
	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POLARIZATION-1].EleccoderConf.comm_rate = 9600;		// 波特率		
	APP_PARAM_ElecCode_count_mode_Read(APP_ELEC_ENCODER_POLARIZATION,&ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POLARIZATION-1].EleccoderConf.count_mode);
	//ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POLARIZATION-1].EleccoderConf.count_mode = 0xE1;	// 顺时针加计数
	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POLARIZATION-1].EleccoderConf.bit_num = 12;			//  17位精度						
	// 对应的串口驱动初始化
	SWIFT_UART1_Init(ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_CABRAGE-1].EleccoderConf.comm_rate);

	BSP_OS_Sem_Creat(&APP_PositionElecCoder_Sem,"APP_PositionElecCoder_Sem",1);
	BSP_OS_Sem_Creat(&APP_CabrageElecCoder_Sem,"APP_CabrageElecCoder_Sem",1);
	BSP_OS_Sem_Creat(&APP_PolarElecCoder_Sem,"APP_PolarElecCoder_Sem",1);
	
	return SUCC;
}

/***********************************************************
**name:	APP_ElecEncoder_DegreeInt2Data
**describe: 电子码盘角度值转为电子码盘数据值
**input:		addr 电子码盘地址    
				degree 角度值，高16位为角度值整数，低16位为角度值小数(0.0001为最小单位值)
**output:		
**return: 		data  角度值转换后的电子码盘数值	
**autor:  andiman
**date:
************************************************************/
INT32U APP_ElecEncoder_DegreeInt2Data(INT8U addr,INT32U degree)
{
	INT32U data;
	FP32 degree_fp32,degree_fra;
	INT32U degree_int;

	degree_int = (degree & 0xffff0000)>>16;
	degree_fra = ((FP32)(degree & 0x0000ffff))/10000;
	degree_fp32 = degree_int + degree_fra;

	data = degree_fp32 * pow(2,ACUDevInfo.DeviceInfo.EleccoderInfo[addr-1].EleccoderConf.bit_num) / 360;

	return data;
}

/***********************************************************
**name:	APP_ElecEncoder_Data2DegreeInt
**describe: 电子码盘数据值转为电子码盘角度值(整型数据)
**input:		addr 电子码盘地址    
				data 电子码盘数值，
**output:			
**return: 	degree  数值转化后的角度值
**autor:  andiman
**date:
************************************************************/
INT32U APP_ElecEncoder_Data2DegreeInt(INT8U addr,INT32U data)
{
	FP32 degree;
	INT32U degree_int32;
	INT32U degree_int,degree_fra;

	degree = data * 360 / pow(2,ACUDevInfo.DeviceInfo.EleccoderInfo[addr-1].EleccoderConf.bit_num);

	degree_int = floor(degree);
	degree_fra =(INT32U)( (degree - degree_int)*10000);

	degree_int32 = ((degree_int & 0x0000FFFF)<<16) + (degree_fra & 0x0000ffff);

	return degree_int32;
}

/***********************************************************
**name:	APP_ElecEncoder_DegreeFloat2Data
**describe: 电子码盘角度值转为电子码盘数据值
**input:		addr 电子码盘地址    
				degree 角度值，浮点型数据
**output:		
**return: 		data  角度值转换后的电子码盘数值	
**autor:  andiman
**date:
************************************************************/
INT32U APP_ElecEncoder_DegreeFloat2Data(INT8U addr,FP32 degree)
{
	INT32U data;
	
	data = degree * pow(2,ACUDevInfo.DeviceInfo.EleccoderInfo[addr-1].EleccoderConf.bit_num) / 360;

	return data;
}

/***********************************************************
**name:	APP_ElecEncoder_Data2Degree
**describe: 电子码盘数据值转为电子码盘角度值
**input:		addr 电子码盘地址    
				data 电子码盘数值，
**output:			
**return: 	degree  数值转化后的角度值
**autor:  andiman
**date:
************************************************************/
void APP_ElecEncoder_Data2DegreeFloat(INT8U addr,INT32U data ,FP32 *degree)
{
	//FP32 degree;

	*degree = ((FP32)data) * ((FP32)360.0) / ((FP32)pow(2,ACUDevInfo.DeviceInfo.EleccoderInfo[addr-1].EleccoderConf.bit_num));

	//return degree;
}

/***********************************************************
**name:	APP_ElecEncoder_Position_DataAddDir_Set
**describe: 对方位电子码盘的数据增值方向进行设置
**input:	
					dir: 方向	
**output:	none
**return:  FAIL:   SUCC
**autor:  andiman
**date:
************************************************************/
int APP_ElecEncoder_Position_DataAddDir_Set(INT8U dir)
{
	INT8U sendbuff[8]={0};
	INT8U Revbuff[8]={0};
	INT32U len;

	if(BSP_OS_Sem_Pend(&APP_PositionElecCoder_Sem,1000) == FAIL)
		return FAIL;
	
	dir = dir==0xE1?0xE1:0xE2;

	sendbuff[0] = dir;

	// 调用方位电子码盘对应的底层硬件驱动函数
	SWIFT_UART5_Predict_RevBytes = 1;
	SWIFT_UART5_SendBuff(sendbuff,1);

	if( BSP_OS_Sem_Pend(&SWIFT_UART5_Rev_Sem,ELEC_ENCODER_DELAY_TIMEOUT) == -1)
	{
		SWIFT_UART5_Predict_RevBytes = 0;
		BSP_OS_Sem_Post(&APP_PositionElecCoder_Sem);
		return FAIL;
	}
	
	// 对接收到的电子码盘数据进行处理
	SWIFT_UART5_RecvBuff(Revbuff,&len);
	if(( len > SWIFT_UART5_Predict_RevBytes ) || ( Revbuff[0] != sendbuff[0]))
	{
		SWIFT_UART5_Predict_RevBytes = 0;
		BSP_OS_Sem_Post(&APP_PositionElecCoder_Sem);
		return FAIL;
	}

	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POSITION-1].EleccoderConf.count_mode = dir;
	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POSITION-1].EleccoderStat.link_stat = COMM_LINK;
	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POSITION-1].EleccoderStat.dev_stat = WORKSTA_NORMAL;

	BSP_OS_Sem_Post(&APP_PositionElecCoder_Sem);

	return SUCC;
}


/***********************************************************
**name:	APP_ElecEncoder_Position_DataAddDir_Que
**describe: 对方位电子码盘的数据增值方向进行查询
**input:	
					dir: 方向	
**output:	none
**return:  FAIL:   SUCC
**autor:  andiman
**date:
************************************************************/
int APP_ElecEncoder_Position_DataAddDir_Que(INT8U *dir)
{
	INT8U sendbuff[8]={0};
	INT8U Revbuff[8]={0};
	INT32U len;

	if(BSP_OS_Sem_Pend(&APP_PositionElecCoder_Sem,1000) == FAIL)
		return FAIL;


	sendbuff[0] = 0xC9;

	// 调用方位电子码盘对应的底层硬件驱动函数
	SWIFT_UART5_Predict_RevBytes = 1;
	SWIFT_UART5_SendBuff(sendbuff,1);

	if( BSP_OS_Sem_Pend(&SWIFT_UART5_Rev_Sem,ELEC_ENCODER_DELAY_TIMEOUT) == -1)
	{
		SWIFT_UART5_Predict_RevBytes = 0;
		BSP_OS_Sem_Post(&APP_PositionElecCoder_Sem);
		return FAIL;
	}
	
	// 对接收到的电子码盘数据进行处理
	SWIFT_UART5_RecvBuff(Revbuff,&len);
	if(( len > SWIFT_UART5_Predict_RevBytes ) ||  ( (Revbuff[0] & 0xF0 ) != 0xE0))
	{
		SWIFT_UART5_Predict_RevBytes = 0;
		BSP_OS_Sem_Post(&APP_PositionElecCoder_Sem);
		return FAIL;
	}

	*dir = Revbuff[0];

	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POSITION-1].EleccoderConf.count_mode = *dir;
	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POSITION-1].EleccoderStat.link_stat = COMM_LINK;
	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POSITION-1].EleccoderStat.dev_stat = WORKSTA_NORMAL;

	BSP_OS_Sem_Post(&APP_PositionElecCoder_Sem);
	
	return SUCC;
}


/***********************************************************
**name:	APP_ElecEncoder_Cabrage_DataAddDir_Set
**describe: 对俯仰电子码盘的数据增值方向进行设置
**input:	
					dir: 方向	
**output:	none
**return:  FAIL:   SUCC
**autor:  andiman
**date:
************************************************************/
int APP_ElecEncoder_Cabrage_DataAddDir_Set(INT8U dir)
{
	INT8U sendbuff[8]={0};
	INT8U Revbuff[8]={0};
	INT32U len;

	if(BSP_OS_Sem_Pend(&APP_CabrageElecCoder_Sem,1000) == FAIL)
		return FAIL;
	
	dir = dir==0xE1?0xE1:0xE2;

	sendbuff[0] = dir;

	// 调用方位电子码盘对应的底层硬件驱动函数
	SWIFT_UART2_Predict_RevBytes = 1;
	SWIFT_UART2_SendBuff(sendbuff,1);

	if( BSP_OS_Sem_Pend(&SWIFT_UART2_Rev_Sem,ELEC_ENCODER_DELAY_TIMEOUT) == -1)
	{
		SWIFT_UART2_Predict_RevBytes = 0;
		BSP_OS_Sem_Post(&APP_CabrageElecCoder_Sem);
		return FAIL;
	}
	
	// 对接收到的电子码盘数据进行处理
	SWIFT_UART2_RecvBuff(Revbuff,&len);
	if(( len > SWIFT_UART2_Predict_RevBytes ) || ( Revbuff[0] != sendbuff[0]))
	{
		SWIFT_UART2_Predict_RevBytes = 0;
		BSP_OS_Sem_Post(&APP_CabrageElecCoder_Sem);
		return FAIL;
	}

	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_CABRAGE-1].EleccoderConf.count_mode = dir;
	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_CABRAGE-1].EleccoderStat.link_stat = COMM_LINK;
	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_CABRAGE-1].EleccoderStat.dev_stat = WORKSTA_NORMAL;

	BSP_OS_Sem_Post(&APP_CabrageElecCoder_Sem);

	return SUCC;
}


/***********************************************************
**name:	APP_ElecEncoder_Cabrage_DataAddDir_Que
**describe: 对俯仰电子码盘的数据增值方向进行查询
**input:	
					dir: 方向	
**output:	none
**return:  FAIL:   SUCC
**autor:  andiman
**date:
************************************************************/
int APP_ElecEncoder_Cabrage_DataAddDir_Que(INT8U *dir)
{
	INT8U sendbuff[8]={0};
	INT8U Revbuff[8]={0};
	INT32U len;

	if(BSP_OS_Sem_Pend(&APP_CabrageElecCoder_Sem,1000) == FAIL)
		return FAIL;


	sendbuff[0] = 0xC9;

	// 调用方位电子码盘对应的底层硬件驱动函数
	SWIFT_UART2_Predict_RevBytes = 1;
	SWIFT_UART2_SendBuff(sendbuff,1);

	if( BSP_OS_Sem_Pend(&SWIFT_UART2_Rev_Sem,ELEC_ENCODER_DELAY_TIMEOUT) == -1)
	{
		SWIFT_UART2_Predict_RevBytes = 0;
		BSP_OS_Sem_Post(&APP_CabrageElecCoder_Sem);
		return FAIL;
	}
	
	// 对接收到的电子码盘数据进行处理
	SWIFT_UART2_RecvBuff(Revbuff,&len);
	if(( len > SWIFT_UART2_Predict_RevBytes ) || ( (Revbuff[0] & 0xF0 ) != 0xE0))
	{
		SWIFT_UART2_Predict_RevBytes = 0;
		BSP_OS_Sem_Post(&APP_CabrageElecCoder_Sem);
		return FAIL;
	}

	*dir = Revbuff[0];

	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_CABRAGE-1].EleccoderConf.count_mode = *dir;
	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_CABRAGE-1].EleccoderStat.link_stat = COMM_LINK;
	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_CABRAGE-1].EleccoderStat.dev_stat = WORKSTA_NORMAL;

	BSP_OS_Sem_Post(&APP_CabrageElecCoder_Sem);

	return SUCC;
}


/***********************************************************
**name:	APP_ElecEncoder_Polarization_DataAddDir_Set
**describe: 对极化电子码盘的数据增值方向进行设置
**input:	
					dir: 方向	
**output:	none
**return:  FAIL:   SUCC
**autor:  andiman
**date:
************************************************************/
int APP_ElecEncoder_Polarization_DataAddDir_Set(INT8U dir)
{
	INT8U sendbuff[8]={0};
	INT8U Revbuff[8]={0};
	INT32U len;

	if(BSP_OS_Sem_Pend(&APP_PolarElecCoder_Sem,1000) == FAIL)
		return FAIL;
	
	dir = dir==0xE1?0xE1:0xE2;

	sendbuff[0] = dir;

	// 调用方位电子码盘对应的底层硬件驱动函数
	SWIFT_UART1_Predict_RevBytes = 1;
	SWIFT_UART1_SendBuff(sendbuff,1);

	if( BSP_OS_Sem_Pend(&SWIFT_UART1_Rev_Sem,ELEC_ENCODER_DELAY_TIMEOUT) == -1)
	{
		SWIFT_UART1_Predict_RevBytes = 0;
		BSP_OS_Sem_Post(&APP_PolarElecCoder_Sem);
		return FAIL;
	}
	
	// 对接收到的电子码盘数据进行处理
	SWIFT_UART1_RecvBuff(Revbuff,&len);
	if(( len > SWIFT_UART1_Predict_RevBytes ) || ( Revbuff[0] != sendbuff[0]))
	{
		SWIFT_UART1_Predict_RevBytes = 0;
		BSP_OS_Sem_Post(&APP_PolarElecCoder_Sem);
		return FAIL;
	}

	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POLARIZATION-1].EleccoderConf.count_mode = dir;
	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POLARIZATION-1].EleccoderStat.link_stat = COMM_LINK;
	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POLARIZATION-1].EleccoderStat.dev_stat = WORKSTA_NORMAL;

	BSP_OS_Sem_Post(&APP_PolarElecCoder_Sem);

	return SUCC;
}


/***********************************************************
**name:	APP_ElecEncoder_Polarization_DataAddDir_Que
**describe: 对俯仰电子码盘的数据增值方向进行查询
**input:	
					dir: 方向	
**output:	none
**return:  FAIL:   SUCC
**autor:  andiman
**date:
************************************************************/
int APP_ElecEncoder_Polarization_DataAddDir_Que(INT8U *dir)
{
	INT8U sendbuff[8]={0};
	INT8U Revbuff[8]={0};
	INT32U len;

	if(BSP_OS_Sem_Pend(&APP_PolarElecCoder_Sem,1000) == FAIL)
		return FAIL;


	sendbuff[0] = 0xC9;

	// 调用方位电子码盘对应的底层硬件驱动函数
	SWIFT_UART1_Predict_RevBytes = 1;
	SWIFT_UART1_SendBuff(sendbuff,1);

	if( BSP_OS_Sem_Pend(&SWIFT_UART1_Rev_Sem,ELEC_ENCODER_DELAY_TIMEOUT) == -1)
	{
		SWIFT_UART1_Predict_RevBytes = 0;
		BSP_OS_Sem_Post(&APP_PolarElecCoder_Sem);
		return FAIL;
	}
	
	// 对接收到的电子码盘数据进行处理
	SWIFT_UART1_RecvBuff(Revbuff,&len);
	if(( len > SWIFT_UART1_Predict_RevBytes ) ||  ( (Revbuff[0] & 0xF0 ) != 0xE0))
	{
		SWIFT_UART1_Predict_RevBytes = 0;
		BSP_OS_Sem_Post(&APP_PolarElecCoder_Sem);
		return FAIL;
	}

	*dir = Revbuff[0];

	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POLARIZATION-1].EleccoderConf.count_mode = *dir;
	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POLARIZATION-1].EleccoderStat.link_stat = COMM_LINK;
	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POLARIZATION-1].EleccoderStat.dev_stat = WORKSTA_NORMAL;

	BSP_OS_Sem_Post(&APP_PolarElecCoder_Sem);

	return SUCC;
}



/***********************************************************
**name:	APP_ElecEncoder_Position_MakeZero_Set
**describe: 对方位电子码盘的归零操作
**input:		
**output:	none
**return:  FAIL:   SUCC
**autor:  andiman
**date:
************************************************************/
int APP_ElecEncoder_Position_MakeZero_Set(void)
{
	
}

/***********************************************************
**name:	APP_ElecEncoder_Cabrage_MakeZero_Set
**describe: 对俯仰电子码盘的归零操作
**input:		
**output:	none
**return:  FAIL:   SUCC
**autor:  andiman
**date:
************************************************************/
int APP_ElecEncoder_Cabrage_MakeZero_Set(void)
{
	
}

/***********************************************************
**name:	APP_ElecEncoder_Polarization_MakeZero_Set
**describe: 对极化电子码盘的归零操作
**input:		
**output:	none
**return:  FAIL:   SUCC
**autor:  andiman
**date:
************************************************************/
int APP_ElecEncoder_Polarization_MakeZero_Set(void)
{
	
}

/***********************************************************
**name:	APP_ElecEncoder_Position_Addr_Que
**describe: 对方位电子码盘的地址进行查询
**input:			
**output:		addr  查询到的地址
**return:  FAIL:   SUCC
**autor:  andiman
**date:
************************************************************/
int APP_ElecEncoder_Position_Addr_Que(INT8U *addr)
{
	INT8U sendbuff[8]={0};
	INT8U Revbuff[8]={0};
	INT32U len;

	if(BSP_OS_Sem_Pend(&APP_PositionElecCoder_Sem,1000) == FAIL)
		return FAIL;
	
	sendbuff[0] = 0xC7;

	// 调用方位电子码盘对应的底层硬件驱动函数
	SWIFT_UART5_Predict_RevBytes = 1;
	SWIFT_UART5_SendBuff(sendbuff,1);

	if( BSP_OS_Sem_Pend(&SWIFT_UART5_Rev_Sem,ELEC_ENCODER_DELAY_TIMEOUT) == -1)
	{
		SWIFT_UART5_Predict_RevBytes = 0;
		BSP_OS_Sem_Post(&APP_PositionElecCoder_Sem);
		return FAIL;
	}
	
	// 对接收到的电子码盘数据进行处理
	SWIFT_UART5_RecvBuff(Revbuff,&len);
	if( (len >SWIFT_UART5_Predict_RevBytes ) || ( (Revbuff[0] & 0xF0 ) != 0xC0))
	{
		SWIFT_UART5_Predict_RevBytes = 0;
		BSP_OS_Sem_Post(&APP_PositionElecCoder_Sem);
		return FAIL;
	}

	*addr = Revbuff[0] - 0xC0;

	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POSITION-1].EleccoderConf.addr = *addr;
	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POSITION-1].EleccoderStat.link_stat = COMM_LINK;
	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POSITION-1].EleccoderStat.dev_stat = WORKSTA_NORMAL;

	BSP_OS_Sem_Post(&APP_PositionElecCoder_Sem);

	return SUCC;
}

/***********************************************************
**name:	APP_ElecEncoder_Cabrage_Addr_Que
**describe: 对俯仰电子码盘的地址进行查询
**input:			
**output:		addr  查询到的地址
**return:  FAIL:   SUCC
**autor:  andiman
**date:
************************************************************/
int APP_ElecEncoder_Cabrage_Addr_Que(INT8U *addr)
{
	INT8U sendbuff[8]={0};
	INT8U Revbuff[8]={0};
	INT32U len;

	if(BSP_OS_Sem_Pend(&APP_CabrageElecCoder_Sem,1000) == FAIL)
		return FAIL;
	
	sendbuff[0] = 0xC7;

	// 调用电子码盘对应的底层硬件驱动函数
	SWIFT_UART2_Predict_RevBytes = 1;
	SWIFT_UART2_SendBuff(sendbuff,1);

	if( BSP_OS_Sem_Pend(&SWIFT_UART2_Rev_Sem,ELEC_ENCODER_DELAY_TIMEOUT) == -1)
	{
		SWIFT_UART2_Predict_RevBytes = 0;
		BSP_OS_Sem_Post(&APP_CabrageElecCoder_Sem);
		return FAIL;
	}
	
	// 对接收到的电子码盘数据进行处理
	SWIFT_UART2_RecvBuff(Revbuff,&len);
	if( (len >SWIFT_UART2_Predict_RevBytes ) || ( (Revbuff[0] & 0xF0 ) != 0xC0))
	{
		SWIFT_UART2_Predict_RevBytes = 0;
		BSP_OS_Sem_Post(&APP_CabrageElecCoder_Sem);
		return FAIL;
	}

	*addr = Revbuff[0] - 0xC0;

	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_CABRAGE-1].EleccoderConf.addr = *addr;
	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_CABRAGE-1].EleccoderStat.link_stat = COMM_LINK;
	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_CABRAGE-1].EleccoderStat.dev_stat = WORKSTA_NORMAL;

	BSP_OS_Sem_Post(&APP_CabrageElecCoder_Sem);

	return SUCC;
}

/***********************************************************
**name:	APP_ElecEncoder_Polarization_Addr_Que
**describe: 对极化方位电子码盘的地址进行查询
**input:			
**output:		addr  查询到的地址
**return:  FAIL:   SUCC
**autor:  andiman
**date:
************************************************************/
int APP_ElecEncoder_Polarization_Addr_Que(INT8U *addr)
{
	INT8U sendbuff[8]={0};
	INT8U Revbuff[8]={0};
	INT32U len;

	if(BSP_OS_Sem_Pend(&APP_PolarElecCoder_Sem,1000) == FAIL)
		return FAIL;
	
	sendbuff[0] = 0xC7;

	// 调用电子码盘对应的底层硬件驱动函数
	SWIFT_UART1_Predict_RevBytes = 1;
	SWIFT_UART1_SendBuff(sendbuff,1);

	if( BSP_OS_Sem_Pend(&SWIFT_UART1_Rev_Sem,ELEC_ENCODER_DELAY_TIMEOUT) == -1)
	{
		SWIFT_UART1_Predict_RevBytes = 0;
		BSP_OS_Sem_Post(&APP_PolarElecCoder_Sem);
		return FAIL;
	}
	
	// 对接收到的电子码盘数据进行处理
	SWIFT_UART1_RecvBuff(Revbuff,&len);
	if( (len >SWIFT_UART1_Predict_RevBytes ) || ( (Revbuff[0] & 0xF0 ) != 0xC0))
	{
		SWIFT_UART1_Predict_RevBytes = 0;
		BSP_OS_Sem_Post(&APP_PolarElecCoder_Sem);
		return FAIL;
	}

	*addr = Revbuff[0] - 0xC0;

	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POLARIZATION-1].EleccoderConf.addr = *addr;
	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POLARIZATION-1].EleccoderStat.link_stat = COMM_LINK;
	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POLARIZATION-1].EleccoderStat.dev_stat = WORKSTA_NORMAL;

	BSP_OS_Sem_Post(&APP_PolarElecCoder_Sem);

	return SUCC;
}

/***********************************************************
**name:	APP_ElecEncoder_Position_Addr_Set
**describe: 对方位电子码盘的地址设置
**input:	
**output:	none
**return:  FAIL:   SUCC
**autor:  andiman
**date:
************************************************************/
int APP_ElecEncoder_Position_Addr_Set(INT8U addr)
{
	INT8U sendbuff[8]={0};
	INT8U Revbuff[8]={0};
	INT32U len;

	if(BSP_OS_Sem_Pend(&APP_PositionElecCoder_Sem,1000) == FAIL)
		return FAIL;
	
	sendbuff[0] = 0xB0 + addr;

	// 调用方位电子码盘对应的底层硬件驱动函数
	SWIFT_UART5_Predict_RevBytes = 1;
	SWIFT_UART5_SendBuff(sendbuff,1);

	if( BSP_OS_Sem_Pend(&SWIFT_UART5_Rev_Sem,ELEC_ENCODER_DELAY_TIMEOUT) == -1)
	{
		SWIFT_UART5_Predict_RevBytes = 0;
		BSP_OS_Sem_Post(&APP_PositionElecCoder_Sem);
		return FAIL;
	}
	
	// 对接收到的电子码盘数据进行处理
	SWIFT_UART5_RecvBuff(Revbuff,&len);
	if( (len >SWIFT_UART5_Predict_RevBytes ) || (Revbuff[0] != sendbuff[0]))
	{
		SWIFT_UART5_Predict_RevBytes = 0;
		BSP_OS_Sem_Post(&APP_PositionElecCoder_Sem);
		return FAIL;
	}

	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POSITION-1].EleccoderConf.addr = addr;
	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POSITION-1].EleccoderStat.link_stat = COMM_LINK;
	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POSITION-1].EleccoderStat.dev_stat = WORKSTA_NORMAL;

	BSP_OS_Sem_Post(&APP_PositionElecCoder_Sem);

	return SUCC;
}

/***********************************************************
**name:	APP_ElecEncoder_Cabrage_Addr_Set
**describe: 对俯仰电子码盘的地址设置
**input:	
					old_addr:				旧地址
**output:	none
**return:  FAIL:   SUCC
**autor:  andiman
**date:
************************************************************/
int APP_ElecEncoder_Cabrage_Addr_Set(INT8U addr)
{
	INT8U sendbuff[8]={0};
	INT8U Revbuff[8]={0};
	INT32U len;

	if(BSP_OS_Sem_Pend(&APP_CabrageElecCoder_Sem,1000) == FAIL)
		return FAIL;
	
	sendbuff[0] = 0xB0 + addr;

	// 调用电子码盘对应的底层硬件驱动函数
	SWIFT_UART2_Predict_RevBytes = 1;
	SWIFT_UART2_SendBuff(sendbuff,1);

	if( BSP_OS_Sem_Pend(&SWIFT_UART2_Rev_Sem,ELEC_ENCODER_DELAY_TIMEOUT) == -1)
	{
		SWIFT_UART2_Predict_RevBytes = 0;
		BSP_OS_Sem_Post(&APP_CabrageElecCoder_Sem);
		return FAIL;
	}
	
	// 对接收到的电子码盘数据进行处理
	SWIFT_UART2_RecvBuff(Revbuff,&len);
	if( (len >SWIFT_UART2_Predict_RevBytes ) ||  (Revbuff[0] != sendbuff[0]))
	{
		SWIFT_UART2_Predict_RevBytes = 0;
		BSP_OS_Sem_Post(&APP_CabrageElecCoder_Sem);
		return FAIL;
	}

	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_CABRAGE-1].EleccoderConf.addr = addr;
	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_CABRAGE-1].EleccoderStat.link_stat = COMM_LINK;
	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_CABRAGE-1].EleccoderStat.dev_stat = WORKSTA_NORMAL;

	BSP_OS_Sem_Post(&APP_CabrageElecCoder_Sem);

	return SUCC;
}

/***********************************************************
**name:	APP_ElecEncoder_Polarization_Addr_Set
**describe: 对极化电子码盘的地址设置
**input:	
					old_addr:				旧地址
**output:	none
**return:  FAIL:   SUCC
**autor:  andiman
**date:
************************************************************/
int APP_ElecEncoder_Polarization_Addr_Set(INT8U addr)
{
	INT8U sendbuff[8]={0};
	INT8U Revbuff[8]={0};
	INT32U len;

	if(BSP_OS_Sem_Pend(&APP_PolarElecCoder_Sem,1000) == FAIL)
		return FAIL;
	
	sendbuff[0] = 0xB0 + addr;

	// 调用电子码盘对应的底层硬件驱动函数
	SWIFT_UART1_Predict_RevBytes = 1;

	SWIFT_UART1_SendBuff(sendbuff,1);

	if( BSP_OS_Sem_Pend(&SWIFT_UART1_Rev_Sem,ELEC_ENCODER_DELAY_TIMEOUT) == -1)
	{
		SWIFT_UART1_Predict_RevBytes = 0;
		BSP_OS_Sem_Post(&APP_PolarElecCoder_Sem);
		return FAIL;
	}
	
	// 对接收到的电子码盘数据进行处理
	SWIFT_UART1_RecvBuff(Revbuff,&len);
	if( (len >SWIFT_UART1_Predict_RevBytes ) ||  (Revbuff[0] != sendbuff[0]))
	{
		SWIFT_UART1_Predict_RevBytes = 0;
		BSP_OS_Sem_Post(&APP_PolarElecCoder_Sem);
		return FAIL;
	}

	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POLARIZATION-1].EleccoderConf.addr = addr;
	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POLARIZATION-1].EleccoderStat.link_stat = COMM_LINK;
	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POLARIZATION-1].EleccoderStat.dev_stat = WORKSTA_NORMAL;

	BSP_OS_Sem_Post(&APP_PolarElecCoder_Sem);
	
	return SUCC;
}


/***********************************************************
**name:	APP_ElecEncoder_Position_CurrData_Get
**describe: 对方位电子码盘的实时数据获取
**input:		
**output:			data:  获取到的实时数据
**return:  FAIL:   SUCC
**autor:  andiman
**date:
************************************************************/
int APP_ElecEncoder_Position_CurrData_Get(INT32U *data)
{
	INT8U sendbuff[8]={0};
	INT8U Revbuff[8]={0};
	INT32U len;
	INT8U checknum = 0;

	if(BSP_OS_Sem_Pend(&APP_PositionElecCoder_Sem,100) == FAIL)
		return FAIL;
	
	sendbuff[0] = 0xA0 + ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POSITION-1].EleccoderConf.addr;

	// 调用方位电子码盘对应的底层硬件驱动函数
	if( ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POSITION-1].EleccoderConf.bit_num > 16 )
		SWIFT_UART5_Predict_RevBytes = 6;
	else
		SWIFT_UART5_Predict_RevBytes = 5;
		
	SWIFT_UART5_SendBuff(sendbuff,1);

	if( BSP_OS_Sem_Pend(&SWIFT_UART5_Rev_Sem,ELEC_ENCODER_DELAY_TIMEOUT) == -1)
	{
		Elec_coder_UlinkCnt[APP_ELEC_ENCODER_POSITION-1]++;
		if( Elec_coder_UlinkCnt[APP_ELEC_ENCODER_POSITION-1] > ELEC_ENCODER_UNLINK_NUM)
			ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POSITION-1].EleccoderStat.link_stat = COMM_UNLINK;
		SWIFT_UART5_Predict_RevBytes = 0;
		BSP_OS_Sem_Post(&APP_PositionElecCoder_Sem);
		return FAIL;
	}
	
	// 对接收到的电子码盘数据进行处理
	SWIFT_UART5_RecvBuff(Revbuff,&len);
	if( (len >SWIFT_UART5_Predict_RevBytes ) || ( Revbuff[0]  != 0x5A))
	{
		Elec_coder_UlinkCnt[APP_ELEC_ENCODER_POSITION-1]++;
		if( Elec_coder_UlinkCnt[APP_ELEC_ENCODER_POSITION-1] > ELEC_ENCODER_UNLINK_NUM)
			ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POSITION-1].EleccoderStat.link_stat = COMM_UNLINK;
		SWIFT_UART5_Predict_RevBytes = 0;
		BSP_OS_Sem_Post(&APP_PositionElecCoder_Sem);
		return FAIL;
	}

	if( ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POSITION-1].EleccoderConf.bit_num > 16 )
	{
		checknum = (Revbuff[0] + Revbuff[1] + Revbuff[2] + Revbuff[3] + Revbuff[4]) & 0x7F;
		if( checknum != Revbuff[5] )
		{
			Elec_coder_UlinkCnt[APP_ELEC_ENCODER_POSITION-1]++;
			if( Elec_coder_UlinkCnt[APP_ELEC_ENCODER_POSITION-1] > ELEC_ENCODER_UNLINK_NUM)
				ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POSITION-1].EleccoderStat.link_stat = COMM_UNLINK;
			SWIFT_UART5_Predict_RevBytes = 0;
			BSP_OS_Sem_Post(&APP_PositionElecCoder_Sem);
			return FAIL;
		}
		*data = (Revbuff[2] << 16) + (Revbuff[3] << 8) + Revbuff[4];
	}
	else
	{
		checknum = (Revbuff[0] + Revbuff[1] + Revbuff[2] + Revbuff[3]) & 0x7F;
		if( checknum != Revbuff[4] )
		{
			Elec_coder_UlinkCnt[APP_ELEC_ENCODER_POSITION-1]++;
			if( Elec_coder_UlinkCnt[APP_ELEC_ENCODER_POSITION-1] > ELEC_ENCODER_UNLINK_NUM)
				ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POSITION-1].EleccoderStat.link_stat = COMM_UNLINK;
			SWIFT_UART5_Predict_RevBytes = 0;
			BSP_OS_Sem_Post(&APP_PositionElecCoder_Sem);
			return FAIL;
		}
		*data = ((Revbuff[2]& 0x0F) << 8) + Revbuff[3];
	}

	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POSITION-1].EleccoderData.data = *data;
	APP_ElecEncoder_Data2DegreeFloat(APP_ELEC_ENCODER_POSITION,*data,&ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POSITION-1].EleccoderData.degree);
	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POSITION-1].EleccoderStat.link_stat = COMM_LINK;
	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POSITION-1].EleccoderStat.dev_stat = WORKSTA_NORMAL;
	Elec_coder_UlinkCnt[APP_ELEC_ENCODER_POSITION-1]=0;

	BSP_OS_Sem_Post(&APP_PositionElecCoder_Sem);

	return SUCC;
}

/***********************************************************
**name:	APP_ElecEncoder_Cabrage_CurrData_Get
**describe: 对俯仰电子码盘的实时数据获取
**input:		
**output:			data:  获取到的实时数据
**return:  FAIL:   SUCC
**autor:  andiman
**date:
************************************************************/
int APP_ElecEncoder_Cabrage_CurrData_Get(INT32U *data)
{
	INT8U sendbuff[8]={0};
	INT8U Revbuff[8]={0};
	INT32U len;
	INT8U checknum = 0;

	if(BSP_OS_Sem_Pend(&APP_CabrageElecCoder_Sem,100) == FAIL)
		return FAIL;
	
	sendbuff[0] = 0xA0 + ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_CABRAGE-1].EleccoderConf.addr;

	// 调用方位电子码盘对应的底层硬件驱动函数
	if( ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_CABRAGE-1].EleccoderConf.bit_num > 16 )
		SWIFT_UART2_Predict_RevBytes = 6;
	else
		SWIFT_UART2_Predict_RevBytes = 5;
		
	SWIFT_UART2_SendBuff(sendbuff,1);

	if( BSP_OS_Sem_Pend(&SWIFT_UART2_Rev_Sem,ELEC_ENCODER_DELAY_TIMEOUT) == -1)
	{
		Elec_coder_UlinkCnt[APP_ELEC_ENCODER_CABRAGE-1]++;
		if( Elec_coder_UlinkCnt[APP_ELEC_ENCODER_CABRAGE-1] > ELEC_ENCODER_UNLINK_NUM)
			ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_CABRAGE-1].EleccoderStat.link_stat = COMM_UNLINK;
		SWIFT_UART2_Predict_RevBytes = 0;
		BSP_OS_Sem_Post(&APP_CabrageElecCoder_Sem);
		return FAIL;
	}
	
	// 对接收到的电子码盘数据进行处理
	SWIFT_UART2_RecvBuff(Revbuff,&len);
	if( (len >SWIFT_UART2_Predict_RevBytes ) || ( Revbuff[0]  != 0x5A))
	{
		Elec_coder_UlinkCnt[APP_ELEC_ENCODER_CABRAGE-1]++;
		if( Elec_coder_UlinkCnt[APP_ELEC_ENCODER_CABRAGE-1] > ELEC_ENCODER_UNLINK_NUM)
			ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_CABRAGE-1].EleccoderStat.link_stat = COMM_UNLINK;
		SWIFT_UART2_Predict_RevBytes = 0;
		BSP_OS_Sem_Post(&APP_CabrageElecCoder_Sem);
		return FAIL;
	}

	if( ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_CABRAGE-1].EleccoderConf.bit_num > 16 )
	{
		checknum = (Revbuff[0] + Revbuff[1] + Revbuff[2] + Revbuff[3] + Revbuff[4]) & 0x7F;
		if( checknum != Revbuff[5] )
		{
			Elec_coder_UlinkCnt[APP_ELEC_ENCODER_CABRAGE-1]++;
			if( Elec_coder_UlinkCnt[APP_ELEC_ENCODER_CABRAGE-1] > ELEC_ENCODER_UNLINK_NUM)
				ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_CABRAGE-1].EleccoderStat.link_stat = COMM_UNLINK;
			SWIFT_UART2_Predict_RevBytes = 0;
			BSP_OS_Sem_Post(&APP_CabrageElecCoder_Sem);
			return FAIL;
		}
		*data = (Revbuff[2] << 16) + (Revbuff[3] << 8) + Revbuff[4];
	}
	else
	{
		checknum = (Revbuff[0] + Revbuff[1] + Revbuff[2] + Revbuff[3]) & 0x7F;
		if( checknum != Revbuff[4] )
		{
			Elec_coder_UlinkCnt[APP_ELEC_ENCODER_CABRAGE-1]++;
			if( Elec_coder_UlinkCnt[APP_ELEC_ENCODER_CABRAGE-1] > ELEC_ENCODER_UNLINK_NUM)
				ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_CABRAGE-1].EleccoderStat.link_stat = COMM_UNLINK;
			SWIFT_UART2_Predict_RevBytes = 0;
			BSP_OS_Sem_Post(&APP_CabrageElecCoder_Sem);
			return FAIL;
		}
		*data = ((Revbuff[2]& 0x0F) << 8) + Revbuff[3];
	}

	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_CABRAGE-1].EleccoderData.data = *data;
//	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_CABRAGE-1].EleccoderData.degree = 
		APP_ElecEncoder_Data2DegreeFloat(APP_ELEC_ENCODER_CABRAGE,*data,&ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_CABRAGE-1].EleccoderData.degree);
	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_CABRAGE-1].EleccoderStat.link_stat = COMM_LINK;
	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_CABRAGE-1].EleccoderStat.dev_stat = WORKSTA_NORMAL;
	Elec_coder_UlinkCnt[APP_ELEC_ENCODER_CABRAGE-1]=0;

	BSP_OS_Sem_Post(&APP_CabrageElecCoder_Sem);

	return SUCC;
}

/***********************************************************
**name:	APP_ElecEncoder_Polarization_CurrData_Get
**describe: 对极化电子码盘的实时数据获取
**input:		
**output:			data:  获取到的实时数据
**return:  FAIL:   SUCC
**autor:  andiman
**date:
************************************************************/
int APP_ElecEncoder_Polarization_CurrData_Get(INT32U *data)
{
	INT8U sendbuff[8]={0};
	INT8U Revbuff[8]={0};
	INT32U len;
	INT8U checknum = 0;

	if(BSP_OS_Sem_Pend(&APP_PolarElecCoder_Sem,100) == FAIL)
		return FAIL;
	
	sendbuff[0] = 0xA0 + ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POLARIZATION-1].EleccoderConf.addr;

	// 调用方位电子码盘对应的底层硬件驱动函数
	if( ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POLARIZATION-1].EleccoderConf.bit_num > 16 )
		SWIFT_UART1_Predict_RevBytes = 6;
	else
		SWIFT_UART1_Predict_RevBytes = 5;
		
	
	SWIFT_UART1_SendBuff(sendbuff,1);

	if( BSP_OS_Sem_Pend(&SWIFT_UART1_Rev_Sem,ELEC_ENCODER_DELAY_TIMEOUT) == -1)
	{
		Elec_coder_UlinkCnt[APP_ELEC_ENCODER_POLARIZATION-1]++;
		if( Elec_coder_UlinkCnt[APP_ELEC_ENCODER_POLARIZATION-1] > ELEC_ENCODER_UNLINK_NUM)
			ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POLARIZATION-1].EleccoderStat.link_stat = COMM_UNLINK;
		SWIFT_UART1_Predict_RevBytes = 0;
		BSP_OS_Sem_Post(&APP_PolarElecCoder_Sem);
		return FAIL;
	}
	
	// 对接收到的电子码盘数据进行处理
	SWIFT_UART1_RecvBuff(Revbuff,&len);
	if( (len >SWIFT_UART1_Predict_RevBytes ) || ( Revbuff[0]  != 0x5A))
	{
		Elec_coder_UlinkCnt[APP_ELEC_ENCODER_POLARIZATION-1]++;
		if( Elec_coder_UlinkCnt[APP_ELEC_ENCODER_POLARIZATION-1] > ELEC_ENCODER_UNLINK_NUM)
			ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POLARIZATION-1].EleccoderStat.link_stat = COMM_UNLINK;
		SWIFT_UART1_Predict_RevBytes = 0;
		BSP_OS_Sem_Post(&APP_PolarElecCoder_Sem);
		return FAIL;
	}

	if( ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POLARIZATION-1].EleccoderConf.bit_num > 16 )
	{
		checknum = (Revbuff[0] + Revbuff[1] + Revbuff[2] + Revbuff[3] + Revbuff[4]) & 0x7F;
		if( checknum != Revbuff[5] )
		{
			Elec_coder_UlinkCnt[APP_ELEC_ENCODER_POLARIZATION-1]++;
			if( Elec_coder_UlinkCnt[APP_ELEC_ENCODER_POLARIZATION-1] > ELEC_ENCODER_UNLINK_NUM)
				ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POLARIZATION-1].EleccoderStat.link_stat = COMM_UNLINK;
			SWIFT_UART1_Predict_RevBytes = 0;
			BSP_OS_Sem_Post(&APP_PolarElecCoder_Sem);
			return FAIL;
		}
		*data = (Revbuff[2] << 16) + (Revbuff[3] << 8) + Revbuff[4];
	}
	else
	{
		checknum = (Revbuff[0] + Revbuff[1] + Revbuff[2] + Revbuff[3]) & 0x7F;
		if( checknum != Revbuff[4] )
		{
			Elec_coder_UlinkCnt[APP_ELEC_ENCODER_POLARIZATION-1]++;
			if( Elec_coder_UlinkCnt[APP_ELEC_ENCODER_POLARIZATION-1] > ELEC_ENCODER_UNLINK_NUM)
				ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POLARIZATION-1].EleccoderStat.link_stat = COMM_UNLINK;
			SWIFT_UART1_Predict_RevBytes = 0;
			BSP_OS_Sem_Post(&APP_PolarElecCoder_Sem);
			return FAIL;
		}
		*data = ((Revbuff[2]& 0x0F) << 8) + Revbuff[3];
	}

	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POLARIZATION-1].EleccoderData.data = *data;
	//ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POLARIZATION-1].EleccoderData.degree = 
		APP_ElecEncoder_Data2DegreeFloat(APP_ELEC_ENCODER_POLARIZATION,*data,&ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POLARIZATION-1].EleccoderData.degree);
	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POLARIZATION-1].EleccoderStat.link_stat = COMM_LINK;
	ACUDevInfo.DeviceInfo.EleccoderInfo[APP_ELEC_ENCODER_POLARIZATION-1].EleccoderStat.dev_stat = WORKSTA_NORMAL;
	Elec_coder_UlinkCnt[APP_ELEC_ENCODER_POLARIZATION-1]=0;

	BSP_OS_Sem_Post(&APP_PolarElecCoder_Sem);

	return SUCC;
}


/***********************************************************
**name:	APP_ElecEncoder_DataAddDir_Set
**describe: 对电子码盘的数据增值方向进行设置
**input:	
					dir: 方向	
**output:	none
**return:  FAIL:   SUCC
**autor:  andiman
**date:
************************************************************/
int APP_ElecEncoder_DataAddDir_Set(INT8U addr,INT8U dir)
{
	if( addr == APP_ELEC_ENCODER_POSITION )
		return (APP_ElecEncoder_Position_DataAddDir_Set(dir));
	else if( addr == APP_ELEC_ENCODER_CABRAGE )
		return (APP_ElecEncoder_Cabrage_DataAddDir_Set(dir));
	else
		return (APP_ElecEncoder_Polarization_DataAddDir_Set(dir));
}

/***********************************************************
**name:	APP_ElecEncoder_DataAddDir_Get
**describe:
**input:	
					dir: 方向	
**output:	none
**return:  FAIL:   SUCC
**autor:  andiman
**date:
************************************************************/
int APP_ElecEncoder_DataAddDir_Get(INT8U addr,INT8U *dir)
{
	if( addr == APP_ELEC_ENCODER_POSITION )
		return (APP_ElecEncoder_Position_DataAddDir_Que(dir));
	else if( addr == APP_ELEC_ENCODER_CABRAGE )
		return (APP_ElecEncoder_Cabrage_DataAddDir_Que(dir));
	else
		return (APP_ElecEncoder_Polarization_DataAddDir_Que(dir));
}

/***********************************************************
**name:	APP_ElecEncoder_DataAddDir_Set
**describe: 对电子码盘的数据增值方向进行设置
**input:	
					dir: 方向	
**output:	none
**return:  FAIL:   SUCC
**autor:  andiman
**date:
************************************************************/
int APP_ElecEncoder_CurrData_Get(INT8U addr,INT32U *data)
{
	if( addr == APP_ELEC_ENCODER_POSITION )
		return (APP_ElecEncoder_Position_CurrData_Get(data));
	else if( addr == APP_ELEC_ENCODER_CABRAGE )
		return (APP_ElecEncoder_Cabrage_CurrData_Get(data));
	else
		return (APP_ElecEncoder_Polarization_CurrData_Get(data));
}

