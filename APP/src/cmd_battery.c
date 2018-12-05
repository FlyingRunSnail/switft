#include <stdlib.h>
#include "command.h"
#include "cmd_battery.h"
#include "exports.h"
#include "hw_battery.h"
#include "kfifo.h"
#include "def.h"


typedef struct st_battery_base_info
{
     unsigned short total_voltage;
     unsigned short current;
     unsigned short remainder_capcity;
     unsigned short std_capcity;
     unsigned short recycle_times;
     unsigned short manufacture_date;
     unsigned short proportionate_status;
     unsigned short proportionate_status_h;
     unsigned short protect_status;
     unsigned char  software_ver;
     unsigned char  rsoc;
     unsigned char  fet_ctrl_status;
     unsigned char  bat_cnt;
     unsigned char  ntc_cnt;

}battery_base_info_t;

#define BATTERY_BUF_LEN (64)
char battery_buf[BATTERY_BUF_LEN] = {0};
struct kfifo *battery_fifo;

static void show_battery_info(void)
{
    unsigned int len;
    char buf[BATTERY_BUF_LEN] = {0};
    battery_base_info_t *bbi;

    if (!battery_fifo)
    {
        return ;
    }

    len = kfifo_len(battery_fifo);
    if (len > 0)
    {
        kfifo_get(battery_fifo, buf, len);

        bbi = (battery_base_info_t *)&buf[4];

        printf("total_voltage:          0x%04x\r\n", ntohs(bbi->total_voltage));
        printf("current:                0x%04x\r\n", ntohs(bbi->current));
        printf("remainder_capcity:      0x%04x\r\n", ntohs(bbi->remainder_capcity));
        printf("std_capcity:            0x%04x\r\n", ntohs(bbi->std_capcity));
        printf("recycle_times:          0x%04x\r\n", ntohs(bbi->recycle_times));
        printf("manufacture_date:       0x%04x\r\n", ntohs(bbi->manufacture_date));
        printf("proportionate_status:   0x%04x\r\n", ntohs(bbi->proportionate_status));
        printf("proportionate_status_h: 0x%04x\r\n", ntohs(bbi->proportionate_status_h));
        printf("protect_status:         0x%04x\r\n", ntohs(bbi->protect_status));
        printf("software_ver:           0x%02x\r\n", bbi->software_ver);
        printf("rsoc:                   0x%02x\r\n", bbi->rsoc);
        printf("fet_ctrl_status:        0x%02x\r\n", bbi->fet_ctrl_status);
        printf("bat_cnt:                0x%02x\r\n", bbi->bat_cnt);
        printf("ntc_cnt:                0x%02x\r\n", bbi->ntc_cnt);
    }
}

static int do_battery(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    INT8U buf[34] = {0};
    INT32U len = 0;

	if (argc != 2) 
	{
		return -1;
	}

    if (!strcmp(argv[1], "read"))
    {
        HW_BATTERY_Read(buf, &len);
    }
    else if (!strcmp(argv[1], "info"))
    {
        show_battery_info();
    }
    else
    {
        return -1;
    }
	
	return 0;
}

int cmd_battery_init(void)
{
	int ret;

    battery_fifo = kfifo_alloc(battery_buf, BATTERY_BUF_LEN);
    if (!battery_fifo)
    {
        printf("battery fifo alloc failed.\r\n");
        return -1;
    }
	
	ret = cmd_register(
		"battery", SHELL_SYS_MAXARGS, 0, do_battery,
		"battery read/info",
		"battery read/info\r\n	 - read or display battery info\r\n"
	);

	return ret;
}


