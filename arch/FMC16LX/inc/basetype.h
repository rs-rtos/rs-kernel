/*
===============================================================================
| COPYRIGHT (C) 2003-2007 Haishen Ruan, All rights reserved.
| SUMMARY:
|   Base types for F2MC-16LX.
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
#ifndef __BASETYPE_H__
#define __BASETYPE_H__

#ifdef __cplusplus
extern "C" {
#endif
/*===========================================================================*/


/*== Characterize the architecture system ===============*/

/*=========================================================
Description:
    The processor data width.
Value:
    8, 16, 32, 64
=========================================================*/
#define ARCH_CPU_BITS       16


/*=========================================================
Description:
    Stack growing direction.
Value:
    UPWARDS -- from low-memory to high-memory
    DOWNWARDS -- from high-memory to low-memory
=========================================================*/
#define ARCH_STACK_GROW     DOWNWARDS


/*=========================================================
Description:
    Order of data storage in memory.
Value:
    LITTLE_ENDIAN -- data storage as little-endian
    BIG_ENDIAN -- data storage as big-endian
=========================================================*/
#define ARCH_BYTE_ORDER     LITTLE_ENDIAN


/*=========================================================
Description:
    Pointer size.
Value:
=========================================================*/
#define ARCH_PTR_SIZE       2


/*== Base types =========================================*/

#define __arch_i8           char
#define __arch_i16          short
#define __arch_i32          long
#define __arch_i64          long long

#define __mail              __arch_i16
#define __mmsz              __arch_i16


/*===========================================================================*/
#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif

