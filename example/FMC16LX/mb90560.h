/* FFMC-16 IO-MAP HEADER FILE                                                */
/* ==========================                                                */
/* SOFTUNE WORKBENCH FORMAT                                                  */
/* C-DEFINITIONS FOR IO-SYMBOLS                                              */
/* CREATED BY IO-WIZARD V2.16                                                */
/* Id: mb90560.h,v 4.3 2003/08/29 12:36:27 dfisch Exp $ */
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


#ifndef   __MB90XXX_H
#  define __MB90XXX_H
/*
- Please define __IO_NEAR in LARGE and COMPACT memory model, if the default
  data bank (DTB) is 00. This will result in better performance in these
  models.
- Please define __IO_FAR in SMALL and MEDIUM memory model, if the default
  data bank (DTB) is other than 00. This might be the case in systems with
  external RAM, which are not using internal RAM as default data area.
- Please define neither __IO_NEAR nor __IO_FAR in all other cases. This
  will work with almost all configurations.
*/

#  ifdef  __IO_NEAR
#    ifdef  __IO_FAR
#      error __IO_NEAR and __IO_FAR must not be defined at the same time
#    else
#      define ___IOWIDTH __near
#    endif
#  else
#    ifdef __IO_FAR
#      define ___IOWIDTH __far
#    else                               /* specified by memory model */
#      define ___IOWIDTH
#    endif
#  endif
#  ifdef  __IO_DEFINE
#    define __IO_EXTERN
#    define __IO_EXTENDED volatile ___IOWIDTH
#  else
#    define __IO_EXTERN   extern      /* for data, which can have __io */
#    define __IO_EXTENDED extern volatile ___IOWIDTH
#  endif

typedef unsigned char		IO_BYTE;
typedef unsigned short		IO_WORD;
typedef unsigned long		IO_LWORD;
typedef const unsigned short	IO_WORD_READ;

/* REGISTER BIT STRUCTURES */

typedef union{   /*  PORT DATA */
    IO_BYTE	byte;
    struct{
    IO_BYTE _P00 :1;
    IO_BYTE _P01 :1;
    IO_BYTE _P02 :1;
    IO_BYTE _P03 :1;
    IO_BYTE _P04 :1;
    IO_BYTE _P05 :1;
    IO_BYTE _P06 :1;
    IO_BYTE _P07 :1;
  }bit;
 }PDR0STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE _P10 :1;
    IO_BYTE _P11 :1;
    IO_BYTE _P12 :1;
    IO_BYTE _P13 :1;
    IO_BYTE _P14 :1;
    IO_BYTE _P15 :1;
    IO_BYTE _P16 :1;
    IO_BYTE _P17 :1;
  }bit;
 }PDR1STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE _P20 :1;
    IO_BYTE _P21 :1;
    IO_BYTE _P22 :1;
    IO_BYTE _P23 :1;
    IO_BYTE _P24 :1;
    IO_BYTE _P25 :1;
    IO_BYTE _P26 :1;
    IO_BYTE _P27 :1;
  }bit;
 }PDR2STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE _P30 :1;
    IO_BYTE _P31 :1;
    IO_BYTE _P32 :1;
    IO_BYTE _P33 :1;
    IO_BYTE _P34 :1;
    IO_BYTE _P35 :1;
    IO_BYTE _P36 :1;
    IO_BYTE _P37 :1;
  }bit;
 }PDR3STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE _P40 :1;
    IO_BYTE _P41 :1;
    IO_BYTE _P42 :1;
    IO_BYTE _P43 :1;
    IO_BYTE _P44 :1;
    IO_BYTE _P45 :1;
    IO_BYTE _P46 :1;
    IO_BYTE _P47 :1;
  }bit;
 }PDR4STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE _P50 :1;
    IO_BYTE _P51 :1;
    IO_BYTE _P52 :1;
    IO_BYTE _P53 :1;
    IO_BYTE _P54 :1;
    IO_BYTE _P55 :1;
    IO_BYTE _P56 :1;
    IO_BYTE _P57 :1;
  }bit;
 }PDR5STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE _P60 :1;
    IO_BYTE _P61 :1;
    IO_BYTE _P62 :1;
    IO_BYTE _P63 :1;
    IO_BYTE _P64 :1;
    IO_BYTE _P65 :1;
    IO_BYTE _P66 :1;
    IO_BYTE _P67 :1;
  }bit;
 }PDR6STR;
typedef union{   /* PORT DIR */
    IO_BYTE	byte;
    struct{
    IO_BYTE _D00 :1;
    IO_BYTE _D01 :1;
    IO_BYTE _D02 :1;
    IO_BYTE _D03 :1;
    IO_BYTE _D04 :1;
    IO_BYTE _D05 :1;
    IO_BYTE _D06 :1;
    IO_BYTE _D07 :1;
  }bit;
 }DDR0STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE _D10 :1;
    IO_BYTE _D11 :1;
    IO_BYTE _D12 :1;
    IO_BYTE _D13 :1;
    IO_BYTE _D14 :1;
    IO_BYTE _D15 :1;
    IO_BYTE _D16 :1;
    IO_BYTE _D17 :1;
  }bit;
 }DDR1STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE _D20 :1;
    IO_BYTE _D21 :1;
    IO_BYTE _D22 :1;
    IO_BYTE _D23 :1;
    IO_BYTE _D24 :1;
    IO_BYTE _D25 :1;
    IO_BYTE _D26 :1;
    IO_BYTE _D27 :1;
  }bit;
 }DDR2STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE _D30 :1;
    IO_BYTE _D31 :1;
    IO_BYTE _D32 :1;
    IO_BYTE _D33 :1;
    IO_BYTE _D34 :1;
    IO_BYTE _D35 :1;
    IO_BYTE _D36 :1;
    IO_BYTE _D37 :1;
  }bit;
 }DDR3STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE _D40 :1;
    IO_BYTE _D41 :1;
    IO_BYTE _D42 :1;
    IO_BYTE _D43 :1;
    IO_BYTE _D44 :1;
    IO_BYTE _D45 :1;
    IO_BYTE _D46 :1;
    IO_BYTE _D47 :1;
  }bit;
 }DDR4STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE _D50 :1;
    IO_BYTE _D51 :1;
    IO_BYTE _D52 :1;
    IO_BYTE _D53 :1;
    IO_BYTE _D54 :1;
    IO_BYTE _D55 :1;
    IO_BYTE _D56 :1;
    IO_BYTE _D57 :1;
  }bit;
 }DDR5STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE _D60 :1;
    IO_BYTE _D61 :1;
    IO_BYTE _D62 :1;
    IO_BYTE _D63 :1;
    IO_BYTE _D64 :1;
    IO_BYTE _D65 :1;
    IO_BYTE _D66 :1;
    IO_BYTE _D67 :1;
  }bit;
 }DDR6STR;
typedef union{   /* Analog Input Enable Register */
    IO_BYTE	byte;
    struct{
    IO_BYTE _AE0 :1;
    IO_BYTE _AE1 :1;
    IO_BYTE _AE2 :1;
    IO_BYTE _AE3 :1;
    IO_BYTE _AE4 :1;
    IO_BYTE _AE5 :1;
    IO_BYTE _AE6 :1;
    IO_BYTE _AE7 :1;
  }bit;
 }ADERSTR;
typedef union{   /* UART0,1 */
    IO_BYTE	byte;
    struct{
    IO_BYTE _SOE :1;
    IO_BYTE _SCKE :1;
    IO_BYTE _BCH :1;
    IO_BYTE _CS0 :1;
    IO_BYTE _CS1 :1;
    IO_BYTE _CS2 :1;
    IO_BYTE _MD0 :1;
    IO_BYTE _MD1 :1;
  }bit;
  struct{
    IO_BYTE :3;
    IO_BYTE _CS :3;
    IO_BYTE _MD :2;
  }bitc;
 }SMR0STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE _TXE :1;
    IO_BYTE _RXE :1;
    IO_BYTE _REC :1;
    IO_BYTE _AD :1;
    IO_BYTE _CL :1;
    IO_BYTE _SBL :1;
    IO_BYTE _P :1;
    IO_BYTE _PEN :1;
  }bit;
 }SCR0STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
  }bit;
 }SIDR0STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE _TIE :1;
    IO_BYTE _RIE :1;
    IO_BYTE _BDS :1;
    IO_BYTE _TDRE :1;
    IO_BYTE _RDRF :1;
    IO_BYTE _FRE :1;
    IO_BYTE _ORE :1;
    IO_BYTE _PE :1;
  }bit;
 }SSR0STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE _SOE :1;
    IO_BYTE _SCKE :1;
    IO_BYTE _BCH :1;
    IO_BYTE _CS0 :1;
    IO_BYTE _CS1 :1;
    IO_BYTE _CS2 :1;
    IO_BYTE _MD0 :1;
    IO_BYTE _MD1 :1;
  }bit;
  struct{
    IO_BYTE :3;
    IO_BYTE _CS :3;
    IO_BYTE _MD :2;
  }bitc;
 }SMR1STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE _TXE :1;
    IO_BYTE _RXE :1;
    IO_BYTE _REC :1;
    IO_BYTE _AD :1;
    IO_BYTE _CL :1;
    IO_BYTE _SBL :1;
    IO_BYTE _P :1;
    IO_BYTE _PEN :1;
  }bit;
 }SCR1STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
  }bit;
 }SIDR1STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE _TIE :1;
    IO_BYTE _RIE :1;
    IO_BYTE _BDS :1;
    IO_BYTE _TDRE :1;
    IO_BYTE _RDRF :1;
    IO_BYTE _FRE :1;
    IO_BYTE _ORE :1;
    IO_BYTE _PE :1;
  }bit;
 }SSR1STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE _DIV0 :1;
    IO_BYTE _DIV1 :1;
    IO_BYTE _DIV2 :1;
    IO_BYTE _DIV3 :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE _MD :1;
  }bit;
  struct{
    IO_BYTE _DIV :4;
  }bitc;
 }CDCR0STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE _DIV0 :1;
    IO_BYTE _DIV1 :1;
    IO_BYTE _DIV2 :1;
    IO_BYTE _DIV3 :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE _MD :1;
  }bit;
  struct{
    IO_BYTE _DIV :4;
  }bitc;
 }CDCR1STR;
typedef union{   /* DTP, External Interrupts */
    IO_BYTE	byte;
    struct{
    IO_BYTE _EN0 :1;
    IO_BYTE _EN1 :1;
    IO_BYTE _EN2 :1;
    IO_BYTE _EN3 :1;
    IO_BYTE _EN4 :1;
    IO_BYTE _EN5 :1;
    IO_BYTE _EN6 :1;
    IO_BYTE _EN7 :1;
  }bit;
 }ENIRSTR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE _ER0 :1;
    IO_BYTE _ER1 :1;
    IO_BYTE _ER2 :1;
    IO_BYTE _ER3 :1;
    IO_BYTE _ER4 :1;
    IO_BYTE _ER5 :1;
    IO_BYTE _ER6 :1;
    IO_BYTE _ER7 :1;
  }bit;
 }EIRRSTR;
typedef union{  
    IO_WORD	word;
    struct{
    IO_WORD _LA0 :1;
    IO_WORD _LB0 :1;
    IO_WORD _LA1 :1;
    IO_WORD _LB1 :1;
    IO_WORD _LA2 :1;
    IO_WORD _LB2 :1;
    IO_WORD _LA3 :1;
    IO_WORD _LB3 :1;
    IO_WORD _LA4 :1;
    IO_WORD _LB4 :1;
    IO_WORD _LA5 :1;
    IO_WORD _LB5 :1;
    IO_WORD _LA6 :1;
    IO_WORD _LB6 :1;
    IO_WORD _LA7 :1;
    IO_WORD _LB7 :1;
  }bit;
 }ELVRSTR;
typedef union{  
    IO_WORD	word;
    struct{
    IO_WORD  :1;
    IO_WORD _POS0 :1;
    IO_WORD _POS1 :1;
    IO_WORD _PUF0 :1;
    IO_WORD _PIE0 :1;
    IO_WORD _POE0 :1;
    IO_WORD _SST0 :1;
    IO_WORD _PEN0 :1;
    IO_WORD  :1;
    IO_WORD _MD00 :1;
    IO_WORD _MD01 :1;
    IO_WORD _PUF1 :1;
    IO_WORD _PIE1 :1;
    IO_WORD _POE1 :1;
    IO_WORD _SST1 :1;
    IO_WORD _PEN1 :1;
  }bit;
  struct{
    IO_WORD :9;
    IO_WORD _MD0 :2;
  }bitc;
 }PPGC01STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE  :1;
    IO_BYTE _POS0 :1;
    IO_BYTE _POS1 :1;
    IO_BYTE _PUF0 :1;
    IO_BYTE _PIE0 :1;
    IO_BYTE _POE0 :1;
    IO_BYTE _SST0 :1;
    IO_BYTE _PEN0 :1;
  }bit;
 }PPGC0STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE  :1;
    IO_BYTE _MD00 :1;
    IO_BYTE _MD01 :1;
    IO_BYTE _PUF1 :1;
    IO_BYTE _PIE1 :1;
    IO_BYTE _POE1 :1;
    IO_BYTE _SST1 :1;
    IO_BYTE _PEN1 :1;
  }bit;
  struct{
    IO_BYTE :1;
    IO_BYTE _MD0 :2;
  }bitc;
 }PPGC1STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE _PC00 :1;
    IO_BYTE _PC01 :1;
    IO_BYTE _PC02 :1;
    IO_BYTE _PC10 :1;
    IO_BYTE _PC11 :1;
    IO_BYTE _PC12 :1;
  }bit;
  struct{
    IO_BYTE :2;
    IO_BYTE _PC0 :3;
    IO_BYTE _PC1 :3;
  }bitc;
 }PCS01STR;
typedef union{  
    IO_WORD	word;
    struct{
    IO_WORD  :1;
    IO_WORD _POS0 :1;
    IO_WORD _POS1 :1;
    IO_WORD _PUF0 :1;
    IO_WORD _PIE0 :1;
    IO_WORD _POE0 :1;
    IO_WORD _SST0 :1;
    IO_WORD _PEN0 :1;
    IO_WORD  :1;
    IO_WORD _MD00 :1;
    IO_WORD _MD01 :1;
    IO_WORD _PUF1 :1;
    IO_WORD _PIE1 :1;
    IO_WORD _POE1 :1;
    IO_WORD _SST1 :1;
    IO_WORD _PEN1 :1;
  }bit;
  struct{
    IO_WORD :9;
    IO_WORD _MD0 :2;
  }bitc;
 }PPGC23STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE  :1;
    IO_BYTE _POS0 :1;
    IO_BYTE _POS1 :1;
    IO_BYTE _PUF0 :1;
    IO_BYTE _PIE0 :1;
    IO_BYTE _POE0 :1;
    IO_BYTE _SST0 :1;
    IO_BYTE _PEN0 :1;
  }bit;
 }PPGC2STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE  :1;
    IO_BYTE _MD00 :1;
    IO_BYTE _MD01 :1;
    IO_BYTE _PUF1 :1;
    IO_BYTE _PIE1 :1;
    IO_BYTE _POE1 :1;
    IO_BYTE _SST1 :1;
    IO_BYTE _PEN1 :1;
  }bit;
  struct{
    IO_BYTE :1;
    IO_BYTE _MD0 :2;
  }bitc;
 }PPGC3STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE _PC00 :1;
    IO_BYTE _PC01 :1;
    IO_BYTE _PC02 :1;
    IO_BYTE _PC10 :1;
    IO_BYTE _PC11 :1;
    IO_BYTE _PC12 :1;
  }bit;
  struct{
    IO_BYTE :2;
    IO_BYTE _PC0 :3;
    IO_BYTE _PC1 :3;
  }bitc;
 }PCS23STR;
typedef union{  
    IO_WORD	word;
    struct{
    IO_WORD  :1;
    IO_WORD _POS0 :1;
    IO_WORD _POS1 :1;
    IO_WORD _PUF0 :1;
    IO_WORD _PIE0 :1;
    IO_WORD _POE0 :1;
    IO_WORD _SST0 :1;
    IO_WORD _PEN0 :1;
    IO_WORD  :1;
    IO_WORD _MD00 :1;
    IO_WORD _MD01 :1;
    IO_WORD _PUF1 :1;
    IO_WORD _PIE1 :1;
    IO_WORD _POE1 :1;
    IO_WORD _SST1 :1;
    IO_WORD _PEN1 :1;
  }bit;
  struct{
    IO_WORD :9;
    IO_WORD _MD0 :2;
  }bitc;
 }PPGC45STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE  :1;
    IO_BYTE _POS0 :1;
    IO_BYTE _POS1 :1;
    IO_BYTE _PUF0 :1;
    IO_BYTE _PIE0 :1;
    IO_BYTE _POE0 :1;
    IO_BYTE _SST0 :1;
    IO_BYTE _PEN0 :1;
  }bit;
 }PPGC4STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE  :1;
    IO_BYTE _MD00 :1;
    IO_BYTE _MD01 :1;
    IO_BYTE _PUF1 :1;
    IO_BYTE _PIE1 :1;
    IO_BYTE _POE1 :1;
    IO_BYTE _SST1 :1;
    IO_BYTE _PEN1 :1;
  }bit;
  struct{
    IO_BYTE :1;
    IO_BYTE _MD0 :2;
  }bitc;
 }PPGC5STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE _PC00 :1;
    IO_BYTE _PC01 :1;
    IO_BYTE _PC02 :1;
    IO_BYTE _PC10 :1;
    IO_BYTE _PC11 :1;
    IO_BYTE _PC12 :1;
  }bit;
  struct{
    IO_BYTE :2;
    IO_BYTE _PC0 :3;
    IO_BYTE _PC1 :3;
  }bitc;
 }PCS45STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE _TMD0 :1;
    IO_BYTE _TMD1 :1;
    IO_BYTE _TMD2 :1;
    IO_BYTE _TMIE :1;
    IO_BYTE _TMIF :1;
    IO_BYTE _PGEN :1;
    IO_BYTE _GTEN :1;
    IO_BYTE _DMOD :1;
  }bit;
  struct{
    IO_BYTE _TMD :3;
  }bitc;
 }DTCR0STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE _TMD0 :1;
    IO_BYTE _TMD1 :1;
    IO_BYTE _TMD2 :1;
    IO_BYTE _TMIE :1;
    IO_BYTE _TMIF :1;
    IO_BYTE _PGEN :1;
    IO_BYTE _GTEN :1;
    IO_BYTE _DMOD :1;
  }bit;
  struct{
    IO_BYTE _TMD :3;
  }bitc;
 }DTCR1STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE _TMD0 :1;
    IO_BYTE _TMD1 :1;
    IO_BYTE _TMD2 :1;
    IO_BYTE _TMIE :1;
    IO_BYTE _TMIF :1;
    IO_BYTE _PGEN :1;
    IO_BYTE _GTEN :1;
    IO_BYTE _DMOD :1;
  }bit;
  struct{
    IO_BYTE _TMD :3;
  }bitc;
 }DTCR2STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE _PGS0 :1;
    IO_BYTE _PGS1 :1;
    IO_BYTE _DCK0 :1;
    IO_BYTE _DCK1 :1;
    IO_BYTE _DCK2 :1;
    IO_BYTE _NRSL :1;
    IO_BYTE _DTIL :1;
    IO_BYTE _DTIE :1;
  }bit;
  struct{
    IO_BYTE _PGS :2;
    IO_BYTE _DCK :3;
  }bitc;
 }SIGCRSTR;
typedef union{   /* 16Bit Free Running Timer                                                   */
    IO_WORD	word;
    struct{
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
  }bit;
 }CPCLRSTR;
typedef union{  
    IO_WORD	word;
    struct{
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
  }bit;
 }TCDTSTR;
typedef union{  
    IO_WORD	word;
    struct{
    IO_WORD _CLK0 :1;
    IO_WORD _CLK1 :1;
    IO_WORD _CLK2 :1;
    IO_WORD _SCLR :1;
    IO_WORD _MODE :1;
    IO_WORD _STOP :1;
    IO_WORD _IVFE :1;
    IO_WORD _IVF :1;
    IO_WORD _ICRE :1;
    IO_WORD _ICLR :1;
    IO_WORD _MSI0 :1;
    IO_WORD _MSI1 :1;
    IO_WORD _MSI2 :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD _ECKE :1;
  }bit;
  struct{
    IO_WORD _CLK :3;
    IO_WORD :7;
    IO_WORD _MSI :3;
  }bitc;
 }TCCSSTR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE _EG00 :1;
    IO_BYTE _EG01 :1;
    IO_BYTE _EG10 :1;
    IO_BYTE _EG11 :1;
    IO_BYTE _ICE0 :1;
    IO_BYTE _ICE1 :1;
    IO_BYTE _ICP0 :1;
    IO_BYTE _ICP1 :1;
  }bit;
  struct{
    IO_BYTE _EG0 :2;
    IO_BYTE _EG1 :2;
  }bitc;
 }ICS01STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE _EG20 :1;
    IO_BYTE _EG21 :1;
    IO_BYTE _EG30 :1;
    IO_BYTE _EG31 :1;
    IO_BYTE _ICE2 :1;
    IO_BYTE _ICE3 :1;
    IO_BYTE _ICP2 :1;
    IO_BYTE _ICP3 :1;
  }bit;
  struct{
    IO_BYTE _EG2 :2;
    IO_BYTE _EG3 :2;
  }bitc;
 }ICS23STR;
typedef union{   /* ROM mirror function */
    IO_BYTE	byte;
    struct{
    IO_BYTE _MI :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
  }bit;
 }ROMMSTR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE _CST0 :1;
    IO_BYTE _CST1 :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE _ICE0 :1;
    IO_BYTE _ICE1 :1;
    IO_BYTE _ICP0 :1;
    IO_BYTE _ICP1 :1;
  }bit;
 }OCS0STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE _CST0 :1;
    IO_BYTE _CST1 :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE _ICE0 :1;
    IO_BYTE _ICE1 :1;
    IO_BYTE _ICP0 :1;
    IO_BYTE _ICP1 :1;
  }bit;
 }OCS2STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE _CST0 :1;
    IO_BYTE _CST1 :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE _ICE0 :1;
    IO_BYTE _ICE1 :1;
    IO_BYTE _ICP0 :1;
    IO_BYTE _ICP1 :1;
  }bit;
 }OCS4STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE _OTD0 :1;
    IO_BYTE _OTD1 :1;
    IO_BYTE _OTE0 :1;
    IO_BYTE _OTE1 :1;
    IO_BYTE _CMOD :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
  }bit;
 }OCS1STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE _OTD0 :1;
    IO_BYTE _OTD1 :1;
    IO_BYTE _OTE0 :1;
    IO_BYTE _OTE1 :1;
    IO_BYTE _CMOD :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
  }bit;
 }OCS3STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE _OTD0 :1;
    IO_BYTE _OTD1 :1;
    IO_BYTE _OTE0 :1;
    IO_BYTE _OTE1 :1;
    IO_BYTE _CMOD :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
  }bit;
 }OCS5STR;
typedef union{   /* 16-Bit Reload Timer */
    IO_WORD	word;
    struct{
    IO_WORD _TRG :1;
    IO_WORD _CNTE :1;
    IO_WORD _UF :1;
    IO_WORD _INTE :1;
    IO_WORD _RELD :1;
    IO_WORD _OUTL :1;
    IO_WORD _OUTE :1;
    IO_WORD _MOD0 :1;
    IO_WORD _MOD1 :1;
    IO_WORD _MOD2 :1;
    IO_WORD _CSL0 :1;
    IO_WORD _CSL1 :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
  }bit;
  struct{
    IO_WORD :7;
    IO_WORD _MOD :3;
    IO_WORD _CSL :2;
  }bitc;
 }TMCR0STR;
typedef union{  
    IO_WORD	word;
    struct{
    IO_WORD _TRG :1;
    IO_WORD _CNTE :1;
    IO_WORD _UF :1;
    IO_WORD _INTE :1;
    IO_WORD _RELD :1;
    IO_WORD _OUTL :1;
    IO_WORD _OUTE :1;
    IO_WORD _MOD0 :1;
    IO_WORD _MOD1 :1;
    IO_WORD _MOD2 :1;
    IO_WORD _CSL0 :1;
    IO_WORD _CSL1 :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
  }bit;
  struct{
    IO_WORD :7;
    IO_WORD _MOD :3;
    IO_WORD _CSL :2;
  }bitc;
 }TMCSR0STR;
typedef union{  
    IO_WORD	word;
    struct{
    IO_WORD _TRG :1;
    IO_WORD _CNTE :1;
    IO_WORD _UF :1;
    IO_WORD _INTE :1;
    IO_WORD _RELD :1;
    IO_WORD _OUTL :1;
    IO_WORD _OUTE :1;
    IO_WORD _MOD0 :1;
    IO_WORD _MOD1 :1;
    IO_WORD _MOD2 :1;
    IO_WORD _CSL0 :1;
    IO_WORD _CSL1 :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
  }bit;
  struct{
    IO_WORD :7;
    IO_WORD _MOD :3;
    IO_WORD _CSL :2;
  }bitc;
 }TMCR1STR;
typedef union{  
    IO_WORD	word;
    struct{
    IO_WORD _TRG :1;
    IO_WORD _CNTE :1;
    IO_WORD _UF :1;
    IO_WORD _INTE :1;
    IO_WORD _RELD :1;
    IO_WORD _OUTL :1;
    IO_WORD _OUTE :1;
    IO_WORD _MOD0 :1;
    IO_WORD _MOD1 :1;
    IO_WORD _MOD2 :1;
    IO_WORD _CSL0 :1;
    IO_WORD _CSL1 :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
    IO_WORD  :1;
  }bit;
  struct{
    IO_WORD :7;
    IO_WORD _MOD :3;
    IO_WORD _CSL :2;
  }bitc;
 }TMCSR1STR;
typedef union{   /* Input Resistance Register */
    IO_BYTE	byte;
    struct{
    IO_BYTE _RD00 :1;
    IO_BYTE _RD01 :1;
    IO_BYTE _RD02 :1;
    IO_BYTE _RD03 :1;
    IO_BYTE _RD04 :1;
    IO_BYTE _RD05 :1;
    IO_BYTE _RD06 :1;
    IO_BYTE _RD07 :1;
  }bit;
 }RDR0STR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE _RD10 :1;
    IO_BYTE _RD11 :1;
    IO_BYTE _RD12 :1;
    IO_BYTE _RD13 :1;
    IO_BYTE _RD14 :1;
    IO_BYTE _RD15 :1;
    IO_BYTE _RD16 :1;
    IO_BYTE _RD17 :1;
  }bit;
 }RDR1STR;
typedef union{   /* rom cORRECTION Function */
    IO_BYTE	byte;
    struct{
    IO_BYTE _AD0D :1;
    IO_BYTE _AD0E :1;
    IO_BYTE _AD1D :1;
    IO_BYTE _AD1E :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
  }bit;
 }PACSRSTR;
typedef union{   /* Delayed Interrupt */
    IO_BYTE	byte;
    struct{
    IO_BYTE _R0 :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
  }bit;
 }DIRRSTR;
typedef union{   /* Low Power Control Registers */
    IO_BYTE	byte;
    struct{
    IO_BYTE _RESV1 :1;
    IO_BYTE _CG0 :1;
    IO_BYTE _CG1 :1;
    IO_BYTE _RESV2 :1;
    IO_BYTE _RST :1;
    IO_BYTE _SPL :1;
    IO_BYTE _SLP :1;
    IO_BYTE _STP :1;
  }bit;
  struct{
    IO_BYTE :1;
    IO_BYTE _CG :2;
  }bitc;
 }LPMCRSTR;
typedef union{  
    IO_BYTE	byte;
    struct{
    IO_BYTE _CS0 :1;
    IO_BYTE _CS1 :1;
    IO_BYTE _MCS :1;
    IO_BYTE _RESV1 :1;
    IO_BYTE _WS0 :1;
    IO_BYTE _WS1 :1;
    IO_BYTE _MCM :1;
    IO_BYTE _RESV2 :1;
  }bit;
  struct{
    IO_BYTE _CS :2;
    IO_BYTE :2;
    IO_BYTE _WS :2;
  }bitc;
 }CKSCRSTR;
typedef union{   /* Watch Dog */
    IO_BYTE	byte;
    struct{
    IO_BYTE _WT0 :1;
    IO_BYTE _WT1 :1;
    IO_BYTE _WTE :1;
    IO_BYTE _SRST :1;
    IO_BYTE _ERST :1;
    IO_BYTE _WRST :1;
    IO_BYTE  :1;
    IO_BYTE _PONR :1;
  }bit;
  struct{
    IO_BYTE _WT :2;
  }bitc;
 }WDTCSTR;
typedef union{   /* Time Base Timer */
    IO_BYTE	byte;
    struct{
    IO_BYTE _TBC0 :1;
    IO_BYTE _TBC1 :1;
    IO_BYTE _TBR :1;
    IO_BYTE _TBOF :1;
    IO_BYTE _TBIE :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE _RESV :1;
  }bit;
  struct{
    IO_BYTE _TBC :2;
  }bitc;
 }TBTCSTR;
typedef union{   /* Flash Control Register */
    IO_BYTE	byte;
    struct{
    IO_BYTE _LPM :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE  :1;
    IO_BYTE _RDY :1;
    IO_BYTE _WE :1;
    IO_BYTE _RDYI :1;
    IO_BYTE _INTE :1;
  }bit;
 }FMCSSTR;

/* C-DECLARATIONS */

__IO_EXTERN __io PDR0STR _pdr0;   /*  PORT DATA */
#define PDR0 _pdr0.byte
#define PDR0_P00 _pdr0.bit._P00
#define PDR0_P01 _pdr0.bit._P01
#define PDR0_P02 _pdr0.bit._P02
#define PDR0_P03 _pdr0.bit._P03
#define PDR0_P04 _pdr0.bit._P04
#define PDR0_P05 _pdr0.bit._P05
#define PDR0_P06 _pdr0.bit._P06
#define PDR0_P07 _pdr0.bit._P07
__IO_EXTERN __io PDR1STR _pdr1;  
#define PDR1 _pdr1.byte
#define PDR1_P10 _pdr1.bit._P10
#define PDR1_P11 _pdr1.bit._P11
#define PDR1_P12 _pdr1.bit._P12
#define PDR1_P13 _pdr1.bit._P13
#define PDR1_P14 _pdr1.bit._P14
#define PDR1_P15 _pdr1.bit._P15
#define PDR1_P16 _pdr1.bit._P16
#define PDR1_P17 _pdr1.bit._P17
__IO_EXTERN __io PDR2STR _pdr2;  
#define PDR2 _pdr2.byte
#define PDR2_P20 _pdr2.bit._P20
#define PDR2_P21 _pdr2.bit._P21
#define PDR2_P22 _pdr2.bit._P22
#define PDR2_P23 _pdr2.bit._P23
#define PDR2_P24 _pdr2.bit._P24
#define PDR2_P25 _pdr2.bit._P25
#define PDR2_P26 _pdr2.bit._P26
#define PDR2_P27 _pdr2.bit._P27
__IO_EXTERN __io PDR3STR _pdr3;  
#define PDR3 _pdr3.byte
#define PDR3_P30 _pdr3.bit._P30
#define PDR3_P31 _pdr3.bit._P31
#define PDR3_P32 _pdr3.bit._P32
#define PDR3_P33 _pdr3.bit._P33
#define PDR3_P34 _pdr3.bit._P34
#define PDR3_P35 _pdr3.bit._P35
#define PDR3_P36 _pdr3.bit._P36
#define PDR3_P37 _pdr3.bit._P37
__IO_EXTERN __io PDR4STR _pdr4;  
#define PDR4 _pdr4.byte
#define PDR4_P40 _pdr4.bit._P40
#define PDR4_P41 _pdr4.bit._P41
#define PDR4_P42 _pdr4.bit._P42
#define PDR4_P43 _pdr4.bit._P43
#define PDR4_P44 _pdr4.bit._P44
#define PDR4_P45 _pdr4.bit._P45
#define PDR4_P46 _pdr4.bit._P46
#define PDR4_P47 _pdr4.bit._P47
__IO_EXTERN __io PDR5STR _pdr5;  
#define PDR5 _pdr5.byte
#define PDR5_P50 _pdr5.bit._P50
#define PDR5_P51 _pdr5.bit._P51
#define PDR5_P52 _pdr5.bit._P52
#define PDR5_P53 _pdr5.bit._P53
#define PDR5_P54 _pdr5.bit._P54
#define PDR5_P55 _pdr5.bit._P55
#define PDR5_P56 _pdr5.bit._P56
#define PDR5_P57 _pdr5.bit._P57
__IO_EXTERN __io PDR6STR _pdr6;  
#define PDR6 _pdr6.byte
#define PDR6_P60 _pdr6.bit._P60
#define PDR6_P61 _pdr6.bit._P61
#define PDR6_P62 _pdr6.bit._P62
#define PDR6_P63 _pdr6.bit._P63
#define PDR6_P64 _pdr6.bit._P64
#define PDR6_P65 _pdr6.bit._P65
#define PDR6_P66 _pdr6.bit._P66
#define PDR6_P67 _pdr6.bit._P67
__IO_EXTERN __io DDR0STR _ddr0;   /* PORT DIR */
#define DDR0 _ddr0.byte
#define DDR0_D00 _ddr0.bit._D00
#define DDR0_D01 _ddr0.bit._D01
#define DDR0_D02 _ddr0.bit._D02
#define DDR0_D03 _ddr0.bit._D03
#define DDR0_D04 _ddr0.bit._D04
#define DDR0_D05 _ddr0.bit._D05
#define DDR0_D06 _ddr0.bit._D06
#define DDR0_D07 _ddr0.bit._D07
__IO_EXTERN __io DDR1STR _ddr1;  
#define DDR1 _ddr1.byte
#define DDR1_D10 _ddr1.bit._D10
#define DDR1_D11 _ddr1.bit._D11
#define DDR1_D12 _ddr1.bit._D12
#define DDR1_D13 _ddr1.bit._D13
#define DDR1_D14 _ddr1.bit._D14
#define DDR1_D15 _ddr1.bit._D15
#define DDR1_D16 _ddr1.bit._D16
#define DDR1_D17 _ddr1.bit._D17
__IO_EXTERN __io DDR2STR _ddr2;  
#define DDR2 _ddr2.byte
#define DDR2_D20 _ddr2.bit._D20
#define DDR2_D21 _ddr2.bit._D21
#define DDR2_D22 _ddr2.bit._D22
#define DDR2_D23 _ddr2.bit._D23
#define DDR2_D24 _ddr2.bit._D24
#define DDR2_D25 _ddr2.bit._D25
#define DDR2_D26 _ddr2.bit._D26
#define DDR2_D27 _ddr2.bit._D27
__IO_EXTERN __io DDR3STR _ddr3;  
#define DDR3 _ddr3.byte
#define DDR3_D30 _ddr3.bit._D30
#define DDR3_D31 _ddr3.bit._D31
#define DDR3_D32 _ddr3.bit._D32
#define DDR3_D33 _ddr3.bit._D33
#define DDR3_D34 _ddr3.bit._D34
#define DDR3_D35 _ddr3.bit._D35
#define DDR3_D36 _ddr3.bit._D36
#define DDR3_D37 _ddr3.bit._D37
__IO_EXTERN __io DDR4STR _ddr4;  
#define DDR4 _ddr4.byte
#define DDR4_D40 _ddr4.bit._D40
#define DDR4_D41 _ddr4.bit._D41
#define DDR4_D42 _ddr4.bit._D42
#define DDR4_D43 _ddr4.bit._D43
#define DDR4_D44 _ddr4.bit._D44
#define DDR4_D45 _ddr4.bit._D45
#define DDR4_D46 _ddr4.bit._D46
#define DDR4_D47 _ddr4.bit._D47
__IO_EXTERN __io DDR5STR _ddr5;  
#define DDR5 _ddr5.byte
#define DDR5_D50 _ddr5.bit._D50
#define DDR5_D51 _ddr5.bit._D51
#define DDR5_D52 _ddr5.bit._D52
#define DDR5_D53 _ddr5.bit._D53
#define DDR5_D54 _ddr5.bit._D54
#define DDR5_D55 _ddr5.bit._D55
#define DDR5_D56 _ddr5.bit._D56
#define DDR5_D57 _ddr5.bit._D57
__IO_EXTERN __io DDR6STR _ddr6;  
#define DDR6 _ddr6.byte
#define DDR6_D60 _ddr6.bit._D60
#define DDR6_D61 _ddr6.bit._D61
#define DDR6_D62 _ddr6.bit._D62
#define DDR6_D63 _ddr6.bit._D63
#define DDR6_D64 _ddr6.bit._D64
#define DDR6_D65 _ddr6.bit._D65
#define DDR6_D66 _ddr6.bit._D66
#define DDR6_D67 _ddr6.bit._D67
__IO_EXTERN __io ADERSTR _ader;   /* Analog Input Enable Register */
#define ADER _ader.byte
#define ADER_AE0 _ader.bit._AE0
#define ADER_AE1 _ader.bit._AE1
#define ADER_AE2 _ader.bit._AE2
#define ADER_AE3 _ader.bit._AE3
#define ADER_AE4 _ader.bit._AE4
#define ADER_AE5 _ader.bit._AE5
#define ADER_AE6 _ader.bit._AE6
#define ADER_AE7 _ader.bit._AE7
__IO_EXTERN __io SMR0STR _smr0;   /* UART0,1 */
#define SMR0 _smr0.byte
#define SMR0_SOE _smr0.bit._SOE
#define SMR0_SCKE _smr0.bit._SCKE
#define SMR0_BCH _smr0.bit._BCH
#define SMR0_CS0 _smr0.bit._CS0
#define SMR0_CS1 _smr0.bit._CS1
#define SMR0_CS2 _smr0.bit._CS2
#define SMR0_MD0 _smr0.bit._MD0
#define SMR0_MD1 _smr0.bit._MD1
#define SMR0_CS _smr0.bitc._CS
#define SMR0_MD _smr0.bitc._MD
__IO_EXTERN __io SCR0STR _scr0;  
#define SCR0 _scr0.byte
#define SCR0_TXE _scr0.bit._TXE
#define SCR0_RXE _scr0.bit._RXE
#define SCR0_REC _scr0.bit._REC
#define SCR0_AD _scr0.bit._AD
#define SCR0_CL _scr0.bit._CL
#define SCR0_SBL _scr0.bit._SBL
#define SCR0_P _scr0.bit._P
#define SCR0_PEN _scr0.bit._PEN
__IO_EXTERN __io SIDR0STR _sidr0;  
#define SIDR0 _sidr0.byte
__IO_EXTERN __io IO_BYTE _sodr0;
#define SODR0 _sodr0   
__IO_EXTERN __io SSR0STR _ssr0;  
#define SSR0 _ssr0.byte
#define SSR0_TIE _ssr0.bit._TIE
#define SSR0_RIE _ssr0.bit._RIE
#define SSR0_BDS _ssr0.bit._BDS
#define SSR0_TDRE _ssr0.bit._TDRE
#define SSR0_RDRF _ssr0.bit._RDRF
#define SSR0_FRE _ssr0.bit._FRE
#define SSR0_ORE _ssr0.bit._ORE
#define SSR0_PE _ssr0.bit._PE
__IO_EXTERN __io SMR1STR _smr1;  
#define SMR1 _smr1.byte
#define SMR1_SOE _smr1.bit._SOE
#define SMR1_SCKE _smr1.bit._SCKE
#define SMR1_BCH _smr1.bit._BCH
#define SMR1_CS0 _smr1.bit._CS0
#define SMR1_CS1 _smr1.bit._CS1
#define SMR1_CS2 _smr1.bit._CS2
#define SMR1_MD0 _smr1.bit._MD0
#define SMR1_MD1 _smr1.bit._MD1
#define SMR1_CS _smr1.bitc._CS
#define SMR1_MD _smr1.bitc._MD
__IO_EXTERN __io SCR1STR _scr1;  
#define SCR1 _scr1.byte
#define SCR1_TXE _scr1.bit._TXE
#define SCR1_RXE _scr1.bit._RXE
#define SCR1_REC _scr1.bit._REC
#define SCR1_AD _scr1.bit._AD
#define SCR1_CL _scr1.bit._CL
#define SCR1_SBL _scr1.bit._SBL
#define SCR1_P _scr1.bit._P
#define SCR1_PEN _scr1.bit._PEN
__IO_EXTERN __io SIDR1STR _sidr1;  
#define SIDR1 _sidr1.byte
__IO_EXTERN __io IO_BYTE _sodr1;
#define SODR1 _sodr1   
__IO_EXTERN __io SSR1STR _ssr1;  
#define SSR1 _ssr1.byte
#define SSR1_TIE _ssr1.bit._TIE
#define SSR1_RIE _ssr1.bit._RIE
#define SSR1_BDS _ssr1.bit._BDS
#define SSR1_TDRE _ssr1.bit._TDRE
#define SSR1_RDRF _ssr1.bit._RDRF
#define SSR1_FRE _ssr1.bit._FRE
#define SSR1_ORE _ssr1.bit._ORE
#define SSR1_PE _ssr1.bit._PE
__IO_EXTERN __io CDCR0STR _cdcr0;  
#define CDCR0 _cdcr0.byte
#define CDCR0_DIV0 _cdcr0.bit._DIV0
#define CDCR0_DIV1 _cdcr0.bit._DIV1
#define CDCR0_DIV2 _cdcr0.bit._DIV2
#define CDCR0_DIV3 _cdcr0.bit._DIV3
#define CDCR0_MD _cdcr0.bit._MD
#define CDCR0_DIV _cdcr0.bitc._DIV
__IO_EXTERN __io CDCR1STR _cdcr1;  
#define CDCR1 _cdcr1.byte
#define CDCR1_DIV0 _cdcr1.bit._DIV0
#define CDCR1_DIV1 _cdcr1.bit._DIV1
#define CDCR1_DIV2 _cdcr1.bit._DIV2
#define CDCR1_DIV3 _cdcr1.bit._DIV3
#define CDCR1_MD _cdcr1.bit._MD
#define CDCR1_DIV _cdcr1.bitc._DIV
__IO_EXTERN __io ENIRSTR _enir;   /* DTP, External Interrupts */
#define ENIR _enir.byte
#define ENIR_EN0 _enir.bit._EN0
#define ENIR_EN1 _enir.bit._EN1
#define ENIR_EN2 _enir.bit._EN2
#define ENIR_EN3 _enir.bit._EN3
#define ENIR_EN4 _enir.bit._EN4
#define ENIR_EN5 _enir.bit._EN5
#define ENIR_EN6 _enir.bit._EN6
#define ENIR_EN7 _enir.bit._EN7
__IO_EXTERN __io EIRRSTR _eirr;  
#define EIRR _eirr.byte
#define EIRR_ER0 _eirr.bit._ER0
#define EIRR_ER1 _eirr.bit._ER1
#define EIRR_ER2 _eirr.bit._ER2
#define EIRR_ER3 _eirr.bit._ER3
#define EIRR_ER4 _eirr.bit._ER4
#define EIRR_ER5 _eirr.bit._ER5
#define EIRR_ER6 _eirr.bit._ER6
#define EIRR_ER7 _eirr.bit._ER7
__IO_EXTERN __io ELVRSTR _elvr;  
#define ELVR _elvr.word
#define ELVR_LA0 _elvr.bit._LA0
#define ELVR_LB0 _elvr.bit._LB0
#define ELVR_LA1 _elvr.bit._LA1
#define ELVR_LB1 _elvr.bit._LB1
#define ELVR_LA2 _elvr.bit._LA2
#define ELVR_LB2 _elvr.bit._LB2
#define ELVR_LA3 _elvr.bit._LA3
#define ELVR_LB3 _elvr.bit._LB3
#define ELVR_LA4 _elvr.bit._LA4
#define ELVR_LB4 _elvr.bit._LB4
#define ELVR_LA5 _elvr.bit._LA5
#define ELVR_LB5 _elvr.bit._LB5
#define ELVR_LA6 _elvr.bit._LA6
#define ELVR_LB6 _elvr.bit._LB6
#define ELVR_LA7 _elvr.bit._LA7
#define ELVR_LB7 _elvr.bit._LB7
/* include : ADC_01.H */


#ifndef __ADC_H
#define __ADC_H

typedef union{   /*  */
    IO_WORD	word;
    struct{
#if defined(__BITFIELD_ORDER_MSB__)
    const IO_WORD BUSY :1;
    const IO_WORD INT  :1;
    const IO_WORD INTE :1;
    const IO_WORD PAUS :1;
    const IO_WORD STS1 :1;
    const IO_WORD STS0 :1;
    const IO_WORD STRT :1;
    const IO_WORD RESV :1; 
    const IO_WORD MD1  :1;
    const IO_WORD MD0  :1;
    const IO_WORD ANS2 :1;
    const IO_WORD ANS1 :1;
    const IO_WORD ANS0 :1;
    const IO_WORD ANE2 :1;
    const IO_WORD ANE1 :1;
    const IO_WORD ANE0 :1;
#else
    const IO_WORD ANE0 :1;
    const IO_WORD ANE1 :1;
    const IO_WORD ANE2 :1;
    const IO_WORD ANS0 :1;
    const IO_WORD ANS1 :1;
    const IO_WORD ANS2 :1;
    const IO_WORD MD0  :1;
    const IO_WORD MD1  :1;
    const IO_WORD RESV :1; 
    const IO_WORD STRT :1;
    const IO_WORD STS0 :1;
    const IO_WORD STS1 :1;
    const IO_WORD PAUS :1;
    const IO_WORD INTE :1;
    const IO_WORD INT  :1;
    const IO_WORD BUSY :1;
#endif
   }bit;
  struct{
#if defined(__BITFIELD_ORDER_MSB__)
    const IO_WORD :4;
    const IO_WORD STS :2;
    const IO_WORD :1;
    const IO_WORD :1;
    const IO_WORD MD :2;
    const IO_WORD ANS :3;
    const IO_WORD ANE :3;
#else
    const IO_WORD ANE :3;
    const IO_WORD ANS :3;
    const IO_WORD MD :2;
    const IO_WORD :1;
    const IO_WORD :1;
    const IO_WORD STS :2;
#endif
  }bitc;
 }ADCSSTR;

typedef union{  
    IO_BYTE	byte;
    struct{
#if defined(__BITFIELD_ORDER_MSB__)
    const IO_BYTE MD1 :1;
    const IO_BYTE MD0 :1;
    const IO_BYTE ANS2 :1;
    const IO_BYTE ANS1 :1;
    const IO_BYTE ANS0 :1;
    const IO_BYTE ANE2 :1;
    const IO_BYTE ANE1 :1;
    const IO_BYTE ANE0 :1;
#else
    const IO_BYTE ANE0 :1;
    const IO_BYTE ANE1 :1;
    const IO_BYTE ANE2 :1;
    const IO_BYTE ANS0 :1;
    const IO_BYTE ANS1 :1;
    const IO_BYTE ANS2 :1;
    const IO_BYTE MD0 :1;
    const IO_BYTE MD1 :1;
#endif
  }bit;
  struct{
#if defined(__BITFIELD_ORDER_MSB__)
    const IO_BYTE MD :2;
    const IO_BYTE ANS :3;
    const IO_BYTE ANE :3;
#else
    const IO_BYTE ANE :3;
    const IO_BYTE ANS :3;
    const IO_BYTE MD :2;
#endif
  }bitc;
 }ADCS0STR;

typedef union{  
    IO_BYTE	byte;
    struct{
#if defined(__BITFIELD_ORDER_MSB__)
    const IO_BYTE BUSY :1;
    const IO_BYTE INT :1;
    const IO_BYTE INTE :1;
    const IO_BYTE PAUS :1;
    const IO_BYTE STS1 :1;
    const IO_BYTE STS0 :1;
    const IO_BYTE STRT :1;
    const IO_BYTE RESV :1;
#else
    const IO_BYTE RESV :1;
    const IO_BYTE STRT :1;
    const IO_BYTE STS0 :1;
    const IO_BYTE STS1 :1;
    const IO_BYTE PAUS :1;
    const IO_BYTE INTE :1;
    const IO_BYTE INT :1;
    const IO_BYTE BUSY :1;
#endif
  }bit;
  struct{
#if defined(__BITFIELD_ORDER_MSB__)
    const IO_BYTE :4;
    const IO_BYTE STS :2;
#else
    const IO_BYTE :2;
    const IO_BYTE STS :2;
#endif
  }bitc;
 }ADCS1STR;

typedef union{  
    IO_WORD	word;
    struct{
#if defined(__BITFIELD_ORDER_MSB__)
    const IO_WORD S10 :1;
    const IO_WORD ST1 :1;
    const IO_WORD ST0 :1;
    const IO_WORD CT1 :1;
    const IO_WORD CT0 :1;
    const IO_WORD  :1;
    const IO_WORD D9 :1;
    const IO_WORD D8 :1;
    const IO_WORD D7 :1;
    const IO_WORD D6 :1;
    const IO_WORD D5 :1;
    const IO_WORD D4 :1;
    const IO_WORD D3 :1;
    const IO_WORD D2 :1;
    const IO_WORD D1 :1;
    const IO_WORD D0 :1;
#else
    const IO_WORD D0 :1;
    const IO_WORD D1 :1;
    const IO_WORD D2 :1;
    const IO_WORD D3 :1;
    const IO_WORD D4 :1;
    const IO_WORD D5 :1;
    const IO_WORD D6 :1;
    const IO_WORD D7 :1;
    const IO_WORD D8 :1;
    const IO_WORD D9 :1;
    const IO_WORD  :1;
    const IO_WORD CT0 :1;
    const IO_WORD CT1 :1;
    const IO_WORD ST0 :1;
    const IO_WORD ST1 :1;
    const IO_WORD S10 :1;
#endif
  }bit;
  struct{
#if defined(__BITFIELD_ORDER_MSB__)
    const IO_WORD ST :2;
    const IO_WORD CT :2;
#else
    const IO_WORD :11;
    const IO_WORD CT :2;
    const IO_WORD ST :2;
#endif
  }bitc;
 }ADCRSTR;

typedef union{  
    const IO_BYTE	byte;
    struct{
#if defined(__BITFIELD_ORDER_MSB__)
    const IO_BYTE D7 :1;
    const IO_BYTE D6 :1;
    const IO_BYTE D5 :1;
    const IO_BYTE D4 :1;
    const IO_BYTE D3 :1;
    const IO_BYTE D2 :1;
    const IO_BYTE D1 :1;
    const IO_BYTE D0 :1;
#else
    const IO_BYTE D0 :1;
    const IO_BYTE D1 :1;
    const IO_BYTE D2 :1;
    const IO_BYTE D3 :1;
    const IO_BYTE D4 :1;
    const IO_BYTE D5 :1;
    const IO_BYTE D6 :1;
    const IO_BYTE D7 :1;
#endif
  }bit;
 }ADCR0STR;

typedef union{  
    IO_BYTE	byte;
    struct{
#if defined(__BITFIELD_ORDER_MSB__)
    const IO_BYTE S10 :1;
    const IO_BYTE ST1 :1;
    const IO_BYTE ST0 :1;
    const IO_BYTE CT1 :1;
    const IO_BYTE CT0 :1;
    const IO_BYTE  :1;
    const IO_BYTE D9 :1;
    const IO_BYTE D8 :1;
#else
    const IO_BYTE D8 :1;
    const IO_BYTE D9 :1;
    const IO_BYTE  :1;
    const IO_BYTE CT0 :1;
    const IO_BYTE CT1 :1;
    const IO_BYTE ST0 :1;
    const IO_BYTE ST1 :1;
    const IO_BYTE S10 :1;
#endif
  }bit;
  struct{
#if defined(__BITFIELD_ORDER_MSB__)
    const IO_BYTE ST :2;
    const IO_BYTE CT :2;
#else
    const IO_BYTE :1;
    const IO_BYTE :1;
    const IO_BYTE :1;
    const IO_BYTE CT :2;
    const IO_BYTE ST :2;
#endif
  }bitc;
 }ADCR1STR;

__IO_EXTERN __io ADCSSTR _adcs;
#define ADCS _adcs.word
#define ADCS_ANE0 _adcs.bit.ANE0
#define ADCS_ANE1 _adcs.bit.ANE1
#define ADCS_ANE2 _adcs.bit.ANE2
#define ADCS_ANS0 _adcs.bit.ANS0
#define ADCS_ANS1 _adcs.bit.ANS1
#define ADCS_ANS2 _adcs.bit.ANS2
#define ADCS_MD0 _adcs.bit.MD0
#define ADCS_MD1 _adcs.bit.MD1
#define ADCS_STRT _adcs.bit.STRT
#define ADCS_STS0 _adcs.bit.STS0
#define ADCS_STS1 _adcs.bit.STS1
#define ADCS_PAUS _adcs.bit.PAUS
#define ADCS_INTE _adcs.bit.INTE
#define ADCS_INT _adcs.bit.INT
#define ADCS_BUSY _adcs.bit.BUSY
#define ADCS_ANE _adcs.bitc.ANE
#define ADCS_ANS _adcs.bitc.ANS
#define ADCS_MD _adcs.bitc.MD
#define ADCS_STS _adcs.bitc.STS
__IO_EXTERN __io ADCS0STR _adcs0;  
#define ADCS0 _adcs0.byte
#define ADCS0_ANE0 _adcs0.bit.ANE0
#define ADCS0_ANE1 _adcs0.bit.ANE1
#define ADCS0_ANE2 _adcs0.bit.ANE2
#define ADCS0_ANS0 _adcs0.bit.ANS0
#define ADCS0_ANS1 _adcs0.bit.ANS1
#define ADCS0_ANS2 _adcs0.bit.ANS2
#define ADCS0_MD0 _adcs0.bit.MD0
#define ADCS0_MD1 _adcs0.bit.MD1
#define ADCS0_ANE _adcs0.bitc.ANE
#define ADCS0_ANS _adcs0.bitc.ANS
#define ADCS0_MD _adcs0.bitc.MD
__IO_EXTERN __io ADCS1STR _adcs1;  
#define ADCS1 _adcs1.byte
#define ADCS1_STRT _adcs1.bit.STRT
#define ADCS1_STS0 _adcs1.bit.STS0
#define ADCS1_STS1 _adcs1.bit.STS1
#define ADCS1_PAUS _adcs1.bit.PAUS
#define ADCS1_INTE _adcs1.bit.INTE
#define ADCS1_INT _adcs1.bit.INT
#define ADCS1_BUSY _adcs1.bit.BUSY
#define ADCS1_STS _adcs1.bitc.STS
__IO_EXTERN __io ADCRSTR _adcr;  
#define ADCR _adcr.word
#define ADCR_D0 _adcr.bit.D0
#define ADCR_D1 _adcr.bit.D1
#define ADCR_D2 _adcr.bit.D2
#define ADCR_D3 _adcr.bit.D3
#define ADCR_D4 _adcr.bit.D4
#define ADCR_D5 _adcr.bit.D5
#define ADCR_D6 _adcr.bit.D6
#define ADCR_D7 _adcr.bit.D7
#define ADCR_D8 _adcr.bit.D8
#define ADCR_D9 _adcr.bit.D9
#define ADCR_CT0 _adcr.bit.CT0
#define ADCR_CT1 _adcr.bit.CT1
#define ADCR_ST0 _adcr.bit.ST0
#define ADCR_ST1 _adcr.bit.ST1
#define ADCR_S10 _adcr.bit.S10
#define ADCR_CT _adcr.bitc.CT
#define ADCR_ST _adcr.bitc.ST
__IO_EXTERN __io ADCR0STR _adcr0;  
#define ADCR0 _adcr0.byte
#define ADCR0_D0 _adcr0.bit.D0
#define ADCR0_D1 _adcr0.bit.D1
#define ADCR0_D2 _adcr0.bit.D2
#define ADCR0_D3 _adcr0.bit.D3
#define ADCR0_D4 _adcr0.bit.D4
#define ADCR0_D5 _adcr0.bit.D5
#define ADCR0_D6 _adcr0.bit.D6
#define ADCR0_D7 _adcr0.bit.D7
__IO_EXTERN __io ADCR1STR _adcr1;  
#define ADCR1 _adcr1.byte
#define ADCR1_D8 _adcr1.bit.D8
#define ADCR1_D9 _adcr1.bit.D9
#define ADCR1_CT0 _adcr1.bit.CT0
#define ADCR1_CT1 _adcr1.bit.CT1
#define ADCR1_ST0 _adcr1.bit.ST0
#define ADCR1_ST1 _adcr1.bit.ST1
#define ADCR1_S10 _adcr1.bit.S10
#define ADCR1_CT _adcr1.bitc.CT
#define ADCR1_ST _adcr1.bitc.ST

#endif   /* __ADC_H  */
__IO_EXTERN __io IO_LWORD _prl01;
#define PRL01 _prl01    /* Puls Pattern Generator Channel 0,1 */
__IO_EXTERN __io IO_WORD _prl0;
#define PRL0 _prl0   
__IO_EXTERN __io IO_BYTE _prll0;
#define PRLL0 _prll0   
__IO_EXTERN __io IO_BYTE _prlh0;
#define PRLH0 _prlh0   
__IO_EXTERN __io IO_WORD _prl1;
#define PRL1 _prl1   
__IO_EXTERN __io IO_BYTE _prll1;
#define PRLL1 _prll1   
__IO_EXTERN __io IO_BYTE _prlh1;
#define PRLH1 _prlh1   
__IO_EXTERN __io PPGC01STR _ppgc01;  
#define PPGC01 _ppgc01.word
#define PPGC01_POS0 _ppgc01.bit._POS0
#define PPGC01_POS1 _ppgc01.bit._POS1
#define PPGC01_PUF0 _ppgc01.bit._PUF0
#define PPGC01_PIE0 _ppgc01.bit._PIE0
#define PPGC01_POE0 _ppgc01.bit._POE0
#define PPGC01_SST0 _ppgc01.bit._SST0
#define PPGC01_PEN0 _ppgc01.bit._PEN0
#define PPGC01_MD00 _ppgc01.bit._MD00
#define PPGC01_MD01 _ppgc01.bit._MD01
#define PPGC01_PUF1 _ppgc01.bit._PUF1
#define PPGC01_PIE1 _ppgc01.bit._PIE1
#define PPGC01_POE1 _ppgc01.bit._POE1
#define PPGC01_SST1 _ppgc01.bit._SST1
#define PPGC01_PEN1 _ppgc01.bit._PEN1
#define PPGC01_MD0 _ppgc01.bitc._MD0
__IO_EXTERN __io PPGC0STR _ppgc0;  
#define PPGC0 _ppgc0.byte
#define PPGC0_POS0 _ppgc0.bit._POS0
#define PPGC0_POS1 _ppgc0.bit._POS1
#define PPGC0_PUF0 _ppgc0.bit._PUF0
#define PPGC0_PIE0 _ppgc0.bit._PIE0
#define PPGC0_POE0 _ppgc0.bit._POE0
#define PPGC0_SST0 _ppgc0.bit._SST0
#define PPGC0_PEN0 _ppgc0.bit._PEN0
__IO_EXTERN __io PPGC1STR _ppgc1;  
#define PPGC1 _ppgc1.byte
#define PPGC1_MD00 _ppgc1.bit._MD00
#define PPGC1_MD01 _ppgc1.bit._MD01
#define PPGC1_PUF1 _ppgc1.bit._PUF1
#define PPGC1_PIE1 _ppgc1.bit._PIE1
#define PPGC1_POE1 _ppgc1.bit._POE1
#define PPGC1_SST1 _ppgc1.bit._SST1
#define PPGC1_PEN1 _ppgc1.bit._PEN1
#define PPGC1_MD0 _ppgc1.bitc._MD0
__IO_EXTERN __io PCS01STR _pcs01;  
#define PCS01 _pcs01.byte
#define PCS01_PC00 _pcs01.bit._PC00
#define PCS01_PC01 _pcs01.bit._PC01
#define PCS01_PC02 _pcs01.bit._PC02
#define PCS01_PC10 _pcs01.bit._PC10
#define PCS01_PC11 _pcs01.bit._PC11
#define PCS01_PC12 _pcs01.bit._PC12
#define PCS01_PC0 _pcs01.bitc._PC0
#define PCS01_PC1 _pcs01.bitc._PC1
__IO_EXTERN __io IO_LWORD _prl23;
#define PRL23 _prl23   
__IO_EXTERN __io IO_WORD _prl2;
#define PRL2 _prl2   
__IO_EXTERN __io IO_BYTE _prll2;
#define PRLL2 _prll2   
__IO_EXTERN __io IO_BYTE _prlh2;
#define PRLH2 _prlh2   
__IO_EXTERN __io IO_WORD _prl3;
#define PRL3 _prl3   
__IO_EXTERN __io IO_BYTE _prll3;
#define PRLL3 _prll3   
__IO_EXTERN __io IO_BYTE _prlh3;
#define PRLH3 _prlh3   
__IO_EXTERN __io PPGC23STR _ppgc23;  
#define PPGC23 _ppgc23.word
#define PPGC23_POS0 _ppgc23.bit._POS0
#define PPGC23_POS1 _ppgc23.bit._POS1
#define PPGC23_PUF0 _ppgc23.bit._PUF0
#define PPGC23_PIE0 _ppgc23.bit._PIE0
#define PPGC23_POE0 _ppgc23.bit._POE0
#define PPGC23_SST0 _ppgc23.bit._SST0
#define PPGC23_PEN0 _ppgc23.bit._PEN0
#define PPGC23_MD00 _ppgc23.bit._MD00
#define PPGC23_MD01 _ppgc23.bit._MD01
#define PPGC23_PUF1 _ppgc23.bit._PUF1
#define PPGC23_PIE1 _ppgc23.bit._PIE1
#define PPGC23_POE1 _ppgc23.bit._POE1
#define PPGC23_SST1 _ppgc23.bit._SST1
#define PPGC23_PEN1 _ppgc23.bit._PEN1
#define PPGC23_MD0 _ppgc23.bitc._MD0
__IO_EXTERN __io PPGC2STR _ppgc2;  
#define PPGC2 _ppgc2.byte
#define PPGC2_POS0 _ppgc2.bit._POS0
#define PPGC2_POS1 _ppgc2.bit._POS1
#define PPGC2_PUF0 _ppgc2.bit._PUF0
#define PPGC2_PIE0 _ppgc2.bit._PIE0
#define PPGC2_POE0 _ppgc2.bit._POE0
#define PPGC2_SST0 _ppgc2.bit._SST0
#define PPGC2_PEN0 _ppgc2.bit._PEN0
__IO_EXTERN __io PPGC3STR _ppgc3;  
#define PPGC3 _ppgc3.byte
#define PPGC3_MD00 _ppgc3.bit._MD00
#define PPGC3_MD01 _ppgc3.bit._MD01
#define PPGC3_PUF1 _ppgc3.bit._PUF1
#define PPGC3_PIE1 _ppgc3.bit._PIE1
#define PPGC3_POE1 _ppgc3.bit._POE1
#define PPGC3_SST1 _ppgc3.bit._SST1
#define PPGC3_PEN1 _ppgc3.bit._PEN1
#define PPGC3_MD0 _ppgc3.bitc._MD0
__IO_EXTERN __io PCS23STR _pcs23;  
#define PCS23 _pcs23.byte
#define PCS23_PC00 _pcs23.bit._PC00
#define PCS23_PC01 _pcs23.bit._PC01
#define PCS23_PC02 _pcs23.bit._PC02
#define PCS23_PC10 _pcs23.bit._PC10
#define PCS23_PC11 _pcs23.bit._PC11
#define PCS23_PC12 _pcs23.bit._PC12
#define PCS23_PC0 _pcs23.bitc._PC0
#define PCS23_PC1 _pcs23.bitc._PC1
__IO_EXTERN __io IO_LWORD _prl45;
#define PRL45 _prl45   
__IO_EXTERN __io IO_WORD _prl4;
#define PRL4 _prl4   
__IO_EXTERN __io IO_BYTE _prll4;
#define PRLL4 _prll4   
__IO_EXTERN __io IO_BYTE _prlh4;
#define PRLH4 _prlh4   
__IO_EXTERN __io IO_WORD _prl5;
#define PRL5 _prl5   
__IO_EXTERN __io IO_BYTE _prll5;
#define PRLL5 _prll5   
__IO_EXTERN __io IO_BYTE _prlh5;
#define PRLH5 _prlh5   
__IO_EXTERN __io PPGC45STR _ppgc45;  
#define PPGC45 _ppgc45.word
#define PPGC45_POS0 _ppgc45.bit._POS0
#define PPGC45_POS1 _ppgc45.bit._POS1
#define PPGC45_PUF0 _ppgc45.bit._PUF0
#define PPGC45_PIE0 _ppgc45.bit._PIE0
#define PPGC45_POE0 _ppgc45.bit._POE0
#define PPGC45_SST0 _ppgc45.bit._SST0
#define PPGC45_PEN0 _ppgc45.bit._PEN0
#define PPGC45_MD00 _ppgc45.bit._MD00
#define PPGC45_MD01 _ppgc45.bit._MD01
#define PPGC45_PUF1 _ppgc45.bit._PUF1
#define PPGC45_PIE1 _ppgc45.bit._PIE1
#define PPGC45_POE1 _ppgc45.bit._POE1
#define PPGC45_SST1 _ppgc45.bit._SST1
#define PPGC45_PEN1 _ppgc45.bit._PEN1
#define PPGC45_MD0 _ppgc45.bitc._MD0
__IO_EXTERN __io PPGC4STR _ppgc4;  
#define PPGC4 _ppgc4.byte
#define PPGC4_POS0 _ppgc4.bit._POS0
#define PPGC4_POS1 _ppgc4.bit._POS1
#define PPGC4_PUF0 _ppgc4.bit._PUF0
#define PPGC4_PIE0 _ppgc4.bit._PIE0
#define PPGC4_POE0 _ppgc4.bit._POE0
#define PPGC4_SST0 _ppgc4.bit._SST0
#define PPGC4_PEN0 _ppgc4.bit._PEN0
__IO_EXTERN __io PPGC5STR _ppgc5;  
#define PPGC5 _ppgc5.byte
#define PPGC5_MD00 _ppgc5.bit._MD00
#define PPGC5_MD01 _ppgc5.bit._MD01
#define PPGC5_PUF1 _ppgc5.bit._PUF1
#define PPGC5_PIE1 _ppgc5.bit._PIE1
#define PPGC5_POE1 _ppgc5.bit._POE1
#define PPGC5_SST1 _ppgc5.bit._SST1
#define PPGC5_PEN1 _ppgc5.bit._PEN1
#define PPGC5_MD0 _ppgc5.bitc._MD0
__IO_EXTERN __io PCS45STR _pcs45;  
#define PCS45 _pcs45.byte
#define PCS45_PC00 _pcs45.bit._PC00
#define PCS45_PC01 _pcs45.bit._PC01
#define PCS45_PC02 _pcs45.bit._PC02
#define PCS45_PC10 _pcs45.bit._PC10
#define PCS45_PC11 _pcs45.bit._PC11
#define PCS45_PC12 _pcs45.bit._PC12
#define PCS45_PC0 _pcs45.bitc._PC0
#define PCS45_PC1 _pcs45.bitc._PC1
__IO_EXTERN __io IO_BYTE _tmrr0;
#define TMRR0 _tmrr0   
__IO_EXTERN __io IO_BYTE _tmrr1;
#define TMRR1 _tmrr1   
__IO_EXTERN __io IO_BYTE _tmrr2;
#define TMRR2 _tmrr2   
__IO_EXTERN __io DTCR0STR _dtcr0;  
#define DTCR0 _dtcr0.byte
#define DTCR0_TMD0 _dtcr0.bit._TMD0
#define DTCR0_TMD1 _dtcr0.bit._TMD1
#define DTCR0_TMD2 _dtcr0.bit._TMD2
#define DTCR0_TMIE _dtcr0.bit._TMIE
#define DTCR0_TMIF _dtcr0.bit._TMIF
#define DTCR0_PGEN _dtcr0.bit._PGEN
#define DTCR0_GTEN _dtcr0.bit._GTEN
#define DTCR0_DMOD _dtcr0.bit._DMOD
#define DTCR0_TMD _dtcr0.bitc._TMD
__IO_EXTERN __io DTCR1STR _dtcr1;  
#define DTCR1 _dtcr1.byte
#define DTCR1_TMD0 _dtcr1.bit._TMD0
#define DTCR1_TMD1 _dtcr1.bit._TMD1
#define DTCR1_TMD2 _dtcr1.bit._TMD2
#define DTCR1_TMIE _dtcr1.bit._TMIE
#define DTCR1_TMIF _dtcr1.bit._TMIF
#define DTCR1_PGEN _dtcr1.bit._PGEN
#define DTCR1_GTEN _dtcr1.bit._GTEN
#define DTCR1_DMOD _dtcr1.bit._DMOD
#define DTCR1_TMD _dtcr1.bitc._TMD
__IO_EXTERN __io DTCR2STR _dtcr2;  
#define DTCR2 _dtcr2.byte
#define DTCR2_TMD0 _dtcr2.bit._TMD0
#define DTCR2_TMD1 _dtcr2.bit._TMD1
#define DTCR2_TMD2 _dtcr2.bit._TMD2
#define DTCR2_TMIE _dtcr2.bit._TMIE
#define DTCR2_TMIF _dtcr2.bit._TMIF
#define DTCR2_PGEN _dtcr2.bit._PGEN
#define DTCR2_GTEN _dtcr2.bit._GTEN
#define DTCR2_DMOD _dtcr2.bit._DMOD
#define DTCR2_TMD _dtcr2.bitc._TMD
__IO_EXTERN __io SIGCRSTR _sigcr;  
#define SIGCR _sigcr.byte
#define SIGCR_PGS0 _sigcr.bit._PGS0
#define SIGCR_PGS1 _sigcr.bit._PGS1
#define SIGCR_DCK0 _sigcr.bit._DCK0
#define SIGCR_DCK1 _sigcr.bit._DCK1
#define SIGCR_DCK2 _sigcr.bit._DCK2
#define SIGCR_NRSL _sigcr.bit._NRSL
#define SIGCR_DTIL _sigcr.bit._DTIL
#define SIGCR_DTIE _sigcr.bit._DTIE
#define SIGCR_PGS _sigcr.bitc._PGS
#define SIGCR_DCK _sigcr.bitc._DCK
__IO_EXTERN __io CPCLRSTR _cpclr;   /* 16Bit Free Running Timer                                                   */
#define CPCLR _cpclr.word
__IO_EXTERN __io TCDTSTR _tcdt;  
#define TCDT _tcdt.word
__IO_EXTERN __io TCCSSTR _tccs;  
#define TCCS _tccs.word
#define TCCS_CLK0 _tccs.bit._CLK0
#define TCCS_CLK1 _tccs.bit._CLK1
#define TCCS_CLK2 _tccs.bit._CLK2
#define TCCS_SCLR _tccs.bit._SCLR
#define TCCS_MODE _tccs.bit._MODE
#define TCCS_STOP _tccs.bit._STOP
#define TCCS_IVFE _tccs.bit._IVFE
#define TCCS_IVF _tccs.bit._IVF
#define TCCS_ICRE _tccs.bit._ICRE
#define TCCS_ICLR _tccs.bit._ICLR
#define TCCS_MSI0 _tccs.bit._MSI0
#define TCCS_MSI1 _tccs.bit._MSI1
#define TCCS_MSI2 _tccs.bit._MSI2
#define TCCS_ECKE _tccs.bit._ECKE
#define TCCS_CLK _tccs.bitc._CLK
#define TCCS_MSI _tccs.bitc._MSI
__IO_EXTERN __io IO_WORD _ipcp0;
#define IPCP0 _ipcp0    /* ICU unit */
__IO_EXTERN __io IO_WORD _ipcp1;
#define IPCP1 _ipcp1   
__IO_EXTERN __io IO_WORD _ipcp2;
#define IPCP2 _ipcp2   
__IO_EXTERN __io IO_WORD _ipcp3;
#define IPCP3 _ipcp3   
__IO_EXTERN __io ICS01STR _ics01;  
#define ICS01 _ics01.byte
#define ICS01_EG00 _ics01.bit._EG00
#define ICS01_EG01 _ics01.bit._EG01
#define ICS01_EG10 _ics01.bit._EG10
#define ICS01_EG11 _ics01.bit._EG11
#define ICS01_ICE0 _ics01.bit._ICE0
#define ICS01_ICE1 _ics01.bit._ICE1
#define ICS01_ICP0 _ics01.bit._ICP0
#define ICS01_ICP1 _ics01.bit._ICP1
#define ICS01_EG0 _ics01.bitc._EG0
#define ICS01_EG1 _ics01.bitc._EG1
__IO_EXTERN __io ICS23STR _ics23;  
#define ICS23 _ics23.byte
#define ICS23_EG20 _ics23.bit._EG20
#define ICS23_EG21 _ics23.bit._EG21
#define ICS23_EG30 _ics23.bit._EG30
#define ICS23_EG31 _ics23.bit._EG31
#define ICS23_ICE2 _ics23.bit._ICE2
#define ICS23_ICE3 _ics23.bit._ICE3
#define ICS23_ICP2 _ics23.bit._ICP2
#define ICS23_ICP3 _ics23.bit._ICP3
#define ICS23_EG2 _ics23.bitc._EG2
#define ICS23_EG3 _ics23.bitc._EG3
__IO_EXTERN __io ROMMSTR _romm;   /* ROM mirror function */
#define ROMM _romm.byte
#define ROMM_MI _romm.bit._MI
__IO_EXTERN __io IO_WORD _occp0;
#define OCCP0 _occp0    /* OCU unit */
__IO_EXTERN __io IO_WORD _occp1;
#define OCCP1 _occp1   
__IO_EXTERN __io IO_WORD _occp2;
#define OCCP2 _occp2   
__IO_EXTERN __io IO_WORD _occp3;
#define OCCP3 _occp3   
__IO_EXTERN __io IO_WORD _occp4;
#define OCCP4 _occp4   
__IO_EXTERN __io IO_WORD _occp5;
#define OCCP5 _occp5   
__IO_EXTERN __io OCS0STR _ocs0;  
#define OCS0 _ocs0.byte
#define OCS0_CST0 _ocs0.bit._CST0
#define OCS0_CST1 _ocs0.bit._CST1
#define OCS0_ICE0 _ocs0.bit._ICE0
#define OCS0_ICE1 _ocs0.bit._ICE1
#define OCS0_ICP0 _ocs0.bit._ICP0
#define OCS0_ICP1 _ocs0.bit._ICP1
__IO_EXTERN __io OCS2STR _ocs2;  
#define OCS2 _ocs2.byte
#define OCS2_CST0 _ocs2.bit._CST0
#define OCS2_CST1 _ocs2.bit._CST1
#define OCS2_ICE0 _ocs2.bit._ICE0
#define OCS2_ICE1 _ocs2.bit._ICE1
#define OCS2_ICP0 _ocs2.bit._ICP0
#define OCS2_ICP1 _ocs2.bit._ICP1
__IO_EXTERN __io OCS4STR _ocs4;  
#define OCS4 _ocs4.byte
#define OCS4_CST0 _ocs4.bit._CST0
#define OCS4_CST1 _ocs4.bit._CST1
#define OCS4_ICE0 _ocs4.bit._ICE0
#define OCS4_ICE1 _ocs4.bit._ICE1
#define OCS4_ICP0 _ocs4.bit._ICP0
#define OCS4_ICP1 _ocs4.bit._ICP1
__IO_EXTERN __io OCS1STR _ocs1;  
#define OCS1 _ocs1.byte
#define OCS1_OTD0 _ocs1.bit._OTD0
#define OCS1_OTD1 _ocs1.bit._OTD1
#define OCS1_OTE0 _ocs1.bit._OTE0
#define OCS1_OTE1 _ocs1.bit._OTE1
#define OCS1_CMOD _ocs1.bit._CMOD
__IO_EXTERN __io OCS3STR _ocs3;  
#define OCS3 _ocs3.byte
#define OCS3_OTD0 _ocs3.bit._OTD0
#define OCS3_OTD1 _ocs3.bit._OTD1
#define OCS3_OTE0 _ocs3.bit._OTE0
#define OCS3_OTE1 _ocs3.bit._OTE1
#define OCS3_CMOD _ocs3.bit._CMOD
__IO_EXTERN __io OCS5STR _ocs5;  
#define OCS5 _ocs5.byte
#define OCS5_OTD0 _ocs5.bit._OTD0
#define OCS5_OTD1 _ocs5.bit._OTD1
#define OCS5_OTE0 _ocs5.bit._OTE0
#define OCS5_OTE1 _ocs5.bit._OTE1
#define OCS5_CMOD _ocs5.bit._CMOD
__IO_EXTERN __io TMCR0STR _tmcr0;   /* 16-Bit Reload Timer */
#define TMCR0 _tmcr0.word
#define TMCR0_TRG _tmcr0.bit._TRG
#define TMCR0_CNTE _tmcr0.bit._CNTE
#define TMCR0_UF _tmcr0.bit._UF
#define TMCR0_INTE _tmcr0.bit._INTE
#define TMCR0_RELD _tmcr0.bit._RELD
#define TMCR0_OUTL _tmcr0.bit._OUTL
#define TMCR0_OUTE _tmcr0.bit._OUTE
#define TMCR0_MOD0 _tmcr0.bit._MOD0
#define TMCR0_MOD1 _tmcr0.bit._MOD1
#define TMCR0_MOD2 _tmcr0.bit._MOD2
#define TMCR0_CSL0 _tmcr0.bit._CSL0
#define TMCR0_CSL1 _tmcr0.bit._CSL1
#define TMCR0_MOD _tmcr0.bitc._MOD
#define TMCR0_CSL _tmcr0.bitc._CSL
__IO_EXTERN __io TMCSR0STR _tmcsr0;  
#define TMCSR0 _tmcsr0.word
#define TMCSR0_TRG _tmcsr0.bit._TRG
#define TMCSR0_CNTE _tmcsr0.bit._CNTE
#define TMCSR0_UF _tmcsr0.bit._UF
#define TMCSR0_INTE _tmcsr0.bit._INTE
#define TMCSR0_RELD _tmcsr0.bit._RELD
#define TMCSR0_OUTL _tmcsr0.bit._OUTL
#define TMCSR0_OUTE _tmcsr0.bit._OUTE
#define TMCSR0_MOD0 _tmcsr0.bit._MOD0
#define TMCSR0_MOD1 _tmcsr0.bit._MOD1
#define TMCSR0_MOD2 _tmcsr0.bit._MOD2
#define TMCSR0_CSL0 _tmcsr0.bit._CSL0
#define TMCSR0_CSL1 _tmcsr0.bit._CSL1
#define TMCSR0_MOD _tmcsr0.bitc._MOD
#define TMCSR0_CSL _tmcsr0.bitc._CSL
__IO_EXTERN __io TMCR1STR _tmcr1;  
#define TMCR1 _tmcr1.word
#define TMCR1_TRG _tmcr1.bit._TRG
#define TMCR1_CNTE _tmcr1.bit._CNTE
#define TMCR1_UF _tmcr1.bit._UF
#define TMCR1_INTE _tmcr1.bit._INTE
#define TMCR1_RELD _tmcr1.bit._RELD
#define TMCR1_OUTL _tmcr1.bit._OUTL
#define TMCR1_OUTE _tmcr1.bit._OUTE
#define TMCR1_MOD0 _tmcr1.bit._MOD0
#define TMCR1_MOD1 _tmcr1.bit._MOD1
#define TMCR1_MOD2 _tmcr1.bit._MOD2
#define TMCR1_CSL0 _tmcr1.bit._CSL0
#define TMCR1_CSL1 _tmcr1.bit._CSL1
#define TMCR1_MOD _tmcr1.bitc._MOD
#define TMCR1_CSL _tmcr1.bitc._CSL
__IO_EXTERN __io TMCSR1STR _tmcsr1;  
#define TMCSR1 _tmcsr1.word
#define TMCSR1_TRG _tmcsr1.bit._TRG
#define TMCSR1_CNTE _tmcsr1.bit._CNTE
#define TMCSR1_UF _tmcsr1.bit._UF
#define TMCSR1_INTE _tmcsr1.bit._INTE
#define TMCSR1_RELD _tmcsr1.bit._RELD
#define TMCSR1_OUTL _tmcsr1.bit._OUTL
#define TMCSR1_OUTE _tmcsr1.bit._OUTE
#define TMCSR1_MOD0 _tmcsr1.bit._MOD0
#define TMCSR1_MOD1 _tmcsr1.bit._MOD1
#define TMCSR1_MOD2 _tmcsr1.bit._MOD2
#define TMCSR1_CSL0 _tmcsr1.bit._CSL0
#define TMCSR1_CSL1 _tmcsr1.bit._CSL1
#define TMCSR1_MOD _tmcsr1.bitc._MOD
#define TMCSR1_CSL _tmcsr1.bitc._CSL
__IO_EXTERN __io IO_WORD _tmr0;
#define TMR0 _tmr0   
__IO_EXTERN __io IO_BYTE _tmr0l;
#define TMR0L _tmr0l   
__IO_EXTERN __io IO_BYTE _tmr0h;
#define TMR0H _tmr0h   
__IO_EXTERN __io IO_WORD _tmrlr0;
#define TMRLR0 _tmrlr0   
__IO_EXTERN __io IO_BYTE _tmrlr0l;
#define TMRLR0L _tmrlr0l   
__IO_EXTERN __io IO_BYTE _tmrlr0h;
#define TMRLR0H _tmrlr0h   
__IO_EXTERN __io IO_WORD _tmr1;
#define TMR1 _tmr1   
__IO_EXTERN __io IO_BYTE _tmr1l;
#define TMR1L _tmr1l   
__IO_EXTERN __io IO_BYTE _tmr1h;
#define TMR1H _tmr1h   
__IO_EXTERN __io IO_WORD _tmrlr1;
#define TMRLR1 _tmrlr1   
__IO_EXTERN __io IO_BYTE _tmrlr1l;
#define TMRLR1L _tmrlr1l   
__IO_EXTERN __io IO_BYTE _tmrlr1h;
#define TMRLR1H _tmrlr1h   
__IO_EXTERN __io RDR0STR _rdr0;   /* Input Resistance Register */
#define RDR0 _rdr0.byte
#define RDR0_RD00 _rdr0.bit._RD00
#define RDR0_RD01 _rdr0.bit._RD01
#define RDR0_RD02 _rdr0.bit._RD02
#define RDR0_RD03 _rdr0.bit._RD03
#define RDR0_RD04 _rdr0.bit._RD04
#define RDR0_RD05 _rdr0.bit._RD05
#define RDR0_RD06 _rdr0.bit._RD06
#define RDR0_RD07 _rdr0.bit._RD07
__IO_EXTERN __io RDR1STR _rdr1;  
#define RDR1 _rdr1.byte
#define RDR1_RD10 _rdr1.bit._RD10
#define RDR1_RD11 _rdr1.bit._RD11
#define RDR1_RD12 _rdr1.bit._RD12
#define RDR1_RD13 _rdr1.bit._RD13
#define RDR1_RD14 _rdr1.bit._RD14
#define RDR1_RD15 _rdr1.bit._RD15
#define RDR1_RD16 _rdr1.bit._RD16
#define RDR1_RD17 _rdr1.bit._RD17
__IO_EXTERN __io PACSRSTR _pacsr;   /* rom cORRECTION Function */
#define PACSR _pacsr.byte
#define PACSR_AD0D _pacsr.bit._AD0D
#define PACSR_AD0E _pacsr.bit._AD0E
#define PACSR_AD1D _pacsr.bit._AD1D
#define PACSR_AD1E _pacsr.bit._AD1E
__IO_EXTERN __io DIRRSTR _dirr;   /* Delayed Interrupt */
#define DIRR _dirr.byte
#define DIRR_R0 _dirr.bit._R0
__IO_EXTERN __io LPMCRSTR _lpmcr;   /* Low Power Control Registers */
#define LPMCR _lpmcr.byte
#define LPMCR_RESV1 _lpmcr.bit._RESV1
#define LPMCR_CG0 _lpmcr.bit._CG0
#define LPMCR_CG1 _lpmcr.bit._CG1
#define LPMCR_RESV2 _lpmcr.bit._RESV2
#define LPMCR_RST _lpmcr.bit._RST
#define LPMCR_SPL _lpmcr.bit._SPL
#define LPMCR_SLP _lpmcr.bit._SLP
#define LPMCR_STP _lpmcr.bit._STP
#define LPMCR_CG _lpmcr.bitc._CG
__IO_EXTERN __io CKSCRSTR _ckscr;  
#define CKSCR _ckscr.byte
#define CKSCR_CS0 _ckscr.bit._CS0
#define CKSCR_CS1 _ckscr.bit._CS1
#define CKSCR_MCS _ckscr.bit._MCS
#define CKSCR_RESV1 _ckscr.bit._RESV1
#define CKSCR_WS0 _ckscr.bit._WS0
#define CKSCR_WS1 _ckscr.bit._WS1
#define CKSCR_MCM _ckscr.bit._MCM
#define CKSCR_RESV2 _ckscr.bit._RESV2
#define CKSCR_CS _ckscr.bitc._CS
#define CKSCR_WS _ckscr.bitc._WS
__IO_EXTERN __io WDTCSTR _wdtc;   /* Watch Dog */
#define WDTC _wdtc.byte
#define WDTC_WT0 _wdtc.bit._WT0
#define WDTC_WT1 _wdtc.bit._WT1
#define WDTC_WTE _wdtc.bit._WTE
#define WDTC_SRST _wdtc.bit._SRST
#define WDTC_ERST _wdtc.bit._ERST
#define WDTC_WRST _wdtc.bit._WRST
#define WDTC_PONR _wdtc.bit._PONR
#define WDTC_WT _wdtc.bitc._WT
__IO_EXTERN __io TBTCSTR _tbtc;   /* Time Base Timer */
#define TBTC _tbtc.byte
#define TBTC_TBC0 _tbtc.bit._TBC0
#define TBTC_TBC1 _tbtc.bit._TBC1
#define TBTC_TBR _tbtc.bit._TBR
#define TBTC_TBOF _tbtc.bit._TBOF
#define TBTC_TBIE _tbtc.bit._TBIE
#define TBTC_RESV _tbtc.bit._RESV
#define TBTC_TBC _tbtc.bitc._TBC
__IO_EXTERN __io FMCSSTR _fmcs;   /* Flash Control Register */
#define FMCS _fmcs.byte
#define FMCS_LPM _fmcs.bit._LPM
#define FMCS_RDY _fmcs.bit._RDY
#define FMCS_WE _fmcs.bit._WE
#define FMCS_RDYI _fmcs.bit._RDYI
#define FMCS_INTE _fmcs.bit._INTE
/* include : icr.h */

#ifndef __ICR_H
#define __ICR_H

typedef union{  
    unsigned char    byte;
    struct{
#if defined(__BITFIELD_ORDER_MSB__)
      const unsigned char ICS3     :1;
      const unsigned char ICS2     :1;
      const unsigned char ICS1     :1;
      const unsigned char ICS0     :1;
      const unsigned char ISE      :1;
      const unsigned char IL2      :1;
      const unsigned char IL1      :1;
      const unsigned char IL0      :1;
#else
      const unsigned char IL0      :1;
      const unsigned char IL1      :1;
      const unsigned char IL2      :1;
      const unsigned char ISE      :1;
      const unsigned char ICS0     :1;
      const unsigned char ICS1     :1;
      const unsigned char ICS2     :1;
      const unsigned char ICS3     :1;
#endif
    }bit;
    struct{
#if defined(__BITFIELD_ORDER_MSB__)
      const unsigned char    :2;
      const unsigned char S1 :1;
      const unsigned char S0 :1;
#else
      const unsigned char    :4;
      const unsigned char S0 :1;
      const unsigned char S1 :1;
#endif
    }bitrd;
    struct{
#if defined(__BITFIELD_ORDER_MSB__)
      const unsigned char ICS      :4;
      const unsigned char :1;
      const unsigned char IL       :3;
#else
      const unsigned char IL       :3;
      const unsigned char :1;
      const unsigned char ICS      :4;
#endif
    }bitc;
    struct{
#if defined(__BITFIELD_ORDER_MSB__)
      const unsigned char    :2;
      const unsigned char S  :2;
#else
      const unsigned char    :4;
      const unsigned char S  :2;
#endif
    }bitcrd;
 }ICRSTR;

__IO_EXTERN __io ICRSTR _icr[16];

#define ICR(i)        _icr[i].byte
#define ICR_IL0(i)    _icr[i].bit.IL0
#define ICR_IL1(i)    _icr[i].bit.IL1
#define ICR_IL2(i)    _icr[i].bit.IL2
#define ICR_ISE(i)    _icr[i].bit.ISE
#define ICR_ICS0(i)   _icr[i].bit.ICS0
#define ICR_ICS1(i)   _icr[i].bit.ICS1
#define ICR_ICS2(i)   _icr[i].bit.ICS2
#define ICR_ICS3(i)   _icr[i].bit.ICS3
#define ICR_IL(i)     _icr[i].bitc.IL
#define ICR_ICS(i)    _icr[i].bitc.ICS
#define ICR_S0(i)     _icr[i].bitrd.S0
#define ICR_S1(i)     _icr[i].bitrd.S1
#define ICR_S(i)      _icr[i].bitcrd.S

#define ICR00       _icr[0].byte
#define ICR00_IL0   _icr[0].bit.IL0
#define ICR00_IL1   _icr[0].bit.IL1
#define ICR00_IL2   _icr[0].bit.IL2
#define ICR00_ISE   _icr[0].bit.ISE
#define ICR00_ICS0  _icr[0].bit.ICS0
#define ICR00_ICS1  _icr[0].bit.ICS1
#define ICR00_ICS2  _icr[0].bit.ICS2
#define ICR00_ICS3  _icr[0].bit.ICS3
#define ICR00_IL    _icr[0].bitc.IL
#define ICR00_ICS   _icr[0].bitc.ICS
#define ICR00_S0    _icr[0].bitrd.S0
#define ICR00_S1    _icr[0].bitrd.S1
#define ICR00_S     _icr[0].bitcrd.S

#define ICR01       _icr[1].byte
#define ICR01_IL0   _icr[1].bit.IL0
#define ICR01_IL1   _icr[1].bit.IL1
#define ICR01_IL2   _icr[1].bit.IL2
#define ICR01_ISE   _icr[1].bit.ISE
#define ICR01_ICS0  _icr[1].bit.ICS0
#define ICR01_ICS1  _icr[1].bit.ICS1
#define ICR01_ICS2  _icr[1].bit.ICS2
#define ICR01_ICS3  _icr[1].bit.ICS3
#define ICR01_IL    _icr[1].bitc.IL
#define ICR01_ICS   _icr[1].bitc.ICS
#define ICR01_S0    _icr[1].bitrd.S0
#define ICR01_S1    _icr[1].bitrd.S1
#define ICR01_S     _icr[1].bitcrd.S

#define ICR02       _icr[2].byte
#define ICR02_IL0   _icr[2].bit.IL0
#define ICR02_IL1   _icr[2].bit.IL1
#define ICR02_IL2   _icr[2].bit.IL2
#define ICR02_ISE   _icr[2].bit.ISE
#define ICR02_ICS0  _icr[2].bit.ICS0
#define ICR02_ICS1  _icr[2].bit.ICS1
#define ICR02_ICS2  _icr[2].bit.ICS2
#define ICR02_ICS3  _icr[2].bit.ICS3
#define ICR02_IL    _icr[2].bitc.IL
#define ICR02_ICS   _icr[2].bitc.ICS
#define ICR02_S0    _icr[2].bitrd.S0
#define ICR02_S1    _icr[2].bitrd.S1
#define ICR02_S     _icr[2].bitcrd.S

#define ICR03       _icr[3].byte
#define ICR03_IL0   _icr[3].bit.IL0
#define ICR03_IL1   _icr[3].bit.IL1
#define ICR03_IL2   _icr[3].bit.IL2
#define ICR03_ISE   _icr[3].bit.ISE
#define ICR03_ICS0  _icr[3].bit.ICS0
#define ICR03_ICS1  _icr[3].bit.ICS1
#define ICR03_ICS2  _icr[3].bit.ICS2
#define ICR03_ICS3  _icr[3].bit.ICS3
#define ICR03_IL    _icr[3].bitc.IL
#define ICR03_ICS   _icr[3].bitc.ICS
#define ICR03_S0    _icr[3].bitrd.S0
#define ICR03_S1    _icr[3].bitrd.S1
#define ICR03_S     _icr[3].bitcrd.S

#define ICR04       _icr[4].byte
#define ICR04_IL4   _icr[4].bit.IL0
#define ICR04_IL1   _icr[4].bit.IL1
#define ICR04_IL2   _icr[4].bit.IL2
#define ICR04_ISE   _icr[4].bit.ISE
#define ICR04_ICS4  _icr[4].bit.ICS0
#define ICR04_ICS1  _icr[4].bit.ICS1
#define ICR04_ICS2  _icr[4].bit.ICS2
#define ICR04_ICS3  _icr[4].bit.ICS3
#define ICR04_IL    _icr[4].bitc.IL
#define ICR04_ICS   _icr[4].bitc.ICS
#define ICR04_S4    _icr[4].bitrd.S0
#define ICR04_S1    _icr[4].bitrd.S1
#define ICR04_S     _icr[4].bitcrd.S

#define ICR05       _icr[5].byte
#define ICR05_IL0   _icr[5].bit.IL0
#define ICR05_IL1   _icr[5].bit.IL1
#define ICR05_IL2   _icr[5].bit.IL2
#define ICR05_ISE   _icr[5].bit.ISE
#define ICR05_ICS0  _icr[5].bit.ICS0
#define ICR05_ICS1  _icr[5].bit.ICS1
#define ICR05_ICS2  _icr[5].bit.ICS2
#define ICR05_ICS3  _icr[5].bit.ICS3
#define ICR05_IL    _icr[5].bitc.IL
#define ICR05_ICS   _icr[5].bitc.ICS
#define ICR05_S0    _icr[5].bitrd.S0
#define ICR05_S1    _icr[5].bitrd.S1
#define ICR05_S     _icr[5].bitcrd.S

#define ICR06       _icr[6].byte
#define ICR06_IL0   _icr[6].bit.IL0
#define ICR06_IL1   _icr[6].bit.IL1
#define ICR06_IL2   _icr[6].bit.IL2
#define ICR06_ISE   _icr[6].bit.ISE
#define ICR06_ICS0  _icr[6].bit.ICS0
#define ICR06_ICS1  _icr[6].bit.ICS1
#define ICR06_ICS2  _icr[6].bit.ICS2
#define ICR06_ICS3  _icr[6].bit.ICS3
#define ICR06_IL    _icr[6].bitc.IL
#define ICR06_ICS   _icr[6].bitc.ICS
#define ICR06_S0    _icr[6].bitrd.S0
#define ICR06_S1    _icr[6].bitrd.S1
#define ICR06_S     _icr[6].bitcrd.S

#define ICR07       _icr[7].byte
#define ICR07_IL0   _icr[7].bit.IL0
#define ICR07_IL1   _icr[7].bit.IL1
#define ICR07_IL2   _icr[7].bit.IL2
#define ICR07_ISE   _icr[7].bit.ISE
#define ICR07_ICS0  _icr[7].bit.ICS0
#define ICR07_ICS1  _icr[7].bit.ICS1
#define ICR07_ICS2  _icr[7].bit.ICS2
#define ICR07_ICS3  _icr[7].bit.ICS3
#define ICR07_IL    _icr[7].bitc.IL
#define ICR07_ICS   _icr[7].bitc.ICS
#define ICR07_S0    _icr[7].bitrd.S0
#define ICR07_S1    _icr[7].bitrd.S1
#define ICR07_S     _icr[7].bitcrd.S

#define ICR08       _icr[8].byte
#define ICR08_IL0   _icr[8].bit.IL0
#define ICR08_IL1   _icr[8].bit.IL1
#define ICR08_IL2   _icr[8].bit.IL2
#define ICR08_ISE   _icr[8].bit.ISE
#define ICR08_ICS0  _icr[8].bit.ICS0
#define ICR08_ICS1  _icr[8].bit.ICS1
#define ICR08_ICS2  _icr[8].bit.ICS2
#define ICR08_ICS3  _icr[8].bit.ICS3
#define ICR08_IL    _icr[8].bitc.IL
#define ICR08_ICS   _icr[8].bitc.ICS
#define ICR08_S0    _icr[8].bitrd.S0
#define ICR08_S1    _icr[8].bitrd.S1
#define ICR08_S     _icr[8].bitcrd.S

#define ICR09       _icr[9].byte
#define ICR09_IL0   _icr[9].bit.IL0
#define ICR09_IL1   _icr[9].bit.IL1
#define ICR09_IL2   _icr[9].bit.IL2
#define ICR09_ISE   _icr[9].bit.ISE
#define ICR09_ICS0  _icr[9].bit.ICS0
#define ICR09_ICS1  _icr[9].bit.ICS1
#define ICR09_ICS2  _icr[9].bit.ICS2
#define ICR09_ICS3  _icr[9].bit.ICS3
#define ICR09_IL    _icr[9].bitc.IL
#define ICR09_ICS   _icr[9].bitc.ICS
#define ICR09_S0    _icr[9].bitrd.S0
#define ICR09_S1    _icr[9].bitrd.S1
#define ICR09_S     _icr[9].bitcrd.S

#define ICR10       _icr[10].byte
#define ICR10_IL0   _icr[10].bit.IL0
#define ICR10_IL1   _icr[10].bit.IL1
#define ICR10_IL2   _icr[10].bit.IL2
#define ICR10_ISE   _icr[10].bit.ISE
#define ICR10_ICS0  _icr[10].bit.ICS0
#define ICR10_ICS1  _icr[10].bit.ICS1
#define ICR10_ICS2  _icr[10].bit.ICS2
#define ICR10_ICS3  _icr[10].bit.ICS3
#define ICR10_IL    _icr[10].bitc.IL
#define ICR10_ICS   _icr[10].bitc.ICS
#define ICR10_S0    _icr[10].bitrd.S0
#define ICR10_S1    _icr[10].bitrd.S1
#define ICR10_S     _icr[10].bitcrd.S

#define ICR11       _icr[11].byte
#define ICR11_IL0   _icr[11].bit.IL0
#define ICR11_IL1   _icr[11].bit.IL1
#define ICR11_IL2   _icr[11].bit.IL2
#define ICR11_ISE   _icr[11].bit.ISE
#define ICR11_ICS0  _icr[11].bit.ICS0
#define ICR11_ICS1  _icr[11].bit.ICS1
#define ICR11_ICS2  _icr[11].bit.ICS2
#define ICR11_ICS3  _icr[11].bit.ICS3
#define ICR11_IL    _icr[11].bitc.IL
#define ICR11_ICS   _icr[11].bitc.ICS
#define ICR11_S0    _icr[11].bitrd.S0
#define ICR11_S1    _icr[11].bitrd.S1
#define ICR11_S     _icr[11].bitcrd.S

#define ICR12       _icr[12].byte
#define ICR12_IL0   _icr[12].bit.IL0
#define ICR12_IL1   _icr[12].bit.IL1
#define ICR12_IL2   _icr[12].bit.IL2
#define ICR12_ISE   _icr[12].bit.ISE
#define ICR12_ICS0  _icr[12].bit.ICS0
#define ICR12_ICS1  _icr[12].bit.ICS1
#define ICR12_ICS2  _icr[12].bit.ICS2
#define ICR12_ICS3  _icr[12].bit.ICS3
#define ICR12_IL    _icr[12].bitc.IL
#define ICR12_ICS   _icr[12].bitc.ICS
#define ICR12_S0    _icr[12].bitrd.S0
#define ICR12_S1    _icr[12].bitrd.S1
#define ICR12_S     _icr[12].bitcrd.S

#define ICR13       _icr[13].byte
#define ICR13_IL0   _icr[13].bit.IL0
#define ICR13_IL1   _icr[13].bit.IL1
#define ICR13_IL2   _icr[13].bit.IL2
#define ICR13_ISE   _icr[13].bit.ISE
#define ICR13_ICS0  _icr[13].bit.ICS0
#define ICR13_ICS1  _icr[13].bit.ICS1
#define ICR13_ICS2  _icr[13].bit.ICS2
#define ICR13_ICS3  _icr[13].bit.ICS3
#define ICR13_IL    _icr[13].bitc.IL
#define ICR13_ICS   _icr[13].bitc.ICS
#define ICR13_S0    _icr[13].bitrd.S0
#define ICR13_S1    _icr[13].bitrd.S1
#define ICR13_S     _icr[13].bitcrd.S

#define ICR14       _icr[14].byte
#define ICR14_IL0   _icr[14].bit.IL0
#define ICR14_IL1   _icr[14].bit.IL1
#define ICR14_IL2   _icr[14].bit.IL2
#define ICR14_ISE   _icr[14].bit.ISE
#define ICR14_ICS0  _icr[14].bit.ICS0
#define ICR14_ICS1  _icr[14].bit.ICS1
#define ICR14_ICS2  _icr[14].bit.ICS2
#define ICR14_ICS3  _icr[14].bit.ICS3
#define ICR14_IL    _icr[14].bitc.IL
#define ICR14_ICS   _icr[14].bitc.ICS
#define ICR14_S0    _icr[14].bitrd.S0
#define ICR14_S1    _icr[14].bitrd.S1
#define ICR14_S     _icr[14].bitcrd.S

#define ICR15       _icr[15].byte
#define ICR15_IL0   _icr[15].bit.IL0
#define ICR15_IL1   _icr[15].bit.IL1
#define ICR15_IL2   _icr[15].bit.IL2
#define ICR15_ISE   _icr[15].bit.ISE
#define ICR15_ICS0  _icr[15].bit.ICS0
#define ICR15_ICS1  _icr[15].bit.ICS1
#define ICR15_ICS2  _icr[15].bit.ICS2
#define ICR15_ICS3  _icr[15].bit.ICS3
#define ICR15_IL    _icr[15].bitc.IL
#define ICR15_ICS   _icr[15].bitc.ICS
#define ICR15_S0    _icr[15].bitrd.S0
#define ICR15_S1    _icr[15].bitrd.S1
#define ICR15_S     _icr[15].bitcrd.S


#endif /*  __ICR_H */
#  undef ___IOWIDTH
#endif                   /* __MB90XXX_H */
