/*
 *	4220Lab1.c
 *	Created on: Jan 24, 2017
 *  Author: ivnqm2
 */
#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <sys/types.h>
#include <math.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main( void )
{
	int pushed = 0;
	int input, fd;
	unsigned long *BasePtr, *pbdr, *pbddr, *pfdr, *pfddr;
	unsigned long mask = 0x00; //Variables!!!

	fd = open("/dev/mem", O_RDWR |O_SYNC);
	if( fd == -1 )
	{
		printf("\n\n\nERROR!!!\n\n\n");
	} //Open File and Error check

	BasePtr = (unsigned long*)mmap(NULL,4096,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0x80840000);
	if(BasePtr == MAP_FAILED)
	{
		printf("\n\n\nSomething went wrong...\n\n\n");
		return 0;
	} //Get registers and error check

	pbdr = (BasePtr + 1);
	pbddr = (BasePtr + 5);
	pfdr = (BasePtr + 12);
	pfddr = (BasePtr + 13);

	*pfddr |= 0x02; //Get registers

	printf("Enter a button number(0 through 4): \n");
	scanf("%d", &input); // Get input

	if(input == 0)
	{
		*pbddr &= 0xFE;
		mask = 0x01;
	}
	else if(input == 1)
	{
		*pbddr &= 0xFD;
		mask = 0x02;
	}
	else if(input == 2)
	{

		*pbddr &= 0xFB;
		mask = 0x04;
	}
	else if(input == 3)
	{
		*pbddr &= 0xF7;
		mask = 0x08;
	}
	else if(input == 4)
	{
		*pbddr &= 0xEF;
		mask = 0x10;
	} // Gets the right button for input given

	unsigned long button=0;
	int first=0;

	while(pushed != 1)
	{
		if(first == 0)
		{
			printf("\nWaiting for button %d\n", input);
		}
		button = *pbdr;
		button &= mask;

		if(button == 0x00)
		{
			printf("\nButton %d has been pressed.\n", input);
			pushed = 1;
			//break;
		}
		++first;
	} // If user pushes button then we will be able to move along, much like the All American Rejects

	printf("Speaker will now sound\n");

	int i;

	for( i = 0; i <= 5; i++ )
	{
			*pfdr |= 0x02;
			printf("ON\n");
			sleep(1);

			*pfdr &= 0xFD;
			printf("OFF\n");
			sleep(1);
	} // This sounds the speaker
	//Speaker only makes a popping noise, but it does work!

	return 0;

}
