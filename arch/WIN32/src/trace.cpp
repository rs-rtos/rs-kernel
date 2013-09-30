/*
===============================================================================
| COPYRIGHT (C) 2003-2007 Haishen Ruan, All rights reserved.
| SUMMARY:
|   Tracing system.
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
//#define WINVER	0x0501

#include <afxtempl.h>

extern "C" void __dump(const char* file, int line)
{
    _RPT2(_CRT_WARN, "%s(%d): ", file, line);
}

extern "C" void* __trace(const char *fmt, ...)
{
    char buff[256];
    va_list args;

    va_start(args, fmt);
    vsprintf(buff, fmt, args);
    va_end(args);

    _RPT1(_CRT_WARN, "%s", buff);

    return __trace;
}

extern "C" void __write(const char *s, int count)
{
//#ifdef _CONSOLE
#if 1
    while (count--) {
        putchar(*s++);
    }
#else
    void DebugOut(const char* s, int count);

    DebugOut(s, count);
#endif
}
/*===========================================================================*/


