/*
===============================================================================
| COPYRIGHT (C) 2003-2007 Haishen Ruan, All rights reserved.
| SUMMARY:
|   Timer implementation.
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


#if CFG_TIMER_EN > 0

/* Timer state */
#define TMS_ENABLE      0x01
#define TMS_FREE        0x02
#define TMS_ACTIVE      0x04
#define TMS_REPEAT      0x80

/* Active timer list */
timer_t __p_* _tm_list = NULL;

/* Timer ticks */
tick_t _tm_ticks = 0;
#endif

#if CFG_TICK_COUNT_EN > 0

/* Kernel ticks counter */
volatile int32u _ticks = 0;


/*==============================================================
Function: tick_get()
Description: Get current ticks of kernel.
Parameters:
Returns:
    Current ticks
Notes:
==============================================================*/
__fun_
int32u r_tick_get(void) __ef_
{
    RSI_REG int32u val;

    IRQ_SAVEAREA;

    irq_lock();

    /* Retrieve kernel ticks. */
    val = _ticks;

    irq_unlock();

    return val;
}

/*==============================================================
Function: tick_get()
Description: Set current ticks of kernel
Parameters:
    ticks -- ticks whtich to be set
Returns:
Notes:
==============================================================*/
__fun_
void r_tick_set(int32u ticks) __ef_
{
    IRQ_SAVEAREA;

    irq_lock();

    /* Set kernel ticks. */
    _ticks = ticks;

    irq_unlock();
}
#endif

/*==============================================================
Function: __do_tick()
Description: Handle kernel tick.
Parameters:
Returns:
    Indicate whether tasks need schedule or not
Notes:
    Internal function.
==============================================================*/
__fun_
int8u __do_tick(void) __ef_
{
    RSI_REG prio_t cprio;
    RSI_REG int8u flag;

    IRQ_SAVEAREA;

    /* Do tick hook function. */
    HOOK_DO_TICK();

    /* Indicates if tasks need schedule. */
    flag = 0;

    /* Check all tasks in sleeping, determine if need wake up. */
    for (cprio = 0; cprio < RSI_IDLE_PRIO; cprio++) {

        irq_lock();

#if CFG_TASK_SUSPEND_EN > 0

        /* See if task is in sleeping. */
        if (_TASKS[cprio].delay != 0 &&
            !(_TASKS[cprio].state & RSI_STA_SUSPEND))
#else

        /* See if task is in sleeping. */
        if (_TASKS[cprio].delay != 0)
#endif
        {
            /* Delay time is not zero.
             * Decrease delay counter, then determine if it is zero. */
            if (--_TASKS[cprio].delay == 0) {

                /* Yes. task should be wake up. */
                __ready_que_insert(cprio);

                /* Need schedule. */
                flag = 1;
            }
        }

        irq_unlock();
    }

#if CFG_TIMER_EN > 0

    irq_lock();

#if CFG_TICK_COUNT_EN > 0
    /* Increase kernel tick counter. */
    _ticks++;
#endif

    /* Decrease timer tick.
     * See if it is zero. */
    if (--_tm_ticks == 0) {

        irq_unlock();

        /* Yes. Invoke timer expiration. */
        __timer_expiration();

    } else {

        irq_unlock();
    }

#else


#if CFG_TICK_COUNT_EN > 0

    irq_lock();

    /* Increase kernel tick counter. */
    _ticks++;

    irq_unlock();

#endif

#endif

    /* Return flag. */
    return flag;
}


#if CFG_TIMER_EN > 0
/*==============================================================
Function: timer_create()
Description: Create a timer.
Parameters:
    timer -- pointer to timer.
    name -- timer name.
    entry -- timer expiration service function.
    arg -- argument of expriation service function.
    ticks -- time of timer.
    options -- options.
Returns:
    RS_EOK -- succeed¡£
Notes:
==============================================================*/
#if CFG_OBJ_NAME_EN > 0
__fun_
status_t r_timer_create(
    timer_t __p_*   timer,
    name_t          name,
    entry_t         entry,
    arg_t           arg,
    tick_t          ticks,
    int8u           options
) __ef_
#else
__fun_
status_t r_timer_create(
    timer_t __p_*   timer,
    entry_t         entry,
    arg_t           arg,
    tick_t          ticks,
    int8u           options
) __ef_
#endif
{
    RSI_ASSERT(timer != NULL);
    RSI_ASSERT(entry != NULL);

#if CFG_OBJ_NAME_EN > 0
    timer->name.w = name;
#endif

    /**
     * Initialize timer control block. */

    /* If ticks is 0, set init-ticks with 1. */
    timer->init_ticks = ticks ? ticks : 1;
    timer->entry    = entry;
    timer->arg      = arg;
    timer->next     = NULL;
    timer->state    = TMS_FREE;

    /* Determine if timer needs repeat at term. */
    if (options & RS_OPT_REPEAT) {
        timer->state |= TMS_REPEAT;
    }

#if CFG_DEBUG > 0
    timer->init = RSI_INITED;
#endif

    /* Determine if timer needs enable. */
    if (!(options & RS_OPT_DISABLE)) {

        /* Yes. Enable timer. */
        timer->state |= TMS_ENABLE;
        __timer_active(timer);
    }

    return RS_EOK;
}

/*==============================================================
Function: timer_delete()
Description: Delete a timer.
Parameters:
    timer -- pointer to timer
Returns:
    RS_ELOCK -- timer is locked
    RS_EOK -- succeed
Notes:
==============================================================*/
__fun_
status_t r_timer_delete(timer_t __p_* timer) __ef_
{
    RSI_ASSERT(timer != NULL);
    RSI_ASSERT(timer->init == RSI_INITED);

    /* If timer is locked. */
    if (timer->state & TMS_FREE) {

        /* If TMS_FREE is changed in interrupt only,
         * here is safe, because interrupts nesting by order. */

        /* Disable timer. */
        timer_disable(timer);

        /* Return succeed. */
        return RS_EOK;
    }

    /* Timer is locked, return failed. */
    return RS_ELOCK;
}


/*==============================================================
Function: timer_enable()
Description: Enable a timer.
Parameters:
    timer -- pointer to timer
Returns:
    RS_EOK -- succeed
Notes:
==============================================================*/
__fun_
status_t r_timer_enable(timer_t __p_* timer) __ef_
{
    IRQ_SAVEAREA;

    RSI_ASSERT(timer != NULL);
    RSI_ASSERT(timer->init == RSI_INITED);

    irq_lock();

    /* Set timer state. */
    timer->state |= TMS_ENABLE;

    irq_unlock();

    /* Enable timer. */
    __timer_active(timer);

    return RS_EOK;
}


/*==============================================================
Function: timer_disable()
Description: Disable a timer
Parameters:
    timer -- pointer to time
Returns:
    RS_EOK -- succeed
Notes:
==============================================================*/
__fun_
status_t r_timer_disable(timer_t __p_* timer) __ef_
{
    RSI_REG timer_t __p_* __p_* pp;

    IRQ_SAVEAREA;

    RSI_ASSERT(timer != NULL);
    RSI_ASSERT(timer->init == RSI_INITED);

    /* Get the address of active timer list. */
    pp = &_tm_list;

    irq_lock();

    /* Set timer state, indicate timer is disable */
    timer->state &= ~TMS_ENABLE;

    /* If timer is active. */
    if (timer->state & TMS_ACTIVE) {

        /* Find specified timer in active list. */
        while (*pp != NULL) {

            /* If find it. */
            if (*pp == timer) {

                /* Yes. Timer is founded, break. */
                *pp = timer->next;
                break;
            }

            /* Point to next timer. */
            pp = &(*pp)->next;
        }

        /* If timer is in head of active list. */
        if (*pp == _tm_list) {

            /* Yes. Adjust timer tick. */
            _tm_ticks += timer->hand_ticks;

        /* If timer is neither in head nor in tail of active list. */
        } else if (*pp != NULL) {

            /* Adjust handle ticks of timer followed the specified timer. */
            (*pp)->hand_ticks += timer->hand_ticks;
        }

        /* Timer is not active. */
        timer->state &= ~TMS_ACTIVE;
    }

    irq_unlock();

    /* Return succeed. */
    return RS_EOK;
}


/*==============================================================
Function: timer_control()
Description: Change timer options.
Parameters:
    timer -- pointer to timer
    options -- options
Returns:
    RS_EOK -- succeed
Notes:
==============================================================*/
__fun_
status_t r_timer_control(timer_t __p_* timer, int8u options) __ef_
{
    IRQ_SAVEAREA;

    RSI_ASSERT(timer != NULL);
    RSI_ASSERT(timer->init == RSI_INITED);


    /* If change timer into repeat-timer. */
    if (options & RS_OPT_REPEAT) {

        irq_lock();

        /* Set timer state. */
        timer->state |= TMS_REPEAT;

        irq_unlock();

    /* If change timer into once-timer. */
    } else if (options & RS_OPT_SINGLE) {

        irq_lock();

        /* Set timer state. */
        timer->state &= ~TMS_REPEAT;

        irq_unlock();
    }

    return RS_EOK;
}


/*==============================================================
Function: __timer_active()
Description: Active timer.
Parameters:
    timer -- pointer to timer
Returns:
Notes:
    Internal function
==============================================================*/
__fun_
void __timer_active(timer_t __p_* timer) __ef_
{
    RSI_REG timer_t __p_* __p_* pp;
    RSI_REG timer_t __p_* p;
    RSI_REG tick_t ticks;

    IRQ_SAVEAREA;

    /* Get initialized ticks. */
    ticks = timer->init_ticks;

    /* Get the address of active timer list. */
    pp = &_tm_list;

    irq_lock();

    /* See if timer is enable. */
    if ((timer->state & (TMS_ENABLE|TMS_ACTIVE))
        == TMS_ENABLE)
    {
        /* Yes. Add timer to the active list. */

        /* Retrieve pointer of active timer. */
        p = _tm_list;

        /* If active list is empty. */
        if (p != NULL) {

            /* No. Retrieve timer ticks. */
            p->hand_ticks = _tm_ticks;

            /* Fine the position that timer insert to */
            while (p != NULL && ticks >= p->hand_ticks) {

                /* Adjust handle ticks. */
                ticks -= p->hand_ticks;

                /* Point to next timer. */
                pp = &p->next;
                p = *pp;
            }
        }

        /* Point to timer. */
        *pp = timer;

        /* Adjust handle ticks. */
        timer->hand_ticks = ticks;

        /* Active timer. */
        timer->state |= TMS_ACTIVE;

        /* Link next timer. */
        timer->next = p;

        /* If timer is not in tial of active list. */
        if (p != NULL) {

            /* Adjust handle ticks of next timer. */
            p->hand_ticks -= ticks;
        }

        /* Adjust timer ticks counter. */
        _tm_ticks = _tm_list->hand_ticks;
    }

    irq_unlock();
}


/*==============================================================
Function: __timer_expiration()
Description: Responsible for processing all timer expirations
Parameters:
Returns:
Notes:
    Internal function
==============================================================*/
__fun_
void __timer_expiration(void) __ef_
{
    RSI_REG timer_t __p_* p;

    IRQ_SAVEAREA;

do_loop:
    irq_lock();

    /* Active list. */
    p = _tm_list;

    /* If active list is empty. */
    if (p != NULL && _tm_ticks == 0) {

        /* Timer has expired. */

        /* Set timer to inactive state. */
        p->state &= ~(TMS_ACTIVE|TMS_FREE);

        /* Point to next timer. */
        _tm_list = p->next;

        /* If active list is empty. */
        if (_tm_list != NULL) {

            /* Yes.
             * Adjust timer ticks counter. */
            _tm_ticks = _tm_list->hand_ticks;
        }

        irq_unlock();

        /* If timer need repeat. */
        if (p->state & TMS_REPEAT) {

            /* Active timer. */
            __timer_active(p);
        }

        RSI_ASSERT(p->entry != NULL);

        /* Invoke timer expiration function specified by application. */
        (*p->entry)(p->arg);


        irq_lock();

        /* Indicate that timer is free. */
        p->state |= TMS_FREE;

        irq_unlock();

        /* Check if another timer has expired. */
        goto do_loop;
    }

    irq_unlock();
}

#endif
/*===========================================================================*/


