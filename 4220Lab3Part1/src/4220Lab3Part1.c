/*
 * 4220Lab3Part1.c
 *
 *  Created on: Feb 21, 2017
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

static RT_TASK mytask;
RTIME period;

static void rt_process(int t)
{
	unsigned long *BasePtr;
	unsigned long *pbdr, *pbddr;

	BasePtr = (unsigned long *)__ioremap(0x80840000, 4096, 0);
	pbdr = BasePtr + 1;#include "ece4220lab3.h"

	pbddr = BasePtr + 5;
	*pbddr |= 0xE0;
	while(1)
	{
		*pbdr |= 0x80;
		rt_task_wait_period();
		//rt_sleep( 0.5 );
		*pbdr |= 0x40;
		rt_task_wait_period();
		//rt_sleep( 0.5 );
		/*if(check_button())
		{
			*pbdr |= 0x20;
			rt_task_wait_period();
			//rt_sleep( 0.5 );
			clear_button();
		}*/
	}
}

int init_module(void)
{
	period = start_rt_timer(nano2count(1000000));

	rt_task_init(&mytask ,rt_process ,0 ,256 ,0 ,0 ,0);
	rt_task_resume(&mytask);
	return 0;
}

void cleanup_module( void )
{
	rt_task_delete(&mytask);
	stop_rt_timer();
}
