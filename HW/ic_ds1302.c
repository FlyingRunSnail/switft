

#define IC_DS1302_DEF

#include "ic_ds1302.h"


#define WRITE_SECOND               	0x80 
#define WRITE_MINUTE           		0x82 
#define WRITE_HOUR                   	0x84
#define WRITE_DAY				0x86
#define WRITE_MONTH			0x88
#define WRITE_YEAR				0x8C
#define WRITE_WEEKDAY			0x8A
#define WRITE_PROTECT              	0x8E

#define READ_SECOND                 	0x81 
#define READ_MINUTE                  	0x83 
#define READ_HOUR                   	0x85 
#define READ_DAY				0x87
#define READ_MONTH				0x89
#define READ_YEAR				0x8D
#define READ_WEEKDAY			0x8B
#define READ_PROTECT              	0x8F

OS_SEM DS1302_Sem;

/***********************************************************
**name:	DS1302_SPI_CS_Enable
**describe: 
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void DS1302_CS_Enable(void)
{
	
	GPIO_WriteBit(DS1302_CS_PIN_PORT,DS1302_CS_PIN, Bit_SET);	
}

/***********************************************************
**name:	DS1302_SPI_CS_Disable
**describe: 
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void DS1302_CS_Disable(void)
{
	GPIO_WriteBit(DS1302_CS_PIN_PORT, DS1302_CS_PIN, Bit_RESET);	
	
}

/***********************************************************
**name:	DS1302_SPI_SCK_High
**describe: 
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void DS1302_SPI_SCK_High(void)
{
	GPIO_SetBits(DS1302_SCK_PIN_PORT,DS1302_SCK_PIN);
}

/***********************************************************
**name:	DS1302_SPI_SCK_Low
**describe: 
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void DS1302_SPI_SCK_Low(void)
{
	GPIO_ResetBits(DS1302_SCK_PIN_PORT,DS1302_SCK_PIN);
}

/***********************************************************
**name:	DS1302_SPI_DATA_Output_High
**describe: 
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void DS1302_SPI_DATA_Output_High(void)
{
	GPIO_SetBits(DS1302_DATA_PIN_PORT,DS1302_DATA_PIN);
}

/***********************************************************
**name:	DS1302_SPI_DATA_Output_Low
**describe: 
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void DS1302_SPI_DATA_Output_Low(void)
{
	GPIO_ResetBits(DS1302_DATA_PIN_PORT,DS1302_DATA_PIN);
}

/***********************************************************
**name:	DS1302_SPI_DATA_Input_Read
**describe: 
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
INT8U DS1302_SPI_DATA_Input_Read(void)
{
	return (GPIO_ReadInputDataBit(DS1302_DATA_PIN_PORT,DS1302_DATA_PIN));
}



/***********************************************************
**name:	DS1302_WriteReg8bit
**describe: 
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void  DS1302_WriteReg(INT8U reg_addr,INT8U reg_data)
{
	INT8U i,temp;
	
	if( BSP_OS_Sem_Pend(&DS1302_Sem,100) == FAIL)
		return;
		
	// 第一步，确保CE引脚输出为低
	DS1302_CS_Disable();

	// 第二步，将DATA引脚初始化为输出，清零时钟总线
	BSP_GPIO_Init(DS1302_DATA_PIN_PORT,	DS1302_DATA_PIN,	GPIO_Mode_OUT,	GPIO_OType_PP,GPIO_PuPd_NOPULL);
	DS1302_SPI_SCK_Low();

	// 第三步，使能CE
	DS1302_CS_Enable();

	//第四步，发送地址
	temp = reg_addr;
	for(i=8;i>0;i--)
	{
		DS1302_SPI_SCK_Low();
		if( temp & 0x01)
			DS1302_SPI_DATA_Output_High();
		else
			DS1302_SPI_DATA_Output_Low();

		DS1302_SPI_SCK_High();
		temp >>= 1;
	}

	// 第五步，发送数据
	temp = reg_data;
	for(i=8;i>0;i--)
	{
		DS1302_SPI_SCK_Low();
		if( temp & 0x01)
			DS1302_SPI_DATA_Output_High();
		else
			DS1302_SPI_DATA_Output_Low();

		DS1302_SPI_SCK_High();
		temp >>= 1;
	}

	//第六步，禁能CE
	DS1302_CS_Disable();

	BSP_OS_Sem_Post(&DS1302_Sem);
}

/***********************************************************
**name:	DS1302_SPI_ReadReg
**describe: 
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
INT8U DS1302_ReadReg(INT8U reg_addr)
{
	INT8U reg_data=0,i,temp;

	if( BSP_OS_Sem_Pend(&DS1302_Sem,100) == FAIL)
		return;

	// 第一步，确保CE引脚输出为低
	DS1302_CS_Disable();

	// 第二步，清零时钟总线
	BSP_GPIO_Init(DS1302_DATA_PIN_PORT,	DS1302_DATA_PIN,	GPIO_Mode_OUT,	GPIO_OType_PP,GPIO_PuPd_NOPULL);
	DS1302_SPI_SCK_Low();

	// 第三步，使能CE
	DS1302_CS_Enable();

	// 第四步，发送地址
	temp = reg_addr;
	for(i=8;i>0;i--)
	{
		DS1302_SPI_SCK_Low();
		if( temp & 0x01)
			DS1302_SPI_DATA_Output_High();
		else
			DS1302_SPI_DATA_Output_Low();

		DS1302_SPI_SCK_High();
		temp >>= 1;
	}

	// 第五步，读取数据
	BSP_GPIO_Init(DS1302_DATA_PIN_PORT,	DS1302_DATA_PIN,	GPIO_Mode_IN,	GPIO_OType_PP,GPIO_PuPd_NOPULL);
	
	reg_data = 0;
	for(i=0;i<8;i++)
	{
		DS1302_SPI_SCK_Low();
		DS1302_SPI_SCK_High();
		temp = DS1302_SPI_DATA_Input_Read();
		reg_data |= temp<<i;
	}

	//第六步，禁能CE
	DS1302_CS_Disable();
	BSP_OS_Sem_Post(&DS1302_Sem);

	return reg_data;
}

/***********************************************************
**name:	DS1302_ReadSecond
**describe:   读取秒钟数
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
INT8U  DS1302_ReadSecond(void)
{
	INT8U reg_addr = READ_SECOND;
	INT8U reg_data;
	INT8U sec;

	reg_data = DS1302_ReadReg(reg_addr);

	sec =  ((reg_data & 0x70)>>4)*10 + (reg_data & 0x0F)%10;

	return sec;
}
/***********************************************************
**name:	DS1302_WriteSecond
**describe: 写入秒钟数
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int  DS1302_WriteSecond(INT8U sec)
{
	INT8U reg_addr = WRITE_SECOND;
	INT8U reg_data;
	
	reg_data = ((sec / 10) << 4) + sec % 10;
	DS1302_WriteReg(reg_addr,reg_data);
	if( DS1302_ReadSecond() != sec)
		return FAIL;
		
	return SUCC;
}

/***********************************************************
**name:	DS1302_ReadMinute
**describe:   读取分钟数
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
INT8U  DS1302_ReadMinute(void)
{
	INT8U reg_addr = READ_MINUTE;
	INT8U reg_data;
	INT8U min;

	reg_data  = DS1302_ReadReg(reg_addr) ;
	min =  ((reg_data & 0x70)>>4)*10 + (reg_data & 0x0F)%10;

	return min;
}
/***********************************************************
**name:	DS1302_WriteMinute
**describe: 写入分钟数
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int  DS1302_WriteMinute(INT8U min)
{
	INT8U reg_addr = WRITE_MINUTE;
	INT8U reg_data;

	reg_data = ((min / 10) << 4) + min % 10;
	DS1302_WriteReg(reg_addr,reg_data);
	if( DS1302_ReadMinute() != min)
		return FAIL;
		
	return SUCC;
}

/***********************************************************
**name:	DS1302_ReadHour
**describe:   读取小时数
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
INT8U  DS1302_ReadHour(void)
{
	INT8U reg_addr = READ_HOUR;
	INT8U reg_data;
	INT8U hour;

	reg_data = DS1302_ReadReg(reg_addr) ;
	hour = ((reg_data & 0x10)>>4)*10 + (reg_data & 0x0F)%10;

	return hour;
}
/***********************************************************
**name:	DS1302_WriteHour
**describe: 写入小时数
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int  DS1302_WriteHour(INT8U hour)
{
	INT8U reg_addr = WRITE_HOUR;
	INT8U reg_data;

	//reg_data = 0x00 + hour;		// 24小时制式
	reg_data = ((hour / 10) << 4) + hour % 10;
	DS1302_WriteReg(reg_addr,reg_data);
	if( DS1302_ReadHour() != hour)
		return FAIL;
		
	return SUCC;
}

/***********************************************************
**name:	DS1302_ReadDay
**describe:   读取天数
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
INT8U  DS1302_ReadDay(void)
{
	INT8U reg_addr = READ_DAY;
	INT8U reg_data;
	INT8U day;

	reg_data = DS1302_ReadReg(reg_addr);
	day = ((reg_data & 0x30)>>4)*10 + (reg_data & 0x0F) % 10;

	return day;
}
/***********************************************************
**name:	DS1302_WriteDay
**describe: 写入天数
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int  DS1302_WriteDay(INT8U day)
{
	INT8U reg_addr = WRITE_DAY;
	INT8U reg_data;

	reg_data = ((day / 10)<<4) + day % 10;
	DS1302_WriteReg(reg_addr,reg_data);
	if( DS1302_ReadDay() != day)
		return FAIL;
		
	return SUCC;
}

/***********************************************************
**name:	DS1302_ReadMonth
**describe:   读取月份数
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
INT8U  DS1302_ReadMonth(void)
{
	INT8U reg_addr = READ_MONTH;
	INT8U reg_data;
	INT8U month;
	

	reg_data = DS1302_ReadReg(reg_addr);
	month = ((reg_data & 0x10)>>4)*10 + (reg_data & 0x0F) % 10;

	return month;
}
/***********************************************************
**name:	DS1302_WriteMonth
**describe: 写入月份数
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int  DS1302_WriteMonth(INT8U month)
{
	INT8U reg_addr = WRITE_MONTH;
	INT8U reg_data;

	reg_data = ((month / 10)<<4) + month % 10;
	DS1302_WriteReg(reg_addr,reg_data);
	if( DS1302_ReadMonth() != month)
		return FAIL;
		
	return SUCC;
}

/***********************************************************
**name:	DS1302_ReadYear
**describe:   读取年份数
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
INT8U  DS1302_ReadYear(void)
{
	INT8U reg_addr = READ_YEAR;
	INT8U reg_data;
	INT8U year;

	reg_data = DS1302_ReadReg(reg_addr);
	year = ((reg_data & 0xF0)>>4)*10 + (reg_data & 0x0F) % 10;

	return year;
}
/***********************************************************
**name:	DS1302_WriteYear
**describe: 写入年分数
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int  DS1302_WriteYear(INT8U year)
{
	INT8U reg_addr = WRITE_YEAR;
	INT8U reg_data;

	reg_data = ((year / 10)<<4) + year % 10;
	DS1302_WriteReg(reg_addr,reg_data);
	if( DS1302_ReadYear() != year)
		return FAIL;
		
	return SUCC;
}

/***********************************************************
**name:	DS1302_ReadWeekday
**describe:   读取星期数
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
INT8U  DS1302_ReadWeekday(void)
{
	INT8U reg_addr = READ_WEEKDAY;
	INT8U weekday;

	weekday = DS1302_ReadReg(reg_addr);

	return weekday;
}
/***********************************************************
**name:	DS1302_WriteWeekday
**describe: 写入星期数
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int  DS1302_WriteWeekday(INT8U weekday)
{
	INT8U reg_addr = WRITE_WEEKDAY;
	INT8U reg_data;

	DS1302_WriteReg(reg_addr,weekday);
	if( DS1302_ReadWeekday() != weekday)
		return FAIL;
		
	return SUCC;
}

/***********************************************************
**name:	DS1302_Init
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void DS1302_Init(void)
{
	BSP_GPIO_Init(DS1302_CS_PIN_PORT,		DS1302_CS_PIN,		GPIO_Mode_OUT,	GPIO_OType_PP,GPIO_PuPd_NOPULL);
	BSP_GPIO_Init(DS1302_DATA_PIN_PORT,	DS1302_DATA_PIN,	GPIO_Mode_IN,	GPIO_OType_PP,GPIO_PuPd_NOPULL);
	BSP_GPIO_Init(DS1302_SCK_PIN_PORT,		DS1302_SCK_PIN,		GPIO_Mode_OUT,	GPIO_OType_PP,GPIO_PuPd_NOPULL);

	BSP_OS_Sem_Creat(&DS1302_Sem,"SPIFLASH Sem",1);
	
	DS1302_CS_Disable();
}


