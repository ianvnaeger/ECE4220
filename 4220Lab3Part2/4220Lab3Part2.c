/*
 * 4220Lab3Part2.c
 *
 *  Created on: Feb 28, 2017
 *      Author: ivnqm2
 */

#ifndef MODULE
#define MODULE
#endif

#ifndef __KERNEL__
#define __KERNEL__
#endif

#include <rtai.h>
#include <rtai_lxrt.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <asm/io.h>
#include <rtai.h>
#include <rtai_sched.h>
#include <rtai_sem.h>
#include "ece4220lab3.h"

MODULE_LICENSE("GPL");

SEM sem;
static RT_TASK mytask1 , mytask2 , mytask3;
RTIME period;
unsigned long *BasePtr;
unsigned long *pbdr, *pbddr;

static void rt_process1(int t)
{
	while(1)
	{
		rt_sem_wait( &sem );
		*pbdr |= 0x80;
		rt_sleep( period*10 );
		*pbdr &= 0x00;
		rt_sem_signal( &sem );
	}
}

static void rt_process2(int t)
{
	while(1)
	{
		rt_sem_wait( &sem );
		*pbdr |= 0x40;
		rt_sleep( period*10 );
		*pbdr &= 0x00;
		rt_sem_signal( &sem );
	}
}

static void rt_process3(int t)
{
	while(1)
	{
		rt_sem_wait( &sem );
		if(check_button() == 1 )
		{
			*pbdr |= 0x20;
			rt_sleep( period*10 );
			*pbdr &= 0x00;
			clear_button();
		}
		rt_sem_signal( &sem );
	}
}



int init_module(void)
{
	rt_set_periodic_mode();
	period = start_rt_timer(nano2count(100000000));

	rt_sem_init(&sem , 1);

	BasePtr = (unsigned long *)__ioremap(0x80840000, 4096, 0);
	pbdr = BasePtr + 1;

	pbddr = BasePtr + 5;
	*pbddr |= 0xE0;

	rt_task_init(&mytask1 ,rt_process1 ,0 ,256 ,1 ,0 ,0);
	rt_task_resume(&mytask1);
	rt_task_init(&mytask2 ,rt_process2 ,0 ,256 ,1 ,0 ,0);
	rt_task_resume(&mytask2);
	rt_task_init(&mytask3 ,rt_process3 ,0 ,256 ,0 ,0 ,0);
	rt_task_resume(&mytask3);

	return 0;
}

void cleanup_module( void )
{
	rt_task_delete(&mytask1);
	rt_task_delete(&mytask2);
	rt_task_delete(&mytask3);

	rt_sem_delete(&sem);

	stop_rt_timer();
}

