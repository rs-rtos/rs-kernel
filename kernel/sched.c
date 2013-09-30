/*
===============================================================================
| COPYRIGHT (C) 2003-2007 Haishen Ruan, All rights reserved.
| SUMMARY:
|   Scheduler implementation.
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
/*===========================================================================*/
#include "arch/arch.h"
#include "inc/const.h"
#include "inc/cque.h"
#include "inc/kernel.h"
#include "inc/memory.h"
#include "inc/ipc.h"
#include "inc/device.h"
#include "inc/kapi.h"
#include "driver/driver.h"


/* The queue of tasks ready for execution. */
cque_t  _ready_queue;

/* The priority of current executing task. */
prio_t  _current_prio;

/* The lock for kernel schedule. */
volatile uint _sched_lock;


#if CFG_SYS_INFO_EN > 0
/* The CPU used percentage information. */
volatile int8s _cpu_usage;

#endif


/* Bitmap table which indicates the highest priority. */
prio_t const __const_ _task_map[] =
{
     0xff, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        7, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        6, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        5, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0,
        4, 0, 1, 0, 2, 0, 1, 0, 3, 0, 1, 0, 2, 0, 1, 0
};


/*==============================================================
Function: r_kernel_lock()
Description: Disables task context switching.
Parameters:
Returns:
Notes:
    r_kernel_lock and r_kernel_unlock can not be used separately.
==============================================================*/
__fun_
void r_kernel_lock(void) __ef_
{
    IRQ_SAVEAREA;

    irq_lock();

    /* Whether the _sched_lock exceed the limitation. */
    if (_sched_lock < RSI_UINT_MAX) {

        /* If within the limitation, increase the number of kernel lock. */
        _sched_lock++;
    }
    irq_unlock();
}


/*==============================================================
Function: r_kernel_unlock()
Description: Enables task context switching.
Parameters:
Returns:
Notes:
    r_kernel_unlock and r_kernel_lock can not be used separately.
==============================================================*/
__fun_
void r_kernel_unlock(void) __ef_
{
    IRQ_SAVEAREA;

    irq_lock();

    /* Whether shortage the limitation. */
    if (_sched_lock > 0x00) {

        /* If within the limitaiton, decrease the number of kernel lock. */
        _sched_lock--;
    }
    irq_unlock();

    __schedule();
}


/*==============================================================
Function: r_kernel_islock()
Description: determining enable/disable task context switching.
Parameters:
Returns:
    0 -- indicates enable task context switching or
    n -- indicates the disabled times.
==============================================================*/
__fun_
uint r_kernel_islock(void) __ef_
{
    /* Return the disabled times. */
    return _sched_lock;
}


#if CFG_SYS_INFO_EN > 0
/*==============================================================
Function: r_kernel_info()
Description: Capture the kernel information.
Parameters:
    info -- Pointer to information srtructure
Returns:
    RS_EOK -- Succeed
Notes:
==============================================================*/
__fun_
status_t r_kernel_info(kinfo_t __p_* info) __ef_
{
    RSI_REG prio_t prio;
    RSI_REG int8u task_no;
    
    /* Calculate task No. in system. */
    prio = CFG_MAX_TASKS - 1;
    task_no = 0;
    
    do {
        /* If task is exist. */
        if (_TASKS[prio].state & RSI_STA_READY) {

            /* Task is ready. */
            task_no++;
        }
    } while (prio--);

    /* Save task No. */
    info->task_no = task_no;

    
    /* Get the CPU used percentage information. */
    info->cpu_usage = _cpu_usage;

    /* Return succeed. */
    return RS_EOK;
}
#endif


/*==============================================================
Function: __kernel_init()
Description: Entry function of the system. This function initialize
             all of kernel components. After system initialized,
             the application_initialize routine is invoked.
Parameters:
Returns:
Notes:
    Internal function.
==============================================================*/
__fun_
void __kernel_init(void) __ef_
{
    /* Hook function of kernel entry routine. */
    HOOK_KERNEL_ENTRY();

    /* To prevent from multi-task scheduling. */
    _sched_lock = 1;

    /* Initialize environment base on hardware */
    hardware_initialize();

    /* Initialize the tasks queue which is ready for execution. */
    __ready_que_init();

#if CFG_INT_VECTSZ > 0
    /* Initialize interrupt vects table. */
    __interrupt_init();
#endif

    /* Initialize task management component. */
    __task_init();

#if CFG_MPOOL_EN > 0
    /* Initialize memory management component. */
    __mm_init();
#endif

#if CFG_DEVICE_EN > 0
    /* Initialize driver management component. */
    __driver_init();
#endif

    /* Invoke application initialization function. */
    application_initialize();

    /* Hook function of kernel exit. */
    HOOK_KERNEL_EXIT();
}



/*==============================================================
Function: KERNEL_ENTRY_FUNCTION()
Description: Entry function of the system. 
Parameters:
Returns:
Notes:
    KERNEL_ENTRY_FUNCTION can be redefined by the application.
==============================================================*/
KERNEL_ENTRY_FUNCTION
{
#if CFG_SYS_INFO_EN > 0

    RSI_REG int32u idle_cnt = 0;
    RSI_REG int32u idle_max = 0;
    RSI_REG int8u  tm = 0;

    IRQ_SAVEAREA;

    /* Disable the interrupt. */
    irq_disable();

    /* Initialize kernel module. */
    __kernel_init();
    
    /* Enable interrupts. */
    irq_enable();
    
    /* Synchronizate task step with tick counter. */
    while ( ((int8u)_ticks & 0x01));
    while (!((int8u)_ticks & 0x01));

    irq_lock();

    /* Clear tick counter. */
    _ticks = 0;

    irq_unlock();

#else

    /* Disable the interrupt. */
    irq_disable();

    /* Initialize kernel module. */
    __kernel_init();

    /* Unlock scheduler. */
    _sched_lock = 0;
    
    /* Enable interrupts. */
    irq_enable();

#endif

    /* System idle loop routine. */
    for (;;) {

        /* Hook function of idle task. */
        HOOK_KERNEL_IDLE();

#if CFG_SYS_INFO_EN > 0

        /* Increase counter for system idle. */
        idle_cnt++;

        /* Calculate CPU usage every 128 ticks. */
        if ((int8u)_ticks & 0x80 ^ tm) {

            /* Next time for calculate. */
            tm ^= 0x80;

            /* Determine if counter of maximal idle tick is calculated. */
            if ((int8u)idle_max) {

                RSI_REG int8s usage;

                /* Calculate CPU used percentage. */
                usage = 100 - (int8u)(idle_cnt * 100L / idle_max);

                /* Adjust resulte. */
                if (usage < 0)
                    usage = 0;

                /* Saves result of CPU usage */
                _cpu_usage = usage;

                /* Clear idle counter. */
                idle_cnt = 0;

            } else {

                /* Save maximal counter for system idle. */
                idle_max = idle_cnt | 1;

                /* Unlock scheduler. */
                _sched_lock = 0;
            }
        }
#endif
    }
}


/*==============================================================
Function: __schedule()
Description: Gain the highest priority of the ready queue.
             Determine if the scheduling needed.
Parameters:
Returns:
Notes:
    Internal function.
==============================================================*/
__fun_
void __schedule(void) __ef_
{
    RSI_REG prio_t current;
    RSI_REG prio_t next;

    IRQ_SAVEAREA;

    /* Disables all interrupts. */
    irq_lock();

    /* Can shcedule tasks? */
    if (_sched_lock == 0) {
        /* Yes. */
        /* Get the priority of current execution task. */
        current = _current_prio;

        /* Retrieve next task. */
        __ready_que_head(next);

        /* Judge if the current task is the highest priority one. */
        if (next != current) {
            /* Task switchover. */

            HOOK_SCHEDULE(current, next);

            /* Switch to another task. */
            _current_prio = next;

            /* Switch to the next task context. */
            __switch_to(&_TASKS[current].stack, _TASKS[next].stack);
        }
    }
    irq_unlock();
}


#if CFG_IPC_EN > 0
/*==============================================================
Function: __ipc_block()
Description: Add the current task into the blocked queue.
Parameters:
Returns:
Notes:
    Internal function.
==============================================================*/
__fun_
void __ipc_block(cque_t __p_* q, tick_t ticks) __ef_
{
    RSI_REG prio_t current;

    /* The priority of the current task.  */
    current = _current_prio;

    /* Save the blocked queue pointer.  */
    _TASKS[current].pend_q = q;

    /* Setup blocking time.  */
    _TASKS[current].delay  = ticks;

    /* Setup blocking flag. */
    _TASKS[current].state |= RSI_STA_BLOCKED;

    /* Remove the task from the ready queue. */
    __ready_que_remove(current);
    /* Add the task to the blocked queue. */
    __cque_insert((*q), current);
}

/*==============================================================
Function: __ipc_block()
Description: Wake up the highest priority task in blocked queue.
Parameters:
Returns:
Notes:
    Internal function.
==============================================================*/
__fun_
prio_t __ipc_resume(cque_t __p_* q) __ef_
{
    RSI_REG prio_t next;

    /* If the queue is empty. */
    if (__cque_is0((*q))) {
        /* Yes. Return to caller. */
        return RSI_NULL_PRIO;
    }

    /* Get the next task from the queue. */
    __cque_take((*q), next);

    /* Setup the blocked queue pointer as NULL. */
    _TASKS[next].pend_q = NULL;

    /* Erase the blocking flag. */
    _TASKS[next].state &= ~RSI_STA_BLOCKED;

#if CFG_TASK_SUSPEND_EN > 0

    /* If the task suspended. */
    if (!(_TASKS[next].state & RSI_STA_SUSPEND)) {

        /* No. Insert the task to ready queue. */
        __ready_que_insert(next);

    } else {

        /* Yes. Setup the task state with RSI_STA_PREREADY.
           This procedure is wake up the suspended task from
           the blocked queue. */
        _TASKS[next].state |= RSI_STA_PREREADY;
    }
#else
    /* Insert the task the ready queue. */
    __ready_que_insert(next);
#endif

    /* Return to the tasks priority. */
    return next;
}

#if CFG_IPC_TIMEOUT_EN > 0
/*==============================================================
Function: __ipc_timeout()
Description: The task is wake up for timed out.
Parameters:
Returns:
Notes:
    Internal function.
==============================================================*/
__fun_
void __ipc_timeout(cque_t __p_* q) __ef_
{
    RSI_REG prio_t current;

    /* current running-task priority. */
    current = _current_prio;

    /* Erase current task from the blcoked queue. */
    __cque_remove((*q), current);

    /* Erase the current task state mark. */
    _TASKS[current].state &= ~RSI_STA_BLOCKED;

    /* Erase the blocked queue pointer. */
    _TASKS[current].pend_q = NULL;
}
#endif
#endif


#if CFG_TASK_MODE > 1
/*==============================================================
Function: __prio_isnormal()
Description: Determine if the inputed priority is normal priority or not.
Parameters:
    prio -- Task priority.
Returns:
    true -- indicates the priority is normal priority.
    false --indicates the priority is abnormal priority.
Notes:
    Internal function.
==============================================================*/
__fun_
bool __prio_isnormal(prio_t prio) __ef_
{
    /* See if task is ready for execution. */
    if (_TASKS[prio].state & RSI_STA_READY) {

        /* See if the priority is normal priority. */
        if (prio == _TASKS[prio].prio.normal) {
            return true;
        }

    /* See if task priority is changed. */
    } else if (_TASKS[prio].state & RSI_STA_RESERVED) {
        return true;
    }

    /* The priority is abnormal priority. retrun false. */
    return false;
}
#endif

#ifndef RSI_NOP
/*==============================================================
Function: __dummy()
Description: dummy function.
Parameters:
Returns:
Notes:
    Internal function.
==============================================================*/
__fun_
void __dummy(void) __ef_
{
    RSI_MPT;
}
#endif

/*===========================================================================*/

