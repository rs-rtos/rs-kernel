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
#ifndef __KDEFINE_H__
#define __KDEFINE_H__

#ifdef __cplusplus
extern "C" {
#endif
/*===========================================================================*/


/*== Kernel API =========================================*/

/* Define Kernel functions. */

#define build_name          __build_name
#define kernel_version      __kernel_version
#define kernel_version_s    __kernel_version_s
#define kernel_lock         __kernel_lock
#define kernel_unlock       __kernel_unlock
#define kernel_islock       __kernel_islock

#if CFG_SYS_INFO_EN > 0

#define kernel_info         __kernel_info

#endif


/*== Interrupt API ======================================*/

/* Define Interrupt management functions. */

#if CFG_INT_VECTSZ > 0

#define interrupt_attach    __interrupt_attach
#define interrupt_detach    __interrupt_detach
#define interrupt_enable    __interrupt_enable
#define interrupt_disable   __interrupt_disable
#define interrupt_catch     __interrupt_catch

#define intnake_attach      __intnake_attach
#define intnake_detach      __intnake_detach
#define intnake_enable      __intnake_enable
#define intnake_disable     __intnake_disable
#define intnake_catch       __intnake_catch

#endif


/*== Tick API ===========================================*/

/* Define Tick management functions. */

#if CFG_TICK_COUNT_EN > 0

#define tick_get            __tick_get
#define tick_set            __tick_set

#endif


/*== Timer API ==========================================*/

/* Define Timer management functions. */

#if CFG_TIMER_EN > 0

#define timer_create        __timer_create
#define timer_delete        __timer_delete
#define timer_enable        __timer_enable
#define timer_disable       __timer_disable
#define timer_control       __timer_control

#endif


/*== Task API ===========================================*/

/* Define Task control functions. */

#define task_create         __task_create

#if CFG_TASK_DELETE_EN > 0
#define task_delete         __task_delete
#endif

#if CFG_TASK_MODE > 0
#define task_change_prio    __task_change_prio
#endif

#if CFG_TASK_MODE > 1
#define task_change_runprio __task_change_runprio
#define task_reset_runprio  __task_reset_runprio
#endif

#define task_ident          __task_ident
#define task_self           __task_self
#define task_current        __task_current
#define task_sleep          __task_sleep
#define task_wake           __task_wake

#if CFG_TASK_SUSPEND_EN > 0
#define task_suspend        __task_suspend
#define task_resume         __task_resume
#endif

#define task_protect        __task_protect
#define task_unprotect      __task_unprotect

#if CFG_TASK_INFO_EN > 0
#define task_info           __task_info
#endif


/*== Binary semaphores API ==============================*/

/* Define Binary semaphore management functions. */

#if CFG_SEMBINARY_EN > 0

#define sembinary_create    __sembinary_create
#define sembinary_wait      __sembinary_wait
#define sembinary_trywait   __sembinary_trywait
#define sembinary_post      __sembinary_post
#define sembinary_info      __sembinary_info
#define sembinary_delete    __sembinary_delete

#endif


/*== Counting semaphores API ============================*/

/* Define Counting semaphore management functions. */

#if CFG_SEMAPHORE_EN > 0

#define semaphore_create    __semaphore_create
#define semaphore_wait      __semaphore_wait
#define semaphore_trywait   __semaphore_trywait
#define semaphore_post      __semaphore_post
#define semaphore_info      __semaphore_info
#define semaphore_delete    __semaphore_delete

#endif


/*== Mutex API ==========================================*/

/* Define Mutex management functions. */

#if CFG_MUTEX_EN > 0

#define mutex_create        __mutex_create
#define mutex_wait          __mutex_wait
#define mutex_trywait       __mutex_trywait
#define mutex_release       __mutex_release
#define mutex_info          __mutex_info
#define mutex_delete        __mutex_delete

#endif


/*== Mailbox API ========================================*/

/* Define Mailbox management functions. */

#if CFG_MAILBOX_EN > 0

#define mailbox_create      __mailbox_create
#define mailbox_send        __mailbox_send
#define mailbox_receive     __mailbox_receive
#define mailbox_flush       __mailbox_flush
#define mailbox_delete      __mailbox_delete

#endif


/*== Message queue API ==================================*/

/* Define Message queue management functions. */

#if CFG_QUEUE_EN > 0

#define queue_create        __queue_create
#define queue_send          __queue_send
#define queue_receive       __queue_receive
#define queue_flush         __queue_flush
#define queue_delete        __queue_delete

#endif


/*== Event API ==========================================*/

/* Define Event group management functions. */

#if CFG_EVENT_EN > 0

#define event_create        __event_create
#define event_wait          __event_wait
#define event_trywait       __event_trywait
#define event_post          __event_post
#define event_info          __event_info
#define event_delete        __event_delete

#endif


/*== Memory API ==============================*/

/* Define Memory management functions. */

#if CFG_MPOOL_EN > 0

#define mpool_alloc         __mpool_alloc
#define mpool_free          __mpool_free
#define mpool_info          __mpool_info

#endif


/*== Device API =========================================*/

/* Define Device management functions. */

#if CFG_DEVICE_EN > 0

#define build_device        __build_device

#define driver_install      __driver_install
#define driver_uninstall    __driver_uninstall

#define device_init         __device_init
#define device_open         __device_open
#define device_close        __device_close
#define device_ioctl        __device_ioctl
#define device_write        __device_write
#define device_read         __device_read

#endif


/*===========================================================================*/
#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif

