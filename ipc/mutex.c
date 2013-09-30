/*
===============================================================================
| COPYRIGHT (C) 2003-2007 Haishen Ruan, All rights reserved.
| SUMMARY:
|   Mutex implementation.
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


#if CFG_MUTEX_EN > 0

/*==============================================================
Function: mutex_create()
Description: Initialize a mutex
Parameters:
    mut -- pointer to mutex
    name -- mutex's name
    prio -- mutex priority ceiling
Returns:
    RS_EOUTRANGE -- priority out of range
    RS_EEXIST -- priority is used by other
    RS_EOK -- succeed
Notes:
==============================================================*/
#if CFG_OBJ_NAME_EN > 0

__fun_
status_t r_mutex_create(mutex_t __p_* mut, name_t name, prio_t prio) __ef_

#else

__fun_
status_t r_mutex_create(mutex_t __p_* mut, prio_t prio) __ef_

#endif
{
    IRQ_SAVEAREA;

    RSI_ASSERT(mut != NULL);

#if CFG_ARG_CHK > 1

    /* Determines whether priority out of range */
    if (prio >= RSI_IDLE_PRIO) {

        /* Yes. Priority out of range, return failed. */
        return RS_EOUTRANGE;
    }
#else
    RSI_ASSERT(prio < RSI_IDLE_PRIO);
#endif

    irq_lock();

    /* See if the specified priority is used. */
    if (_TASKS[prio].state != RSI_STA_NONE) {

        irq_unlock();

        /* Priority is used by other, return failed. */
        return RS_EEXIST;
    }

    /* Reserve the priority. */
    _TASKS[prio].state = RSI_STA_ZOMBIE;

    irq_unlock();

#if CFG_OBJ_NAME_EN > 0

    /* Save mutex name. */
    mut->name.w = name;
#endif

    /* Initialize mutex control block. */
    mut->ceiling = prio;
    mut->owner   = RSI_NULL_PRIO;

#if CFG_MUTEX_NEST_EN > 0
    mut->reference = 0;
#endif

    /* Clear wait queue. */
    __cque_init(mut->waits);

#if CFG_DEBUG > 0

    /* Mutex initialized flag. */
    mut->init = RSI_INITED;
#endif

    /* Return succeed. */
    return RS_EOK;
}


#if CFG_MUTEX_EN == 1
/*==============================================================
Function: mutex_wait()
Description: Obtain a mutex.(Priority Inheritance Pattern)
Parameters:
    mut -- pointer to mutex
    ticks -- ticks to wait
Returns:
    RS_ELOCK -- kernel is locked
    RS_EAGAIN -- obtain the same mutex again
    RS_ETIMEOUT -- time out
    RS_EOK -- succeed
Notes:
    Avoid calling this function from interrupt routine.
==============================================================*/
#if CFG_IPC_TIMEOUT_EN > 0 || CFG_DEBUG > 0

__fun_
status_t r_mutex_wait(mutex_t __p_* mut, tick_t ticks) __ef_

#else

__fun_
status_t r_mutex_wait(mutex_t __p_* mut) __ef_

#endif
{
    RSI_REG prio_t current;
    RSI_REG prio_t ceiling;
    RSI_REG prio_t owner;

    IRQ_SAVEAREA;

    RSI_ASSERT(mut != NULL);
    RSI_ASSERT(mut->init == RSI_INITED);

#if CFG_ARG_CHK > 0

    /* Determine if kernel is locked. */
    if (_sched_lock > 0) {

        /* Yes. Return failed. */
        return RS_ELOCK;
    }
#else
    RSI_ASSERT(_sched_lock == 0);
#endif

    /* Get mutex ceiling priority. */
    ceiling = mut->ceiling;

    for (;;) {

        irq_lock();

        /* Get priority of current task. */
        current = _current_prio;

        /* Get priority of owner task. */
        owner = mut->owner;

        /* See if mutex is available. */
        if (owner == RSI_NULL_PRIO) {

            /* Yes. Save priority of current task. */
            mut->owner = _TASKS[current].prio.normal;

            /* Lock current task. */
            __nprio_lock(current);

            irq_unlock();

            /* Return succeed. */
            return RS_EOK;
        }

        /* Mutex is not available. */

        /* Determine whether current task higher than owner task. */
        if (current < owner &&
            !(_TASKS[owner].state & RSI_STA_RESERVED))
        {
            /* Change owner task priority to ceiling priority. */
            _TASKS[ceiling] = _TASKS[owner];

            /* Reserve owner task control block. */
            _TASKS[owner].delay = 0;
            _TASKS[owner].state = (RSI_STA_ZOMBIE|RSI_STA_RESERVED);

            /* Adjust running priority of owner task. */
            _TASKS[owner].prio.current = ceiling;

            /* Determine if pointer to blocked queue is empty. */
            if (_TASKS[ceiling].pend_q == NULL) {

                /* Adjust task priority in ready queue. */
                __ready_que_move(owner, ceiling);

            } else {

                RSI_REG cque_t __p_* rp;

                /* Pointer to blocked queue. */
                rp = _TASKS[ceiling].pend_q;

                /* Adjust task priority in blocked queue. */
                __cque_remove((*rp), owner);
                __cque_insert((*rp), ceiling);
            }
#if CFG_MUTEX_NEST_EN > 0

        /* Determine if priority of current task is same as owner task. */
        } else if (owner == _TASKS[current].prio.normal) {

            /* Increase mutex reference counter. */
            mut->reference++;

            irq_unlock();

            /* Return succeed. */
            return RS_EOK;

#elif CFG_ARG_CHK > 0

        /* prevent a task waiting the same mutex again. */
        } else if (owner == _TASKS[current].prio.normal) {

            irq_unlock();

            /* Return failed. */
            return RS_EAGAIN;
#endif
        }


#if CFG_IPC_TIMEOUT_EN > 0

        /* Mutex is not available, block current task. */
        __ipc_block(&mut->waits, ticks);
#else

        /* Mutex is not available, block current task. */
        __ipc_block(&mut->waits, 0);
#endif

        irq_unlock();

        /* Schedule tasks. */
        __schedule();

#if CFG_IPC_TIMEOUT_EN > 0

        irq_lock();

        /* Get priority of current executing task. */
        current = _current_prio;

        /* Determine if task is waked up because time out. */
        if (_TASKS[current].state & RSI_STA_BLOCKED) {

            /* Yes. */
            __ipc_timeout(&mut->waits);

            irq_unlock();

            /* Task has time out, Return. */
            return RS_ETIMEOUT;
        }

        /* Task is waked up because mutex available.
         * Adjust task delay counter. */
        __adjust_delay(current, ticks);

        irq_unlock();

#elif CFG_DEBUG > 0
        if (ticks != 0) {
            RSI_TRACE("!! Warning: CFG_IPC_TIMEOUT_EN == 0, ticks is ignored");
        }
#endif
    }
}


/*==============================================================
Function: mutex_trywait()
Description: Obtain a mutex dispense with wait.
             (Priority Inheritance Pattern)
Parameters:
    mut -- pointer to mutex
Returns:
    RS_ELOCK -- kernel is locked
    RS_ENAVAIL -- mutex is not available
    RS_EOK -- succeed
Notes:
    Avoid calling this function from interrupt routine.
==============================================================*/
__fun_
status_t r_mutex_trywait(mutex_t __p_* mut) __ef_
{
    RSI_REG prio_t current;

    IRQ_SAVEAREA;

    RSI_ASSERT(mut != NULL);
    RSI_ASSERT(mut->init == RSI_INITED);

#if CFG_ARG_CHK > 0

    /* Determine if kernel is locked. */
    if (_sched_lock > 0) {

        /* Yes. Return failed. */
        return RS_ELOCK;
    }
#else
    RSI_ASSERT(_sched_lock == 0);
#endif

    irq_lock();

    /* Get priority of current task. */
    current = _current_prio;

    /* See if mutex is available. */
    if (mut->owner == RSI_NULL_PRIO) {

        /* Yes. Save priority of current task. */
        mut->owner = _TASKS[current].prio.normal;

        /* Lock current task. */
        __nprio_lock(current);

        irq_unlock();

        /* Return succeed. */
        return RS_EOK;

#if CFG_MUTEX_NEST_EN > 0

    /* Determine if priority of current task is same as owner task. */
    } else if (mut->owner == _TASKS[current].prio.normal) {

        /* Increase mutex reference counter. */
        mut->reference++;

        irq_unlock();

        /* Return succeed. */
        return RS_EOK;
#endif
    }

    irq_unlock();

    /* Mutex is not available. Return failed. */
    return RS_ENAVAIL;
}


#else
/*==============================================================
Function: mutex_wait()
Description: Obtain a mutex. (Priority Ceiling Pattern)
Parameters:
    mut -- pointer to mutex
    ticks -- ticks to wait
Returns:
    RS_ELOCK -- kernel is locked
    RS_EAGAIN -- obtain the same mutex again
    RS_ETIMEOUT -- time out
    RS_EOK -- succeed
Notes:
    Avoid calling this function from interrupt routine.
==============================================================*/
#if CFG_IPC_TIMEOUT_EN > 0 || CFG_DEBUG > 0

__fun_
status_t r_mutex_wait(mutex_t __p_* mut, tick_t ticks) __ef_

#else

__fun_
status_t r_mutex_wait(mutex_t __p_* mut) __ef_

#endif
{
    RSI_REG prio_t current;
    RSI_REG prio_t ceiling;

    IRQ_SAVEAREA;

    RSI_ASSERT(mut != NULL);
    RSI_ASSERT(mut->init == RSI_INITED);

#if CFG_ARG_CHK > 0

    /* Determine if kernel is locked. */
    if (_sched_lock > 0) {

        /* Yes. Return failed. */
        return RS_ELOCK;
    }
#else
    RSI_ASSERT(_sched_lock == 0);
#endif

    /* Get mutex ceiling priority. */
    ceiling = mut->ceiling;

    for (;;) {

        irq_lock();

        /* Get priority of current task. */
        current = _current_prio;

        /* Whether mutex is available. */
        if (mut->owner == RSI_NULL_PRIO) {

            /* Yes. Save priority of current task. */
            mut->owner = _TASKS[current].prio.normal;

            /* Lock current task. */
            __nprio_lock(current);

            /* Determine if current task priority is same as owner priority. */
            if (current == mut->owner) {

                /* Yes.
                 * Change current task priority to ceiling priority. */
                _TASKS[ceiling] = _TASKS[current];

                /* Reserve current priority. */
                _TASKS[current].state = (RSI_STA_ZOMBIE|RSI_STA_RESERVED);

                /* Adjust running priority to ceiling priority. */
                _TASKS[current].prio.current = ceiling;

                /* Adjust task priority in ready queue. */
                __ready_que_remove(current);
                __ready_que_insert(ceiling);

                /* Adjust priority of current executing task. */
                _current_prio = ceiling;
            }

            irq_unlock();

            /* Return succeed. */
            return RS_EOK;

#if CFG_MUTEX_NEST_EN > 0

        /* Determine if priority of current task is same as owner task. */
        } else if (mut->owner == _TASKS[current].prio.normal) {

            /* Increase mutex reference counter. */
            mut->reference++;

            irq_unlock();

            /* Return succeed. */
            return RS_EOK;

#elif CFG_ARG_CHK > 0

        /* prevent a task waiting the same mutex again. */
        } else if (mut->owner == _TASKS[current].prio.normal) {

            irq_unlock();

            /* Return failed. */
            return RS_EAGAIN;
#endif
        }


#if CFG_IPC_TIMEOUT_EN > 0

        /* Mutex is not available, block current task. */
        __ipc_block(&mut->waits, ticks);
#else

        /* Mutex is not available, block current task. */
        __ipc_block(&mut->waits, 0);
#endif
        irq_unlock();

        /* Schedule tasks. */
        __schedule();

#if CFG_IPC_TIMEOUT_EN > 0
        irq_lock();

        /* Get priority of current executing task. */
        current = _current_prio;

        /* Determine if task is waked up because time out. */
        if (_TASKS[current].state & RSI_STA_BLOCKED) {

            /* Yes. */
            __ipc_timeout(&mut->waits);

            irq_unlock();

            /* Task has time out, Return. */
            return RS_ETIMEOUT;
        }

        /* Task is waked up because mutex available.
         * Adjust task delay counter. */
        __adjust_delay(current, ticks);

        irq_unlock();

#elif CFG_DEBUG > 0
        if (ticks != 0) {
            RSI_TRACE("!! Warning: CFG_IPC_TIMEOUT_EN == 0, ticks is ignored");
        }
#endif
    }
}


/*==============================================================
Function: mutex_trywait()
Description: Obtain a mutex dispense with wait.
             (Priority Ceiling Pattern)
Parameters:
    mut -- pointer to mutex
    ticks -- ticks to wait
Returns:
    RS_ELOCK -- kernel is locked
    RS_ENAVAIL -- mutex is not available
    RS_EOK -- succeed
Notes:
    Avoid calling this function from interrupt routine.
==============================================================*/
__fun_
status_t r_mutex_trywait(mutex_t __p_* mut) __ef_
{
    RSI_REG prio_t current;
    RSI_REG prio_t ceiling;

    IRQ_SAVEAREA;

    RSI_ASSERT(mut != NULL);
    RSI_ASSERT(mut->init == RSI_INITED);

#if CFG_ARG_CHK > 0

    /* Determine if kernel is locked. */
    if (_sched_lock > 0) {

        /* Yes. Return failed. */
        return RS_ELOCK;
    }
#else
    RSI_ASSERT(_sched_lock == 0);
#endif

    /* Get mutex ceiling priority. */
    ceiling = mut->ceiling;

    irq_lock();

    /* Get priority of current task. */
    current = _current_prio;

    /* Whether mutex is available. */
    if (mut->owner == RSI_NULL_PRIO) {

        /* Yes. Save priority of current task. */
        mut->owner = _TASKS[current].prio.normal;

        /* Lock current task. */
        __nprio_lock(current);

        /* Determine if current task priority is same as owner priority. */
        if (current == mut->owner) {

            /* Yes.
             * Change current task priority to ceiling priority. */
            _TASKS[ceiling] = _TASKS[current];

            /* Reserve current priority. */
            _TASKS[current].state = (RSI_STA_ZOMBIE|RSI_STA_RESERVED);

            /* Adjust running priority to ceiling priority. */
            _TASKS[current].prio.current = ceiling;

            /* Adjust task priority in ready queue. */
            __ready_que_remove(current);
            __ready_que_insert(ceiling);

            /* Adjust priority of current executing task. */
            _current_prio = ceiling;
        }

        irq_unlock();

        /* Return succeed. */
        return RS_EOK;

#if CFG_MUTEX_NEST_EN > 0

    /* Determine if priority of current task is same as owner task. */
    } else if (mut->owner == _TASKS[current].prio.normal) {

        /* Increase mutex reference counter. */
        mut->reference++;

        irq_unlock();

        /* Return succeed. */
        return RS_EOK;
#endif
    }
    irq_unlock();

    /* Mutex is not available, Return failed. */
    return RS_ENAVAIL;
}
#endif


/*==============================================================
Function: mutex_release()
Description: Release a mutex
Parameters:
    mut -- pointer to mutex
Returns:
    RS_ELOCK -- kernel is locked
    RS_EINVAL -- attempt to release a mutex which is not obtained
              by himself
    RS_EOK -- succeed
Notes:
    Avoid calling this function from interrupt routine.
==============================================================*/
__fun_
status_t r_mutex_release(mutex_t __p_* mut) __ef_
{
    RSI_REG prio_t current;
    RSI_REG prio_t owner;

    IRQ_SAVEAREA;

    RSI_ASSERT(mut != NULL);
    RSI_ASSERT(mut->init == RSI_INITED);

#if CFG_ARG_CHK > 0

    /* Determine if kernel is locked. */
    if (_sched_lock > 0) {

        /* Yes. Return failed. */
        return RS_ELOCK;
    }
#else
    RSI_ASSERT(_sched_lock == 0);
#endif

    irq_lock();

    /* Get priority of current executing task. */
    current = _current_prio;

    /* Get priority of owner task. */
    owner = mut->owner;

    /* Unlock current task. */
    __nprio_unlock(current);

#if CFG_MUTEX_NEST_EN > 0
#if CFG_ARG_CHK > 0

    /* Determine if priority of current task is same as owner task. */
    if (owner != _TASKS[current].prio.normal) {

        /* Not the same.
         * Lock current task we have been unlock it above. */
        __nprio_lock(current);

        irq_unlock();

        /* Return failed. */
        return RS_EINVAL;

    } else
#endif

    /* If mutex reference counter is zero. */
    if (mut->reference > 0) {

        /* No. Decrease reference counter. */
        mut->reference--;

        /* Return succeed. */
        return RS_EOK;

    } else
#endif

    /* Determine if current task priority is same as ceiling priority. */
    if (current == mut->ceiling) {

        /* Yes.
         * Reset task priority. */
        _TASKS[owner] = _TASKS[current];

        /* Reserve ceiling TCB. */
        _TASKS[current].state = RSI_STA_ZOMBIE;

        /* Adjust task priority in ready queue. */
        __ready_que_remove(current);
        __ready_que_insert(owner);

        /* The priority of current executing task has been changed.
         * Adjust current executing priority. */
        _current_prio = owner;

#if CFG_MUTEX_NEST_EN == 0 && CFG_ARG_CHK > 0

    /* Determine if priority of current task is same as owner task. */
    } else if (owner != _TASKS[current].prio.normal) {

        /* We have been unlock current task above.
         * Lock it again. */
        __nprio_lock(current);

        irq_unlock();

        /* Return failed. */
        return RS_EINVAL;
#endif
    }

    /* Release mutex. */
    mut->owner = RSI_NULL_PRIO;

    /* Resume the task which hold the highest priority in waiting queue. */
    __ipc_resume(&mut->waits);

    irq_unlock();

    /* Tasks need schedule. */
    __schedule();

    /* Return succeed. */
    return RS_EOK;
}


/*==============================================================
Function: mutex_getowner()
Description: Get mutex infomation.
Parameters:
    mut -- pointer to mutex
    info -- pointer to mutex information struct
Returns:
    RS_EOK -- succeed
Notes:
==============================================================*/
__fun_
status_t r_mutex_info(mutex_t __p_* mut, mutinfo_t __out_* info) __ef_
{
    IRQ_SAVEAREA;

    RSI_ASSERT(mut != NULL);
    RSI_ASSERT(mut->init == RSI_INITED);

#if CFG_OBJ_NAME_EN > 0

    /* Get mutex name. */
    info->name[0] = mut->name.c[0];
    info->name[1] = mut->name.c[1];
    info->name[2] = mut->name.c[2];
    info->name[3] = mut->name.c[3];

#else
    info->name[0] = 0;
    
#endif

    /* Determine if mutex has a name. */
    if (info->name[0] == 0) {
        info->name[0] = RSI_NA0;
        info->name[1] = RSI_NA1;
        info->name[2] = RSI_NA2;
        info->name[3] = RSI_NA3;
    }

    info->name[4] = 0;

    /* Return priority of mutex. */
    info->prio = mut->ceiling;

    irq_lock();
    
    /* Return owner priority. */
    info->owner = mut->owner;

#if CFG_MUTEX_NEST_EN > 0

    /* Return reference of mutex. */
    info->reference = mut->reference;
    
#endif

    irq_unlock();
    
    /* Return succeed. */
    return RS_EOK;
}


/*==============================================================
Function: mutex_delete()
Description: Destroy a mutex
Parameters:
    mut -- pointer to mutex
Returns:
    RS_EEXIST -- there are tasks waiting mutex
    RS_EOK -- succeed
Notes:
    If there are tasks waiting mutex, the mutex can not be destroy.
==============================================================*/
__fun_
status_t r_mutex_delete(mutex_t __p_* mut) __ef_
{
    RSI_ASSERT(mut != NULL);
    RSI_ASSERT(mut->init == RSI_INITED);

    /* If mutex is not available. */
    if (mut->owner != RSI_NULL_PRIO) {

        /* Return failed. */
        return RS_EEXIST;
    }

    /* Release priority occupancy by mutex. */
    _TASKS[mut->ceiling].state = RSI_STA_NONE;

#if CFG_DEBUG > 0

    /* Mutex initialized flag. */
    mut->init = 0;
#endif

    /* Return succeed. */
    return RS_EOK;
}
#endif

/*===========================================================================*/


