/*
===============================================================================
| COPYRIGHT (C) 2003-2007 Haishen Ruan, All rights reserved.
| SUMMARY:
|   Kernel macros define.
|
|
| LICENSE INFORMATION
|   RS-RTOS is free software; you can redistribute it and/or modify it under
|   terms of the GNU General Public License as published by the
|   Free Software Foundation; either version 2, or (at your option) any
|   later version.
|
|   RS-RTOS is distributed in the hope that it will be useful,
|   but WITHOUT ANY WARRANTY; without even the implied warranty of
|   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
|   General Public License for more details.
|
|   You should have received a copy of the GNU General Public License
|   along with RS-RTOS; see file COPYING. If not, write to the Free Software
|   Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
|
|   As a special exception, including RS-RTOS header files in a file,
|   instantiating RS-RTOS generics or templates, or linking other files
|   with RS-RTOS objects to produce an executable application, does not
|   by itself cause the resulting executable application to be covered
|   by the GNU General Public License. This exception does not
|   however invalidate any other reasons why the executable file might be
|   covered by the GNU Public License.
|
|
| DESCRIPTION:
|   See http://www.RS-RTOS.org for documentation, latest information, license
|   and contact details.
|   email:ruanhaishen@gmail.com
=============================================================================*/
#ifndef __CONST_H__
#define __CONST_H__

#ifdef __cplusplus
extern "C" {
#endif
/*===========================================================================*/

/* Version of Kernel */
#define RSI_VERSION             0x0122

/* Version string of Kernel */
#define RSI_VERSION_STRING      "1.22"

/* Indicates self priority */
#define RSI_SELF_PRIO           0xff

/* Priority of idle task */
#define RSI_IDLE_PRIO           CFG_MAX_TASKS

/* The size of queue array */
#define RSI_QUEUE_SZ            ((CFG_MAX_TASKS - 1) / ARCH_CPU_BITS + 1)

/* States of task */
#define RSI_STA_NONE            0x00        /* tcb is free                   */
#define RSI_STA_READY           0x01        /* ready to run                  */
#define RSI_STA_BLOCKED         0x02        /* task blocked                  */
#define RSI_STA_SUSPEND         0x04        /* task is suspended             */
#define RSI_STA_PREREADY        0x08        /* task is suspended from ready  */
#define RSI_STA_ZOMBIE          0x10        /* tcb is deformity              */
#define RSI_STA_RESERVED        0x20        /* tcb is reserved               */
#define RSI_STA_KFREE           0x80        /* task stack needs mpool_free        */


/*== Internal macros define =============================*/

#if CFG_TASK_MODE > 0 || CFG_TASK_DELETE_EN > 0
#define RSI_DEF_TASK_LOCK       1
#else
#define RSI_DEF_TASK_LOCK       0
#endif


#define CFG_IPC_EN \
    (CFG_SEMBINARY_EN|CFG_SEMAPHORE_EN|CFG_MUTEX_EN|CFG_MAILBOX_EN|CFG_QUEUE_EN)

#define RSI_DEBUG               CFG_DEBUG

#define RSI_UINT_MAX            ((uint)-1)


#ifdef ARCH_PTR_SIZE
#define RSI_PTR_SIZE            ARCH_PTR_SIZE
#else
#define RSI_PTR_SIZE            sizeof(void __p_*)
#endif


#define UPWARDS                 0
#define DOWNWARDS               1
#define LITTLE_ENDIAN           1234
#define BIG_ENDIAN              4321

#if CFG_TASK_INFO_EN > 0
#define RSI_STACK_UNUSE         0xca
#endif

#if CFG_DEBUG > 0
#define RSI_INITED              0xa5
#endif

#if CFG_MPOOL_EN > 0
#define RSI_MM_USED             0xac
#endif


#define RSI_NA0                 '<'
#define RSI_NA1                 'N'
#define RSI_NA2                 'A'
#define RSI_NA3                 '>'


#define RSI_REG                 __reg


#define RSI_EXTERN              __extern


#define __out_                  __p_
#define __in_                   __p_
#define __inout_                __p_


#define RSI_DEF(fun)            __fun_ fun __ef_


/*== Defaullt hook function define ======================*/

#ifndef HOOK_KERNEL_ENTRY
#define HOOK_KERNEL_ENTRY()             RSI_MPT
#endif

#ifndef HOOK_KERNEL_EXIT
#define HOOK_KERNEL_EXIT()              RSI_MPT
#endif

#ifndef HOOK_TASK_CREATE
#define HOOK_TASK_CREATE(prio)          RSI_MPT
#endif

#ifndef HOOK_TASK_DELETE
#define HOOK_TASK_DELETE(prio)          RSI_MPT
#endif

#ifndef HOOK_KERNEL_IDLE
#define HOOK_KERNEL_IDLE()              RSI_MPT
#endif

#ifndef HOOK_DO_TICK
#define HOOK_DO_TICK()                  RSI_MPT
#endif

#ifndef HOOK_SCHEDULE
#define HOOK_SCHEDULE(current, next)    RSI_MPT
#endif

#ifndef HOOK_INTERRUPT
#define HOOK_INTERRUPT(vect)            RSI_MPT
#endif



#ifndef KERNEL_ENTRY_FUNCTION

void main(void);

#define KERNEL_ENTRY_FUNCTION           void main(void)
#undef  KERNEL_ENTRY_RETURN

#endif


#ifndef RSI_DISABLE_WARNING

#define RSI_DISABLE_WARNING(x)          (x) = (x)

#endif


/*== Macros check =======================================*/

#ifndef ARCH_CPU_BITS
    #error "Basetype error:Nndefine ARCH_CPU_BITS"
#endif
#if ARCH_CPU_BITS != 8 && ARCH_CPU_BITS != 16 && ARCH_CPU_BITS != 32 && ARCH_CPU_BITS != 64
    #error "Basetype error:ARCH_CPU_BITS must be 8,16,32,or 64"
#endif


#ifndef ARCH_STACK_GROW
    #error "Basetype error:Nndefine ARCH_STACK_GROW"
#endif
#if ARCH_STACK_GROW != UPWARDS && ARCH_STACK_GROW != DOWNWARDS
    #error "Basetype error:ARCH_STACK_GROW must be Up or Down"
#endif


#ifndef ARCH_BYTE_ORDER
    #error "Basetype error:Nndefine ARCH_BYTE_ORDER"
#endif
#if ARCH_BYTE_ORDER != LITTLE_ENDIAN && ARCH_BYTE_ORDER != BIG_ENDIAN
    #error "Basetype error:ARCH_BYTE_ORDER must be LITTLE_ENDIAN or BIG_ENDIAN"
#endif


#ifndef CFG_MAX_TASKS
    #error "Configure error:Nndefine CFG_MAX_TASKS"
#endif
#if CFG_MAX_TASKS < 2 || CFG_MAX_TASKS > ARCH_CPU_BITS * 8
    #error "Configure error:CFG_MAX_TASKS must from 2 to ARCH_CPU_BITS * 8 (max.256)"
#endif


#ifndef CFG_TASK_MODE
    #error "Configure error:Nndefine CFG_TASK_MODE"
#endif
#if CFG_TASK_MODE != 0 && CFG_TASK_MODE != 1 && CFG_TASK_MODE != 2
    #error "Configure error:CFG_TASK_MODE must be 0,1,or 2"
#endif


#ifndef CFG_TASK_DELETE_EN
    #error "Configure error:Nndefine CFG_TASK_DELETE_EN"
#endif
#if CFG_TASK_DELETE_EN != 0 && CFG_TASK_DELETE_EN != 1
    #error "Configure error:CFG_TASK_DELETE_EN must be 0,or 1"
#endif


#ifndef CFG_TASK_SUSPEND_EN
    #error "Configure error:Nndefine CFG_TASK_SUSPEND_EN"
#endif
#if CFG_TASK_SUSPEND_EN != 0 && CFG_TASK_SUSPEND_EN != 1
    #error "Configure error:CFG_TASK_SUSPEND_EN must be 0,or 1"
#endif


#ifndef CFG_OBJ_NAME_EN
    #error "Configure error:Nndefine CFG_OBJ_NAME_EN"
#endif


#ifndef CFG_ARG_CHK
    #error "Configure error:Nndefine CFG_ARG_CHK"
#endif
#if CFG_ARG_CHK != 0 && CFG_ARG_CHK != 1 && CFG_ARG_CHK != 2
    #error "Configure error:CFG_ARG_CHK must be 0,1,or 2"
#endif


#ifndef CFG_TICK_COUNT_EN
    #error "Configure error:Nndefine CFG_TICK_COUNT_EN"
#endif
#if CFG_TICK_COUNT_EN != 0 && CFG_TICK_COUNT_EN != 1
    #error "Configure error:CFG_TICK_COUNT_EN must be 0,or 1"
#endif


#ifndef CFG_IPC_TIMEOUT_EN
    #error "Configure error:Nndefine CFG_IPC_TIMEOUT_EN"
#endif
#if CFG_IPC_TIMEOUT_EN != 0 && CFG_IPC_TIMEOUT_EN != 1
    #error "Configure error:CFG_IPC_TIMEOUT_EN must be 0,or 1"
#endif


#ifndef CFG_SEMBINARY_EN
    #error "Configure error:Nndefine CFG_SEMBINARY_EN"
#endif
#if CFG_SEMBINARY_EN != 0 && CFG_SEMBINARY_EN != 1
    #error "Configure error:CFG_SEMBINARY_EN must be 0 or 1"
#endif


#ifndef CFG_SEMAPHORE_EN
    #error "Configure error:Nndefine CFG_SEMAPHORE_EN"
#endif
#if CFG_SEMAPHORE_EN != 0 && CFG_SEMAPHORE_EN != 1
    #error "Configure error:CFG_SEMAPHORE_EN must be 0 or 1"
#endif


#ifndef CFG_MUTEX_EN
    #error "Configure error:Nndefine CFG_MUTEX_EN"
#endif
#if CFG_MUTEX_EN != 0 && CFG_MUTEX_EN != 1 && CFG_MUTEX_EN != 2
    #error "Configure error:CFG_MUTEX_EN must be 0,1,or 2"
#endif


#ifndef CFG_MAILBOX_EN
    #error "Configure error:Nndefine CFG_MAILBOX_EN"
#endif
#if CFG_MAILBOX_EN != 0 && CFG_MAILBOX_EN != 1
    #error "Configure error:CFG_MAILBOX_EN must be 0 or 1"
#endif


#ifndef CFG_QUEUE_EN
    #error "Configure error:Nndefine CFG_QUEUE_EN"
#endif
#if CFG_QUEUE_EN != 0 && CFG_QUEUE_EN != 1
    #error "Configure error:CFG_QUEUE_EN must be 0 or 1"
#endif


#ifndef CFG_MPOOL_EN
    #error "Configure error:Nndefine CFG_MPOOL_EN"
#endif
#if CFG_MPOOL_EN != 0 && CFG_MPOOL_EN != 1
    #error "Configure error:CFG_MPOOL_EN must be 0 or 1"
#endif


#ifndef CFG_MM_BLOCKS
    #error "Configure error:Nndefine CFG_MM_BLOCKS"
#endif


#ifndef CFG_DEBUG
    #error "Configure error:Nndefine CFG_DEBUG"
#endif
#if CFG_DEBUG != 0 && CFG_DEBUG != 1
    #error "Configure error:CFG_DEBUG must be 0,or 1"
#endif


#ifndef CFG_DUMP_BUFFSZ
    #error "Configure error:Nndefine CFG_DUMP_BUFFSZ"
#endif


#ifndef CFG_SYS_INFO_EN
    #error "Configure error:Nndefine CFG_SYS_INFO_EN"
#endif
#if CFG_SYS_INFO_EN != 0 && CFG_SYS_INFO_EN != 1
    #error "Configure error:CFG_SYS_INFO_EN must be 0,or 1"
#endif


#ifndef CFG_INT_VECTSZ
    #error "Configure error:Nndefine CFG_INT_VECTSZ"
#endif


#ifndef CFG_EVENT_EN
    #error "Configure error:Nndefine CFG_EVENT_EN"
#endif
#if CFG_EVENT_EN != 0 && CFG_EVENT_EN != 1 && CFG_EVENT_EN != 2
    #error "Configure error:CFG_EVENT_EN must be 0,or 1"
#endif


#ifndef CFG_EVENT_BITS
    #error "Configure error:Nndefine CFG_EVENT_BITS"
#endif


#ifndef CFG_TIMER_EN
    #error "Configure error:Nndefine CFG_TIMER_EN"
#endif
#if CFG_TIMER_EN != 0 && CFG_TIMER_EN != 1
    #error "Configure error:CFG_TIMER_EN must be 0,or 1"
#endif


#ifndef CFG_TASK_INFO_EN
    #error "Configure error:Nndefine CFG_TASK_INFO_EN"
#endif
#if CFG_TASK_INFO_EN != 0 && CFG_TASK_INFO_EN != 1
    #error "Configure error:CFG_TASK_INFO_EN must be 0,or 1"
#endif


#ifndef CFG_DEVICE_EN
    #error "Configure error:Nndefine CFG_DEVICE_EN"
#endif
#if CFG_DEVICE_EN != 0 && CFG_DEVICE_EN != 1
    #error "Configure error:CFG_DEVICE_EN must be 0,or 1"
#endif


#if CFG_DEVICE_EN > 0
#ifndef CFG_DEVICE_SIZE
    #error "Configure error:Nndefine CFG_DEVICE_SIZE"
#endif
#if CFG_DEVICE_SIZE < 1 || CFG_DEVICE_SIZE > 32768
    #error "Configure error:CFG_DEVICE_SIZE must from 1 to 32768"
#endif
#endif


/*=======================================================*/

#if CFG_MUTEX_EN > 0 && CFG_TASK_MODE < 2
    #error "Configure error:CFG_MUTEX_EN requires CFG_TASK_MODE >= 2"
#endif


/*===========================================================================*/
#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif

