/*
===============================================================================
| COPYRIGHT (C) 2003-2007 Haishen Ruan, All rights reserved.
| SUMMARY:
|   Memory management implementation.
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


#if CFG_MPOOL_EN > 0

/* The No. of pools. */
#define POOL_NUM    (sizeof(_pools)/sizeof(pool_t))
#define MM_USED     ((int8u)(((int32u)MM_START) >> 24) - 1)


#ifdef CFG_MM_START_ADDR

/* Memory start address. */
#define MM_START    ((byte __p_*)(CFG_MM_START_ADDR))

#else

/* Memory start address. */
#define MM_START    _heap
static  byte        _heap[CFG_MM_SIZE];

#endif


static pool_t       _pools[] = { CFG_MM_BLOCKS };

#if CFG_DEBUG > 0
static int8u        _mm_init = 0;
#endif


#if CFG_DEBUG > 0
/*==============================================================
Function: __mm_flag_check()
Description: Check block structure define
Parameters:
Returns:
Notes:
    Internal function
==============================================================*/
__fun_
void __mm_flag_check(void) __ef_
{
    RSI_REG block_t bk;

#if RSI_PTR_SIZE == 4
    bk.u.next = (block_t __p_*)0x19831117L;
    RSI_ASSERT(bk.u.used.flag == 0x19);
#elif RSI_PTR_SIZE == 3
    bk.u.next = (block_t __p_*)0x198311L;
    RSI_ASSERT(bk.u.used.flag == 0x19);
#elif RSI_PTR_SIZE == 2
    bk.u.next = (block_t __p_*)0x1983;
    RSI_ASSERT(bk.u.used.flag == 0x19);
#elif RSI_PTR_SIZE == 1
    #error "Basetype error:RSI_PTR_SIZE must be > 1"
#endif

}
#endif


/*==============================================================
Function: __mm_init()
Description: Initialize memory management
Parameters:
Returns:
Notes:
    Internal function
==============================================================*/
__fun_
void __mm_init(void) __ef_
{
    RSI_REG uint i;
    RSI_REG byte __p_* block_start;

#if CFG_DEBUG > 0
    __mm_flag_check();
#endif

    /* Point to start of memory. */
    block_start = MM_START;

    /* Initialize all of pools. */
    for (i = 0; i < POOL_NUM; i++) {

        /* Initialize blocks in a memory pool. */
        __mm_pool_init(block_start, _pools[i].size, _pools[i].blocks);

        /* Point to the first free block. */
        _pools[i].pfree = (block_t __p_*)block_start;

        /* Start address of next pool. */
        block_start += _pools[i].size * _pools[i].blocks;

        /* Save no. of free blocks. */
        _pools[i].frees = _pools[i].blocks;
    }

#if CFG_DEBUG > 0
    _mm_init = RSI_INITED;
#endif
}


/*==============================================================
Function: __mm_pool_init()
Description: Initialize a memory pool
Parameters:
    start -- start pointer of pool
    size -- size of block
    blocks -- no. of block
Returns:
Notes:
    Internal function
==============================================================*/
__fun_
void __mm_pool_init(byte __p_* start, mmsz_t size, mmsz_t blocks) __ef_
{
    RSI_REG int32u i;
    RSI_REG block_t __p_* bk;

    /* Point to start address of pool. */
    bk = (block_t __p_*)start;

    /* Initialize blocks */
    for (i = 0; i < blocks - 1; i++) {

        /* Link with next block. */
        bk->u.next = (block_t __p_*)((byte __p_*)bk + size);

        /* Point to next block */
        bk = bk->u.next;
    }

    /* The last block. */
    bk->u.next = NULL;
}


/*==============================================================
Function: __mm_get_pool()
Description: Find a pool with appropriate size.
Parameters:
    size -- specified size
Returns:
    index of pool
Notes:
    Internal function
==============================================================*/
__fun_
uint __mm_get_pool(mmsz_t size) __ef_
{
    RSI_REG uint i;

    /* Add the size of the header */
    size += sizeof(block_t);

    /* In all pools */
    for (i = 0; i < POOL_NUM; i++) {

        /* If pool size is satisfied */
        if (size <= _pools[i].size) {

            /* Yes. Return index */
            return i;
        }
    }

    /* Not satisfied size, Return max index. */
    return POOL_NUM;
}


/*==============================================================
Function: mpool_alloc()
Description: Allocate size bytes of memory and return a pointer to it.
Parameters:
    size -- specified size to allocate
Returns:
    pointer to memory
Notes:
==============================================================*/
__fun_
void __p_* r_mpool_alloc(mmsz_t size) __ef_
{
    RSI_REG uint pool;
    RSI_REG block_t __p_* bk;

    IRQ_SAVEAREA;

    /*RSI_ASSERT(size != 0);*/
    RSI_ASSERT(_mm_init == RSI_INITED);

    /* Get index of pool. */
    pool = __mm_get_pool(size);

    /* Determine if a satisfied pool has found. */
    if (pool == POOL_NUM) {

        /* No. Return empty. */
        return NULL;
    }

    irq_lock();

    /* Get a free block in pool. */
    if ((bk = _pools[pool].pfree) != NULL) {

        /* Point to next block. */
        _pools[pool].pfree = bk->u.next;

        /* Decrease no. of free blocks. */
        _pools[pool].frees--;

        irq_unlock();

        /* Setup the flag. */
        bk->u.used.pool = (int8u)(pool);
        bk->u.used.flag = RSI_MM_USED;

        /* Increments past header. */
        return bk + 1;
    }

    irq_unlock();

    /* Return empty. */
    return NULL;
}


/*==============================================================
Function: mpool_free()
Description: Free a block of memory previously allocated with mpool_alloc.
Parameters:
    p -- pointer to memory
Returns:
Notes:
==============================================================*/
__fun_
void r_mpool_free(void __p_* p) __ef_
{
    RSI_REG uint pool;
    RSI_REG block_t __p_* bk;

    IRQ_SAVEAREA;

    RSI_ASSERT(p != NULL);
    RSI_ASSERT(_mm_init == RSI_INITED);

    /* Point to start address of block. */
    bk = (block_t __p_*)p - 1;

    irq_lock();

    /* Check if block is used. */
    if (bk->u.used.flag == RSI_MM_USED) {

        /* Get index of pool. */
        pool = (uint)bk->u.used.pool;

        /* Release it to pool. */
        bk->u.next = _pools[pool].pfree;
        _pools[pool].pfree = bk;

        /* Increase no. of free blocks. */
        _pools[pool].frees++;
    }

    irq_unlock();
}


/*==============================================================
Function: mpool_info()
Description: Get memory infomation
Parameters:
    info -- pointer to infomation structure.
Returns:
    RS_ENOTEXIST -- specified pool not found
    RS_EOK -- succeed
Notes:
==============================================================*/
__fun_
status_t r_mpool_info(mpinfo_t __out_* info) __ef_
{
    RSI_REG uint i;

    IRQ_SAVEAREA;

    RSI_ASSERT(_mm_init == RSI_INITED);

    /* Retrieve index of pool. */
    i = info->index;

    /* Determine if specified pool is exist. */
    if (i < POOL_NUM) {

        /* Retrieve pool information. */
        info->size   = _pools[i].size;
        info->blocks = _pools[i].blocks;

        irq_lock();

        info->frees = _pools[i].frees;
        info->pfree  = _pools[i].pfree;

        irq_unlock();

        /* Return succeed. */
        return RS_EOK;
    }

    /* Specified pool is not exist.
     * Return failed. */
    return RS_ENOTEXIST;
}
#endif


/*===========================================================================*/


