/*
===============================================================================
| COPYRIGHT (C) 2003-2007 Haishen Ruan, All rights reserved.
| SUMMARY:
|   Base support function and condition variable definitions.
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
#ifndef __DEVICE_H__
#define __DEVICE_H__

#ifdef __cplusplus
extern "C" {
#endif
/*===========================================================================*/

#if CFG_DEVICE_EN > 0


/* Define device identity. */
typedef int32u device_t;

/* Define driver read interface. */
typedef __fun_ int (*driver_read)(int16u, char __out_*, int) __ef_;

/* Define driver write interface. */
typedef __fun_ int (*driver_write)(int16u, const char __p_*, int) __ef_;

/* Define driver control interface. */
typedef __fun_ int (*driver_ioctl)(int16u, int16u, arg_t) __ef_;


/* Define driver table structure. */
typedef struct _struct_driver_table
{
    /* Read request procedure. */
    driver_read read;

    /* Write request procedure. */
    driver_write write;

    /* Special functions procedure. */
    driver_ioctl ioctl;

} driver_table_t;


/* Define driver control block structure. */
typedef struct _struct_device_driver
{
    /* Device Driver table. */
    driver_table_t  driver_table;

    /* Options of driver. */
    int8u  options;
    
} device_driver_t;


/* Operate for device special functions. */
#define RS_DEVICE_INIT      1u
#define RS_DEVICE_OPEN      2u
#define RS_DEVICE_CLOSE     3u
#define RS_DEVICE_IOCTL     4u


/* Pre-define major number of device driver. */
#define RS_DRIVER_UART      (CFG_DEVICE_SIZE - 1)
#define RS_DRIVER_NET       (CFG_DEVICE_SIZE - 2)
#define RS_DRIVER_USB       (CFG_DEVICE_SIZE - 3)


/* Pre-define device. */
#define RS_DEVICE_UART1     build_device(RS_DRIVER_UART, 0)
#define RS_DEVICE_UART2     build_device(RS_DRIVER_UART, 1)
#define RS_DEVICE_NET1      build_device(RS_DRIVER_NET, 0)
#define RS_DEVICE_NET2      build_device(RS_DRIVER_NET, 1)
#define RS_DEVICE_USB1      build_device(RS_DRIVER_USB, 0)
#define RS_DEVICE_USB2      build_device(RS_DRIVER_USB, 1)
#define RS_DEVICE_USB3      build_device(RS_DRIVER_USB, 2)
#define RS_DEVICE_USB4      build_device(RS_DRIVER_USB, 3)
#define RS_DEVICE_USB5      build_device(RS_DRIVER_USB, 4)
#define RS_DEVICE_USB6      build_device(RS_DRIVER_USB, 5)
#define RS_DEVICE_USB7      build_device(RS_DRIVER_USB, 6)
#define RS_DEVICE_USB8      build_device(RS_DRIVER_USB, 7)

#define RS_DEVICE_UART      RS_DEVICE_UART1
#define RS_DEVICE_NET       RS_DEVICE_NET1
#define RS_DEVICE_USB       RS_DEVICE_USB1


/* Driver options. */
#define RSI_DRV_USED        0x01
#define RSI_DRV_LOCK        0x02


__fun_
status_t r_driver_install(
    int16u major,
    driver_table_t __p_* table
    ) __ef_;

__fun_
status_t r_driver_uninstall(int16u major) __ef_;

__fun_
int r_device_ioctl(
    device_t    device,
    int16u      operate,
    arg_t       arg
    ) __ef_;

__fun_
int r_device_write(
    device_t         device,
    const char __p_* buff,
    int              size
    ) __ef_;

__fun_
int r_device_read(
    device_t     device,
    char __out_* buff,
    int          size
    ) __ef_;


/* Build device name. */
#define __build_device(major, minor) \
    (device_t)((int32u)(major) << 16 | (int32u)(minor))


__fun_
void __driver_init(void) __ef_;


#define __device_init(device, arg) \
    r_device_ioctl(device, RS_DEVICE_INIT, arg)

#define __device_open(device, arg) \
    r_device_ioctl(device, RS_DEVICE_OPEN, arg)

#define __device_close(device) \
    r_device_ioctl(device, RS_DEVICE_CLOSE, (arg_t)0)

#define __device_ioctl(device, arg) \
    r_device_ioctl(device, RS_DEVICE_IOCTL, arg)


#define RS_DEVICE_MAJOR(device) \
    (int16u)(device >> 16)

#define RS_DEVICE_MINOR(device) \
    (int16u)(device)


#define __driver_install    r_driver_install
#define __driver_uninstall  r_driver_uninstall
#define __device_write      r_device_write
#define __device_read       r_device_read


#endif

/*===========================================================================*/
#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif

