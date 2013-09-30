/*
===============================================================================
| COPYRIGHT (C) 2003-2007 RSGuru, All rights reserved.
| SUMMARY:
|   RS-RTOS Sample support macros define.
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
|
|
| AUTHOR & LOG:
|   Haishen Ruan <ruanhaishen@gmail.com>:
|   9 Sep 2006, Created it.
|
|
=============================================================================*/

#ifndef __APP_SAMPLE_H__
#define __APP_SAMPLE_H__

#ifdef __cplusplus
extern "C" {
#endif
/*===========================================================================*/


/* ÑÝÊ¾Àý×ÓÑ¡Ôñ */
#define APP_SAMPLE_01A


#define UPCASE(c)   (((c)>=97 && (c)<=122) ? (c)-32 : c)

#define APP_SAMPLE_INFO \
do { \
    const char *p = __FILE__; \
    int i = 0;  char name[20]; \
    p += sizeof(__FILE__); \
    while (*p != '\\') p--; p++; \
    while (p[i] != '.') { name[i] = UPCASE(p[i]) ; i++; } \
    name[i] = 0; \
    printk("###################################################\n"); \
    printk("# RS-RTOS V%-3s Sample %-26s #\n", kernel_version_s(), name); \
    printk("# File-Name: %-36s #\n", p); \
    printk("# Compile-Time: %-33s #\n", __DATE__##" "##__TIME__); \
    printk("###################################################\n\n"); \
} while (0)


#define ASSERT  assert


/*===========================================================================*/
#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif


