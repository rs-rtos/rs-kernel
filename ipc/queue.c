/*
===============================================================================
| COPYRIGHT (C) 2003-2007 Haishen Ruan, All rights reserved.
| SUMMARY:
|   Message queue implementation
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


#if CFG_QUEUE_EN > 0


/* Take a message from head of queue. */
#define __queue_take_head(queue, buff) \
do { \
    memcpy(buff, queue->phead, queue->msgsz); \
    queue->phead += queue->msgsz; \
    if (queue->phead == queue->pend) { \
        queue->phead = queue->pstart; \
    } \
} while (0)


/* Take a message from tail of queue. */
#define __queue_take_tail(queue, buff) \
do { \
    if (queue->ptail == queue->pstart) { \
        queue->ptail = queue->pend; \
    } \
    queue->ptail -= queue->msgsz; \
    memcpy(buff, queue->ptail, queue->msgsz); \
} while (0)


/*==============================================================
Function: queue_create()
Description: Initialize message queue.
Parameters:
    queue -- pointer to message queue
    name -- message queue's name
    size -- size of a message
    entries -- entries of messages in queue
    options -- message queue options
Returns:
    RS_ENULL -- stack address is unspecified. or memory is lack.
    RS_EOK -- succeed
Notes:
==============================================================*/
#if CFG_OBJ_NAME_EN > 0

__fun_
status_t r_queue_create(
    queue_t __p_*   queue,
    name_t          name,
    void    __p_*   buff,
    mmsz_t          size,
    mmsz_t          entries,
    int8u           options
    ) __ef_
    
#else

__fun_
status_t r_queue_create(
    queue_t __p_*   queue,
    void    __p_*   buff,
    mmsz_t          size,
    mmsz_t          entries,
    int8u           options
    ) __ef_
    
#endif
{
    RSI_ASSERT(queue != NULL);
    RSI_ASSERT(size > 0);
    RSI_ASSERT(entries > 0);

    /* If buffer pointer is not empty. */
    if (buff != NULL) {

        /* Specified queue buffer address. */
        queue->pstart = buff;

#if CFG_MPOOL_EN > 0

    } else {

        /* Allocate a block of memory on the heap. */
        queue->pstart = mpool_alloc(size * entries);

        /* If allocate memory successfully. */
        if (queue->pstart == NULL) {

            /* No. Memory is lack, return failed. */
            return RS_ENULL;
        }

        /* Indicate buffer should be free by message queue itself. */
        options |= RS_OPT_KFREE;
#else

    } else {

        /* Buffer pointer unspecified, Return failed. */
        return RS_ENULL;
#endif
    }

#if CFG_OBJ_NAME_EN > 0
    queue->name.w = name;
#endif

    /* Initialize message queue control block. */
    queue->phead =
        queue->ptail = queue->pstart;
    queue->pend      = queue->pstart + size * entries;
    queue->msgsz     = size;
    queue->entries   = entries;
    queue->options   = options;
    queue->avail     = 0;

    /* Initialize message queue wait queue. */
    __cque_init(queue->waits);

#if CFG_DEBUG > 0
    /* Initialized flag. */
    queue->init = RSI_INITED;
#endif

    /* Return succeed. */
    return RS_EOK;
}


/*==============================================================
Function: queue_send()
Description: Send a message to the message queue.
Parameters:
    queue -- pointer to message queue
    buff -- pointer to message buffer
Returns:
    RS_EFULL -- message queue is full
    RS_EOK -- succeed
Notes:
==============================================================*/
__fun_
status_t r_queue_send(queue_t __p_* queue, const void __p_* buff) __ef_
{
    RSI_REG int32u __p_* source;
    RSI_REG int32u __p_* destination;
    RSI_REG uint scheded = 0;

    IRQ_SAVEAREA;

    RSI_ASSERT(queue != NULL);
    RSI_ASSERT(queue->init == RSI_INITED);
    RSI_ASSERT(buff != NULL);

    irq_lock();

    /* Determine if there is enough room in the queue for the message. */
    if (queue->avail < queue->entries) {

        /* Yes. Copy message to queue. */
        /* memcpy(queue->ptail, buff, queue->msgsz); */

        /* Retrieve destination pointer. */
        destination = (int32u __p_*)queue->ptail;

        /* Adjust tail of queue pointer. */
        queue->ptail += queue->msgsz;

        /* Determine if queue tail is arrive at end of queue buffer. */
        if (queue->ptail == queue->pend) {

            /* Yes. Adjust pointer to start of queue buffer. */
            queue->ptail = queue->pstart;
        }

        /* Increase No. of message in queue,
         * and judge there is any task waiting in queue. */
        if (++queue->avail == 1 &&
            __ipc_resume(&queue->waits) != RSI_NULL_PRIO)
        {
            /* Tasks need schedule. */
            scheded = 1;
        }

        /* Lock kernel. */
        __lock_k();

        irq_unlock();

        /* Retrieve source pointer. */
        source = (int32u __p_*)buff;

        /* Copy the message to queue. */
        if (queue->msgsz == RS_MSG_1ULONG) {
        
            /* Copy a single long word message to queue. */
            *destination = *source;

        } else if (queue->msgsz == RS_MSG_2ULONG) {

            /* Copy a double long word message to queue. */
            *destination++ = *source++;
            *destination = *source;

        } else if (queue->msgsz == RS_MSG_4ULONG) {

            /* Copy a four long word message to queue. */
            *destination++ = *source++;
            *destination++ = *source++;
            *destination++ = *source++;
            *destination = *source;

        } else if (queue->msgsz == RS_MSG_8ULONG) {

            /* Copy an eight long word message to queue. */
            *destination++ = *source++;
            *destination++ = *source++;
            *destination++ = *source++;
            *destination++ = *source++;
            *destination++ = *source++;
            *destination++ = *source++;
            *destination++ = *source++;
            *destination = *source;

        } else {
        
            /* Copy message to queue. */
            memcpy(destination, source, queue->msgsz);
        }

        /* Unlock kernel. */
        __unlock_k();

        /* Determine if tasks need schedule. */
        if (scheded) {

            /* Yes. schedule tasks. */
            __schedule();
        }
        
        /* Return succeed. */
        return RS_EOK;
    }

    irq_unlock();

    /* Message queue is full. Return failed. */
    return RS_EFULL;
}


/*==============================================================
Function: queue_receive()
Description: Receive a message from queue.
Parameters:
    queue -- pointer to message queue
    buff -- pointer to message buffer
    ticks -- ticks to wait
Returns:
    RS_ELOCK -- kernel is locked.
    RS_ETIMEOUT -- time out
    RS_EOK -- succeed
Notes:
    Avoid calling this function from interrupt routine.
==============================================================*/
#if CFG_IPC_TIMEOUT_EN > 0 || CFG_DEBUG > 0

__fun_
status_t r_queue_receive(
    queue_t __p_*   queue,
    void    __out_* buff,
    tick_t          ticks
    ) __ef_

#else

__fun_
status_t r_queue_receive(
    queue_t __p_*   queue,
    void    __out_* buff
    ) __ef_

#endif
{
#if CFG_IPC_TIMEOUT_EN > 0
    RSI_REG prio_t current;
#endif
    RSI_REG int32u __p_* source;
    RSI_REG int32u __p_* destination;

    IRQ_SAVEAREA;

    RSI_ASSERT(queue != NULL);
    RSI_ASSERT(queue->init == RSI_INITED);
    RSI_ASSERT(buff != NULL);

#if CFG_ARG_CHK > 0

    /* Determine if kernel is locked. */
    if (_sched_lock > 0) {

        /* Yes. Return failed. */
        return RS_ELOCK;
    }
#else
    RSI_ASSERT(_sched_lock == 0);
#endif

    for (;;) {

        irq_lock();

        /* See if any message in the queue */
        if (queue->avail > 0) {

            /* Yes. Decrease No. of message in queue. */
            queue->avail--;

            /* Message queue options. */
            if (queue->options & RS_OPT_LIFO) {

                /* Last in first out.
                 * Take a message from tail of queue. */
                /* __queue_take_tail(queue, buff); */

                /* Adjust tail pointer of queue. */
                if (queue->ptail == queue->pstart) {
                    queue->ptail = queue->pend;
                }
                queue->ptail -= queue->msgsz;

                /* Retrieve source pointer. */
                source = (int32u __p_*)queue->ptail;

            } else {

                /* First in first out.
                 * Take a message from head of queue. */
                /* __queue_take_head(queue, buff); */

                /* Retrieve source pointer. */
                source = (int32u __p_*)queue->phead;

                /* Adjust head pointer of queue. */
                queue->phead += queue->msgsz;
                if (queue->phead == queue->pend) {
                    queue->phead = queue->pstart;
                }
            }

            /* Lock kernel. */
            __lock_k();

            irq_unlock();

            /* Retrieve destination pointer. */
            destination = (int32u __p_*)buff;

            /* Copy the message to queue. */
            if (queue->msgsz == RS_MSG_1ULONG) {
            
                /* Copy a single long word message to queue. */
                *destination = *source;

            } else if (queue->msgsz == RS_MSG_2ULONG) {

                /* Copy a double long word message to queue. */
                *destination++ = *source++;
                *destination = *source;

            } else if (queue->msgsz == RS_MSG_4ULONG) {

                /* Copy a four long word message to queue. */
                *destination++ = *source++;
                *destination++ = *source++;
                *destination++ = *source++;
                *destination = *source;

            } else if (queue->msgsz == RS_MSG_8ULONG) {

                /* Copy an eight long word message to queue. */
                *destination++ = *source++;
                *destination++ = *source++;
                *destination++ = *source++;
                *destination++ = *source++;
                *destination++ = *source++;
                *destination++ = *source++;
                *destination++ = *source++;
                *destination = *source;

            } else {
            
                /* Copy message to queue. */
                memcpy(destination, source, queue->msgsz);
            }

            /* Unlock kernel. */
            __unlock_k();
        
            /* Return succeed. */
            return RS_EOK;
        }


#if CFG_IPC_TIMEOUT_EN > 0

        /* Message queue is empty, block current task. */
        __ipc_block(&queue->waits, ticks);
#else

        /* Message queue is empty, block current task. */
        __ipc_block(&queue->waits, 0);
#endif
        irq_unlock();

        /* Tasks schedule. */
        __schedule();

#if CFG_IPC_TIMEOUT_EN > 0

        irq_lock();

        /* Get priority of current executing task. */
        current = _current_prio;

        /* See if task is waked up because time out. */
        if (_TASKS[current].state & RSI_STA_BLOCKED) {

            /* Yes. Invoke IPC time out function. */
            __ipc_timeout(&queue->waits);

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
Function: queue_flush()
Description: Clear all of message in queue.
Parameters:
    queue -- pointer to message queue
Returns:
    RS_EOK -- succeed
Notes:
==============================================================*/
__fun_
status_t r_queue_flush(queue_t __p_* queue) __ef_
{
    IRQ_SAVEAREA;

    RSI_ASSERT(queue != NULL);
    RSI_ASSERT(queue->init == RSI_INITED);

    irq_lock();

    /* Adjust pointer */
    queue->phead =
        queue->ptail = queue->pstart;

    /* Clear No. of message in queue. */
    queue->avail = 0;

    irq_unlock();

    /* Return succeed. */
    return RS_EOK;
}


/*==============================================================
Function: queue_delete()
Description: Destroy a message queue.
Parameters:
    queue -- pointer to message queue
Returns:
    RS_EEXIST -- there are tasks waiting message
    RS_EOK -- succeed
Notes:
    If there are tasks waiting message queue, the queue can not
    be destroy.
==============================================================*/
__fun_
status_t r_queue_delete(queue_t __p_* queue) __ef_
{
    IRQ_SAVEAREA;

    RSI_ASSERT(queue != NULL);
    RSI_ASSERT(queue->init == RSI_INITED);

    irq_lock();

    /* If there is any task waiting message. */
    if (__cque_is1(queue->waits)) {

        /* Yes. message queue can not be destroy. */

        irq_unlock();

        /* Return failed. */
        return RS_EEXIST;
    }

    irq_unlock();

#if CFG_MPOOL_EN > 0

    /* Here enable interrupts briefly */
    __dummy();

    irq_lock();

    /* Judge if need free queue buffer. */
    if (queue->options & RS_OPT_KFREE) {

        /* Yes. */
        queue->options = 0;

        /* Free the buffer of queue. */
        mpool_free(queue->pstart);
    }

    irq_unlock();
#endif

#if CFG_DEBUG > 0
    /* Initialized flag. */
    queue->init = 0;
#endif

    /* Return succeed. */
    return RS_EOK;
}
#endif


/*===========================================================================*/


