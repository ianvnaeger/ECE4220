/*
 * 4220Lab4Part1.c
 *
 *  Created on: Mar 7, 2017
 *      Author: ivnqm2
 */

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <rtai.h>
#include <rtai_lxrt.h>
#include "serial_ece4220.h"
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>

struct buffer
{
	unsigned char data;
	double time;
};

struct info
{
	double pressTime;
	double pressGPS;
	double startTime;
	double startGPS;
	double stopTime;
	double stopGPS;
};

struct buffer Common;
struct timeval clk;
double RB;
sem_t sem;


void child1( void* ptr )
{
	struct info *data;
	data = (struct info *)ptr;
	while(RB == data->startGPS)
	{
		usleep(1); //waits for the GPS to change
	}
	data->stopTime = (double)clk.tv_sec *100000 + (double)clk.tv_usec;
	data->stopGPS = RB;
	data->pressGPS = (double)data->startGPS/*(double)data->startTime*/ + ((double)(data->pressTime - data->startTime) * ((double)(data->stopGPS - data->startGPS)/(double)(data->stopTime - data->startTime)));

	sem_wait(&sem);
	printf("\nStart GPS: %lf\n", data->startGPS);
	printf("Start Time: %lf\n", data->startTime);
	printf("Button GPS: %lf\n", data->pressGPS);
	printf("Button Time: %lf\n", data->pressTime);
	printf("Stop GPS: %lf\n", data->stopGPS);
	printf("Stop Time %lf\n", data->stopTime);
	sem_post(&sem);
}

void child2( void* ptr )
{
	struct info *data;
	data = (struct info *)ptr;
	while(RB == data->startGPS)
	{
		usleep(1); //waits for the GPS to change
	}
	data->stopTime = (double)clk.tv_sec *100000 +(double)clk.tv_usec;
	data->stopGPS = RB;
	data->pressGPS = (double)data->startGPS/*(double)data->startTime*/ + ((double)(data->pressTime - data->startTime) * ((double)(data->stopGPS - data->startGPS)/(double)(data->stopTime - data->startTime)));

	sem_wait(&sem);
	printf("\nStart GPS: %lf\n", data->startGPS);
	printf("Start Time: %lf\n", data->startTime);
	printf("Button GPS: %lf\n", data->pressGPS);
	printf("Button Time: %lf\n", data->pressTime);
	printf("Stop GPS: %lf\n", data->stopGPS);
	printf("Stop Time %lf\n", data->stopTime);
	sem_post(&sem);
}

void child3( void* ptr )
{
	struct info *data;
	data = (struct info *)ptr;
	while(RB == data->startGPS)
	{
		usleep(1); //waits for the GPS to change
	}
	data->stopTime = (double)clk.tv_sec *100000 + (double)clk.tv_usec;
	data->stopGPS = RB;
	data->pressGPS = (double)data->startGPS/*(double)data->startTime*/ + ((double)(data->pressTime - data->startTime) * ((double)(data->stopGPS - data->startGPS)/(double)(data->stopTime - data->startTime)));

	sem_wait(&sem);
	printf("\nStart GPS: %lf\n", data->startGPS);
	printf("Start Time: %lf\n", data->startTime);
	printf("Button GPS: %lf\n", data->pressGPS);
	printf("Button Time: %lf\n", data->pressTime);
	printf("Stop GPS: %lf\n", data->stopGPS);
	printf("Stop Time %lf\n", data->stopTime);
	sem_post(&sem);
}

void child4( void* ptr )
{
	struct info *data;
	data = (struct info *)ptr;
	while(RB == data->startGPS)
	{
		usleep(1); //waits for the GPS to change
	}
	data->stopTime = (double)clk.tv_sec *100000 + (double)clk.tv_usec;
	data->stopGPS = RB;
	data->pressGPS = (double)data->startGPS/*(double)data->startTime*/ + ((double)(data->pressTime - data->startTime) * ((double)(data->stopGPS - data->startGPS)/(double)(data->stopTime - data->startTime)));

	sem_wait(&sem);
	printf("\nStart GPS: %lf\n", data->startGPS);
	printf("Start Time: %lf\n", data->startTime);
	printf("Button GPS: %lf\n", data->pressGPS);
	printf("Button Time: %lf\n", data->pressTime);
	printf("Stop GPS: %lf\n", data->stopGPS);
	printf("Stop Time %lf\n", data->stopTime);
	sem_post(&sem);
}

void *Read( void )
{
	struct timeval buff;
	double passTime;
	struct timeval moreTime;
	pthread_t thrd1, thrd2, thrd3, thrd4;
	struct timeval start;
	int fd_fifo_in = open("/dev/rtf/0", O_RDONLY);
	int count;
	int press = 0;
	struct info *pass1 = malloc(sizeof(struct info));
	struct info *pass2 = malloc(sizeof(struct info));
	struct info *pass3 = malloc(sizeof(struct info));
	struct info *pass4 = malloc(sizeof(struct info));

	while(1)
	{
		count = read(fd_fifo_in, &passTime, sizeof(buff) );
		if( press == 0 )
		{
			start = clk;
			/*unsigned long int*/double startTime = ((double)start.tv_sec*100000 + (double)start.tv_usec);
			pass1->startTime = startTime;
			pass1->startGPS = RB;
			gettimeofday(&moreTime, NULL);
			pass1->pressTime = ((double)moreTime.tv_sec*100000 + (double)moreTime.tv_usec);
			pthread_create(&thrd1, NULL, (void*)&child1, (void*)pass1);
			press++;
		}
		if( press == 1 )
		{
			start = clk;
			double startTime = ((double)start.tv_sec*100000 + (double)start.tv_usec);
			pass2->startTime = startTime;
			pass2->startGPS = RB;
			gettimeofday(&moreTime, NULL);
			pass2->pressTime = ((double)moreTime.tv_sec*100000 + (double)moreTime.tv_usec);
			//pass2->pressTime = passTime;
			pthread_create(&thrd2, NULL, (void*)&child2, (void*)pass2);
			press++;
		}
		if( press == 2 )
		{
			start = clk;
			double startTime = ((double)start.tv_sec*100000 + (double)start.tv_usec);
			pass3->startTime = startTime;
			pass3->startGPS = RB;
			gettimeofday(&moreTime, NULL);
			pass3->pressTime = ((double)moreTime.tv_sec*100000 + (double)moreTime.tv_usec);
			pthread_create(&thrd3, NULL, (void*)&child3, (void*)pass3);
			press++;
		}
		else
		{
			start = clk;
			double startTime = ((double)start.tv_sec*100000 + (double)start.tv_usec);
			pass4->startTime = startTime;
			pass4->startGPS = RB;
			gettimeofday(&moreTime, NULL);
			pass4->pressTime = ((double)moreTime.tv_sec*100000 + (double)moreTime.tv_usec);
			pthread_create(&thrd4, NULL, (void*)&child4, (void*)pass4);
			press = 0;
		}
	}
	close(fd_fifo_in);
	pthread_exit(0);
	return NULL;
}

int main( void )
{
	pthread_t thrd;
	int id;
	unsigned char c;
	ssize_t bytes;
	sem_init(&sem,0,1);

	id= serial_open( 0, 5 , 5 );
	pthread_create(&thrd, NULL, Read, NULL);
	usleep(10);

	while(1)
	{
		bytes = read( id, &c , 1 );
		gettimeofday(&clk, NULL);
		RB = (double)c;
		fflush(stdout);
	}

	pthread_join(thrd, NULL);
	sem_destory(&sem);

}
