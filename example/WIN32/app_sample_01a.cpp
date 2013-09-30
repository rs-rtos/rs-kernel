/*
===============================================================================
| COPYRIGHT (C) 2003-2007 Haishen Ruan, All rights reserved.
| SUMMARY: 
|   RS-RTOS Sample APP_SAMPLE_01A.
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
#include "app_sample.h"

#ifdef APP_SAMPLE_01A

/**
 * 例子APP_SAMPLE_01A初始化创建了两个任务,分别以不同频率
 * 控制LED闪烁.任务通过使用互斥量获取对LED控制权,并对LED
 * 维持一定时间的控制输出,成功运行该程序将看到LED指示灯
 * 以不同频率交替闪烁一段时间.
 */


/* 任务栈大小 */
#define APP_STACK_SIZE      1024 * 32


enum {
    /* 互斥量优先级 */
    APP_PRIO_MU1    = 0,

    /* 任务优先级 */
    APP_PRIO_TA1    = 1,
    APP_PRIO_TA2    = 2,
};


/* LED控制 */
#define LED_OFF()           printk("\r  - - ")
#define LED_ON()            printk("\r  @ @ ")

/* 任务栈 */
stack_t app_stack_ta1[APP_STACK_SIZE];
stack_t app_stack_ta2[APP_STACK_SIZE];

/* 信号量 */
mutex_t led_mu1;


void app_task_1(arg_t arg);
void app_task_2(arg_t arg);


/* 硬件初始化 */
void hardware_initialize(void)
{

}


/* 应用初始化 */
void application_initialize(void)
{
    status_t status;

    /* 打印关于示例一些信息 */
    APP_SAMPLE_INFO;

    /* 创建任务TA1 */
    status = task_create(
        APP_PRIO_TA1,
        build_name('T', 'A', '1', '\0'),
        app_task_1, 0,
        app_stack_ta1, APP_STACK_SIZE,
        0);

    ASSERT(status == RS_EOK);

    /* 创建任务TA2 */
    status = task_create(
        APP_PRIO_TA2,
        build_name('T', 'A', '2', '\0'),
        app_task_2, 0,
        app_stack_ta2, APP_STACK_SIZE,
        0);
    
    ASSERT(status == RS_EOK);

    /* 初始化互斥量MU1 */
    status = mutex_create(
        &led_mu1,
        build_name('M', 'U', '1', '\0'),
        APP_PRIO_MU1);

    ASSERT(status == RS_EOK);
}

/* 任务TA1入口 */
void app_task_1(arg_t arg)
{
    int i;

    /* 防止编译器警告 */
    arg = arg;
    
    for (;;)
    {
        /**
         * 获取互斥量MU1的控制权,如互斥量无效,
         * 将任务进入等待状态. */
        mutex_wait(&led_mu1, RS_WAIT_FOREVER);

        /* LED以1Hz频率闪烁,维持时间5s */
        for (i = 0; i < 5; i++)
        {
            /* 点亮LED. */
            LED_ON();
            /* 延时500ms. */
            task_sleep(RS_TICK_FREQ * 1 / 2);
            
            /* 关闭LED. */
            LED_OFF();
            /* 延时500ms. */
            task_sleep(RS_TICK_FREQ * 1 / 2);
        }
        
        /* 释放互斥量MU1的控制权. */
        mutex_release(&led_mu1);

        /* 休眠1时钟单位,
         * 这将允许低优先级的任务有机会运行. */
        task_sleep(1);
    }
}

/* 任务TA2入口 */
void app_task_2(arg_t arg)
{
    int i;
    
    /* 防止编译器警告 */
    arg = arg;

    for (;;)
    {
        /**
         * 获取互斥量MU1的控制权,如互斥量无效,
         * 将任务进入等待状态. */
        mutex_wait(&led_mu1, RS_WAIT_FOREVER);

        /* LED以5Hz频率闪烁,维持时间4s */
        for (i = 0; i < 25; i++)
        {
            /* 点亮LED. */
            LED_ON();
            /* 延时100ms. */
            task_sleep(RS_TICK_FREQ * 1 / 10);
            
            /* 关闭LED. */
            LED_OFF();
            /* 延时100ms. */
            task_sleep(RS_TICK_FREQ * 1 / 10);
        }
        
        /* 释放互斥量MU1的控制权. */
        mutex_release(&led_mu1);

        /* 休眠1时钟单位,
         * 这将允许低优先级的任务有机会运行. */
        task_sleep(1);
    }
}


#endif
/*===========================================================================*/


