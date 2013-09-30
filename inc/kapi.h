/*
===============================================================================
| COPYRIGHT (C) 2003-2007 Haishen Ruan, All rights reserved.
| SUMMARY:
|   Define the function prototypes of the RS-KERNEL API.
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
#ifndef __KAPI_H__
#define __KAPI_H__

#ifdef __cplusplus
extern "C" {
#endif
/*===========================================================================*/
#include "arch/arch.h"
#include "inc/const.h"
#include "inc/cque.h"
#include "inc/kernel.h"
#include "inc/memory.h"
#include "inc/ipc.h"
#include "inc/device.h"
#include "shell/shell.h"


/*== Define all API Options =============================*/

/* Task control API options */
#define RS_OPT_SUSPEND      0x01    /* Created in suspended state */
#define RS_OPT_KFREE        0x80    /* free memory automatically */


/* Timer control API options */
#define RS_OPT_ENABLE       0x00    /* Enable timer */
#define RS_OPT_DISABLE      0x01    /* Diable timer */
#define RS_OPT_REPEAT       0x02    /* Repeated timer */
#define RS_OPT_SINGLE       0x04    /* Single timer */


/* Message queue API options */
#define RS_OPT_FIFO         0x00    /* First in first out */
#define RS_OPT_LIFO         0x01    /* Last in first out */
#undef  RS_OPT_KFREE
#define RS_OPT_KFREE        0x80    /* free memory automatically */


/* Event API options */
#define RS_OPT_AND          0x01    /* All bits are set */
#define RS_OPT_OR           0x02    /* Any bit is set */
#define RS_OPT_NAND         0x03    /* All bits are clear */
#define RS_OPT_NOR          0x04    /* Any bit is clear */
#define RS_OPT_XAND         0x05    /* All bits are reverse */
#define RS_OPT_XOR          0x06    /* Any bit is reverse */

#define RS_OPT_SET          0x01    /* Set bits */
#define RS_OPT_CLR          0x02    /* Clear bits */

#define RS_OPT_CONSUME      0x80    /* Consume bits */


/* Readline API options */
#define RS_OPT_ECHO         0x01    /* Echo input character */
#define RS_OPT_HISTORY      0x02    /* Echo input character */


/*== Define macros for IPC wait =========================*/

#define RS_WAIT_FOREVER     0


/*== Common message queue size ==========================*/

#define RS_MSG_1ULONG       (4 * 1)
#define RS_MSG_2ULONG       (4 * 2)
#define RS_MSG_4ULONG       (4 * 4)
#define RS_MSG_8ULONG       (4 * 8)


/*== Define NULL priority ===============================*/

/* Indicate priority is NULL. */
#define RS_PRIO_NULL        255


/*== Define error no. ===================================*/

enum {
        RS_EOK              = 0,    /* no error */
        RS_EUNKNOW          = -1,   /* unknow error */
        RS_EEXIST           = -2,   /* already exists */
        RS_ENOTEXIST        = -3,   /* not exists */
        RS_EOUTRANGE        = -4,   /* out of range */
        RS_EINVAL           = -5,   /* invalid argument */
        RS_ENAVAIL          = -6,   /* not available */
        RS_EAGAIN           = -7,   /* not allow operation again */
        RS_ENULL            = -8,   /* pointer to NULL */
        RS_ETIMEOUT         = -9,   /* time out */
        RS_ELOCK            = -10,  /* schedule lock */
        RS_EFULL            = -11,  /* object is full */
        RS_EMMCHK           = -12   /* memory check error */
};


/*== Application define =================================*/

/* Define Application functions.
 * These functions are invoked by kernel, must implemented by application.
 * and should never invoked by application directly. */
void hardware_initialize(void);
void application_initialize(void);


/*== Kernel API =========================================*/

/* Define Kernel functions. */

name_t build_name(char c1, char c2, char c3, char c4);

int16u  kernel_version(void);
char __p_* kernel_version_s(void);
void kernel_lock(void);
void kernel_unlock(void);
uint kernel_islock(void);

#if CFG_SYS_INFO_EN > 0

status_t kernel_info(kinfo_t __p_* info);

#endif


/*== Tick API ===========================================*/

/* Define Tick management functions. */

#if CFG_TICK_COUNT_EN > 0

int32u  tick_get(void);
void tick_set(int32u ticks);

#endif


/*== Interrupt API ======================================*/

/* Define Interrupt management functions. */

#if CFG_INT_VECTSZ > 0

ientry_t interrupt_attach(vect_t vect, ientry_t ientry);
ientry_t interrupt_detach(vect_t vect);
status_t interrupt_enable(vect_t vect);
status_t interrupt_disable(vect_t vect);
status_t interrupt_catch(vect_t vect);

inake_t intnake_attach(vect_t vect, inake_t ientry);
inake_t intnake_detach(vect_t vect);
status_t intnake_enable(vect_t vect);
status_t intnake_disable(vect_t vect);
status_t intnake_catch(vect_t vect);


#endif


/*== Timer API ==========================================*/

/* Define Timer management functions. */

#if CFG_TIMER_EN > 0

status_t timer_create(
        timer_t __p_*   timer,
        name_t          name,
        entry_t         entry,
        arg_t           arg,
        tick_t          ticks,
        int8u           options
        );

status_t timer_delete(timer_t __p_* timer);
status_t timer_enable(timer_t __p_* timer);
status_t timer_disable(timer_t __p_* timer);
status_t timer_control(timer_t __p_* timer, int8u options);


#endif


/*== Task API ===========================================*/

/* Define Task control functions. */

status_t task_create(
        prio_t          prio,           /* Task priority        */
        name_t          name,           /* Optional task name   */
        entry_t         entry,          /* Entry point function */
        arg_t           arg,            /* Entry data           */
        sp_t            stack_base,     /* Stack base           */
        mmsz_t          stack_size,     /* Stack size           */
        int8u           options         /* Task start options   */
        );


#if CFG_TASK_DELETE_EN > 0

status_t task_delete(prio_t prio);

#endif


#if CFG_TASK_MODE > 0

status_t task_change_prio(prio_t prio, prio_t new_prio);

#endif


#if CFG_TASK_MODE > 1

status_t task_change_runprio(prio_t prio, prio_t run_prio);
status_t task_reset_runprio(prio_t prio);

#endif


status_t task_sleep(tick_t ticks);
status_t task_wake(prio_t prio);


#if CFG_TASK_SUSPEND_EN > 0

status_t task_suspend(prio_t prio);
status_t task_resume(prio_t prio);

#endif


#if CFG_TASK_INFO_EN > 0

status_t task_info(tinfo_t __p_* info);

#endif


prio_t task_current(void);
prio_t task_ident(void);
prio_t task_self(void);

void task_protect(void);
void task_unprotect(void);


/*== Binary semaphores API ==============================*/

/* Define Binary semaphore management functions. */

#if CFG_SEMBINARY_EN > 0

status_t sembinary_create(semb_t __p_* semb, name_t name, bool value);
status_t sembinary_wait(semb_t __p_* semb, tick_t ticks);
status_t sembinary_trywait(semb_t __p_* semb);
status_t sembinary_post(semb_t __p_* semb);
status_t sembinary_info(semb_t __p_* semb, sembinfo_t __out_* info);
status_t sembinary_delete(semb_t __p_* semb);


#endif


/*== Counting semaphores API ============================*/

/* Define Counting semaphore management functions. */

#if CFG_SEMAPHORE_EN > 0

status_t semaphore_create(sem_t __p_* sem, name_t name, count_t value);
status_t semaphore_wait(sem_t __p_* sem, tick_t ticks);
status_t semaphore_trywait(sem_t __p_* sem);
status_t semaphore_post(sem_t __p_* sem);
status_t semaphore_info(sem_t __p_* sem, seminfo_t __out_* info);
status_t semaphore_delete(sem_t __p_* sem);


#endif


/*== Mutex API ==========================================*/

/* Define Mutex management functions. */

#if CFG_MUTEX_EN > 0

status_t mutex_create(mutex_t __p_* mut, name_t name, prio_t prio);
status_t mutex_wait(mutex_t __p_* mut, tick_t ticks);
status_t mutex_trywait(mutex_t __p_* mut);
status_t mutex_release(mutex_t __p_* mut);
status_t mutex_info(mutex_t __p_* mut, mutinfo_t __out_* info);
status_t mutex_delete(mutex_t __p_* mut);


#endif


/*== Mailbox API ========================================*/

/* Define Mailbox management functions. */

#if CFG_MAILBOX_EN > 0

status_t mailbox_create(mbox_t __p_* mbox, name_t name);
status_t mailbox_send(mbox_t __p_* mbox, mail_t mail);
status_t mailbox_receive(mbox_t __p_* mbox, mail_t __out_* mail, tick_t ticks);
status_t mailbox_flush(mbox_t __p_* mbox);
status_t mailbox_delete(mbox_t __p_* mbox);


#endif


/*== Message queue API ==================================*/

/* Define Message queue management functions. */

#if CFG_QUEUE_EN > 0

status_t queue_create(
        queue_t __p_*   queue,
        name_t          name,
        void   __p_*    buff,
        mmsz_t          size,
        mmsz_t          entries,
        int8u           options
        );
status_t queue_send(queue_t __p_* queue, const void __p_* buff);
status_t queue_receive(queue_t __p_* queue, void __out_* buff, tick_t ticks);
status_t queue_flush(queue_t __p_* queue);
status_t queue_delete(queue_t __p_* queue);


#endif


/*== Event API ==========================================*/

/* Define Event group management functions. */

#if CFG_EVENT_EN > 0

status_t event_create(event_t __p_* event, name_t name, bits_t bits);
status_t event_wait(event_t __p_* event, bits_t bits, int8u options, tick_t ticks);
status_t event_trywait(event_t __p_* event, bits_t bits, int8u options);
status_t event_post(event_t __p_* event, bits_t bits, int8u options);
status_t event_info(event_t __p_* event, evinfo_t __out_* info);
status_t event_delete(event_t __p_* event);


#endif


/*== Memory management API ==============================*/

/* Define Memory management functions. */

#if CFG_MPOOL_EN > 0

void __p_* mpool_alloc(mmsz_t size);
void mpool_free(void __p_* p);
status_t mpool_info(mpinfo_t __out_* info);

#endif


/*== Device management API ==============================*/

/* Define Device management functions. */

#if CFG_DEVICE_EN > 0

device_t build_device(int16u major, int16u minor);

status_t driver_install(int16u major, driver_table_t __p_* table);
status_t driver_uninstall(int16u major);

int device_init(device_t device, arg_t arg);
int device_open(device_t device, arg_t arg);
int device_close(device_t device);
int device_ioctl(device_t device, arg_t arg);
int device_write(device_t device, const char __p_* buff, int size);
int device_read(device_t device, char __out_* buff, int size);


#endif


/*=======================================================*/

#include "kdefine.h"


/*===========================================================================*/
#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif


