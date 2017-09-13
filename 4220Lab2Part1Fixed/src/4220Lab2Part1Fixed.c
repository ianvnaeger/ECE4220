/*
 ============================================================================
 Name        : 4220Lab2Part1.c
 Author      : Ian Naeger
 Version     :
 Copyright   : N/A
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <semaphore.h>
#include <pthread.h>

typedef struct mtrx{
	int r;
	int c;
	int** array;
}Matrix;
//Matrix Struct for easy access

typedef struct Argu{
	struct timeval start;
	struct timeval end;
	double delta;
	int row;
	int col;
}Arguments;
//Various Arguments that you would want for any of the functions

Matrix MatrixMa , MatrixMb , MatrixAa , MatrixAb , MatrixMt, MatrixAt;

Arguments MultAll, MultRow, MultInd, AddAll, AddRow, AddInd;
//Global Variables to make my life a little bit easier

sem_t lock;
//Used to block access of variables from other threads

int i , k , j;

void *MultiplyAll( void )
{
	int k , c; //, i;
	gettimeofday(&MultAll.start, NULL);
	sem_wait(&lock);
	//Gets the start time and locks variables

	for(k=0; k<MatrixMa.r; k++)
	{
		for(c=0; c<MatrixMb.c; c++)
		{
			for(i=0; i<MatrixMa.r; i++)
			{
				MatrixMt.array[k][c] = MatrixMt.array[k][c] + (MatrixMa.array[k][i] * MatrixMb.array[i][c]);
			}
		}
	}//Does Matrix math to multiply the matricies, only does the first half of the matrix for some reason...

	sem_post(&lock);
	gettimeofday(&MultAll.end, NULL);
	MultAll.delta=(double)( (MultAll.end.tv_sec*1000000 + MultAll.end.tv_usec) - (MultAll.start.tv_sec*1000000 + MultAll.start.tv_usec) );
	pthread_exit(0);
	//unlocks variables, gets the end time, calculates the time difference and exits the thread
}

/*
void *MultiplyRow( void )
{
	int c , i;
	sem_wait(&lock);

	for(c=0; c<MatrixMb.c; c++)
	{
		for(i=0; i<MatrixMa.r; i++)
		{
			MatrixMt.array[MultRow.row][c] = MatrixMt.array[MultRow.row][c] + (MatrixMa.array[MultRow.row][i] * MatrixMb.array[i][c]);
		}
	}

	sem_post(&lock);
	pthread_exit(0);
}

//Never got these 2 (Row and Ind) to work, but their basic idea was to breakdown the for loop used in the math calulations to be used for individual threads

void *MultiplyInd( void )
{

	int i;
	sem_wait(&lock);

	for(i=0; i<MatrixMa.r; i++)
	{
		MatrixMt.array[MultInd.row][MultInd.col] = MatrixMt.array[MultInd.row][MultInd.col] + (MatrixMa.array[MultInd.row][i] * MatrixMb.array[i][MultInd.col]);
	}

	sem_post(&lock);
	pthread_exit(0);
}*/

void *AdditionAll( void )
{
	int k , c;
	gettimeofday(&AddAll.start, NULL);
	sem_wait(&lock);
	//gets the start time and locks down variables

	for(k=0; k<MatrixAt.r; k++)
	{
		for(c=0; c<MatrixAt.c; c++)
		{
			MatrixAt.array[k][c] = MatrixAa.array[k][c] + MatrixAb.array[k][c];
		}
	}//Does Matrix math to add the 2 matricies

	sem_post(&lock);
	gettimeofday(&AddAll.end, NULL);
	AddAll.delta=(double)( (AddAll.end.tv_sec*1000000 + AddAll.end.tv_usec) - (AddAll.start.tv_sec*1000000 + AddAll.start.tv_usec) );
	pthread_exit(0);
	//unlocks variables, gets the end time, calculates the time difference and exits the thread
}

/*
void *AdditionRow( void )
{

	int c;
	sem_wait(&lock);

	for(c=0; c<MatrixAt.c; c++)
	{
		MatrixAt.array[AddRow.row][c] = MatrixAa.array[AddRow.row][c] + MatrixAb.array[AddRow.row][c];
	}

	sem_post(&lock);
	pthread_exit(0);
}
/*
//Never got these 2 (Row and Ind) to work, but their basic idea was to breakdown the for loop used in the math calulations to be used for individual threads

void *AdditionInd( void )
{
	sem_wait(&lock);

	MatrixAt.array[AddInd.row][AddInd.col] = MatrixAa.array[AddInd.row][AddInd.col] + MatrixAb.array[AddInd.row][AddInd.col];

	sem_post(&lock);
	pthread_exit(0);
}
*/

void PrintIt( Matrix ToBePrinted )
{
	int k , c;
	for(k=0; k<ToBePrinted.r; k++)
	{
		for(c=0; c<ToBePrinted.c; c++)
		{
			printf("%d ", ToBePrinted.array[k][c]);
		}
		printf("\n");
	}
}//Prints out the provided Matrix piece by piece

void FreeIt( Matrix ToBeFreed )
{
	int k;
	for(k=0; k<ToBeFreed.r; k++)
	{
		free( ToBeFreed.array[k] );
	}
	free( ToBeFreed.array );
}// Frees the space allocated for the Matrix

int main( int arc , char **argv )
{
	FILE *fpA;
	FILE *fpM;

	fpA = fopen( "addition.txt" , "r");
	if(fpA == NULL)
	{
		printf("\n\n\nERROR!!! addition.txt could not be opened. \n\n\n");
		return 0;
	}
	fpM = fopen( "multiplication.txt" , "r");
	if(fpM == NULL)
	{
		printf("\n\n\nERROR!!! multiplication.txt could not be opened.\n\n\n");
		return 0;
	}// Opens the files for the Matricies

	fscanf( fpA , "%d %d\n" , &MatrixAa.r , &MatrixAa.c);
	MatrixAa.array = (int **)malloc(sizeof(int*) * MatrixAa.r);
	for(k=0; k<MatrixAa.r; k++)
	{
		MatrixAa.array[k] = (int*)malloc(sizeof(int) * MatrixAa.c);
	}
	//Gets size of the Matrix and allocates space for it
	for(i=0; i<MatrixAa.r; i++)
	{
		for(j=0; j<MatrixAa.c; j++)
		{
			fscanf( fpA , "%d " , &MatrixAa.array[i][j]);

		}
	}//Populates the Matrix with data

	fscanf( fpA , "\n%d %d\n" , &MatrixAb.r , &MatrixAb.c);
	MatrixAb.array = (int **)malloc(sizeof(int*) * MatrixAb.r);
	for(k=0; k<MatrixAb.r; k++)
	{
		MatrixAb.array[k] = (int*)malloc(sizeof(int) * MatrixAb.c);
	}
	//Gets size of the Matrix and allocates space for it
	for(i=0; i<MatrixAb.r; i++)
	{
		for(j=0; j<MatrixAb.c; j++)
		{
			fscanf( fpA , "%d " , &MatrixAb.array[i][j]);
		}
	}//Populates the Matrix with data

	MatrixAt.r = MatrixAa.r;
	MatrixAt.c = MatrixAb.c;
	MatrixAt.array = (int **)malloc(sizeof(int*) * MatrixAt.r);
	for(k=0; k<MatrixAt.r; k++)
	{
		MatrixAt.array[k] = (int*)malloc(sizeof(int) * MatrixAt.c);
	}//Gets size of solution array and allocates space for it

	if( MatrixAa.r != MatrixAb.r || MatrixAa.c != MatrixAb.c )
	{
		printf("\n\n\nERROR!!! Matrices are not of equal sides, can not add.\n\n\n");
		return 0;
	}//error checks to see that matrix math is possible with provided matricies



	fscanf( fpM , "\n%d %d\n" , &MatrixMa.r , &MatrixMa.c);
	MatrixMa.array = (int **)malloc(sizeof(int*) * MatrixMa.r);
	for(k=0; k<MatrixMa.r; k++)
	{
		MatrixMa.array[k] = (int*)malloc(sizeof(int) * MatrixMa.c);
	}
	//Gets size of the Matrix and allocates space for it
	for(i=0; i<MatrixMa.r; i++)
	{
		for(j=0; j<MatrixMa.c; j++)
		{
			fscanf( fpM , "%d " , &MatrixMa.array[i][j]);
		}
	}//Populates the Matrix with data



	fscanf( fpM , "\n%d %d\n" , &MatrixMb.r , &MatrixMb.c);
	MatrixMb.array = (int **)malloc(sizeof(int*) * MatrixMb.r);
	for(k=0; k<MatrixMb.r; k++)
	{
		MatrixMb.array[k] = (int*)malloc(sizeof(int) * MatrixMb.c);
	}
	//Gets size of the Matrix and allocates space for it
	for(i=0; i<MatrixMb.r; i++)
	{
		for(j=0; j<MatrixMb.c; j++)
		{
			fscanf( fpM , "%d " , &MatrixMb.array[i][j]);
		}
	}//Populates the Matrix with data

	MatrixMt.r = MatrixMb.r;
	MatrixMt.c = MatrixMa.c;
	MatrixMt.array = (int **)malloc(sizeof(int*) * MatrixMt.r);
	for(k=0; k<MatrixMt.r; k++)
	{
		MatrixMt.array[k] = (int*)malloc(sizeof(int) * MatrixMt.c);
	}//Gets size of solution array and allocates space for it

	if( MatrixMa.c != MatrixMb.r )
	{
		printf("\n\n\nERROR!!! Matrices are of incorrect size, can not multiply.\n\n\n");
		return 0;
	}//Populates the Matrix with data

	sem_init(&lock, 0 , 1);
	//Initalizes the lock used for thread variable control

	printf("Multiply All:\n");
	pthread_create(&MultAll, NULL, MultiplyAll, NULL);
	PrintIt( MatrixMt );
	printf("Time for Multiply All: %f\n\n", MultAll.delta);
	//Does calculations in a new thread and then prints the resulting matrix and how long it took to do.

/*	printf("Multiply by Row:\n");
	MultRow.row = 1;
	gettimeofday(&MultRow.start, NULL);
	for(i=0; i<MatrixMt.r; i++)
	{
		pthread_create(&MultRow, NULL, MultiplyRow, NULL);
		MultRow.row = MultRow.row + 1;
	}
	gettimeofday(&MultRow.end, NULL);
	MultRow.delta = (double)( (MultRow.end.tv_sec*1000000 + MultRow.end.tv_usec) - (MultRow.start.tv_sec*1000000 + MultRow.start.tv_usec) );
	//PrintIt( MatrixMt );
	printf("%d \n", MatrixMt.array[1][1]);
	for(k=0; k<20; k++)
	{
		for(j=0; j<20; j++)
		{
			printf("%d ", MatrixMt.array[k][j]);
		}
		printf("/n");
	}
	printf("Time for Multiply by Row: %f\n\n", MultRow.delta);


//These don't work, but the theory behind them is that they take the for loop out of the function and use them to call new threads of smaller calculations


/*	printf("Multiply Individually:\n");
	MultInd.row = 1;
	MultInd.col = 1;
	gettimeofday(&MultInd.start, NULL);
	for(i=0; i<MatrixMt.r; i++)
	{
		for(j=0; j<MatrixMt.c; j++)
		{
			pthread_create(&MultiplyInd, NULL, MultiplyInd, NULL);
			MultInd.col = MultInd.col + 1;
		}
		MultInd.row = MultInd.row + 1;
	}
	gettimeofday(&MultInd.end, NULL);
	MultInd.delta = (double)( (MultInd.end.tv_sec*1000000 + MultInd.end.tv_usec) - (MultInd.start.tv_sec*1000000 + MultInd.start.tv_usec) );
	PrintIt( MatrixMt );
	printf("Time for Multiply Individually: %f\n\n", MultInd.delta);
*/


	printf("Addition All:\n");
	pthread_create(&AddAll, NULL, AdditionAll, NULL);
	PrintIt( MatrixAt );
	printf("Time for Addition All: %f\n\n", AddAll.delta);
	//Does calculations in a new thread and then prints the resulting matrix and how long it took to do.

/*	printf("Addition by Row:\n");
	AddRow.row = 1;
	gettimeofday(&AddRow.start, NULL);
	for(i=0; i<MatrixAa.r; i++)
	{
		pthread_create(&AddRow, NULL, AdditionRow, NULL);
		AddRow.row = AddRow.row + 1;
	}
	gettimeofday(&AddRow.end, NULL);
	AddRow.delta=(double)( (AddRow.end.tv_sec*1000000 + AddRow.end.tv_usec) - (AddRow.start.tv_sec*1000000 + AddRow.start.tv_usec) );
	PrintIt( MatrixAt );
	printf("Time for Multiply by Row: %f\n\n", AddRow.delta);


//These don't work, but the theory behind them is that they take the for loop out of the function and use them to call new threads of smaller calculations


	/*printf("Addition Individually:\n");
	AddInd.row = 1;
	AddInd.col = 1;
	gettimeofday(&AddInd.start, NULL);
	for(i=0; i<MatrixAt.r; i++)
	{
		for(j=0; j<MatrixAt.c; j++)
		{
			pthread_create(&AddInd, NULL, AdditionInd, NULL);
			AddInd.col = AddInd.col + 1;
		}
		AddInd.row = AddInd.row + 1;
	}
	gettimeofday(&AddInd.end, NULL);
	AddInd.delta = (double)( (AddInd.end.tv_sec*1000000 + AddInd.end.tv_usec) - (AddInd.start.tv_sec*1000000 + AddInd.start.tv_usec) );
	PrintIt( MatrixAt );
	printf("Time for Multiply Individually: %f\n\n", AddInd.delta);
*/
	FreeIt( MatrixAa );
	FreeIt( MatrixAb );
	FreeIt( MatrixAt );
	FreeIt( MatrixMa );
	FreeIt( MatrixMb );
	FreeIt( MatrixMt );
	sem_destroy(&lock);
	return 0;
	//Frees all the Matricies and gets rid of the lock
}
