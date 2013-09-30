/*
===============================================================================
| COPYRIGHT (C) 2003-2007 Haishen Ruan, All rights reserved.
| SUMMARY:
|   Device implementation.
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
#include "arch/arch.h"
#include "inc/const.h"
#include "inc/cque.h"
#include "inc/kernel.h"
#include "inc/memory.h"
#include "inc/ipc.h"
#include "inc/device.h"
#include "inc/kapi.h"
#include "driver/driver.h"


#if CFG_DEVICE_EN > 0

/* Driver control block table. */
static device_driver_t _device_driver[CFG_DEVICE_SIZE];


/*==============================================================
Function: __driver_init
Description: Initialize device driver management
Parameters:
Returns:
Notes:
    Internal function.
==============================================================*/
__fun_
void __driver_init(void) __ef_
{
    RSI_REG int16u i;

    /* Initialize driver control blocks. */
    for (i = 0; i < CFG_DEVICE_SIZE; i++) {

        /* Clear driver control block. */
        _device_driver[i].options = 0;
    }
}


/*==============================================================
Function: driver_install
Description: Install device driver.
Parameters:
    major -- Device major number, 0 means allocate a number
    table -- Device driver interface table
Returns:
    RS_EOUTRANGE -- major number exceed the limitation
    RS_EFULL -- major is exist
    >= 0 -- the major number which is installed
Notes:
==============================================================*/
__fun_
status_t r_driver_install(int16u major, driver_table_t __p_* table) __ef_
{
    RSI_REG int16u i;

    IRQ_SAVEAREA;

    RSI_ASSERT(table != NULL);

#if CFG_ARG_CHK > 1

    /* Wether the device within the limitation. */
    if (major >= CFG_DEVICE_SIZE) {

        /* No, the device exceed the limitation.
         * Return failed. */
        return RS_EOUTRANGE;
    }
    
#else
    RSI_ASSERT(major < CFG_DEVICE_SIZE);
#endif


    /* Default value. */
    i = CFG_DEVICE_SIZE;

    /* Determine if need allocate a major number. */
    if (major == 0) {

        /* Yes.
         * Find a free table space in driver table. */
        for (i = 0; i < CFG_DEVICE_SIZE; i++) {

            irq_lock();

            /* If driver table is not in used. */
            if (_device_driver[i].options == 0) {

                /* Yes. Lock the driver table. */
                _device_driver[i].options = RSI_DRV_LOCK;

                irq_unlock();
                
                /* Found it. */
                break;
            }
            irq_unlock();
        }

    /* Else major number specified by caller. */
    } else {

        irq_lock();

        /* If driver table is not in used. */
        if (_device_driver[major].options == 0) {

            /* Yes. Lock the driver table. */
            _device_driver[major].options = RSI_DRV_LOCK;

            /* Save major number. */
            i = major;
        }
        
        irq_unlock();
    }

    /* Determine if driver can be installed. */
    if (i != CFG_DEVICE_SIZE) {

        /* Yes. Install driver. */
        _device_driver[i].driver_table = *table;
        
        /* Driver options. */
        _device_driver[i].options = RSI_DRV_USED;

        /* Device driver install 
         * successfully, return major which is installed. */
        return (status_t)i;
    }

    /* Return failed. */
    return RS_EFULL;
}


/*==============================================================
Function: driver_uninstall
Description: Uninstall device driver.
Parameters:
    major -- Device major number.
Returns:
    RS_EOUTRANGE -- major number exceed the limitation
    RS_ENOTEXIST -- device driver is not exist
    RS_ELOCK -- device driver is locked
    RS_EOK -- succeed.
Notes:
==============================================================*/
__fun_
status_t r_driver_uninstall(int16u major) __ef_
{
    IRQ_SAVEAREA;

#if CFG_ARG_CHK > 1

    /* Wether the device within the limitation. */
    if (major >= CFG_DEVICE_SIZE) {

        /* No, the device exceed the limitation.
         * Return failed. */
        return RS_EOUTRANGE;
    }
    
#else
    RSI_ASSERT(major < CFG_DEVICE_SIZE);
#endif


    irq_lock();

    /* Determine if device driver is in used. */
    if (_device_driver[major].options == RSI_DRV_USED) {

        /* Yes. Free device driver. */
        _device_driver[major].options = 0;

        irq_unlock();

        /* Return succeed. */
        return RS_EOK;

    /* See if device driver is locked. */
    } else if (_device_driver[major].options & RSI_DRV_LOCK) {

        /* Yes. */
        
        irq_unlock();

        /* Return failed. */
        return RS_ELOCK;

    }
    irq_unlock();

    /* Return failed. */
    return RS_ENOTEXIST;
}


/*==============================================================
Function: __device_ioctl
Description: device I/O control service function
Parameters:
    device -- device
    operate -- operation of device.
    arg -- argument of device driver service function
Returns:
    RS_EOUTRANGE -- major number exceed the limitation
    RS_EUNKNOW -- service function not found.
    <OTHER> -- define by driver service function
Notes:
    Internal function.
==============================================================*/
__fun_
int r_device_ioctl(device_t device, int16u operate, arg_t arg) __ef_
{
    RSI_REG int16u major;
    RSI_REG driver_ioctl caller;
    RSI_REG int status;

    IRQ_SAVEAREA;

    /* Retrieve major number. */
    major = RS_DEVICE_MAJOR(device);
    
#if CFG_DEVICE_SIZE < 0x10000 && CFG_ARG_CHK > 1

    /* Wether the device within the limitation. */
    if (major >= CFG_DEVICE_SIZE) {

        /* No, the device exceed the limitation.
         * Return failed. */
        return RS_EOUTRANGE;
    }
#else
    RSI_ASSERT(major < CFG_DEVICE_SIZE);
#endif

    /* Initialized value. */
    caller = (driver_ioctl)0;

    irq_lock();

    /* Determine if driver has installed. */
    if (_device_driver[major].options & RSI_DRV_USED) {
    
        /* Yes. 
         * Retrieve ioctl function. */
        caller = _device_driver[major].driver_table.ioctl;

        /* Lock driver. */
        _device_driver[major].options |= RSI_DRV_LOCK;
    }
    
    irq_unlock();

    /* Default status. */
    status = RS_EUNKNOW;
    
    /* Determine if caller is not empty. */
    if (caller != (driver_ioctl)0) {

        /* Yes.
         * Invoke ioctl function. */
        status = (*caller)((int16u)device, operate, arg);    
    }

    irq_lock();

    /* Unlock driver. */
    _device_driver[major].options &= ~RSI_DRV_LOCK;

    irq_unlock();

    /* Return status. */
    return status;
}


/*==============================================================
Function: device_read
Description: read from device buffer.
Parameters:
    device -- device
    buff -- buffer of read
    size -- size of buffer
Returns:
    RS_EOUTRANGE -- major number exceed the limitation
    RS_EUNKNOW -- service function not found
    >= 0 -- size which is read
Notes:
==============================================================*/
__fun_
int r_device_read(device_t device, char __out_* buff, int size) __ef_
{
    RSI_REG int16u major;
    RSI_REG driver_read caller;
    RSI_REG int status;

    IRQ_SAVEAREA;

    /* Retrieve major number. */
    major = RS_DEVICE_MAJOR(device);
    
#if CFG_DEVICE_SIZE < 0x10000 && CFG_ARG_CHK > 1

    /* Wether the device within the limitation. */
    if (major >= CFG_DEVICE_SIZE) {

        /* No, the device exceed the limitation.
         * Return failed. */
        return RS_EOUTRANGE;
    }
#else
    RSI_ASSERT(major < CFG_DEVICE_SIZE);
#endif

    /* Initialized value. */
    caller = (driver_read)0;

    irq_lock();

    /* Determine if driver has installed. */
    if (_device_driver[major].options & RSI_DRV_USED) {
    
        /* Yes. 
         * Retrieve read function. */
        caller = _device_driver[major].driver_table.read;

        /* Lock driver. */
        _device_driver[major].options |= RSI_DRV_LOCK;
    }

    irq_unlock();

    /* Default status. */
    status = RS_EUNKNOW;
    
    /* Determine if caller is not empty. */
    if (caller != (driver_read)0) {

        /* Yes.
         * Invoke read function. */
        status = (*caller)((int16u)device, buff, size);    
    }

    irq_lock();

    /* Unlock driver. */
    _device_driver[major].options &= ~RSI_DRV_LOCK;

    irq_unlock();


    /* Return status. */
    return status;
}


/*==============================================================
Function: device_write
Description: write buff to device.
Parameters:
    device -- device
    buff -- buffer to write
    size -- size of buffer
Returns:
    RS_EOUTRANGE -- major number exceed the limitation
    RS_EUNKNOW -- service function not found
    >= 0 -- size which is wrote
Notes:
==============================================================*/
__fun_
int r_device_write(device_t device, const char __p_* buff, int size) __ef_
{
    RSI_REG int16u major;
    RSI_REG driver_write caller;
    RSI_REG int status;

    IRQ_SAVEAREA;

    /* Retrieve major number. */
    major = RS_DEVICE_MAJOR(device);
    
#if CFG_DEVICE_SIZE < 0x10000 && CFG_ARG_CHK > 1

    /* Wether the device within the limitation. */
    if (major >= CFG_DEVICE_SIZE) {

        /* No, the device exceed the limitation.
         * Return failed. */
        return RS_EOUTRANGE;
    }
#else
    RSI_ASSERT(major < CFG_DEVICE_SIZE);
#endif

    /* Initialized value. */
    caller = (driver_write)0;

    irq_lock();

    /* Determine if driver has installed. */
    if (_device_driver[major].options & RSI_DRV_USED) {
    
        /* Yes. 
         * Retrieve read function. */
        caller = _device_driver[major].driver_table.write;

        /* Lock driver. */
        _device_driver[major].options |= RSI_DRV_LOCK;
    }

    irq_unlock();
    
    /* Default status. */
    status = RS_EUNKNOW;
    
    /* Determine if ioctl is not empty. */
    if (caller != (driver_write)0) {

        /* Yes.
         * Invoke ioctl function. */
        status = (*caller)((int16u)device, buff, size);    
    }

    irq_lock();

    /* Unlock driver. */
    _device_driver[major].options &= ~RSI_DRV_LOCK;

    irq_unlock();


    /* Return status. */
    return status;
}


#endif
/*===========================================================================*/


