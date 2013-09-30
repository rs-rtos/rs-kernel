/*
===============================================================================
| COPYRIGHT (C) 2003-2007 Haishen Ruan, All rights reserved.
| SUMMARY:
|   Timer service.
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


__fun_
void __declspec(naked) __switch_to(sp_t* cur_sp, sp_t next_sp) __ef_
{
    __asm {

        /* Save registers. */
        PUSH    EBP
        MOV     EBP, ESP
        PUSH    ESI
        PUSH    EDI

        /* Switch task stack. */
        MOV     EAX, [cur_sp]
        MOV     [EAX], ESP
        MOV     ESP, next_sp

        /* Resume registers. */
        POP     EDI
        POP     ESI
        POP     EBP
        
        RET
    }
}


__fun_
void __declspec(naked) __timer_handle(void) __ef_
{
    __asm {
        /**
         * Lock scheduler first. */
        INC     _sched_lock

        /* Save registers. */
        PUSHAD

        /* Kernel tick handle. */
        CALL    __do_tick

        /* Clear interrupt flag. */
        MOV     simuFL.b.IF, 0

        /* Unlock scheduler. */
        DEC     _sched_lock

        /* Tasks Schedule. */
        CALL    FAR __schedule

        /* Resume registers. */
        POPAD

        /* IRET */
        POPFD   /*--]           */
        RET     /*--]---IRET    */
    }
}


__fun_
void __declspec(naked) __interrupt_handle(void) __ef_
{
    __asm {
        /**
         * Lock scheduler first. */
        INC     _sched_lock

        /* Save registers. */
        PUSHAD
    }

    /* Dispatch interrupt. */
    __interrupt_dispatch(simuFL.b.IRQ);

    __asm {

        /* Clear interrupt flag. */
        MOV     simuFL.b.IF, 0

        /* Unlock scheduler. */
        DEC     _sched_lock
        
        /* Tasks Schedule. */
        CALL    FAR __schedule

        /* Resume registers. */
        POPAD

        /* IRET */
        POPFD   /*--]           */
        RET     /*--]---IRET    */
    }
}


/*===========================================================================*/

