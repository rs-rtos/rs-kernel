/*
===============================================================================
| COPYRIGHT (C) 2003-2007 Haishen Ruan, All rights reserved.
| SUMMARY:
|   Miscellaneous functions.
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


typedef struct _struct_regs
{
    int16u rw0;
    int16u rw1;
    int16u rw2;
    int16u rw4;
    int16u rw5;
    int16u rw6;
    int16u rw7;
    int16u rw3_fp;

#if RSI_MEM_MODEL == SMALL || RSI_MEM_MODEL == COMPACT

    int16u task_fore;
    int16u task_entry;
    int16u task_return;
    int16u task_arg;

#else

    int32u task_fore;
    int32u task_entry;
    int32u task_return;
    int32u task_arg;
    
#endif

    int16u task_rarg;

} regs_t;


__fun_
static void __task_fore(void) __ef_
{
    irq_enable();
}


__fun_
sp_t __stack_init(entry_t entry, arg_t arg, sp_t stack_base) __ef_
{
    regs_t __p_* regs;

    regs = (regs_t __p_*)(stack_base - sizeof(regs_t));
    regs->rw7 = 0x7777;
    regs->rw6 = 0x6666;
    regs->rw5 = 0x5555;
    regs->rw4 = 0x4444;

    regs->rw2 = 0x2222;
    regs->rw1 = 0x1111;
    regs->rw0 = 0x0000;
    regs->rw3_fp      = (int16u)0x3333;

#if RSI_MEM_MODEL == SMALL || RSI_MEM_MODEL == COMPACT

    regs->task_fore   = (int16u)__task_fore;
    regs->task_entry  = (int16u)entry;

#if CFG_TASK_DELETE_EN > 0 || CFG_DEBUG > 0
    regs->task_return = (int16u)task_delete;
#endif
    regs->task_arg    = (int16u)arg;

#else

    regs->task_fore   = (int32u)__task_fore;
    regs->task_entry  = (int32u)entry;

#if CFG_TASK_DELETE_EN > 0 || CFG_DEBUG > 0
    regs->task_return = (int32u)task_delete;
#endif
    regs->task_arg    = (int32u)arg;

#endif

    regs->task_rarg   = (int16u)0xffff;
    
    return  (sp_t)regs;
}


/*===========================================================================*/


