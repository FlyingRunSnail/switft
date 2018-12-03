
#define HW_BATTERY_DEF

#include <hw_battery.h>


/***********************************************************
**name:	HW_BATTERY_Init
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void HW_BATTERY_Init(void)
{
    BSP_GPIO_Init(HW_BATTERY_READ_PIN_PORT, HW_BATTERY_READ_PIN, GPIO_Mode_OUT, GPIO_OType_PP,GPIO_PuPd_NOPULL);
}

/***********************************************************
**name:	HW_Battery_Read_Enable
**describe: get battery internal enable
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
static void HW_Battery_Read_Enable(void)
{
	BSP_GPIO_PinWriteHigh(HW_BATTERY_READ_PIN_PORT, HW_BATTERY_READ_PIN);
}

static void HW_Battery_Send_Read_Req(void)
{
#define BATTERY_REQ_BUF_LEN (7)
    INT8U brq[BATTERY_REQ_BUF_LEN] = {0xdd, 0xa5, 0x03, 0x00, 0xff, 0xfd, 0x77};
    
    SWIFT_UART3_SendBuff(&brq[0], BATTERY_REQ_BUF_LEN);
}

static void HW_Battery_Rcv_Data(INT8U *buf, INT32U len)
{
    
}

static void HW_Battery_Buf_Dump(INT8U *buf, INT32U len)
{
    INT32U i;

    for (i = 0; i < len; i++)
    {
        printf("0x%02x ", buf[i]);
    }

    printf("\r\n");
}

/***********************************************************
**name:	HW_Battery_Read
**describe:
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void HW_BATTERY_Read(INT8U *buf, INT32U len)
{
    HW_Battery_Read_Enable();
    BSP_Delay_ms(1);

    HW_Battery_Send_Read_Req();
    SWIFT_UART3_RecvBuff(buf, &len);

    HW_Battery_Buf_Dump(buf, len);
}

