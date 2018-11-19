


#define APP_LOG_DEF

#include <app_log.h>
#include <devdef.h>



#define	OPTLOG_INFO_FLASH_ADDR_BEGIN	0x00000000
#define	OPTLOG_INFO_FLASH_ADDR_END		0x000FA000

#define	OPTLOG_FLASH_ADDR_BEGIN			0x000FA000
#define 	OPTLOG_FLASH_ADDR_END			0x002EE000	


#define	WARNLOG_INFO_FLASH_ADDR_BEGIN		0x002EE000
#define	WARNLOG_INFO_FLASH_ADDR_END		0x003E8000

#define	WARNLOG_FLASH_ADDR_BEGIN		0x003E8000
#define 	WARNLOG_FLASH_ADDR_END			0x005DC000	





OS_SEM Log_Sem;

static INT8U  Log_Err_cnt = 0;

/***********************************************************
**name:	APP_LOG_Init
**describe:  
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_LOG_Init(void)
{
	INT32U flash_addr;
	INT32U log_record_addr;
	INT8U readbuff[64]={0},writebuff[64]={0},index = 0;
	INT32U readbufflen,writebufflen;
	INT32U log_currinfo_addr;		// 当前最近的一条日志记录地址
	INT32U log_currinfo_index;		// 当前最近的一条日志记录编号
	INT32U log_currinfo_size;			// 当前最近的一条日志记录的大小，

	INT32U log_beforeinfo_addr;		// 之前的一条日志记录地址
	INT32U log_beforeinfo_index;		// 之前的一条日志记录编号
	INT32U log_beforeinfo_size;		// 之前的一条日志记录的大小，

	INT8U checksum8bit=0,head,tail;

	INT32U log_total_num;			// 日志总条数
	INT16U  optlog_cmd;
	INT32U i;

	strLogFlash OptLogFlash,WarnLogFlash;

	Log_Err_cnt = 0;

// 	MICRON_FLASH_BulkErase();

	MICRON_FLASH_ReadID(readbuff);

	if( (readbuff[0] == 0x20 ) && (readbuff[1] == 0xBA )&&(readbuff[2] == 0x18 ))
	{
		ACUDevInfo.DeviceInfo.SPIFlashInfo.link_stat = COMM_LINK;
		 ACUDevInfo.DeviceInfo.SPIFlashInfo.comm_stat = COMM_LINK;
	}
	else
	{
		ACUDevInfo.DeviceInfo.SPIFlashInfo.link_stat = COMM_UNLINK;
		ACUDevInfo.DeviceInfo.SPIFlashInfo.comm_stat = COMM_UNLINK;
	}

	BSP_OS_Sem_Creat(&Log_Sem,"Log Sem",1);

	memset(&OptLogInfo,0,sizeof(OptLogInfo));
	//  查询操作日志，初始化操作日志的结构体数据
	for( log_record_addr = OPTLOG_INFO_FLASH_ADDR_BEGIN; log_record_addr < OPTLOG_INFO_FLASH_ADDR_END;log_record_addr = log_record_addr + LOG_SYS_RECORD_SIZE)
	{
		head = MICRON_FLASH_B3ReadDataByte(log_record_addr);
		MICRON_FLASH_B3Read4ByteData(log_record_addr+1,&log_total_num);
		MICRON_FLASH_B3Read4ByteData(log_record_addr+5,&log_currinfo_index);
		MICRON_FLASH_B3Read4ByteData(log_record_addr+9,&log_currinfo_addr);
		MICRON_FLASH_B3Read4ByteData(log_record_addr+13,&log_currinfo_size);
		MICRON_FLASH_B3Read4ByteData(log_record_addr+17,&log_beforeinfo_index);
		MICRON_FLASH_B3Read4ByteData(log_record_addr+21,&log_beforeinfo_addr);
		MICRON_FLASH_B3Read4ByteData(log_record_addr+25,&log_beforeinfo_size);
		tail = MICRON_FLASH_B3ReadDataByte(log_record_addr+29);

		if( (head == 0xBC ) && (tail == 0xCB))
		{
			OptLogInfo.log_sys_addr = log_record_addr;
			OptLogInfo.log_total_num = log_total_num;
			OptLogInfo.currlog_addr = log_currinfo_addr;
			OptLogInfo.currlog_index = log_currinfo_index;
			OptLogInfo.currlog_size = log_currinfo_size;
			OptLogInfo.beforelog_addr = log_beforeinfo_addr;
			OptLogInfo.beforelog_index = log_beforeinfo_index;
			OptLogInfo.beforelog_size = log_beforeinfo_size;
		}
		else
		{
			if( (head == 0xFF ) && ( tail == 0xFF))
			{
				break;
			}
			else if (head != 0xFF )
			{
				MICRON_FLASH_B3SubSectorErase(OPTLOG_INFO_FLASH_ADDR_BEGIN);
				index  = 0;
				writebuff[index++] = 0xBC;

				writebuff[index++] = (OptLogInfo.log_total_num &  0xFF000000)>>24;
				writebuff[index++] = (OptLogInfo.log_total_num &  0x00FF0000)>>16;
				writebuff[index++] = (OptLogInfo.log_total_num &  0x0000FF00)>>8;
				writebuff[index++] = (OptLogInfo.log_total_num &  0x000000FF);

				writebuff[index++] = (OptLogInfo.currlog_index &  0xFF000000)>>24;
				writebuff[index++] = (OptLogInfo.currlog_index &  0x00FF0000)>>16;
				writebuff[index++] = (OptLogInfo.currlog_index &  0x0000FF00)>>8;
				writebuff[index++] = (OptLogInfo.currlog_index &  0x000000FF);

				writebuff[index++] = (OptLogInfo.currlog_addr &  0xFF000000)>>24;
				writebuff[index++] = (OptLogInfo.currlog_addr &  0x00FF0000)>>16;
				writebuff[index++] = (OptLogInfo.currlog_addr &  0x0000FF00)>>8;
				writebuff[index++] = (OptLogInfo.currlog_addr &  0x000000FF);

				writebuff[index++] = (OptLogInfo.currlog_size &  0xFF000000)>>24;
				writebuff[index++] = (OptLogInfo.currlog_size &  0x00FF0000)>>16;
				writebuff[index++] = (OptLogInfo.currlog_size &  0x0000FF00)>>8;
				writebuff[index++] = (OptLogInfo.currlog_size &  0x000000FF);

				writebuff[index++] = (OptLogInfo.beforelog_index &  0xFF000000)>>24;
				writebuff[index++] = (OptLogInfo.beforelog_index &  0x00FF0000)>>16;
				writebuff[index++] = (OptLogInfo.beforelog_index &  0x0000FF00)>>8;
				writebuff[index++] = (OptLogInfo.beforelog_index &  0x000000FF);

				writebuff[index++] = (OptLogInfo.beforelog_addr &  0xFF000000)>>24;
				writebuff[index++] = (OptLogInfo.beforelog_addr &  0x00FF0000)>>16;
				writebuff[index++] = (OptLogInfo.beforelog_addr &  0x0000FF00)>>8;
				writebuff[index++] = (OptLogInfo.beforelog_addr &  0x000000FF);

				writebuff[index++] = (OptLogInfo.beforelog_size &  0xFF000000)>>24;
				writebuff[index++] = (OptLogInfo.beforelog_size &  0x00FF0000)>>16;
				writebuff[index++] = (OptLogInfo.beforelog_size &  0x0000FF00)>>8;
				writebuff[index++] = (OptLogInfo.beforelog_size &  0x000000FF);

				writebuff[index++] = 0xCB;

				if(MICRON_FLASH_B3WriteDataBuff(OPTLOG_INFO_FLASH_ADDR_BEGIN,writebuff,index) == SUCC)
				{
					BSP_Delay_s(1);
					BSP_SoftReset();
				}
				else
				{
					MICRON_FLASH_B3SubSectorErase(OPTLOG_INFO_FLASH_ADDR_BEGIN);
					BSP_Delay_ms(100);
					if(MICRON_FLASH_B3WriteDataBuff(OPTLOG_INFO_FLASH_ADDR_BEGIN,writebuff,index) == SUCC)
					{
						BSP_Delay_s(1);
						BSP_SoftReset();
					}
					else
					{
						ACUDevInfo.DeviceInfo.SPIFlashInfo.link_stat = COMM_UNLINK;
						ACUDevInfo.DeviceInfo.SPIFlashInfo.comm_stat = COMM_UNLINK;
					}
				}
				
			}
			break;
		}
		
	}
	if( OptLogInfo.log_total_num == 0x00)
	{
		
		OptLogInfo.currlog_addr = OPTLOG_FLASH_ADDR_BEGIN;
		OptLogInfo.currlog_index = 1;
		OptLogInfo.currlog_size = 0;
		OptLogInfo.log_sys_addr = OPTLOG_INFO_FLASH_ADDR_BEGIN;
	}

	memset(&WarnLogInfo,0,sizeof(WarnLogInfo));
	//  查询告警日志，初始化告警日志的结构体数据
	for( log_record_addr = WARNLOG_INFO_FLASH_ADDR_BEGIN; log_record_addr < WARNLOG_INFO_FLASH_ADDR_END;log_record_addr = log_record_addr + LOG_SYS_RECORD_SIZE)
	{
		head = MICRON_FLASH_B3ReadDataByte(log_record_addr);
		MICRON_FLASH_B3Read4ByteData(log_record_addr+1,&log_total_num);
		MICRON_FLASH_B3Read4ByteData(log_record_addr+5,&log_currinfo_index);
		MICRON_FLASH_B3Read4ByteData(log_record_addr+9,&log_currinfo_addr);
		MICRON_FLASH_B3Read4ByteData(log_record_addr+13,&log_currinfo_size);
		MICRON_FLASH_B3Read4ByteData(log_record_addr+17,&log_beforeinfo_index);
		MICRON_FLASH_B3Read4ByteData(log_record_addr+21,&log_beforeinfo_addr);
		MICRON_FLASH_B3Read4ByteData(log_record_addr+25,&log_beforeinfo_size);
		tail = MICRON_FLASH_B3ReadDataByte(log_record_addr+29);

		if( (head == 0xBC ) && (tail == 0xCB))
		{
			WarnLogInfo.log_sys_addr = log_record_addr;
			WarnLogInfo.log_total_num = log_total_num;
			WarnLogInfo.currlog_addr = log_currinfo_addr;
			WarnLogInfo.currlog_index = log_currinfo_index;
			WarnLogInfo.currlog_size = log_currinfo_size;
			WarnLogInfo.beforelog_addr = log_beforeinfo_addr;
			WarnLogInfo.beforelog_index = log_beforeinfo_index;
			WarnLogInfo.beforelog_size = log_beforeinfo_size;
		}
		else
		{
			if( (head == 0xFF ) && ( tail == 0xFF))
			{
				break;
			}
			else if (head != 0xFF )
			{
				MICRON_FLASH_B3SubSectorErase(WARNLOG_INFO_FLASH_ADDR_BEGIN);
				index  = 0;
				writebuff[index++] = 0xBC;

				writebuff[index++] = (WarnLogInfo.log_total_num &  0xFF000000)>>24;
				writebuff[index++] = (WarnLogInfo.log_total_num &  0x00FF0000)>>16;
				writebuff[index++] = (WarnLogInfo.log_total_num &  0x0000FF00)>>8;
				writebuff[index++] = (WarnLogInfo.log_total_num &  0x000000FF);

				writebuff[index++] = (WarnLogInfo.currlog_index &  0xFF000000)>>24;
				writebuff[index++] = (WarnLogInfo.currlog_index &  0x00FF0000)>>16;
				writebuff[index++] = (WarnLogInfo.currlog_index &  0x0000FF00)>>8;
				writebuff[index++] = (WarnLogInfo.currlog_index &  0x000000FF);

				writebuff[index++] = (WarnLogInfo.currlog_addr &  0xFF000000)>>24;
				writebuff[index++] = (WarnLogInfo.currlog_addr &  0x00FF0000)>>16;
				writebuff[index++] = (WarnLogInfo.currlog_addr &  0x0000FF00)>>8;
				writebuff[index++] = (WarnLogInfo.currlog_addr &  0x000000FF);

				writebuff[index++] = (WarnLogInfo.currlog_size &  0xFF000000)>>24;
				writebuff[index++] = (WarnLogInfo.currlog_size &  0x00FF0000)>>16;
				writebuff[index++] = (WarnLogInfo.currlog_size &  0x0000FF00)>>8;
				writebuff[index++] = (WarnLogInfo.currlog_size &  0x000000FF);

				writebuff[index++] = (WarnLogInfo.beforelog_index &  0xFF000000)>>24;
				writebuff[index++] = (WarnLogInfo.beforelog_index &  0x00FF0000)>>16;
				writebuff[index++] = (WarnLogInfo.beforelog_index &  0x0000FF00)>>8;
				writebuff[index++] = (WarnLogInfo.beforelog_index &  0x000000FF);

				writebuff[index++] = (WarnLogInfo.beforelog_addr &  0xFF000000)>>24;
				writebuff[index++] = (WarnLogInfo.beforelog_addr &  0x00FF0000)>>16;
				writebuff[index++] = (WarnLogInfo.beforelog_addr &  0x0000FF00)>>8;
				writebuff[index++] = (WarnLogInfo.beforelog_addr &  0x000000FF);

				writebuff[index++] = (WarnLogInfo.beforelog_size &  0xFF000000)>>24;
				writebuff[index++] = (WarnLogInfo.beforelog_size &  0x00FF0000)>>16;
				writebuff[index++] = (WarnLogInfo.beforelog_size &  0x0000FF00)>>8;
				writebuff[index++] = (WarnLogInfo.beforelog_size &  0x000000FF);

				writebuff[index++] = 0xCB;

				if(MICRON_FLASH_B3WriteDataBuff(WARNLOG_INFO_FLASH_ADDR_BEGIN,writebuff,index) == SUCC)
				{
					BSP_Delay_s(1);
					BSP_SoftReset();
				}
				else
				{
					MICRON_FLASH_B3SubSectorErase(WARNLOG_INFO_FLASH_ADDR_BEGIN);
					BSP_Delay_ms(100);
					if(MICRON_FLASH_B3WriteDataBuff(WARNLOG_INFO_FLASH_ADDR_BEGIN,writebuff,index) == SUCC)
					{
						BSP_Delay_s(1);
						BSP_SoftReset();
					}
					else
					{
						ACUDevInfo.DeviceInfo.SPIFlashInfo.link_stat = COMM_UNLINK;
						ACUDevInfo.DeviceInfo.SPIFlashInfo.comm_stat = COMM_UNLINK;
					}
				}
				
			}
			break;
		}
	}

	if( WarnLogInfo.log_total_num == 0x00)
	{
		
		WarnLogInfo.currlog_addr = WARNLOG_FLASH_ADDR_BEGIN;
		WarnLogInfo.currlog_index = 1;
		WarnLogInfo.currlog_size = 0;
		WarnLogInfo.log_sys_addr =WARNLOG_INFO_FLASH_ADDR_BEGIN;
	}
}



/***********************************************************
**name:	APP_OptLOG_Save2Flash
**describe: 存储操作日志信息
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OptLOG_Save2Flash(INT8U *buff , INT8U len)
{
	INT32U log_currinfo_index,log_total_num,log_size;
	INT32U flash_addr;
	INT8U logbuff[256]={0};
	INT16U index=0,i;
	INT8U checkcum8bit = 0;
	
	strLogFlash	OptLog;
	strSysDateTime		SysDateTime;

	if( ACUDevInfo.DeviceInfo.SPIFlashInfo.comm_stat == 0x01)
		return FAIL;

	flash_addr = OptLogInfo.currlog_addr + OptLogInfo.currlog_size ;

	if( (flash_addr < OPTLOG_FLASH_ADDR_BEGIN ) || ( flash_addr >= OPTLOG_FLASH_ADDR_END))
		flash_addr = OPTLOG_FLASH_ADDR_BEGIN;

	if( OptLogInfo.log_total_num == 0 )
	{
		flash_addr = OPTLOG_FLASH_ADDR_BEGIN;
		log_currinfo_index = 1;
		log_size = len+26;

		OptLogInfo.currlog_addr = OPTLOG_FLASH_ADDR_BEGIN;
		OptLogInfo.currlog_index = 1;
		OptLogInfo.currlog_size = len+26;
	}
	else
	{
		flash_addr = OptLogInfo.currlog_addr + OptLogInfo.currlog_size ;
		if( flash_addr >= OPTLOG_FLASH_ADDR_END)
		{
			flash_addr = OPTLOG_FLASH_ADDR_BEGIN;
			if( log_total_num > 64000 )
				log_total_num -= 128;
		}

		log_currinfo_index = OptLogInfo.currlog_index+ 1;
		log_total_num = OptLogInfo.log_total_num + 1;
		log_size = len+26;
	}
	
	// 判断是否出了一个扇区,则需要进行一次扇区擦除
	if( ((flash_addr +log_size)  & 0xFFFFF000 ) != ( flash_addr & 0xFFFFF000 ))
	{
		MICRON_FLASH_B3SubSectorErase(flash_addr +log_size);
	}

	memset(logbuff,0,sizeof(logbuff));
	MICRON_FLASH_B3ReadDataBuff(flash_addr,logbuff,log_size);
	for( i=0;i<log_size;i++ )
	{
		if( logbuff[i] != 0xFF )
		{
			flash_addr += SPI_FLASH_SectorSize;
			MICRON_FLASH_B3SubSectorErase(flash_addr);
			break;
		}
	}


	logbuff[index++] = 0xBC;
	
	logbuff[index++] = (log_size & 0xFF000000)>>24;
	logbuff[index++] = (log_size & 0x00FF0000)>>16;
	logbuff[index++] = (log_size & 0x0000FF00)>>8;
	logbuff[index++] = (log_size & 0x000000FF);
	
	logbuff[index++] = (log_currinfo_index & 0xFF000000)>>24;
	logbuff[index++] = (log_currinfo_index & 0x00FF0000)>>16;
	logbuff[index++] = (log_currinfo_index & 0x0000FF00)>>8;
	logbuff[index++] = (log_currinfo_index & 0x000000FF);

	DATETIME_ReadFromRtcIC(&ACUDevInfo.SysDateTime);
	TOOL_DateTime2Bytebuff(ACUDevInfo.SysDateTime, &logbuff[index], 7);
	index += 7;
	
	memcpy( &logbuff[index],buff,len);
	index += len;

	logbuff[index++] = (OptLogInfo.currlog_addr & 0xFF000000)>>24;
	logbuff[index++] = (OptLogInfo.currlog_addr & 0x00FF0000)>>16;
	logbuff[index++] = (OptLogInfo.currlog_addr & 0x0000FF00)>>8;
	logbuff[index++] = (OptLogInfo.currlog_addr & 0x000000FF);
	
	logbuff[index++] = (OptLogInfo.currlog_size & 0xFF000000)>>24;
	logbuff[index++] = (OptLogInfo.currlog_size & 0x00FF0000)>>16;
	logbuff[index++] = (OptLogInfo.currlog_size & 0x0000FF00)>>8;
	logbuff[index++] = (OptLogInfo.currlog_size & 0x000000FF);

	checkcum8bit = TOOL_CheckSum8BitGet(logbuff,index);

	logbuff[index++] = checkcum8bit;

	logbuff[index++] = 0xCB;

	if( MICRON_FLASH_B3WriteDataBuff(flash_addr,logbuff,index) == FAIL)
		return FAIL;

	
	OptLogInfo.beforelog_addr = OptLogInfo.currlog_addr;
	OptLogInfo.beforelog_index = OptLogInfo.currlog_index;
	OptLogInfo.beforelog_size = OptLogInfo.currlog_size;
	OptLogInfo.currlog_addr = flash_addr;
	OptLogInfo.currlog_index = log_currinfo_index;
	OptLogInfo.currlog_size = log_size;
	OptLogInfo.log_total_num++;
	if( OptLogInfo.log_total_num > 1)
		OptLogInfo.log_sys_addr += LOG_SYS_RECORD_SIZE;
	else
		OptLogInfo.log_sys_addr = OPTLOG_INFO_FLASH_ADDR_BEGIN;

	if( OptLogInfo.log_sys_addr >= OPTLOG_INFO_FLASH_ADDR_END )
		OptLogInfo.log_sys_addr = OPTLOG_INFO_FLASH_ADDR_BEGIN;

	index = 0;
	logbuff[index++] = 0xBC;

	logbuff[index++] = (OptLogInfo.log_total_num & 0xFF000000)>>24;
	logbuff[index++] = (OptLogInfo.log_total_num & 0x00FF0000)>>16;
	logbuff[index++] = (OptLogInfo.log_total_num & 0x0000FF00)>>8;
	logbuff[index++] = (OptLogInfo.log_total_num & 0x000000FF);

	logbuff[index++] = (OptLogInfo.currlog_index & 0xFF000000)>>24;
	logbuff[index++] = (OptLogInfo.currlog_index & 0x00FF0000)>>16;
	logbuff[index++] = (OptLogInfo.currlog_index & 0x0000FF00)>>8;
	logbuff[index++] = (OptLogInfo.currlog_index & 0x000000FF);

	logbuff[index++] = (OptLogInfo.currlog_addr & 0xFF000000)>>24;
	logbuff[index++] = (OptLogInfo.currlog_addr & 0x00FF0000)>>16;
	logbuff[index++] = (OptLogInfo.currlog_addr & 0x0000FF00)>>8;
	logbuff[index++] = (OptLogInfo.currlog_addr & 0x000000FF);

	logbuff[index++] = (OptLogInfo.currlog_size & 0xFF000000)>>24;
	logbuff[index++] = (OptLogInfo.currlog_size & 0x00FF0000)>>16;
	logbuff[index++] = (OptLogInfo.currlog_size & 0x0000FF00)>>8;
	logbuff[index++] = (OptLogInfo.currlog_size & 0x000000FF);

	

	logbuff[index++] = (OptLogInfo.beforelog_index & 0xFF000000)>>24;
	logbuff[index++] = (OptLogInfo.beforelog_index & 0x00FF0000)>>16;
	logbuff[index++] = (OptLogInfo.beforelog_index & 0x0000FF00)>>8;
	logbuff[index++] = (OptLogInfo.beforelog_index & 0x000000FF);

	logbuff[index++] = (OptLogInfo.beforelog_addr & 0xFF000000)>>24;
	logbuff[index++] = (OptLogInfo.beforelog_addr & 0x00FF0000)>>16;
	logbuff[index++] = (OptLogInfo.beforelog_addr & 0x0000FF00)>>8;
	logbuff[index++] = (OptLogInfo.beforelog_addr & 0x000000FF);

	logbuff[index++] = (OptLogInfo.beforelog_size & 0xFF000000)>>24;
	logbuff[index++] = (OptLogInfo.beforelog_size & 0x00FF0000)>>16;
	logbuff[index++] = (OptLogInfo.beforelog_size & 0x0000FF00)>>8;
	logbuff[index++] = (OptLogInfo.beforelog_size & 0x000000FF);

	logbuff[index++] = 0xCB;

	flash_addr = OptLogInfo.log_sys_addr;

	if( ((flash_addr + LOG_SYS_RECORD_SIZE)  & 0xFFFFF000 ) != ( flash_addr & 0xFFFFF000 ))
	{
		MICRON_FLASH_B3SubSectorErase(flash_addr + LOG_SYS_RECORD_SIZE);
	}
	
	if( MICRON_FLASH_B3WriteDataBuff(flash_addr,logbuff,index) == FAIL)
	{
		flash_addr += LOG_SYS_RECORD_SIZE;
		if( MICRON_FLASH_B3WriteDataBuff(flash_addr,logbuff,index) == FAIL)
			return FAIL;
	}
	
	OptLogInfo.log_sys_addr = flash_addr;

	return SUCC;
}


/***********************************************************
**name:	APP_OptLOG_Save2Flash
**describe: 存储操作日志信息
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OptLOG_Save(INT32U code,INT8U *buff,INT8U len)
{
	INT8U logbuff[256]={0};
	int sta;

	if( BSP_OS_Sem_Pend(&Log_Sem,1000) == FAIL)
	{
		Log_Err_cnt++;	
		if( Log_Err_cnt >= 2 )
		{
			APP_LOG_Reset();
		}
		return FAIL;
	}
	
	TOOL_INT32Data2Bytebuff(code,logbuff,1);
	memcpy( &logbuff[4],buff,len);
	sta = APP_OptLOG_Save2Flash(logbuff,len+4);

	if( BSP_OS_Sem_Post(&Log_Sem) == FAIL)
		return FAIL;

	if( sta == FAIL)
	{
		Log_Err_cnt++;	
		if( Log_Err_cnt >= 2 )
		{
			APP_LOG_Reset();
		}
			
	}
	else
		Log_Err_cnt = 0;
		
	return sta;
}

/***********************************************************
**name:	APP_OptLOG_SaveCode
**describe: 存储操作日志编码
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OptLOG_SaveCode(INT32U code)
{
	INT8U logbuff[256]={0};
	int sta;

	if( BSP_OS_Sem_Pend(&Log_Sem,1000) == FAIL)
	{
		Log_Err_cnt++;	
		if( Log_Err_cnt >= 2 )
		{
			APP_LOG_Reset();
		}
		return FAIL;
	}

	TOOL_INT32Data2Bytebuff(code,logbuff,1);
	sta = APP_OptLOG_Save2Flash(logbuff,4);

	if( BSP_OS_Sem_Post(&Log_Sem) == FAIL)
		return FAIL;

	if( sta == FAIL)
	{
		Log_Err_cnt++;	
		if( Log_Err_cnt >= 2 )
		{
			APP_LOG_Reset();
		}
			
	}
	else
		Log_Err_cnt = 0;
	
	return sta;
}



/***********************************************************
**name:	APP_WarnLOG_Save2Flash
**describe: 存储告警日志信息
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_WarnLOG_Save2Flash(INT8U *buff , INT8U len)
{
	INT32U log_currinfo_index,log_total_num,log_size;
	INT32U flash_addr;
	INT8U logbuff[256]={0};
	INT16U index=0,i;
	INT8U checkcum8bit = 0;
	
	strLogFlash	OptLog;
	strSysDateTime		SysDateTime;

	if( ACUDevInfo.DeviceInfo.SPIFlashInfo.comm_stat == 0x01)
		return FAIL;

	flash_addr = WarnLogInfo.currlog_addr + WarnLogInfo.currlog_size ;

	if( (flash_addr < WARNLOG_FLASH_ADDR_BEGIN ) || ( flash_addr >= WARNLOG_FLASH_ADDR_END))
		flash_addr = WARNLOG_FLASH_ADDR_BEGIN;

	if( WarnLogInfo.log_total_num == 0 )
	{
		flash_addr = WARNLOG_FLASH_ADDR_BEGIN;
		log_currinfo_index = 1;
		log_size = len+26;

		WarnLogInfo.currlog_addr = WARNLOG_FLASH_ADDR_BEGIN;
		WarnLogInfo.currlog_index = 1;
		WarnLogInfo.currlog_size = len+26;
	}
	else
	{
		flash_addr = WarnLogInfo.currlog_addr + WarnLogInfo.currlog_size ;
		if( flash_addr >= WARNLOG_FLASH_ADDR_END)
		{
			flash_addr = WARNLOG_FLASH_ADDR_BEGIN;
			if( log_total_num > 64000 )
				log_total_num -= 128;
		}

		log_currinfo_index = WarnLogInfo.currlog_index+ 1;
		log_total_num = WarnLogInfo.log_total_num + 1;
		log_size = len+26;
	}
	
	// 判断是否出了一个扇区,则需要进行一次扇区擦除
	if( ((flash_addr +log_size)  & 0xFFFFF000 ) != ( flash_addr & 0xFFFFF000 ))
	{
		MICRON_FLASH_B3SubSectorErase(flash_addr +log_size);
	}

	memset(logbuff,0,sizeof(logbuff));
	MICRON_FLASH_B3ReadDataBuff(flash_addr,logbuff,log_size);
	for( i=0;i<log_size;i++ )
	{
		if( logbuff[i] != 0xFF )
		{
			flash_addr += SPI_FLASH_SectorSize;
			MICRON_FLASH_B3SubSectorErase(flash_addr);
			break;
		}
	}


	logbuff[index++] = 0xBC;
	
	logbuff[index++] = (log_size & 0xFF000000)>>24;
	logbuff[index++] = (log_size & 0x00FF0000)>>16;
	logbuff[index++] = (log_size & 0x0000FF00)>>8;
	logbuff[index++] = (log_size & 0x000000FF);
	
	logbuff[index++] = (log_currinfo_index & 0xFF000000)>>24;
	logbuff[index++] = (log_currinfo_index & 0x00FF0000)>>16;
	logbuff[index++] = (log_currinfo_index & 0x0000FF00)>>8;
	logbuff[index++] = (log_currinfo_index & 0x000000FF);

	DATETIME_ReadFromRtcIC(&ACUDevInfo.SysDateTime);
	TOOL_DateTime2Bytebuff(ACUDevInfo.SysDateTime, &logbuff[index], 7);
	index += 7;
	
	memcpy( &logbuff[index],buff,len);
	index += len;

	logbuff[index++] = (WarnLogInfo.currlog_addr & 0xFF000000)>>24;
	logbuff[index++] = (WarnLogInfo.currlog_addr & 0x00FF0000)>>16;
	logbuff[index++] = (WarnLogInfo.currlog_addr & 0x0000FF00)>>8;
	logbuff[index++] = (WarnLogInfo.currlog_addr & 0x000000FF);
	
	logbuff[index++] = (WarnLogInfo.currlog_size & 0xFF000000)>>24;
	logbuff[index++] = (WarnLogInfo.currlog_size & 0x00FF0000)>>16;
	logbuff[index++] = (WarnLogInfo.currlog_size & 0x0000FF00)>>8;
	logbuff[index++] = (WarnLogInfo.currlog_size & 0x000000FF);

	checkcum8bit = TOOL_CheckSum8BitGet(logbuff,index);

	logbuff[index++] = checkcum8bit;

	logbuff[index++] = 0xCB;

	if( MICRON_FLASH_B3WriteDataBuff(flash_addr,logbuff,index) == FAIL)
		return FAIL;

	
	WarnLogInfo.beforelog_addr = WarnLogInfo.currlog_addr;
	WarnLogInfo.beforelog_index = WarnLogInfo.currlog_index;
	WarnLogInfo.beforelog_size = WarnLogInfo.currlog_size;
	WarnLogInfo.currlog_addr = flash_addr;
	WarnLogInfo.currlog_index = log_currinfo_index;
	WarnLogInfo.currlog_size = log_size;
	WarnLogInfo.log_total_num++;
	if( WarnLogInfo.log_total_num > 1)
		WarnLogInfo.log_sys_addr += LOG_SYS_RECORD_SIZE;
	else
		WarnLogInfo.log_sys_addr = WARNLOG_INFO_FLASH_ADDR_BEGIN;

	if( WarnLogInfo.log_sys_addr >= WARNLOG_INFO_FLASH_ADDR_END )
		WarnLogInfo.log_sys_addr = WARNLOG_INFO_FLASH_ADDR_BEGIN;

	index = 0;
	logbuff[index++] = 0xBC;

	logbuff[index++] = (WarnLogInfo.log_total_num & 0xFF000000)>>24;
	logbuff[index++] = (WarnLogInfo.log_total_num & 0x00FF0000)>>16;
	logbuff[index++] = (WarnLogInfo.log_total_num & 0x0000FF00)>>8;
	logbuff[index++] = (WarnLogInfo.log_total_num & 0x000000FF);

	logbuff[index++] = (WarnLogInfo.currlog_index & 0xFF000000)>>24;
	logbuff[index++] = (WarnLogInfo.currlog_index & 0x00FF0000)>>16;
	logbuff[index++] = (WarnLogInfo.currlog_index & 0x0000FF00)>>8;
	logbuff[index++] = (WarnLogInfo.currlog_index & 0x000000FF);

	logbuff[index++] = (WarnLogInfo.currlog_addr & 0xFF000000)>>24;
	logbuff[index++] = (WarnLogInfo.currlog_addr & 0x00FF0000)>>16;
	logbuff[index++] = (WarnLogInfo.currlog_addr & 0x0000FF00)>>8;
	logbuff[index++] = (WarnLogInfo.currlog_addr & 0x000000FF);

	logbuff[index++] = (WarnLogInfo.currlog_size & 0xFF000000)>>24;
	logbuff[index++] = (WarnLogInfo.currlog_size & 0x00FF0000)>>16;
	logbuff[index++] = (WarnLogInfo.currlog_size & 0x0000FF00)>>8;
	logbuff[index++] = (WarnLogInfo.currlog_size & 0x000000FF);

	

	logbuff[index++] = (WarnLogInfo.beforelog_index & 0xFF000000)>>24;
	logbuff[index++] = (WarnLogInfo.beforelog_index & 0x00FF0000)>>16;
	logbuff[index++] = (WarnLogInfo.beforelog_index & 0x0000FF00)>>8;
	logbuff[index++] = (WarnLogInfo.beforelog_index & 0x000000FF);

	logbuff[index++] = (WarnLogInfo.beforelog_addr & 0xFF000000)>>24;
	logbuff[index++] = (WarnLogInfo.beforelog_addr & 0x00FF0000)>>16;
	logbuff[index++] = (WarnLogInfo.beforelog_addr & 0x0000FF00)>>8;
	logbuff[index++] = (WarnLogInfo.beforelog_addr & 0x000000FF);

	logbuff[index++] = (WarnLogInfo.beforelog_size & 0xFF000000)>>24;
	logbuff[index++] = (WarnLogInfo.beforelog_size & 0x00FF0000)>>16;
	logbuff[index++] = (WarnLogInfo.beforelog_size & 0x0000FF00)>>8;
	logbuff[index++] = (WarnLogInfo.beforelog_size & 0x000000FF);

	logbuff[index++] = 0xCB;

	flash_addr = WarnLogInfo.log_sys_addr;

	if( ((flash_addr + LOG_SYS_RECORD_SIZE)  & 0xFFFFF000 ) != ( flash_addr & 0xFFFFF000 ))
	{
		MICRON_FLASH_B3SubSectorErase(flash_addr + LOG_SYS_RECORD_SIZE);
	}
	
	if( MICRON_FLASH_B3WriteDataBuff(flash_addr,logbuff,index) == FAIL)
	{
		flash_addr += LOG_SYS_RECORD_SIZE;
		if( MICRON_FLASH_B3WriteDataBuff(flash_addr,logbuff,index) == FAIL)
			return FAIL;
	}
	
	WarnLogInfo.log_sys_addr = flash_addr;

	return SUCC;
}


/***********************************************************
**name:	APP_WarnLOG_Save
**describe: 存储日志信息
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_WarnLOG_Save(INT32U code,INT8U *buff,INT8U len)
{
	INT8U logbuff[256]={0};
	int sta;

	if( BSP_OS_Sem_Pend(&Log_Sem,1000) == FAIL)
	{
		Log_Err_cnt++;	
		if( Log_Err_cnt >= 2 )
		{
			APP_LOG_Reset();
		}
		return FAIL;
	}

	TOOL_INT32Data2Bytebuff(code,logbuff,1);
	memcpy( &logbuff[4],buff,len+4);
	sta = APP_WarnLOG_Save2Flash(logbuff,len+4);

	if( BSP_OS_Sem_Post(&Log_Sem) == FAIL)
		return FAIL;

	if( sta == FAIL)
	{
		Log_Err_cnt++;	
		if( Log_Err_cnt >= 2 )
		{
			APP_LOG_Reset();
		}
			
	}
	else
		Log_Err_cnt = 0;
		
	return sta;
}

/***********************************************************
**name:	APP_WarnLOG_SaveCode
**describe: 存储日志信息
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_WarnLOG_SaveCode(INT32U code)
{
	INT8U logbuff[256]={0};
	int sta;

	if( BSP_OS_Sem_Pend(&Log_Sem,1000) == FAIL)
	{
		Log_Err_cnt++;	
		if( Log_Err_cnt >= 2 )
		{
			APP_LOG_Reset();
		}
		return FAIL;
	}

	TOOL_INT32Data2Bytebuff(code,logbuff,1);
	sta = APP_WarnLOG_Save2Flash(logbuff,4);

	if( BSP_OS_Sem_Post(&Log_Sem) == FAIL)
		return FAIL;

	if( sta == FAIL)
	{
		Log_Err_cnt++;	
		if( Log_Err_cnt >= 2 )
		{
			APP_LOG_Reset();
		}
			
	}
	else
		Log_Err_cnt = 0;

	return sta;
}



/***********************************************************
**name:	APP_LOG_Read1Log
**describe: 读取一条日志信息
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_LOG_Read1Log(strLogInfo*CurrLogInfo,INT8U *logbuff,INT8U *len)
{
	INT32U index = 0;
	INT8U i,checksum8bit=0;

	if( ACUDevInfo.DeviceInfo.SPIFlashInfo.comm_stat == 0x01)
		return FAIL;

	if(( CurrLogInfo->currlog_size <= 0 ) || ( CurrLogInfo->currlog_size  > 1000))
		return FAIL;

	// 读取对应的log信息
	MICRON_FLASH_B3ReadDataBuff(CurrLogInfo->currlog_addr,logbuff,CurrLogInfo->currlog_size);

	if( logbuff[0] != 0xBC)
		return FAIL;

	checksum8bit = TOOL_CheckSum8BitGet(logbuff,CurrLogInfo->currlog_size-2);

	if( checksum8bit != logbuff[CurrLogInfo->currlog_size-2])
		return FAIL;


	*len = CurrLogInfo->currlog_size;
	return SUCC;;
}

/***********************************************************
**name:	APP_LOG_SatParam2Logbuff
**describe: 将卫星参数存进log缓存
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void APP_LOG_SatParam2Logbuff(strSatParam 	SatParam,INT8U *logbuff,INT8U *len)
{
	INT8U index=0;
	INT32U data_INT32U;

	memcpy(&logbuff[index],SatParam.name,sizeof(SatParam.name));
	index += sizeof(SatParam.name);

	logbuff[index++] = SatParam.lon_flag;

	data_INT32U = (INT32U)(SatParam.lon * 1000);
	logbuff[index++] = (data_INT32U & 0xFF000000)>>24 ;
	logbuff[index++] = (data_INT32U & 0x00FF0000)>>16 ;
	logbuff[index++] = (data_INT32U & 0x0000FF00)>>8 ;
	logbuff[index++] = (data_INT32U & 0x000000FF);

	data_INT32U = SatParam.freq;
	logbuff[index++] = (data_INT32U & 0xFF000000)>>24 ;
	logbuff[index++] = (data_INT32U & 0x00FF0000)>>16 ;
	logbuff[index++] = (data_INT32U & 0x0000FF00)>>8 ;
	logbuff[index++] = (data_INT32U & 0x000000FF);

	logbuff[index++] = SatParam.polar_type;

	*len = index;
}


/***********************************************************
**name:	APP_LOG_LocallParam2Logbuff
**describe: 将本地参数存进log缓存
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void APP_LOG_LocallParam2Logbuff(strHandLocalPositionParam 	LocalPositionParam,INT8U *logbuff,INT8U *len)
{
	INT8U index=0;
	INT32U data_INT32U;


	logbuff[index++] = LocalPositionParam.lon_flag;
	data_INT32U = (INT32U)(LocalPositionParam.lon * 1000);
	logbuff[index++] = (data_INT32U & 0xFF000000)>>24 ;
	logbuff[index++] = (data_INT32U & 0x00FF0000)>>16 ;
	logbuff[index++] = (data_INT32U & 0x0000FF00)>>8 ;
	logbuff[index++] = (data_INT32U & 0x000000FF);

	logbuff[index++] = LocalPositionParam.lat_flag;
	data_INT32U = (INT32U)(LocalPositionParam.lat * 1000);
	logbuff[index++] = (data_INT32U & 0xFF000000)>>24 ;
	logbuff[index++] = (data_INT32U & 0x00FF0000)>>16 ;
	logbuff[index++] = (data_INT32U & 0x0000FF00)>>8 ;
	logbuff[index++] = (data_INT32U & 0x000000FF);


	data_INT32U = LocalPositionParam.height;
	logbuff[index++] = (data_INT32U & 0xFF000000)>>24 ;
	logbuff[index++] = (data_INT32U & 0x00FF0000)>>16 ;
	logbuff[index++] = (data_INT32U & 0x0000FF00)>>8 ;
	logbuff[index++] = (data_INT32U & 0x000000FF);

	*len = index;
}


/***********************************************************
**name:	APP_LOG_AntennaParam2Logbuff
**describe: 将天线参数存进log缓存
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void APP_LOG_AntennaParam2Logbuff(strAntennaParam 	AntennaParam,INT8U *logbuff,INT8U *len)
{
	INT8U index=0;
	INT32U data_INT32U;

	logbuff[index++] = AntennaParam.type;

	data_INT32U = (INT32U)( AntennaParam.offset_angle * 1000);
	logbuff[index++] = (data_INT32U & 0xFF000000)>>24 ;
	logbuff[index++] = (data_INT32U & 0x00FF0000)>>16 ;
	logbuff[index++] = (data_INT32U & 0x0000FF00)>>8 ;
	logbuff[index++] = (data_INT32U & 0x000000FF);

	logbuff[index++] = AntennaParam.caliber;

	data_INT32U = (INT32U)( AntennaParam.lo_freq);
	logbuff[index++] = (data_INT32U & 0xFF000000)>>24 ;
	logbuff[index++] = (data_INT32U & 0x00FF0000)>>16 ;
	logbuff[index++] = (data_INT32U & 0x0000FF00)>>8 ;
	logbuff[index++] = (data_INT32U & 0x000000FF);

	logbuff[index++] = AntennaParam.gain_range_flag;

	logbuff[index++] = AntennaParam.elec_coder_num;

	*len = index;
}

/***********************************************************
**name:	APP_LOG_AntennaPoseData2Logbuff
**describe: 将将天线姿态位置信息存进log缓存
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void APP_LOG_AntennaPoseData2Logbuff(FP32 degree_AZ,FP32 degree_EL,FP32 degree_PL,INT8U *logbuff,INT8U *len)
{
	INT8U index=0;
	//INT32U data_INT32U;
	INT32S data_INT32S;

	data_INT32S = (INT32S)( degree_AZ * 1000);
	logbuff[index++] = (data_INT32S & 0xFF000000)>>24 ;
	logbuff[index++] = (data_INT32S & 0x00FF0000)>>16 ;
	logbuff[index++] = (data_INT32S & 0x0000FF00)>>8 ;
	logbuff[index++] = (data_INT32S & 0x000000FF);

	data_INT32S = (INT32S)( degree_EL * 1000);
	logbuff[index++] = (data_INT32S & 0xFF000000)>>24 ;
	logbuff[index++] = (data_INT32S & 0x00FF0000)>>16 ;
	logbuff[index++] = (data_INT32S & 0x0000FF00)>>8 ;
	logbuff[index++] = (data_INT32S & 0x000000FF);

	data_INT32S = (INT32S)( degree_PL * 1000);
	logbuff[index++] = (data_INT32S & 0xFF000000)>>24 ;
	logbuff[index++] = (data_INT32S & 0x00FF0000)>>16 ;
	logbuff[index++] = (data_INT32S & 0x0000FF00)>>8 ;
	logbuff[index++] = (data_INT32S & 0x000000FF);

	*len = index;
}



/***********************************************************
**name:	APP_OptLOG_Save_PowerOn
**describe: 存储开机操作日志信息
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OptLOG_Save_PowerOn(void)
{
	INT8U logbuff[256]={0};
	int sta;

	
	DATETIME_ReadFromRtcIC(&ACUDevInfo.SysDateTime);
	sta = APP_OptLOG_Save(LOG_OPT_DEV_POWER_ON,logbuff,0);
	
	return sta;
}


/***********************************************************
**name:	APP_OptLOG_Save_SatOpt
**describe: 存储卫星相关操作日志信息
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OptLOG_Save_SatOpt(INT32U code,strSatParam 	SatParam)
{
	INT8U logbuff[256]={0};
	INT8U len;

	APP_LOG_SatParam2Logbuff(SatParam,logbuff,&len);

	return (APP_OptLOG_Save(code,logbuff,len));
}

/***********************************************************
**name:	APP_OptLOG_Save_LocallParamOpt
**describe: 存储本地相关操作日志信息
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OptLOG_Save_LocallParamOpt(INT32U code,strHandLocalPositionParam 	LocalPositionParam)
{
	INT8U logbuff[256]={0};
	INT8U len;

	APP_LOG_LocallParam2Logbuff(LocalPositionParam,logbuff,&len);

	return (APP_OptLOG_Save(code,logbuff,len));
}

/***********************************************************
**name:	APP_OptLOG_Save_AntennaParamOpt
**describe: 存储天线参数操作日志信息
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OptLOG_Save_AntennaParamOpt(INT32U code,strAntennaParam 	AntennaParam)
{
	INT8U logbuff[256]={0};
	INT8U len;

	APP_LOG_AntennaParam2Logbuff(AntennaParam,logbuff,&len);

	return (APP_OptLOG_Save(code,logbuff,len));
}

/***********************************************************
**name:	APP_OptLOG_Save_AntennaPoseOpt
**describe: 存储天线姿态相关操作日志信息
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_OptLOG_Save_AntennaPoseOpt(INT32U code,strSatParam 	SatParam,strAntennaParam AntennaParam,FP32 degree_AZ,FP32 degree_EL,FP32 degree_PL)
{
	INT8U logbuff[256]={0};
	INT8U index=0;
	INT8U len;

	APP_LOG_SatParam2Logbuff(SatParam,&logbuff[index],&len);
	index += len;
	
	APP_LOG_AntennaParam2Logbuff(AntennaParam,&logbuff[index],&len);
	index += len;

	APP_LOG_AntennaPoseData2Logbuff(degree_AZ,degree_EL,degree_PL,&logbuff[index],&len);;
	index += len;

	len = index;

	return (APP_OptLOG_Save(code,logbuff,len));
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//以下代码作为测试使用
/***********************************************************
**name:	APP_LOG_RecordSatDrift
**describe: 存储卫星漂移日志信息
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_LOG_RecordSatDrift(INT32U period_ms)
{
	static INT32U time_cnt = 0;
	static INT8U agc_record_cnt = 0;
	static FP32 AGC_Value = 0,dBm_Value = 0;
	INT8U savebuff[64],index=0;
	INT32U code = 0x00B0B0B0;
	INT32U data_INT32U;
	INT32S data_INT32S;

	// 测试卫星飘逸模式
	if(ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode == 0xB0)
	{
		time_cnt++;
		if( time_cnt * period_ms >= 600000)
// 		if( time_cnt * period_ms >= 60000)
		{
			AGC_Value += ACUDevInfo.ACUCurrSta.beacon_rxpower_mv;
			dBm_Value += ACUDevInfo.ACUCurrSta.beacon_rxpower_db;
			agc_record_cnt++;
			if( agc_record_cnt >= 100 )
			{
				AGC_Value = AGC_Value / agc_record_cnt;
				dBm_Value = dBm_Value / agc_record_cnt;
				agc_record_cnt = 0;
				time_cnt = 0;
				savebuff[index++] = ACUDevInfo.ACUParam.CurrSatParam.lon_flag;
				data_INT32U = (INT32U)(ACUDevInfo.ACUParam.CurrSatParam.lon * 1000);
				savebuff[index++] = (data_INT32U & 0xFF000000)>>24 ;
				savebuff[index++] = (data_INT32U & 0x00FF0000)>>16 ;
				savebuff[index++] = (data_INT32U & 0x0000FF00)>>8 ;
				savebuff[index++] = (data_INT32U & 0x000000FF);
				savebuff[index++] = (ACUDevInfo.ACUParam.CurrSatParam.freq & 0xFF000000)>>24 ;
				savebuff[index++] = (ACUDevInfo.ACUParam.CurrSatParam.freq & 0x00FF0000)>>16 ;
				savebuff[index++] = (ACUDevInfo.ACUParam.CurrSatParam.freq & 0x0000FF00)>>8 ;
				savebuff[index++] = (ACUDevInfo.ACUParam.CurrSatParam.freq & 0x000000FF);
				savebuff[index++] = ACUDevInfo.ACUParam.CurrSatParam.polar_type;

				

				data_INT32U = (INT32U)(ACUDevInfo.ACUCurrSta.antenna_position * 1000);
				savebuff[index++] = (data_INT32U & 0xFF000000)>>24 ;
				savebuff[index++] = (data_INT32U & 0x00FF0000)>>16 ;
				savebuff[index++] = (data_INT32U & 0x0000FF00)>>8 ;
				savebuff[index++] = (data_INT32U & 0x000000FF);

				data_INT32U = (INT32U)(ACUDevInfo.ACUCurrSta.antenna_cabrage * 1000);
				savebuff[index++] = (data_INT32U & 0xFF000000)>>24 ;
				savebuff[index++] = (data_INT32U & 0x00FF0000)>>16 ;
				savebuff[index++] = (data_INT32U & 0x0000FF00)>>8 ;
				savebuff[index++] = (data_INT32U & 0x000000FF);

				BSP_Delay_ms(1);

				data_INT32S = (INT32S)(ACUDevInfo.ACUCurrSta.antenna_polar * 1000);
				savebuff[index++] = (data_INT32S & 0xFF000000)>>24 ;
				savebuff[index++] = (data_INT32S & 0x00FF0000)>>16 ;
				savebuff[index++] = (data_INT32S & 0x0000FF00)>>8 ;
				savebuff[index++] = (data_INT32S & 0x000000FF);

				data_INT32U = (INT32U)ACUDevInfo.ACUParam.SeekSatParam.AimAtSat_Limit_mv;
				savebuff[index++] = (data_INT32U & 0xFF000000)>>24 ;
				savebuff[index++] = (data_INT32U & 0x00FF0000)>>16 ;
				savebuff[index++] = (data_INT32U & 0x0000FF00)>>8 ;
				savebuff[index++] = (data_INT32U & 0x000000FF);

				data_INT32U = (INT32U)AGC_Value;
				savebuff[index++] = (data_INT32U & 0xFF000000)>>24 ;
				savebuff[index++] = (data_INT32U & 0x00FF0000)>>16 ;
				savebuff[index++] = (data_INT32U & 0x0000FF00)>>8 ;
				savebuff[index++] = (data_INT32U & 0x000000FF);

				data_INT32U = (INT32U)( dBm_Value * 1000);
				savebuff[index++] = (data_INT32U & 0xFF000000)>>24 ;
				savebuff[index++] = (data_INT32U & 0x00FF0000)>>16 ;
				savebuff[index++] = (data_INT32U & 0x0000FF00)>>8 ;
				savebuff[index++] = (data_INT32U & 0x000000FF);

				AGC_Value  = 0;
				dBm_Value = 0;

				APP_OptLOG_Save(code,savebuff,index);
			}
		}
	}
}



/***********************************************************
**name:	APP_LOG_FollowSatRecord
**describe: 存储跟踪模式情况日志
**input:			code:  0xB1B1B1   启动跟踪；   0xB1B2B2   跟踪过程；0xB1B3B3  结束跟踪
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_LOG_FollowSatRecord(INT32U code)
{

	static INT8U agc_record_cnt = 0;
	static FP32 AGC_Value = 0,dBm_Value = 0;
	INT8U savebuff[64],index=0;
//	INT32U code = 0x00B1B1B1;
	INT32U data_INT32U;


	savebuff[index++] = ACUDevInfo.ACUParam.CurrSatParam.lon_flag;
	
	data_INT32U = (INT32U)(ACUDevInfo.ACUParam.CurrSatParam.lon * 1000);
	savebuff[index++] = (data_INT32U & 0xFF000000)>>24 ;
	savebuff[index++] = (data_INT32U & 0x00FF0000)>>16 ;
	savebuff[index++] = (data_INT32U & 0x0000FF00)>>8 ;
	savebuff[index++] = (data_INT32U & 0x000000FF);
	
	savebuff[index++] = (ACUDevInfo.ACUParam.CurrSatParam.freq & 0xFF000000)>>24 ;
	savebuff[index++] = (ACUDevInfo.ACUParam.CurrSatParam.freq & 0x00FF0000)>>16 ;
	savebuff[index++] = (ACUDevInfo.ACUParam.CurrSatParam.freq & 0x0000FF00)>>8 ;
	savebuff[index++] = (ACUDevInfo.ACUParam.CurrSatParam.freq & 0x000000FF);
	
	savebuff[index++] = ACUDevInfo.ACUParam.CurrSatParam.polar_type;

	data_INT32U = (INT32U)(ACUDevInfo.ACUCurrSta.antenna_position * 1000);
	savebuff[index++] = (data_INT32U & 0xFF000000)>>24 ;
	savebuff[index++] = (data_INT32U & 0x00FF0000)>>16 ;
	savebuff[index++] = (data_INT32U & 0x0000FF00)>>8 ;
	savebuff[index++] = (data_INT32U & 0x000000FF);

	data_INT32U = (INT32U)(ACUDevInfo.ACUCurrSta.antenna_cabrage * 1000);
	savebuff[index++] = (data_INT32U & 0xFF000000)>>24 ;
	savebuff[index++] = (data_INT32U & 0x00FF0000)>>16 ;
	savebuff[index++] = (data_INT32U & 0x0000FF00)>>8 ;
	savebuff[index++] = (data_INT32U & 0x000000FF);

	BSP_Delay_ms(1);

	data_INT32U = (INT32U)(ACUDevInfo.ACUCurrSta.antenna_polar * 1000);
	savebuff[index++] = (data_INT32U & 0xFF000000)>>24 ;
	savebuff[index++] = (data_INT32U & 0x00FF0000)>>16 ;
	savebuff[index++] = (data_INT32U & 0x0000FF00)>>8 ;
	savebuff[index++] = (data_INT32U & 0x000000FF);

	data_INT32U = (INT32U)(ACUDevInfo.ACUParam.SeekSatParam.AimAtSat_Limit_mv*1);
	savebuff[index++] = (data_INT32U & 0xFF000000)>>24 ;
	savebuff[index++] = (data_INT32U & 0x00FF0000)>>16 ;
	savebuff[index++] = (data_INT32U & 0x0000FF00)>>8 ;
	savebuff[index++] = (data_INT32U & 0x000000FF);

	data_INT32U = (INT32U)(ACUDevInfo.ACUCurrSta.beacon_rxpower_mv * 1);
	savebuff[index++] = (data_INT32U & 0xFF000000)>>24 ;
	savebuff[index++] = (data_INT32U & 0x00FF0000)>>16 ;
	savebuff[index++] = (data_INT32U & 0x0000FF00)>>8 ;
	savebuff[index++] = (data_INT32U & 0x000000FF);

	data_INT32U = (INT32U)( ACUDevInfo.ACUCurrSta.beacon_rxpower_db * 1000);
	savebuff[index++] = (data_INT32U & 0xFF000000)>>24 ;
	savebuff[index++] = (data_INT32U & 0x00FF0000)>>16 ;
	savebuff[index++] = (data_INT32U & 0x0000FF00)>>8 ;
	savebuff[index++] = (data_INT32U & 0x000000FF);


	APP_OptLOG_Save(code,savebuff,index);

}


/***********************************************************
**name:	APP_LOG_Reset
**describe:  
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int APP_LOG_Reset(void)
{
	APP_PARAM_EthRelinkResetMCUFlagSet_Save();
	
	APP_PARAM_AimAtSatMode_Save(ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode);
	APP_PARAM_AimAtSatWorkSta_Save(ACUDevInfo.ACUCurrSta.dev_work_sta);
	// 暂时的处理是重启MCU
	BSP_SoftReset();
}


