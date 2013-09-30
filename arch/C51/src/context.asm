;*
;==============================================================================
; COPYRIGHT (C) 2003-2007 Haishen Ruan, All rights reserved.
; SUMMARY: 
;   Timer service.
;
;
; LICENSE INFORMATION
;   RS-RTOS is free software; you can redistribute it and/or modify it under
;   terms of the GNU General Public License as published by the
;   Free Software Foundation; either version 2, or (at your option) any
;   later version.
;
;   RS-RTOS is distributed in the hope that it will be useful,
;   but WITHOUT ANY WARRANTY; without even the implied warranty of
;   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
;   General Public License for more details.
;
;   You should have received a copy of the GNU General Public License
;   along with RS-RTOS; see file COPYING. If not, write to the Free Software
;   Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
;
;   As a special exception, including RS-RTOS header files in a file,
;   instantiating RS-RTOS generics or templates, or linking other files
;   with RS-RTOS objects to produce an executable application, does not
;   by itself cause the resulting executable application to be covered
;   by the GNU General Public License. This exception does not
;   however invalidate any other reasons why the executable file might be
;   covered by the GNU Public License.
;
;
; DESCRIPTION:
;   See http://www.RS-RTOS.org for documentation, latest information, license
;   and contact details.
;   email:ruanhaishen@gmail.com
;============================================================================*/
;*===========================================================================*/
#include "example/KeilC51/config.h"
#include "arch/C51/inc/mem_model.h"



#if RSI_MEM_MODEL == SMALL


        PUBLIC  __task_fore
        PUBLIC  ___switch_to

        EXTRN   CODE(__do_tick)
        EXTRN   CODE(__schedule)
        EXTRN   DATA(_sched_lock)


#elif RSI_MEM_MODEL == COMPACT


        PUBLIC  __task_fore
        PUBLIC  ___switch_to

        EXTRN   CODE(__do_tick)
        EXTRN   CODE(__schedule)
        EXTRN   XDATA(_sched_lock)


#elif RSI_MEM_MODEL == LARGE_I


        PUBLIC  __task_fore
        PUBLIC  __load_xbp
        PUBLIC  ___switch_to
        PUBLIC  _hardware_stack

        EXTRN   CODE(__do_tick)
        EXTRN   CODE(__schedule)
        EXTRN   XDATA(_sched_lock)
        EXTRN   DATA (?C_XBP)


#elif RSI_MEM_MODEL == LARGE_II


        PUBLIC  _?__task_fore
        PUBLIC  _?__load_xbp
        PUBLIC  _?__switch_to
        PUBLIC  _hardware_stack

        EXTRN   CODE(_?__do_tick)
        EXTRN   CODE(_?__schedule)
        EXTRN   XDATA(_sched_lock)
        EXTRN   DATA (?C_XBP)


#endif

        
        COUNTER         EQU       20000
        T0H_COUNTER     EQU       (65536 - COUNTER) / 256
        T0L_COUNTER     EQU       (65536 - COUNTER) - T0H_COUNTER * 256


#if RSI_MEM_MODEL == LARGE_I || RSI_MEM_MODEL == LARGE_II

?STACK  SEGMENT IDATA
        RSEG    ?STACK
_hardware_stack:
        DS 1
        
#endif


CSEG    AT      000BH
        CLR     EA
        MOV     TH0, #T0H_COUNTER
        MOV     TL0, #T0L_COUNTER
        JMP     __timer_handle


CSEG    AT      0100H



;/*==============================================================
;Function: __load_xbp()
;Description: This function return ?C_XBP address
;Parameters:
;Returns:
;   ?C_XBP
;Notes:
;==============================================================*/
;__fun_
;int8u __load_xbp(void) __ef_
;{
#if RSI_MEM_MODEL == LARGE_I

__load_xbp:
        MOV     R7, #?C_XBP
        RET

#elif RSI_MEM_MODEL == LARGE_II

_?__load_xbp:
        MOV     R7, #?C_XBP
        RET

#endif

;}


;/*==============================================================
;Function: __switch_to()
;Description: This function save current task stack and switch stack
;             to next task.
;Parameters:
;    curr_sp -- pointer to current task stack
;    next_sp -- pointer to next task stack
;Returns:
;Notes:
;==============================================================*/
;__fun_
;void __switch_to(sp_t __p_* curr_sp, sp_t next_sp) __ef_
;{
#if RSI_MEM_MODEL == SMALL
___switch_to:
#elif RSI_MEM_MODEL == COMPACT
___switch_to:
#elif RSI_MEM_MODEL == LARGE_I
___switch_to:
#elif RSI_MEM_MODEL == LARGE_II
_?__switch_to:
#endif

#if RSI_MEM_MODEL == SMALL

        MOV     R0, 0x07
        MOV     @R0, SP

        MOV     SP, 0x05
        RET

#elif RSI_MEM_MODEL == COMPACT

        MOV     R0, 0x07
        MOV     A, SP
        MOVX    @R0, A

        MOV     SP, 0x05
        RET

#elif RSI_MEM_MODEL == LARGE_I || RSI_MEM_MODEL == LARGE_II

        ;**
        ;* Calc stack length *
        CLR     C
        MOV     A, SP
        SUBB    A, #7-1
        MOV     R0, A

        ;**
        ;* Get ?C_XBP
        CLR     C
        MOV     A, ?C_XBP+1
        SUBB    A, R0
        MOV     R3, A

        MOV     A, ?C_XBP
        SUBB    A, #0
        MOV     R2, A

        ;**
        ;* Save ?C_XBP
        MOV     DPL, R7
        MOV     DPH, R6
        
        MOV     A, R2
        MOVX    @DPTR, A
        INC     DPTR
        MOV     A, R3
        MOVX    @DPTR, A

        ;**
        ;* Save stack length *
        MOV     DPL, R3
        MOV     DPH, R2

        DEC     R0
        MOV     A, R0
        MOVX    @DPTR, A

        MOV     R1, #7
        ;**
        ;* Save stack *
__save_stack:
        INC     R1
        INC     DPTR
        MOV     A, @R1
        MOVX    @DPTR, A
        DJNZ    R0, __save_stack


        ;**
        ;* Retrieve next stack *
        MOV     DPL, R5
        MOV     DPH, R4

        ;**
        ;* Get stack length *
        MOVX    A, @DPTR
        MOV     R0, A

        MOV     SP, #7

__load_stack:
        INC     DPTR
        MOVX    A, @DPTR
        PUSH    ACC
        DJNZ    R0, __load_stack

        RET
#endif
;}


;/*==============================================================
;Function: __task_fore()
;Description: This function 
;Parameters:
;Returns:
;Notes:
;==============================================================*/
;__fun_
;void __task_fore(void) __ef_
;{
#if RSI_MEM_MODEL == LARGE_II
_?__task_fore:
#else
__task_fore:
#endif
        ;**
        ;* Get argument from stack
        ;* Save argument to R6 & R7
        POP     ACC
        MOV     R6, A
        POP     ACC
        MOV     R7, A

        ; Enable interrupt
        SETB    EA
        
        RET
;}


;/*==============================================================
;Function: __timer_handle()
;Description: This function save current task stack and switch stack
;             to next task.
;Parameters:
;    curr_sp -- pointer to current task stack
;    next_sp -- pointer to next task stack
;Returns:
;Notes:
;==============================================================*/
;__fun_
;void __timer_handle(void) __ef_
;{
__timer_handle:
        PUSH    ACC
        PUSH    B
        PUSH    PSW
        PUSH    DPH
        PUSH    DPL
        PUSH    00H
        PUSH    01H
        PUSH    02H
        PUSH    03H
        PUSH    04H
        PUSH    05H
        PUSH    06H
        PUSH    07H

;   _sched_lock++
#if RSI_MEM_MODEL == SMALL

        INC     _sched_lock

#elif RSI_MEM_MODEL == COMPACT

        MOV     R0, #LOW _sched_lock
        MOVX    A, @R0
        INC     A
        MOVX    @R0, A

#else

        MOV     DPTR, #_sched_lock
        MOVX    A, @DPTR
        INC     A
        MOVX    @DPTR, A

#endif

        SETB    EA

;------------------------------------
#if RSI_MEM_MODEL == LARGE_II
        LCALL   _?__do_tick
#else
        LCALL   __do_tick
#endif

        MOV     A, #LOW  timer_exit
        PUSH    ACC
        MOV     A, #HIGH timer_exit
        PUSH    ACC
        RETI

;------------------------------------
timer_exit:

#if RSI_MEM_MODEL == SMALL

        DEC     _sched_lock
        MOV     A, _sched_lock
        
#elif RSI_MEM_MODEL == COMPACT

        MOV     R0, #LOW _sched_lock
        MOVX    A, @R0
        DEC     A
        MOVX    @R0, A

#else

        MOV     DPTR, #_sched_lock
        MOVX    A, @DPTR
        DEC     A
        MOVX    @DPTR, A

#endif

        JNZ     timer_return

#if RSI_MEM_MODEL == LARGE_II
        LCALL   _?__schedule
#else
        LCALL   __schedule
#endif

timer_return:

        POP     07H
        POP     06H
        POP     05H
        POP     04H
        POP     03H
        POP     02H
        POP     01H
        POP     00H
        POP     DPL
        POP     DPH
        POP     PSW
        POP     B
        POP     ACC
        RET
;}


;*===========================================================================*/
        END
        

