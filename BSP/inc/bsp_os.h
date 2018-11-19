/*
*********************************************************************************************************
*                                     MICIRUM BOARD SUPPORT PACKAGE
*
*                             (c) Copyright 2012; Micrium, Inc.; Weston, FL
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
* Filename      : bsp_os.h
* Version       : V1.00
* Programmer(s) : DC
*
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                                 MODULE
*
* Note(s) : (1) This header file is protected from multiple pre-processor inclusion through use of the
*               BSP present pre-processor macro definition.
*********************************************************************************************************
*/

#ifndef  BSP_OS_PRESENT
#define  BSP_OS_PRESENT


/*
*********************************************************************************************************
*                                              INCLUDE FILES
*********************************************************************************************************
*/

#include  <os.h>


/*
*********************************************************************************************************
*                                                 EXTERNS
*********************************************************************************************************
*/

#ifdef   BSP_OS_MODULE
#define  BSP_OS_EXT
#else
#define  BSP_OS_EXT  extern
#endif


/*
*********************************************************************************************************
*                                               DEFINES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             DATA TYPES
*********************************************************************************************************
*/

typedef   OS_SEM       BSP_OS_SEM;
typedef   OS_SEM_CTR   BSP_OS_SEM_VAL;


/*
*********************************************************************************************************
*                                          GLOBAL VARIABLES
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                               MACRO'S
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                            FUNCTION PROTOTYPES
*********************************************************************************************************
*/

BSP_OS_EXT CPU_BOOLEAN  BSP_OS_SemCreate          (BSP_OS_SEM     *p_sem,
                                        BSP_OS_SEM_VAL  sem_val,
                                        CPU_CHAR       *p_sem_name);

BSP_OS_EXT CPU_BOOLEAN  BSP_OS_SemWait            (BSP_OS_SEM     *p_sem,
                                        CPU_INT32U      dly_ms);

BSP_OS_EXT CPU_BOOLEAN  BSP_OS_SemPost            (BSP_OS_SEM     *p_sem);

BSP_OS_EXT void         BSP_OS_TmrTickInit        (CPU_INT32U      tick_rate);
BSP_OS_EXT void         BSP_OS_TimeDlyMs          (CPU_INT32U      dly_ms);

BSP_OS_EXT int BSP_OS_Q_Creat(OS_Q *bsp_os_q,char    *p_name,INT16U   max_qty);

BSP_OS_EXT int BSP_OS_Q_Post(OS_Q *bsp_os_q,void *buff,	INT16U buffsize);

BSP_OS_EXT int BSP_OS_Q_Block_Pend(OS_Q *bsp_os_q,void *buff,INT16U *buffsize,INT32U timeout);

BSP_OS_EXT int BSP_OS_Q_NoBlock_Pend(OS_Q *bsp_os_q,void *buff,INT16U *buffsize);

BSP_OS_EXT int BSP_OS_Q_Del(OS_Q *bsp_os_q);

BSP_OS_EXT void BSP_OS_Q_Flush(OS_Q *bsp_os_q);

BSP_OS_EXT int BSP_OS_Sem_Creat(OS_SEM *bsp_os_sem,char    *p_name,INT32U   init_value);

BSP_OS_EXT int BSP_OS_Sem_Post(OS_SEM *bsp_os_sem);

BSP_OS_EXT int BSP_OS_Sem_Pend(OS_SEM *bsp_os_sem,INT32U timeout_ms);

BSP_OS_EXT int BSP_OS_Sem_Del(OS_SEM *bsp_os_sem);
/*
*********************************************************************************************************
*                                          CONFIGURATION ERRORS
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                             MODULE END
*********************************************************************************************************
*/

#endif                                                          /* End of module include.                               */
