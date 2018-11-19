


#define MICRON_FLASH_DEF

#include <ic_n25q128.h>


////////////////////////////////////////////////////////
//SPIFLASH 命令字

//reset cmd
#define RESET_ENANBLE		0x66
#define RESET_MEMORY		0x99

//id opt
#define	READ_ID			0x9F
#define MULTIPLE_IO_READ_ID	0xAF
#define READ_SERIAL_FLASH_DISCOVER_PARA		0x5A

//read opt
#define B3_READ_DATA		0x03
#define B3_FAST_READ_DATA	0x0B

#define B4_READ_DATA		0x13
#define B4_FAST_READ_DATA	0x0C

//write enable switch opt
#define WRITE_ENABLE		0x06
#define WRITE_DISABLE		0x04

//register opt
#define READ_STATUS_REG	0x05
#define WRITE_STATUS_REG	0x01

#define READ_LOCK_REG		0xE8
#define WRITE_LOCK_REG	0xE5

#define READ_FLAG_STA_REG		0x70
#define CLEAR_FLAG_STA_REG	0x50

//program opt
#define B3_PAGE_PROGRAM		0x02
#define B4_PAGE_PROGRAM		0x12

//erase opt
#define B3_SUBSECTOR_ERASE	0x20
#define B4_SUBSECTOR_ERASE	0x21

#define B3_SECTOR_ERASE		0xD8
#define B4_SECTOR_ERASE		0xDC

#define BULK_ERASE				0xC7


/////////////////////////////////////////////////////////////




//无效数据
#define DUMMY_BYTE		0xFF





/***********************************************************
**name:	MICRON_FLASH_CS_Enable
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void MICRON_FLASH_CS_Enable(void)
{
	HW_SPIFLASH_CsEnable();
}

/***********************************************************
**name:	MICRON_FLASH_CS_Disable
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void MICRON_FLASH_CS_Disable(void)
{
	HW_SPIFLASH_CsDisable();
}

/***********************************************************
**name:	S25FL128S_WriteByte
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
INT8U MICRON_FLASH_WriteByte(INT8U data)
{
	HW_SPIFLASH_WriteByte(data);
}

/***********************************************************
**name:	MICRON_FLASH_ReadByte
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
INT8U MICRON_FLASH_ReadByte(void)
{
	return HW_SPIFLASH_ReadByte();
}

/***********************************************************
**name:	MICRON_FLASH_WriteEnable
**describe: 写使能，所有涉及对flash的写、擦除的操作均需要先调用此函数
**input:			
**output:	none
**return:	staus   byte
**autor:  andiman
**date:
************************************************************/
void MICRON_FLASH_WriteEnable(void)
{
	

	MICRON_FLASH_CS_Enable();

	MICRON_FLASH_WriteByte(WRITE_ENABLE);
	
	MICRON_FLASH_CS_Disable();
}

/***********************************************************
**name:	MICRON_FLASH_WriteEnable
**describe: 写禁能，建议每次进行写、擦除之后调用此函数
**input:			
**output:	none
**return:	staus   byte
**autor:  andiman
**date:
************************************************************/
void MICRON_FLASH_WriteDisable(void)
{
	MICRON_FLASH_CS_Enable();

	MICRON_FLASH_WriteByte(WRITE_DISABLE);
	
	MICRON_FLASH_CS_Disable();
}




/***********************************************************
**name:	MICRON_FLASH_Read_StaReg
**describe:
**input:			
**output:	none
**return:	staus   byte
**autor:  andiman
**date:
************************************************************/
INT8U MICRON_FLASH_Read_StaReg(void)
{
	INT8U status;
	
	MICRON_FLASH_CS_Enable();

	MICRON_FLASH_WriteByte(READ_STATUS_REG);

	status = MICRON_FLASH_ReadByte();

	MICRON_FLASH_CS_Disable();

	return status;
}

/***********************************************************
**name:	MICRON_FLASH_Write_StaReg
**describe:
**input:			
**output:	none
**return:	staus   byte
**autor:  andiman
**date:
************************************************************/
void MICRON_FLASH_Write_StaReg(INT8U data)
{
	INT8U status;

	MICRON_FLASH_WriteEnable();					//写使能
	
	MICRON_FLASH_CS_Enable();

	MICRON_FLASH_WriteByte(WRITE_STATUS_REG);

	MICRON_FLASH_WriteByte(data);;

	MICRON_FLASH_CS_Disable();
}

/***********************************************************
**name:	MICRON_FLASH_Read_FlagReg
**describe: 读取状态标志寄存器
**input:			
**output:	none
**return:	staus   byte
**autor:  andiman
**date:
************************************************************/
INT8U MICRON_FLASH_Read_FlagReg(void)
{
	INT8U flag;
	
	MICRON_FLASH_CS_Enable();

	MICRON_FLASH_WriteByte(READ_FLAG_STA_REG);

	flag = MICRON_FLASH_ReadByte();

	MICRON_FLASH_CS_Disable();

	return flag;
}


/***********************************************************
**name:	MICRON_FLASH_WaitPageProgram
**describe:
**input:			
**output:	none
**return:	staus   byte
**autor:  andiman
**date:
************************************************************/
void MICRON_FLASH_WaitPageProgram(void)
{
	while(MICRON_FLASH_Read_StaReg() & 0x01);
}

/***********************************************************
**name:	MICRON_FLASH_WaitSectorErase
**describe:
**input:			
**output:	none
**return:	staus   byte
**autor:  andiman
**date:
************************************************************/
void MICRON_FLASH_WaitSectorErase(void)
{
	while(MICRON_FLASH_Read_StaReg() & 0x01);
// 	{
// 		BSP_Dealy_ms(5);
// 	}
}

/***********************************************************
**name:	MICRON_FLASH_WaitBulkErase
**describe:
**input:			
**output:	none
**return:	staus   byte
**autor:  andiman
**date:
************************************************************/
void MICRON_FLASH_WaitBulkErase(void)
{
	while(MICRON_FLASH_Read_StaReg() & 0x01);
// 	{
// 		BSP_Dealy_ms(5);
// 	}
}




/***********************************************************
**name:	MICRON_FLASH_Reset
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void MICRON_FLASH_Reset(void)
{
	MICRON_FLASH_WriteEnable();					//写使能
	MICRON_FLASH_CS_Enable();
	MICRON_FLASH_WriteByte(RESET_ENANBLE);					//发送重启命令
	MICRON_FLASH_CS_Disable();
}

/***********************************************************
**name:	MICRON_FLASH_ReadID
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void MICRON_FLASH_ReadID(INT8U *idbuff)
{
	INT8U i=0;
	
	MICRON_FLASH_CS_Enable();
	MICRON_FLASH_WriteByte(READ_ID);					//发送读取ID命令

	idbuff[i++] = MICRON_FLASH_ReadByte();
	idbuff[i++] = MICRON_FLASH_ReadByte();
	idbuff[i++] = MICRON_FLASH_ReadByte();

	MICRON_FLASH_CS_Disable();
}


/***********************************************************
**name:	MICRON_FLASH_B3WriteDataByte
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int MICRON_FLASH_B3WriteDataByte(INT32U addr, INT8U byte)
{
	int status = FAIL;
	
	
	MICRON_FLASH_WriteEnable();					//写使能
	
	MICRON_FLASH_CS_Enable();
	MICRON_FLASH_WriteByte(B3_PAGE_PROGRAM);			//发送写命令
	MICRON_FLASH_WriteByte((addr&0x00ff0000)>>16 );	
	MICRON_FLASH_WriteByte((addr&0x0000ff00)>>8 );
	MICRON_FLASH_WriteByte(addr&0x000000ff);
	MICRON_FLASH_WriteByte(byte);					//发送字节数据
	MICRON_FLASH_CS_Disable();

	MICRON_FLASH_WaitPageProgram();

	//对写入的FLASH 的数据进行验证
	if(MICRON_FLASH_B3ReadDataByte(addr) != byte)
		return FAIL;
	
	return SUCC;

}

/***********************************************************
**name:	MICRON_FLASH_B3ReadDataByte
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
INT8U MICRON_FLASH_B3ReadDataByte(INT32U addr)
{
	
	INT8U	data;

	MICRON_FLASH_CS_Enable();
	MICRON_FLASH_WriteByte(B3_READ_DATA);					//发送读命令
	MICRON_FLASH_WriteByte((addr&0x00ff0000)>>16 );	
	MICRON_FLASH_WriteByte((addr&0xff00)>>8 );
	MICRON_FLASH_WriteByte(addr&0xff);

	data = MICRON_FLASH_ReadByte();	

	MICRON_FLASH_CS_Disable();
	
	return data;

}


/***********************************************************
**name:	MICRON_FLASH_B4WriteDataByte
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int MICRON_FLASH_B4WriteDataByte(INT32U addr, INT8U byte)
{
	int status = FAIL;
	
	
	MICRON_FLASH_WriteEnable();					//写使能
	
	MICRON_FLASH_CS_Enable();
	MICRON_FLASH_WriteByte(B4_PAGE_PROGRAM);			//发送写命令
	MICRON_FLASH_WriteByte((addr&0xff000000)>>24 );	
	MICRON_FLASH_WriteByte((addr&0x00ff0000)>>16 );	
	MICRON_FLASH_WriteByte((addr&0x0000ff00)>>8 );
	MICRON_FLASH_WriteByte(addr&0x000000ff);
	MICRON_FLASH_WriteByte(byte);					//发送字节数据
	MICRON_FLASH_CS_Disable();

	MICRON_FLASH_WaitPageProgram();

	//对写入的FLASH 的数据进行验证
	if(MICRON_FLASH_B3ReadDataByte(addr) != byte)
		return FAIL;
	
	return SUCC;

}

/***********************************************************
**name:	MICRON_FLASH_B4ReadDataByte
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
INT8U MICRON_FLASH_B4ReadDataByte(INT32U addr)
{
	
	INT8U	data;

	MICRON_FLASH_CS_Enable();
	MICRON_FLASH_WriteByte(B4_READ_DATA);					//发送读命令
	MICRON_FLASH_WriteByte((addr&0xff000000)>>24 );	
	MICRON_FLASH_WriteByte((addr&0x00ff0000)>>16 );	
	MICRON_FLASH_WriteByte((addr&0xff00)>>8 );
	MICRON_FLASH_WriteByte(addr&0xff);

	data = MICRON_FLASH_ReadByte();	

	MICRON_FLASH_CS_Disable();
	
	return data;

}

/***********************************************************
**name:	MICRON_FLASH_B3PageWrite
**describe: 
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void MICRON_FLASH_B3PageWrite(INT32U addr, INT8U *buff, INT32U len)
{
	if(len > 256)
		len = 256;

	MICRON_FLASH_WriteEnable();					//写使能

	MICRON_FLASH_CS_Enable();
	MICRON_FLASH_WriteByte(B3_PAGE_PROGRAM);			//发送写命令
	MICRON_FLASH_WriteByte((addr&0x00ff0000)>>16 );	
	MICRON_FLASH_WriteByte((addr&0xff00)>>8 );
	MICRON_FLASH_WriteByte(addr&0xff);

	while(len--)
	{
		MICRON_FLASH_WriteByte(*buff);					//发送字节数据
		buff++;
	}
	
	MICRON_FLASH_CS_Disable();

	MICRON_FLASH_WaitPageProgram();

}

/***********************************************************
**name:	MICRON_FLASH_B4PageWrite
**describe: 
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void MICRON_FLASH_B4PageWrite(INT32U addr, INT8U *buff, INT32U len)
{
	
	INT8U sta=0xFF;
	if(len > 256)
		len = 256;

	MICRON_FLASH_WriteEnable();					//写使能

	MICRON_FLASH_CS_Enable();
	//MICRON_FLASH_WriteByte(B4_PAGE_PROGRAM);			//发送写命令
	MICRON_FLASH_WriteByte(B3_PAGE_PROGRAM);			//发送写命令
	MICRON_FLASH_WriteByte((addr&0xff000000)>>24 );	//按字节发送32位地址
	MICRON_FLASH_WriteByte((addr&0x00ff0000)>>16 );	
	MICRON_FLASH_WriteByte((addr&0xff00)>>8 );
	MICRON_FLASH_WriteByte(addr&0xff);

	while(len--)
	{
		MICRON_FLASH_WriteByte(*buff);					//发送字节数据
		buff++;
	}
	
	MICRON_FLASH_CS_Disable();

	MICRON_FLASH_WaitPageProgram();
	
}

/***********************************************************
**name:	MICRON_FLASH_B3WriteDataBuff
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int MICRON_FLASH_B3WriteDataBuff(INT32U addr, INT8U *buff, INT32U len)
{
	unsigned int flash_addr;
	unsigned int pagelen=0;
	INT32U checklen,checkpagelen,i;
	INT8U  checkbuff[256];
	INT8U  *checksrcbuff;

	checksrcbuff = buff;
	checklen = len;
	flash_addr = addr;
	if( addr % SPI_FLASH_PageSize == 0)		//起始地址刚好对齐片地址
	{
		while(len > SPI_FLASH_PageSize)
		{
			pagelen = SPI_FLASH_PageSize;
			MICRON_FLASH_B3PageWrite(flash_addr, buff, pagelen);

			flash_addr += SPI_FLASH_PageSize;
			buff += SPI_FLASH_PageSize;
			len -= SPI_FLASH_PageSize;
		}
		if( len > 0 )
			MICRON_FLASH_B3PageWrite(flash_addr, buff, len);
	}
	else									
	{
		//先将前面起始地址未对齐的部分写入
		if(len > SPI_FLASH_PageSize - (addr % SPI_FLASH_PageSize))
		{
			pagelen = SPI_FLASH_PageSize - (addr % SPI_FLASH_PageSize);
			MICRON_FLASH_B3PageWrite(flash_addr, buff, pagelen);
			flash_addr += pagelen;
			buff += pagelen;
			len -= pagelen;
		}
		while(len > SPI_FLASH_PageSize)
		{
			pagelen = SPI_FLASH_PageSize;
			MICRON_FLASH_B3PageWrite(flash_addr, buff, pagelen);
			flash_addr += SPI_FLASH_PageSize;
			buff += SPI_FLASH_PageSize;
			len -= SPI_FLASH_PageSize;
		}
		if( len > 0 )
			MICRON_FLASH_B3PageWrite(flash_addr, buff, len);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// 检查写入的数据是否正确

	flash_addr = addr;
	while(checklen)
	{
		if( checklen > SPI_FLASH_PageSize )
			checkpagelen = SPI_FLASH_PageSize;
		else
			checkpagelen = checklen;
		MICRON_FLASH_B3ReadDataBuff(flash_addr, checkbuff,checkpagelen);
		
		for(i=0;i<checkpagelen;i++)
		{
			if(checkbuff[i] != checksrcbuff[i])
				return FAIL;
		}
		checksrcbuff += checkpagelen;
		flash_addr += checkpagelen;
		checklen -= checkpagelen;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	MICRON_FLASH_WriteDisable();
	return SUCC;
}


/***********************************************************
**name:	MICRON_FLASH_B3ReadDataBuff
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void MICRON_FLASH_B3ReadDataBuff(INT32U addr, INT8U *buff, INT32U len)
{
	MICRON_FLASH_CS_Enable();
	if(len > SPI_FLASH_PageSize)
		MICRON_FLASH_WriteByte(B3_FAST_READ_DATA);					//发送快读命令
	else
		MICRON_FLASH_WriteByte(B3_READ_DATA);			//发送读命令
		
	MICRON_FLASH_WriteByte((addr&0x00ff0000)>>16 );	
	MICRON_FLASH_WriteByte((addr&0xff00)>>8 );
	MICRON_FLASH_WriteByte(addr&0xff);

	if(len > SPI_FLASH_PageSize)
		*buff = MICRON_FLASH_ReadByte();
	while(len--)
	{
		*buff = MICRON_FLASH_ReadByte();
		buff++;
	}

	MICRON_FLASH_CS_Disable();
}


/***********************************************************
**name:	MICRON_FLASH_B4WriteDataBuff
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int MICRON_FLASH_B4WriteDataBuff(INT32U addr, INT8U *buff, INT32U len)
{
	unsigned int flash_addr;
	unsigned int pagelen=0;
	INT32U checklen,checkpagelen,i;
	INT8U  checkbuff[256];
	INT8U  *checksrcbuff;

	checksrcbuff = buff;
	checklen = len;
	flash_addr = addr;
	if( addr % SPI_FLASH_PageSize == 0)		//起始地址刚好对齐片地址
	{
		while(len > SPI_FLASH_PageSize)
		{
			pagelen = SPI_FLASH_PageSize;
			MICRON_FLASH_B4PageWrite(flash_addr, buff, pagelen);

			flash_addr += SPI_FLASH_PageSize;
			buff += SPI_FLASH_PageSize;
			len -= SPI_FLASH_PageSize;
		}
		if( len > 0 )
			MICRON_FLASH_B4PageWrite(flash_addr, buff, len);
	}
	else									
	{
		//先将前面起始地址未对齐的部分写入
		if(len > SPI_FLASH_PageSize - (addr % SPI_FLASH_PageSize))
		{
			pagelen = SPI_FLASH_PageSize - (addr % SPI_FLASH_PageSize);
			MICRON_FLASH_B4PageWrite(flash_addr, buff, pagelen);
			flash_addr += pagelen;
			buff += pagelen;
			len -= pagelen;
		}
		while(len > SPI_FLASH_PageSize)
		{
			pagelen = SPI_FLASH_PageSize;
			MICRON_FLASH_B4PageWrite(flash_addr, buff, pagelen);
			flash_addr += SPI_FLASH_PageSize;
			buff += SPI_FLASH_PageSize;
			len -= SPI_FLASH_PageSize;
		}
		if( len > 0 )
			MICRON_FLASH_B4PageWrite(flash_addr, buff, len);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	// 检查写入的数据是否正确

	flash_addr = addr;
	while(checklen)
	{
		if( checklen > SPI_FLASH_PageSize )
			checkpagelen = SPI_FLASH_PageSize;
		else
			checkpagelen = checklen;
		MICRON_FLASH_B4ReadDataBuff(flash_addr, checkbuff,checkpagelen);
		
		for(i=0;i<checkpagelen;i++)
		{
			if(checkbuff[i] != checksrcbuff[i])
				return FAIL;
		}
		checksrcbuff += checkpagelen;
		flash_addr += checkpagelen;
		checklen -= checkpagelen;
	}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	MICRON_FLASH_WriteDisable();
	return SUCC;
}




/***********************************************************
**name:	MICRON_FLASH_B4ReadDataBuff
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void MICRON_FLASH_B4ReadDataBuff(INT32U addr, INT8U *buff, INT32U len)
{
	MICRON_FLASH_CS_Enable();
	if(len > SPI_FLASH_PageSize)
		//MICRON_FLASH_WriteByte(B4_FAST_READ_DATA);					//发送快读命令
		MICRON_FLASH_WriteByte(B3_FAST_READ_DATA);					//发送快读命令
	else
		//MICRON_FLASH_WriteByte(B4_READ_DATA);			//发送读命令
		MICRON_FLASH_WriteByte(B3_READ_DATA);			//发送读命令
	
	MICRON_FLASH_WriteByte((addr&0xff000000)>>24 );	
	MICRON_FLASH_WriteByte((addr&0x00ff0000)>>16 );	
	MICRON_FLASH_WriteByte((addr&0xff00)>>8 );
	MICRON_FLASH_WriteByte(addr&0xff);

	if(len > SPI_FLASH_PageSize)
		*buff = MICRON_FLASH_ReadByte();
	while(len--)
	{
		*buff = MICRON_FLASH_ReadByte();
		buff++;
	}

	MICRON_FLASH_CS_Disable();
}


/***********************************************************
**name:	MICRON_FLASH_B3SubSectorErase
**describe: 删除当前所在地址的子扇区，4KB大小
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void MICRON_FLASH_B3SubSectorErase(INT32U addr)
{	
	MICRON_FLASH_WriteEnable();					//写使能

	MICRON_FLASH_CS_Enable();

	MICRON_FLASH_WriteByte(B3_SUBSECTOR_ERASE);			//发送擦除4KB扇区命令

	MICRON_FLASH_WriteByte((addr&0x00ff0000)>>16 );	
	MICRON_FLASH_WriteByte((addr&0xff00)>>8 );
	MICRON_FLASH_WriteByte(addr&0xff);

	MICRON_FLASH_CS_Disable();
	
	MICRON_FLASH_WaitSectorErase();

	MICRON_FLASH_WriteDisable();

}

/***********************************************************
**name:	MICRON_FLASH_B4SubSectorErase
**describe: 删除当前所在地址的子扇区，4KB大小
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void MICRON_FLASH_B4SubSectorErase(INT32U addr)
{	
	MICRON_FLASH_WriteEnable();					//写使能

	MICRON_FLASH_CS_Enable();


	//MICRON_FLASH_WriteByte(B4_SUBSECTOR_ERASE);			//发送擦除4KB扇区命令
	MICRON_FLASH_WriteByte(B3_SUBSECTOR_ERASE);			//发送擦除4KB扇区命令

	MICRON_FLASH_WriteByte((addr&0xff000000)>>24 );	//按字节发送32位地址
	MICRON_FLASH_WriteByte((addr&0x00ff0000)>>16 );	
	MICRON_FLASH_WriteByte((addr&0xff00)>>8 );
	MICRON_FLASH_WriteByte(addr&0xff);

	MICRON_FLASH_CS_Disable();
	
	MICRON_FLASH_WaitSectorErase();

	MICRON_FLASH_WriteDisable();

}

/***********************************************************
**name:	MICRON_FLASH_B3SectorErase
**describe: 删除当前所在地址扇区，64KB大小
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void MICRON_FLASH_B3SectorErase(INT32U addr)
{	
	MICRON_FLASH_WriteEnable();					//写使能

	MICRON_FLASH_CS_Enable();

	MICRON_FLASH_WriteByte(B3_SECTOR_ERASE);			//发送擦除64KB扇区命令

	MICRON_FLASH_WriteByte((addr&0x00ff0000)>>16 );	
	MICRON_FLASH_WriteByte((addr&0xff00)>>8 );
	MICRON_FLASH_WriteByte(addr&0xff);

	MICRON_FLASH_CS_Disable();
	
	MICRON_FLASH_WaitSectorErase();

	MICRON_FLASH_WriteDisable();

}

/***********************************************************
**name:	MICRON_FLASH_B4SectorErase
**describe: 删除当前所在地址扇区，64KB大小
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void MICRON_FLASH_B4SectorErase(INT32U addr)
{	
	MICRON_FLASH_WriteEnable();					//写使能

	MICRON_FLASH_CS_Enable();


	//MICRON_FLASH_WriteByte(B4_SECTOR_ERASE);			//发送擦除64KB扇区命令
	MICRON_FLASH_WriteByte(B3_SECTOR_ERASE);			//发送擦除64KB扇区命令

	MICRON_FLASH_WriteByte((addr&0xff000000)>>24 );	//按字节发送32位地址
	MICRON_FLASH_WriteByte((addr&0x00ff0000)>>16 );	
	MICRON_FLASH_WriteByte((addr&0xff00)>>8 );
	MICRON_FLASH_WriteByte(addr&0xff);

	MICRON_FLASH_CS_Disable();
	
	MICRON_FLASH_WaitSectorErase();

	MICRON_FLASH_WriteDisable();

}

/***********************************************************
**name:	MICRON_FLASH_BulkErase
**describe: 擦除整个flash
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void MICRON_FLASH_BulkErase(void)
{		
	MICRON_FLASH_WriteEnable();					//写使能
	MICRON_FLASH_CS_Enable();
	MICRON_FLASH_WriteByte(BULK_ERASE);
	MICRON_FLASH_CS_Disable();	
	MICRON_FLASH_WaitBulkErase();

	MICRON_FLASH_WriteDisable();	
}

/***********************************************************
**name:	MICRON_FLASH_Byte3AddrMode_Set
**describe: 
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int MICRON_FLASH_Byte3AddrMode_Set(void)
{
	INT8U i=3;
	INT16U data_cfg = 0;

	#if 1	

	if( (MICRON_FLASH_Read_FlagReg() & 0x01) == 0x01)
	{

		MICRON_FLASH_WriteEnable();					//写使能
	
		MICRON_FLASH_CS_Enable();
		MICRON_FLASH_WriteByte(EXIT_4BYTE_ADDRESS_MODE);
		MICRON_FLASH_CS_Disable();	

		MICRON_FLASH_WriteDisable();

		while(i--)
		{
			if( (MICRON_FLASH_Read_FlagReg() & 0x01) == 0x00)
				return SUCC;
			MICRON_FLASH_WriteEnable();					//写使能
			MICRON_FLASH_CS_Enable();
			MICRON_FLASH_WriteByte(EXIT_4BYTE_ADDRESS_MODE);
			MICRON_FLASH_CS_Disable();	
			MICRON_FLASH_WriteDisable();

			BSP_Delay_ms(1);
		}

		return FAIL;
	}

	#else 
	*/
	MICRON_FLASH_CS_Enable();
	MICRON_FLASH_WriteByte(0xB5);
	data_cfg = MICRON_FLASH_ReadByte();
	data_cfg = (data_cfg << 8) + MICRON_FLASH_ReadByte();
	MICRON_FLASH_CS_Disable();	

	MICRON_FLASH_WriteEnable();					//写使能
	
	MICRON_FLASH_CS_Enable();
	MICRON_FLASH_WriteByte(0xB1);
	MICRON_FLASH_WriteByte((data_cfg & 0xff00)>>8);
	MICRON_FLASH_WriteByte((data_cfg & 0x00ff) | 0x03);
	MICRON_FLASH_CS_Disable();	

	
	MICRON_FLASH_CS_Enable();
	MICRON_FLASH_WriteByte(0xC8);
	data_cfg = MICRON_FLASH_ReadByte();
	MICRON_FLASH_CS_Disable();	

	MICRON_FLASH_WriteEnable();					//写使能
	
	MICRON_FLASH_CS_Enable();
	MICRON_FLASH_WriteByte(0xC5);
	MICRON_FLASH_WriteByte((data_cfg & 0xfe));
	MICRON_FLASH_CS_Disable();	

	MICRON_FLASH_CS_Enable();
	MICRON_FLASH_WriteByte(0xC8);
	data_cfg = MICRON_FLASH_ReadByte();
	MICRON_FLASH_CS_Disable();	
	#endif

	return SUCC;
	
}

/***********************************************************
**name:	MICRON_FLASH_Byte3AddrMode_Set
**describe: 
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int MICRON_FLASH_Byte4AddrMode_Set(void)
{
	INT8U i=3;
	INT16U data_cfg = 0;

	#if 1

	
	if( (MICRON_FLASH_Read_FlagReg() & 0x01) == 0x00)
	{

		MICRON_FLASH_WriteEnable();					//写使能
	
		MICRON_FLASH_CS_Enable();
		MICRON_FLASH_WriteByte(ENTER_4BYTE_ADDRESS_MODE);
		MICRON_FLASH_CS_Disable();	

		MICRON_FLASH_WriteDisable();

		while(i--)
		{
			if( (MICRON_FLASH_Read_FlagReg() & 0x01) == 0x01)
				return SUCC;
			MICRON_FLASH_WriteEnable();					//写使能
			MICRON_FLASH_CS_Enable();
			MICRON_FLASH_WriteByte(ENTER_4BYTE_ADDRESS_MODE);
			MICRON_FLASH_CS_Disable();	
			MICRON_FLASH_WriteDisable();

			BSP_Delay_ms(1);
		}

		return FAIL;
	}
	#else 
	*/
	MICRON_FLASH_CS_Enable();
	MICRON_FLASH_WriteByte(0xB5);
	data_cfg = MICRON_FLASH_ReadByte();
	data_cfg = (data_cfg << 8) + MICRON_FLASH_ReadByte();
	MICRON_FLASH_CS_Disable();	

	MICRON_FLASH_WriteEnable();					//写使能
	
	MICRON_FLASH_CS_Enable();
	MICRON_FLASH_WriteByte(0xB1);
	MICRON_FLASH_WriteByte((data_cfg & 0xff00)>>8);
	MICRON_FLASH_WriteByte((data_cfg & 0x00fc)|0x01);
	MICRON_FLASH_CS_Disable();	

	
	MICRON_FLASH_CS_Enable();
	MICRON_FLASH_WriteByte(0xC8);
	data_cfg = MICRON_FLASH_ReadByte();
	MICRON_FLASH_CS_Disable();	

	MICRON_FLASH_WriteEnable();					//写使能
	
	MICRON_FLASH_CS_Enable();
	MICRON_FLASH_WriteByte(0xC5);
	MICRON_FLASH_WriteByte((data_cfg & 0xff)|0x01);
	MICRON_FLASH_CS_Disable();	

	MICRON_FLASH_CS_Enable();
	MICRON_FLASH_WriteByte(0xC8);
	data_cfg = MICRON_FLASH_ReadByte();
	MICRON_FLASH_CS_Disable();	
	
	#endif
	return SUCC;
	
}


/***********************************************************
**name:	MICRON_FLASH_B3Write4ByteData
**describe: 
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int MICRON_FLASH_B3Write4ByteData(INT32U addr, INT32U data)
{
	INT8U buff[4];

	buff[0] = (data & 0xFF000000)>>24;
	buff[1] = (data & 0x00FF0000)>>16;
	buff[2] = (data & 0x0000FF00)>>8;
	buff[3] = (data & 0x000000FF);
	return (MICRON_FLASH_B3WriteDataBuff(addr, buff, 4));
}

/***********************************************************
**name:	MICRON_FLASH_B3Read4ByteData
**describe: 
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void MICRON_FLASH_B3Read4ByteData(INT32U addr, INT32U *data)
{
	INT8U buff[4];

	MICRON_FLASH_B3ReadDataBuff(addr, buff, 4);

	*data = (buff[0]<<24) +  (buff[1]<<16) +  (buff[2]<<8) +  buff[3];
}

/***********************************************************
**name:	MICRON_FLASH_B3Write2ByteData
**describe: 
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int MICRON_FLASH_B3Write2ByteData(INT32U addr, INT16U data)
{
	INT8U buff[2];
	buff[0] = (data & 0xFF00)>>8;
	buff[1] = (data & 0x00FF);
	return (MICRON_FLASH_B3WriteDataBuff(addr, buff, 2));
}

/***********************************************************
**name:	MICRON_FLASH_B3Read2ByteData
**describe: 
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void MICRON_FLASH_B3Read2ByteData(INT32U addr, INT16U *data)
{
	INT8U buff[2];

	MICRON_FLASH_B3ReadDataBuff(addr, buff, 2);

	*data = (buff[0]<<8) +  buff[1];
}

