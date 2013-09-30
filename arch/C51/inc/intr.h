/*
===============================================================================
| COPYRIGHT (C) 2003-2007 Haishen Ruan, All rights reserved.
| SUMMARY: 
|   Interrupt and clock support.
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
#ifndef __ARCH_INTR_H__
#define __ARCH_INTR_H__

#ifdef __cplusplus
extern "C" {
#endif
/*===========================================================================*/

#define irq_disable()       do { EA = 0; } while (0)
#define irq_enable()        do { EA = 1; } while (0)


#define IRQ_SAVEAREA        RSI_MPT

#define irq_lock() \
do { \
    EA = 0; \
} while (0)

#define irq_unlock() \
do { \
    EA = 1; \
} while (0)


/* The frequency of system tick. */
#define RS_TICK_FREQ        50


/*== context.c ==========================================*/

__fun_ void __switch_to(sp_t __p_*, sp_t) __ef_;
__fun_ void __timer_handle(void) __ef_;
__fun_ void __interrupt_handle(void) __ef_;
__fun_ void __task_fore(void) __ef_;
__fun_ int8u __load_xbp(void) __ef_;


/*===========================================================================*/
#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif


