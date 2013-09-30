/*
===============================================================================
| COPYRIGHT (C) 2003-2007 Haishen Ruan, All rights reserved.
| SUMMARY:
|   Interrupt service routine implementation.
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


#if CFG_INT_VECTSZ > 0


/* If defined the CPU_ISR_ENABLE,It will execute the CPU interrupt operation
 * directly. this can decrease the CPU interrupt handling time. */

#ifdef  CPU_ISR_ENABLE


#ifdef  CPU_ISR_CRITICAL_DECLARE
#define ISR_CRITICAL_DECLARE    CPU_ISR_CRITICAL_DECLARE
#else
#define ISR_CRITICAL_DECLARE
#endif

#ifdef  CPU_ISR_INIT
#define ISR_INIT(vect)          CPU_ISR_INIT(vect)
#else
#define ISR_INIT(vect)          RSI_MPT
#endif

#define ISR_ENABLE(vect)        CPU_ISR_ENABLE(vect)
#define ISR_DISABLE(vect)       CPU_ISR_DISABLE(vect)
#define IF_ISR_ENABLE(vect)


#else


#define ISR_CRITICAL_DECLARE    SOFT_ISR_CRITICAL_DECLARE
#define ISR_INIT(vect)          SOFT_ISR_INIT(vect)
#define ISR_ENABLE(vect)        SOFT_ISR_ENABLE(vect)
#define ISR_DISABLE(vect)       SOFT_ISR_DISABLE(vect)
#define IF_ISR_ENABLE(vect)     if (SOFT_ISR_IS_ENABLE(vect))

#endif


#ifdef  CPU_ISR_SETUP

#define ISR_SETUP(vect, old_ientry, new_ientry) \
        CPU_ISR_SETUP(vect, old_ientry, new_ientry)

#else

#define ISR_SETUP(vect, old_ientry, new_ientry) \
do { \
    old_ientry = (inake_t)NULL; \
} while (0)

#endif


/* Interrupt vetors table. */
static intvect_t _vector[CFG_INT_VECTSZ];

/*==============================================================
Function: __interrupt_init()
Description: Interrupt vetors table initialization
Parameters:
Returns:
    RS_EOK -- succeed
Notes:
    intrinsic function
==============================================================*/
__fun_
status_t __interrupt_init(void) __ef_
{
    RSI_REG vect_t i;

    i = CFG_INT_VECTSZ - 1;
    do {
        ISR_INIT(i);
        _vector[i].routine = (ientry_t)NULL;
    } while (i--);

    return RS_EOK;
}


/*==============================================================
Function: __interrupt_dispatch()
Description: Default interrupt handling
Parameters:
    vect -- Vetor No.
Returns:
Notes:
    Internal function.
==============================================================*/
__fun_
void __interrupt_dispatch(vect_t vect) __ef_
{
    RSI_REG ientry_t ientry;

    IRQ_SAVEAREA;

#if CFG_INT_VECTSZ != 0x100 || CFG_INT_VECTSZ != 0x10000

    RSI_ASSERT(vect < CFG_INT_VECTSZ);
#endif

    IF_ISR_ENABLE(vect) {

        HOOK_INTERRUPT(vect);

        irq_lock();
        /* Retrieve the interrupt service entry point. */
        ientry = _vector[vect].routine;
        irq_unlock();

        /* determine if interrupt be setted up. */
        if (ientry != (ientry_t)NULL) {

            /* Yes, execute interrupt service routine. */
            (*ientry)(vect);
        }
    }
}


/*==============================================================
Function: interrupt_attach()
Description: Setup interrupt service entry point
Parameters:
    vect -- Vector No.
    ientry -- Interrupt service entry point,
              NULL indicates un-setup.
Returns:
    pervious interrupt sevice entry point
Notes:
==============================================================*/
__fun_
ientry_t r_interrupt_attach(vect_t vect, ientry_t ientry) __ef_
{
    RSI_REG ientry_t old_ientry;

    IRQ_SAVEAREA;

#if CFG_INT_VECTSZ != 0x100 && CFG_INT_VECTSZ != 0x10000
#if CFG_ARG_CHK > 1

    /* Wether the interrupt vector No. within the limitation. */
    if (vect >= CFG_INT_VECTSZ) {

        /* No, the interruptes vector No. exceed the limitation.
         * Return NULL. */
        return (ientry_t)NULL;
    }

#else

    RSI_ASSERT(vect < CFG_INT_VECTSZ);
#endif
#endif


    irq_lock();
    /* Retrieve to the pervious entry point. */
    old_ientry = _vector[vect].routine;

    /* Setup the new entry point. */
    _vector[vect].routine = ientry;
    irq_unlock();

    /* Return to the pervious entry point. */
    return old_ientry;
}


/*==============================================================
Function: intnake_attach()
Description: Setup nake interrupt service entry point
Parameters:
    vect -- Vector No.
    ientry -- Nake interrupt service entry point,
              NULL indicates un-setup.
Returns:
    pervious interrupt sevice entry point
Notes:
==============================================================*/
__fun_
inake_t r_intnake_attach(vect_t vect, inake_t ientry) __ef_
{
    RSI_REG inake_t old_ientry;

#if CFG_INT_VECTSZ != 0x100 && CFG_INT_VECTSZ != 0x10000
#if CFG_ARG_CHK > 1

    /* Wether the interrupt vector No. within the limitation. */
    if (vect >= CFG_INT_VECTSZ) {

        /* No, the interruptes vector No. exceed the limitation.
         * Return NULL. */
        return (inake_t)NULL;
    }

#else

    RSI_ASSERT(vect < CFG_INT_VECTSZ);
#endif
#endif

    /* Setup nake interrupt service entry point */
    ISR_SETUP(vect, old_ientry, ientry);

    /* Return to the pervious entry point. */
    return old_ientry;
}


/*==============================================================
Function: interrupt_disable()
Description: Mask the interrupt.
Parameters:
    vect -- Vector No.
Returns:
    RS_EOUTRANGE -- Vector No. exceed the limitation.
    RS_EOK -- succeed
Notes:
==============================================================*/
__fun_
status_t r_interrupt_disable(vect_t vect) __ef_
{
    ISR_CRITICAL_DECLARE;

#if CFG_INT_VECTSZ != 0x100 && CFG_INT_VECTSZ != 0x10000
#if CFG_ARG_CHK > 1

    /* Wether the interrupt vector No. within the limitation. */
    if (vect >= CFG_INT_VECTSZ) {

        /* No, the interruptes vector No. exceed the limitation.
         * Return failed. */
        return RS_EOUTRANGE;
    }

#else

    RSI_ASSERT(vect < CFG_INT_VECTSZ);
#endif
#endif


    /* The interrupt is masked */
    ISR_DISABLE(vect);

    return RS_EOK;
}


/*==============================================================
Function: interrupt_enable()
Description: Un-masked interrupt.
Parameters:
    vect -- Vector No.
Returns:
    RS_EOUTRANGE -- Vector No. exceed the limitation.
    RS_EOK -- succeed
Notes:
==============================================================*/
__fun_
status_t r_interrupt_enable(vect_t vect) __ef_
{
    ISR_CRITICAL_DECLARE;

#if CFG_INT_VECTSZ != 0x100 && CFG_INT_VECTSZ != 0x10000
#if CFG_ARG_CHK > 1

    /* Wether the interrupt vector No. within the limitation. */
    if (vect >= CFG_INT_VECTSZ) {

        /* No, the interruptes vector No. exceed the limitation.
         * Return failed. */
        return RS_EOUTRANGE;
    }

#else

    RSI_ASSERT(vect < CFG_INT_VECTSZ);
#endif
#endif


    /* The interrupt is un-masked */
    ISR_ENABLE(vect);

    /* Return succeed. */
    return RS_EOK;
}


/*==============================================================
Function: interrupt_catch()
Description: This routine temporarily un-mask the designated interrupt
             and then mask it.
Parameters:
    vect -- Vector No.
Returns:
    RS_EOUTRANGE -- Vector No. exceed the limitation.
    RS_EOK -- succeed
Notes:
==============================================================*/
__fun_
status_t r_interrupt_catch(vect_t vect) __ef_
{
    ISR_CRITICAL_DECLARE;

#if CFG_INT_VECTSZ != 0x100 && CFG_INT_VECTSZ != 0x10000
#if CFG_ARG_CHK > 1

    /* Wether the interrupt vector No. within the limitation. */
    if (vect >= CFG_INT_VECTSZ) {

        /* No, the interruptes vector No. exceed the limitation.
         * Return failed. */
        return RS_EOUTRANGE;
    }

#else

    RSI_ASSERT(vect < CFG_INT_VECTSZ);
#endif
#endif


    /* Un-masked the interrupt. */
    ISR_ENABLE(vect);

    /* Here enable interrupts briefly */
    __dummy();

    /* Masked the interrupt. */
    ISR_DISABLE(vect);

    /* Return succeed. */
    return RS_EOK;
}


#else


/*==============================================================
Function: __interrupt_dispatch()
Description: Default interrupt handling
Parameters:
    vect -- Vetor No.
Returns:
Notes:
    Internal function.
==============================================================*/
__fun_
void __interrupt_dispatch(vect_t vect) __ef_
{
    RSI_DISABLE_WARNING(vect);
}


#endif
/*===========================================================================*/


