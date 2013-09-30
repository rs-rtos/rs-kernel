/*
===============================================================================
| COPYRIGHT (C) 2003-2007 Haishen Ruan, All rights reserved.
| SUMMARY:
|   Task implementation.
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


/* Task control block area. */
tasks_t _tasks;

/* Initialize task normal priority. */
#if CFG_TASK_MODE > 1

#define __nprio_init(prio) \
do { \
    _TASKS[prio].prio.normal = prio; \
} while (0)

#else

#define __nprio_init(prio)    RSI_MPT

#endif


/* Initialize pointer which pointer to blocked queue. */
#if CFG_IPC_EN > 0

#define __pend_q_init(prio) \
do { \
    _TASKS[prio].pend_q = NULL; \
} while (0)

#else

#define __pend_q_init(prio)   RSI_MPT

#endif


/* Initialize task control block. */
#define __tcb_init(prio, stack_base) \
do { \
    _TASKS[prio].stack       = stack_base; \
    _TASKS[prio].delay       = 0; \
    __nprio_init(prio); \
    __nprio_lock_init(prio); \
    __pend_q_init(prio); \
} while(0)

/* Free task control block. */
#define __tcb_free(prio) \
do { \
    _TASKS[prio].delay  = 0; \
    _TASKS[prio].state  = RSI_STA_NONE; \
} while(0)


#define __tcb_zero(prio)    __tcb_free(prio)


/*==============================================================
Function: task_create()
Description: This function creates a task and then places it
             on the list of ready queue.
Parameters:
    prio -- task priority
    entry -- entry point function
    arg -- entry data
    name -- optional task name
    stack_base -- stack base address
    stack_size -- stack size
    options -- task create options
Returns:
    RS_EOUTRANGE -- task priority out of range
    RS_EEXIST -- the task with the same priority already exist.
    RS_ENULL -- stack address is unspecified. or memory is lack.
    RS_EOK -- succeed
Notes:
==============================================================*/
__fun_
status_t r_task_create(
    prio_t      prio,
#if CFG_OBJ_NAME_EN > 0
    name_t      name,
#endif
    entry_t     entry,
    arg_t       arg,
    sp_t        stack_base
#if RSI_DEF_PARAM_STKSZ > 0
    ,
    mmsz_t      stack_size
#endif
#if RSI_DEF_PARAM_OPTS > 0
    ,
    int8u       options
#endif
) __ef_
{
    RSI_REG sp_t pstack;

    IRQ_SAVEAREA;

    RSI_ASSERT(entry != NULL);

#if RSI_IDLE_PRIO < 0xff && CFG_ARG_CHK > 1

    /* Wether the priority within the limitation. */
    if (prio > RSI_IDLE_PRIO) {

        /* No, the priority exceed the limitation.
         * Return failed. */
        return RS_EOUTRANGE;
    }

#elif RSI_IDLE_PRIO < 0xff
    RSI_ASSERT(prio <= RSI_IDLE_PRIO);
#endif

    irq_lock();

    /* Determine if the TCB designated priority is free. */
    if (_TASKS[prio].state != RSI_STA_NONE) {

        irq_unlock();

        /* No. The TCB already used by other one, return failed. */
        return RS_EEXIST;
    }

    /* Used the TCB. */
    _TASKS[prio].state = RSI_STA_ZOMBIE;

    irq_unlock();

    /* If address of stack is specified. */
    if (stack_base == (sp_t)0)
    {
        /* No. Address of stack is unspecified. */

#if CFG_MPOOL_EN > 0
        /* Allocate a block of memory on the heap. */
        stack_base = mpool_alloc(stack_size);

        /* If allocate memory successfully. */
        if (stack_base == NULL) {

            /* Free task control block. */
            _TASKS[prio].state = RSI_STA_NONE;

            /* No. Memory is lack, return failed. */
            return RS_ENULL;
        }

        options |= RS_OPT_KFREE;

#else
        /* Free task control block. */
        _TASKS[prio].state = RSI_STA_NONE;

        /* Return failed. */
        return RS_ENULL;
#endif
    }


    /* Task create hook function. */
    HOOK_TASK_CREATE(prio);


#if CFG_TASK_INFO_EN > 0

    /* Fill task stack with specified character. */
    memset(stack_base, RSI_STACK_UNUSE, stack_size);

#endif


#if ARCH_STACK_GROW == UPWARDS

    /* Initialize task stack. */
    pstack = __stack_init(entry, arg, stack_base);
#else

    /* Initialize task stack. */
    pstack = __stack_init(entry, arg, stack_base + stack_size);
#endif

    /* Initialize task control block. */
    __tcb_init(prio, pstack);

#if CFG_TASK_INFO_EN > 0

    /* Record task stack information. */
    _TASKS[prio].stack_base = stack_base;
    _TASKS[prio].stack_size = stack_size;

#elif CFG_MPOOL_EN > 0

    /* Save task stack base pointer. */
    _TASKS[prio].stack_base = stack_base;

#endif


#if CFG_OBJ_NAME_EN > 0
    /* Task's name */
    _TASKS[prio].name.w = name;
#endif


#if RSI_DEF_PARAM_OPTS > 0

    /* Determine if task stack should be free automatically. */
    if (options & RS_OPT_KFREE) {

        /* Yes. Set task state with RSI_STA_KFREE. */
        _TASKS[prio].state |= RSI_STA_KFREE;
    }

    /* Determine if task should be suspend. */
    if (options & RS_OPT_SUSPEND) {

        /* Yes. suspend task. */
        _TASKS[prio].state = (_TASKS[prio].state & ~RSI_STA_ZOMBIE) |
                             (RSI_STA_READY|RSI_STA_SUSPEND|RSI_STA_PREREADY);


        /* Return succeed. */
        return RS_EOK;
    }

    irq_lock();

    /* Ready for execution. */
    _TASKS[prio].state = (_TASKS[prio].state & ~RSI_STA_ZOMBIE) |
                          RSI_STA_READY;

    /* Insert the task to ready queue. */
    __ready_que_insert(prio);

    irq_unlock();

#else

    irq_lock();

    /* Ready for execution. */
    _TASKS[prio].state = RSI_STA_READY;

    /* Insert the task to ready queue. */
    __ready_que_insert(prio);

    irq_unlock();

#endif


    /* Need schedule task. */
    __schedule();

    /* Return succeed. */
    return RS_EOK;
}


#if CFG_TASK_DELETE_EN > 0 && CFG_TASK_MODE < 2
/*==============================================================
Function: task_delete()
Description: Delete a task and free task control block.
Parameters:
    prio -- task priority
Returns:
    RS_EOUTRANGE -- task priority out of range.
    RS_ENOTEXIST -- task not exist
    RS_ELOCK -- task is locked.
    RS_EOK -- succeed
Notes:
==============================================================*/
__fun_
status_t r_task_delete(prio_t prio) __ef_
{
    IRQ_SAVEAREA;

#if RSI_IDLE_PRIO < 0xff && CFG_ARG_CHK > 1

    /* Wether the priority within the limitation. */
    if (prio >= RSI_IDLE_PRIO && prio != RSI_SELF_PRIO) {

        /* No, the priority exceed the limitation.
         * Return failed. */
        return RS_EOUTRANGE;
    }
#else
    RSI_ASSERT(prio < RSI_IDLE_PRIO || prio == RSI_SELF_PRIO);
#endif

    irq_lock();

    if (prio == RSI_SELF_PRIO) {

        /* Current task priority. */
        prio = _current_prio;

    /* Determine if task designated priority is exist. */
    } else if (!(_TASKS[prio].state & RSI_STA_READY)) {

        irq_unlock();

        /* No. return failed. */
        return RS_ENOTEXIST;

    /* Determine if task is locked. */
    } else if (_TASKS[prio].lock > 0) {

        irq_unlock();

        /* Yes. retrun failed. */
        return RS_ELOCK;
    }

#if CFG_IPC_EN > 0

    /* Determine if pointer to blocked queue is empty. */
    if (_TASKS[prio].pend_q != NULL) {

        /* Remove task form blocked queue. */
        __cque_remove_optm(_TASKS[prio].pend_q, prio);
    } else
#endif
    /* Remove task form ready queue. */
    { __ready_que_remove(prio); }

    /* Lock kernel. */
    __lock_k();

    irq_unlock();

#if CFG_MPOOL_EN > 0

    /* If need free task stack. */
    if (_TASKS[prio].state & RSI_STA_KFREE) {

        /* Yes. Free task stack. */
        mpool_free(_TASKS[prio].stack_base);
    }
#endif

    /* Free task control block. */
    __tcb_free(prio);

    /* Task delete hook function. */
    HOOK_TASK_DELETE(prio);

    /* Unlock kernel. */
    __unlock_k();

    /* Need schedule task. */
    __schedule();

    /* Return succeed. */
    return RS_EOK;
}


#elif CFG_TASK_DELETE_EN > 0
/*==============================================================
Function: task_delete()
Description: Delete a task and free task control block.
Parameters:
    prio -- task priority
Returns:
    RS_EOUTRANGE -- task priority out of range.
    RS_ENOTEXIST -- task not exist
    RS_ELOCK -- task is locked.
    RS_EOK -- succeed
Notes:
==============================================================*/
__fun_
status_t r_task_delete(prio_t prio) __ef_
{
    RSI_REG prio_t cprio;

    IRQ_SAVEAREA;

#if RSI_IDLE_PRIO < 0xff && CFG_ARG_CHK > 1

    /* Wether the priority within the limitation. */
    if (prio >= RSI_IDLE_PRIO && prio != RSI_SELF_PRIO) {

        /* No, the priority exceed the limitation.
         * Return failed. */
        return RS_EOUTRANGE;
    }
#else
    RSI_ASSERT(prio < RSI_IDLE_PRIO || prio == RSI_SELF_PRIO);
#endif

    irq_lock();

    /* Determine if priority is self priority for the current task. */
    if (prio == RSI_SELF_PRIO) {

        /* Yes. Get current priority of current executing task. */
        cprio = _current_prio;

        /* Normal priority of thask current executing. */
        prio = _TASKS[cprio].prio.normal;

    /* Determine if the priority is normal priority. */
    } else if (!__prio_isnormal(prio)) {

        irq_unlock();

        /* Task not exist, retrun failed. */
        return RS_ENOTEXIST;

    } else {

        /* Task current priority. */
        cprio = _TASKS[prio].prio.current;
    }

    /* Determine if Task is locked. */
    if (_TASKS[cprio].lock > 0) {

        irq_unlock();

        /* Task is locked, retrun failed. */
        return RS_ELOCK;
    }


#if CFG_IPC_EN > 0

    /* Determine if pointer to blocked queue is empty. */
    if (_TASKS[cprio].pend_q != NULL) {

        /* Remove task form blocked queue. */
        __cque_remove_optm(_TASKS[cprio].pend_q, cprio);
    } else
#endif
    /* Remove task form ready queue. */
    { __ready_que_remove(cprio); }

    /* Lock kernel. */
    __lock_k();

    irq_unlock();

#if CFG_MPOOL_EN > 0

    /* If need free task stack. */
    if (_TASKS[prio].state & RSI_STA_KFREE) {

        /* Yes. Free task stack. */
        mpool_free(_TASKS[prio].stack_base);
    }
#endif

    /* Clear Task control block. */
    __tcb_free(cprio);

    /* Task delete hook function. */
    HOOK_TASK_DELETE(prio);

    /* Unlock kernel. */
    __unlock_k();

    /* Need schedule task. */
    __schedule();

    /* Return succeed. */
    return RS_EOK;
}


#elif CFG_DEBUG > 0
/*==============================================================
Function: task_delete()
Description: Delete a task and free task control block.
Parameters:
    prio -- task priority
Returns:
    RS_EOK -- succeed
Notes:
    Debug version. Only configure (CFG_TASK_DELETE_EN == 0),
    and (CFG_DEBUG > 0), this function Determine whether the application
    attempt to delete a task.
==============================================================*/
__fun_
status_t r_task_delete(prio_t prio) __ef_
{
    RSI_DISABLE_WARNING(prio);

    RSI_ASSERT(!"Task delete not allow!");
    return RS_EOK;
}
#endif


#if CFG_TASK_MODE == 1
/*==============================================================
Function: task_change_prio()
Description: Change task priority.
Parameters:
    prio -- task priority
    new_prio -- new priority
Returns:
    RS_EOUTRANGE -- task priority out of range.
    RS_ENOTEXIST -- task not exist.
    RS_ELOCK -- task is locked.
    RS_EOK -- succeed.
Notes:
==============================================================*/
__fun_
status_t r_task_change_prio(prio_t prio, prio_t new_prio) __ef_
{
    IRQ_SAVEAREA;

#if RSI_IDLE_PRIO < 0xff && CFG_ARG_CHK > 1

    /* Wether the priority within the limitation. */
    if ((prio >= RSI_IDLE_PRIO && prio != RSI_SELF_PRIO) ||
        new_prio >= RSI_IDLE_PRIO)
    {
        /* No, the priority exceed the limitation.
         * Return failed. */
        return RS_EOUTRANGE;
    }
#elif CFG_ARG_CHK > 1

    /* Wether the priority within the limitation. */
    if (new_prio >= RSI_IDLE_PRIO) {

        /* No, the priority exceed the limitation.
         * Return failed. */
        return RS_EOUTRANGE;
    }
#else

    RSI_ASSERT((prio < RSI_IDLE_PRIO || prio == RSI_SELF_PRIO) &&
            new_prio < RSI_IDLE_PRIO);
#endif

    irq_lock();

    /* Determine if the TCB designated by new priority is free. */
    if (_TASKS[new_prio].state != RSI_STA_NONE) {

        irq_unlock();

        /* TCB is already in used, return failed. */
        return RS_EEXIST;
    }

    /* Reserve TCB area. */
    _TASKS[new_prio].state = RSI_STA_ZOMBIE;

    irq_unlock();

    /* Here enable interrupts briefly */
    __dummy();

    irq_lock();

    /* Determine if priority is self priority for the current task. */
    if (prio == RSI_SELF_PRIO) {

        /* Yes. Get priority of current executing task. */
        prio = _current_prio;

    /* See if TCB designated by priority is available.*/
    } else if (!(_TASKS[prio].state & RSI_STA_READY)) {

        irq_unlock();

        /* Free TCB designated by new priority. */
        _TASKS[new_prio].state = RSI_STA_NONE;

        /* Return failed. */
        return RS_ENOTEXIST;
    }

    /* Determine if Task is locked. */
    if (_TASKS[prio].lock > 0) {

        irq_unlock();

        /* Free TCB designated by new priority. */
        _TASKS[new_prio].state = RSI_STA_NONE;

        /* Task is locked, return failed. */
        return RS_ELOCK;
    }

    /* Copy TCB to new position. */
    _TASKS[new_prio] = _TASKS[prio];

    /* Clear TCB designated by old priority */
    __tcb_free(prio);

#if CFG_IPC_EN > 0

    /* Determine if pointer to blocked queue is empty. */
    if (_TASKS[new_prio].pend_q != NULL) {

        RSI_REG cque_t __p_* rp;

        /* Retrieve blocked queue. */
        rp = _TASKS[new_prio].pend_q;

        /* Adjust task priority in blocked queue. */
        __cque_remove((*rp), prio);
        __cque_insert((*rp), new_prio);
    } else
#endif
    /* Adjust task priority in ready queue. */
    { __ready_que_move(prio, new_prio); }

    /* See if Task is current executing one. */
    if (_current_prio == prio) {

        /* Yes. Adjust current priority to new priority. */
        _current_prio = new_prio;
    }

    irq_unlock();

    /* Need schedule task. */
    __schedule();

    /* Return succeed. */
    return RS_EOK;
}
#endif


#if CFG_TASK_MODE == 2
/*==============================================================
Function: task_change_prio()
Description: Change task priority.
Parameters:
    prio -- task priority
    new_prio -- new task priority
Returns:
    RS_EOUTRANGE -- task priority out of range.
    RS_ENOTEXIST -- task not exist.
    RS_ELOCK -- task is locked.
    RS_EOK -- succeed
Notes:
==============================================================*/
__fun_
status_t r_task_change_prio(prio_t prio, prio_t new_prio) __ef_
{
    RSI_REG prio_t cprio;

    IRQ_SAVEAREA;

#if RSI_IDLE_PRIO < 0xff && CFG_ARG_CHK > 1

    /* Wether the priority within the limitation. */
    if ((prio >= RSI_IDLE_PRIO && prio != RSI_SELF_PRIO) ||
        new_prio >= RSI_IDLE_PRIO)
    {
        /* No, the priority exceed the limitation.
         * Return failed. */
        return RS_EOUTRANGE;
    }
#elif CFG_ARG_CHK > 1

    /* Wether the new priority within the limitation. */
    if (new_prio >= RSI_IDLE_PRIO) {

        /* No, the priority exceed the limitation.
         * Return failed. */
        return RS_EOUTRANGE;
    }
#else

    RSI_ASSERT((prio < RSI_IDLE_PRIO || prio == RSI_SELF_PRIO) &&
            new_prio < RSI_IDLE_PRIO);
#endif

    irq_lock();

    /* Determine if the TCB designated by new priority is free. */
    if (_TASKS[new_prio].state != RSI_STA_NONE) {
        irq_unlock();

        /* TCB is already in used, return failed. */
        return RS_EEXIST;
    }

    /* Set task state with mark RSI_STA_ZOMBIE. */
    _TASKS[new_prio].state = RSI_STA_ZOMBIE;

    irq_unlock();

    /* Here enable interrupts briefly */
    __dummy();

    irq_lock();

    /* Determine if priority is self priority for the current task. */
    if (prio == RSI_SELF_PRIO) {

        /* Yes. Get current priority of current executing task. */
        cprio = _current_prio;

        /* Get normal priority of current executing task. */
        prio = _TASKS[cprio].prio.normal;

    /* See if the priority is normal priority. */
    } else if (!__prio_isnormal(prio)) {

        irq_unlock();

        /* Free TCB designated by new priority. */
        _TASKS[new_prio].state = RSI_STA_NONE;

        /* No. Task is not exist, return failed. */
        return RS_ENOTEXIST;

    } else {

        /* Yes. Get task current priority. */
        cprio = _TASKS[prio].prio.current;
    }

    /* Determine if Task is locked. */
    if (_TASKS[cprio].lock > 0) {

        irq_unlock();

        /* Free TCB designated by new priority. */
        _TASKS[new_prio].state = RSI_STA_NONE;

        /* Task is locked, return failed. */
        return RS_ELOCK;
    }

    /* Change normal priority to new value. */
    _TASKS[cprio].prio.normal = new_prio;

    /* Copy TCB to new position. */
    _TASKS[new_prio] = _TASKS[prio];

    /* Clear TCB designated by old priority. */
    __tcb_free(prio);

    /* Determine if task is executing in normal priority. */
    if (cprio == prio) {

#if CFG_IPC_EN > 0

        /* Determine if pointer to blocked queue is empty. */
        if (_TASKS[new_prio].pend_q != NULL) {
            RSI_REG cque_t __p_* rp;

            /* Retrieve blocked queue. */
            rp = _TASKS[new_prio].pend_q;

            /* Adjust task priority in blocked queue. */
            __cque_remove((*rp), cprio);
            __cque_insert((*rp), new_prio);
        } else
#endif
        /* Adjust task priority in ready queue. */
        { __ready_que_move(cprio, new_prio); }
    }

    /* See if Task is current executing one. */
    if (_current_prio == prio) {

        /* Yes. Adjust current priority to new priority. */
        _current_prio = new_prio;
    }

    irq_unlock();

    /* Need schedule task. */
    __schedule();

    /* Return succeed. */
    return RS_EOK;
}


/*==============================================================
Function: task_change_runprio()
Description: Change task current executing priority.
Parameters:
    prio -- task priority
    runprio -- running priority
Returns:
    RS_EOUTRANGE -- task priority out of range.
    RS_ENOTEXIST -- task is not exist.
    RS_EINVAL -- running priority is in used already.
    RS_ENAVAIL -- task is locked.
    RS_EOK -- succeed.
Notes:
==============================================================*/
__fun_
status_t r_task_change_runprio(prio_t prio, prio_t runprio) __ef_
{
    RSI_REG prio_t cprio;

    IRQ_SAVEAREA;

#if RSI_IDLE_PRIO < 0xff && CFG_ARG_CHK > 1

    /* Wether the priority within the limitation. */
    if ((prio >= RSI_IDLE_PRIO && prio != RSI_SELF_PRIO) ||
        (runprio >= RSI_IDLE_PRIO && runprio != RSI_SELF_PRIO))
    {
        /* No, the priority exceed the limitation.
         * Return failed. */
        return RS_EOUTRANGE;
    }
/* #elif CFG_ARG_CHK > 1 */

    /* Wether the running priority within the limitation. */
    /* if (runprio >= RSI_IDLE_PRIO) { */

        /* No, the priority exceed the limitation.
         * Return failed. */
        /* return RS_EOUTRANGE;
    }*/
#else

    RSI_ASSERT((prio < RSI_IDLE_PRIO || prio == RSI_SELF_PRIO) &&
            (runprio < RSI_IDLE_PRIO || runprio == RSI_SELF_PRIO));
#endif

    irq_lock();

    /* Determine if priority is self priority for the current task. */
    if (prio == RSI_SELF_PRIO) {

        /* Yes. Get current priority of current executing task. */
        cprio = _current_prio;

        /* Get normal priority of current executing task. */
        prio = _TASKS[cprio].prio.normal;

    /* See if the priority is normal priority. */
    } else if (!__prio_isnormal(prio)) {

        irq_unlock();

        /* No. Task is not exist, return failed. */
        return RS_ENOTEXIST;

    } else {

        /* Get current priority of task. */
        cprio = _TASKS[prio].prio.current;
    }

    /* Lock task. */
    __nprio_lock(cprio);

    irq_unlock();

    /* See if runprio is self priority. */
    if (runprio == RSI_SELF_PRIO) {

        /* Yes.
         * Reset task run priority to normal priority. */
        runprio = prio;
    }
    
    /* Here enable interrupts briefly */
    /* __dummy(); */

    irq_lock();

    /* Determine if TCB is in used.
     * Or application attempt to reset the running priority to normal. */
    if (_TASKS[runprio].state == RSI_STA_NONE ||
        (_TASKS[runprio].state == RSI_STA_RESERVED &&
        prio == runprio))
    {
        /* Lock task state. */
        _TASKS[runprio].state |= RSI_STA_ZOMBIE;

    } else {

        /* Get task current executing priority. */
        cprio = _TASKS[prio].prio.current;

        /* Unlock task. */
        __nprio_unlock(cprio);

        irq_unlock();

        /* Running priority is in used, return failed. */
        return RS_EINVAL;
    }

    irq_unlock();

    /* Here enable interrupts briefly */
    __dummy();

    irq_lock();

    /* Get task current executing priority. */
    cprio = _TASKS[prio].prio.current;

    /* Determine if Task state is in normal case. */
    if (!(_TASKS[prio].state & RSI_STA_ZOMBIE) ||
        prio == runprio)
    {
        /* Unlock task. */
        __nprio_unlock(cprio);

        /* Copy the current-priority TCB to runing-priority TCB. */
        _TASKS[runprio] = _TASKS[cprio];

        /* Free the old current TCB. */
        __tcb_free(cprio);

        /* See if task is execting in normal priority. */
        if (prio != runprio) {

            /* No. Reserve task normal priority. */
            _TASKS[prio].delay  = 0;
            _TASKS[prio].state  = RSI_STA_RESERVED;

            /* Set current priority to running priority. */
            _TASKS[prio].prio.current = runprio;
        }

#if CFG_IPC_EN > 0

        /* Determine if pointer to blocked queue is empty. */
        if (_TASKS[runprio].pend_q != NULL) {

            RSI_REG cque_t __p_* rp;

            /* Gain blocked queue pointer. */
            rp = _TASKS[runprio].pend_q;

            /* Adjust task priority form blocked queue. */
            __cque_remove((*rp), cprio);
            __cque_insert((*rp), runprio);

        } else
#endif
        /* Adjust task priority from ready queue. */
        { __ready_que_move(cprio, runprio); }

        /* See if Task is current executing one. */
        if (_current_prio == cprio) {

            /* Yes. Adjust current priority to running priority. */
            _current_prio = runprio;
        }

        irq_unlock();

        /* Schedule task. */
        __schedule();

        /* Return succeed. */
        return RS_EOK;

    } else {

        /* Unlock task. */
        __nprio_unlock(cprio);

        /* Clear task state. */
        _TASKS[runprio].state &= ~RSI_STA_ZOMBIE;

        irq_unlock();

        /* Return failed. */
        return RS_ENAVAIL;
    }
}
#endif


/*==============================================================
Function: task_sleep()
Description: This function lead the calling task sleep for a time
             which can be specified.
Parameters:
    ticks -- the sleeping time.
Returns:
    RS_EOK -- succeed
Notes:
==============================================================*/
__fun_
status_t r_task_sleep(tick_t ticks) __ef_
{
    RSI_REG prio_t current;

    IRQ_SAVEAREA;

    /* Specified ticks. */
    if (ticks > 0) {

        irq_lock();

        /* Get current executing task priority. */
        current = _current_prio;

        /* Sleep for specified ticks. */
        _TASKS[current].delay = ticks;

        /* Remove the task from the ready queue. */
        __ready_que_remove(current);

        irq_unlock();

        /* Need schedule task. Zzzz... */
        __schedule();
    }

    return RS_EOK;
}


#if CFG_TASK_MODE < 2
/*==============================================================
Function: task_wake()
Description: Wake up a sleeping task.
Parameters:
    prio -- priority of task which be waked up
Returns:
    RS_EOUTRANGE -- priority out of range.
    RS_EUNKNOW -- task is not in sleep, or task is suspended.
    RS_EOK -- succeed
Notes:
==============================================================*/
__fun_
status_t r_task_wake(prio_t prio) __ef_
{
    IRQ_SAVEAREA;

#if CFG_ARG_CHK > 1

    /* Wether the priority within the limitation. */
    if (prio >= RSI_IDLE_PRIO) {

        /* No, the priority exceed the limitation.
         * Return failed. */
        return RS_EOUTRANGE;
    }
#else
    RSI_ASSERT(prio < RSI_IDLE_PRIO);
#endif

    irq_lock();

#if CFG_TASK_SUSPEND_EN > 0

    /* If task is sleeping. And task didn't suspend. */
    if (_TASKS[prio].delay != 0 &&
        !(_TASKS[prio].state & RSI_STA_SUSPEND))

#else

    /* If task is sleeping. */
    if (_TASKS[prio].delay != 0)
#endif
    {
        /* Wake up. */
        _TASKS[prio].delay = 0;

        /* Insert task to ready queue. */
        __ready_que_insert(prio);
        irq_unlock();

        /* Need schedule task. */
        __schedule();

        return RS_EOK;
    }

    irq_unlock();

    /* Return failed. */
    return RS_EUNKNOW;
}


#else
/*==============================================================
Function: task_wake()
Description: Wake up a sleeping task.
Parameters:
    prio -- Priority of task witch be waked
Returns:
    RS_EOUTRANGE -- Priority out of range.
    RS_ENOTEXIST -- No such task.
    RS_EUNKNOW -- task is not sleeping, or task is suspended.
    RS_EOK -- succeed
Notes:
==============================================================*/
__fun_
status_t r_task_wake(prio_t prio) __ef_
{
    RSI_REG prio_t cprio;

    IRQ_SAVEAREA;

#if CFG_ARG_CHK > 1

    /* If the task priority in the limitation. */
    if (prio >= RSI_IDLE_PRIO) {

        /* No, the priority is exceed the limitation. */
        return RS_EOUTRANGE;
    }
#else
    RSI_ASSERT(prio < RSI_IDLE_PRIO);
#endif

    irq_lock();

    /* If normal priority. */
    if (!__prio_isnormal(prio)) {

        irq_unlock();

        /* No such task. Return failed. */
        return RS_ENOTEXIST;
    }

    /* The current priority of the task */
    cprio = _TASKS[prio].prio.current;

#if CFG_TASK_SUSPEND_EN > 0

    /* Make sure it is in sleep and not suspended. */
    if (_TASKS[cprio].delay != 0 &&
        !(_TASKS[cprio].state & RSI_STA_SUSPEND))
#else

    /* If the task is in sleep. */
    if (_TASKS[cprio].delay != 0)
#endif
    {
        /* Sleep is over. */
        _TASKS[cprio].delay = 0;

        /* Add into the ready queue. */
        __ready_que_insert(cprio);

        irq_unlock();

        /* Tasks need to be scheduled. */
        __schedule();

        return RS_EOK;
    }

    irq_unlock();

    /* The task can not be wake up. Return failed. */
    return RS_EUNKNOW;
}
#endif


#if CFG_TASK_SUSPEND_EN > 0 && CFG_TASK_MODE < 2
/*==============================================================
Function: task_suspend()
Description: Suspend a task.
Parameters:
    prio -- Priority of task
Returns:
    RS_EOUTRANGE -- Priority out of range.
    RS_ENOTEXIST -- No such task.
    RS_EOK -- succeed
Notes:
==============================================================*/
__fun_
status_t r_task_suspend(prio_t prio) __ef_
{
    IRQ_SAVEAREA;

#if CFG_ARG_CHK > 1

    /* If the task priority in the limitation. */
    if (prio >= RSI_IDLE_PRIO && prio != RSI_SELF_PRIO) {

        /* No, the priority is exceed the limitation. */
        return RS_EOUTRANGE;
    }
#else
    RSI_ASSERT(prio < RSI_IDLE_PRIO || prio == RSI_SELF_PRIO);
#endif

    irq_lock();

    /* Whether suspend itself. */
    if (prio == RSI_SELF_PRIO) {

        /* Yes. Get the current task priority. */
        prio = _current_prio;
    }

    /* Judge if TCB is valid. */
    if (_TASKS[prio].state & RSI_STA_READY) {

        /* Set as suspend state. */
        _TASKS[prio].state |= RSI_STA_SUSPEND;

        /* Erase the task from ready queue. */
        __ready_que_susremove(prio, _TASKS[prio].state);

        irq_unlock();

        /* Tasks need to be scheduled. */
        __schedule();

        return RS_EOK;
    }
    irq_unlock();

    /* No such task. Return failed. */
    return RS_ENOTEXIST;
}


/*==============================================================
Function: task_resume()
Description: Resume the suspended task.
Parameters:
    prio -- Priority of task
Returns:
    RS_EOUTRANGE -- Priority out of range.
    RS_ENOTEXIST -- No such task.
    RS_EOK -- succeed
Notes:
==============================================================*/
__fun_
status_t r_task_resume(prio_t prio) __ef_
{
    IRQ_SAVEAREA;

#if CFG_ARG_CHK > 1

    /* If the task priority in the limitation. */
    if (prio >= RSI_IDLE_PRIO) {

        /* No, the priority is exceed the limitation. */
        return RS_EOUTRANGE;
    }
#else
    RSI_ASSERT(prio < RSI_IDLE_PRIO);
#endif

    irq_lock();

    /* Determine Whether TCB is in effect. */
    if (_TASKS[prio].state & RSI_STA_READY) {

        /* Yes. Clear suspend flag. */
        _TASKS[prio].state &= ~RSI_STA_SUSPEND;

        /* Judge if task is in ready. */
        if ((_TASKS[prio].state & RSI_STA_PREREADY) ||
            (!(_TASKS[prio].state & RSI_STA_BLOCKED) &&
            _TASKS[prio].delay == 0))
        {
            /* Clear PREREADY */
            _TASKS[prio].state &= ~RSI_STA_PREREADY;

            /* Add task to ready queue. */
            __ready_que_insert(prio);
        }

        irq_unlock();

        /* Tasks need to be scheduled. */
        __schedule();

        /* Return succeed. */
        return RS_EOK;
    }

    irq_unlock();

    /* No such task. Return failed. */
    return RS_ENOTEXIST;
}


#elif CFG_TASK_SUSPEND_EN > 0
/*==============================================================
Function: task_suspend()
Description: Suspend a task.
Parameters:
    prio -- Priority of task
Returns:
    RS_EOUTRANGE -- Priority out of range.
    RS_ENOTEXIST -- No such task.
    RS_EOK -- succeed
Notes:
==============================================================*/
__fun_
status_t r_task_suspend(prio_t prio) __ef_
{
    RSI_REG prio_t cprio;

    IRQ_SAVEAREA;

#if CFG_ARG_CHK > 1

    /* If the task priority in the limitation. */
    if (prio >= RSI_IDLE_PRIO && prio != RSI_SELF_PRIO) {

        /* No, the priority is exceed the limitation. */
        return RS_EOUTRANGE;
    }
#else
    RSI_ASSERT(prio < RSI_IDLE_PRIO || prio == RSI_SELF_PRIO);
#endif

    irq_lock();

    /* Whether suspend itself. */
    if (prio == RSI_SELF_PRIO) {

        /* Yes. Get current priority of current task. */
        cprio = _current_prio;

        /* Get normal priority of current task. */
        prio  = _TASKS[cprio].prio.normal;

    /* If normal priority. */
    } else if (!__prio_isnormal(prio)) {

        irq_unlock();

        /* No such task. Return failed. */
        return RS_ENOTEXIST;

    } else {

        /* Get current priority of current task. */
        cprio = _TASKS[prio].prio.current;
    }

    /* Set task state with RSI_STA_SUSPEND. */
    _TASKS[cprio].state |= RSI_STA_SUSPEND;

    /* Remove task from ready queue. */
    __ready_que_susremove(cprio, _TASKS[cprio].state);

    irq_unlock();

    /* Tasks need to be scheduled. */
    __schedule();

    /* Return succeed. */
    return RS_EOK;
}


/*==============================================================
Function: task_resume()
Description: Resume a task.
Parameters:
    prio -- Priority of task
Returns:
    RS_EOUTRANGE -- Priority out of range.
    RS_ENOTEXIST -- No such task.
    RS_EOK -- succeed
Notes:
==============================================================*/
__fun_
status_t r_task_resume(prio_t prio) __ef_
{
    RSI_REG prio_t cprio;

    IRQ_SAVEAREA;

#if CFG_ARG_CHK > 1

    /* If the task priority in the limitation. */
    if (prio >= RSI_IDLE_PRIO) {

        /* No, the priority is exceed the limitation. */
        return RS_EOUTRANGE;
    }
#else
    RSI_ASSERT(prio < RSI_IDLE_PRIO);
#endif

    irq_lock();

    /* Determine Whether TCB is in effect. */
    if (__prio_isnormal(prio)) {

        /* Yes. Get current priority of task. */
        cprio = _TASKS[prio].prio.current;

        /* Yes. Clear suspend flag. */
        _TASKS[cprio].state &= ~RSI_STA_SUSPEND;

        /* Judge if task is in ready. */
        if ((_TASKS[cprio].state & RSI_STA_PREREADY) ||
            (!(_TASKS[cprio].state & RSI_STA_BLOCKED) &&
            _TASKS[cprio].delay == 0))
        {
            /* Make task ready. */

            /* Clear PREREADY falg. */
            _TASKS[cprio].state &= ~RSI_STA_PREREADY;

            /* Add task to ready queue. */
            __ready_que_insert(cprio);
        }

        irq_unlock();

        /* Tasks need to be scheduled. */
        __schedule();

        /* Return succeed. */
        return RS_EOK;
    }

    irq_unlock();

    /* No such task. Return failed. */
    return RS_ENOTEXIST;
}
#endif


#if CFG_TASK_INFO_EN > 0
/*==============================================================
Function: task_info()
Description: Retrieve a task information.
Parameters:
    info -- Pointer to task information struct
Returns:
    RS_EOUTRANGE -- Priority out of range.
    RS_ENOTEXIST -- No such task.
    RS_EOK -- succeed
Notes:
==============================================================*/
__fun_
status_t r_task_info(tinfo_t __p_* info) __ef_
{
    RSI_REG prio_t prio;
    RSI_REG sp_t sp_end;

    IRQ_SAVEAREA;

    /* Priority of task which information you care. */
    prio = info->prio;

#if RSI_IDLE_PRIO < 0xff && CFG_ARG_CHK > 1

    /* If the task priority in the limitation. */
    if (prio >= RSI_IDLE_PRIO && prio != RSI_SELF_PRIO) {

        /* No, the priority is exceed the limitation. */
        return RS_EOUTRANGE;
    }
#else
    RSI_ASSERT(prio < RSI_IDLE_PRIO || prio == RSI_SELF_PRIO);
#endif

    irq_lock();

    /* Whether priority indicates itself. */
    if (prio == RSI_SELF_PRIO) {

        /* Get current task priority. */
        prio = _current_prio;

        /* Save current priority. */
        info->prio = prio;

#if CFG_TASK_MODE <= 1

    /* If task is exist */
    } else if (!(_TASKS[prio].state & RSI_STA_READY)) {

        irq_unlock();

        /* No. Return failed. */
        return RS_ENOTEXIST;

#else

    /* Determine if priority is normal. */
    } else if (!__prio_isnormal(prio)) {

        /* No. */
        irq_unlock();
        /* No such task. Return failed. */
        return RS_ENOTEXIST;

    } else {

        /* Save normal priority of task. */
        info->prio = prio;

        /* Get running priority of task. */
        prio = _TASKS[prio].prio.current;

#endif
    }

    /* Get information of task. */
    info->state = _TASKS[prio].state;
    info->stack_base = _TASKS[prio].stack_base;
    info->stack_size = _TASKS[prio].stack_size;

#if CFG_OBJ_NAME_EN > 0

    /* Get task name. */
    info->name[0] = _TASKS[prio].name.c[0];
    info->name[1] = _TASKS[prio].name.c[1];
    info->name[2] = _TASKS[prio].name.c[2];
    info->name[3] = _TASKS[prio].name.c[3];

#else
    info->name[0] = 0;
    
#endif

    irq_unlock();

    /* Determine if task has a name. */
    if (info->name[0] == 0) {
        info->name[0] = RSI_NA0;
        info->name[1] = RSI_NA1;
        info->name[2] = RSI_NA2;
        info->name[3] = RSI_NA3;
    }

    info->name[4] = 0;

#if ARCH_STACK_GROW == UPWARDS

    /* Point to end address of task stack. */
    sp_end = info->stack_base + info->stack_size;

    /* Count unused space of stack. */
    while (*(--sp_end) == RSI_STACK_UNUSE);

    /* Calculate bytes of used space. */
    info->stack_used = sp_end - info->stack_base + 1;

    /* Calculate bytes of free space. */
    info->stack_free = info->stack_size - info->stack_used;

#else

    /* Point to end address of task stack. */
    sp_end = info->stack_base - 1;

    /* Count unused space of stack. */
    while (*(++sp_end) == RSI_STACK_UNUSE);

    /* Calculate bytes of free space. */
    info->stack_free = sp_end - info->stack_base;

    /* Calculate bytes of used space. */
    info->stack_used = info->stack_size - info->stack_free;

#endif


    /* Return succeed. */
    return RS_EOK;
}
#endif


#if CFG_TASK_MODE >= 2
/*==============================================================
Function: task_ident()
Description: Retrieve priority of task current executing.
Parameters:
Returns:
    priority of current task.
Notes:
    return value is the priority of the current task during the function 
    running. This return value is only for reference if the priority can be
    changed. Do trust to use task_self() as task control function.
==============================================================*/
__fun_
prio_t r_task_ident(void) __ef_
{
    RSI_REG prio_t prio;

    IRQ_SAVEAREA;

    irq_lock();

    /* Get normal priority of task current executing. */
    prio = _TASKS[_current_prio].prio.normal;

    irq_unlock();

    /* Return priority of task current executing. */
    return prio;
}
#endif


/*==============================================================
Function: task_current()
Description: Retrieve running priority of task current executing.
Parameters:
Returns:
    running priority of current task.
Notes:
    return value is the priority of the current task during the function 
    running. This return value is only for reference if the priority can be
    changed. Do trust to use task_self() as task control function.
==============================================================*/
__fun_
prio_t r_task_current(void) __ef_
{
    /* Return priority of task current executing. */
    return _current_prio;
}


#if RSI_DEF_TASK_LOCK > 0
/*==============================================================
Function: task_protect()
Description: Lock current executing task, prevent task be changed
             or deleted.
Parameters:
Returns:
Notes:
==============================================================*/
__fun_
void r_task_protect(void) __ef_
{
    IRQ_SAVEAREA;

    irq_lock();

    /* Lock current task. */
    __nprio_lock(_current_prio);

    irq_unlock();
}


/*==============================================================
Function: task_unprotect()
Description: Unlock current executing task.
Parameters:
Returns:
Notes:
==============================================================*/
__fun_
void r_task_unprotect(void) __ef_
{
    IRQ_SAVEAREA;

    irq_lock();

    /* Unlock current task. */
    __nprio_unlock(_current_prio);

    irq_unlock();
}
#endif


/*==============================================================
Function: __task_init()
Description: Initialize kernel task.
Parameters:
    arg -- Parameter of Idle task
Returns:
Notes:
    Internal function
==============================================================*/
__fun_
void __task_init(void) __ef_
{
    RSI_REG prio_t prio;

    /* Initialize task control blocks. */
    for (prio = 0; prio < RSI_IDLE_PRIO; prio++) {

        /* Clear task control block. */
        __tcb_zero(prio);
    }

    /* Initialize idle task control block. */
    _TASKS[RSI_IDLE_PRIO].state = RSI_STA_READY;

    /* Initialize priority of current executing task. */
    _current_prio = RSI_IDLE_PRIO;
    
    /* Insert idle task to ready queue. */
    __ready_que_insert(RSI_IDLE_PRIO);
}


/*===========================================================================*/


