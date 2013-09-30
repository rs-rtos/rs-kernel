/*
===============================================================================
| COPYRIGHT (C) 2003-2007 Haishen Ruan, All rights reserved.
| SUMMARY:
|   Standard types for RS-KERNEL.
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
#ifndef __TYPES_H__
#define __TYPES_H__

#ifdef __cplusplus
extern "C" {
#endif
/*===========================================================================*/

/* Base types */
typedef unsigned __arch_i8  u8;
typedef   signed __arch_i8  s8;
typedef unsigned __arch_i16 u16;
typedef   signed __arch_i16 s16;
typedef unsigned __arch_i32 u32;
typedef   signed __arch_i32 s32;

typedef unsigned __arch_i8  int8u;
typedef   signed __arch_i8  int8s;
typedef unsigned __arch_i16 int16u;
typedef   signed __arch_i16 int16s;
typedef unsigned __arch_i32 int32u;
typedef   signed __arch_i32 int32s;

#ifndef __cplusplus
#ifndef __arch_bool

/**
 * typedef enum {false, true}  bool;*/
typedef int8u       bool;

#define false       0
#define true        1

#else

#define false       0
#define true        1
#define bool        __arch_bool

#endif
#endif


typedef int8u  uchar;
typedef int8u  byte;
typedef int16u word;


#if ARCH_CPU_BITS == 8
typedef int8u   uint;
typedef int8s   sint;
#elif ARCH_CPU_BITS == 16
typedef int16u  uint;
typedef int16s  sint;
#elif ARCH_CPU_BITS == 32
typedef int32u  uint;
typedef int32s  sint;
#endif


#ifndef NULL
#define NULL    ((void __p_*)0)
#endif


/* Assistant for pointer */
#ifndef __p_
#define __p_
#endif

/* Assistant for constant */
#ifndef __const_
#define __const_
#endif

/* Stack type */
#ifndef __stack
#define __stack     byte
#endif

/* Stack pointer type */
#ifndef __sp
#define __sp        __stack*
#endif

/* Mailbox type */
#ifndef __mail
#define __mail      int32u
#endif

/* Kernel tick type */
#ifndef __tick
#define __tick      int32u
#endif

/* Size memory */
#ifndef __mmsz
#define __mmsz      size_t
#endif

/* Counter type */
#ifndef __count
#define __count     int32u
#endif


/* Argument type */
#ifndef __arg
#define __arg       void __p_*
#endif


/* Register type */
#ifndef __reg
#define __reg       register
#endif


/* Assistant for global data */
#ifndef __extern
#define __extern
#endif


/* Function decorate */
#ifndef __fun_
#define __fun_
#endif

#ifndef __ef_
#define __ef_
#endif



/* Kenrnel date type */
typedef __stack     stack_t;
typedef __sp        sp_t;
typedef __mail      mail_t;
typedef __mmsz      mmsz_t;
typedef __tick      tick_t;
typedef __count     count_t;
typedef int32u      name_t;
typedef int8u       prio_t;
typedef __arg       arg_t;
typedef void (*entry_t)(arg_t);


#if ARCH_CPU_BITS == 8 && CFG_DEVICE_SIZE > 128
typedef int16s      status_t;
#else
typedef sint        status_t;
#endif

/*
typedef status_t    err_t; */


/* Empty code */
#define RSI_MPT     do {} while (0)


/*===========================================================================*/
#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif

