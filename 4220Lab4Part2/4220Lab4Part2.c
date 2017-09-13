/*
 * 4220Lab4Part2.c
 *
 *  Created on: Mar 7, 2017
 *      Author: ivnqm2
 */

#ifndef MODULE
#define MODULE
#endif

#ifndef __KERNEL__
#define __KERNEL__
#endif

#include <rtai_lxrt.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/time.h>
#include <asm/io.h>
#include <rtai.h>
#include <rtai_sched.h>
#include <rtai_sem.h>
#include <rtai_fifos.h>

MODULE_LICENSE("GPL");

static RT_TASK mytask;
RTIME period;
unsigned long *BasePtr;
unsigned long *pbdr, *pbddr;
struct timeval time;

static void rt_process(int t)
{
	BasePtr = (unsigned long *)__ioremap(0x80840000, 4096, 0);
	pbdr = BasePtr + 1;
	pbddr = BasePtr + 5;
	*pbddr &= 0x0;
	int num = *pbdr;
	/*unsigned long int*/double pass;
	printk("PROCESS RUNNING!!!\n");
	while(1)
	{
		if( (num - *pbdr) == 1)
		{
			printk("BUTTON PRESSED!!!\n");
			do_gettimeofday(&time);
			rt_sleep(period*25);
			pass = ((double)time.tv_sec*100000 + (double)time.tv_usec);
			rtf_put( 0, &pass, sizeof(double));
			//rt_sleep(period*25);
		}

		rt_task_wait_period();
	}
}

int init_module(void)
{
	printk("MODULE INSTALLED!!!\n");
	rt_set_periodic_mode();
	period = start_rt_timer(nano2count(1000000));

	rt_task_init(&mytask ,rt_process ,0 ,256 ,0 ,0 ,0);
	rt_task_make_periodic(&mytask, rt_get_time()+10*period, 75*period);

	rtf_create(0 , sizeof(double));

	return 0;
}

void cleanup_module( void )
{
	rt_task_delete(&mytask);

	stop_rt_timer();

	rtf_destroy(0);
}
