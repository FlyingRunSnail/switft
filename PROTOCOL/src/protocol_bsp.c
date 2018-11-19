

#define  PROTOCOL_BSP_DEF


#include <protocol_bsp.h>


INT8U UDP_RevBuff[100]={0};
INT16U UDP_RevBuff_len = 0;

INT8U UDP_TxBuff[1500]={0};
INT16U UDP_TxBuff_len = 0;


strPCCOMMPro Txframebuff,Rxframebuff;


INT8U UDP_TestTxBuff[1500]={0};

/***********************************************************
**name:	
**describe:  
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void PROTOCOL_RevFframe_Get(void*p,INT16U len)
{
 	memcpy(UDP_RevBuff,p,len);
 	UDP_RevBuff_len = len;
}

/***********************************************************
**name:	
**describe:  
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int PROTOCOL_RevFrame_Check(INT8U *buff, INT16U len)
{
	INT8U datalen;
	INT8U checksum=0;
	
	if( buff[0] != 0xBC)
		return FAIL;
	datalen = buff[1];

	if( len > datalen + 4 )
		len = datalen + 4;

	if( buff[len -1] != 0xCB)
		return FAIL;

	checksum = TOOL_CheckSum8BitGet(buff,len-2);
	if( checksum != buff[len -2] )
		return FAIL;

	Rxframebuff.head = buff[0];
	Rxframebuff.datalen = datalen;
	Rxframebuff.databuff = &buff[3];
	Rxframebuff.checksum = checksum;
	Rxframebuff.end = 0xCB;

	return SUCC;
}


/***********************************************************
**name:	PROTOCOL_TxFrame_Get
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void PROTOCOL_TxFrame_Get(strPCCOMMPro framebuff)
{
	INT16U index=0;

	UDP_TxBuff[index++] = framebuff.head;
	UDP_TxBuff[index++] = framebuff.datalen;
	memcpy(&UDP_TxBuff[index],framebuff.databuff,framebuff.datalen);
	index += framebuff.datalen;
	UDP_TxBuff[index++] = TOOL_CheckSum8BitGet(UDP_TxBuff,index);
	UDP_TxBuff[index++] = framebuff.end;

	UDP_TxBuff_len = index;
}

/***********************************************************
**name:	PROTOCOL_PCcomm_Deal
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void PROTOCOL_PCcomm_Deal(void*Revp,INT16U rev_len)
{
	INT16U i=0;
	
	PROTOCOL_RevFframe_Get(Revp,rev_len);

	PROTOCOL_RevFrame_Check(UDP_RevBuff, UDP_RevBuff_len);

	
	
	PROTOCOL_TxFrame_Get(Txframebuff);
}

