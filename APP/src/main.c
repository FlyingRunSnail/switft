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

#include <app_shell.h>


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
	OS_ERR      os_err;
	INT32U		IDbuff[3];
	INT32U period_ms1 = 100,period_ms2,period_cnt1=0,period_cnt2=0;
	INT16U delay_ms;

	(void)p_arg;

	CPU_Init();                                                 /* Init CPU name & int. dis. time measuring fncts.          */	
	Mem_Init();                                                 /* Initialize Memory managment                              */
	BSP_CPU_TickInit();                                         /* Start Tick Initialization                                */
	                                                /* Init BSP fncts.                                          */
	
	 ACUDevInfo.ACUCurrSta.dev_work_sta = 0xFF;                                               
	BSP_Init(); 
	

	APP_Shell_Init(); 
	
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
    while(1)
    {
		BSP_Delay_ms(250);
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
               
#if 0
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

