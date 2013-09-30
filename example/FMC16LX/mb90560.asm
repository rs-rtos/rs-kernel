/* FFMC-16 IO-MAP HEADER FILE                                                */
/* ==========================                                                */
/* CREATED BY IO-WIZARD V2.16                                                */
/* Id: mb90560.asm,v 4.3 2003/08/29 12:36:27 dfisch Exp $ */
/*                                                                           */
/* ***********************************************************               */
/*               FUJITSU MIKROELEKTRONIK GMBH                                */
/*               Am Siebenstein 6-10, 63303 Dreieich                         */
/*               Tel.:++49/6103/690-0,Fax - 122                              */
/*                                                                           */
/* The following software is for demonstration purposes only.                */
/* It is not fully tested, nor validated in order to fullfill                */
/* its task under all circumstances. Therefore, this software                */
/* or any part of it must only be used in an evaluation                      */
/* laboratory environment.                                                   */
/* This software is subject to the rules of our standard                     */
/* DISCLAIMER, that is delivered with our SW-tools (on the CD                */
/* "Micros Documentation & Software V3.0" see "\START.HTM" or                */
/* see our Internet Page -                                                   */
/* http://www.fujitsu-ede.com/products/micro/disclaimer.html                 */
/* ***********************************************************               */
/* ----------------------------------------------------------------------    */
/* Id: mb90560.iow,v 4.0 2003/05/07 15:27:09 dfisch Exp                      */
/* ----------------------------------------------------------------------    */
/* History:                                                                  */
/* Date      Version     Author      Description                             */
/* 26-01-99 : initial creation                                               */
/* 20.03.99 : wrong io adresses created due to error in IOWizard input file  */
/*                     a special section was defined for the patch function register */
/*                     which lead to complete wrong IO adress map            */
/*                     -> IOBASE section added for IO section generation, Patch section removed */
/*                                                                           */
/* 29.03.99 : PPGC01, PPGC23, PPGC45 added to be able to start two PPGs simultaneously, */
/*                     additional bit names are modified according to PPG which is used */
/*                     Modified bit names in PCS01, PCS23, PCS45 according to PPG which is used */
/*                                                                           */
/* 14.04.00	v1.3    JRO                                                      */
/*         	-   generation of header and c file with IO-Wizard V 1.9         */
/*                                                                           */
/* 30.04.99	V1.4	VSA                                                         */
/* 	- Bitdefinitions for parallelports are changed to Pxx, Dxx               */
/* 	- 082h TMCSR0 -> TMCR0                                                   */
/* 	- 086h TMCSR1 -> TMCR1                                                   */
/*                                                                           */
/* 18.05.99	V1.5	VSA                                                         */
/* 	- disclaimer added                                                       */
/* 02.08.99	V1.6	VSA                                                         */
/* 	- TMCSRx are added as double identifiers for TMCRx                       */
/* 27.08.99	V1.7	VSA                                                         */
/* 	- ICS23 Addr changed from 0069h to 006Ah                                 */
/*                                                                           */
/* 01.09.99 	V1.8	MST                                                        */
/*          	- PPGCx change bit name, same bitname will be used for all channels */
/*          	- PCSx change bit name, same bitname will be used for all channels */
/* 30.11.99 	V1.9	VSA                                                        */
/* 	- 0036 ADCR 2 D0 .. D9 added                                             */
/* 13.04.00	V1.10	VSA                                                        */
/* 	- Security section is moved to the end of file                           */
/* 03.07.00	V1.11	VSA                                                        */
/* 	- ADC unit included (adc_01.h)                                           */
/* 10.10.00     V2.0    MST                                                  */
/*              - second Security2 section added at the beginning of the flash */
/* 01.11.00	V2.10	NMP                                                        */
/* 	- ADC structure realignment                                              */
/* 08.11.00     V2.11   NMP                                                  */
/*              - Rebuild to remove incorrect comments                       */
/* 23.07.02     V2.12   HWe     new adc_01.h, icr.h (RMW-Problem)            */
/* 30.01.03     V2.13   HWE     longwordaccess to PPG Reload: PRL01, .. , PRL45 */
/* Id: mb90560.iow,v 4.0 2003/05/07 15:27:09 dfisch Exp                      */
/*      - CVS and make controlled, specific security includefile             */
/* ----------------------------------------------------------------------    */
/* Id: adc_01.h,v 2.2 2003/08/19 09:46:21 dfisch Exp                         */
/* ----------------------------------------------------------------------    */
/* DESCRIPTION:  Interrupt Control Register Declaration                      */
/*                                                                           */
/* AUTHOR:       Fujitsu Mikroelektronik GmbH                                */
/*                                                                           */
/* HISTORY:                                                                  */
/* Version 1.0      03.07.00   : original version                            */
/*         1.1      01.11.00   : Structure alignment                         */
/* Version 1.2      22.07.2002 : HW  ADCS0, ADCR-Bitdefinitions as const, no RMV allowed */
/* Id: adc_01.h,v 2.0 2003/05/06 13:59:57 dfisch Exp                         */
/*      - CVS and make controlled                                            */
/* Id: adc_01.h,v 2.1 2003/06/27 14:30:51 dfisch Exp                         */
/*      - adapted to BITFIELD_ORDER_MSB                                      */
/* Id: adc_01.h,v 2.2 2003/08/19 09:46:21 dfisch Exp                         */
/*      - ADCS1 Bit-defs as const, only Byte-write                           */
/* ----------------------------------------------------------------------    */
/* Id: ICR.H,v 2.1 2003/06/27 14:30:51 dfisch Exp                            */
/* ----------------------------------------------------------------------    */
/*                                                                           */
/* DESCRIPTION:  Interrupt Control Register Declaration                      */
/*                                                                           */
/* AUTHOR:       Fujitsu Mikroelektronik GmbH                                */
/*                                                                           */
/* HISTORY:                                                                  */
/* Version 1.0      26.01.99:                                                */
/*      - original version                                                   */
/* Version 1.2      11.02.99                                                 */
/*      - "extern" changed to pre-defined macro of IO-Wizard                 */
/*        (__IO_EXTERN), requires IO-Wizard 1.7 or later                     */
/*                                                                           */
/* Version 1.3      17.07.2002  HW  Bitdefinitions as const, no RMV allowed  */
/* Id: ICR.H,v 2.0 2003/05/06 09:03:53 dfisch Exp                            */
/*      - CVS and make controlled                                            */
/* Id: ICR.H,v 2.1 2003/06/27 14:30:51 dfisch Exp                            */
/*      - adapted to BITFIELD_ORDER_MSB                                      */
/* ----------------------------------------------------------------------    */
/* Id: security_mb90560.asm,v 1.2 2003/08/29 12:36:12 dfisch Exp             */
/* ----------------------------------------------------------------------    */
/* Id: security_mb90560.asm,v 1.1 2003/05/07 14:50:48 dfisch Exp             */
/*      - CVS and make controlled                                            */
/* Id: security_mb90560.asm,v 1.2 2003/08/29 12:36:12 dfisch Exp             */
/*      - new families added                                                 */
/*      - Security DISABLED/ENABLED prepared                                 */
 .PROGRAM MB90560
 .TITLE   MB90560

;------------------------
; IO-AREA DEFINITIONS :
;------------------------



 .section IOBASE, IO, locate=0x0000  ; /*  PORT DATA */
 .GLOBAL __pdr0,     __pdr1,     __pdr2,     __pdr3,     __pdr4,     __pdr5
 .GLOBAL __pdr6,     __ddr0,     __ddr1,     __ddr2,     __ddr3,     __ddr4
 .GLOBAL __ddr5,     __ddr6,     __ader,     __smr0,     __scr0,     __sidr0
 .GLOBAL __sodr0,    __ssr0,     __smr1,     __scr1,     __sidr1,    __sodr1
 .GLOBAL __ssr1,     __cdcr0,    __cdcr1,    __enir,     __eirr,     __elvr
 .GLOBAL __adcs,     __adcs0,    __adcs1,    __adcr,     __adcr0,    __adcr1
 .GLOBAL __prl01,    __prl0,     __prll0,    __prlh0,    __prl1,     __prll1
 .GLOBAL __prlh1,    __ppgc01,   __ppgc0,    __ppgc1,    __pcs01,    __prl23
 .GLOBAL __prl2,     __prll2,    __prlh2,    __prl3,     __prll3,    __prlh3
 .GLOBAL __ppgc23,   __ppgc2,    __ppgc3,    __pcs23,    __prl45,    __prl4
 .GLOBAL __prll4,    __prlh4,    __prl5,     __prll5,    __prlh5,    __ppgc45
 .GLOBAL __ppgc4,    __ppgc5,    __pcs45,    __tmrr0,    __tmrr1,    __tmrr2
 .GLOBAL __dtcr0,    __dtcr1,    __dtcr2,    __sigcr,    __cpclr,    __tcdt
 .GLOBAL __tccs,     __ipcp0,    __ipcp1,    __ipcp2,    __ipcp3,    __ics01
 .GLOBAL __ics23,    __romm,     __occp0,    __occp1,    __occp2,    __occp3
 .GLOBAL __occp4,    __occp5,    __ocs0,     __ocs2,     __ocs4,     __ocs1
 .GLOBAL __ocs3,     __ocs5,     __tmcr0,    __tmcsr0,   __tmcr1,    __tmcsr1
 .GLOBAL __tmr0,     __tmr0l,    __tmr0h,    __tmrlr0,   __tmrlr0l,  __tmrlr0h
 .GLOBAL __tmr1,     __tmr1l,    __tmr1h,    __tmrlr1,   __tmrlr1l,  __tmrlr1h
 .GLOBAL __rdr0,     __rdr1,     __pacsr,    __dirr,     __lpmcr,    __ckscr
 .GLOBAL __wdtc,     __tbtc,     __fmcs,     __icr,      ___endio

__pdr0   .res.b 1             ;000000  /*  PORT DATA */
PDR0    .equ 0x0000
__pdr1   .res.b 1             ;000001
PDR1    .equ 0x0001
__pdr2   .res.b 1             ;000002
PDR2    .equ 0x0002
__pdr3   .res.b 1             ;000003
PDR3    .equ 0x0003
__pdr4   .res.b 1             ;000004
PDR4    .equ 0x0004
__pdr5   .res.b 1             ;000005
PDR5    .equ 0x0005
__pdr6   .res.b 1             ;000006
PDR6    .equ 0x0006
 .org 0x0010
__ddr0   .res.b 1             ;000010  /* PORT DIR */
DDR0    .equ 0x0010
__ddr1   .res.b 1             ;000011
DDR1    .equ 0x0011
__ddr2   .res.b 1             ;000012
DDR2    .equ 0x0012
__ddr3   .res.b 1             ;000013
DDR3    .equ 0x0013
__ddr4   .res.b 1             ;000014
DDR4    .equ 0x0014
__ddr5   .res.b 1             ;000015
DDR5    .equ 0x0015
__ddr6   .res.b 1             ;000016
DDR6    .equ 0x0016
__ader   .res.b 1             ;000017  /* Analog Input Enable Register */
ADER    .equ 0x0017
 .org 0x0020
__smr0   .res.b 1             ;000020  /* UART0,1 */
SMR0    .equ 0x0020
__scr0   .res.b 1             ;000021
SCR0    .equ 0x0021
__sidr0   .res.b 1             ;000022
SIDR0    .equ 0x0022
 .org 0x0022
__sodr0   .res.b 1             ;000022
SODR0    .equ 0x0022
__ssr0   .res.b 1             ;000023
SSR0    .equ 0x0023
__smr1   .res.b 1             ;000024
SMR1    .equ 0x0024
__scr1   .res.b 1             ;000025
SCR1    .equ 0x0025
__sidr1   .res.b 1             ;000026
SIDR1    .equ 0x0026
 .org 0x0026
__sodr1   .res.b 1             ;000026
SODR1    .equ 0x0026
__ssr1   .res.b 1             ;000027
SSR1    .equ 0x0027
 .org 0x0029
__cdcr0   .res.b 1             ;000029
CDCR0    .equ 0x0029
 .org 0x002B
__cdcr1   .res.b 1             ;00002B
CDCR1    .equ 0x002B
 .org 0x0030
__enir   .res.b 1             ;000030  /* DTP, External Interrupts */
ENIR    .equ 0x0030
__eirr   .res.b 1             ;000031
EIRR    .equ 0x0031
__elvr   .res.b 2             ;000032
ELVR    .equ 0x0032
__adcs   .res.b 2             ;000034  /* AD Converter */
ADCS    .equ 0x0034
 .org 0x0034
__adcs0   .res.b 1             ;000034
ADCS0    .equ 0x0034
__adcs1   .res.b 1             ;000035
ADCS1    .equ 0x0035
__adcr   .res.b 2             ;000036
ADCR    .equ 0x0036
 .org 0x0036
__adcr0   .res.b 1             ;000036
ADCR0    .equ 0x0036
__adcr1   .res.b 1             ;000037
ADCR1    .equ 0x0037
__prl01   .res.b 4             ;000038  /* Puls Pattern Generator Channel 0,1 */
PRL01    .equ 0x0038
 .org 0x0038
__prl0   .res.b 2             ;000038
PRL0    .equ 0x0038
 .org 0x0038
__prll0   .res.b 1             ;000038
PRLL0    .equ 0x0038
__prlh0   .res.b 1             ;000039
PRLH0    .equ 0x0039
__prl1   .res.b 2             ;00003A
PRL1    .equ 0x003A
 .org 0x003A
__prll1   .res.b 1             ;00003A
PRLL1    .equ 0x003A
__prlh1   .res.b 1             ;00003B
PRLH1    .equ 0x003B
__ppgc01   .res.b 2             ;00003C
PPGC01    .equ 0x003C
 .org 0x003C
__ppgc0   .res.b 1             ;00003C
PPGC0    .equ 0x003C
__ppgc1   .res.b 1             ;00003D
PPGC1    .equ 0x003D
__pcs01   .res.b 1             ;00003E
PCS01    .equ 0x003E
 .org 0x0040
__prl23   .res.b 4             ;000040
PRL23    .equ 0x0040
 .org 0x0040
__prl2   .res.b 2             ;000040
PRL2    .equ 0x0040
 .org 0x0040
__prll2   .res.b 1             ;000040
PRLL2    .equ 0x0040
__prlh2   .res.b 1             ;000041
PRLH2    .equ 0x0041
__prl3   .res.b 2             ;000042
PRL3    .equ 0x0042
 .org 0x0042
__prll3   .res.b 1             ;000042
PRLL3    .equ 0x0042
__prlh3   .res.b 1             ;000043
PRLH3    .equ 0x0043
__ppgc23   .res.b 2             ;000044
PPGC23    .equ 0x0044
 .org 0x0044
__ppgc2   .res.b 1             ;000044
PPGC2    .equ 0x0044
__ppgc3   .res.b 1             ;000045
PPGC3    .equ 0x0045
__pcs23   .res.b 1             ;000046
PCS23    .equ 0x0046
 .org 0x0048
__prl45   .res.b 4             ;000048
PRL45    .equ 0x0048
 .org 0x0048
__prl4   .res.b 2             ;000048
PRL4    .equ 0x0048
 .org 0x0048
__prll4   .res.b 1             ;000048
PRLL4    .equ 0x0048
__prlh4   .res.b 1             ;000049
PRLH4    .equ 0x0049
__prl5   .res.b 2             ;00004A
PRL5    .equ 0x004A
 .org 0x004A
__prll5   .res.b 1             ;00004A
PRLL5    .equ 0x004A
__prlh5   .res.b 1             ;00004B
PRLH5    .equ 0x004B
__ppgc45   .res.b 2             ;00004C
PPGC45    .equ 0x004C
 .org 0x004C
__ppgc4   .res.b 1             ;00004C
PPGC4    .equ 0x004C
__ppgc5   .res.b 1             ;00004D
PPGC5    .equ 0x004D
__pcs45   .res.b 1             ;00004E
PCS45    .equ 0x004E
 .org 0x0050
__tmrr0   .res.b 1             ;000050
TMRR0    .equ 0x0050
 .org 0x0052
__tmrr1   .res.b 1             ;000052
TMRR1    .equ 0x0052
 .org 0x0054
__tmrr2   .res.b 1             ;000054
TMRR2    .equ 0x0054
 .org 0x0051
__dtcr0   .res.b 1             ;000051
DTCR0    .equ 0x0051
 .org 0x0053
__dtcr1   .res.b 1             ;000053
DTCR1    .equ 0x0053
 .org 0x0055
__dtcr2   .res.b 1             ;000055
DTCR2    .equ 0x0055
__sigcr   .res.b 1             ;000056
SIGCR    .equ 0x0056
 .org 0x0058
__cpclr   .res.b 2             ;000058  /* 16Bit Free Running Timer                                                   */
CPCLR    .equ 0x0058
__tcdt   .res.b 2             ;00005A
TCDT    .equ 0x005A
__tccs   .res.b 2             ;00005C
TCCS    .equ 0x005C
 .org 0x0060
__ipcp0   .res.b 2             ;000060  /* ICU unit */
IPCP0    .equ 0x0060
__ipcp1   .res.b 2             ;000062
IPCP1    .equ 0x0062
__ipcp2   .res.b 2             ;000064
IPCP2    .equ 0x0064
__ipcp3   .res.b 2             ;000066
IPCP3    .equ 0x0066
__ics01   .res.b 1             ;000068
ICS01    .equ 0x0068
 .org 0x006A
__ics23   .res.b 1             ;00006A
ICS23    .equ 0x006A
 .org 0x006F
__romm   .res.b 1             ;00006F  /* ROM mirror function */
ROMM    .equ 0x006F
__occp0   .res.b 2             ;000070  /* OCU unit */
OCCP0    .equ 0x0070
__occp1   .res.b 2             ;000072
OCCP1    .equ 0x0072
__occp2   .res.b 2             ;000074
OCCP2    .equ 0x0074
__occp3   .res.b 2             ;000076
OCCP3    .equ 0x0076
__occp4   .res.b 2             ;000078
OCCP4    .equ 0x0078
__occp5   .res.b 2             ;00007A
OCCP5    .equ 0x007A
__ocs0   .res.b 1             ;00007C
OCS0    .equ 0x007C
 .org 0x007E
__ocs2   .res.b 1             ;00007E
OCS2    .equ 0x007E
 .org 0x0080
__ocs4   .res.b 1             ;000080
OCS4    .equ 0x0080
 .org 0x007D
__ocs1   .res.b 1             ;00007D
OCS1    .equ 0x007D
 .org 0x007F
__ocs3   .res.b 1             ;00007F
OCS3    .equ 0x007F
 .org 0x0081
__ocs5   .res.b 1             ;000081
OCS5    .equ 0x0081
__tmcr0   .res.b 2             ;000082  /* 16-Bit Reload Timer */
TMCR0    .equ 0x0082
 .org 0x0082
__tmcsr0   .res.b 2             ;000082
TMCSR0    .equ 0x0082
 .org 0x0086
__tmcr1   .res.b 2             ;000086
TMCR1    .equ 0x0086
 .org 0x0086
__tmcsr1   .res.b 2             ;000086
TMCSR1    .equ 0x0086
 .org 0x0084
__tmr0   .res.b 2             ;000084
TMR0    .equ 0x0084
 .org 0x0084
__tmr0l   .res.b 1             ;000084
TMR0L    .equ 0x0084
__tmr0h   .res.b 1             ;000085
TMR0H    .equ 0x0085
 .org 0x0084
__tmrlr0   .res.b 2             ;000084
TMRLR0    .equ 0x0084
 .org 0x0084
__tmrlr0l   .res.b 1             ;000084
TMRLR0L    .equ 0x0084
__tmrlr0h   .res.b 1             ;000085
TMRLR0H    .equ 0x0085
 .org 0x0088
__tmr1   .res.b 2             ;000088
TMR1    .equ 0x0088
 .org 0x0088
__tmr1l   .res.b 1             ;000088
TMR1L    .equ 0x0088
__tmr1h   .res.b 1             ;000089
TMR1H    .equ 0x0089
 .org 0x0088
__tmrlr1   .res.b 2             ;000088
TMRLR1    .equ 0x0088
 .org 0x0088
__tmrlr1l   .res.b 1             ;000088
TMRLR1L    .equ 0x0088
__tmrlr1h   .res.b 1             ;000089
TMRLR1H    .equ 0x0089
 .org 0x008C
__rdr0   .res.b 1             ;00008C  /* Input Resistance Register */
RDR0    .equ 0x008C
__rdr1   .res.b 1             ;00008D
RDR1    .equ 0x008D
 .org 0x009E
__pacsr   .res.b 1             ;00009E  /* rom cORRECTION Function */
PACSR    .equ 0x009E
__dirr   .res.b 1             ;00009F  /* Delayed Interrupt */
DIRR    .equ 0x009F
__lpmcr   .res.b 1             ;0000A0  /* Low Power Control Registers */
LPMCR    .equ 0x00A0
__ckscr   .res.b 1             ;0000A1
CKSCR    .equ 0x00A1
 .org 0x00A8
__wdtc   .res.b 1             ;0000A8  /* Watch Dog */
WDTC    .equ 0x00A8
__tbtc   .res.b 1             ;0000A9  /* Time Base Timer */
TBTC    .equ 0x00A9
 .org 0x00AE
__fmcs   .res.b 1             ;0000AE  /* Flash Control Register */
FMCS    .equ 0x00AE
 .org 0x00B0
__icr   .res.b 0x10          ;0000B0  /* Interrupt Control Registers */
ICR    .equ 0x00B0
; .org 0x00FF
;___endio   .res.b 1             ;0000FF
;_ENDIO    .equ 0x00FF
; /* include : security_mb90560.asm */

#if defined __CPU_MB90F562B__
  .section SECURITY, CONST, locate=0xFF0000  ; /*  Security Control Byte */
  .DATA.W 0xFF55 ; Security DISABLED
 ; .DATA.W 0x01FF ; Security ENABLED

#elif defined __CPU_MB90F562__
  .section SECURITY, CONST, locate=0xFFFFFE  ; /*  Security Control Byte */
  .RES.W 1

#elif defined __CPU_MB90V560__
  .section SECURITY1, CONST, locate=0xFFFFFE  ; /*  Security1 Control Word */
  .RES.W 1
  .section SECURITY2, CONST, locate=0xFF0000  ; /*  Security2 Control Word */
  .RES.W 1
 
#elif defined __CPU_MB90561__ || __CPU_MB90561A__ || __CPU_MB90562__ || __CPU_MB90562A__

#else 
 #error *** Please specify CPU-type in Project-setup !!! ***
#endif

 




 .end
