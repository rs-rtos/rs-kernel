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
 * ����APP_SAMPLE_01A��ʼ����������������,�ֱ��Բ�ͬƵ��
 * ����LED��˸.����ͨ��ʹ�û�������ȡ��LED����Ȩ,����LED
 * ά��һ��ʱ��Ŀ������,�ɹ����иó��򽫿���LEDָʾ��
 * �Բ�ͬƵ�ʽ�����˸һ��ʱ��.
 */


/* ����ջ��С */
#define APP_STACK_SIZE      1024 * 32


enum {
    /* ���������ȼ� */
    APP_PRIO_MU1    = 0,

    /* �������ȼ� */
    APP_PRIO_TA1    = 1,
    APP_PRIO_TA2    = 2,
};


/* LED���� */
#define LED_OFF()           printk("\r  - - ")
#define LED_ON()            printk("\r  @ @ ")

/* ����ջ */
stack_t app_stack_ta1[APP_STACK_SIZE];
stack_t app_stack_ta2[APP_STACK_SIZE];

/* �ź��� */
mutex_t led_mu1;


void app_task_1(arg_t arg);
void app_task_2(arg_t arg);


/* Ӳ����ʼ�� */
void hardware_initialize(void)
{

}


/* Ӧ�ó�ʼ�� */
void application_initialize(void)
{
    status_t status;

    /* ��ӡ����ʾ��һЩ��Ϣ */
    APP_SAMPLE_INFO;

    /* ��������TA1 */
    status = task_create(
        APP_PRIO_TA1,
        build_name('T', 'A', '1', '\0'),
        app_task_1, 0,
        app_stack_ta1, APP_STACK_SIZE,
        0);

    ASSERT(status == RS_EOK);

    /* ��������TA2 */
    status = task_create(
        APP_PRIO_TA2,
        build_name('T', 'A', '2', '\0'),
        app_task_2, 0,
        app_stack_ta2, APP_STACK_SIZE,
        0);
    
    ASSERT(status == RS_EOK);

    /* ��ʼ��������MU1 */
    status = mutex_create(
        &led_mu1,
        build_name('M', 'U', '1', '\0'),
        APP_PRIO_MU1);

    ASSERT(status == RS_EOK);
}

/* ����TA1��� */
void app_task_1(arg_t arg)
{
    int i;

    /* ��ֹ���������� */
    arg = arg;
    
    for (;;)
    {
        /**
         * ��ȡ������MU1�Ŀ���Ȩ,�绥������Ч,
         * ���������ȴ�״̬. */
        mutex_wait(&led_mu1, RS_WAIT_FOREVER);

        /* LED��1HzƵ����˸,ά��ʱ��5s */
        for (i = 0; i < 5; i++)
        {
            /* ����LED. */
            LED_ON();
            /* ��ʱ500ms. */
            task_sleep(RS_TICK_FREQ * 1 / 2);
            
            /* �ر�LED. */
            LED_OFF();
            /* ��ʱ500ms. */
            task_sleep(RS_TICK_FREQ * 1 / 2);
        }
        
        /* �ͷŻ�����MU1�Ŀ���Ȩ. */
        mutex_release(&led_mu1);

        /* ����1ʱ�ӵ�λ,
         * �⽫��������ȼ��������л�������. */
        task_sleep(1);
    }
}

/* ����TA2��� */
void app_task_2(arg_t arg)
{
    int i;
    
    /* ��ֹ���������� */
    arg = arg;

    for (;;)
    {
        /**
         * ��ȡ������MU1�Ŀ���Ȩ,�绥������Ч,
         * ���������ȴ�״̬. */
        mutex_wait(&led_mu1, RS_WAIT_FOREVER);

        /* LED��5HzƵ����˸,ά��ʱ��4s */
        for (i = 0; i < 25; i++)
        {
            /* ����LED. */
            LED_ON();
            /* ��ʱ100ms. */
            task_sleep(RS_TICK_FREQ * 1 / 10);
            
            /* �ر�LED. */
            LED_OFF();
            /* ��ʱ100ms. */
            task_sleep(RS_TICK_FREQ * 1 / 10);
        }
        
        /* �ͷŻ�����MU1�Ŀ���Ȩ. */
        mutex_release(&led_mu1);

        /* ����1ʱ�ӵ�λ,
         * �⽫��������ȼ��������л�������. */
        task_sleep(1);
    }
}


#endif
/*===========================================================================*/


