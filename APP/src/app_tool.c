


#define APP_TOOL_DEF

#include <app_tool.h>




/***********************************************************
**name:	TOOL_CheckSum8BitGet
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
INT8U TOOL_CheckSum8BitGet(INT8U *buff, INT32U len)
{
	INT8U checksum=0;

	while(len--)
	{
		checksum += *buff++;
	}

	return ((INT8U)checksum);
}


/***********************************************************
**name:	SWIFT_UART6_Init
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
INT32U TOOL_SysTickGet(void)
{
	INT32U tick;
	OS_ERR err;
	
	tick = OSTimeGet(&err);

	return tick;
}

/***********************************************************
**name:	TOOL_INT32Data2Bytebuff
**describe:
**input:			save_mode :   1表示大端在前  0表示小段在前
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void TOOL_INT32Data2Bytebuff(INT32U data, INT8U *buff, INT8U save_mode)
{
	INT8U i=0;

	if( save_mode )			// 大端在前
	{
		buff[i++] = (data & 0xFF000000)>>24;
		buff[i++] = (data & 0x00FF0000)>>16;
		buff[i++] = (data & 0x0000FF00)>>8;
		buff[i++] = (data & 0x000000FF);
	}
	else
	{
		buff[i++] = (data & 0x000000FF);
		buff[i++] = (data & 0x0000FF00)>>8;
		buff[i++] = (data & 0x00FF0000)>>16;
		buff[i++] = (data & 0xFF000000)>>24;
	}
}

/***********************************************************
**name:	TOOL_INT16Data2Bytebuff
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void TOOL_INT16Data2Bytebuff(INT16U data, INT8U *buff, INT8U save_mode)
{
	INT8U i=0;

	if( save_mode )			// 大端在前
	{
		buff[i++] = (data & 0x0000FF00)>>8;
		buff[i++] = (data & 0x000000FF);
	}
	else
	{
		buff[i++] = (data & 0x000000FF);
		buff[i++] = (data & 0x0000FF00)>>8;
	}
}

/***********************************************************
**name:	TOOL_Bytebuff2INT32Data
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
INT32U TOOL_Bytebuff2INT32Data(INT8U *buff,INT8U save_mode)
{
	INT8U i;
	INT32U data=0;

	if( save_mode )
	{
		for(i=0;i<4;i++)
			data = (data << 8) + buff[i];
	}
	else
	{
		for(i=4;i!=0;i--)
			data = (data << 8) + buff[i];
	}

	return data;
}

/***********************************************************
**name:	TOOL_Bytebuff2INT16Data
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
INT16U TOOL_Bytebuff2INT16Data(INT8U *buff,INT8U save_mode)
{
	INT8U i;
	INT32U data=0;

	if( save_mode )
	{
		for(i=0;i<4;i++)
			data = (data << 8) + buff[i];
	}
	else
	{
		for(i=2;i!=0;i--)
			data = (data << 8) + buff[i];
	}

	return data;
}

/***********************************************************
**name:	TOOL_GetCPI_ID
**describe:  获取MCU的96bit的ID
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void TOOL_GetCPI_ID(INT32U *IDbuff)  
{  
    //获取CPU唯一ID  
    IDbuff[0]=*(INT32U*)(0x1fff7a10);  
    IDbuff[1]=*(INT32U*)(0x1fff7a14);  
    IDbuff[2]=*(INT32U*)(0x1fff7a18);  
}

/***********************************************************
**name:	TOOL_Date2Bytebuff
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
// void TOOL_Date2Bytebuff(strSysDateTime SysDateTime,INT8U *buff,INT8U buff_len)
// {
// 	INT8U i=0;
// 	if( buff_len > 3)
// 	{
// 		buff[i++] = (SysDateTime.year & 0xFF00)>>8;
// 	}

// 	buff[i++] = SysDateTime.year & 0x00FF;
// 	buff[i++] = SysDateTime.month;
// 	buff[i++] = SysDateTime.day;

// }

/***********************************************************
**name:	TOOL_Time2Bytebuff
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
// void TOOL_Time2Bytebuff(strSysDateTime SysDateTime,INT8U *buff)
// {
// 	INT8U i=0;
// 	
// 	buff[i++] = SysDateTime.hour;
// 	buff[i++] = SysDateTime.min;
// 	buff[i++] = SysDateTime.senc;
// }

/***********************************************************
**name:	TOOL_Bytebuff2INT16Data
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
 void TOOL_DateTime2Bytebuff(strSysDateTime SysDateTime,INT8U *buff,INT8U buff_len)
 {
 	INT8U i=0;
 	if( buff_len > 6)
 	{
 		buff[i++] = (SysDateTime.year & 0xFF00)>>8;
 		buff[i++] = (SysDateTime.year & 0x00FF);
 	}
 	else
 		buff[i++] = SysDateTime.year % 100;
 	buff[i++] = SysDateTime.month;
 	buff[i++] = SysDateTime.day;
 	buff[i++] = SysDateTime.hour;
 	buff[i++] = SysDateTime.min;
 	buff[i++] = SysDateTime.senc;
 }


