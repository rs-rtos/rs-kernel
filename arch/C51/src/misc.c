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


#if RSI_MEM_MODEL == SMALL || RSI_MEM_MODEL == COMPACT

typedef struct regs_context
{
    int16u task_entry;
    int16u task_fore;
    int16u task_arg;
} regs_t;


#elif RSI_MEM_MODEL == LARGE_I || RSI_MEM_MODEL == LARGE_II

extern idata int8u _hardware_stack[1];

#endif



/*==============================================================
Function: __stack_init()
Description: This function initialize task stack
Parameters:
    entry -- entry point function
    arg -- entry data
    stack_base -- stack base address
Returns:
    task stack
Notes:
==============================================================*/
__fun_
sp_t __stack_init(entry_t entry, arg_t arg, sp_t stack_base) __ef_
{

#if RSI_MEM_MODEL == SMALL || RSI_MEM_MODEL == COMPACT

    RSI_REG sp_t stack;

    stack = stack_base;
    
    *stack = (int8u)(entry);
    *++stack = (int8u)((int16u)entry >> 8);
    *++stack = (int8u)(arg);
    *++stack = (int8u)((int16u)arg >> 8);
    *++stack = (int8u)(__task_fore);
    *++stack = (int8u)((int16u)__task_fore >> 8);

    return stack;


#elif RSI_MEM_MODEL == LARGE_I || RSI_MEM_MODEL == LARGE_II

    RSI_REG int8u i;
    RSI_REG sp_t stack;

    stack = stack_base;
    
    *--stack = (int8u)((int16u)__task_fore >> 8);
    *--stack = (int8u)(__task_fore);
    *--stack = (int8u)((int16u)arg >> 8);
    *--stack = (int8u)(arg);
    *--stack = (int8u)((int16u)entry >> 8);
    *--stack = (int8u)(entry);

    i = (int8u)_hardware_stack;

    while (--i > 7) {
        if (i == __load_xbp() + 1) {
            *--stack = (int8u)(stack_base);
            *--stack = (int8u)((int16u)stack_base >> 8);
            --i;
            continue;
        }
        --stack;
    }

    *--stack = _hardware_stack - 8 + 6;
    
    return stack;

#endif

}


#if 0 /*RSI_MEM_MODEL == SMALL || RSI_MEM_MODEL == COMPACT*/
/*==============================================================
Function: __switch_to()
Description: This function save current task stack and switch stack
             to next task.
Parameters:
    curr_sp -- pointer to current task stack
    next_sp -- pointer to next task stack
Returns:
Notes:
==============================================================*/
__fun_
void __switch_to(sp_t __p_* curr_sp, sp_t next_sp) __ef_
{
    *curr_sp = (sp_t)SP;
    SP = next_sp;
}

#endif


/*===========================================================================*/


