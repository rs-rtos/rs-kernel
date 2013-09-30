/*
===============================================================================
| COPYRIGHT (C) 2003-2007 Haishen Ruan, All rights reserved.
| SUMMARY: 
|   RS-KERNEL configuration file.
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
#ifndef __CONFIG_H__
#define __CONFIG_H__
/*===========================================================================*/


/*== Configure task =====================================*/

/*=========================================================
Description:
    Maximum number of tasks, include kernel's tasks at least
    include IDLE task
Value:
    2 to 255
=========================================================*/
#define CFG_MAX_TASKS           255


/*=========================================================
Description:
    Task mode
Value:
    0 -- Simple priority mode
    1 -- Simple priority mode
    2 -- Complex task priority mode
=========================================================*/
#define CFG_TASK_MODE           2


/*=========================================================
Description:
    Task delete support
Value:
    0 -- Disable delete task support
    1 -- Enable delete task support
=========================================================*/
#define CFG_TASK_DELETE_EN      1


/*=========================================================
Description:
    Task suspend support
Value:
    0 -- Disable task suspend and resume support
    1 -- Enable task suspend and resume support
=========================================================*/
#define CFG_TASK_SUSPEND_EN     1


/*=========================================================
Description:
    Task information support
Value:
    0 -- Disable task information support
    1 -- Enable task information support
=========================================================*/
#define CFG_TASK_INFO_EN        1


/*== Configure interrupt ================================*/

/*=========================================================
Description:
    Configure size of interrupt vector configure
Value:
    0 -- Disable interrupt management
    n -- Size of interrupt vector 
=========================================================*/
#define CFG_INT_VECTSZ          256


/*== Configure tick =====================================*/

/*=========================================================
Description:
    Tick counting support
Value:
    0 -- Disable tick counting
    1 -- Enable tick counting
=========================================================*/
#define CFG_TICK_COUNT_EN       1


/*== Configure timer ====================================*/

/*=========================================================
Description:
    Timer management support
Value:
    0 -- Disable timer management
    1 -- Enable timer management
=========================================================*/
#define CFG_TIMER_EN            1


/*== Configure IPC =====================================*/

/*=========================================================
Description:
    IPC timeout support
Value:
    0 -- Disable IPC timeout
    1 -- Enable IPC timeout
=========================================================*/
#define CFG_IPC_TIMEOUT_EN      1


/*== Configure binary semaphore==========================*/

/*=========================================================
Description:
    Binary semaphore support
Value:
    0 -- Disable binary semaphore
    1 -- Enable binary semaphore
=========================================================*/
#define CFG_SEMBINARY_EN        1


/*== Configure counting semaphore========================*/

/*=========================================================
Description:
    Counting semaphore support
Value:
    0 -- Disable counting semaphore
    1 -- Enable counting semaphore
=========================================================*/
#define CFG_SEMAPHORE_EN        1


/*== Configure mutex ====================================*/

/*=========================================================
Description:
    Mutex support
Value:
    0 -- Disable mutex
    1 -- Enable mutex (inheritable pattern)
    2 -- Enable mutex (ceiling pattern)
Note:
    Enable mutex requires CFG_TASK_MODE >= 2
=========================================================*/
#define CFG_MUTEX_EN            2


/*=========================================================
Description:
    Mutex nesting mode support
Value:
    0 -- Disable mutex nesting mode
    1 -- Enable mutex nesting mode
=========================================================*/
#define CFG_MUTEX_NEST_EN       1


/*== Configure mailbox ==================================*/

/*=========================================================
Description:
    Mailbox support
Value:
    0 -- Disable mailbox
    1 -- Enable mailbox
=========================================================*/
#define CFG_MAILBOX_EN          1


/*== Configure message queue ============================*/

/*=========================================================
Description: Message queue support
Value:
    0 -- Disable message queue
    1 -- Enable message queue
Note:
    Enable message queue requires CFG_MPOOL_EN >= 1 
=========================================================*/
#define CFG_QUEUE_EN            1


/*== Configure event group management ===================*/

/*=========================================================
Description:
    Event group management support
Value:
    0 -- Disable event group management
    1 -- Enable event group management (simple mode)
    2 -- Enable event group management 
=========================================================*/
#define CFG_EVENT_EN            2


/*=========================================================
Description:
    Size of event group bits
Value:
    8 -- 8 bits event
    16 -- 16 bits event
    32 -- 32 bits event
=========================================================*/
#define CFG_EVENT_BITS          32


/*== Configure memory management ========================*/

/*=========================================================
Description: Memory management support
Value:
    0 -- Disable memory management
    1 -- Enable memory management
=========================================================*/
#define CFG_MPOOL_EN            1


/*=========================================================
Description:
    Defines the size of memory management can use
    OR you can also define start address
Value:
    The size of memory
Note:
    You can also define start address like:
    #define CFG_MM_START_ADDR 0x1122aabb
=========================================================*/
#define CFG_MM_SIZE \
                (sizeof(void*) +     8) * 128 + \
                (sizeof(void*) +    16) * 128 + \
                (sizeof(void*) +    32) * 256 + \
                (sizeof(void*) +    64) * 256 + \
                (sizeof(void*) +   128) * 256 + \
                (sizeof(void*) +   256) * 128 + \
                (sizeof(void*) +   512) * 128 + \
                (sizeof(void*) +  1024) *  64 + \
                (sizeof(void*) +  2048) *  64 + \
                (sizeof(void*) +  4096) *  32 + \
                (sizeof(void*) +  8192) *  32 + \
                (sizeof(void*) + 16348) *  16 + \
                (sizeof(void*) + 32768) *  10 + \
                (sizeof(void*) + 65536) *   8 


/*=========================================================
Description:
    Defines the blocks and block's size, 
    blocks and block's size must be > 0, the number of
    different block_size must be less than 256 (max. 255)
Value:
    { block_size (bytes), blocks }
Note:
    The CFG_MM_SIZE must be equal to the total blocks sizes
=========================================================*/
#define CFG_MM_BLOCKS \
                { (sizeof(void*) +     8),  128 }, \
                { (sizeof(void*) +    16),  128 }, \
                { (sizeof(void*) +    32),  256 }, \
                { (sizeof(void*) +    64),  256 }, \
                { (sizeof(void*) +   128),  256 }, \
                { (sizeof(void*) +   256),  128 }, \
                { (sizeof(void*) +   512),  128 }, \
                { (sizeof(void*) +  1024),   64 }, \
                { (sizeof(void*) +  2048),   64 }, \
                { (sizeof(void*) +  4096),   32 }, \
                { (sizeof(void*) +  8192),   32 }, \
                { (sizeof(void*) + 16348),   16 }, \
                { (sizeof(void*) + 32768),   10 }, \
                { (sizeof(void*) + 65536),    8 }


/*== Configure miscellaneous ============================*/

/*=========================================================
Description:
    Task object name description support
Value:
    0 -- Disable object name description
    1 -- Enable object name description
=========================================================*/
#define CFG_OBJ_NAME_EN         1


/*=========================================================
Description:
    Argument checking support
Value:
    0 -- Disable argument checking
    1 -- Normal argument checking
    2 -- Full argument checking
=========================================================*/
#define CFG_ARG_CHK             2


/*=========================================================
Description:
    Kernel debug support
Value:
    0 -- Disable kernel debug
    1 -- Enable kernel debug
=========================================================*/
#define CFG_DEBUG               1


/*=========================================================
Description:
    Maximum size of dump info buffer (chars)
Value:
    0 -- Disable dump info support.
    n -- Size of dump info buffer,
=========================================================*/
#define CFG_DUMP_BUFFSZ         512


/*=========================================================
Description:
    CPU information management support
Value:
    0 -- Disable cpu information support
    1 -- Enable cpu information support
=========================================================*/
#define CFG_SYS_INFO_EN         1


/*=========================================================
Description:
    Device management support
Value:
    0 -- Disable device management support
    1 -- Enable device management support
=========================================================*/
#define CFG_DEVICE_EN           1


/*=========================================================
Description:
    Maximum size of devices.
Value:
    1 to 32768
=========================================================*/
#define CFG_DEVICE_SIZE         50



/*===========================================================================*/
#endif


