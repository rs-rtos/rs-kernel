/*
===============================================================================
| COPYRIGHT (C) 2003-2007 Haishen Ruan, All rights reserved.
| SUMMARY:
|   Internal common queue interfaces.
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
#ifndef __CQUE_H__
#define __CQUE_H__

#ifdef __cplusplus
extern "C" {
#endif
/*===========================================================================*/
#define __CLR_QB(prio, q) \
    if ((prio) < ARCH_CPU_BITS) { \
        q &= ~(1 << (prio)); \
        break; \
    }

#define __CLR_Q(prio, q, i) \
    if ((prio) < (i + 1) * ARCH_CPU_BITS) { \
        q &= ~(1 << ((prio) - i * ARCH_CPU_BITS)); \
        break; \
    }

#define __CLR_QE(prio, q, i) \
    q &= ~(1 << ((prio) - i * ARCH_CPU_BITS))

#define __CLR_QBE(prio, q) \
    q &= ~(1 << (prio))

/*=======================================================*/
#define __SET_QB(prio, q) \
    if ((prio) < ARCH_CPU_BITS ) { \
        q |= (1 << (prio)); \
        break; \
    }

#define __SET_Q(prio, q, i) \
    if ((prio) < (i + 1) * ARCH_CPU_BITS) { \
        q |= (1 << ((prio) - i * ARCH_CPU_BITS)); \
        break; \
    }

#define __SET_QE(prio, q, i) \
    q |= (1 << ((prio) - i * ARCH_CPU_BITS))

#define __SET_QBE(prio, q) \
    q |= (1 << (prio))

/*=======================================================*/
#define __GET_Q_I0F0(next, q) \
    next = _task_map[(int8u)q]

#define __GET_Q_I0F1(next, q, f) \
    next = _task_map[(int8u)(q >> f)]

/*=======================================================*/
#define __G_AJST(next, i, f) \
    next += i * ARCH_CPU_BITS + f

#define __G_AJST_I0F0(next) \
    if (next != 0xff) { \
        break; \
    }

#define __G_AJST_I1F1(next, i, f) \
    if (next != 0xff) { \
        __G_AJST(next, i, f); \
        break; \
    }

#define __G_AJST_I1F1E(next, i, f) \
    __G_AJST(next, i, f)

/*=======================================================*/
#define __T_CLR(q, next, f) \
    q &= ~((1 << f) << next)

#define __T_CLR_I0F0(q, next) \
    if (next != 0xff) { \
        __T_CLR(q, next, 0); \
        break; \
    }

#define __T_CLR_I1F1(q, next, i, f) \
    if (next != 0xff) { \
        __T_CLR(q, next, f); \
        next += i * ARCH_CPU_BITS + f; \
        break; \
    }

#define __T_CLR_I0F0E(q, next) \
    __T_CLR(q, next, 0)

#define __T_CLR_I1F1E(q, next, i, f) \
    __T_CLR(q, next, f); \
    next += i * ARCH_CPU_BITS + f

/*=======================================================*/
#if ARCH_CPU_BITS == 64
#define __GET_QB(next, q) \
    __GET_Q_I0F0(next, q);      __G_AJST_I0F0(next) \
    __GET_Q_I0F1(next, q, 8);   __G_AJST_I1F1(next, 0, 8) \
    __GET_Q_I0F1(next, q, 16);  __G_AJST_I1F1(next, 0, 16) \
    __GET_Q_I0F1(next, q, 24);  __G_AJST_I1F1(next, 0, 24) \
    __GET_Q_I0F1(next, q, 32);  __G_AJST_I1F1(next, 0, 32) \
    __GET_Q_I0F1(next, q, 40);  __G_AJST_I1F1(next, 0, 40) \
    __GET_Q_I0F1(next, q, 48);  __G_AJST_I1F1(next, 0, 48) \
    __GET_Q_I0F1(next, q, 56);  __G_AJST_I1F1(next, 0, 56)
#define __GET_Q(next, q, i) \
    __GET_Q_I0F0(next, q);      __G_AJST_I1F1(next, i, 0) \
    __GET_Q_I0F1(next, q, 8);   __G_AJST_I1F1(next, i, 8) \
    __GET_Q_I0F1(next, q, 16);  __G_AJST_I1F1(next, i, 16) \
    __GET_Q_I0F1(next, q, 24);  __G_AJST_I1F1(next, i, 24) \
    __GET_Q_I0F1(next, q, 32);  __G_AJST_I1F1(next, i, 32) \
    __GET_Q_I0F1(next, q, 40);  __G_AJST_I1F1(next, i, 40) \
    __GET_Q_I0F1(next, q, 48);  __G_AJST_I1F1(next, i, 48) \
    __GET_Q_I0F1(next, q, 56);  __G_AJST_I1F1(next, i, 56)
#define __GET_QE(next, q, i) \
    __GET_Q_I0F0(next, q);      __G_AJST_I1F1(next, i, 0) \
    __GET_Q_I0F1(next, q, 8);   __G_AJST_I1F1(next, i, 8) \
    __GET_Q_I0F1(next, q, 16);  __G_AJST_I1F1(next, i, 16) \
    __GET_Q_I0F1(next, q, 24);  __G_AJST_I1F1(next, i, 24) \
    __GET_Q_I0F1(next, q, 32);  __G_AJST_I1F1(next, i, 32) \
    __GET_Q_I0F1(next, q, 40);  __G_AJST_I1F1(next, i, 40) \
    __GET_Q_I0F1(next, q, 48);  __G_AJST_I1F1(next, i, 48) \
    __GET_Q_I0F1(next, q, 56);  __G_AJST_I1F1E(next, i, 56)
#define __GET_QBE(next, q) \
    __GET_Q_I0F0(next, q);      __G_AJST_I0F0(next) \
    __GET_Q_I0F1(next, q, 8);   __G_AJST_I1F1(next, 0, 8) \
    __GET_Q_I0F1(next, q, 16);  __G_AJST_I1F1(next, 0, 16) \
    __GET_Q_I0F1(next, q, 24);  __G_AJST_I1F1(next, 0, 24) \
    __GET_Q_I0F1(next, q, 32);  __G_AJST_I1F1(next, 0, 32) \
    __GET_Q_I0F1(next, q, 40);  __G_AJST_I1F1(next, 0, 40) \
    __GET_Q_I0F1(next, q, 48);  __G_AJST_I1F1(next, 0, 48) \
    __GET_Q_I0F1(next, q, 56);  __G_AJST_I1F1E(next, 0, 56)

#define __TAKE_QB(next, q) \
    __GET_Q_I0F0(next, q);      __T_CLR_I0F0(q, next) \
    __GET_Q_I0F1(next, q, 8);   __T_CLR_I1F1(q, next, 0, 8) \
    __GET_Q_I0F1(next, q, 16);  __T_CLR_I1F1(q, next, 0, 16) \
    __GET_Q_I0F1(next, q, 24);  __T_CLR_I1F1(q, next, 0, 24) \
    __GET_Q_I0F1(next, q, 32);  __T_CLR_I1F1(q, next, 0, 32) \
    __GET_Q_I0F1(next, q, 40);  __T_CLR_I1F1(q, next, 0, 40) \
    __GET_Q_I0F1(next, q, 48);  __T_CLR_I1F1(q, next, 0, 48) \
    __GET_Q_I0F1(next, q, 56);  __T_CLR_I1F1(q, next, 0, 56)
#define __TAKE_Q(next, q, i) \
    __GET_Q_I0F0(next, q);      __T_CLR_I1F1(q, next, i, 0) \
    __GET_Q_I0F1(next, q, 8);   __T_CLR_I1F1(q, next, i, 8) \
    __GET_Q_I0F1(next, q, 16);  __T_CLR_I1F1(q, next, i, 16) \
    __GET_Q_I0F1(next, q, 24);  __T_CLR_I1F1(q, next, i, 24) \
    __GET_Q_I0F1(next, q, 32);  __T_CLR_I1F1(q, next, i, 32) \
    __GET_Q_I0F1(next, q, 40);  __T_CLR_I1F1(q, next, i, 40) \
    __GET_Q_I0F1(next, q, 48);  __T_CLR_I1F1(q, next, i, 48) \
    __GET_Q_I0F1(next, q, 56);  __T_CLR_I1F1(q, next, i, 56)
#define __TAKE_QE(next, q, i) \
    __GET_Q_I0F0(next, q);      __T_CLR_I1F1(q, next, i, 0) \
    __GET_Q_I0F1(next, q, 8);   __T_CLR_I1F1(q, next, i, 8) \
    __GET_Q_I0F1(next, q, 16);  __T_CLR_I1F1(q, next, i, 16) \
    __GET_Q_I0F1(next, q, 24);  __T_CLR_I1F1(q, next, i, 24) \
    __GET_Q_I0F1(next, q, 32);  __T_CLR_I1F1(q, next, i, 32) \
    __GET_Q_I0F1(next, q, 40);  __T_CLR_I1F1(q, next, i, 40) \
    __GET_Q_I0F1(next, q, 48);  __T_CLR_I1F1(q, next, i, 48) \
    __GET_Q_I0F1(next, q, 56);  __T_CLR_I1F1E(q, next, i, 56)
#define __TAKE_QBE(next, q) \
    __GET_Q_I0F0(next, q);      __T_CLR_I0F0(q, next) \
    __GET_Q_I0F1(next, q, 8);   __T_CLR_I1F1(q, next, 0, 8) \
    __GET_Q_I0F1(next, q, 16);  __T_CLR_I1F1(q, next, 0, 16) \
    __GET_Q_I0F1(next, q, 24);  __T_CLR_I1F1(q, next, 0, 24) \
    __GET_Q_I0F1(next, q, 32);  __T_CLR_I1F1(q, next, 0, 32) \
    __GET_Q_I0F1(next, q, 40);  __T_CLR_I1F1(q, next, 0, 40) \
    __GET_Q_I0F1(next, q, 48);  __T_CLR_I1F1(q, next, 0, 48) \
    __GET_Q_I0F1(next, q, 56);  __T_CLR_I1F1E(q, next, 0, 56)


#elif ARCH_CPU_BITS == 32
#define __GET_QB(next, q) \
    __GET_Q_I0F0(next, q);      __G_AJST_I0F0(next) \
    __GET_Q_I0F1(next, q, 8);   __G_AJST_I1F1(next, 0, 8) \
    __GET_Q_I0F1(next, q, 16);  __G_AJST_I1F1(next, 0, 16) \
    __GET_Q_I0F1(next, q, 24);  __G_AJST_I1F1(next, 0, 24)
#define __GET_Q(next, q, i) \
    __GET_Q_I0F0(next, q);      __G_AJST_I1F1(next, i, 0) \
    __GET_Q_I0F1(next, q, 8);   __G_AJST_I1F1(next, i, 8) \
    __GET_Q_I0F1(next, q, 16);  __G_AJST_I1F1(next, i, 16) \
    __GET_Q_I0F1(next, q, 24);  __G_AJST_I1F1(next, i, 24)
#define __GET_QE(next, q, i) \
    __GET_Q_I0F0(next, q);      __G_AJST_I1F1(next, i, 0) \
    __GET_Q_I0F1(next, q, 8);   __G_AJST_I1F1(next, i, 8) \
    __GET_Q_I0F1(next, q, 16);  __G_AJST_I1F1(next, i, 16) \
    __GET_Q_I0F1(next, q, 24);  __G_AJST_I1F1E(next, i, 24)
#define __GET_QBE(next, q) \
    __GET_Q_I0F0(next, q);      __G_AJST_I0F0(next) \
    __GET_Q_I0F1(next, q, 8);   __G_AJST_I1F1(next, 0, 8) \
    __GET_Q_I0F1(next, q, 16);  __G_AJST_I1F1(next, 0, 16) \
    __GET_Q_I0F1(next, q, 24);  __G_AJST_I1F1E(next, 0, 24)

#define __TAKE_QB(next, q) \
    __GET_Q_I0F0(next, q);      __T_CLR_I0F0(q, next) \
    __GET_Q_I0F1(next, q, 8);   __T_CLR_I1F1(q, next, 0, 8) \
    __GET_Q_I0F1(next, q, 16);  __T_CLR_I1F1(q, next, 0, 16) \
    __GET_Q_I0F1(next, q, 24);  __T_CLR_I1F1(q, next, 0, 24)
#define __TAKE_Q(next, q, i) \
    __GET_Q_I0F0(next, q);      __T_CLR_I1F1(q, next, i, 0) \
    __GET_Q_I0F1(next, q, 8);   __T_CLR_I1F1(q, next, i, 8) \
    __GET_Q_I0F1(next, q, 16);  __T_CLR_I1F1(q, next, i, 16) \
    __GET_Q_I0F1(next, q, 24);  __T_CLR_I1F1(q, next, i, 24)
#define __TAKE_QE(next, q, i) \
    __GET_Q_I0F0(next, q);      __T_CLR_I1F1(q, next, i, 0) \
    __GET_Q_I0F1(next, q, 8);   __T_CLR_I1F1(q, next, i, 8) \
    __GET_Q_I0F1(next, q, 16);  __T_CLR_I1F1(q, next, i, 16) \
    __GET_Q_I0F1(next, q, 24);  __T_CLR_I1F1E(q, next, i, 24)
#define __TAKE_QBE(next, q) \
    __GET_Q_I0F0(next, q);      __T_CLR_I0F0(q, next) \
    __GET_Q_I0F1(next, q, 8);   __T_CLR_I1F1(q, next, 0, 8) \
    __GET_Q_I0F1(next, q, 16);  __T_CLR_I1F1(q, next, 0, 16) \
    __GET_Q_I0F1(next, q, 24);  __T_CLR_I1F1E(q, next, 0, 24)


#elif ARCH_CPU_BITS == 16
#define __GET_QB(next, q) \
    __GET_Q_I0F0(next, q);      __G_AJST_I0F0(next) \
    __GET_Q_I0F1(next, q, 8);   __G_AJST_I1F1(next,0, 8)
#define __GET_Q(next, q, i) \
    __GET_Q_I0F0(next, q);      __G_AJST_I1F1(next, i, 0) \
    __GET_Q_I0F1(next, q, 8);   __G_AJST_I1F1(next, i, 8)
#define __GET_QE(next, q, i) \
    __GET_Q_I0F0(next, q);      __G_AJST_I1F1(next, i, 0) \
    __GET_Q_I0F1(next, q, 8);   __G_AJST_I1F1E(next, i, 8)
#define __GET_QBE(next, q) \
    __GET_Q_I0F0(next, q);      __G_AJST_I0F0(next) \
    __GET_Q_I0F1(next, q, 8);   __G_AJST_I1F1E(next, 0, 8)

#define __TAKE_QB(next, q) \
    __GET_Q_I0F0(next, q);      __T_CLR_I0F0(q, next) \
    __GET_Q_I0F1(next, q, 8);   __T_CLR_I1F1(q, next, 0, 8)
#define __TAKE_Q(next, q, i) \
    __GET_Q_I0F0(next, q);      __T_CLR_I1F1(q, next, i, 0) \
    __GET_Q_I0F1(next, q, 8);   __T_CLR_I1F1(q, next, i, 8)
#define __TAKE_QE(next, q, i) \
    __GET_Q_I0F0(next, q);      __T_CLR_I1F1(q, next, i, 0) \
    __GET_Q_I0F1(next, q, 8);   __T_CLR_I1F1E(q, next, i, 8)
#define __TAKE_QBE(next, q) \
    __GET_Q_I0F0(next, q);      __T_CLR_I0F0(q, next) \
    __GET_Q_I0F1(next, q, 8);   __T_CLR_I1F1E(q, next, 0, 8)


#elif ARCH_CPU_BITS == 8
#define __GET_QB(next, q) \
    __GET_Q_I0F0(next, q);      __G_AJST_I0F0(next)
#define __GET_Q(next, q, i) \
    __GET_Q_I0F0(next, q);      __G_AJST_I1F1(next, i, 0)
#define __GET_QE(next, q, i) \
    __GET_Q_I0F0(next, q);      __G_AJST_I1F1E(next, i, 0)
#define __GET_QBE(next, q) \
    __GET_Q_I0F0(next, q)

#define __TAKE_QB(next, q) \
    __GET_Q_I0F0(next, q);      __T_CLR_I0F0(q, next)
#define __TAKE_Q(next, q, i) \
    __GET_Q_I0F0(next, q);      __T_CLR_I1F1(q, next, i, 0)
#define __TAKE_QE(next, q, i) \
    __GET_Q_I0F0(next, q);      __T_CLR_I1F1E(q, next, i, 0)
#define __TAKE_QBE(next, q) \
    __GET_Q_I0F0(next, q);      __T_CLR_I0F0E(q, next)
#endif

/*=======================================================*/
#if RSI_QUEUE_SZ == 1
#define __CLR_ALL(Q)    Q = 0
#else
#define __CLR_ALL(Q) \
do { \
    RSI_REG uint iii; \
    for(iii = 0; iii < RSI_QUEUE_SZ; iii++) \
        Q[iii] = 0; \
} while (0)
#endif

/*=======================================================*/
#define __NQB(i)   (__q[i] != Q[i])

#if RSI_QUEUE_SZ == 1
#define __IF1(Q)            (Q)
#define __CLR(prio, Q)      __CLR_QBE(prio, Q)
#define __SET(prio, Q)      __SET_QBE(prio, Q)
#define __GET(next, Q)      __GET_QBE(next, Q)
#define __TAKE(next, Q)     __TAKE_QBE(next, Q)
#define __SAVEQ(Q)          uint __q = Q
#define __NQ(Q)             __q != Q

#elif RSI_QUEUE_SZ == 2
#define __IF1(Q)    (Q[0]|Q[1])
#define __CLR(prio, Q) \
    __CLR_QB(prio, Q[0]) \
    __CLR_QE(prio, Q[1], 1)

#define __SET(prio, Q) \
    __SET_QB(prio, Q[0]) \
    __SET_QE(prio, Q[1], 1)

#define __GET(next, Q) \
    __GET_QB(next, Q[0]) \
    __GET_QE(next, Q[1], 1)

#define __TAKE(next, Q) \
    __TAKE_QB(next, Q[0]) \
    __TAKE_QE(next, Q[1], 1)

#define __SAVEQ(Q) \
    uint __q[2]; \
    __q[0] = Q[0]; \
    __q[1] = Q[1]

#define __NQ(Q) \
    __q[0] != Q[0] || \
    __q[1] != Q[1]

#elif RSI_QUEUE_SZ == 3
#define __IF1(Q)    (Q[0]|Q[1]|Q[2])
#define __CLR(prio, Q) \
    __CLR_QB(prio, Q[0]) \
    __CLR_Q(prio, Q[1], 1) \
    __CLR_QE(prio, Q[2], 2)

#define __SET(prio, Q) \
    __SET_QB(prio, Q[0]) \
    __SET_Q(prio, Q[1], 1) \
    __SET_QE(prio, Q[2], 2)

#define __GET(next, Q) \
    __GET_QB(next, Q[0]) \
    __GET_Q(next, Q[1], 1) \
    __GET_QE(next, Q[2], 2)

#define __TAKE(next, Q) \
    __TAKE_QB(next, Q[0]) \
    __TAKE_Q(next, Q[1], 1) \
    __TAKE_QE(next, Q[2], 2)

#define __SAVEQ(Q) \
    uint __q[3]; \
    __q[0] = Q[0]; \
    __q[1] = Q[1]; \
    __q[2] = Q[2]

#define __NQ(Q) \
    __q[0] != Q[0] || \
    __q[1] != Q[1] || \
    __q[2] != Q[2]

#elif RSI_QUEUE_SZ == 4
#define __IF1(Q)    (Q[0]|Q[1]|Q[2]|Q[3])
#define __CLR(prio, Q) \
    __CLR_QB(prio, Q[0]) \
    __CLR_Q(prio, Q[1], 1) \
    __CLR_Q(prio, Q[2], 2) \
    __CLR_QE(prio, Q[3], 3)

#define __SET(prio, Q) \
    __SET_QB(prio, Q[0]) \
    __SET_Q(prio, Q[1], 1) \
    __SET_Q(prio, Q[2], 2) \
    __SET_QE(prio, Q[3], 3)

#define __GET(next, Q) \
    __GET_QB(next, Q[0]) \
    __GET_Q(next, Q[1], 1) \
    __GET_Q(next, Q[2], 2) \
    __GET_QE(next, Q[3], 3)

#define __TAKE(next, Q) \
    __TAKE_QB(next, Q[0]) \
    __TAKE_Q(next, Q[1], 1) \
    __TAKE_Q(next, Q[2], 2) \
    __TAKE_QE(next, Q[3], 3)

#define __SAVEQ(Q) \
    uint __q[4]; \
    __q[0] = Q[0]; \
    __q[1] = Q[1]; \
    __q[2] = Q[2]; \
    __q[3] = Q[3]

#define __NQ(Q) \
    __q[0] != Q[0] || \
    __q[1] != Q[1] || \
    __q[2] != Q[2] || \
    __q[3] != Q[3]

#elif RSI_QUEUE_SZ == 5
#define __IF1(Q)    (Q[0]|Q[1]|Q[2]|Q[3]|Q[4])
#define __CLR(prio, Q) \
    __CLR_QB(prio, Q[0]) \
    __CLR_Q(prio, Q[1], 1) \
    __CLR_Q(prio, Q[2], 2) \
    __CLR_Q(prio, Q[3], 3) \
    __CLR_QE(prio, Q[4], 4)

#define __SET(prio, Q) \
    __SET_QB(prio, Q[0]) \
    __SET_Q(prio, Q[1], 1) \
    __SET_Q(prio, Q[2], 2) \
    __SET_Q(prio, Q[3], 3) \
    __SET_QE(prio, Q[4], 4)

#define __GET(next, Q) \
    __GET_QB(next, Q[0]) \
    __GET_Q(next, Q[1], 1) \
    __GET_Q(next, Q[2], 2) \
    __GET_Q(next, Q[3], 3) \
    __GET_QE(next, Q[4], 4)

#define __TAKE(next, Q) \
    __TAKE_QB(next, Q[0]) \
    __TAKE_Q(next, Q[1], 1) \
    __TAKE_Q(next, Q[2], 2) \
    __TAKE_Q(next, Q[3], 3) \
    __TAKE_QE(next, Q[4], 4)

#define __SAVEQ(Q) \
    uint __q[5]; \
    __q[0] = Q[0]; \
    __q[1] = Q[1]; \
    __q[2] = Q[2]; \
    __q[3] = Q[3]; \
    __q[4] = Q[4]

#define __NQ(Q) \
    __q[0] != Q[0] || \
    __q[1] != Q[1] || \
    __q[2] != Q[2] || \
    __q[3] != Q[3] || \
    __q[4] != Q[4]

#elif RSI_QUEUE_SZ == 6
#define __IF1(Q)    (Q[0]|Q[1]|Q[2]|Q[3]|Q[4]|Q[5])
#define __CLR(prio, Q) \
    __CLR_QB(prio, Q[0]) \
    __CLR_Q(prio, Q[1], 1) \
    __CLR_Q(prio, Q[2], 2) \
    __CLR_Q(prio, Q[3], 3) \
    __CLR_Q(prio, Q[4], 4) \
    __CLR_QE(prio, Q[5], 5)

#define __SET(prio, Q) \
    __SET_QB(prio, Q[0]) \
    __SET_Q(prio, Q[1], 1) \
    __SET_Q(prio, Q[2], 2) \
    __SET_Q(prio, Q[3], 3) \
    __SET_Q(prio, Q[4], 4) \
    __SET_QE(prio, Q[5], 5)

#define __GET(next, Q) \
    __GET_QB(next, Q[0]) \
    __GET_Q(next, Q[1], 1) \
    __GET_Q(next, Q[2], 2) \
    __GET_Q(next, Q[3], 3) \
    __GET_Q(next, Q[4], 4) \
    __GET_QE(next, Q[5], 5)

#define __TAKE(next, Q) \
    __TAKE_QB(next, Q[0]) \
    __TAKE_Q(next, Q[1], 1) \
    __TAKE_Q(next, Q[2], 2) \
    __TAKE_Q(next, Q[3], 3) \
    __TAKE_Q(next, Q[4], 4) \
    __TAKE_QE(next, Q[5], 5)

#define __SAVEQ(Q) \
    uint __q[6]; \
    __q[0] = Q[0]; \
    __q[1] = Q[1]; \
    __q[2] = Q[2]; \
    __q[3] = Q[3]; \
    __q[4] = Q[4]; \
    __q[5] = Q[5]

#define __NQ(Q) \
    __q[0] != Q[0] || \
    __q[1] != Q[1] || \
    __q[2] != Q[2] || \
    __q[3] != Q[3] || \
    __q[4] != Q[4] || \
    __q[5] != Q[5]

#elif RSI_QUEUE_SZ == 7
#define __IF1(Q)    (Q[0]|Q[1]|Q[2]|Q[3]|Q[4]|Q[5]|Q[6])
#define __CLR(prio, Q) \
    __CLR_QB(prio, Q[0]) \
    __CLR_Q(prio, Q[1], 1) \
    __CLR_Q(prio, Q[2], 2) \
    __CLR_Q(prio, Q[3], 3) \
    __CLR_Q(prio, Q[4], 4) \
    __CLR_Q(prio, Q[5], 5) \
    __CLR_QE(prio, Q[6], 6)

#define __SET(prio, Q) \
    __SET_QB(prio, Q[0]) \
    __SET_Q(prio, Q[1], 1) \
    __SET_Q(prio, Q[2], 2) \
    __SET_Q(prio, Q[3], 3) \
    __SET_Q(prio, Q[4], 4) \
    __SET_Q(prio, Q[5], 5) \
    __SET_QE(prio, Q[6], 6)

#define __GET(next, Q) \
    __GET_QB(next, Q[0]) \
    __GET_Q(next, Q[1], 1) \
    __GET_Q(next, Q[2], 2) \
    __GET_Q(next, Q[3], 3) \
    __GET_Q(next, Q[4], 4) \
    __GET_Q(next, Q[5], 5) \
    __GET_QE(next, Q[6], 6)

#define __TAKE(next, Q) \
    __TAKE_QB(next, Q[0]) \
    __TAKE_Q(next, Q[1], 1) \
    __TAKE_Q(next, Q[2], 2) \
    __TAKE_Q(next, Q[3], 3) \
    __TAKE_Q(next, Q[4], 4) \
    __TAKE_Q(next, Q[5], 5) \
    __TAKE_QE(next, Q[6], 6)

#define __SAVEQ(Q) \
    uint __q[7]; \
    __q[0] = Q[0]; \
    __q[1] = Q[1]; \
    __q[2] = Q[2]; \
    __q[3] = Q[3]; \
    __q[4] = Q[4]; \
    __q[5] = Q[5]; \
    __q[6] = Q[6]

#define __NQ(Q) \
    __q[0] != Q[0] || \
    __q[1] != Q[1] || \
    __q[2] != Q[2] || \
    __q[3] != Q[3] || \
    __q[4] != Q[4] || \
    __q[5] != Q[5] || \
    __q[6] != Q[6]

#elif RSI_QUEUE_SZ == 8
#define __IF1(Q)    (Q[0]|Q[1]|Q[2]|Q[3]|Q[4]|Q[5]|Q[6]|Q[7])
#define __CLR(prio, Q) \
    __CLR_QB(prio, Q[0]) \
    __CLR_Q(prio, Q[1], 1) \
    __CLR_Q(prio, Q[2], 2) \
    __CLR_Q(prio, Q[3], 3) \
    __CLR_Q(prio, Q[4], 4) \
    __CLR_Q(prio, Q[5], 5) \
    __CLR_Q(prio, Q[6], 6) \
    __CLR_QE(prio, Q[7], 7)

#define __SET(prio, Q) \
    __SET_QB(prio, Q[0]) \
    __SET_Q(prio, Q[1], 1) \
    __SET_Q(prio, Q[2], 2) \
    __SET_Q(prio, Q[3], 3) \
    __SET_Q(prio, Q[4], 4) \
    __SET_Q(prio, Q[5], 5) \
    __SET_Q(prio, Q[6], 6) \
    __SET_QE(prio, Q[7], 7)

#define __GET(next, Q) \
    __GET_QB(next, Q[0]) \
    __GET_Q(next, Q[1], 1) \
    __GET_Q(next, Q[2], 2) \
    __GET_Q(next, Q[3], 3) \
    __GET_Q(next, Q[4], 4) \
    __GET_Q(next, Q[5], 5) \
    __GET_Q(next, Q[6], 6) \
    __GET_QE(next, Q[7], 7)

#define __TAKE(next, Q) \
    __TAKE_QB(next, Q[0]) \
    __TAKE_Q(next, Q[1], 1) \
    __TAKE_Q(next, Q[2], 2) \
    __TAKE_Q(next, Q[3], 3) \
    __TAKE_Q(next, Q[4], 4) \
    __TAKE_Q(next, Q[5], 5) \
    __TAKE_Q(next, Q[6], 6) \
    __TAKE_QE(next, Q[7], 7)

#define __SAVEQ(Q) \
    uint __q[8]; \
    __q[0] = Q[0]; \
    __q[1] = Q[1]; \
    __q[2] = Q[2]; \
    __q[3] = Q[3]; \
    __q[4] = Q[4]; \
    __q[5] = Q[5]; \
    __q[6] = Q[6]; \
    __q[7] = Q[7]

#define __NQ(Q) \
    __q[0] != Q[0] || \
    __q[1] != Q[1] || \
    __q[2] != Q[2] || \
    __q[3] != Q[3] || \
    __q[4] != Q[4] || \
    __q[5] != Q[5] || \
    __q[6] != Q[6] || \
    __q[7] != Q[7]

#endif

/*=======================================================*/

/* Initialize queue */
#define __cque_init(q) \
do { \
    __CLR_ALL(q.que); \
} while (0)


/* Remove a priority from queue */
#define __cque_remove(q, prio) \
do { \
    __CLR(prio, q.que); \
} while (0)


/* Insert a priority into queue */
#define __cque_insert(q, prio) \
do { \
    __SET(prio, q.que); \
} while (0)


/* Retrieve the highest priority in queue. */
#define __cque_head(q, next) \
do { \
    __GET(next, q.que); \
} while (0)


/* Take the highest priority in queue. */
#define __cque_take(q, next) \
do { \
    __TAKE(next, q.que); \
} while (0)


/* Remove a priority from queue, optimization version. */
#define __cque_remove_optm(pq, prio) \
do { \
    RSI_REG cque_t __p_* rq = pq; \
    __CLR(prio, (*rq).que); \
} while (0)


/* Insert a priority into queue, optimization version. */
#define __cque_insert_optm(pq, prio) \
do { \
    RSI_REG cque_t __p_* rq = pq; \
    __SET(prio, (*rq).que); \
} while (0)


/* Retrieve the highest priority in queue, optimization version. */
#define __cque_head_optm(pq, next) \
do { \
    RSI_REG cque_t rq = *pq; \
    __GET(next, rq.que); \
} while (0)


/* Move the priority in queue. */
#define __cque_move(q, from, to) \
do { \
    __SAVEQ(q.que); \
    __cque_remove(q, from); \
    if (__NQ(q.que)) { \
        __cque_insert(q, to); \
    } \
} while (0)


/* Remove a priority from queue, for suspend a task. */
#define __cque_susremove(q, prio, state) \
do { \
    __SAVEQ(q.que); \
    __cque_remove(q, prio); \
    if (__NQ(q.que)) { \
        state |= RSI_STA_PREREADY; \
    } \
} while (0)


/* If queue is not empty. */
#define __cque_is1(q)  __IF1(q.que)


/* If queue is empty. */
#define __cque_is0(q)  (__IF1(q.que) == 0)


/*===========================================================================*/
#ifdef __cplusplus
} /* end of extern "C" */
#endif

#endif

