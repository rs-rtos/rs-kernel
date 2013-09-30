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
|   email:ruanhaishen@gmail.com
=============================================================================*/ 
/*===========================================================================*/
#include "inc/kapi.h"

/*
* 该例子初始化创建了两个任务,分别以不同频率控制LED闪烁.任务通过使用信号量获取对
* LED控制权,每个任务对LED进行4秒的控制输出,成功运行该程序将看到LED指示灯以不同
* 频率交替闪烁一段时间.
*/


/* 任务栈大小 */
#define APP_STACK_SIZE      200

/* 任务优先级 */
#define APP_PRIO_TASK1      0
#define APP_PRIO_TASK2      1


#define LED_OFF()           PDR2_P27 = 1
#define LED_ON()            PDR2_P27 = 0


/* 任务栈 */
unsigned char app_stacks[2][APP_STACK_SIZE];

/* LED控制信号量 */
sem_t sem_led;

void app_task_1(arg_t arg);
void app_task_2(arg_t arg);

        
/* 初始化硬件 */
void hardware_initialize(void)
{
    /* Sets the ROM mirroring function. */
    ROMM_MI = 1;


    /* I/O function initializing. */
    RDR0 = 0;
    PDR0 = 0x20;
    DDR0 = 0x20;

    RDR1 = 0xff;
    PDR1 = 0xeb;
    DDR1 = 0x94;

    PDR2 = 0xbd;
    DDR2 = 0xca;

    PDR3 = 0xff;
    DDR3 = 0xbf;

    PDR4 = 0xd7;
    DDR4 = 0x7f;

    PDR5 = 0xff;
    DDR5 = 0x80;

    PDR6 = 0xfb;
    DDR6 = 0x06;


    /* Interrupt control initializing. */
    ICR00 = 7;      /*  IRQ11 IRQ12 */
    ICR01 = 7;      /*  IRQ13 IRQ14 */
    ICR02 = 7;      /*  IRQ15 IRQ16 */
    ICR03 = 7;      /*  IRQ17 IRQ18 */
    ICR04 = 7;      /*  IRQ19 IRQ20 */
    ICR05 = 7;      /*  IRQ21 IRQ22 */
    ICR06 = 7;      /*  IRQ23 IRQ24 */
    ICR07 = 7;      /*  IRQ25 IRQ26 */
    ICR08 = 7;      /*  IRQ27 IRQ28 */
    ICR09 = 7;      /*  IRQ29 IRQ30 */
    ICR10 = 7;      /*  IRQ31 IRQ32 */
    ICR11 = 7;      /*  IRQ33 IRQ34 */
    ICR12 = 7;      /*  IRQ35 IRQ36 */
    ICR13 = 7;      /*  IRQ37 IRQ38 */
    ICR14 = 7;      /*  IRQ39 IRQ40 */
    ICR15 = 7;      /*  IRQ41 IRQ42 */


    /* Set the timebase timer for system tick. */
    TBTC  = 0x92;
    ICR12 = 6;


    /* Sets the ILM in the PS to level 7. */
    __set_il(7);
}


/* 应用初始化,初始化创建两个任务,分别以不同频率控制LED闪烁. */
void application_initialize(void)
{
    /* 创建Task1任务 */
    task_create(
        APP_PRIO_TASK1,
        build_name('T', 'A', '1', '\0'),
        app_task_1, 0,
        app_stacks[0], APP_STACK_SIZE,
        0);

    /* 创建Task2任务 */
    task_create(
        APP_PRIO_TASK2,
        build_name('T', 'A', '2', '\0'),
        app_task_2, 0,
        app_stacks[1], APP_STACK_SIZE,
        0);

    /* 初始化信号量 */
    semaphore_create(
        &sem_led,
        build_name('S', 'E', 'M', '\0'),
        1);
}


/* Task1任务入口 */
void app_task_1(arg_t arg)
{
    int i;

    arg = arg;

    for (;;)
    {
        semaphore_wait(&sem_led, RS_WAIT_FOREVER);

        for (i = 0; i < 4; i++)
        {
            /* Turn led on. */
            LED_ON();
            /* Delay 1/2 second. */
            task_sleep(RS_TICK_FREQ * 1 / 2);
            
            /* Turn led off. */
            LED_OFF();
            /* Delay 1/2 second. */
            task_sleep(RS_TICK_FREQ * 1 / 2);
        }
        
        semaphore_post(&sem_led);
    }
}

/* Task2任务入口 */
void app_task_2(arg_t arg)
{
    int i;
    
    arg = arg;

    for (;;)
    {
        semaphore_wait(&sem_led, RS_WAIT_FOREVER);

        for (i = 0; i < 20; i++)
        {
            /* Turn led on. */
            LED_ON();
            /* Delay 1/10 second. */
            task_sleep(RS_TICK_FREQ * 1 / 10);
            
            /* Turn led off. */
            LED_OFF();
            /* Delay 1/10 second. */
            task_sleep(RS_TICK_FREQ * 1 / 10);
        }
        
        semaphore_post(&sem_led);
    }
}



/*===========================================================================*/


