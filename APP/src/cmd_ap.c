#include <stdlib.h>
#include "command.h"
#include "cmd_ap.h"
#include "exports.h"
#include "kfifo.h"


#define AP_RCV_BUF_LEN (1024)

static char ap_rcv_buf[AP_RCV_BUF_LEN] = {0};

struct kfifo *ap_rcv_fifo;


static void get_rcv_fifo(void)
{
    int i;
    char lbuf[AP_RCV_BUF_LEN] = {0};

    if (!ap_rcv_fifo)
    {
        return ;
    }

    int len = kfifo_len(ap_rcv_fifo);
    if (len > 0)
    {
        kfifo_get(ap_rcv_fifo, lbuf, len);
    
        for (i = 0; i < len; i++)
        {
            // change to UART2 later
            printf("%c", lbuf[i]);
        }
    }

    return ;
}

static void clear_rcv_fifo(void)
{
    if (ap_rcv_fifo)
    {
        kfifo_reset(ap_rcv_fifo);
    }
}

static int do_ap(cmd_tbl_t *cmdtp, int flag, int argc, char * const argv[])
{
    char buf[32] = {0};
    int i;

	if (argc < 2) 
	{
		return -1;
	}

#if 0
    printf("argc: %d\r\n", argc);
    for (i = 0; i < argc; i++)
    {
        printf("%s ", argv[i]);
    }

    printf("\r\n");
#endif

    if (!strcmp(argv[1], "send"))
    {
        for (i = 2; i < argc; i++)
        {
            //printf("i %d %s\r\n", i, argv[i]);
            SWIFT_UART6_SendBuff(argv[i], strlen(argv[i]));
            SWIFT_UART6_SendBuff(" ", 1);
        }

        SWIFT_UART6_SendBuff("\r\n", 2);
    }
    else if (!strcmp(argv[1], "rcv"))
    {
        get_rcv_fifo();
    }
    else if (!strcmp(argv[1], "clear"))
    {
        clear_rcv_fifo();
    }
    else
    {
        return -1;
    }
	
	return 0;
}

int cmd_ap_init(void)
{
	int ret;

    ap_rcv_fifo = kfifo_alloc(ap_rcv_buf, AP_RCV_BUF_LEN);
    if (!ap_rcv_fifo)
    {
        printf("ap rcv fifo init failed.\r\n");
        return -1;
    }
	
	ret = cmd_register(
		"ap", SHELL_SYS_MAXARGS, 0, do_ap,
		"ap send xxx or rcv",
		"ap send xxx or ap rcv\r\n	 - send command to ap or rcv data from ap\r\n"
	);

	return ret;
}


