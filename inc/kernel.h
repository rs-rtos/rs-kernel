/*
===============================================================================
| COPYRIGHT (C) 2003-2007 Haishen Ruan, All rights reserved.
| SUMMARY:
|   Base support function and condition variable definitions.
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
#ifndef __KERNEL_H__
#define __KERNEL_H__

#ifdef __cplusplus
extern "C" {
#endif
/*===========================================================================*/

/* Define common queue structure. */
typedef struct _struct_cque
{
#if RSI_QUEUE_SZ == 1
    uint que;
#else
    uint que[RSI_QUEUE_SZ];
#endif
} cque_t;


/* Define task control block structure. */
typedef struct _struct_task
{
    /* Pointer to task stack. */
    sp_t    stack;

    /* Task executing state. */
    int8u   state;

#if CFG_TASK_MODE > 1

    /* Task priority. */
    union {

        /* Normal priority. */
        prio_t  normal;

        /* Current executing priority. */
        prio_t  current;

    } prio;

#endif

#if RSI_DEF_TASK_LOCK > 0

    /* Task lock */
    int8u   lock;

#endif

    /* Ticks of task sleep. */
    tick_t  delay;

#if CFG_OBJ_NAME_EN > 0

    /* Define task name. */
    union {
        int8u  c[4];
        name_t w;
    } name;

#endif

#if CFG_TASK_INFO_EN > 0

    /* Base pointer to task stack. */
    sp_t    stack_base;

    /* Size of task stack. */
    mmsz_t  stack_size;

#elif CFG_MPOOL_EN > 0

    /* Base pointer to task stack. */
    sp_t    stack_base;

#endif

#if CFG_IPC_EN > 0

    /* Pointer to blocked queue. */
    cque_t __p_* pend_q;

#endif

} task_t;


/* Define array of TCB area structure. */
typedef struct _struct_tasks
{
    /* The array of task control block area. */
    task_t  task[CFG_MAX_TASKS];

    /* Pointer to kernel stack. */
    sp_t    stack;

    /* Kernel idle executing state. */
    int8u   state;
    
} tasks_t;


#if CFG_TASK_INFO_EN > 0

/* Define task info structure. */
typedef struct _struct_tinfo
{
    /* Input the task priority. */
    prio_t  prio;

    /* Task executing state. */
    int8u  state;

    /* Task stack info. */
    sp_t    stack_base;
    mmsz_t  stack_size;
    mmsz_t  stack_used;
    mmsz_t  stack_free;

    /* Task name.
     * locate at the end of structure. */
    int8u  name[5];

} tinfo_t;

#endif


#if CFG_SYS_INFO_EN > 0

/* Define kernel's information structure. */
typedef struct _struct_kinfo
{
    /* CPU used percentage. */
    int8s  cpu_usage;

    /* Task No. */
    int8u  task_no;
    
} kinfo_t;

#endif


/* Define interrupt vector type. */
#if CFG_INT_VECTSZ > 65536
    typedef int32u vect_t;
#elif CFG_INT_VECTSZ > 256
    typedef int16u vect_t;
#else
    typedef int8u vect_t;
#endif


#if CFG_INT_VECTSZ > 0


/* Define the interrupt entry point. */
typedef __fun_ void (*ientry_t)(vect_t) __ef_;


/* Define the nake interrupt entry point. */
typedef __fun_ void (*inake_t)(void) __ef_;


/* Define interrupt vectors structure. */
typedef struct _struct_intvect
{
    /* The interrupt entry point. */
    ientry_t   routine;

#ifndef CPU_ISR_ENABLE

    /* Enable/disable interrupt. */

    /* int8u  enable  :1;*/
    /* In strict ANSI C, the only types allowed for a bit field are int,
     * signed int and unsigned int. */
    bool enable;

#endif

} intvect_t;


#endif


#if CFG_TIMER_EN > 0

/* Define timer control block structure. */
typedef struct _struct_timer
{
#if CFG_OBJ_NAME_EN > 0

    /* Timer name. */
    union {
        int8u  c[4];
        name_t w;
    } name;
#endif

    /* This ticks belongs to the timer. */
    tick_t  hand_ticks;
    /* Initial ticks. */
    tick_t  init_ticks;
    /* Timer expiration entry point. */
    entry_t entry;
    /* Entry function argument. */
    arg_t   arg;

    /* Timer state. */
    int8u   state;

#if CFG_DEBUG > 0
    int8u   init;
#endif
    /* Pointer to the next timer. */
    struct _struct_timer __p_* next;

} timer_t;

#endif


/*=======================================================*/

/* kernel control variable declaration. */
extern prio_t   _current_prio;
extern volatile uint _sched_lock;
extern tasks_t  _tasks;
extern cque_t   _ready_queue;


#if CFG_SYS_INFO_EN > 0

extern volatile int8s _cpu_usage;

#endif


#define _TASKS  _tasks.task


/* Ready queue operation macros */
#define __ready_que_init() \
        __cque_init(_ready_queue)

#define __ready_que_remove(prio) \
        __cque_remove(_ready_queue, prio)

#define __ready_que_insert(prio) \
        __cque_insert(_ready_queue, prio)

#define __ready_que_head(next) \
        __cque_head(_ready_queue, next)

#define __ready_que_move(from, to) \
        __cque_move(_ready_queue, from, to)

#define __ready_que_susremove(prio, state) \
        __cque_susremove(_ready_queue, prio, state)


#define __lock_k() \
        _sched_lock++

#define __unlock_k() \
        _sched_lock--

        
/*=======================================================*/

/* Dummy function. */
#ifdef RSI_NOP

#define __dummy()           RSI_NOP

#else

__fun_ void __dummy(void) __ef_;

#endif


#if LITTLE_ENDIAN

#define __build_name(c1, c2, c3, c4) \
    (name_t)((int32u)(c4) << 24 | (int32u)(c3) << 16 | (int32u)(c2) << 8 | (int32u)(c1))

#else

#define __build_name(c1, c2, c3, c4) \
    (name_t)((int32u)(c1) << 24 | (int32u)(c2) << 16 | (int32u)(c3) << 8 | (int32u)(c4))

#endif


#define __kernel_version() \
        RSI_VERSION


#define __kernel_version_s() \
        RSI_VERSION_STRING


/*== sched.c ============================================*/

__fun_ void r_kernel_lock(void) __ef_;
__fun_ void r_kernel_unlock(void) __ef_;
__fun_ uint r_kernel_islock(void) __ef_;

#if CFG_SYS_INFO_EN > 0

__fun_ status_t r_kernel_info(kinfo_t __p_* info) __ef_;

#endif

__fun_ void __kernel_init(void) __ef_;
__fun_ void __schedule(void) __ef_;


#if CFG_TASK_MODE > 1

__fun_ bool __prio_isnormal(prio_t prio) __ef_;

#endif

#define __prio_normal(cprio)    _TASKS[cprio].prio.normal
#define __prio_current(prio)    _TASKS[prio].prio.current


#if RSI_DEF_TASK_LOCK > 0

#define __nprio_lock_init(cprio) \
do { \
    _TASKS[cprio].lock = 0; \
} while (0)

#define __nprio_lock(cprio) \
do { \
    _TASKS[cprio].lock++; \
} while (0)

#define __nprio_unlock(cprio) \
do { \
    _TASKS[cprio].lock--; \
} while (0)

#else

#define __nprio_lock_init(cprio)    RSI_MPT

#endif


#define __kernel_lock       r_kernel_lock
#define __kernel_unlock     r_kernel_unlock
#define __kernel_islock     r_kernel_islock
#define __kernel_info       r_kernel_info


/*== timer.c ============================================*/

#if CFG_TICK_COUNT_EN > 0

extern volatile int32u _ticks;


__fun_ int32u r_tick_get(void) __ef_;
__fun_ void r_tick_set(int32u ticks) __ef_;


#define __tick_get          r_tick_get
#define __tick_set          r_tick_set


#endif


__fun_ int8u __do_tick(void) __ef_;


#if CFG_TIMER_EN > 0

#if CFG_OBJ_NAME_EN > 0

__fun_
status_t r_timer_create(
        timer_t __p_*   timer,
        name_t          name,
        entry_t         entry,
        arg_t           arg,
        tick_t          ticks,
        int8u           options
        ) __ef_;

#define __timer_create(timer, name, entry, arg, ticks, options) \
        r_timer_create(timer, name, entry, arg, ticks, options)

#else

__fun_
status_t r_timer_create(
        timer_t __p_*   timer,
        entry_t         entry,
        arg_t           arg,
        tick_t          ticks,
        int8u           options
        ) __ef_;

#define __timer_create(timer, name, entry, arg, ticks, options) \
        r_timer_create(timer, entry, arg, ticks, options)

#endif

__fun_ status_t r_timer_delete(timer_t __p_* timer) __ef_;
__fun_ status_t r_timer_enable(timer_t __p_* timer) __ef_;
__fun_ status_t r_timer_disable(timer_t __p_* timer) __ef_;
__fun_ status_t r_timer_control(timer_t __p_* timer, int8u options) __ef_;

__fun_ void __timer_active(timer_t __p_* timer) __ef_;
__fun_ void __timer_expiration(void) __ef_;


#define __timer_delete      r_timer_delete
#define __timer_enable      r_timer_enable
#define __timer_disable     r_timer_disable
#define __timer_control     r_timer_control


#endif


/*== task.c =============================================*/


#if CFG_TASK_INFO_EN > 0 || CFG_MPOOL_EN > 0 || ARCH_STACK_GROW == DOWNWARDS
#define RSI_DEF_PARAM_STKSZ      1
#else
#define RSI_DEF_PARAM_STKSZ      0
#endif


#if CFG_TASK_SUSPEND_EN > 0 || CFG_MPOOL_EN > 0
#define RSI_DEF_PARAM_OPTS      1
#else
#define RSI_DEF_PARAM_OPTS      0
#endif


__fun_ status_t r_task_create(
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
            ) __ef_;


#if CFG_TASK_DELETE_EN > 0

__fun_ status_t r_task_delete(prio_t prio) __ef_;

#endif


#if CFG_TASK_MODE > 0

__fun_ status_t r_task_change_prio(prio_t prio, prio_t new_prio) __ef_;

#endif


#if CFG_TASK_MODE > 1

__fun_ status_t r_task_change_runprio(prio_t prio, prio_t run_prio) __ef_;

#endif


#if CFG_TASK_MODE >= 2

__fun_ prio_t r_task_ident(void) __ef_;

#endif


__fun_ prio_t r_task_current(void) __ef_;

__fun_ status_t r_task_sleep(tick_t ticks) __ef_;
__fun_ status_t r_task_wake(prio_t prio) __ef_;


#if CFG_TASK_SUSPEND_EN > 0

__fun_ status_t r_task_suspend(prio_t prio) __ef_;
__fun_ status_t r_task_resume(prio_t prio) __ef_;

#endif


#if RSI_DEF_TASK_LOCK > 0

__fun_ void r_task_protect(void) __ef_;
__fun_ void r_task_unprotect(void) __ef_;

#endif


#if CFG_TASK_INFO_EN > 0

__fun_ status_t r_task_info(tinfo_t __p_* info) __ef_;

#endif


__fun_ sp_t __stack_init(entry_t entry, arg_t arg, sp_t stack_base) __ef_;
__fun_ void __task_init(void) __ef_;


/* GCC Compiler:
 * The token-pasting operator is only for making tokens - 'foo##bar'
 * becomes the single token 'foobar' and its result must always
 * produce a single valid token (or the results are undefined).
 * past a comma onto the begining of a multichar token, 
 * and , is only a valid token by itself. */
 
#if CFG_OBJ_NAME_EN > 0
#define RSI_SELECT_NAME(string)         ,string
#else
#define RSI_SELECT_NAME(string)
#endif


#if RSI_DEF_PARAM_STKSZ > 0
#define RSI_SELECT_STKSZ(string)        ,string
#define RSI_SELECT_STKBS(base, size)    ,base
#else
#define RSI_SELECT_STKSZ(string)
#define RSI_SELECT_STKBS(base, size)    ,((base) + size)
#endif


#if RSI_DEF_PARAM_OPTS > 0
#define RSI_SELECT_OPTS(string)         ,string
#else
#define RSI_SELECT_OPTS(string)
#endif


#define __task_create( \
            prio, name, entry, arg, stack_base, stack_size, options) \
        r_task_create( \
            prio \
            RSI_SELECT_NAME(name), \
            entry, arg, \
            stack_base \
            RSI_SELECT_STKSZ(stack_size) \
            RSI_SELECT_OPTS(options))


#define __task_delete           r_task_delete
#define __task_change_prio      r_task_change_prio
#define __task_change_runprio   r_task_change_runprio


#if CFG_TASK_MODE > 1
#define __task_reset_runprio(prio) \
        r_task_change_runprio(prio, prio)
#endif


#if CFG_TASK_MODE < 2
#define __task_ident            r_task_current
#else
#define __task_ident            r_task_ident
#endif


#define __task_self()           RSI_SELF_PRIO
#define __task_current          r_task_current
#define __task_sleep            r_task_sleep
#define __task_wake             r_task_wake
#define __task_suspend          r_task_suspend
#define __task_resume           r_task_resume


#if RSI_DEF_TASK_LOCK > 0

#define __task_protect() \
        r_task_protect()

#define __task_unprotect() \
        r_task_unprotect()

#else

#define __task_protect() \
        RSI_MPT

#define __task_unprotect() \
        RSI_MPT

#endif


#define __task_info             r_task_info


/*== ints.c =============================================*/


__fun_ void  __interrupt_dispatch(vect_t vect) __ef_;


#if CFG_INT_VECTSZ > 0

__fun_ ientry_t r_interrupt_attach(vect_t vect, ientry_t ientry) __ef_;
__fun_ status_t r_interrupt_enable(vect_t vect) __ef_;
__fun_ status_t r_interrupt_disable(vect_t vect) __ef_;
__fun_ status_t r_interrupt_catch(vect_t vect) __ef_;
__fun_ inake_t r_intnake_attach(vect_t vect, inake_t ientry) __ef_;

__fun_ status_t __interrupt_init(void) __ef_;


#define __interrupt_attach      r_interrupt_attach

#define __interrupt_detach(vect) \
        r_interrupt_attach(vect, NULL)

#define __interrupt_enable      r_interrupt_enable
#define __interrupt_disable     r_interrupt_disable
#define __interrupt_catch       r_interrupt_catch


#define __intnake_attach        r_intnake_attach

#define __intnake_detach(vect) \
        r_intnake_attach(vect, NULL)

#define __intnake_enable(vect) \
        r_interrupt_enable(vect)

#define __intnake_disable(vect) \
        r_interrupt_disable(vect)

#define __intnake_catch(vect) \
        r_interrupt_catch(vect)


#define SOFT_ISR_CRITICAL_DECLARE \
    IRQ_SAVEAREA

#define SOFT_ISR_INIT(vect) \
do { \
    _vector[vect].enable = false; \
} while (0)


#define SOFT_ISR_ENABLE(vect) \
do { \
    irq_lock(); \
    _vector[vect].enable = true; \
    irq_unlock(); \
} while (0)


#define SOFT_ISR_DISABLE(vect) \
do { \
    irq_lock(); \
    _vector[vect].enable = false; \
    irq_unlock(); \
} while (0)


#define SOFT_ISR_IS_ENABLE(vect) \
    (_vector[vect].enable)


#define SOFT_ISR_SETUP(vect, old_ientry, new_ientry) \
do { \
    irq_lock(); \
    old_ientry = _vector[vect].routine; \
    _vector[vect].routine = new_ientry; \
    irq_unlock(); \
} while (0)


#endif


/*===========================================================================*/
#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif

