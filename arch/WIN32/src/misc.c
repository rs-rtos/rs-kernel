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
#define  _WIN32_WINNT    0x0500
#include "arch/arch.h"
#include "inc/const.h"
#include "inc/cque.h"
#include "inc/kernel.h"
#include "inc/memory.h"
#include "inc/ipc.h"
#include "inc/device.h"
#include "inc/kapi.h"
#include "driver/driver.h"


/* Task stack registers structure. */
typedef struct _struct_regs
{
    int32u edi;
    int32u esi;
    int32u ebp;
    int32u task_fore;
    int32u task_entry;
    int32u task_return;
    int32u task_arg;
    int16u task_rarg;
} regs_t;


__fun_
static void __declspec(naked) __task_fore(void) __ef_
{
    irq_enable();
    _asm { ret }
}


__fun_
sp_t __stack_init(entry_t entry, arg_t arg, sp_t stack_base) __ef_
{
    regs_t* regs;

    regs = (regs_t*)((int32u)stack_base - sizeof(regs_t));

    regs->edi       = (int32u)0x00000000;
    regs->esi       = (int32u)0x00000000;
    regs->ebp       = (int32u)0x00000000;

    regs->task_fore   = (int32u)&__task_fore;

    regs->task_entry = (int32u)entry;
    
#if CFG_TASK_DELETE_EN > 0 || CFG_DEBUG > 0

    regs->task_return = (int32u)task_delete;
#endif

    regs->task_arg  = (int32u)arg;
    regs->task_rarg = (int16u)0xffff;

    return  (sp_t)regs;
}


/*===========================================================================*/

