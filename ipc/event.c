/*
===============================================================================
| COPYRIGHT (C) 2003-2007 Haishen Ruan, All rights reserved.
| SUMMARY:
|   Event group implementation.
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


#if CFG_EVENT_EN > 0

#define RSI_XAND_OPERATE    RS_OPT_XOR + 1
#define RSI_XOR_OPERATE     RS_OPT_XOR + 2

/*==============================================================
Function: event_create()
Description: Initialize event
Parameters:
    event -- pointer to event
    name -- event name
    bits -- initialized bits
Returns:
    RS_EOK -- succeed
Notes:
==============================================================*/
#if CFG_OBJ_NAME_EN > 0

__fun_
status_t r_event_create(
    event_t __p_*   event,
    name_t          name,
    bits_t          bits
    ) __ef_

#else

__fun_
status_t r_event_create(
    event_t __p_*   event,
    bits_t          bits
    ) __ef_

#endif
{
    RSI_ASSERT(event != NULL);

#if CFG_OBJ_NAME_EN > 0
    event->name.w = name;
#endif

    /* Initialize event control block. */
    event->bits = bits;

#if CFG_EVENT_EN == 2
    event->cares = 0;
#endif

    /* Initialize event wait queue. */
    __cque_init(event->waits);

#if CFG_DEBUG > 0
    event->init = RSI_INITED;
#endif

    return RS_EOK;
}


/*==============================================================
Function: event_wait()
Description: Wait until a combination of specified events have occurred.
Parameters:
    event -- pointer to event
    bits -- events to wait
    options -- event options
    ticks -- ticks to wait, 0 indicates wait forever.
             if configure CFG_IPC_TIMEOUT_EN == 0, ticks is ignored.
Returns:
    RS_ELOCK -- kernel is locked
    RS_ETIMEOUT -- time out
    RS_EINVAL -- bits or options is invalid
    RS_EOK -- succeed
Notes:
    Avoid calling this function from interrupt routine.
==============================================================*/
#if CFG_IPC_TIMEOUT_EN > 0 || CFG_DEBUG > 0

__fun_
status_t r_event_wait(
    event_t __p_*   event,
    bits_t          bits,
    int8u           options,
    tick_t          ticks
    ) __ef_
    
#else

__fun_
status_t r_event_wait(
    event_t __p_*   event,
    bits_t          bits,
    int8u           options
    ) __ef_

#endif
{
#if CFG_IPC_TIMEOUT_EN > 0
    RSI_REG prio_t current;
#endif
    RSI_REG bool consume = false;
    RSI_REG bits_t tarev = 0;

    IRQ_SAVEAREA;

    RSI_ASSERT(event != NULL);
    RSI_ASSERT(event->init == RSI_INITED);

#if CFG_ARG_CHK > 0

    /* Determine if kernel is locked. */
    if (_sched_lock > 0) {

        /* Yes. Return failed. */
        return RS_ELOCK;
    }

    /* Determine if bits is zero. */
    if (bits == 0) {

        /* Yes. Return failed. */
        return RS_EINVAL;
    }
#else
    RSI_ASSERT(_sched_lock == 0);
    RSI_ASSERT(bits != 0);
#endif


    /* Save CONSUME option. */
    if (options & RS_OPT_CONSUME) {
        options &= ~RS_OPT_CONSUME;
        consume = true;
    }

    for (;;) {

        /* Judge by option. */
        switch (options) {

        /* All bits in 'bits' to be set */
        case RS_OPT_AND:

            irq_lock();

            /* See if all specified bits have occurred. */
            if ((event->bits & bits) == bits) {

                /* Yes. If CONSUME is specified.
                 * reset all specified bits. */
                if (consume) {
                    event->bits &= ~bits;
                }

                irq_unlock();

                /* Return succeed. */
                return RS_EOK;
            }
            break;

        /* Any bit in 'bits' to be set */
        case RS_OPT_OR:

            irq_lock();

            /* See if any specified bits has occurred. */
            if (event->bits & bits) {

                /* Yes. If CONSUME is specified.
                 * reset all specified bits. */
                if (consume) {
                    event->bits &= ~bits;
                }

                irq_unlock();

                /* Return succeed. */
                return RS_EOK;
            }
            break;

        /* All bits in 'bits' to be clear */
        case RS_OPT_NAND:

            irq_lock();

            /* See if all specified bits have clear. */
            if (((event->bits & bits) ^ bits) == bits) {

                /* Yes. If CONSUME is specified.
                 * reset all specified bits. */
                if (consume) {
                    event->bits &= ~bits;
                }

                irq_unlock();

                /* Return succeed. */
                return RS_EOK;
            }
            break;

        /* Any bit in 'bits' to be clear */
        case RS_OPT_NOR:

            irq_lock();

            /* See if any specified bits has clear. */
            if ((event->bits & bits) ^ bits) {

                /* Yes. If CONSUME is specified.
                 * reset all specified bits. */
                if (consume) {
                    event->bits &= ~bits;
                }

                irq_unlock();

                /* Return succeed. */
                return RS_EOK;
            }
            break;

        /* Step 1: All bits in 'bits' to be reversed */
        case RS_OPT_XAND:

            irq_lock();

            /* Get the target event. */
            tarev = ~event->bits & bits;
            options = RSI_XAND_OPERATE;

            break;

        /* Step 2: All bits in 'bits' to be reversed */
        case RSI_XAND_OPERATE:

            irq_lock();

            /* See if all specified bits have reversed. */
            if ((event->bits & bits) == tarev) {

                /* Yes. If CONSUME is specified.
                 * reset all specified bits. */
                if (consume) {
                    event->bits &= ~bits;
                }

                irq_unlock();

                /* Return succeed. */
                return RS_EOK;
            }
            break;

        /* Step 1: Any bit in 'bits' to be reversed */
        case RS_OPT_XOR:

            irq_lock();

            /* Get the target event. */
            tarev = event->bits & bits;
            options = RSI_XOR_OPERATE;

            break;

        /* Step 2: Any bit in 'bits' to be reversed */
        case RSI_XOR_OPERATE:

            irq_lock();

            /* See if any specified bits has reversed. */
            if ((event->bits & bits) != tarev) {

                /* Yes. If CONSUME is specified.
                 * reset all specified bits. */
                if (consume) {
                    event->bits &= ~bits;
                }

                irq_unlock();

                /* Return succeed. */
                return RS_EOK;
            }
            break;

        default:
            /* Invaild options, Return failed. */
            return RS_EINVAL;
        }


#if CFG_EVENT_EN == 2

        /* Save all all specified bits. */
        event->cares |= bits;
#endif


#if CFG_IPC_TIMEOUT_EN > 0

        /* Specified events have not occurred, block current task. */
        __ipc_block(&event->waits, ticks);
#else

        /* Specified events have not occurred, block current task. */
        __ipc_block(&event->waits, 0);
#endif
        irq_unlock();

        /* Tasks schedule. */
        __schedule();


#if CFG_IPC_TIMEOUT_EN > 0

        irq_lock();

        /* Get priority of current task. */
        current = _current_prio;

        /* See if task is waked up because time out. */
        if (_TASKS[current].state & RSI_STA_BLOCKED) {

            /* Yes. Invoke IPC time out function. */
            __ipc_timeout(&event->waits);

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

    /* Retuen succeed. */
    return RS_EOK;
}


/*==============================================================
Function: event_post()
Description: Set or clear specified events.
Parameters:
    event -- pointer to event
    bits -- events to set or clear
    options -- event options
Returns:
    RS_EINVAL -- Invaild options
    RS_EOK -- succeed
Notes:
==============================================================*/
__fun_
status_t r_event_post(
    event_t __p_*   event,
    bits_t          bits,
    int8u           options
    ) __ef_
{
    RSI_REG prio_t prio;

    IRQ_SAVEAREA;

    RSI_ASSERT(event != NULL);
    RSI_ASSERT(event->init == RSI_INITED);

    switch (options) {

    /* Set bits */
    case RS_OPT_SET:

        /* Kernel lock, To avoid lower priority task preempt
         * high priority task that waiting the specified events. */
        __lock_k();

        irq_lock();

        /* Set specified bits. */
        event->bits |= bits;

        irq_unlock();
        break;

    /* Clear bits */
    case RS_OPT_CLR:

        /* Kernel lock, To avoid lower priority task preempt
         * high priority task that waiting the specified events. */
        __lock_k();

        irq_lock();

        /* Clear specified bits. */
        event->bits &= ~bits;

        irq_unlock();
        break;

    default:
        /* Invaild options, Return failed. */
        return RS_EINVAL;
    }


#if CFG_EVENT_EN == 2

    /* If there is any thask care about bits. */
    if (bits & event->cares) {

        /* Clear care bits. */
        event->cares = 0;
#endif
        do {
            irq_lock();

            /* Wake up tasks in wait queue. */
            prio = __ipc_resume(&event->waits);

            irq_unlock();

        } while (prio != RSI_NULL_PRIO);

#if CFG_EVENT_EN == 2
    }
#endif

    /* Kernel unlock. */
    __unlock_k();

    /* Tasks schedule. */
    __schedule();

    return RS_EOK;
}


/*==============================================================
Function: event_trywait()
Description: Receive a combination of specified events have occurred
             dispense with wait.
Parameters:
    event -- pointer to event group
    bits -- events to wait
    options -- event options
Returns:
    RS_EINVAL -- invalid options or bits
    RS_ENAVAIL -- specified bits are not occurred.
    RS_EOK -- succeed
Notes:
==============================================================*/
__fun_
status_t r_event_trywait(
    event_t __p_*   event,
    bits_t          bits,
    int8u           options
    ) __ef_
{
    RSI_REG bool consume = false;
    RSI_REG bits_t tarev = 0;

    IRQ_SAVEAREA;

    RSI_ASSERT(event != NULL);
    RSI_ASSERT(event->init == RSI_INITED);

#if CFG_ARG_CHK > 0

    /* Determine if bits is zero. */
    if (bits == 0) {

        /* Yes. Return failed. */
        return RS_EINVAL;
    }
#else
    RSI_ASSERT(bits != 0);
#endif


    /* Save CONSUME option. */
    if (options & RS_OPT_CONSUME) {
        options &= ~RS_OPT_CONSUME;
        consume = true;
    }

    /* Judge by option. */
    switch (options) {

    /* All bits in 'bits' to be set */
    case RS_OPT_AND:

        irq_lock();

        /* See if all specified bits have occurred. */
        if ((event->bits & bits) == bits) {

            /* Yes. If CONSUME is specified.
             * reset all specified bits. */
            if (consume) {
                event->bits &= ~bits;
            }

            irq_unlock();

            /* Return succeed. */
            return RS_EOK;
        }
        break;

    /* Any bit in 'bits' to be set */
    case RS_OPT_OR:

        irq_lock();

        /* See if any specified bits has occurred. */
        if (event->bits & bits) {

            /* Yes. If CONSUME is specified.
             * reset all specified bits. */
            if (consume) {
                event->bits &= ~bits;
            }

            irq_unlock();

            /* Return succeed. */
            return RS_EOK;
        }
        break;

    /* All bits in 'bits' to be clear */
    case RS_OPT_NAND:

        irq_lock();

        /* See if all specified bits have clear. */
        if (((event->bits & bits) ^ bits) == bits) {

            /* Yes. If CONSUME is specified.
             * reset all specified bits. */
            if (consume) {
                event->bits &= ~bits;
            }

            irq_unlock();

            /* Return succeed. */
            return RS_EOK;
        }
        break;

    /* Any bit in 'bits' to be clear */
    case RS_OPT_NOR:

        irq_lock();

        /* See if any specified bits has clear. */
        if ((event->bits & bits) ^ bits) {

            /* Yes. If CONSUME is specified.
             * reset all specified bits. */
            if (consume) {
                event->bits &= ~bits;
            }

            irq_unlock();

            /* Return succeed. */
            return RS_EOK;
        }
        break;

    /* Step 1: All bits in 'bits' to be reversed */
    case RS_OPT_XAND:

        irq_lock();

        /* Get the target event. */
        tarev = ~event->bits & bits;
        options = RSI_XAND_OPERATE;

        break;

    /* Step 2: All bits in 'bits' to be reversed */
    case RSI_XAND_OPERATE:

        irq_lock();

        /* See if all specified bits have reversed. */
        if ((event->bits & bits) == tarev) {

            /* Yes. If CONSUME is specified.
             * reset all specified bits. */
            if (consume) {
                event->bits &= ~bits;
            }

            irq_unlock();

            /* Return succeed. */
            return RS_EOK;
        }
        break;

    /* Step 1: Any bit in 'bits' to be reversed */
    case RS_OPT_XOR:

        irq_lock();

        /* Get the target event. */
        tarev = event->bits & bits;
        options = RSI_XOR_OPERATE;

        break;

    /* Step 2: Any bit in 'bits' to be reversed */
    case RSI_XOR_OPERATE:

        irq_lock();

        /* See if any specified bits has reversed. */
        if ((event->bits & bits) != tarev) {

            /* Yes. If CONSUME is specified.
             * reset all specified bits. */
            if (consume) {
                event->bits &= ~bits;
            }

            irq_unlock();

            /* Return succeed. */
            return RS_EOK;
        }
        break;

    default:
        /* Invaild options, Return failed. */
        return RS_EINVAL;
    }

    irq_unlock();

    /* Specified events not match. Return failed. */
    return RS_ENAVAIL;
}


/*==============================================================
Function: event_info()
Description: Get event infomation.
Parameters:
    event -- pointer to event
    info -- pointer to event information struct
Returns:
    RS_EOK -- succeed
Notes:
==============================================================*/
__fun_
status_t r_event_info(event_t __p_* event, evinfo_t __out_* info) __ef_
{
    IRQ_SAVEAREA;

    RSI_ASSERT(event != NULL);
    RSI_ASSERT(event->init == RSI_INITED);

#if CFG_OBJ_NAME_EN > 0

    /* Get event name. */
    info->name[0] = event->name.c[0];
    info->name[1] = event->name.c[1];
    info->name[2] = event->name.c[2];
    info->name[3] = event->name.c[3];

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

    irq_lock();

    /* Return event bits. */
    info->bits = event->bits;

    irq_unlock();

    /* Return succeed. */
    return RS_EOK;
}


/*==============================================================
Function: event_delete()
Description: Destroy event
Parameters:
    event -- pointer to event
Returns:
    RS_EEXIST -- there are tasks waiting event
    RS_EOK -- succeed
Notes:
    If there are tasks waiting event, the event can not be destroy.
==============================================================*/
__fun_
status_t r_event_delete(event_t __p_* event) __ef_
{
    IRQ_SAVEAREA;

    RSI_ASSERT(event != NULL);
    RSI_ASSERT(event->init == RSI_INITED);

    irq_lock();

    /* If there is any task waiting mail box. */
    if (__cque_is1(event->waits)) {

        /* Yes. Event can not be destroy. */

        irq_unlock();

        /* Return failed. */
        return RS_EEXIST;
    }

    irq_unlock();

#if CFG_DEBUG > 0
    event->init = 0;
#endif

    /* Return succeed. */
    return RS_EOK;
}
#endif


/*===========================================================================*/


