;====================================================================
; $Id: START.ASM,v 3.8 2004/04/15 15:37:46 hloesc Exp $
;====================================================================
; THIS SAMPLE CODE IS PROVIDED AS IS AND IS SUBJECT TO ALTERATIONS.  
;  FUJITSU MICROELECTRONICS ACCEPTS NO RESPONSIBILITY OR LIABILITY   
;         FOR ANY ERRORS OR ELIGIBILITY FOR ANY PURPOSES.            
;                                                                    
;    Startup file for memory and basic controller initialisation     
;                                                                    
;    MB90500/MB90300/MB90400/MB90800/MB90600 Series C Compiler,      
;    (compatible but untested for MB90700/MB90700H/MB90200)          
;                                                                    
;    (C) FUJITSU MICROELECTRONICS EUROPE 1998-2003                   
;====================================================================

          .PROGRAM  STARTUP  
          .TITLE    "STARTUP FILE FOR MEMORY INITIALISATION"

;====================================================================
; 1  Contents
;====================================================================
; 1   Contents
; 2   Disclaimer
; 3   History
;
; 4   SETTINGS              (USER INTERFACE)
; 4.1   Controller Family
; 4.2   Memory model
; 4.3   Constant Data Handling
; 4.4   Stack Type and Stack Size
; 4.5   General Register Bank
; 4.6   Low-Level Library Interface
; 4.7   Clock Selection
; 4.8   External Bus Interface
; 4.9   Reset Vector
; 4.10  Enable RAMCODE Copying
; 4.11  Enable information stamps in ROM
;
; 5   Section and Data Declaration
; 5.1   Several fixed addresses (fixed for MB90xxx controllers)
; 5.2   Declaration of __near addressed data sections
; 5.3   Declaration of RAMCODE section and labels
; 5.4   Declaration of sections containing other sections description
; 5.5   Stack area and stack top definition
; 5.6   Direct page register dummy label definition
;
; 6   Start-Up Code
; 6.1   Import external symbols
; 6.2   Program start (the reset vector should point here)
; 6.3   "NOT RESET YET" WARNING
; 6.4   Initialisation of processor status
; 6.5   Set clock ratio 
; 6.6   Set external bus configuaration
; 6.7   Copy initial values to data areas.
; 6.8   Clear uninitialised data areas to zero
; 6.9   Prepare stacks and set the active stack type
; 6.10  Set Data Bank Register (DTB) and Direct Page Register (DPR)
; 6.11  Wait for PLL to stabilise
; 6.12  Initialise Low-Level Library Interface
; 6.13  Call C-language main function
; 6.14  Shut down library
; 6.15  Program end loop
; 6.16  Configuration stamp in ROM (currently test only)
; 6.17  Debug address specification
;
;====================================================================
; 2  Disclaimer
;====================================================================
;                  FUJITSU MICROELECTRONICS EUROPE GMBH
;                  Am Siebenstein 6-10, 63303 Dreieich          
;                  Tel.:++49 6103 690-0, Fax -122
;                                                               
;    The following software is for demonstration purposes only. 
;    It is not fully tested, nor validated in order to fulfil 
;    its task under all circumstances. Therefore, this software 
;    or any part of it must only be used in an evaluation       
;    laboratory environment.                                    
;    This software is subject to the rules of our standard      
;    DISCLAIMER, that is delivered with our SW-tools on the CD 
;    "Microcontrollers - Documentation & Software" (V3.4) or
;    see our Internet Page -                                      
;    http://www.fme.fujitsu.com/products/micro/disclaimer.html

;
;====================================================================
; 3  History
;====================================================================
; $Id: START.ASM,v 3.8 2004/04/15 15:37:46 hloesc Exp $

#define VERSION  "3.08"
/*
$Log: START.ASM,v $
Revision 3.8  2004/04/15 15:37:46  hloesc
- missing underscore in __psccr (copy and paste error)

Revision 3.7  2004/04/05 10:33:53  hloesc
- missing colon after CVS "Log" command

Version 3.06      05 April 2004,  HLo
- checked in CVS
- changed back to manual setting of EXTENDED_PLL, since clearing register could
  be necessary, if exist

Version 3.05      01 April 2004,  HLo
- removed setting EXTENDED_PLL
  It is now set automatically, if PLL factor is set higher than 4
- changed _psccr reference to __psccr

Version 3.04      02. Jan 2004,  HLo
- removed fixed address PSCCR, because it is not fixed

Version 3.03      18. Dec 2003,  HLo
- configuration option for Version stamp and Configuration stamp
- DATA2 and INIT2 added to list of cleared/initialised sections

Version 3.02      26. June 2003  HLo
- more NOP instructions after PLL disable for faster PLL types
- on default stack is filled with pattern
- size alignment for stack re-added 
- initialise stack by stack symbols to force linkage of
  externaly defined stack or to force linker error
- label _start plus one NOP moved before "notresetwarning", in 
  order to achieve same cycle count for evaluation chip and Flash
  chip with fixed reset vector.
- pre-setting stack with pattern is now default

Version 3.01      12 May 2003  HWe
- ADDRESSMODE for External Bus Interface added


Version 3.00      30 Jan 2003  HLo
- branch 2.02b merged to 3.00: added NOP after PLL disable
- stack size specified in bytes now
- new: additional PLL factors for MB90340/350/480
- new: option to preset stack with pattern
- new: symbols to access stack in C
- new: option to skip reservation of stack area
- new: LX-family MB90800 added

Version 2.03      06 Feb 2002  HLo
- company name/link updated 
- CONST2/CINIT2 section added to support split CONST area

Version 2.02      20 Aug 2001  NPo
- MB90300 family added

Branch  2.02b     16 Nov 99    MSt
- insert wait loop to check if PLL has stopped

Version 2.01      11 Oct 99    HLo
- unused macros WIDTH_8 and WIDTH_16 removed

Version 2.00      23 July 99    HLo
- decriptors of __near sections now in DCLEAR/DTRANS
- code for __near section init removed, now initialised by DCLEAR
  and DTRANS processing
- RAMCODE option added
- table of contents added
- memory model AUTO renamed to AUTOMODEL
- CONSTDATA mode AUTOCONST introduced to make it clearer

Version 1.12      23 June 99    HLo
- CLIBINIT is using ON/OFF instead of extra macro now

Version 1.11      1 June 99     HLo
- confusing old line with comment removed, was refering to unused
  old BUSWIDTH

Version 1.10      17 May 99     HLo
- change in 1.09 (MODEBYTE) related to BUSWIDTH was not complete

Version 1.09      12 May 99     HLo
- MB90400 family added
- several coments changed
- INTROM_EXTBUS macro was wrong
- BUSWIDTH macro removed, now resolved from bus signal
- disclaimer added
- disabling interrupts added

Version 1.08      16 April 99   JRo
- Version placed in separate section (caused problems with fixed
  reset vector)
  
Version 1.07      01 April 99   HLo
- Version string had wrong number (1.05 instead of 1.06)
- Copyright slightly changed

Version 1.06      18 Feb 99   HLo
- default external bus configuration: WR signal enabled
- ROMMIRROR macro processing simplified (less warnings>

Version 1.05      28 Oct 98   HLo
- CALL/CALLP _exit was not differed

Version 1.04      21 Oct 98   HLo
- ROM mirror option added
- _exit call added
- bug: EQU ON/OFF move to upper lines 

Version 1.03      19 Oct 98   HLo
- bug: SEGCOPY macro: used size changed from sizeof(src) to 
  sizeof(dest). It was conflicting with RAMCONST, if compiler
  is set to ROMCONST.

Version 1.02      16 Oct 98   HLo
- memory model AUTO introduced (use far calls only and repair
  stack, if necessary
- colons removed from EQU labels
- stream_init call added
- RAMCONST set as default (also for ROMCONST systems)

Version 1.01      31 Aug 98   HLo
- bug: conditional for reset vector was missing

Version 1.00      25 Aug 98   HLo
- original version
*/
;====================================================================

;====================================================================
; 4  Settings
;====================================================================
;
; CHECK ALL OPTIONS WHETHER THEY FIT TO THE APPLICATION;
;
; Configure this startup file in the "Settings" section. Search for
; comments with leading "; <<<". This points to the items to be set.
;====================================================================
#set      OFF       0
#set      ON        1

;====================================================================
; 4.1  Controller Family
;====================================================================

#set      MB90700   0             ; FFMC-16
#set      MB90200   1             ; FFMC-16F
#set      MB90600   2             ; FFMC-16L
#set      MB90500   3             ; FFMC-16LX
#set      MB90400   4             ; FFMC-16LX
#set      MB90300   5             ; FFMC-16LX
#set      MB90800   6             ; FFMC-16LX

#set      FAMILY    MB90500        ; <<< select family

; NOTE: This startup file is not suitable for MB90610 series. Use
; start610.asm instead.

;====================================================================
; 4.2  C-language Memory model
;====================================================================

                                   ;      data      code   
#set      SMALL     0              ;     16 Bit    16 Bit
#set      MEDIUM    1              ;     16 Bit    24 Bit
#set      COMPACT   2              ;     24 Bit    16 Bit
#set      LARGE     3              ;     24 Bit    24 Bit
#set      AUTOMODEL 4              ; works always, might occupy two
                                   ; additional bytes


#set      MEMMODEL  AUTOMODEL      ; <<< C-memory model

; The selected memory model should be set in order to fit to the
; model selected for the compiler. 
; Note, in this startup version AUTOMODEL will work for all
; C-models. However, if the compiler is configured for SMALL or
; COMPACT, two additional bytes on stack are occupied. If this is not
; acceptable, the above setting should be set to the correct model.

;====================================================================
; 4.3  Constant Data Handling
;====================================================================

#set      ROMCONST    0            ; works only with compiler ROMCONST
#set      RAMCONST    1            ; works with BOTH compiler settings
#set      AUTOCONST   RAMCONST     ; works with BOTH compiler settings
                              
#set      CONSTDATA   AUTOCONST    ; <<< set RAM/ROM/AUTOCONST

; - AUTOCONST (default) is a the same as RAMCONST
; - RAMCONST/AUTOCONST should always work, even if compiler is set to
;   ROMCONST. If compiler is set to ROMCONST and this startup file is
;   set to RAMCONST or AUTOCONST, this startup file will generate an
;   empty section CINIT in RAM. However, the code, which copies from 
;   CONST to CINIT will not have any effect, because size of section 0.
; - It is highly recommended to set the compiler to ROMCONST for 
;   single-chip mode or internal ROM+ext bus. The start-up file 
;   should be set to AUTOCONST.
; - ROMCONST setting on systems with full external bus requires exter-
;   nal address mapping.
;   Single-chip can be emulated by the emulator debugger.
;   ROM mirror can also be used with simulator.
;
; see also MIRROR options of external bus settings

;====================================================================
; 4.4  Stack Type and Stack Size
;====================================================================

#set      USRSTACK      0    ; user stack: for main program
#set      SYSSTACK      1    ; system stack: for main program and interrupts

#set      STACKUSE  SYSSTACK    ; <<< set active stack

#set      STACK_RESERVE ON      ; <<< reserve stack area in this module
#set      STACK_SYS_SIZE 128     ; <<< byte size of System stack
#set      STACK_USR_SIZE 0      ; <<< byte size of User stack 

#set      STACK_FILL    ON      ; <<< fills the stack area with pattern
#set      STACK_PATTERN 0x55AA  ; <<< the pattern to write to stack

; - If the active stack is set to SYSSTACK, it is used for main program
;   and interrupts. In this case, the user stack could be set to a dummy
;   size.
;   If the active stack is set to user stack, it is used for the main
;   program but the system stack s automatically activated, if an inter-
;   rupt is serviced. Both stack areas must have a reasonable size.
; - If STACK_RESERVE is ON, the sections USTACK and SSTACK are reserved 
;   in this module. Otherwise, they have to be reserved in other modules.
;   If STACK_RESERVE is OFF, the size definitions STACK_SYS_SIZE and 
;   STACK_USR_SIZE have no meaning.
; - Even if they are reverved in other modules, they are still initialised
;   in this start-up file.
; - Filling the stack with pattern allows to dynamically check the stack 
;   area, which had already been used.
;
; - If only system stack is used and SSB is linked to a different bank
;   than USB, make sure that all C-modules (which generate far pointers 
;   to stack data) have "#pragma SSB". Applies only to exclusive confi-
;   gurations.
; - Note, several library functions require quite a big stack (due to 
;   ANSI). Check the stack information files (*.stk) in the LIB\907
;   directory.

;====================================================================
; 4.5  General Register Bank
;====================================================================

#set      REGBANK   0              ; <<< set default register bank

; set the General Register Bank that is to be used after startup.
; Usually, this is bank 0, which applies to address H'180..H'18F. Set
; in the range from 0 to 31.
; Note: All used register banks have to be reserved (linker options).

#if REGBANK > 31 || REGBANK < 0
#  error REGBANK setting out of range
#endif

;====================================================================
; 4.6  Low-Level Library Interface
;====================================================================

#set          CLIBINIT  OFF        ; <<< select extended libray usage

; This option has only to be set, if stream-IO/standard-IO function of
; the C-libraray have to be used (printf(), fopen()...). This also 
; requires low-level functions to be defined by the application 
; software.
; For other library functions like (e.g. sprintf()) all this is not
; necessary. However, several functions consume a large amount of stack.

;====================================================================
; 4.7  Clock Selection
;====================================================================

#set      NOCLOCK        0         ; do not touch CKSCR register
#set      MAINCLOCK      1         ; select main clock (1/2 external)
#set      PLLx1          2         ; set PLL to x1 ext. clock/quartz
#set      PLLx2          3         ; set PLL to x2 ext. clock/quartz
#set      PLLx3          4         ; set PLL to x3 ext. clock/quartz
#set      PLLx4          5         ; set PLL to x4 ext. clock/quartz

; next factors are for specific MCU only
; Check datasheet or header file for presence of register PSCCR. The
; header file (assembler part) must be member in project, so linker
; can find the extended PLL register.
#set      PLLx6          6         ; set PLL to x6 ext. clock/quartz
#set      PLLx8          7         ; set PLL to x8 ext. clock/quartz


#set      CLOCKSPEED    PLLx4      ; <<< set PLL ratio
#set      CLOCKWAIT      ON        ; <<< wait for stabilized PLL, if
                                   ;     PLL is used
#set      EXTENDED_PLL   OFF       ; <<< set ON, if PSCCR exists

; The clock is set quiet early. However, if CLOCKWAIT is ON, polling 
; for machine clock to be switched to PLL is done at the end of this
; file. Therefore, the stabilization time is not wasted. Main() will
; finally start at correct speed. Resources can immediately be used.
;
; This startup file version does not support subclock.
; Note, for controllers with extended PLL and sub-clock the sub-clock
; divider is set to 4.

#if EXTENDED_PLL == ON
#  define PSCCR __psccr
          .import PSCCR            ; import definition from IO-file
#else

#  if CLOCKSPEED > PLLx4
#    error: clock > factor 4 requires spec. MCU and EXTENDED__PLL == ON
#  endif

#endif

;====================================================================
; 4.8  External Bus Interface
;====================================================================

#set      SINGLE_CHIP    0         ; all internal
#set      INTROM_EXTBUS  1         ; mask ROM, FLASH, or OTP ROM used
#set      EXTROM_EXTBUS  2         ; full external bus (INROM not used) 

#set      BUSMODE SINGLE_CHIP      ; <<< set bus mode (see mode pins)

#set      MULTIPLEXED     0        ; 
#set      NON_MULTIPLEXED 1        ; only if supported by the device

#set      ADDRESSMODE MULTIPLEXED  ; <<< set address-mode

; Some devices support multiplexed and/or non-multiplexed Bus mode
; please refer to the related datasheet/hardwaremanual

#set      ROMMIRROR      ON        ; <<< ROM mirror function ON/OFF
                                   ;     MB90500/400/300/800 family only

; In Internal ROM / External Bus mode one can select whether to mirror
; area FF4000..FFFFFF to 004000..00FFFF. This is necessary to get the 
; compiler ROMCONST option working. However, if ROMCONST is not used,
; this area might be used to access external memory. This is intended
; to increase performance, if a lot of dynamic data have to be accessed.
; In SMALL and MEDIUM model these data can be accessed within bank 0,
; which allows to use near addressing.
; These controller without the ROMM-control register always have the
; mirror function on in INROM mode.

; If BUSMODE is "SINGLE_CHIP", ignore remaining bus settings.

#set      AUTOWAIT_IO    0         ; <<< 0..3 waitstates for IO area
#set      AUTOWAIT_LO    0         ; <<< 0..3 for lower external area
#set      AUTOWAIT_HI    0         ; <<< 0..3 for higher external area

#set      ADDR_PINS B'00000000     ; <<< select used address lines 
                                   ;     A23..A16 to be output.
; This is the value to be set in HACR-register. "1" means: pin used as
; IO-port. (B'10000000 => A23 not used, B'00000001 => A16 not used)

#set      BUS_SIGNAL B'00000100    ; <<< enable bus control signals 
;                      |||||||+-- ignored
;                      ||||||+--- bus width lower memory (0:16, 1:8Bit)
;                      |||||+---- output WR signal(s)    (1: enabled  )
;                      ||||+----- bus width upper memory (0:16, 1:8Bit)
;                      |||+------ bus width ext IO area  (0:16, 1:8Bit)
;                      ||+------- enable HRQ input       (1: enabled  )
;                      |+-------- enable RDY input       (1: enabled  )
;                      +--------- output CLK signal      (1:enabled   )

; These settings correspond to the EPCR-register.
; Hint: Except for MB90500/400/300/800 devices the clock output is
; needed for external RDY synchronisation, if Ready function is used.
; Hint: Don't forget to enable WR signals, if external RAM has to be
; written to.

#set      iARSR     ((AUTOWAIT_IO<<6)|((AUTOWAIT_HI&3)<<4)|(AUTOWAIT_LO&3))

;====================================================================
; 4.9  Reset Vector
;====================================================================

#set      RESET_VECTOR   ON        ; <<< enable reset vector

#if BUSMODE == SINGLE_CHIP
#  set    MODEBYTE       0

#else                              ; external bus
#  set MODEBYTE (((BUSMODE&3)<<6) | ((ADDRESSMODE&1)<<4) | ((~BUS_SIGNAL)&8))

#endif

; Above setting can also be used, if all other interrupt vectors are
; specified via "pragma intvect". Only if interrupts 0..7 are specified
; via "pragma intvect", this will conflict with the vector in this
; module. The reason is the INTVECT section, which includes the whole
; area from the lowest to the highest specified vector.

#if RESET_VECTOR == ON
          .SECTION        RESVECT, CONST, LOCATE=H'FFFFDC
          .DATA.E _start
          .DATA.B MODEBYTE
#endif

;====================================================================
; 4.10  Enable RAMCODE Copying
;====================================================================

#set      COPY_RAMCODE      OFF     ; <<< enable RAMCODE section to
                                    ; be copied from ROM to RAM

; to get this option properly working the code to be executed has to
; be linked to section RAMCODE (e.g. by #pragma section). The section
; RAMCODE has be located in RAM and the section @RAMCODE has to be
; located at a fixed address in ROM by linker settings.

;====================================================================
; 4.11  Enable information stamps in ROM
;====================================================================

#set      VERSION_STAMP     OFF     ; <<< enable version number in
                                    ; separated section

#set      CONFIG_STAMP      OFF     ; <<< enable bit pattern with 
                                    ; current configuration at end of
                                    ; start module (a few bytes)

#if VERSION_STAMP == ON
          .SECTION  VERSIONS, CONST ; change name, if necessary
          .SDATA    "Start ", VERSION, "\n\0" 
#endif

; <<< END OF SETTINGS >>>

;====================================================================
; 5  Section and Data Declaration
;====================================================================

;====================================================================
; 5.1  Several fixed addresses (fixed for MB90xxx controllers)
;====================================================================

LPMCR     .EQU      0xA0           ; Low power mode control register
CKSCR     .EQU      0xA1           ; Clock select control register
#if BUSMODE != SINGLE_CHIP
ARSR      .EQU      0xA5           ; *1) Automatic ready function reg
HACR      .EQU      0xA6           ; *1) External address output reg
EPCR      .EQU      0xA7           ; *1) Bus control signal selection
#endif
#if FAMILY == MB90500 || FAMILY == MB90400 || FAMILY == MB90300 || FAMILY == MB90800
ROMM      .EQU      0x6F           ; *2) ROM mirror control register
#endif
WDTC      .EQU      0xA8           ; Watchdog control register
TBTC      .EQU      0xA9           ; Timerbase timer control register

; *1 only for devices with external bus
; *2 only for MB905xx (FFMC-16LX) devices

;====================================================================
; 5.2  Declaration of __near addressed data sections
;====================================================================

; sections to be cleared
          .SECTION  DATA,      DATA,   ALIGN=2  ; zero clear area
          .SECTION  DATA2,     DATA,   ALIGN=2  ; zero clear area
          .SECTION  DIRDATA,   DIR,    ALIGN=2  ; zero clear direct
          .SECTION  LIBDATA,   DATA,   ALIGN=2  ; zero clear lib area

; sections to be initialised with start-up values
          .SECTION  INIT,      DATA,   ALIGN=2  ; initialised area
          .SECTION  INIT2,     DATA,   ALIGN=2  ; initialised area
          .SECTION  DIRINIT,   DIR,    ALIGN=2  ; initialised dir
          .SECTION  LIBINIT,   DATA,   ALIGN=2  ; initialised lib area
#if CONSTDATA == RAMCONST
          .SECTION  CINIT,     DATA,   ALIGN=2  ; initialised const
          .SECTION  CINIT2,    DATA,   ALIGN=2  ; initialised const
#endif

; sections containing start-up values for initialised sections above
          .SECTION  DCONST,    CONST,  ALIGN=2  ; DINIT initialisers
          .SECTION  DIRCONST, DIRCONST,ALIGN=2  ; DIRINIT initialisers
          .SECTION  LIBDCONST, CONST,  ALIGN=2  ; LIBDCONST init val

          ; following setion is either copied to CINIT (RAMCONST) or
          ; mapped by ROM-mirror function (ROMCONST)
          .SECTION  CONST,     CONST,  ALIGN=2  ; CINIT initialisers
          .SECTION  CONST2,    CONST,  ALIGN=2  ; CINIT initialisers

;====================================================================
; 5.3  Declaration of RAMCODE section and labels
;====================================================================

#if COPY_RAMCODE == ON
          .SECTION  RAMCODE,   CODE,  ALIGN=1  
          .IMPORT _RAM_RAMCODE                  ; provided by linker
          .IMPORT _ROM_RAMCODE                  ; provided by linker
#endif


;====================================================================
; 5.4  Declaration of sections containing other sections description
;====================================================================

; DCLEAR contains start address and size of all sections to be cleared
; DTRANS contains source and destination address and size of all 
; sections to be initialised with start-up values
; The compiler automatically adds a descriptor for each __far addressed
; data section to DCLEAR or DTRANS. These __far section are separated 
; for each C-module.

; In addition the start-up file adds the descriptors of the previously
; declared __near section here. This way the same code in the start-up
; file can be used for initialising all sections.

   .SECTION  DCLEAR,    CONST,  ALIGN=2  ; zero clear table
   ;    Address         Bank            Size
   .DATA.H DATA,    BNKSEC DATA,    SIZEOF(DATA   )
   .DATA.H DIRDATA, BNKSEC DIRDATA, SIZEOF(DIRDATA)
   .DATA.H LIBDATA, BNKSEC LIBDATA, SIZEOF(LIBDATA)

   .SECTION  DTRANS,    CONST,  ALIGN=2  ; copy table
   ;    Address         Bank          Address     Bank          Size
   .DATA.H DCONST,   BNKSEC DCONST,   INIT,   BNKSEC INIT,   SIZEOF INIT   
   .DATA.H DIRCONST, BNKSEC DIRCONST, DIRINIT,BNKSEC DIRINIT,SIZEOF DIRINIT
   .DATA.H LIBDCONST,BNKSEC LIBDCONST,LIBINIT,BNKSEC LIBINIT,SIZEOF LIBINIT

#if CONSTDATA == RAMCONST
   .DATA.H CONST,    BNKSEC CONST,    CINIT,  BNKSEC CINIT,  SIZEOF CINIT  
   .DATA.H CONST2,   BNKSEC CONST,    CINIT2, BNKSEC CINIT2, SIZEOF CINIT2
#endif

#if COPY_RAMCODE == ON
   .DATA.L _ROM_RAMCODE, _RAM_RAMCODE
   .DATA.H SIZEOF RAMCODE
#endif

;====================================================================
; 5.5  Stack area and stack top definition/declaration
;====================================================================
#if STACK_RESERVE == ON
            .SECTION  SSTACK, STACK, ALIGN=2

            .EXPORT __systemstack, __systemstack_top
__systemstack:
            .RES.B    (STACK_SYS_SIZE + 1) & 0xFFFE
__systemstack_top:
SSTACK_TOP:



            .SECTION  USTACK, STACK, ALIGN=2

            .EXPORT __userstack, __userstack_top
__userstack:
            .RES.B    (STACK_USR_SIZE + 1) & 0xFFFE
__userstack_top:
USTACK_TOP:

#else
            .SECTION  SSTACK, STACK, ALIGN=2
            .SECTION  USTACK, STACK, ALIGN=2

            .IMPORT __systemstack, __systemstack_top
            .IMPORT __userstack, __userstack_top
#endif

;====================================================================
; 5.6  Direct page register dummy label definition
;====================================================================

          .SECTION  DIRDATA  ; zero clear direct
DIRDATA_S:                                      ; label for DPR init       

; This label is used to get the page of the __direct data.
; Depending on the linkage order  order this startup file the label is
; placed anywhere within the __direct data page. However, the
; statement "PAGE (DIRDATA_S)" is processed. Therefore, the lower 
; 8 Bit of the address of DIRDATA_S are not relevant and this feature 
; becomes linkage order independent. 
; Note, the linker settings have to make sure that the all __direct
; data are located within the same physical page (256 Byte block).

;====================================================================
; 6  Start-Up Code
;====================================================================

;====================================================================
; 6.1  Import external symbols
;====================================================================

          .IMPORT   _main                    ; user code entrance
#if CLIBINIT == ON
          .IMPORT   __stream_init
          .IMPORT   _exit
          .EXPORT   __exit
#endif          
          .EXPORT   _start

;====================================================================
;   ___  _____   __    ___  _____
;  /       |    /  \  |   \   |                  
;  \___    |   |    | |___/   |   
;      \   |   |----| |  \    |   
;   ___/   |   |    | |   \   |      Begin of actual code section
;====================================================================
          .SECTION  CODE_START, CODE, ALIGN=1

;====================================================================
; 6.2  Program start (the reset vector should point here)
;====================================================================
_start:
          NOP  ; This NOP is only for debugging. On debugger the IP
               ; (instruction pointer) should point here after reset

;====================================================================
; 6.3  "NOT RESET YET" WARNING
;====================================================================
notresetyet:
          NOP  ; read hint below!!!!!!!
; If the debugger stays at this NOP after download, the controller has
; not been reset yet. In order to reset all hardware register it is
; highly recommended to reset the controller.
; However, if no reset vector has been defined on purpose, this start
; address can also be used.
; This mechanism is using the .END instruction at the end of this mo-
; dule. It is not necessary for controller operation but improves 
; security during debugging (mainly emulator debugger).
; If the debugger stays here after a single step from label "_start"
; to label "notresetyet", this note can be ignored.

;====================================================================
; 6.4  Initialisation of processor status
;====================================================================
          AND  CCR, #0             ; disable interrups
          MOV  ILM,#7              ; set interrupt level mask to ALL
          MOV  RP,#REGBANK         ; set register bank pointer 

;====================================================================
; 6.5  Set clock ratio (ignore subclock)
;====================================================================
#if CLOCKSPEED != NOCLOCK
          SETB I:CKSCR:2           ; set main clock
#  if CLOCKSPEED > MAINCLOCK
          NOP                      ; ensure that PLL is stopped before
          NOP                      ; writing to CKSCR and PSCCR, the  
          NOP                      ; instructions after NOP and before
          NOP                      ; writing CKSCR/PSCCR do not give 
                                   ; enough cycles (n = f_pll/f_main)

          MOV  A, I:CKSCR          ; copy clock register
          AND  A, #0xFC            ; set x1 for PLL
#    if CLOCKSPEED == PLLx2
          OR   A, #0x01            ; set x2 for PLL
#    elif CLOCKSPEED == PLLx3 || CLOCKSPEED == PLLx6
          OR   A, #0x02            ; set x3 for PLL
#    elif CLOCKSPEED == PLLx4 || CLOCKSPEED == PLLx8
          OR   A, #0x03            ; set x4 for PLL
#    endif

#if EXTENDED_PLL == ON
#  if CLOCKSPEED == PLLx6 || CLOCKSPEED == PLLx8
          MOV PSCCR, #0x01       ; double the factor
#  else
          MOV PSCCR, #0x00       ; no doubling

#  endif ; CLOCKSPEED
#endif ; EXTENDED_PLL

          MOV  I:CKSCR, A          ; write back 
          CLRB I:CKSCR:2           ; enable PLL, PLL is not switched
                                   ; to the MCU yet but after stabi-
                                   ; lizing it switchs on its own to
                                   ; higher speed (see below)
#  endif ; CLOCKSPEED > MAINCLOCK 
#endif ; CLOCKSPEED != NOCLOCK 

;====================================================================
; 6.6  Set external bus configuaration
;====================================================================

#if BUSMODE != SINGLE_CHIP         ; ext bus used
          MOV  I:HACR, #ADDR_PINS  ; set used upper address lines
          MOV  I:EPCR, #BUS_SIGNAL ; set used bus signals
          MOV  I:ARSR, #iARSR      ; set auto-wait cycles
#endif 

#if FAMILY == MB90500 || FAMILY == MB90400 || FAMILY == MB90300 || FAMILY == MB90800
; only these have ROMM

#  if BUSMODE == INTROM_EXTBUS     ; EXTBUS and INTROM/EXTROM
#    if ROMMIRROR == OFF && CONSTDATA == ROMCONST
#      error Mirror function must be ON to mirror internal ROM
#    endif
#  endif

          MOV  I:ROMM, #ROMMIRROR
#endif

;====================================================================
; 6.7  Copy initial values to data areas.
;====================================================================
;
; Each C-module has its own __far INIT section. The names are generic.
; DCONST_module contains the initialisers for the far data of the one
; module. INIT_module reserves the RAM area, which has to be loaded
; with the data from DCONST_module. ("module" is the name of the *.c
; file) 
; All separated DCONST_module/INIT_module areas are described in 
; DTRANS section by start addresses and length of each far section.
;   0000 1. source address (ROM)
;   0004 1. destination address (RAM)
;   0008 length of sections 1
;   000A 2. source address  (ROM)
;   000E 2. destination address (RAM)
;   0012 length of sections 2
;   0014 3. source address ...
; In addition the start-up file adds the descriptors of the __near 
; sections to this table. The order of the descriptors in this table 
; depends on the linkage order.
;====================================================================
          MOV  A, #BNKSEC DTRANS   ; get bank of table
          MOV  DTB, A              ; store bank in DTB
          MOVW RW1, #DTRANS        ; get start offset of table
          OR   CCR, #H'20          ; System stack flag set (SSB used)
          BRA  LABEL2              ; branch to loop condition
LABEL1:
          MOVW A, @RW1+6           ; get bank of destination
          MOV  SSB, A              ; save dest bank in SSB
          MOVW A, @RW1+2           ; get source bank
          MOV  ADB, A              ; save source bank in ADB
          MOVW A, @RW1+4           ; move destination addr in AL
          MOVW A, @RW1             ; AL ->AH, src addr -> AL 
          MOVW RW0, @RW1+8         ; number of bytes to copy -> RW0
          MOVSI     SPB, ADB       ; copy data
          MOVN A, #10              ; length of one table entry is 10
          ADDW RW1, A              ; set pointer to next table entry
LABEL2:                             
          MOVW A, RW1              ; get address of next block
          SUBW A, #DTRANS          ; sub address of first block
          CMPW A, #SIZEOF (DTRANS) ; all blocks processed ?
          BNE  LABEL1              ; if not, branch

;====================================================================
; 6.8   Clear uninitialised data areas to zero
;====================================================================
;
; Each C-module has its own __far DATA section. The names are generic.
; DATA_module contains the reserved area (RAM) to be cleared.
; ("module" is the name of the *.c file) 
; All separated DATA_module areas are described in DCLEAR section by
; start addresses and length of all far section.
;   0000 1. section address (RAM)
;   0004 length of section 1
;   0006 2. section address (RAM)
;   000A length of section 2
;   000C 3. section address (RAM)
;   0010 length of section 3 ...
; In addition the start-up file adds the descriptors of the __near 
; section. to this table. The order of the descriptors in this table 
; depends on the linkage order.
;====================================================================
          MOV  A, #BNKSEC DCLEAR   ; get bank of table
          MOV  DTB, A              ; store bank in DTB
          MOVW RW1, #DCLEAR        ; get start offset of table
          BRA  LABEL4              ; branch to loop condition
LABEL3:                            
          MOV  A, @RW1+2           ; get section bank
          MOV  ADB, A              ; save section bank in ADB
          MOVW RW0, @RW1+4         ; number of bytes to copy -> RW0
          MOVW A, @RW1             ; move section addr in AL
          MOVN A, #0               ; AL ->AH, init value -> AL 
          FILSI     ADB            ; write 0 to section
          MOVN A, #6               ; length of one table entry is 6
          ADDW RW1, A              ; set pointer to next table entry
LABEL4:
          MOVW A, RW1              ; get address of next block
          SUBW A, #DCLEAR          ; sub address of first block
          CMPW A, #SIZEOF (DCLEAR) ; all blocks processed ?
          BNE  LABEL3              ; if not, branch

;====================================================================
; 6.9  Prepare stacks and set the default stack type
;====================================================================

          AND  CCR,#H'DF            ; clear system stack flag
          MOVL A, #(__userstack_top) & ~1   
          MOVW SP,A                 ; load offset of stack top to pointer
          SWAPW                     ; swap higher word to AL
          MOV  USB, A               ; set bank

#if STACK_FILL == ON                ; preset the stack
          MOV  ADB, A
          MOVW A, #USTACK           ; load start stack address to AL
          MOVW A, #STACK_PATTERN    ; AL -> AH, pattern in AL
          MOVW RW0, #SIZEOF(USTACK) / 2 ; get byte count
          FILSWI    ADB             ; write pattern to stack
#endif

          OR   CCR,#H'20            ; set System stack flag
          MOVL A, #(__systemstack_top) & ~1   
          MOVW SP,A                 ; load offset of stack top to pointer
          SWAPW                     ; swap higher word to AL
          MOV  SSB, A               ; set bank

#if STACK_FILL == ON                ; preset the stack
          MOV  ADB, A
          MOVW A, #SSTACK            ; load start stack address to AL
          MOVW A, #STACK_PATTERN    ; AL -> AH, pattern in AL
          MOVW RW0, #SIZEOF(SSTACK) / 2; get byte count
          FILSWI    ADB             ; write pattern to stack
#endif

#if STACKUSE == USRSTACK
          AND  CCR,#H'DF            ; clear system stack flag
#endif


;   Following macros are needed because of the AUTOMODEL option. If the
;   model is not known while assembling the module, one has to expect 
;   completion of streaminit() by RET or RETP. Because RET removes 2 bytes
;   from stack and RETP removes 4 bytes from stack, SP is reloaded.

#  macro RELOAD_SP

#if STACKUSE == USRSTACK
          MOVW A, #(__userstack_top) & ~1
#else 
          MOVW A, #(__systemstack_top) & ~1
#endif
          MOVW SP,A                         
#  endm

;====================================================================
; 6.10  Set Data Bank Register (DTB) and Direct Page Register (DPR)
;====================================================================
          MOV  A,#BNKSEC DATA          ; User data bank offset
          MOV  DTB,A

          MOV  A,#PAGE DIRDATA_S       ; User direct page
          MOV  DPR,A

;====================================================================
; 6.11  Wait for PLL to stabilise
;====================================================================

#if CLOCKSPEED > MAINCLOCK && CLOCKWAIT == ON
no_PLL_yet:
          BBS  I:CKSCR:6,no_PLL_yet    ; check MCM and wait for
                                       ; PLL to stabilize
#endif

;====================================================================
; 6.12  Initialise Low-Level Library Interface
;====================================================================
;
; Call lib init function and reload stack afterwards, if AUTOMODEL
;====================================================================
#if CLIBINIT == ON
#  if MEMMODEL == SMALL || MEMMODEL == COMPACT
          CALL __stream_init       ; initialise library IO
#  else                            ; MEDIUM, LARGE, AUTOMODEL
          CALLP __stream_init      ; initialise library IO
#    if MEMMODEL == AUTOMODEL          
          RELOAD_SP                ; reload stack since stream_init was
                                   ; possibly left by RET (not RETP)
#    endif  ; AUTOMODEL
#  endif  ; MEDIUM, LARGE, AUTOMODEL
#endif  ; LIBINI

;====================================================================
; 6.13  Call C-language main function
;====================================================================
#if MEMMODEL == SMALL || MEMMODEL == COMPACT
          CALL _main               ; Start main function
#else                              ; MEDIUM, LARGE, AUTOMODEL
          CALLP _main              ; Start main function
                                   ; ignore remaining word on stack, 
                                   ; if main was completed by RET
;====================================================================
; 6.14  Shut down library
;====================================================================
#endif
#if CLIBINIT == ON
#  if MEMMODEL == SMALL || MEMMODEL == COMPACT
          CALL _exit               
#  else                            ; MEDIUM, LARGE, AUTOMODEL
          CALLP _exit              ; ignore remaining word on stack, 
                                   ; if main was completed by RET
#  endif
__exit:
#endif          

;====================================================================
; 6.15  Program end loop
;====================================================================

end:      BRA  end                 ; Loop


;====================================================================
; 6.16  Configuration stamp in ROM (currently test only)
;====================================================================

;   Bit7     Bit6     Bit5     Bit4     Bit3     Bit2     Bit1     Bit0
;+--------+--------+--------+--------+--------+--------+--------+--------+
;|                     Format ID of version stamp "V"                    |
;+--------+--------+--------+--------+--------+--------+--------+--------+
;|                     Format ID of version stamp "S"                    |
;+--------+--------+--------+--------+--------+--------+--------+--------+
;|                 Format version number of version stamp                |
;+--------+--------+--------+--------+--------+--------+--------+--------+
;|              FAMILY (4)           | CONST  |         MODEL (3)        |
;+--------+--------+--------+--------+--------+--------+--------+--------+
;|STACKUSE|        |  FILL  | LIBINIT| STAMP  |        |        |        |
;+--------+--------+--------+--------+--------+--------+--------+--------+
;| RESVEC | RAMCODE|        |          REGISTER BANK POINTER (5)         |
;+--------+--------+--------+--------+--------+--------+--------+--------+
;|           CLOCKSPEED (4)          |  WAIT  | ADDRESS| MIRROR |        |
;+--------+--------+--------+--------+--------+--------+--------+--------+
;|   BUSMODE (2)   | AUTOWAIT_IO (2) | AUTOWAIT_LO (29 | AUTOWAIT_HI (2) |
;+--------+--------+--------+--------+--------+--------+--------+--------+
;|                               ADDR_PINS                               |
;+--------+--------+--------+--------+--------+--------+--------+--------+
;|                               BUS_SIGNAL                              |
;+--------+--------+--------+--------+--------+--------+--------+--------+
;|                          VERSION 1st character                        |
;+--------+--------+--------+--------+--------+--------+--------+--------+
;|                          VERSION 2nd character                        |
;+--------+--------+--------+--------+--------+--------+--------+--------+
;                                   ...
;+--------+--------+--------+--------+--------+--------+--------+--------+
;|                                  NULL                                 |
;+--------+--------+--------+--------+--------+--------+--------+--------+

#if CONFIG_STAMP == ON

stamp:
  .DATA.B 'V', 'S', 0                 ; version stamp format 0
  .DATA.B endstamp - stamp            ; size of format stamp

  .DATA.B (FAMILY << 4) | (CONSTDATA << 3) |  MEMMODEL
  .DATA.B STACKUSE<<7 | STACK_RESERVE<<6 | STACK_FILL<<5 | CLIBINIT<<4
  .DATA.B RESET_VECTOR << 7 | COPY_RAMCODE << 6 | REGBANK
  .DATA.B CLOCKSPEED<<4 | CLOCKWAIT<<3 | ADDRESSMODE<<2 | ROMMIRROR<<1
  .DATA.B BUSMODE<<6 | AUTOWAIT_IO<<4 | AUTOWAIT_LO<<2 | AUTOWAIT_HI
  .DATA.B ADDR_PINS
  .DATA.B BUS_SIGNAL

  .SDATA VERSION, "\0" 
endstamp:  

#endif

;====================================================================
; 6.17  Debug address specification
;====================================================================
          .END notresetyet         ; define debugger start address

;====================================================================
; ----------------------- End of Start-up file ---------------------
;====================================================================
