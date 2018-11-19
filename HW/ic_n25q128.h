


#ifdef MICRON_FLASH_DEF
	#define MICRON_FLASH_GLOBE
#else
	#define MICRON_FLASH_GLOBE extern
#endif


#define M25P128_SEM_DEF

#include <hw_spiflash.h>

#define MCU_FLASH_CS_SEL		0x01
#define FPGA_FLASH_CS_SEL		0x02

#define SPI_FLASH_SectorSize_64k     65536

#define SPI_FLASH_SectorSize         4096
#define SPI_FLASH_PageSize           256


#define FLASH_MAX_ADDR			0xFFFFFF

// 4 byte address mode opt
#define ENTER_4BYTE_ADDRESS_MODE	0xB7
#define EXIT_4BYTE_ADDRESS_MODE		0xE9



/***********************************************************
**name:	MICRON_FLASH_CS_Enable
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
MICRON_FLASH_GLOBE void MICRON_FLASH_CS_Enable(void);

/***********************************************************
**name:	MICRON_FLASH_CS_Disable
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
MICRON_FLASH_GLOBE void MICRON_FLASH_CS_Disable(void);

/***********************************************************
**name:	S25FL128S_WriteByte
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
MICRON_FLASH_GLOBE INT8U MICRON_FLASH_WriteByte(INT8U data);

/***********************************************************
**name:	MICRON_FLASH_ReadByte
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
MICRON_FLASH_GLOBE INT8U MICRON_FLASH_ReadByte(void);

/***********************************************************
**name:	MICRON_FLASH_WriteEnable
**describe: 写使能，所有涉及对flash的写、擦除的操作均需要先调用此函数
**input:			
**output:	none
**return:	staus   byte
**autor:  andiman
**date:
************************************************************/
MICRON_FLASH_GLOBE void MICRON_FLASH_WriteEnable(void);

/***********************************************************
**name:	MICRON_FLASH_WriteEnable
**describe: 写禁能，建议每次进行写、擦除之后调用此函数
**input:			
**output:	none
**return:	staus   byte
**autor:  andiman
**date:
************************************************************/
MICRON_FLASH_GLOBE void MICRON_FLASH_WriteDisable(void);

/***********************************************************
**name:	MICRON_FLASH_Read_StaReg
**describe:
**input:			
**output:	none
**return:	staus   byte
**autor:  andiman
**date:
************************************************************/
MICRON_FLASH_GLOBE INT8U MICRON_FLASH_Read_StaReg(void);

MICRON_FLASH_GLOBE void MICRON_FLASH_Write_StaReg(INT8U data);

/***********************************************************
**name:	MICRON_FLASH_Read_FlagReg
**describe: 读取状态标志寄存器
**input:			
**output:	none
**return:	staus   byte
**autor:  andiman
**date:
************************************************************/
MICRON_FLASH_GLOBE INT8U MICRON_FLASH_Read_FlagReg(void);

/***********************************************************
**name:	MICRON_FLASH_WaitPageProgram
**describe:
**input:			
**output:	none
**return:	staus   byte
**autor:  andiman
**date:
************************************************************/
MICRON_FLASH_GLOBE void MICRON_FLASH_WaitPageProgram(void);

/***********************************************************
**name:	MICRON_FLASH_WaitSectorErase
**describe:
**input:			
**output:	none
**return:	staus   byte
**autor:  andiman
**date:
************************************************************/
MICRON_FLASH_GLOBE void MICRON_FLASH_WaitSectorErase(void);

/***********************************************************
**name:	MICRON_FLASH_WaitBulkErase
**describe:
**input:			
**output:	none
**return:	staus   byte
**autor:  andiman
**date:
************************************************************/
MICRON_FLASH_GLOBE void MICRON_FLASH_WaitBulkErase(void);

/***********************************************************
**name:	MICRON_FLASH_Reset
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
MICRON_FLASH_GLOBE void MICRON_FLASH_Reset(void);

/***********************************************************
**name:	MICRON_FLASH_ReadID
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
MICRON_FLASH_GLOBE void MICRON_FLASH_ReadID(INT8U *idbuff);

/***********************************************************
**name:	MICRON_FLASH_B3WriteDataByte
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
MICRON_FLASH_GLOBE int MICRON_FLASH_B3WriteDataByte(INT32U addr, INT8U byte);

/***********************************************************
**name:	MICRON_FLASH_B3ReadDataByte
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
MICRON_FLASH_GLOBE INT8U MICRON_FLASH_B3ReadDataByte(INT32U addr);

/***********************************************************
**name:	MICRON_FLASH_B4WriteDataByte
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
MICRON_FLASH_GLOBE int MICRON_FLASH_B4WriteDataByte(INT32U addr, INT8U byte);

/***********************************************************
**name:	MICRON_FLASH_B4ReadDataByte
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
MICRON_FLASH_GLOBE INT8U MICRON_FLASH_B4ReadDataByte(INT32U addr);

/***********************************************************
**name:	MICRON_FLASH_B3PageWrite
**describe: 
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
MICRON_FLASH_GLOBE void MICRON_FLASH_B3PageWrite(INT32U addr, INT8U *buff, INT32U len);

/***********************************************************
**name:	MICRON_FLASH_B4PageWrite
**describe: 
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
MICRON_FLASH_GLOBE void MICRON_FLASH_B4PageWrite(INT32U addr, INT8U *buff, INT32U len);

/***********************************************************
**name:	MICRON_FLASH_B3WriteDataBuff
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
MICRON_FLASH_GLOBE int MICRON_FLASH_B3WriteDataBuff(INT32U addr, INT8U *buff, INT32U len);

/***********************************************************
**name:	MICRON_FLASH_B3ReadDataBuff
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
MICRON_FLASH_GLOBE void MICRON_FLASH_B3ReadDataBuff(INT32U addr, INT8U *buff, INT32U len);

/***********************************************************
**name:	MICRON_FLASH_B4WriteDataBuff
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
MICRON_FLASH_GLOBE int MICRON_FLASH_B4WriteDataBuff(INT32U addr, INT8U *buff, INT32U len);

/***********************************************************
**name:	MICRON_FLASH_B4ReadDataBuff
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
MICRON_FLASH_GLOBE void MICRON_FLASH_B4ReadDataBuff(INT32U addr, INT8U *buff, INT32U len);

/***********************************************************
**name:	MICRON_FLASH_B3SubSectorErase
**describe: 删除当前所在地址的子扇区，4KB大小
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
MICRON_FLASH_GLOBE void MICRON_FLASH_B3SubSectorErase(INT32U addr);

/***********************************************************
**name:	MICRON_FLASH_B4SubSectorErase
**describe: 删除当前所在地址的子扇区，4KB大小
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
MICRON_FLASH_GLOBE void MICRON_FLASH_B4SubSectorErase(INT32U addr);

/***********************************************************
**name:	MICRON_FLASH_B3SectorErase
**describe: 删除当前所在地址的子扇区，64KB大小
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
MICRON_FLASH_GLOBE void MICRON_FLASH_B3SectorErase(INT32U addr);

/***********************************************************
**name:	MICRON_FLASH_B4SectorErase
**describe: 删除当前所在地址的子扇区，64KB大小
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
MICRON_FLASH_GLOBE void MICRON_FLASH_B4SectorErase(INT32U addr);

/***********************************************************
**name:	MICRON_FLASH_BulkErase
**describe: 擦除整个flash
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
MICRON_FLASH_GLOBE void MICRON_FLASH_BulkErase(void);

/***********************************************************
**name:	MICRON_FLASH_Byte3AddrMode_Set
**describe: 
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
MICRON_FLASH_GLOBE int MICRON_FLASH_Byte3AddrMode_Set(void);

/***********************************************************
**name:	MICRON_FLASH_Byte4AddrMode_Set
**describe: 
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
MICRON_FLASH_GLOBE int MICRON_FLASH_Byte4AddrMode_Set(void);



MICRON_FLASH_GLOBE int MICRON_FLASH_B3Write4ByteData(INT32U addr, INT32U data);


MICRON_FLASH_GLOBE void MICRON_FLASH_B3Read4ByteData(INT32U addr, INT32U *data);


MICRON_FLASH_GLOBE int MICRON_FLASH_B3Write2ByteData(INT32U addr, INT16U data);


MICRON_FLASH_GLOBE void MICRON_FLASH_B3Read2ByteData(INT32U addr, INT16U *data);




