/*
===============================================================================
| COPYRIGHT (C) 2003-2007 Haishen Ruan, All rights reserved.
| SUMMARY:
|   C-language Memory model for C51.
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
#ifndef __MEM_MODEL_H__
#define __MEM_MODEL_H__


/*=========================================================
Description:
    C-language Memory model. The selected memory model should
    be set in order to fit to the model selected for the compiler.
Value:
    SMALL, COMPACT, LARGE
=========================================================*/
#define RSI_MEM_MODEL    COMPACT



/*=======================================================*/

#define SMALL       0
#define COMPACT     1
#define LARGE_I     2
#define LARGE_II    3

#define UN_SPECIFIED   4        /* unspecified   */



#if RSI_MEM_MODEL != SMALL
 #if RSI_MEM_MODEL != COMPACT
  #if RSI_MEM_MODEL != LARGE_I
  #if RSI_MEM_MODEL != LARGE_II
   #error "RSI_MEM_MODEL" must be specified!
  #endif
  #endif
 #endif
#endif


/*=======================================================*/

#endif


