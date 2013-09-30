;*
;==============================================================================
; COPYRIGHT (C) 2003-2007 Haishen Ruan, All rights reserved.
; SUMMARY: 
;   Interrupt service for fujitsu F2MC-16LX/16F.
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
#include "../inc/mem_model.h"

TBTC    .EQU    0xA9        ; Timerbase timer control register
TBOF    .EQU    TBTC:3      ; Interrupt request flag bit
        
        .GLOBAL ___switch_to
        .GLOBAL ___timer_handle
        .GLOBAL ___interrupt_handle

        .GLOBAL ___do_tick
        .GLOBAL ___schedule
        .GLOBAL ___interrupt_dispatch

        .GLOBAL __sched_lock

        
        .SECTION CODE, CODE, ALIGN=1



;====================================================================
;void __switch_to(sp_t __p_* curr_sp, sp_t next_sp)
;{
___switch_to:
        LINK    #00
        
        PUSHW   (RW0,RW1,RW2,RW4,RW5,RW6,RW7)

;   *curr_sp = SP;
;   SP = next_sp;
#if RSI_MEM_MODEL == SMALL

        MOVW    A,SP
        MOVW    A,@RW3+04H
        MOVW    @AL,AH
        
        MOVW    A,@RW3+06H
        MOVW    SP,A

#elif RSI_MEM_MODEL == MEDIUM

        MOVW    A,SP
        MOVW    A,@RW3+06H
        MOVW    @AL,AH
        
        MOVW    A,@RW3+08H
        MOVW    SP,A

#elif RSI_MEM_MODEL == COMPACT

        MOVL    A,@RW3+04H
        MOVW    RW0,A
        SWAPW
        MOV     ADB,A
        MOV     A,SSB
        MOVW    A,SP
        ADB
        MOVL    @RW0,A

        MOVL    A,@RW3+08H
        MOVW    SP,A
        SWAPW
        MOV     SSB,A

#else

        MOVL    A,@RW3+06H
        MOVW    RW0,A
        SWAPW
        MOV     ADB,A
        MOV     A,SSB
        MOVW    A,SP
        ADB
        MOVL    @RW0,A

        MOVL    A,@RW3+0AH
        MOVW    SP,A
        SWAPW
        MOV     SSB,A

#endif
        
        POPW    (RW0,RW1,RW2,RW4,RW5,RW6,RW7)
        MOVW    A, SP
        MOVW    RW3, A
        
        UNLINK

#if RSI_MEM_MODEL == SMALL || RSI_MEM_MODEL == COMPACT
        RET
#else
        RETP
#endif

;}


;====================================================================

___timer_handle:

#if RSI_MEM_MODEL == COMPACT || RSI_MEM_MODEL == LARGE

        AND     CCR, #0BFH

        MOV     A, #bnksym __sched_lock
        MOV     ADB, A
        ADB
#endif

        INCW    __sched_lock

#if RSI_MEM_MODEL == COMPACT || RSI_MEM_MODEL == LARGE
        OR      CCR, #40H
#endif

;======================================
;            (High memory)
;          |       |       |
;          |       |       | <- SSP must be here after reti instruction
;          |       AH      |
;          |       AL      |
;          |  DPR  |  ADB  |
;          |  DTB  |  PCB  |
;          |       PC      |
;          |       PS      | <- SSP must be here before reti instruction
;            (Low memory)
;======================================

#if RSI_MEM_MODEL == SMALL || RSI_MEM_MODEL == COMPACT
        CALL    ___do_tick
#else
        CALLP   ___do_tick
#endif

;======================================
        
#if RSI_MEM_MODEL == COMPACT || RSI_MEM_MODEL == LARGE

        MOV     A, #bnksym __sched_lock
        MOV     ADB, A
        ADB
#endif

        DECW    __sched_lock

        CLRB    I:TBOF

        MOVW    A, __sched_lock
        BNZ     timer_return

#if RSI_MEM_MODEL == SMALL || RSI_MEM_MODEL == COMPACT
        CALL    ___schedule
#else
        CALLP   ___schedule
#endif


timer_return:

        RETI


;====================================================================

___interrupt_handle:

#if RSI_MEM_MODEL == COMPACT || RSI_MEM_MODEL == LARGE

        MOV     A, #bnksym __sched_lock
        MOV     ADB, A
        ADB
#endif
        INCW    __sched_lock

        OR      CCR, #40H
        
        POPW    A
        SUBW    A, #5 + ___interrupt_begin
        MOVN    A, #5
        DIV     A
        
        PUSHW   A

#if RSI_MEM_MODEL == SMALL || RSI_MEM_MODEL == COMPACT
        CALL    ___interrupt_dispatch
#else
        CALLP   ___interrupt_dispatch
#endif

        ADDSP   #02
        
;-----------------------------------
#if RSI_MEM_MODEL == COMPACT || RSI_MEM_MODEL == LARGE

        MOV     A, #bnksym __sched_lock
        MOV     ADB, A
        ADB
#endif
        DECW    __sched_lock

        MOVW    A, __sched_lock
        BNZ     handle_return
        
#if RSI_MEM_MODEL == SMALL || RSI_MEM_MODEL == COMPACT
        CALL    ___schedule
#else
        CALLP   ___schedule
#endif
        
handle_return:

        RETI


;====================================================================

___interrupt_begin:

___int0:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int1:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int2:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int3:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int4:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int5:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int6:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int7:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int8:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int9:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int10:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int11:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int12:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int13:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int14:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int15:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int16:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int17:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int18:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int19:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int20:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int21:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int22:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int23:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int24:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int25:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int26:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int27:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int28:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int29:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int30:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int31:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int32:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int33:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int34:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int35:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int36:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int37:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int38:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int39:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int40:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int41:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int42:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int43:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int44:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int45:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int46:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int47:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int48:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int49:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int50:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int51:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int52:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int53:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int54:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int55:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int56:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int57:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int58:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int59:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int60:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int61:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int62:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int63:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int64:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int65:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int66:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int67:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int68:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int69:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int70:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int71:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int72:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int73:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int74:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int75:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int76:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int77:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int78:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int79:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int80:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int81:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int82:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int83:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int84:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int85:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int86:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int87:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int88:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int89:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int90:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int91:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int92:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int93:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int94:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int95:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int96:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int97:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int98:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int99:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int100:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int101:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int102:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int103:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int104:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int105:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int106:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int107:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int108:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int109:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int110:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int111:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int112:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int113:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int114:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int115:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int116:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int117:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int118:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int119:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int120:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int121:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int122:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int123:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int124:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int125:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int126:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int127:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int128:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int129:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int130:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int131:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int132:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int133:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int134:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int135:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int136:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int137:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int138:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int139:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int140:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int141:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int142:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int143:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int144:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int145:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int146:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int147:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int148:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int149:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int150:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int151:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int152:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int153:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int154:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int155:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int156:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int157:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int158:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int159:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int160:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int161:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int162:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int163:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int164:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int165:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int166:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int167:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int168:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int169:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int170:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int171:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int172:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int173:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int174:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int175:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int176:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int177:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int178:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int179:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int180:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int181:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int182:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int183:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int184:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int185:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int186:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int187:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int188:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int189:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int190:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int191:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int192:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int193:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int194:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int195:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int196:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int197:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int198:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int199:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int200:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int201:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int202:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int203:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int204:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int205:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int206:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int207:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int208:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int209:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int210:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int211:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int212:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int213:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int214:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int215:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int216:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int217:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int218:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int219:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int220:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int221:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int222:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int223:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int224:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int225:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int226:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int227:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int228:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int229:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int230:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int231:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int232:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int233:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int234:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int235:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int236:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int237:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int238:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int239:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int240:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int241:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int242:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int243:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int244:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int245:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int246:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int247:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int248:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int249:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int250:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int251:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int252:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int253:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int254:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle
___int255:
        AND     CCR, #0BFH
        CALL    ___interrupt_handle

___interrupt_end:


;====================================================================

        .SECTION INTVECT1, CONST, LOCATE=H'FFFC00


        .ORG    H'FFFC00
        .DATA.L ___int255

        .ORG    H'FFFC04
        .DATA.L ___int254

        .ORG    H'FFFC08
        .DATA.L ___int253

        .ORG    H'FFFC0C
        .DATA.L ___int252

        .ORG    H'FFFC10
        .DATA.L ___int251

        .ORG    H'FFFC14
        .DATA.L ___int250

        .ORG    H'FFFC18
        .DATA.L ___int249

        .ORG    H'FFFC1C
        .DATA.L ___int248

        .ORG    H'FFFC20
        .DATA.L ___int247

        .ORG    H'FFFC24
        .DATA.L ___int246

        .ORG    H'FFFC28
        .DATA.L ___int245

        .ORG    H'FFFC2C
        .DATA.L ___int244

        .ORG    H'FFFC30
        .DATA.L ___int243

        .ORG    H'FFFC34
        .DATA.L ___int242

        .ORG    H'FFFC38
        .DATA.L ___int241

        .ORG    H'FFFC3C
        .DATA.L ___int240

        .ORG    H'FFFC40
        .DATA.L ___int239

        .ORG    H'FFFC44
        .DATA.L ___int238

        .ORG    H'FFFC48
        .DATA.L ___int237

        .ORG    H'FFFC4C
        .DATA.L ___int236

        .ORG    H'FFFC50
        .DATA.L ___int235

        .ORG    H'FFFC54
        .DATA.L ___int234

        .ORG    H'FFFC58
        .DATA.L ___int233

        .ORG    H'FFFC5C
        .DATA.L ___int232

        .ORG    H'FFFC60
        .DATA.L ___int231

        .ORG    H'FFFC64
        .DATA.L ___int230

        .ORG    H'FFFC68
        .DATA.L ___int229

        .ORG    H'FFFC6C
        .DATA.L ___int228

        .ORG    H'FFFC70
        .DATA.L ___int227

        .ORG    H'FFFC74
        .DATA.L ___int226

        .ORG    H'FFFC78
        .DATA.L ___int225

        .ORG    H'FFFC7C
        .DATA.L ___int224

        .ORG    H'FFFC80
        .DATA.L ___int223

        .ORG    H'FFFC84
        .DATA.L ___int222

        .ORG    H'FFFC88
        .DATA.L ___int221

        .ORG    H'FFFC8C
        .DATA.L ___int220

        .ORG    H'FFFC90
        .DATA.L ___int219

        .ORG    H'FFFC94
        .DATA.L ___int218

        .ORG    H'FFFC98
        .DATA.L ___int217

        .ORG    H'FFFC9C
        .DATA.L ___int216

        .ORG    H'FFFCA0
        .DATA.L ___int215

        .ORG    H'FFFCA4
        .DATA.L ___int214

        .ORG    H'FFFCA8
        .DATA.L ___int213

        .ORG    H'FFFCAC
        .DATA.L ___int212

        .ORG    H'FFFCB0
        .DATA.L ___int211

        .ORG    H'FFFCB4
        .DATA.L ___int210

        .ORG    H'FFFCB8
        .DATA.L ___int209

        .ORG    H'FFFCBC
        .DATA.L ___int208

        .ORG    H'FFFCC0
        .DATA.L ___int207

        .ORG    H'FFFCC4
        .DATA.L ___int206

        .ORG    H'FFFCC8
        .DATA.L ___int205

        .ORG    H'FFFCCC
        .DATA.L ___int204

        .ORG    H'FFFCD0
        .DATA.L ___int203

        .ORG    H'FFFCD4
        .DATA.L ___int202

        .ORG    H'FFFCD8
        .DATA.L ___int201

        .ORG    H'FFFCDC
        .DATA.L ___int200

        .ORG    H'FFFCE0
        .DATA.L ___int199

        .ORG    H'FFFCE4
        .DATA.L ___int198

        .ORG    H'FFFCE8
        .DATA.L ___int197

        .ORG    H'FFFCEC
        .DATA.L ___int196

        .ORG    H'FFFCF0
        .DATA.L ___int195

        .ORG    H'FFFCF4
        .DATA.L ___int194

        .ORG    H'FFFCF8
        .DATA.L ___int193

        .ORG    H'FFFCFC
        .DATA.L ___int192

        .ORG    H'FFFD00
        .DATA.L ___int191

        .ORG    H'FFFD04
        .DATA.L ___int190

        .ORG    H'FFFD08
        .DATA.L ___int189

        .ORG    H'FFFD0C
        .DATA.L ___int188

        .ORG    H'FFFD10
        .DATA.L ___int187

        .ORG    H'FFFD14
        .DATA.L ___int186

        .ORG    H'FFFD18
        .DATA.L ___int185

        .ORG    H'FFFD1C
        .DATA.L ___int184

        .ORG    H'FFFD20
        .DATA.L ___int183

        .ORG    H'FFFD24
        .DATA.L ___int182

        .ORG    H'FFFD28
        .DATA.L ___int181

        .ORG    H'FFFD2C
        .DATA.L ___int180

        .ORG    H'FFFD30
        .DATA.L ___int179

        .ORG    H'FFFD34
        .DATA.L ___int178

        .ORG    H'FFFD38
        .DATA.L ___int177

        .ORG    H'FFFD3C
        .DATA.L ___int176

        .ORG    H'FFFD40
        .DATA.L ___int175

        .ORG    H'FFFD44
        .DATA.L ___int174

        .ORG    H'FFFD48
        .DATA.L ___int173

        .ORG    H'FFFD4C
        .DATA.L ___int172

        .ORG    H'FFFD50
        .DATA.L ___int171

        .ORG    H'FFFD54
        .DATA.L ___int170

        .ORG    H'FFFD58
        .DATA.L ___int169

        .ORG    H'FFFD5C
        .DATA.L ___int168

        .ORG    H'FFFD60
        .DATA.L ___int167

        .ORG    H'FFFD64
        .DATA.L ___int166

        .ORG    H'FFFD68
        .DATA.L ___int165

        .ORG    H'FFFD6C
        .DATA.L ___int164

        .ORG    H'FFFD70
        .DATA.L ___int163

        .ORG    H'FFFD74
        .DATA.L ___int162

        .ORG    H'FFFD78
        .DATA.L ___int161

        .ORG    H'FFFD7C
        .DATA.L ___int160

        .ORG    H'FFFD80
        .DATA.L ___int159

        .ORG    H'FFFD84
        .DATA.L ___int158

        .ORG    H'FFFD88
        .DATA.L ___int157

        .ORG    H'FFFD8C
        .DATA.L ___int156

        .ORG    H'FFFD90
        .DATA.L ___int155

        .ORG    H'FFFD94
        .DATA.L ___int154

        .ORG    H'FFFD98
        .DATA.L ___int153

        .ORG    H'FFFD9C
        .DATA.L ___int152

        .ORG    H'FFFDA0
        .DATA.L ___int151

        .ORG    H'FFFDA4
        .DATA.L ___int150

        .ORG    H'FFFDA8
        .DATA.L ___int149

        .ORG    H'FFFDAC
        .DATA.L ___int148

        .ORG    H'FFFDB0
        .DATA.L ___int147

        .ORG    H'FFFDB4
        .DATA.L ___int146

        .ORG    H'FFFDB8
        .DATA.L ___int145

        .ORG    H'FFFDBC
        .DATA.L ___int144

        .ORG    H'FFFDC0
        .DATA.L ___int143

        .ORG    H'FFFDC4
        .DATA.L ___int142

        .ORG    H'FFFDC8
        .DATA.L ___int141

        .ORG    H'FFFDCC
        .DATA.L ___int140

        .ORG    H'FFFDD0
        .DATA.L ___int139

        .ORG    H'FFFDD4
        .DATA.L ___int138

        .ORG    H'FFFDD8
        .DATA.L ___int137

        .ORG    H'FFFDDC
        .DATA.L ___int136

        .ORG    H'FFFDE0
        .DATA.L ___int135

        .ORG    H'FFFDE4
        .DATA.L ___int134

        .ORG    H'FFFDE8
        .DATA.L ___int133

        .ORG    H'FFFDEC
        .DATA.L ___int132

        .ORG    H'FFFDF0
        .DATA.L ___int131

        .ORG    H'FFFDF4
        .DATA.L ___int130

        .ORG    H'FFFDF8
        .DATA.L ___int129

        .ORG    H'FFFDFC
        .DATA.L ___int128

        .ORG    H'FFFE00
        .DATA.L ___int127

        .ORG    H'FFFE04
        .DATA.L ___int126

        .ORG    H'FFFE08
        .DATA.L ___int125

        .ORG    H'FFFE0C
        .DATA.L ___int124

        .ORG    H'FFFE10
        .DATA.L ___int123

        .ORG    H'FFFE14
        .DATA.L ___int122

        .ORG    H'FFFE18
        .DATA.L ___int121

        .ORG    H'FFFE1C
        .DATA.L ___int120

        .ORG    H'FFFE20
        .DATA.L ___int119

        .ORG    H'FFFE24
        .DATA.L ___int118

        .ORG    H'FFFE28
        .DATA.L ___int117

        .ORG    H'FFFE2C
        .DATA.L ___int116

        .ORG    H'FFFE30
        .DATA.L ___int115

        .ORG    H'FFFE34
        .DATA.L ___int114

        .ORG    H'FFFE38
        .DATA.L ___int113

        .ORG    H'FFFE3C
        .DATA.L ___int112

        .ORG    H'FFFE40
        .DATA.L ___int111

        .ORG    H'FFFE44
        .DATA.L ___int110

        .ORG    H'FFFE48
        .DATA.L ___int109

        .ORG    H'FFFE4C
        .DATA.L ___int108

        .ORG    H'FFFE50
        .DATA.L ___int107

        .ORG    H'FFFE54
        .DATA.L ___int106

        .ORG    H'FFFE58
        .DATA.L ___int105

        .ORG    H'FFFE5C
        .DATA.L ___int104

        .ORG    H'FFFE60
        .DATA.L ___int103

        .ORG    H'FFFE64
        .DATA.L ___int102

        .ORG    H'FFFE68
        .DATA.L ___int101

        .ORG    H'FFFE6C
        .DATA.L ___int100

        .ORG    H'FFFE70
        .DATA.L ___int99

        .ORG    H'FFFE74
        .DATA.L ___int98

        .ORG    H'FFFE78
        .DATA.L ___int97

        .ORG    H'FFFE7C
        .DATA.L ___int96

        .ORG    H'FFFE80
        .DATA.L ___int95

        .ORG    H'FFFE84
        .DATA.L ___int94

        .ORG    H'FFFE88
        .DATA.L ___int93

        .ORG    H'FFFE8C
        .DATA.L ___int92

        .ORG    H'FFFE90
        .DATA.L ___int91

        .ORG    H'FFFE94
        .DATA.L ___int90

        .ORG    H'FFFE98
        .DATA.L ___int89

        .ORG    H'FFFE9C
        .DATA.L ___int88

        .ORG    H'FFFEA0
        .DATA.L ___int87

        .ORG    H'FFFEA4
        .DATA.L ___int86

        .ORG    H'FFFEA8
        .DATA.L ___int85

        .ORG    H'FFFEAC
        .DATA.L ___int84

        .ORG    H'FFFEB0
        .DATA.L ___int83

        .ORG    H'FFFEB4
        .DATA.L ___int82

        .ORG    H'FFFEB8
        .DATA.L ___int81

        .ORG    H'FFFEBC
        .DATA.L ___int80

        .ORG    H'FFFEC0
        .DATA.L ___int79

        .ORG    H'FFFEC4
        .DATA.L ___int78

        .ORG    H'FFFEC8
        .DATA.L ___int77

        .ORG    H'FFFECC
        .DATA.L ___int76

        .ORG    H'FFFED0
        .DATA.L ___int75

        .ORG    H'FFFED4
        .DATA.L ___int74

        .ORG    H'FFFED8
        .DATA.L ___int73

        .ORG    H'FFFEDC
        .DATA.L ___int72

        .ORG    H'FFFEE0
        .DATA.L ___int71

        .ORG    H'FFFEE4
        .DATA.L ___int70

        .ORG    H'FFFEE8
        .DATA.L ___int69

        .ORG    H'FFFEEC
        .DATA.L ___int68

        .ORG    H'FFFEF0
        .DATA.L ___int67

        .ORG    H'FFFEF4
        .DATA.L ___int66

        .ORG    H'FFFEF8
        .DATA.L ___int65

        .ORG    H'FFFEFC
        .DATA.L ___int64

        .ORG    H'FFFF00
        .DATA.L ___int63

        .ORG    H'FFFF04
        .DATA.L ___int62

        .ORG    H'FFFF08
        .DATA.L ___int61

        .ORG    H'FFFF0C
        .DATA.L ___int60

        .ORG    H'FFFF10
        .DATA.L ___int59

        .ORG    H'FFFF14
        .DATA.L ___int58

        .ORG    H'FFFF18
        .DATA.L ___int57

        .ORG    H'FFFF1C
        .DATA.L ___int56

        .ORG    H'FFFF20
        .DATA.L ___int55

        .ORG    H'FFFF24
        .DATA.L ___int54

        .ORG    H'FFFF28
        .DATA.L ___int53

        .ORG    H'FFFF2C
        .DATA.L ___int52

        .ORG    H'FFFF30
        .DATA.L ___int51

        .ORG    H'FFFF34
        .DATA.L ___int50

        .ORG    H'FFFF38
        .DATA.L ___int49

        .ORG    H'FFFF3C
        .DATA.L ___int48

        .ORG    H'FFFF40
        .DATA.L ___int47

        .ORG    H'FFFF44
        .DATA.L ___int46

        .ORG    H'FFFF48
        .DATA.L ___int45

        .ORG    H'FFFF4C
        .DATA.L ___int44

        .ORG    H'FFFF50
        .DATA.L ___int43

        .ORG    H'FFFF54
        .DATA.L ___int42

        .ORG    H'FFFF58
        .DATA.L ___int41

        .ORG    H'FFFF5C
        .DATA.L ___int40

        .ORG    H'FFFF60
        .DATA.L ___int39

        .ORG    H'FFFF64
        .DATA.L ___int38

        .ORG    H'FFFF68
        .DATA.L ___int37

        .ORG    H'FFFF6C
        .DATA.L ___timer_handle

        .ORG    H'FFFF70
        .DATA.L ___int35

        .ORG    H'FFFF74
        .DATA.L ___int34

        .ORG    H'FFFF78
        .DATA.L ___int33

        .ORG    H'FFFF7C
        .DATA.L ___int32

        .ORG    H'FFFF80
        .DATA.L ___int31

        .ORG    H'FFFF84
        .DATA.L ___int30

        .ORG    H'FFFF88
        .DATA.L ___int29

        .ORG    H'FFFF8C
        .DATA.L ___int28

        .ORG    H'FFFF90
        .DATA.L ___int27

        .ORG    H'FFFF94
        .DATA.L ___int26

        .ORG    H'FFFF98
        .DATA.L ___int25

        .ORG    H'FFFF9C
        .DATA.L ___int24

        .ORG    H'FFFFA0
        .DATA.L ___int23

        .ORG    H'FFFFA4
        .DATA.L ___int22

        .ORG    H'FFFFA8
        .DATA.L ___int21

        .ORG    H'FFFFAC
        .DATA.L ___int20

        .ORG    H'FFFFB0
        .DATA.L ___int19

        .ORG    H'FFFFB4
        .DATA.L ___int18

        .ORG    H'FFFFB8
        .DATA.L ___int17

        .ORG    H'FFFFBC
        .DATA.L ___int16

        .ORG    H'FFFFC0
        .DATA.L ___int15

        .ORG    H'FFFFC4
        .DATA.L ___int14

        .ORG    H'FFFFC8
        .DATA.L ___int13

        .ORG    H'FFFFCC
        .DATA.L ___int12

        .ORG    H'FFFFD0
        .DATA.L ___int11

        .ORG    H'FFFFD4
        .DATA.L ___int10

        .ORG    H'FFFFD8
        .DATA.L ___int9

        ;.ORG    H'FFFFDC
        ;.DATA.L ___int8
        
        .SECTION INTVECT2, CONST, LOCATE=H'FFFFE0

        .ORG    H'FFFFE0
        .DATA.L ___int7

        .ORG    H'FFFFE4
        .DATA.L ___int6

        .ORG    H'FFFFE8
        .DATA.L ___int5

        .ORG    H'FFFFEC
        .DATA.L ___int4

        .ORG    H'FFFFF0
        .DATA.L ___int3

        .ORG    H'FFFFF4
        .DATA.L ___int2

        .ORG    H'FFFFF8
        .DATA.L ___int1

        .ORG    H'FFFFFC
        .DATA.L ___int0


;*===========================================================================*/
        .END
        

