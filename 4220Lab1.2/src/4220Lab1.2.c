/*
 ============================================================================
 Name        : 2.c
 Author      : Ian Naeger
 Version     :
 Copyright   : N/A
 Description : Hello World in C, Ansi-style
 ============================================================================
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
/*#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>*/

MODULE_LICENSE("GPL");

int init_module( void )
{
	//int fd;
	unsigned long *BasePtr;
	unsigned long *pbdr, *pbddr;

	printk("MODULE INSTALLED!!!\n");
	//fd = open("/dev/mem", O_RDWR|O_SYNC);

	BasePtr = (unsigned long *)__ioremap(0x80840000, 4096, 0);
	pbdr = BasePtr + 1;
	pbddr = BasePtr + 5;

	*pbddr |= 0xE0;

	*pbdr |= 0x60;

	return 0;
}

void cleanup_module( void )
{
	//int fd;
	unsigned long *BasePtr;
	unsigned long *pbdr, *pbddr;

	printk("MODULE REMOVED!!!\n");
	//fd = open("/dev/mem", O_RDWR|O_SYNC);

	BasePtr = (unsigned long *)__ioremap(0x80840000, 4096, 0);
	pbdr = BasePtr + 1;
	pbddr = BasePtr + 5;

	*pbddr |= 0xE0;

	*pbdr &= ~0x60;

}

