/*
===============================================================================
| COPYRIGHT (C) 2003-2007 Haishen Ruan, All rights reserved.
| SUMMARY:
|   Intertask communication and synchronization (includes binary semaphore,
|   counting semaphore, mutex, mailbox, message queue) support functions
|   and condition variable definitions.
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
#ifndef __IPC_H__
#define __IPC_H__

#ifdef __cplusplus
extern "C" {
#endif
/*===========================================================================*/

#if CFG_IPC_EN > 0

/* Indicate blocked queue is empty. */
#define  RSI_NULL_PRIO      0xff
/* Indicate that task is not blocked. */
#define  RSI_IPC_NOWAIT     0

#endif


/*===================================================*/

#if CFG_SEMBINARY_EN > 0

typedef struct _struct_semb
{
#if CFG_OBJ_NAME_EN > 0
    union {
        int8u  c[4];
        name_t w;
    } name;
#endif

    bool    avail;         /* semb available flag */

#if CFG_DEBUG > 0
    int8u   init;
#endif

    cque_t  waits;

} semb_t;

/* Define binary semaphore info struct */
typedef struct _struct_sembinfo
{
    /* semaphore info. */
    bool value;

    /* semaphore name.
     * locate at the end of structure. */
    int8u  name[5];

} sembinfo_t;


#endif


#if CFG_SEMAPHORE_EN > 0

typedef struct _struct_sem
{
#if CFG_OBJ_NAME_EN > 0
    union {
        int8u  c[4];
        name_t w;
    } name;
#endif

    count_t count;

#if CFG_DEBUG > 0
    int8u   init;
#endif

    cque_t  waits;

} sem_t;

/* Define semaphore info struct */
typedef struct _struct_seminfo
{
    /* semaphore info. */
    count_t count;

    /* semaphore name.
     * locate at the end of structure. */
    int8u  name[5];

} seminfo_t;


#endif

#if CFG_EVENT_EN > 0

#if CFG_EVENT_BITS > 16
typedef int32u bits_t;
#elif CFG_EVENT_BITS > 8
typedef int16u bits_t;
#else
typedef int8u bits_t;
#endif

typedef struct _struct_event
{
#if CFG_OBJ_NAME_EN > 0
    union {
        int8u  c[4];
        name_t w;
    } name;
#endif

    bits_t  bits;

#if CFG_EVENT_EN == 2
    bits_t  cares;
#endif

#if CFG_DEBUG > 0
    int8u   init;
#endif

    cque_t  waits;

} event_t;

/* Define event info struct */
typedef struct _struct_evinfo
{
    /* event info. */
    bits_t  bits;

    /* event name.
     * locate at the end of structure. */
    int8u  name[5];

} evinfo_t;

#endif


#if CFG_MUTEX_EN > 0

typedef struct _struct_mutex
{
#if CFG_OBJ_NAME_EN > 0
    union {
        int8u  c[4];
        name_t w;
    } name;
#endif

    prio_t  ceiling;
    prio_t  owner;

#if CFG_MUTEX_NEST_EN > 0
    count_t reference;
#endif

#if CFG_DEBUG > 0
    int8u   init;
#endif

    cque_t  waits;

} mutex_t;


/* Define mutex info struct */
typedef struct _struct_mutinfo
{
    /* Mutex info. */
    prio_t  prio;
    prio_t  owner;
    count_t reference;

    /* Mutex name.
     * locate at the end of structure. */
    int8u  name[5];
    
} mutinfo_t;


#endif


#if CFG_MAILBOX_EN > 0

typedef struct _struct_mbox
{
#if CFG_OBJ_NAME_EN > 0
    union {
        int8u  c[4];
        name_t w;
    } name;
#endif

    /* mailbox value */
    mail_t  mail;

    /* mailbox available flag */
    bool    avail;

#if CFG_DEBUG > 0
    int8u   init;
#endif

    cque_t  waits;

} mbox_t;

#endif


#if CFG_QUEUE_EN > 0
typedef struct _struct_queue
{
#if CFG_OBJ_NAME_EN > 0
    union {
        int8u  c[4];
        name_t w;
    } name;
#endif

    int8u __p_* pstart;
    int8u __p_* phead;
    int8u __p_* ptail;
    int8u __p_* pend;
    mmsz_t      avail;
    mmsz_t      msgsz;
    mmsz_t      entries;
    int8u       options;

#if CFG_DEBUG > 0
    int8u       init;
#endif

    cque_t      waits;

} queue_t;

#endif

/*===================================================*/

#if CFG_IPC_EN > 0
__fun_ void __ipc_block(cque_t __p_* q, tick_t ticks) __ef_;
__fun_ prio_t __ipc_resume(cque_t __p_* q) __ef_;

#if CFG_IPC_TIMEOUT_EN > 0
__fun_ void __ipc_timeout(cque_t __p_* q) __ef_;
#endif

/* Adjust task delay counter. */
#define __adjust_delay(prio, ticks) \
do { \
    if (ticks != 0) { \
        ticks = _TASKS[prio].delay; \
        _TASKS[prio].delay = 0; \
        if (ticks == 0) { \
            ticks = 1; \
        } \
    } \
} while (0)

#endif



/*== semb.c =============================================*/

#if CFG_SEMBINARY_EN > 0

#if CFG_OBJ_NAME_EN > 0

__fun_
status_t r_sembinary_create(
            semb_t __p_* semb,
            name_t       name,
            bool         value
            ) __ef_;

#define __sembinary_create(semb, name, value) \
        r_sembinary_create(semb, name, value)

#else

__fun_
status_t r_sembinary_create(
            semb_t __p_* semb,
            bool         value
            ) __ef_;

#define __sembinary_create(semb, name, value) \
        r_sembinary_create(semb, value)

#endif


#if CFG_IPC_TIMEOUT_EN > 0 || CFG_DEBUG > 0

__fun_
status_t r_sembinary_wait(
            semb_t __p_* semb,
            tick_t       ticks
            ) __ef_;

#define __sembinary_wait(semb, ticks) \
        r_sembinary_wait(semb, ticks)

#else

__fun_
status_t r_sembinary_wait(
            semb_t __p_* semb
            ) __ef_;

#define __sembinary_wait(semb, ticks) \
        r_sembinary_wait(semb)

#endif


__fun_ status_t r_sembinary_trywait(semb_t __p_* semb) __ef_;
__fun_ status_t r_sembinary_post(semb_t __p_* semb) __ef_;
__fun_ status_t r_sembinary_info(
            semb_t __p_* semb,
            sembinfo_t __out_* info
            ) __ef_;
__fun_ status_t r_sembinary_delete(semb_t __p_* semb) __ef_;


#define __sembinary_trywait     r_sembinary_trywait
#define __sembinary_post        r_sembinary_post
#define __sembinary_info        r_sembinary_info
#define __sembinary_delete      r_sembinary_delete


#endif


/*== sem.c ==============================================*/

#if CFG_SEMAPHORE_EN > 0

#if CFG_OBJ_NAME_EN > 0

__fun_
status_t r_semaphore_create(
            sem_t __p_* sem,
            name_t      name,
            count_t     value
            ) __ef_;

#define __semaphore_create(sem, name, value) \
        r_semaphore_create(sem, name, value)

#else

__fun_
status_t r_semaphore_create(
            sem_t __p_* sem,
            count_t     value
            ) __ef_;

#define __semaphore_create(sem, name, value) \
        r_semaphore_create(sem, value)

#endif


#if CFG_IPC_TIMEOUT_EN > 0 || CFG_DEBUG > 0

__fun_ status_t r_semaphore_wait(sem_t __p_* sem, tick_t ticks) __ef_;

#define __semaphore_wait(sem, ticks) \
        r_semaphore_wait(sem, ticks)

#else

__fun_ status_t r_semaphore_wait(sem_t __p_* sem) __ef_;

#define __semaphore_wait(sem, ticks) \
        r_semaphore_wait(sem)

#endif


__fun_ status_t r_semaphore_trywait(sem_t __p_* sem) __ef_;
__fun_ status_t r_semaphore_post(sem_t __p_* sem) __ef_;
__fun_ status_t r_semaphore_info(
            sem_t     __p_* sem,
            seminfo_t __out_* info
            ) __ef_;
__fun_ status_t r_semaphore_delete(sem_t __p_* sem) __ef_;


#define __semaphore_trywait     r_semaphore_trywait
#define __semaphore_post        r_semaphore_post
#define __semaphore_info        r_semaphore_info
#define __semaphore_delete      r_semaphore_delete


#endif


/*== mutex.c ============================================*/

#if CFG_MUTEX_EN > 0

#if CFG_OBJ_NAME_EN > 0

__fun_
status_t r_mutex_create(
            mutex_t __p_* mut,
            name_t        name,
            prio_t        prio
            ) __ef_;

#define __mutex_create(mut, name, prio) \
        r_mutex_create(mut, name, prio)


#else

__fun_
status_t r_mutex_create(mutex_t __p_* mut, prio_t prio) __ef_;

#define __mutex_create(mut, name, prio) \
        r_mutex_create(mut, prio)

#endif


#if CFG_IPC_TIMEOUT_EN > 0 || CFG_DEBUG > 0

__fun_
status_t r_mutex_wait(mutex_t __p_* mut, tick_t ticks) __ef_;

#define __mutex_wait(mut, ticks) \
        r_mutex_wait(mut, ticks)

#else

__fun_
status_t r_mutex_wait(mutex_t __p_* mut) __ef_;

#define __mutex_wait(mut, ticks) \
        r_mutex_wait(mut)

#endif


__fun_ status_t r_mutex_trywait(mutex_t __p_* mut) __ef_;
__fun_ status_t r_mutex_release(mutex_t __p_* mut) __ef_;
__fun_ status_t r_mutex_info(
            mutex_t     __p_* mut,
            mutinfo_t   __out_* info
            ) __ef_;
__fun_ status_t r_mutex_delete(mutex_t __p_* mut) __ef_;


#define __mutex_trywait     r_mutex_trywait
#define __mutex_release     r_mutex_release
#define __mutex_info        r_mutex_info
#define __mutex_delete      r_mutex_delete


#endif


/*== mailbox.c ==========================================*/

#if CFG_MAILBOX_EN > 0

#if CFG_OBJ_NAME_EN > 0

__fun_
status_t r_mailbox_create(mbox_t __p_* mbox, name_t name) __ef_;

#define __mailbox_create(mbox, name) \
        r_mailbox_create(mbox, name)

#else

__fun_
status_t r_mailbox_create(mbox_t __p_* mbox) __ef_;

#define __mailbox_create(mbox, name) \
        r_mailbox_create(mbox)

#endif


#if CFG_IPC_TIMEOUT_EN > 0 || CFG_DEBUG > 0

__fun_
status_t r_mailbox_receive(
    mbox_t __p_*    mbox,
    mail_t __out_*  mail,
    tick_t ticks
    ) __ef_;

#define __mailbox_receive(mbox, mail, ticks) \
        r_mailbox_receive(mbox, mail, ticks);

#else

__fun_
status_t r_mailbox_receive(
    mbox_t __p_*    mbox,
    mail_t __out_*  mail
    ) __ef_;

#define __mailbox_receive(mbox, mail, ticks) \
        r_mailbox_receive(mbox, mail);

#endif


__fun_ status_t r_mailbox_send(mbox_t __p_* mbox, mail_t mail) __ef_;
__fun_ status_t r_mailbox_flush(mbox_t __p_* mbox) __ef_;
__fun_ status_t r_mailbox_delete(mbox_t __p_* mbox) __ef_;


#define __mailbox_send      r_mailbox_send
#define __mailbox_flush     r_mailbox_flush
#define __mailbox_delete    r_mailbox_delete



#endif


/*== queue.c =============================================*/

#if CFG_QUEUE_EN > 0

#if CFG_OBJ_NAME_EN > 0

__fun_
status_t r_queue_create(
    queue_t __p_*   queue,
    name_t          name,
    void    __p_*   buff,
    mmsz_t          size,
    mmsz_t          entries,
    int8u           options
    ) __ef_;

#define __queue_create(queue, name, buff, size, entries, options) \
        r_queue_create(queue, name, buff, size, entries, options)

#else

__fun_
status_t r_queue_create(
    queue_t __p_*   queue,
    void    __p_*   buff,
    mmsz_t          size,
    mmsz_t          entries,
    int8u           options
    ) __ef_;

#define __queue_create(queue, name, buff, size, entries, options) \
        r_queue_create(queue, buff, size, entries, options)

#endif


#if CFG_IPC_TIMEOUT_EN > 0 || CFG_DEBUG > 0

__fun_
status_t r_queue_receive(
    queue_t __p_*   queue,
    void    __out_* buff,
    tick_t ticks
    ) __ef_;

#define __queue_receive(queue, buff, ticks) \
        r_queue_receive(queue, buff, ticks)

#else

__fun_
status_t r_queue_receive(
    queue_t __p_*   queue,
    void    __out_* buff
    ) __ef_;

#define __queue_receive(queue, buff, ticks) \
        r_queue_receive(queue, buff)

#endif


__fun_ status_t r_queue_send(
    queue_t     __p_* queue,
    const void  __p_* buff
    ) __ef_;
__fun_ status_t r_queue_flush(queue_t __p_* queue) __ef_;
__fun_ status_t r_queue_delete(queue_t __p_* queue) __ef_;


#define __queue_send        r_queue_send
#define __queue_flush       r_queue_flush
#define __queue_delete      r_queue_delete


#endif


/*== event.c ============================================*/

#if CFG_EVENT_EN > 0

#if CFG_OBJ_NAME_EN > 0

__fun_
status_t r_event_create(
    event_t __p_*   event,
    name_t          name,
    bits_t          bits
    ) __ef_;

#define __event_create(event, name, bits) \
        r_event_create(event, name, bits)

#else

__fun_
status_t r_event_create(
    event_t __p_*   event,
    bits_t          bits
    ) __ef_;

#define __event_create(event, name, bits) \
        r_event_create(event, bits)

#endif


#if CFG_IPC_TIMEOUT_EN > 0 || CFG_DEBUG > 0

__fun_
status_t r_event_wait(
    event_t __p_*   event,
    bits_t          bits,
    int8u           options,
    tick_t          ticks
    ) __ef_;

#define __event_wait(event, bits, options, ticks) \
        r_event_wait(event, bits, options, ticks)

#else

__fun_
status_t r_event_wait(
    event_t __p_*   event,
    bits_t          bits,
    int8u           options
    ) __ef_;

#define __event_wait(event, bits, options, ticks) \
        r_event_wait(event, bits, options)

#endif


__fun_
status_t r_event_trywait(
    event_t __p_*   event,
    bits_t          bits,
    int8u           options
    ) __ef_; 

__fun_
status_t r_event_post(
    event_t __p_*   event,
    bits_t          bits,
    int8u           options
    ) __ef_;

__fun_
status_t r_event_info(
    event_t  __p_*   event,
    evinfo_t __out_* info
    ) __ef_;
    
__fun_
status_t r_event_delete(event_t __p_* event) __ef_;


#define __event_trywait     r_event_trywait
#define __event_post        r_event_post
#define __event_info        r_event_info
#define __event_delete      r_event_delete


#endif


/*===========================================================================*/
#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif

