/*
===============================================================================
| COPYRIGHT (C) 2003-2007 Haishen Ruan, All rights reserved.
| SUMMARY:
|   Architecture system includes
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
#ifndef __arch_h__
#define __arch_h__
/*===========================================================================*/


#ifdef __CPU_16LX__
#define ARCH_DEFINE

#include "example/fmc16lx/mb90560.h"
#include "example/fmc16lx/config.h"
#include "arch/fmc16lx/inc/mem_model.h"
#include "arch/fmc16lx/inc/basetype.h"
#include "arch/types.h"
#include "arch/fmc16lx/inc/intr.h"
#include "arch/fmc16lx/inc/trace.h"

#endif


#ifdef __C51__
#define ARCH_DEFINE

#include <reg51.h>
#include <string.h>
#include "example/KeilC51/config.h"
#include "arch/C51/inc/mem_model.h"
#include "arch/C51/inc/basetype.h"
#include "arch/types.h"
#include "arch/C51/inc/intr.h"
#include "arch/C51/inc/trace.h"

#endif


#ifdef _WIN32
#define ARCH_DEFINE

#include <assert.h>
#include <windows.h>
#include <string.h>
#include "example/win32/config.h"
#include "arch/win32/inc/basetype.h"
#include "arch/types.h"
#include "arch/win32/inc/intr.h"
#include "arch/win32/inc/trace.h"


#endif



#ifndef ARCH_DEFINE
#error This architecture not supported yet!
#endif


/*===========================================================================*/
#endif

