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
#include <app_pducomm.h>
#include <app_oledcomm.h>
#include <app_eleccoder.h>
#include <app_log.h>

static INT16U Period_Ms = 1000;
static INT16U Period_Ms_Cnt = 0;



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
    OS_ERR err;
    INT32U cnt=0;
    INT8U curr_eth_link,old_eth_link=1;

    
    
    if( APP_PARAM_EthRelinkResetMCUFlag_Read()>0)
    {
        curr_eth_link = APP_MONITOR_ETH_LinkSta_Get();
        if( curr_eth_link > 0)
        {
            APP_PARAM_EthRelinkResetMCUFlag_Del();
            
            if( ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode == 0x02)
            {
                TaskSatelliteTrackCreate();
            }
            else if( ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode == 0x03)
            {
                TaskSatGradientTrackCreate();
            }
            else if( ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode == 0x04)
            {
                TaskAntennaRecordGotoCreate();
            }
            
            OSTaskDel((OS_TCB  *)0,&err);
            
        }
    }

    old_eth_link = APP_MONITOR_ETH_LinkSta_Get();
    while (DEF_TRUE) 
    {         
        //监测以太网的连接状态
        curr_eth_link = APP_MONITOR_ETH_LinkSta_Get();

        if( curr_eth_link >  0 )
        {
            if( old_eth_link != curr_eth_link )
            {
                APP_MONITOR_ETH_ReLink_Deal(curr_eth_link);
            }
            cnt++;
            if( cnt > 1000)
                OSTaskDel((OS_TCB  *)0,&err);
        }
        else
            cnt = 0;
            
        old_eth_link = curr_eth_link;
        BSP_Delay_ms(50);   

        
    }
}



/***********************************************************
**name: TaskSatelliteTrack
**describe:  一键对星任务
**input:            
**output:   none
**return: none
**autor:  andiman
**date:
************************************************************/
static  void  TaskSatelliteTrack(void *p_arg)
{
    OS_ERR err;


    Task_Run_Flag[0] = 1;
    while (DEF_TRUE) 
    {         
        if( ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode == 0x02)
        {
            //if( APP_Satellite_Track_Start() == FAIL)
            {
                ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode = 0x00;
                ACUDevInfo.ACUCurrSta.dev_work_sta = 0x00;

                //APP_OptLOG_SaveCode(LOG_OPT_AUTO_AIM_STAR_END_BY_ACU_UNLOCK);
                
            }
            //else
            {
                //APP_OptLOG_SaveCode(LOG_OPT_AUTO_AIM_STAR_END_BY_ACU_LOCK);
            }
        }
        Task_Run_Flag[0] = 0;

        //if(Antenna_AllDriveEnd() == FAIL)
        //    Antenna_AllDriveEnd();
        OSTaskDel ((OS_TCB  *)0,&err);
        BSP_Delay_ms(50);
    }
}


/*
*********************************************************************************************************
*                                          TaskSatelliteTrackDel
*
* Description :
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : 
*
* Note(s)     : none.
*********************************************************************************************************
*/
void  TaskSatelliteTrackDel (void)
{
    OS_ERR err;

    Task_Run_Flag[0] = 0;
    OSTaskDel ((OS_TCB  *)&TaskSatelliteTrackTCB,&err);
    
}

/*
*********************************************************************************************************
*                                          TaskSatelliteTrackCreate()
*
* Description : Create application tasks.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : 
*
* Note(s)     : none.
*********************************************************************************************************
*/
void  TaskSatelliteTrackCreate(void)
{
    OS_ERR err;
    OSTaskCreate((OS_TCB     *)&TaskSatelliteTrackTCB,                /* Create the start task                                    */
                 (CPU_CHAR   *)"Task SatelliteTrack",
                 (OS_TASK_PTR ) TaskSatelliteTrack,
                 (void       *) 0,
                 (OS_PRIO     ) TASK_SatelliteTrack_PRIO,
                 (CPU_STK    *)&TaskSatelliteTrackStk[0],
                 (CPU_STK     )(TASK_SatelliteTrack_STK_SIZE / 10u),
                 (CPU_STK_SIZE) TASK_SatelliteTrack_STK_SIZE,
                 (OS_MSG_QTY  ) 0,
                 (OS_TICK     ) 0,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);     
}




/***********************************************************
**name: TaskAntennaRecordGoto
**describe:  驱动天线前往指定位置
**input:            
**output:   none
**return: none
**autor:  andiman
**date:
************************************************************/
static  void  TaskAntennaRecordGoto(void *p_arg)
{
    
    OS_ERR err;

    INT8U dir[3] = {0},cmp_dir[3]={0},motor_run_flag[3]={0};
    INT32U err_cnt[3] = {0};
    FP32    aim_polar,aim_position,aim_cabrage;
    FP32       old_polar,old_position,old_cabrage;
    FP32        D_value[3];
    INT8U i;

    while(1)
    {
        BSP_Delay_ms(100);

        if( Task_Run_Flag[2] == 1 )
            break;
    }

    

    TaskAntennaRecordGoto_Begin:

    Task_Run_Flag[2] = 1;

    if( ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode == 0x04)  // 姿态回归模式
    {
        aim_polar = ACUDevInfo.ACUParam.AntennaParam.aim_polar;
        aim_position = ACUDevInfo.ACUParam.AntennaParam.aim_position;
        aim_cabrage = ACUDevInfo.ACUParam.AntennaParam.aim_cabrage;
        ACUDevInfo.ACUCurrSta.dev_work_sta = 0x40;
    }
    else if( ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode == 0x01 )        // 手动模式
    {
        aim_polar = ACUDevInfo.ACUParam.AntennaParam.aim_polar_hand;
        aim_position = ACUDevInfo.ACUParam.AntennaParam.aim_position_hand;
        aim_cabrage = ACUDevInfo.ACUParam.AntennaParam.aim_cabrage_hand;
        ACUDevInfo.ACUCurrSta.dev_work_sta = 0x10;
    }

    memset(motor_run_flag,0x00,sizeof(motor_run_flag));
    memset(err_cnt,0x00,sizeof(err_cnt));

    for(i=0;i<3;i++)
    {
        // 方位启动
        D_value[motor_position_id-1] = 0.01;
        if( ACUDevInfo.ACUCurrSta.antenna_position > aim_position + D_value[motor_position_id-1] )
            dir[motor_position_id-1] = ANTENNA_DIR_LEFT_DOWN_ANTICLOCK;// 左转
        else if( ACUDevInfo.ACUCurrSta.antenna_position < aim_position - D_value[motor_position_id-1] )
            dir[motor_position_id-1] = ANTENNA_DIR_RIGHT_UP_CLOCK;// 右转
        else
            dir[motor_position_id-1] = 0;

        if( dir[motor_position_id-1] > 0)
        {
            if(Antenna_DriveStart(motor_position_id,dir[motor_position_id-1]) == FAIL)  
            {
                BSP_Delay_ms(50);
                if(Antenna_DriveStart(motor_position_id,dir[motor_position_id-1]) == FAIL)  
                    goto TaskAntennaRecordGoto_END;
            }
            motor_run_flag[motor_position_id-1] = 1;
        }
        else
            motor_run_flag[motor_position_id-1] = 0;

        // 俯仰启动
        D_value[motor_cabrage_id-1] = 0.01;
        if( ACUDevInfo.ACUCurrSta.antenna_cabrage>aim_cabrage+ D_value[motor_cabrage_id-1]  )
            dir[motor_cabrage_id-1] = ANTENNA_DIR_LEFT_DOWN_ANTICLOCK;// 下转
        else if( ACUDevInfo.ACUCurrSta.antenna_cabrage < aim_cabrage - D_value[motor_cabrage_id-1]  )
            dir[motor_cabrage_id-1] = ANTENNA_DIR_RIGHT_UP_CLOCK;// 上转
        else
            dir[motor_cabrage_id-1] = 0;

        if( dir[motor_cabrage_id-1] > 0)
        {
            if(Antenna_DriveStart(motor_cabrage_id,dir[motor_cabrage_id-1]) == FAIL)    
            {
                BSP_Delay_ms(50);
                if(Antenna_DriveStart(motor_cabrage_id,dir[motor_cabrage_id-1]) == FAIL)    
                    goto TaskAntennaRecordGoto_END;
            }
            motor_run_flag[motor_cabrage_id-1] = 1;
        }
        else
            motor_run_flag[motor_cabrage_id-1] = 0;

        //极化启动
        if( ACUDevInfo.ACUParam.AntennaParam.elec_coder_num >= 3)
        {
            // 到达极化角位置
            D_value[motor_polar_id-1] = 0.05;
            if( ACUDevInfo.ACUCurrSta.antenna_polar > aim_polar + D_value[motor_polar_id-1] )
                dir[motor_polar_id-1]  = ANTENNA_DIR_LEFT_DOWN_ANTICLOCK;// 逆时针转
            else if( ACUDevInfo.ACUCurrSta.antenna_polar < aim_polar - D_value[motor_polar_id-1]  )
                dir[motor_polar_id-1]  = ANTENNA_DIR_RIGHT_UP_CLOCK;// 顺时针转 
            else
                dir[motor_polar_id-1]  = 0;

            if( dir[motor_polar_id-1]  > 0)
            {
                if(Antenna_DriveStart(motor_polar_id,dir[motor_polar_id-1] ) == FAIL)   
                {
                    BSP_Delay_ms(50);
                    if(Antenna_DriveStart(motor_polar_id,dir[motor_polar_id-1] ) == FAIL)   
                        goto TaskAntennaRecordGoto_END;
                }
                motor_run_flag[motor_polar_id-1] = 1;
            }
            else
                motor_run_flag[motor_polar_id-1] = 0;
            
        }

        // 结束处理
        while(1)
        {
            // 方位判断
            // 以下防止异常情况下，方位电极一个方向转动

            if( motor_run_flag[motor_position_id-1] > 0)
            {
                if( ACUDevInfo.ACUCurrSta.antenna_position > aim_position + D_value[motor_position_id-1] )
                    cmp_dir[motor_position_id-1] = ANTENNA_DIR_LEFT_DOWN_ANTICLOCK;// 左转
                else if( ACUDevInfo.ACUCurrSta.antenna_position < aim_position - D_value[motor_position_id-1] )
                    cmp_dir[motor_position_id-1] = ANTENNA_DIR_RIGHT_UP_CLOCK;// 右转
                else
                {
                    cmp_dir[motor_position_id-1] = 0;
                    if(Antenna_DriveEnd(motor_position_id,0x00) == FAIL)            //  停止
                    {
                        BSP_Delay_ms(50);
                        if(Antenna_DriveEnd(motor_position_id,0x00) == FAIL)    
                            goto TaskAntennaRecordGoto_END;
                    }
                    motor_run_flag[motor_position_id-1] = 0;
                }
                if( dir[motor_position_id-1] != cmp_dir[motor_position_id-1] )
                {
                    if(Antenna_DriveEnd(motor_position_id,0x00) == FAIL)            //  停止
                    {
                        BSP_Delay_ms(50);
                        if(Antenna_DriveEnd(motor_position_id,0x00) == FAIL)    
                            goto TaskAntennaRecordGoto_END;
                    }
                    motor_run_flag[motor_position_id-1] = 0;
                }

                BSP_Delay_ms(2);

                // 防止因电机故障，一直不动，导致无法退出本程序
                // 如果值一直在2.5倍最小分别率范围内，连续1000次，则判定电机已经无法运动，则判为停止运动
                if( (old_position <=  ACUDevInfo.ACUCurrSta.antenna_position + 0.008 ) && (old_position >=  ACUDevInfo.ACUCurrSta.antenna_position - 0.008))        
                {
                    err_cnt[ motor_position_id - 1]++;
                    if( err_cnt[ motor_position_id - 1] > 1000 )
                    {
                        if(Antenna_DriveEnd(motor_position_id,0x00) == FAIL)            //  停止
                        {
                            BSP_Delay_ms(200);
                            if(Antenna_DriveEnd(motor_position_id,0x00) == FAIL)    
                                goto TaskAntennaRecordGoto_END;
                        }
                        motor_run_flag[motor_position_id-1] = 0;
                    }
                }
                else
                {
                    old_position = ACUDevInfo.ACUCurrSta.antenna_position;
                    err_cnt[ motor_position_id - 1] = 0;
                }
            }
            BSP_Delay_ms(5);

            // 俯仰判断
            // 以下防止异常情况下，俯仰电机一个方向转动
            if( motor_run_flag[motor_cabrage_id-1] > 0)
            {
                if( ACUDevInfo.ACUCurrSta.antenna_cabrage>aim_cabrage+ D_value[motor_cabrage_id-1])
                    cmp_dir[motor_cabrage_id-1] = ANTENNA_DIR_LEFT_DOWN_ANTICLOCK;// 下转
                else if( ACUDevInfo.ACUCurrSta.antenna_cabrage < aim_cabrage - D_value[motor_cabrage_id-1] )
                    cmp_dir[motor_cabrage_id-1] = ANTENNA_DIR_RIGHT_UP_CLOCK;// 上转
                else
                {
                    cmp_dir[motor_cabrage_id-1] = 0;
                    if(Antenna_DriveEnd(motor_cabrage_id,0x00) == FAIL)
                    {
                        BSP_Delay_ms(50);
                        if(Antenna_DriveEnd(motor_cabrage_id,0x00) == FAIL)
                            goto TaskAntennaRecordGoto_END;
                    }
                    motor_run_flag[motor_cabrage_id-1] = 0;
                }
                if( dir[motor_cabrage_id-1] != cmp_dir[motor_cabrage_id-1] )
                {
                    if(Antenna_DriveEnd(motor_cabrage_id,0x00) == FAIL)         //  停止站东
                    {
                        BSP_Delay_ms(50);
                        if(Antenna_DriveEnd(motor_cabrage_id,0x00) == FAIL)
                            goto TaskAntennaRecordGoto_END;
                    }   
                    motor_run_flag[motor_cabrage_id-1] = 0;
                }

                BSP_Delay_ms(2);

                // 如果值一直在2.5倍最小分别率范围内，连续1000次，则判定电机已经无法运动，则判为停止运动
                if( (old_cabrage <=  ACUDevInfo.ACUCurrSta.antenna_cabrage + 0.008 ) && (old_cabrage >=  ACUDevInfo.ACUCurrSta.antenna_cabrage - 0.008))        
                {
                    err_cnt[ motor_cabrage_id - 1]++;
                    if( err_cnt[ motor_cabrage_id - 1] > 1000 )
                    {
                        if(Antenna_DriveEnd(motor_cabrage_id,0x00) == FAIL)         //  停止
                        {
                            BSP_Delay_ms(200);
                            if(Antenna_DriveEnd(motor_cabrage_id,0x00) == FAIL) 
                                goto TaskAntennaRecordGoto_END;
                        }
                        motor_run_flag[motor_cabrage_id-1] = 0;
                    }
                }
                else
                {
                    old_cabrage = ACUDevInfo.ACUCurrSta.antenna_cabrage;
                    err_cnt[ motor_cabrage_id - 1] = 0;
                }
            }

            BSP_Delay_ms(5);

            if( (ACUDevInfo.ACUParam.AntennaParam.elec_coder_num >= 3) && ( motor_run_flag[motor_polar_id-1] > 0))
            {
                // 极化判断
                // 以下防止异常情况下，极化电机一个方向转动
                if( ACUDevInfo.ACUCurrSta.antenna_polar >aim_polar + D_value[motor_polar_id-1])
                    cmp_dir[motor_polar_id-1] = ANTENNA_DIR_LEFT_DOWN_ANTICLOCK;// 下转
                else if( ACUDevInfo.ACUCurrSta.antenna_polar < aim_polar  - D_value[motor_polar_id-1] )
                    cmp_dir[motor_polar_id-1] = ANTENNA_DIR_RIGHT_UP_CLOCK;// 上转
                else
                {
                    cmp_dir[motor_polar_id-1] = 0;
                    if(Antenna_DriveEnd(motor_polar_id,0x00) == FAIL)
                    {
                        BSP_Delay_ms(50);
                        if(Antenna_DriveEnd(motor_polar_id,0x00) == FAIL)
                            goto TaskAntennaRecordGoto_END;
                    }
                    motor_run_flag[motor_polar_id-1] = 0;
                }
                if( dir[motor_polar_id-1] != cmp_dir[motor_polar_id-1] )
                {
                    if(Antenna_DriveEnd(motor_polar_id,0x00) == FAIL)           //  停止站东
                    {
                        BSP_Delay_ms(50);
                        if(Antenna_DriveEnd(motor_polar_id,0x00) == FAIL)
                            goto TaskAntennaRecordGoto_END;
                    }
                    motor_run_flag[motor_polar_id-1] = 0;
                }

                BSP_Delay_ms(2);

                // 如果值一直在2.5倍最小分别率范围内，连续1000次，则判定电机已经无法运动，则判为停止运动
                if( (old_polar <=  ACUDevInfo.ACUCurrSta.antenna_polar + 0.2 ) && (old_polar >=  ACUDevInfo.ACUCurrSta.antenna_polar - 0.2))        
                {
                    err_cnt[ motor_polar_id - 1]++;
                    if( err_cnt[ motor_polar_id - 1] > 1000 )
                    {
                        if(Antenna_DriveEnd(motor_polar_id,0x00) == FAIL)           //  停止
                        {
                            BSP_Delay_ms(200);
                            if(Antenna_DriveEnd(motor_polar_id,0x00) == FAIL)   
                                goto TaskAntennaRecordGoto_END;
                        }
                        motor_run_flag[motor_polar_id-1] = 0;
                    }
                }
                else
                {
                    old_polar = ACUDevInfo.ACUCurrSta.antenna_polar;
                    err_cnt[ motor_polar_id - 1] = 0;
                }

                
            }
            BSP_Delay_ms(5);

            if( (motor_run_flag[motor_position_id-1] == 0 ) && (motor_run_flag[motor_cabrage_id-1] == 0) &&(motor_run_flag[motor_polar_id-1] == 0))
                break;

            
            if( Task_Run_Flag[2] == 0 )         //  外部设置终止本程序
            {
                //if(Antenna_AllDriveEnd() == FAIL)           //  停止站东
               // {
                    //BSP_Delay_ms(50);
                    //if(Antenna_AllDriveEnd() == FAIL)
                    //    goto TaskAntennaRecordGoto_END;
                //}

                goto TaskAntennaRecordGoto_END;
                
            }

            
            if(motor_run_flag[motor_position_id-1] == 0 )
                BSP_Delay_ms(2);
            if(motor_run_flag[motor_cabrage_id-1] == 0 )
                BSP_Delay_ms(2);
            if(motor_run_flag[motor_polar_id -1] == 0 )
                BSP_Delay_ms(2);
        }
        BSP_Delay_s(3);
    }

    TaskAntennaRecordGoto_END:

    Task_Run_Flag[2] = 0;

    //if( Antenna_AllDriveEnd() == FAIL)
       // Antenna_AllDriveEnd();

    if( ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode == 0x00)
    {
        ACUDevInfo.ACUCurrSta.dev_work_sta = 0x00;
    }
    else if( ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode == 0x04)
    {
        //APP_OptLOG_SaveCode(LOG_OPT_ANTENNA_POSE_FINISH);

        ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode  = 0x00;
        ACUDevInfo.ACUCurrSta.dev_work_sta = 0x00;
        
    }
    else if( ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode == 0x01)
    {
        ACUDevInfo.ACUCurrSta.dev_work_sta = 0x11;
    }

        
    while(1)
    {
        if( Task_Run_Flag[2] == 1 )
            goto TaskAntennaRecordGoto_Begin;
        BSP_Delay_ms(50);
    }
}

/*
*********************************************************************************************************
*                                          TaskAntennaRecordGotoDel
*
* Description :
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : 
*
* Note(s)     : none.
*********************************************************************************************************
*/
void  TaskAntennaRecordGotoDel (void)
{
    OS_ERR err;

    Task_Run_Flag[2] = 0;
    //OSTaskDel ((OS_TCB  *)&TaskSatelliteTrackTCB,&err);
}

/*
*********************************************************************************************************
*                                          TaskAntennaRecordGotoCreate()
*
* Description : Create application tasks.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : 
*
* Note(s)     : none.
*********************************************************************************************************
*/
void  TaskAntennaRecordGotoCreate(void)
{
    OS_ERR err;

    Task_Run_Flag[2] = 1;

    #if 0
    OSTaskCreate((OS_TCB     *)&TaskAntennaRecordGotoTCB,                /* Create the start task    */
                 (CPU_CHAR   *)"Task AntennaRecordGoto",
                 (OS_TASK_PTR ) TaskAntennaRecordGoto,
                 (void       *) 0,
                 (OS_PRIO     ) TASK_AntennaRecordGoto_PRIO,
                 (CPU_STK    *)&TaskAntennaRecordGotoStk[0],
                 (CPU_STK     )(TASK_AntennaRecordGoto_STK_SIZE / 10u),
                 (CPU_STK_SIZE) TASK_AntennaRecordGoto_STK_SIZE,
                 (OS_MSG_QTY  ) 0,
                 (OS_TICK     ) 0,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);     

        #endif
}



/***********************************************************
**name: TaskSatGradientTrack
**describe:   追星任务
**input:            
**output:   none
**return: none
**autor:  andiman
**date:
************************************************************/
static  void  TaskSatGradientTrack(void *p_arg)
{
    OS_ERR err;


    Task_Run_Flag[1] = 1;
    while (DEF_TRUE) 
    {         
        if( ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode == 0x03)
        {
            //if(APP_Sat_GradientTrack() == FAIL)
            {
                ACUDevInfo.ACUParam.SeekSatParam.SeekSat_Mode = 0x00;
                ACUDevInfo.ACUCurrSta.dev_work_sta = 0x00;

                //APP_OptLOG_SaveCode(LOG_OPT_FOLLOW_STAR_END_BY_ACU);
            }
        }
        BSP_Delay_ms(50);
    }
}

/*
*********************************************************************************************************
*                                          TaskSatGradientTrackDel
*
* Description :
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : 
*
* Note(s)     : none.
*********************************************************************************************************
*/
void  TaskSatGradientTrackDel (void)
{
    OS_ERR err;
    Task_Run_Flag[1] = 0;
    OSTaskDel ((OS_TCB  *)&TaskSatGradientTrackTCB,&err);
}



/*
*********************************************************************************************************
*                                          TaskSatGradientTrackCreate()
*
* Description : Create application tasks.
*
* Argument(s) : none
*
* Return(s)   : none
*
* Caller(s)   : 
*
* Note(s)     : none.
*********************************************************************************************************
*/
void  TaskSatGradientTrackCreate(void)
{
    OS_ERR err;
    OSTaskCreate((OS_TCB     *)&TaskSatGradientTrackTCB,                /* Create the start task                                    */
                 (CPU_CHAR   *)"Task SatGradientTrack",
                 (OS_TASK_PTR ) TaskSatGradientTrack,
                 (void       *) 0,
                 (OS_PRIO     ) TASK_SatGradientTrack_PRIO,
                 (CPU_STK    *)&TaskSatGradientTrackStk[0],
                 (CPU_STK     )(TASK_SatGradientTrack_STK_SIZE / 10u),
                 (CPU_STK_SIZE) TASK_SatelliteTrack_STK_SIZE,
                 (OS_MSG_QTY  ) 0,
                 (OS_TICK     ) 0,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);     
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
#if 0
    OSTaskCreate((OS_TCB     *)&TaskPositionElecCoderTCB,                /* Create the start task                                    */
                 (CPU_CHAR   *)"Task Position",
                 (OS_TASK_PTR ) TaskPositionElecCoderRevData,
                 (void       *) 0,
                 (OS_PRIO     ) TASK_Position_ElecCoder_PRIO,
                 (CPU_STK    *)&TaskPositionElecCoderStk[0],
                 (CPU_STK     )(TASK_Position_ElecCoder_STK_SIZE / 10u),
                 (CPU_STK_SIZE) TASK_Position_ElecCoder_STK_SIZE,
                 (OS_MSG_QTY  ) 0,
                 (OS_TICK     ) 0,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

    OSTaskCreate((OS_TCB     *)&TaskCabrageElecCoderTCB,                /* Create the start task                                    */
                 (CPU_CHAR   *)"Task Cabrage",
                 (OS_TASK_PTR ) TaskCabrageElecCoderRevData,
                 (void       *) 0,
                 (OS_PRIO     ) TASK_Cabrage_ElecCoder_PRIO,
                 (CPU_STK    *)&TaskCabrageElecCoderStk[0],
                 (CPU_STK     )(TASK_Cabrage_ElecCoder_STK_SIZE / 10u),
                 (CPU_STK_SIZE) TASK_Cabrage_ElecCoder_STK_SIZE,
                 (OS_MSG_QTY  ) 0,
                 (OS_TICK     ) 0,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

    OSTaskCreate((OS_TCB     *)&TaskPolarElecCoderTCB,                /* Create the start task                                    */
                 (CPU_CHAR   *)"Task Polar",
                 (OS_TASK_PTR ) TaskPolarElecCoderRevData,
                 (void       *) 0,
                 (OS_PRIO     ) TASK_Polar_ElecCoder_PRIO,
                 (CPU_STK    *)&TaskPolarElecCoderStk[0],
                 (CPU_STK     )(TASK_Polar_ElecCoder_STK_SIZE / 10u),
                 (CPU_STK_SIZE) TASK_Polar_ElecCoder_STK_SIZE,
                 (OS_MSG_QTY  ) 0,
                 (OS_TICK     ) 0,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err); 
#endif
#if 0
        OSTaskCreate((OS_TCB     *)&TaskShellTCB,                /* Create the start task                                    */
                 (CPU_CHAR   *)"Task Shell",
                 (OS_TASK_PTR ) TaskShell,
                 (void       *) 0,
                 (OS_PRIO     ) TASK_OledCommRx_PRIO,
                 (CPU_STK    *)&TaskOledCommRxStk[0],
                 (CPU_STK     )(TASK_OledCommRx_STK_SIZE / 10u),
                 (CPU_STK_SIZE) TASK_OledCommRx_STK_SIZE,
                 (OS_MSG_QTY  ) 0,
                 (OS_TICK     ) 0,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err); 
#endif
                 
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
       OSTaskCreate((OS_TCB     *)&TaskBeaconTCB,                /* Create the start task                                    */
                 (CPU_CHAR   *)"Task Beacon",
                 (OS_TASK_PTR ) TaskBeaconRevData,
                 (void       *) 0,
                 (OS_PRIO     ) TASK_Beacon_PRIO,
                 (CPU_STK    *)&TaskBeaconStk[0],
                 (CPU_STK     )(TASK_Beacon_STK_SIZE / 10u),
                 (CPU_STK_SIZE) TASK_Beacon_STK_SIZE,
                 (OS_MSG_QTY  ) 0,
                 (OS_TICK     ) 0,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);     

    OSTaskCreate((OS_TCB     *)&TaskMonitorTCB,                /* Create the start task                                    */
                 (CPU_CHAR   *)"Task Monitor",
                 (OS_TASK_PTR ) TaskMonitor,
                 (void       *) 0,
                 (OS_PRIO     ) TASK_Monitor_PRIO,
                 (CPU_STK    *)&TaskMonitorStk[0],
                 (CPU_STK     )(TASK_Monitor_STK_SIZE / 10u),
                 (CPU_STK_SIZE) TASK_Monitor_STK_SIZE,
                 (OS_MSG_QTY  ) 0,
                 (OS_TICK     ) 0,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);                

    
#endif
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

