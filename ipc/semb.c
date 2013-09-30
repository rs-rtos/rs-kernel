/*
===============================================================================
| COPYRIGHT (C) 2003-2007 Haishen Ruan, All rights reserved.
| SUMMARY:
|   Binary semaphore implementation.
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


#if CFG_SEMBINARY_EN > 0

/*==============================================================
Function: sembinary_create()
Description: Initialize binary semaphore.
Parameters:
    sem -- pointer to semaphore
    value -- initialized value
Returns:
    RS_EOK -- succeed
Notes:
==============================================================*/
#if CFG_OBJ_NAME_EN > 0
__fun_
status_t r_sembinary_create(
            semb_t __p_* semb,
            name_t       name,
            bool         value
            ) __ef_
#else
__fun_
status_t r_sembinary_create(
            semb_t __p_* semb,
            bool         value
            ) __ef_
#endif
{
    RSI_ASSERT(semb != NULL);

#if CFG_OBJ_NAME_EN > 0
    semb->name.w = name;
#endif

    /* Initialize semaphore control block. */
    semb->avail = value;

    /* Initialize wait queue. */
    __cque_init(semb->waits);

#if CFG_DEBUG > 0
    semb->init = RSI_INITED;
#endif

    return RS_EOK;
}


/*==============================================================
Function: sembinary_wait()
Description: Obtain a binary semaphore.
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
status_t r_sembinary_wait(
            semb_t __p_* semb,
            tick_t       ticks
            ) __ef_
#else
__fun_
status_t r_sembinary_wait(
            semb_t __p_* semb
            ) __ef_
#endif
{
    IRQ_SAVEAREA;

    RSI_ASSERT(semb != NULL);
    RSI_ASSERT(semb->init == RSI_INITED);

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
    if (semb->avail) {

        /* Yes. Obtain semaphore. */
        semb->avail = false;

        irq_unlock();

        /* Return succeed. */
        return RS_EOK;
    }

#if CFG_IPC_TIMEOUT_EN > 0

    /* Semaphore is not available, block current task. */
    __ipc_block(&semb->waits, ticks);
#else

    /* Semaphore is not available, block current task. */
    __ipc_block(&semb->waits, 0);
#endif

    irq_unlock();

    /* Schedule tasks. */
    __schedule();

#if CFG_IPC_TIMEOUT_EN > 0

    irq_lock();

    /* Determine if task is waked up because time out. */
    if (_TASKS[_current_prio].state & RSI_STA_BLOCKED) {

        /* Yes. call ipc timeout. */
        __ipc_timeout(&semb->waits);

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
Function: sembinary_post()
Description: Release a binary semaphore.
Parameters:
    sem -- pointer to semaphore
Returns:
    RS_EOK -- succeed
Notes:
==============================================================*/
__fun_
status_t r_sembinary_post(semb_t __p_* semb) __ef_
{
    IRQ_SAVEAREA;

    RSI_ASSERT(semb != NULL);
    RSI_ASSERT(semb->init == RSI_INITED);

    irq_lock();

    /* If there are tasks waiting semaphore.
     * Resume the highest priority one. */
    if (__ipc_resume(&semb->waits) != RSI_NULL_PRIO) {

        irq_unlock();

        /* Task is resume, need schedule. */
        __schedule();

    } else {

        /* No task waiting semaphore.
         * Release it. */
        semb->avail = true;

        irq_unlock();
    }

    return RS_EOK;
}

/*==============================================================
Function: sembinary_trywait()
Description: Obtain a binary semaphore dispense with wait.
Parameters:
    sem -- pointer to semaphore
Returns:
    RS_ENAVAIL -- semaphore is not available
    RS_EOK -- succeed
Notes:
==============================================================*/
__fun_
status_t r_sembinary_trywait(semb_t __p_* semb) __ef_
{
    IRQ_SAVEAREA;

    RSI_ASSERT(semb != NULL);
    RSI_ASSERT(semb->init == RSI_INITED);

    irq_lock();

    /* Determine whether semaphore is available. */
    if (semb->avail) {

        /* Yes. Obtain semaphore. */
        semb->avail = false;

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
Function: sembinary_info()
Description: Get binary semaphore information.
Parameters:
    sem -- pointer to binary semaphore
    info -- pointer to binary semaphore information struct
Returns:
    RS_EOK -- succeed.
Notes:
==============================================================*/
__fun_
status_t r_sembinary_info(
            semb_t      __p_* semb,
            sembinfo_t  __out_* info
            ) __ef_
{

    RSI_ASSERT(semb != NULL);
    RSI_ASSERT(semb->init == RSI_INITED);

#if CFG_OBJ_NAME_EN > 0

    /* Get binary semaphore name. */
    info->name[0] = semb->name.c[0];
    info->name[1] = semb->name.c[1];
    info->name[2] = semb->name.c[2];
    info->name[3] = semb->name.c[3];

#else
    info->name[0] = 0;
    
#endif

    /* Determine if binary semaphore has a name. */
    if (info->name[0] == 0) {
        info->name[0] = RSI_NA0;
        info->name[1] = RSI_NA1;
        info->name[2] = RSI_NA2;
        info->name[3] = RSI_NA3;
    }

    info->name[4] = 0;

    /* Return binary semaphore value. */
    info->value = semb->avail;

    /* Return succeed. */
    return RS_EOK;
}

/*==============================================================
Function: sembinary_delete()
Description: Destroy a binary semaphore.
Parameters:
    sem -- pointer to semaphore
Returns:
    RS_EOK -- succeed
Notes:
    This operation will wake up all of tasks which are waiting
    the binary semaphore available.
==============================================================*/
__fun_
status_t r_sembinary_delete(semb_t __p_* semb) __ef_
{
    IRQ_SAVEAREA;

    RSI_ASSERT(semb != NULL);
    RSI_ASSERT(semb->init == RSI_INITED);

    for (;;) {

        irq_lock();

        /* If there are tasks waiting the binary semaphore,
         * resume the highest priority one. */
        if (__ipc_resume(&semb->waits) == RSI_NULL_PRIO) {

            irq_unlock();

            /* Wait queue is empty, break. */
            break;
        }

        irq_unlock();
    }

#if CFG_DEBUG > 0
    semb->init = 0;
#endif

    /* Return succeed. */
    return RS_EOK;
}
#endif


/*===========================================================================*/


