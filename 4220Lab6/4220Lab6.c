/*
 * 4220Lab6.c
 *
 *  Created on: Apr 11, 2017
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
#include <linux/unistd.h>
#include <linux/time.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <asm/io.h>
#include <rtai.h>
#include <rtai_sched.h>
#include <rtai_sem.h>
#include <rtai_fifos.h>

MODULE_LICENSE("GPL");

static RT_TASK mytask;
RTIME period;
unsigned long *BasePtr;
unsigned long *pbdr, *pbddr, *pfdr, *pfddr;
struct timeval time;
int check;
char note;
char send;
unsigned long *ptrGPIOBIntType1;
unsigned long *ptrGPIOBIntType2;
unsigned long *ptrGPIOBEOI;
unsigned long *ptrGPIOIntEn;
unsigned long *ptrRawIntStsB;
unsigned long *ptrGPIOBDB;
unsigned long *ptrVIC2IntEn;
unsigned long *ptrVIC2SoftInt;
unsigned long *ptrVIC2SoftIntClear;
//declare all of the global variables!!!

static void rt_process()
{

	//Speaker Operations
	while(1)
	{
		if( check == 0 )
		{
			check = 1;
		}
		else
		{
			check = 0;
		}

		if( check == 1 )
		{
			*pfdr |= 0x02;
			rt_sleep(period);
		}
		else
		{
			*pfdr &= 0xFD;
			rt_sleep(period);
		}
	}//turns the speaker on and off so it can make noise

}

static void HardHandler( unsigned irq_num, void *cookie )
{
	rt_disable_irq( irq_num );
	int btn = *ptrRawIntStsB & 0x1F;

	switch(btn)
	{
	case 0x01:
		send = 'A';
		period = start_rt_timer( nano2count(2200000) );
		break;
	case 0x02:
		send = 'B';
		period = start_rt_timer( nano2count(2469400) );
		break;
	case 0x04:
		send = 'C';
		period = start_rt_timer( nano2count(2616300) );
		break;
	case 0x08:
		send = 'D';
		period = start_rt_timer( nano2count(2936600) );
		break;
	case 0x10:
		send = 'E';
		period = start_rt_timer( nano2count(3296300) );
		break;
	}

	rtf_put(1, &send, sizeof( char ) );
	rt_task_init( &mytask, rt_process, 0 , 256 , 0 , 0 , 0 );
	rt_task_make_periodic( &mytask, rt_get_time(), period);
	*ptrGPIOBEOI |= 0x1F;

	rt_enable_irq( irq_num );
	//If someone pushes a button, it turns on the proper sound
}

static void SoftHandler( unsigned irq_num, void *cookie )
{
	rt_disable_irq( irq_num );
	rtf_get( 0, &note, sizeof(note) );
	switch(note)
	{
	case( 'A' ):
		period = start_rt_timer( nano2count(2200000) );
		break;
	case( 'B' ):
		period = start_rt_timer( nano2count(2469400) );
		break;
	case( 'C' ):
		period = start_rt_timer( nano2count(2616300) );
		break;
	case( 'D' ):
		period = start_rt_timer( nano2count(2936600) );
		break;
	case( 'E' ):
		period = start_rt_timer( nano2count(3296300) );
		break;
	}
	rt_task_init( &mytask, rt_process, 0 , 256 , 0 , 0 , 0 );
	rt_task_make_periodic( &mytask, rt_get_time(), period);
	*ptrVIC2SoftIntClear |= 0x080000000;

	rt_enable_irq( irq_num );
	//If it receives a note from the user space program, it turns on the proper sound
}

int init_module(void)
{
	printk("MODULE INSTALLED!!!\n");
	rtf_create(1, sizeof(char) );

	check = 0;
	unsigned long *ptr = (unsigned long *) __ioremap(0x80840000, 4096, 0);
	unsigned long *ptr2 = (unsigned long *) __ioremap(0x800C0000, 4096, 0);
	ptrVIC2IntEn = (unsigned long*)( (char*)ptr2 + 0x10);
	ptrVIC2SoftIntClear = (unsigned long*)( (char*)ptr2 + 0x1C);
	*ptrVIC2IntEn |= 0x01;
	pbddr = ptr+5;
	pbdr = ptr+1;
	pfdr = ptr+12;
	pfddr = ptr+13;
	*pfddr |= 0x02;
	ptrRawIntStsB = (unsigned long*)( (char*)ptr + 0xC0);
	ptrGPIOBEOI =(unsigned long*)( (char*) ptr + 0xB4);
	ptrGPIOBIntType1 = (unsigned long*)( (char*)ptr + 0xAC);
	ptrGPIOBIntType2 = (unsigned long*)( (char*)ptr + 0xB0);
	ptrGPIOIntEn = (unsigned long*)( (char*)ptr + 0xB8);
	ptrGPIOBDB = (unsigned long*) ((char*) ptr + 0xC4);
	*ptrGPIOBDB |= 0x1F;
	*ptrGPIOBIntType1 |= 0x1F;
	*ptrGPIOBIntType2 &= 0xE0;
	*ptrGPIOIntEn |= 0x1F;
	*ptrGPIOBEOI |= 0x1F;
	//Gets all of the interrupts and registers in order

	rt_set_periodic_mode();
	period = start_rt_timer(nano2count(1000000));
	note = period*100;
	rt_task_init(&mytask ,rt_process ,0 ,256 ,0 ,0 ,0);
	//sets up real time task with it's period

	rtf_create(0 , sizeof(unsigned long int));
	//opens up comms with the user space program

	rt_request_irq( 59, HardHandler, 0, 1);
	rt_enable_irq( 59 );
	rt_request_irq( 63, SoftHandler, 0, 1);
	rt_enable_irq( 63 );
	//enables interrupts

	return 0;
}

void cleanup_module( void )
{
	*pbdr &= 0x00;
	*pbddr &= 0x00;
	*ptrGPIOBIntType1 &= 0x00;
	*ptrGPIOBIntType2 &= 0x00;
	*ptrGPIOBEOI |= 0xFF;
	*ptrGPIOIntEn &= 0x00;
	*pfddr |= 0x02;
	*pfdr &= 0xFD;

	stop_rt_timer();
	rt_task_delete(&mytask);

	rtf_destroy(0);
	//Cleans up everything so that it is like how we found it
}


