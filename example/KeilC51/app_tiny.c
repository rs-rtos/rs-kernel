/*
===============================================================================
| COPYRIGHT (C) 2003-2007 Haishen Ruan, All rights reserved.
| SUMMARY: 
|   RS-RTOS demo implementation.
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
|   email:ruanhaishen@01s.org
=============================================================================*/ 
/*===========================================================================*/
#if 1
#include "inc/kapi.h"


/*
* ��������ʾ��51�ϼ�΢С������,��������֧��2������,�����ÿ���û�ж��ʵ������
* ����Ϊ����ʾRS-KERNEL����Ĳü�����,����������(example\keilC51\config.h):

* #define CFG_MAX_TASKS           2

* ����������ر�.
*/


/* ����ջ��С */
#define APP_STACK_SIZE      30

/* �������ȼ� */
#define APP_PRIO_TASK1      0
#define APP_PRIO_TASK2      1
 

/* ����ջ */
static byte idata app_stacks[2][APP_STACK_SIZE];


void app_task_1(arg_t arg);
void app_task_2(arg_t arg);

        
/* ��ʼ��Ӳ�� */
void hardware_initialize(void)
{
    /* ��ʼ��ʱ�� */
    TMOD = 0x01;
    ET0  = 1;
    TR0  = 1;
}


/* Ӧ�ó�ʼ��,��ʼ�������������� */
void application_initialize(void)
{
    /* ����Task1���� */
    task_create(
        APP_PRIO_TASK1,
        build_name('T', 'A', '1', '\0'),
        app_task_1, 0,
        app_stacks[0], APP_STACK_SIZE,
        0);

    /* ����Task2���� */
    task_create(
        APP_PRIO_TASK2,
        build_name('T', 'A', '2', '\0'),
        app_task_2, 0,
        app_stacks[1], APP_STACK_SIZE,
        0);
}


/* Task1������� */
void app_task_1(arg_t arg)
{
    arg = arg;

    for (;;)
    {
        /* Delay 1/2 second. */
        task_sleep(RS_TICK_FREQ * 1 / 2);
    }
}

/* Task2������� */
void app_task_2(arg_t arg)
{
    arg = arg;

    for (;;)
    {
        /* Delay 1 second. */
        task_sleep(RS_TICK_FREQ * 1);
    }
}

#endif
/*===========================================================================*/


