
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

static INT16U battery_check_sum(INT8U *data, INT32U len)
{
    INT16U checksum = 0;
    INT32U i;

    for (i = 0; i < len; i++)
    {
        checksum += data[i];
    }

    checksum = ~checksum;
    checksum += 1;

    return checksum;
}

static void HW_Battery_Send_Read_Req(void)
{
#define BATTERY_REQ_BUF_LEN (7)
#define BATTERY_REQ_HEAD    (0)
#define BATTERY_REQ_START   (1)
#define BATTERY_REQ_FUNC    (2)
#define BATTERY_REQ_LEN     (3)
#define BATTERY_REQ_CSUM_H  (4)
#define BATTERY_REQ_CSUM_L  (5)
#define BATTERY_REQ_TAIL    (6)

    INT8U brq[BATTERY_REQ_BUF_LEN] = {0xdd, 0xa5, 0x03, 0x0, 0x0, 0x0, 0x77};
    INT32U len = 2;
    INT16U checksum = battery_check_sum(&brq[BATTERY_REQ_START], len);

    brq[BATTERY_REQ_CSUM_H] = checksum >> 8;
    brq[BATTERY_REQ_CSUM_L] = checksum & 0xff;
    
    SWIFT_UART3_SendBuff(&brq[BATTERY_REQ_HEAD], BATTERY_REQ_BUF_LEN);
}

static void HW_Battery_Rcv_Data(INT8U *buf, INT32U len)
{
    
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
void HW_Battery_Read(battery_id_t id, battery_base_info_t *bbi)
{
    HW_Battery_Read_Enable();
	BSP_GPIO_PinWriteHigh(HW_4G_PIN_PORT,HW_4G_PIN);
}

