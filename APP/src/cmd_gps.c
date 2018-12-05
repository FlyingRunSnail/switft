#include <stdlib.h>
#include "command.h"
#include "cmd_gps.h"
#include "exports.h"
#include "hw_gps.h"

#define GPS_RCV_BUF_LEN (256)
static char gps_rcv_buf[GPS_RCV_BUF_LEN] = {0};
struct kfifo *gps_rcv_fifo;

static void get_gps_info(void)
{
    int i;
    int len;

    if (!gps_rcv_fifo)
    {
        return ;
    }

    len = kfifo_len(gps_rcv_fifo);
    if (len > 0)
    {
        kfifo_get(gps_rcv_fifo, gps_rcv_buf, len);

        for (i = 0; i < len; i++)
        {
            printf("%c", gps_rcv_buf[i]);
        }
    }
}

static int do_gps(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
	if (argc != 2) 
	{
		return -1;
	}

    if (!strcmp(argv[1], "on"))
    {
        HW_GPS_On();
    }
    else if (!strcmp(argv[1], "off"))
    {
        HW_GPS_Off();
    }
    else if (!strcmp(argv[1], "reset"))
    {
        HW_GPS_Rst();
    }
    else if (!strcmp(argv[1], "info"))
    {
        get_gps_info();
    }
    else
    {
        return -1;
    }
	
	return 0;
}

int cmd_gps_init(void)
{
	int ret;

    gps_rcv_fifo = kfifo_alloc(gps_rcv_buf, GPS_RCV_BUF_LEN);
    if (!gps_rcv_fifo)
    {
        printf("gps rcv fifo alloc failed.\r\n");
        return -1;
    }
	
	ret = cmd_register(
		"gps", SHELL_SYS_MAXARGS, 0, do_gps,
		"gps on/off/reset/info",
		"gps on/off/reset/info\r\n	 - control the gps on/off/reset and get gps info\r\n"
	);

	return ret;
}


