#include "bbuff.h"
#include <time.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <semaphore.h>
#include <string.h>



void* boundedBuffer[BUFFER_SIZE];
sem_t bufferMutex;
sem_t bufferIsEmpty;
sem_t bufferIsFull;
int remainingInBuffer = 0;


void bbuff_init(void)
{
	remainingInBuffer= BUFFER_SIZE;	
	int successfulSem;
	successfulSem = sem_init(&bufferIsFull, SharedBetweenThreads, 0);
	if (successfulSem != 0)
	{
		printf("ERROR MAKING SEMAPHORE\n");
		exit(0);
	}
	successfulSem = sem_init(&bufferIsEmpty, SharedBetweenThreads, BUFFER_SIZE);
	if (successfulSem != 0)
	{
		printf("ERROR MAKING SEMAPHORE\n");
		exit(0);
	}
	successfulSem = sem_init(&bufferMutex, SharedBetweenThreads, 1);
	if (successfulSem != 0)
	{
		printf("ERROR MAKING SEMAPHORE\n");
		exit(0);
	}
}

void bbuff_blocking_insert(void* item)
{	
	//Block buffer
	sem_wait(&bufferIsEmpty);
	sem_wait(&bufferMutex);
	//CRITICAL SECTIO
	boundedBuffer[BUFFER_SIZE - remainingInBuffer] = item;
	remainingInBuffer--;
	//Unblock the buffer
	sem_post(&bufferMutex);
	sem_post(&bufferIsFull);
}

void* bbuff_blocking_extract(void)
{

	//Block Buffer
	sem_wait(&bufferIsFull);
	sem_wait(&bufferMutex);
	//CRITICAL SECTION
	void* item = boundedBuffer[BUFFER_SIZE - (remainingInBuffer+1)];
	boundedBuffer[BUFFER_SIZE - (remainingInBuffer+1)] = NULL;
	remainingInBuffer++;
	//Unblock Buffer
	sem_post(&bufferMutex);
	sem_post(&bufferIsEmpty);

	return item;
}

_Bool bbuff_is_empty(void)
{
	if(remainingInBuffer == BUFFER_SIZE)
	{
		return true;
	}
	else
	{
		return false;
	}
}
