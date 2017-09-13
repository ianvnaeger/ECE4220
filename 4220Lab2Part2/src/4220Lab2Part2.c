/*
 * 4220Lab2Part2.c
 *
 *  Created on: Feb 14, 2017
 *      Author: ivnqm2
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <rtai.h>
#include <rtai_lxrt.h>

char CommonBuffer[512];
char saved[20][512];
int counter;
FILE *fpf;
FILE *fps;

void *Thrd1( FILE *fpf , RTIME BaseP )
{
	RT_TASK* rttask1 = rt_task_init(nam2num("thrd1"), 0, 512, 256);
	rt_task_make_periodic(rttask1, rt_get_time()* BaseP, 20* BaseP);
	while( feof(fpf) )
	{
		fgets(CommonBuffer, 512, fpf);
		//fscanf(fpf, "%s", CommonBuffer);
		rt_task_wait_period();
	}
	pthread_exit(0);
	return NULL;
}

void *Thrd2( FILE *fps , RTIME BaseP )
{
	RT_TASK* rttask2 = rt_task_init(nam2num("thrd2"), 0, 512, 256);
	rt_task_make_periodic(rttask2, rt_get_time()+20* BaseP, 20* BaseP);
	while( feof(fps) )
	{
		fgets(CommonBuffer, 512, fps);
		//fscanf(fps, "%s", CommonBuffer);
		rt_task_wait_period();
	}
	pthread_exit(0);
	return NULL;
}

void *Thrd3( RTIME BaseP)
{
	RT_TASK* rttask3 = rt_task_init(nam2num("thrd3"), 0, 512, 256);
	rt_task_make_periodic(rttask3, rt_get_time()+10* BaseP, 10* BaseP);
	while( counter < 20 )
	{
		*(saved[counter]) = CommonBuffer;
		printf("%s\n", (saved[counter]));
		counter++;
		rt_task_wait_period();
	}
	pthread_exit(0);
	return NULL;
}


int main( void )
{
	counter = 0;
	pthread_t Thrd1, Thrd2, Thrd3;

	fpf = fopen("first.txt", "r");
	fps = fopen("second.txt", "r");

	RTIME BaseP;

	BaseP = start_rt_timer(nano2count(1000000));

	pthread_create(&Thrd1, NULL, Thrd1, &BaseP);
	pthread_create(&Thrd2, NULL, Thrd2, &BaseP);
	pthread_create(&Thrd3, NULL, Thrd3, &BaseP);

	return 0;
}
