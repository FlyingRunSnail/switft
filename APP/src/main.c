/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                          (c) Copyright 2003-2012; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                            EXAMPLE CODE
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                           STM3220G-EVAL
*                                         Evaluation Board
*
* Filename      : app.c
* Version       : V1.00
* Programmer(s) : EHS
*                 DC 
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define  MAIN_DEF

#define DEV_DEF


//#include  <includes.h>
#include <main.h>
#include <devdef.h>
#include <math.h>
#include "cli.h"
#include <app_shell.h>
#include "def.h"

/*********************************************************************************************************
  EVENT(ITC)    事件(任务间通信)
*********************************************************************************************************/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/

/* ----------------- APPLICATION GLOBALS ------------------ */


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/
static  void  TaskStart (void *p_arg);
static  void  TaskCreate (void);

void   AppObjCreate(void);


/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
* 
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************
*/
int main(void)
{
    OS_ERR  err;
    BSP_IntDisAll();                                            /* Disable all interrupts.                                  */
    SystemInit();
    CPU_Init();                                                 /* Initialize uC/CPU services.                              */
    OSInit(&err);                                               /* Init OS.                                                 */
    
    OSTaskCreate((OS_TCB     *)&TaskStartTCB,                /* Create the start task                                    */
                 (CPU_CHAR   *)"Task Start",
                 (OS_TASK_PTR ) TaskStart,
                 (void       *) 0,
                 (OS_PRIO     ) TASK_START_PRIO,
                 (CPU_STK    *)&TaskStartStk[0],
                 (CPU_STK     )(TASK_START_STK_SIZE / 10u),
                 (CPU_STK_SIZE) TASK_START_STK_SIZE,
                 (OS_MSG_QTY  ) 0,
                 (OS_TICK     ) 0,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
    
    OSStart(&err); 

    while (DEF_ON) {
        /* Should Never Get Here */
    };
}


/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'AppTaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

static  void  TaskStart (void *p_arg)
{
	(void)p_arg;

	CPU_Init();                                                 /* Init CPU name & int. dis. time measuring fncts.          */	
	Mem_Init();                                                 /* Initialize Memory managment                              */
	BSP_CPU_TickInit();                                         /* Start Tick Initialization                                */
	                                                /* Init BSP fncts.                                          */
	BSP_Init(); 
	
	APP_Shell_Init(); 
	
    HW_PHY_Rst();

	BSP_Delay_ms(500);

 	LWIP_Init();

	#if (OS_TASK_STAT_EN > 0)
    		OSStatInit();                                               /* Determine CPU capacity                                   */
	#endif

	udpecho_init();

 	BSP_Delay_ms(100);

	// 创建其他任务
	TaskCreate();                                            /* Create Application Tasks                                 */

	// 初始化看门狗
	//HW_WTD_Init();

  	//从时钟芯片 获取时间，随后在本任务中自动计时
	while (DEF_TRUE) 
	{     
		//HW_WTD_Feed();
		HW_LED1_ON();
		HW_LED2_ON();
		BSP_Delay_ms(250);
		HW_LED1_OFF();
		HW_LED2_OFF();
		BSP_Delay_ms(250);
	}
}

/***********************************************************
**name: TaskShell
**describe:   设备接收调试串口的命令，分解命令，以及处理命令
**input:            
**output:   none
**return: none
**autor:  andiman
**date:
************************************************************/
static  void  TaskShell(void *p_arg)
{
    while (DEF_TRUE) 
    {         
        cli_loop();
    }
}

/***********************************************************
**name: TaskEthRelink
**describe:         以太网连接检测补丁代码，在上电初始化时，未能检测到连接，而在上电稳定之后有网线接入，则重启设备，以便于
**              设备能够以太网通信
**input:            
**output:   none
**return: none
**autor:  andiman
**date:
************************************************************/
static  void  TaskEthRelink(void *p_arg)
{
    INT8U buf[34] = {0};
    INT32U len = 0;
    INT32U i;
    INT8S ret;

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

    battery_base_info_t *bbi;

    while(1)
    {
		//BSP_Delay_ms(250);
        buf[len] = SWIFT_USART3_GETC();
        len = len + 1;
        if (len == 34)
        {
#if 0
            for (i = 0; i < len; i++)
            {
                printf("0x%02x ", buf[i]);
            }
#endif
            ret = TOOL_BatteryFrameCheck(buf, len);
            if (ret == 0)
            {
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

            len = 0;
        }
    }
}

/*
*********************************************************************************************************
*                                          TaskCreate()
*
* Description : Create application tasks.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : AppTaskStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/

static  void  TaskCreate (void)
{
    OS_ERR  err;
                 
    OSTaskCreate((OS_TCB     *)&TaskShellTCB,                /* Create the start task                                    */
                 (CPU_CHAR   *)"Task Shell",
                 (OS_TASK_PTR ) TaskShell,
                 (void       *) 0,
                 (OS_PRIO     ) TASK_SHELL_PRIO,
                 (CPU_STK    *)&TaskShellStk[0],
                 (CPU_STK     )(TASK_SHELL_STK_SIZE / 10u),
                 (CPU_STK_SIZE) TASK_SHELL_STK_SIZE,
                 (OS_MSG_QTY  ) 0,
                 (OS_TICK     ) 0,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);     
               
#if 1
    OSTaskCreate((OS_TCB     *)&TaskEthRelinkTCB,                /* Create the start task                                    */
                 (CPU_CHAR   *)"Task EthRelink",
                 (OS_TASK_PTR ) TaskEthRelink,
                 (void       *) 0,
                 (OS_PRIO     ) TASK_EthRelink_PRIO,
                 (CPU_STK    *)&TaskEthRelinkStk[0],
                 (CPU_STK     )(TASK_EthRelink_STK_SIZE / 10u),
                 (CPU_STK_SIZE) TASK_EthRelink_STK_SIZE,
                 (OS_MSG_QTY  ) 0,
                 (OS_TICK     ) 0,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

#endif
}

