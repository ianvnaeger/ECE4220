/*
 * 4220Lab2Part2Fixed.c
 *
 *  Created on: Feb 20, 2017
 *      Author: ivnqm2
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <rtai.h>
#include <rtai_lxrt.h>

char CommonBuffer[512];
int counter;
RTIME BaseP;
//Global Variables for ease of access

void *Thrd1( void )
{
	RT_TASK* rttask1 = rt_task_init(nam2num("thrd1"), 1, 512, 256);
	rt_task_make_periodic(rttask1, rt_get_time()+0* BaseP, 20* BaseP);
	//Sets the period of the thread
	FILE *fpf = fopen("first.txt", "r");
	//Opens file
	while( !feof(fpf) )
	{
		fgets(CommonBuffer, 512, fpf);
		rt_task_wait_period();
	}//reads in a line from the file and then waits a period
	pthread_exit(0);
}

void *Thrd2( void )
{
	RT_TASK* rttask2 = rt_task_init(nam2num("thrd2"), 1, 512, 256);
	rt_task_make_periodic(rttask2, rt_get_time()+10* BaseP, 20* BaseP);
	//Sets the period of the thread
	FILE *fps = fopen("second.txt", "r");
	//Opens file
	while( !feof(fps) )
	{
		fgets(CommonBuffer, 512, fps);
		rt_task_wait_period();
	}//reads in a line from the file and then waits a period
	pthread_exit(0);
}

void *Thrd3( void )
{

	RT_TASK* rttask3 = rt_task_init(nam2num("thrd3"), 1, 512, 256);
	rt_task_make_periodic(rttask3, rt_get_time()+5* BaseP, 10* BaseP);
	//Sets the period of the thread
	while( counter < 20 )
	{
		printf("%s\n", CommonBuffer);
		counter++;
		rt_task_wait_period();
	}//Prints a line and waits a period to print another
	pthread_exit(0);
}


int main( void )
{
	counter = 0;
	pthread_t thrd1, thrd2, thrd3;
	//Initialize Variables

	BaseP = start_rt_timer(nano2count(10000000));
	//Gets timer, used for period stuff

	pthread_create(&thrd1, NULL, Thrd1, NULL);
	pthread_create(&thrd2, NULL, Thrd2, NULL);
	pthread_create(&thrd3, NULL, Thrd3, NULL);
//Makes threads
	pthread_join(thrd1, NULL);
	pthread_join(thrd2, NULL);
	pthread_join(thrd3, NULL);
//Joins threads

	return 0;
}


