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

typedef struct Argu{
	struct timeval start;
	struct timeval end;
	double delta;
	int row;
	int col;
}Arguments;

Matrix MatrixMa , MatrixMb , MatrixAa , MatrixAb , MatrixMt, MatrixAt;

Arguments MultAll, MultRow, MultInd, AddAll, AddRow, AddInd;

sem_t lock;

int i , k , j;

void *MultiplyAll( void )
{
	int k , c , i;
	gettimeofday(&MultAll.start, NULL);
	sem_wait(&lock);

	for(k=0; k<MatrixMa.r; k++)
	{
		for(c=0; c<MatrixMb.c; c++)
		{
			for(i=0; i<MatrixMa.r; i++)
			{
				MatrixMt.array[k][c] = MatrixMt.array[k][c] + (MatrixMa.array[k][i] * MatrixMb.array[i][c]);
			}
		}
	}

	sem_post(&lock);
	gettimeofday(&MultAll.end, NULL);
	MultAll.delta=(double)( (MultAll.end.tv_sec*1000000 + MultAll.end.tv_usec) - (MultAll.start.tv_sec*1000000 + MultAll.start.tv_usec) );
	pthread_exit(0);
	return NULL;
}

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
	return NULL;
}


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
	return NULL;
}

void *AdditionAll( void )
{
	int k , c;
	gettimeofday(&AddAll.start, NULL);
	sem_wait(&lock);

	for(k=0; k<MatrixAt.r; k++)
	{
		for(c=0; c<MatrixAt.c; c++)
		{
			MatrixAt.array[k][c] = MatrixAa.array[k][c] + MatrixAb.array[k][c];
		}
	}

	sem_post(&lock);
	gettimeofday(&MultAll.end, NULL);
	AddAll.delta=(double)( (AddAll.end.tv_sec*1000000 + AddAll.end.tv_usec) - (AddAll.start.tv_sec*1000000 + AddAll.start.tv_usec) );
	pthread_exit(0);
	return NULL;
}

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
	return NULL;
}

void *AdditionInd( void )
{
	sem_wait(&lock);

	MatrixAt.array[AddInd.row][AddInd.col] = MatrixAa.array[AddInd.row][AddInd.col] + MatrixAb.array[AddInd.row][AddInd.col];

	sem_post(&lock);
	pthread_exit(0);
	return NULL;
}

void PrintIt( Matrix ToBePrinted )
{
	int k , c;
	for(k=0; k<ToBePrinted.r; k++)
	{
		for(c=0; c<ToBePrinted.c; c++)
		{
			printf("%d ", ToBePrinted.array[k][c]);
		}
		printf("/n");
	}
}

int main( int arc , char **argv )
{
	FILE *fpA;
	FILE *fpM;
	//int i , j , k;

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
	}

	//int i , j , k;

	fscanf( fpA , "%d %d\n" , &MatrixAa.r , &MatrixAb.c);
	MatrixAa.array = (int **)malloc(sizeof(int*) * MatrixAa.r);
	for(k=0; k<MatrixAa.r; k++)
	{
		MatrixAa.array[k] = (int*)malloc(sizeof(int) * MatrixAa.c);
	}
	for(i=0; i<MatrixAa.r; i++)
	{
		for(j=0; j<MatrixAa.c; j++)
		{
			fscanf( fpA , "%d" , &MatrixAa.array[i][j]);

		}
	}

	fscanf( fpA , "\n%d %d\n" , &MatrixAb.r , &MatrixAb.c);
	MatrixAb.array = (int **)malloc(sizeof(int*) * MatrixAb.r);
	for(k=0; k<MatrixAb.r; k++)
	{
		MatrixAb.array[k] = (int*)malloc(sizeof(int) * MatrixAb.c);
	}
	for(i=0; i<MatrixAb.r; i++)
	{
		for(j=0; j<MatrixAb.c; j++)
		{
			fscanf( fpA , "%d" , &MatrixAb.array[i][j]);
		}
	}

	if( MatrixAa.r != MatrixAb.r || MatrixAa.c != MatrixAb.c )
	{
		printf("\n\n\nERROR!!! Matrices are not of equal sides, can not add.\n\n\n");
		return 0;
	}



	fscanf( fpM , "\n%d %d\n" , &MatrixMa.r , &MatrixMa.c);
	MatrixMa.array = (int **)malloc(sizeof(int*) * MatrixMa.r);
	for(k=0; k<MatrixMa.r; k++)
	{
		MatrixMa.array[k] = (int*)malloc(sizeof(int) * MatrixMa.c);
	}

	for(i=0; i<MatrixMa.r; i++)
	{
		for(j=0; j<MatrixMa.c; j++)
		{
			fscanf( fpM , "%d\n" , &MatrixMa.array[i][j]);
		}
	}

	fscanf( fpM , "\n%d %d\n" , &MatrixMb.r , &MatrixMb.c);
	MatrixMb.array = (int **)malloc(sizeof(int*) * MatrixMb.r);
	for(k=0; k<MatrixAa.r; k++)
	{
		MatrixMb.array[k] = (int*)malloc(sizeof(int) * MatrixMb.c);
	}
	for(i=0; i<MatrixMb.r; i++)
	{
		for(j=0; j<MatrixMb.c; j++)
		{
			fscanf( fpM , "%d" , &MatrixMb.array[i][j]);
		}
	}

	if( MatrixMa.c != MatrixMb.r )
	{
		printf("\n\n\nERROR!!! Matrices are of incorrect size, can not multiply.\n\n\n");
		return 0;
	}

	sem_init(&lock, 0 , 1);

	printf("Multiply All:/n");
	pthread_create(&MultAll, NULL, MultiplyAll, NULL);
	PrintIt( MatrixMt );
	printf("Time for Multiply All: %f/n", MultAll.delta);

	printf("Multiply by Row:\n");
	MultRow.row = 1;
	gettimeofday(&MultRow.start, NULL);
	for(i=0; i<MatrixMt.r; i++)
	{
		pthread_create(&MultRow, NULL, MultiplyRow, NULL);
		MultRow.row = MultRow.row + 1;
	}
	gettimeofday(&MultRow.end, NULL);
	MultRow.delta = (double)( (MultRow.end.tv_sec*1000000 + MultRow.end.tv_usec) - (MultRow.start.tv_sec*1000000 + MultRow.start.tv_usec) );
	PrintIt( MatrixMt );
	printf("Time for Multiply by Row: %f/n", MultRow.delta);

	printf("Multiply Individually:\n");
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
	printf("Time for Multiply Individually: %f\n", MultInd.delta);



	printf("Addition All:/n");
	pthread_create(&AddAll, NULL, AdditionAll, NULL);
	PrintIt( MatrixAt );
	printf("Time for Addition All: %f\n", AddAll.delta);

	printf("Addition by Row:\n");
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
	printf("Time for Multiply by Row: %f/n", AddRow.delta);

	printf("Addition Individually:\n");
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
	printf("Time for Multiply Individually: %f\n", AddInd.delta);

	sem_destroy(&lock);
	return 0;
}
