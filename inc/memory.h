/*
===============================================================================
| COPYRIGHT (C) 2003-2007 Haishen Ruan, All rights reserved.
| SUMMARY:
|   Memory management functions and structures definitions.
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
#ifndef __MEMORY_H__
#define __MEMORY_H__

#ifdef __cplusplus
extern "C" {
#endif
/*===========================================================================*/

#if CFG_MPOOL_EN > 0

#define POOLSIZE(x)     (_pools[x].size)
#define POOLTOTAL(x)    (_pools[x].blocks)
#define POOLFREE(x)     (_pools[x].frees)
#define POOLFIRST(x)    (_pools[x].pfree)


#if ARCH_BYTE_ORDER == LITTLE_ENDIAN

typedef struct _struct_block
{
    union {
        struct used {

#if RSI_PTR_SIZE > 2
            int8u  res[RSI_PTR_SIZE - 2];
#endif
            int8u  pool;
            int8u  flag;

        } used;

        struct _struct_block __p_* next;

    } u;

} block_t;

#elif ARCH_BYTE_ORDER == BIG_ENDIAN

typedef struct _struct_block
{
    union {
        struct used {

            int8u  flag;
            int8u  pool;

#if RSI_PTR_SIZE > 2
            int8u  res[RSI_PTR_SIZE - 2];
#endif
        } used;

        struct _struct_block __p_* next;

    } u;

} block_t;

#endif


typedef struct _struct_pool
{
    mmsz_t          size;
    mmsz_t          blocks;
    mmsz_t          frees;
    block_t __p_*   pfree;

} pool_t;


typedef struct _struct_mpinfo
{
    /* Input the index of pools */
    int8u  index;

    mmsz_t          size;
    mmsz_t          blocks;
    mmsz_t          frees;
    block_t __p_*   pfree;

} mpinfo_t;


#if CFG_DEBUG > 0
__fun_ void __mm_flag_check(void) __ef_;
#endif

__fun_ void __mm_init(void) __ef_;
__fun_ void __mm_pool_init(
    byte __p_*  start,
    mmsz_t      size,
    mmsz_t      blocks
    ) __ef_;
__fun_ uint __mm_get_pool(mmsz_t size) __ef_;


__fun_ void __p_* r_mpool_alloc(mmsz_t size) __ef_;
__fun_ void r_mpool_free(void __p_* p) __ef_;
__fun_ status_t r_mpool_info(mpinfo_t __out_* info) __ef_;


#define __mpool_alloc   r_mpool_alloc
#define __mpool_free    r_mpool_free
#define __mpool_info    r_mpool_info


#endif


/*===========================================================================*/
#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif

