/*
===============================================================================
| COPYRIGHT (C) 2003-2007 Haishen Ruan, All rights reserved.
| SUMMARY:
|   Counting semaphore implementation.
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


#if CFG_SEMAPHORE_EN > 0

/*==============================================================
Function: semaphore_create()
Description: Initialize semaphore.
Parameters:
    sem -- pointer to semaphore
    name -- semaphore name
    value -- initialized value
Returns:
    RS_EOK -- succeed
Notes:
==============================================================*/
#if CFG_OBJ_NAME_EN > 0

__fun_
status_t r_semaphore_create(
            sem_t __p_* sem,
            name_t      name,
            count_t     value
            ) __ef_

#else

__fun_
status_t r_semaphore_create(
            sem_t __p_* sem,
            count_t     value
            ) __ef_

#endif
{
    RSI_ASSERT(sem != NULL);

#if CFG_OBJ_NAME_EN > 0

    /* Save semaphore name. */
    sem->name.w = name;

#endif

    /* Initialize semaphore control block. */
    sem->count = value;

    /* Initialize wait queue. */
    __cque_init(sem->waits);

#if CFG_DEBUG > 0
    sem->init = RSI_INITED;
#endif

    /* Return succeed. */
    return RS_EOK;
}


/*==============================================================
Function: semaphore_wait()
Description: Obtain a semaphore
Parameters:
    sem -- pointer to semaphore
    ticks -- ticks to wait
Returns:
    RS_ELOCK -- kernel is locked
    RS_ETIMEOUT -- timer out
    RS_EOK -- succeed
Notes:
    Avoid calling this function from interrupt routine.
==============================================================*/
#if CFG_IPC_TIMEOUT_EN > 0 || CFG_DEBUG > 0

__fun_
status_t r_semaphore_wait(sem_t __p_* sem, tick_t ticks) __ef_

#else

__fun_
status_t r_semaphore_wait(sem_t __p_* sem) __ef_

#endif
{
    IRQ_SAVEAREA;

    RSI_ASSERT(sem != NULL);
    RSI_ASSERT(sem->init == RSI_INITED);

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

    /* Determine whether semaphore is available. */
    if (sem->count > 0) {

        /* Yes.
         * Decrease semaphore value. */
        sem->count--;

        irq_unlock();

        /* Return succeed. */
        return RS_EOK;
    }

#if CFG_IPC_TIMEOUT_EN > 0

    /* Semaphore is not available, block current task. */
    __ipc_block(&sem->waits, ticks);
#else

    /* Semaphore is not available, block current task. */
    __ipc_block(&sem->waits, 0);
#endif

    irq_unlock();

    /* Schedule tasks. */
    __schedule();

#if CFG_IPC_TIMEOUT_EN > 0

    irq_lock();

    /* Determine if task is waked up because time out. */
    if (_TASKS[_current_prio].state & RSI_STA_BLOCKED) {

        /* Yes. */
        __ipc_timeout(&sem->waits);

        irq_unlock();

        /* Task has time out, Return. */
        return RS_ETIMEOUT;
    }

    /* Task is waked up because semaphore available.
     * Clear task delay counter. */
    _TASKS[_current_prio].delay = 0;

    irq_unlock();

#elif CFG_DEBUG > 0
    if (ticks != 0) {
        RSI_TRACE("!! Warning: CFG_IPC_TIMEOUT_EN == 0, ticks is ignored");
    }
#endif

    /* Return succeed. */
    return RS_EOK;
}


/*==============================================================
Function: semaphore_post()
Description: Release a semaphore.
Parameters:
    sem -- pointer to semaphore
Returns:
    RS_EOK -- succeed
Notes:
==============================================================*/
__fun_
status_t r_semaphore_post(sem_t __p_* sem) __ef_
{
    IRQ_SAVEAREA;

    RSI_ASSERT(sem != NULL);
    RSI_ASSERT(sem->init == RSI_INITED);

    irq_lock();

    /* If there are tasks waiting semaphore.
     * Resume the highest priority one. */
    if (__ipc_resume(&sem->waits) != RSI_NULL_PRIO) {

        irq_unlock();

        /* Task is resume, need schedule. */
        __schedule();

    /* Wether the semaphore value within the limitation. */
    } else if (sem->count < (count_t)-1) {

        /* Increase semaphore value. */
        sem->count++;

        irq_unlock();

    } else {
        irq_unlock();
    }

    return RS_EOK;
}


/*==============================================================
Function: semaphore_trywait()
Description: Obtain a semaphore dispense with wait.
Parameters:
    sem -- pointer to semaphore
Returns:
    RS_ENAVAIL -- semaphore is not available
    RS_EOK -- succeed
Notes:
==============================================================*/
__fun_
status_t r_semaphore_trywait(sem_t __p_* sem) __ef_
{
    IRQ_SAVEAREA;

    RSI_ASSERT(sem != NULL);
    RSI_ASSERT(sem->init == RSI_INITED);

    irq_lock();

    /* Whether semaphore is available. */
    if (sem->count > 0) {

        /* Yes.
         * Decrease semaphore value. */
        sem->count--;

        irq_unlock();

        /* Return succeed. */
        return RS_EOK;
    }

    irq_unlock();

    /* Semaphore is not available.
     * Return failed. */
    return RS_ENAVAIL;
}


/*==============================================================
Function: semaphore_getvalue()
Description: Get semaphore information.
Parameters:
    sem -- pointer to semaphore
    info -- pointer to semaphore information struct
Returns:
    RS_EOK -- succeed
Notes:
==============================================================*/
__fun_
status_t r_semaphore_info(
            sem_t     __p_* sem,
            seminfo_t __out_* info
            ) __ef_
{

    RSI_ASSERT(sem != NULL);
    RSI_ASSERT(sem->init == RSI_INITED);

#if CFG_OBJ_NAME_EN > 0

    /* Get semaphore name. */
    info->name[0] = sem->name.c[0];
    info->name[1] = sem->name.c[1];
    info->name[2] = sem->name.c[2];
    info->name[3] = sem->name.c[3];

#else
    info->name[0] = 0;
    
#endif

    /* Determine if semaphore has a name. */
    if (info->name[0] == 0) {
        info->name[0] = RSI_NA0;
        info->name[1] = RSI_NA1;
        info->name[2] = RSI_NA2;
        info->name[3] = RSI_NA3;
    }

    info->name[4] = 0;

    /* Retrieve semaphore value. */
    info->count = sem->count;

    /* Return succeed. */
    return RS_EOK;
}


/*==============================================================
Function: semaphore_delete()
Description: Destroy a semaphore.
Parameters:
    sem -- pointer to semaphore
Returns:
    RS_EOK -- succeed
Notes:
    This operation will wake up all of tasks which are waiting
    the semaphore available.
==============================================================*/
__fun_
status_t r_semaphore_delete(sem_t __p_* sem) __ef_
{
    IRQ_SAVEAREA;

    RSI_ASSERT(sem != NULL);
    RSI_ASSERT(sem->init == RSI_INITED);

    for (;;) {

        irq_lock();

        /* If there are tasks waiting the binary semaphore,
         * resume the highest priority one. */
        if (__ipc_resume(&sem->waits) == RSI_NULL_PRIO) {

            irq_unlock();

            /* Wait queue is empty, break. */
            break;
        }

        irq_unlock();
    }

#if CFG_DEBUG > 0
    sem->init = 0;
#endif

    /* Return succeed. */
    return RS_EOK;
}
#endif


/*===========================================================================*/


