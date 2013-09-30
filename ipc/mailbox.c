/*
===============================================================================
| COPYRIGHT (C) 2003-2007 Haishen Ruan, All rights reserved.
| SUMMARY:
|   Mailbox implementation.
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


#if CFG_MAILBOX_EN > 0


/*==============================================================
Function: mailbox_create()
Description: Initialize mail box.
Parameters:
    mbox -- pointer to mailbox
    name -- mailbox name
Returns:
    RS_EOK -- succeed
Notes:
==============================================================*/
#if CFG_OBJ_NAME_EN > 0

__fun_
status_t r_mailbox_create(mbox_t __p_* mbox, name_t name) __ef_

#else

__fun_
status_t r_mailbox_create(mbox_t __p_* mbox) __ef_

#endif
{
    RSI_ASSERT(mbox != NULL);

#if CFG_OBJ_NAME_EN > 0
    mbox->name.w = name;
#endif

    /* Initialize mailbox control block. */
    mbox->mail  = 0;
    mbox->avail = false;

    /* Initialize wait queue. */
    __cque_init(mbox->waits);

#if CFG_DEBUG > 0
    mbox->init = RSI_INITED;
#endif

    return RS_EOK;
}


/*==============================================================
Function: mailbox_send()
Description: send a message to the mailbox
Parameters:
    mbox -- pointer to mailbox
    mail -- message to send
Returns:
    RS_EFULL -- failed, mailbox is full
    RS_EOK -- succeed
Notes:
==============================================================*/
__fun_
status_t r_mailbox_send(mbox_t __p_* mbox, mail_t mail) __ef_
{
    IRQ_SAVEAREA;

    RSI_ASSERT(mbox != NULL);
    RSI_ASSERT(mbox->init == RSI_INITED);

    irq_lock();

    /* See if there is message in the mailbox */
    if (!mbox->avail) {

        /* No.
         * Copy message to mailbox. */
        mbox->avail = true;
        mbox->mail  = mail;

        /* If there is any thask in wait queue.
         * If has, wake up it */
        if (__ipc_resume(&mbox->waits) != RSI_NULL_PRIO) {

            irq_unlock();

            /* Tasks schedule. */
            __schedule();

        } else {
            irq_unlock();
        }

        /* Return succeed. */
        return RS_EOK;
    }

    irq_unlock();

    /* Mailbox is full, Return failed. */
    return RS_EFULL;
}


/*==============================================================
Function: mailbox_receive()
Description: receives a message from mailbox
Parameters:
    mbox -- pointer to mailbox
    pmail -- pointer to message of received
    ticks -- ticks to wait
Returns:
    RS_ELOCK -- kernel is locked
    RS_ETIMEOUT -- time out
    RS_EOK -- succeed
Notes:
    Avoid calling this function from interrupt routine.
==============================================================*/
#if CFG_IPC_TIMEOUT_EN > 0 || CFG_DEBUG > 0

__fun_
status_t r_mailbox_receive(
    mbox_t __p_*    mbox,
    mail_t __out_*  mail,
    tick_t ticks
    ) __ef_

#else

__fun_
status_t r_mailbox_receive(
    mbox_t __p_*    mbox,
    mail_t __out_*  mail
    ) __ef_

#endif
{
#if CFG_IPC_TIMEOUT_EN > 0
    RSI_REG prio_t current;
#endif

    IRQ_SAVEAREA;

    RSI_ASSERT(mbox != NULL);
    RSI_ASSERT(mbox->init == RSI_INITED);
    RSI_ASSERT(mail != NULL);

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

        /* See if there is message in the mailbox */
        if (mbox->avail) {

            /* Yes. Take a message in mailbox. */
            mbox->avail = false;
            *mail = mbox->mail;

            irq_unlock();

            /* Return succeed. */
            return RS_EOK;
        }


#if CFG_IPC_TIMEOUT_EN > 0

        /* Mailbox is empty, block current task. */
        __ipc_block(&mbox->waits, ticks);
#else

        /* Mailbox is empty, block current task. */
        __ipc_block(&mbox->waits, 0);
#endif
        irq_unlock();

        /* Tasks schedule. */
        __schedule();

#if CFG_IPC_TIMEOUT_EN > 0

        irq_lock();

        /* Retrieve priority of current task. */
        current = _current_prio;

        /* See if task is waked up because time out. */
        if (_TASKS[current].state & RSI_STA_BLOCKED) {

            /* Yes. Invoke IPC time out function. */
            __ipc_timeout(&mbox->waits);

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
Function: mailbox_flush()
Description: Clear message in mailbox.
Parameters:
    mbox -- pointer to mailbox
Returns:
    RS_EOK -- succeed
Notes:
==============================================================*/
__fun_
status_t r_mailbox_flush(mbox_t __p_* mbox) __ef_
{
    IRQ_SAVEAREA;

    RSI_ASSERT(mbox != NULL);
    RSI_ASSERT(mbox->init == RSI_INITED);

    irq_lock();

    /* Clear mailbox. */
    mbox->avail = false;
    mbox->mail  = 0;

    irq_unlock();

    /* Return succeed. */
    return RS_EOK;
}


/*==============================================================
Function: mailbox_delete()
Description: Destroy a mail box
Parameters:
    mbox -- pointer to mailbox
Returns:
    RS_EEXIST -- there are tasks waiting message
    RS_EOK -- succeed
Notes:
    If there are tasks waiting mail box, the mail box can not
    be destroy.
==============================================================*/
__fun_
status_t r_mailbox_delete(mbox_t __p_* mbox) __ef_
{
    IRQ_SAVEAREA;

    RSI_ASSERT(mbox != NULL);
    RSI_ASSERT(mbox->init == RSI_INITED);

    irq_lock();

    /* If there is any task waiting mail box. */
    if (__cque_is1(mbox->waits)) {

        /* Yes. Mail box can not be destroy. */

        irq_unlock();

        /* Return failed. */
        return RS_EEXIST;
    }

    irq_unlock();

#if CFG_DEBUG > 0
    mbox->init = 0;
#endif

    /* Return succeed. */
    return RS_EOK;
}
#endif


/*===========================================================================*/


