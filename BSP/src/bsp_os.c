/*
*********************************************************************************************************
*                                     MICRIUM BOARD SUPPORT SUPPORT
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
*                                       BOARD SUPPORT PACKAGE
*                                          uCOS-III LAYER
*
* Filename      : bsp_os.c
* Version       : V1.00
* Programmer(s) : DC
*
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#define  BSP_OS_MODULE
#include <bsp.h>
#include <bsp_os.h>


/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                           LOCAL CONSTANTS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                          LOCAL DATA TYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            LOCAL TABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/




/*
*********************************************************************************************************
*                                      LOCAL FUNCTION PROTOTYPES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                     LOCAL CONFIGURATION ERRORS
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*********************************************************************************************************
**                                         GLOBAL FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*********************************************************************************************************
**                                     BSP OS LOCKS FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*********************************************************************************************************
**                                     BSP OS LOCKS FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                      BSP_OS_SemCreate()
*
* Description : Creates a sempahore to lock/unlock
*
* Argument(s) : p_sem        Pointer to a BSP_OS_SEM structure
*
*               sem_val      Initial value of the semaphore.
*
*               p_sem_name   Pointer to the semaphore name.
*
* Return(s)   : DEF_OK       if the semaphore was created.
*               DEF_FAIL     if the sempahore could not be created.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

CPU_BOOLEAN  BSP_OS_SemCreate (BSP_OS_SEM       *p_sem,
                               BSP_OS_SEM_VAL    sem_val,
                               CPU_CHAR         *p_sem_name)
{
    OS_ERR     err;


    OSSemCreate((OS_SEM    *)p_sem,
                (CPU_CHAR  *)p_sem_name,
                (OS_SEM_CTR )sem_val,
                (OS_ERR    *)&err);

    if (err != OS_ERR_NONE) {
        return (DEF_FAIL);
    }

    return (DEF_OK);
}


/*
*********************************************************************************************************
*                                     BSP_OS_SemWait()
*
* Description : Wait on a semaphore to become available
*
* Argument(s) : sem          sempahore handler
*
*               dly_ms       delay in miliseconds to wait on the semaphore
*
* Return(s)   : DEF_OK       if the semaphore was acquire
*               DEF_FAIL     if the sempahore could not be acquire
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_BOOLEAN  BSP_OS_SemWait (BSP_OS_SEM  *p_sem,
                             CPU_INT32U   dly_ms)
{
    OS_ERR      err;
    CPU_INT32U  dly_ticks;


    dly_ticks  = ((dly_ms * DEF_TIME_NBR_mS_PER_SEC) / OSCfg_TickRate_Hz);

    OSSemPend((OS_SEM *)p_sem,
              (OS_TICK )dly_ticks,
              (OS_OPT  )OS_OPT_PEND_BLOCKING,
              (CPU_TS  )0,
              (OS_ERR *)&err);

    if (err != OS_ERR_NONE) {
       return (DEF_FAIL);
    }

    return (DEF_OK);
}

/*
*********************************************************************************************************
*                                      BSP_OS_SemPost()
*
* Description : Post a semaphore
*
* Argument(s) : sem          Semaphore handler
*
* Return(s)   : DEF_OK     if the semaphore was posted.
*               DEF_FAIL      if the sempahore could not be posted.
*
* Caller(s)   : Application.
*
* Note(s)     : none.
*********************************************************************************************************
*/

CPU_BOOLEAN  BSP_OS_SemPost (BSP_OS_SEM *p_sem)
{
    OS_ERR  err;


    OSSemPost((OS_SEM *)p_sem,
              (OS_OPT  )OS_OPT_POST_1,
              (OS_ERR *)&err);

    if (err != OS_ERR_NONE) {
        return (DEF_FAIL);
    }

    return (DEF_OK);
}


/*
*********************************************************************************************************
*********************************************************************************************************
**                                     uC/OS-III TIMER FUNCTIONS
*********************************************************************************************************
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                    BSP_TimeDlyMs()
*
* Description : This function delay the exceution for specifi amount of miliseconds
*
* Argument(s) : dly_ms       Delay in miliseconds
*
* Return(s)   : none.
*
* Caller(s)   : Application
*
* Note(s)     : none.
*
*********************************************************************************************************
*/

void   BSP_OS_TimeDlyMs (CPU_INT32U  dly_ms)
{
    CPU_INT16U  ms;
    CPU_INT16U  sec;
    OS_ERR      err;


    if (dly_ms > 10000u) {                                       /* Limit delays to 10 seconds.                        */
        dly_ms = 10000u;
    }

    if (dly_ms >= 1000u) {
        ms  = dly_ms / 1000u;
        sec = dly_ms % 1000u;
    } else {
        ms  = dly_ms;
        sec = 0u;
    }


    OSTimeDlyHMSM((CPU_INT16U) 0u,
                  (CPU_INT16U) 0u,
                  (CPU_INT16U) sec,
                  (CPU_INT32U) ms,
                  (OS_OPT    ) OS_OPT_TIME_HMSM_STRICT,
                  (OS_ERR   *)&err);
}


/***********************************************************
**name:	BSP_OS_Q_Creat
**describe: 
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int BSP_OS_Q_Creat(OS_Q *bsp_os_q,char    *p_name,INT16U   max_qty)
{
	OS_ERR     err;

	OSQCreate (bsp_os_q,p_name,max_qty,&err);
	if( err != OS_ERR_NONE)
	{
		// 告警日志


		return (-1);
	}

	return 0;
}

/***********************************************************
**name:	BSP_OS_Q_Post
**describe: 
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int BSP_OS_Q_Post(OS_Q *bsp_os_q,void *buff,	INT16U buffsize)
{
	OS_ERR     err;

	OSQPost(bsp_os_q, buff, buffsize,OS_OPT_POST_ALL, &err);
	if( err != OS_ERR_NONE)
	{
		// 告警日志


		return (-1);
	}

	return 0;
}

/***********************************************************
**name:	BSP_OS_Q_Pend
**describe: 
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int BSP_OS_Q_Block_Pend(OS_Q *bsp_os_q,void *buff,INT16U *buffsize,INT32U timeout)
{
	OS_ERR     err;
	CPU_TS       ts;
	void *pbuff;

	pbuff = OSQPend(bsp_os_q, timeout, OS_OPT_PEND_BLOCKING, buffsize, &ts,&err);
	
	if( err != OS_ERR_NONE)
	{
		// 告警日志


		return (-1);
	}
	memcpy(buff,pbuff,*buffsize);
	return 0;
}

/***********************************************************
**name:	BSP_OS_Q_Flush
**describe: 
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
void BSP_OS_Q_Flush(OS_Q *bsp_os_q)
{
	OS_ERR     err;

	OSQFlush(bsp_os_q,&err);
}


/***********************************************************
**name:	BSP_OS_Q_Del
**describe: 
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int BSP_OS_Q_Del(OS_Q *bsp_os_q)
{
	OS_ERR     err;
	CPU_TS       ts;

	OSQDel(bsp_os_q,OS_OPT_DEL_NO_PEND,&err);
	if( err != OS_ERR_NONE)
	{
		// 告警日志


		return (-1);
	}

	return 0;
}


/***********************************************************
**name:	BSP_OS_Q_Pend
**describe: 
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int BSP_OS_Q_NoBlock_Pend(OS_Q *bsp_os_q,void *buff,INT16U *buffsize)
{
	OS_ERR     err;
	CPU_TS       ts;
	void *pbuff;

	buff = OSQPend(bsp_os_q, 0, OS_OPT_PEND_NON_BLOCKING, buffsize, &ts,&err);
	memcpy(buff,pbuff,*buffsize);
	if( err != OS_ERR_NONE)
	{
		// 告警日志


		return (-1);
	}

	return 0;
}


/***********************************************************
**name:	BSP_OS_Sem_Creat
**describe: 
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int BSP_OS_Sem_Creat(OS_SEM *bsp_os_sem,char    *p_name,INT32U   init_value)
{
	OS_ERR     err;

	OSSemCreate(bsp_os_sem,p_name,(OS_SEM_CTR)init_value,&err);
	if( err != OS_ERR_NONE)
	{
		// 告警日志


		return (-1);
	}

	return 0;
}

/***********************************************************
**name:	BSP_OS_Sem_Post
**describe: 
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int BSP_OS_Sem_Post(OS_SEM *bsp_os_sem)
{
	OS_ERR     err;

	OSSemPost(bsp_os_sem,OS_OPT_POST_ALL,&err);
	if( err != OS_ERR_NONE)
	{
		// 告警日志


		return (-1);
	}

	return 0;
}

/***********************************************************
**name:	BSP_OS_Sem_Pend
**describe: 
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int BSP_OS_Sem_Pend(OS_SEM *bsp_os_sem,INT32U timeout_ms)
{
	OS_ERR     err;
	CPU_TS       ts;

	OSSemPend(bsp_os_sem,timeout_ms,OS_OPT_PEND_BLOCKING,&ts,&err);
	if( err != OS_ERR_NONE)
	{
		// 告警日志


		return (-1);
	}

	return 0;
}

/***********************************************************
**name:	BSP_OS_Sem_Del
**describe: 
**input:			
**output:	none
**return:
**autor:  andiman
**date:
************************************************************/
int BSP_OS_Sem_Del(OS_SEM *bsp_os_sem)
{
	OS_ERR     err;
	CPU_TS       ts;

	OSSemDel(bsp_os_sem,OS_OPT_DEL_ALWAYS,&err);
	if( err != OS_ERR_NONE)
	{
		// 告警日志


		return (-1);
	}

	return 0;
}



