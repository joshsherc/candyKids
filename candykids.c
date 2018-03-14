#include "stats.h"
#include "bbuff.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>

//Candy Struct

typedef struct candy
{
	int factory_number;
	double time_stamp_in_ms;
} candy_t;

//Stop Thread flag, advicesd from course website
_Bool stop_thread = false;

//Taken directly from cmpt300 website
double current_time_in_ms(void)
{
	struct timespec now;
	clock_gettime(CLOCK_REALTIME, &now);
	return now.tv_sec * 1000.0 + now.tv_nsec/1000000.0;
}

//kid thread function
void* kid_thread(void* arg)
{	
	while (true)
	{
		int randomWaitTime = rand()%2;
		candy_t* candy = (candy_t*) bbuff_blocking_extract();
		stats_record_consumed(candy->factory_number, current_time_in_ms()-candy->time_stamp_in_ms);
		sleep(randomWaitTime);
		free(candy);
	}

	pthread_exit(NULL);
}

//factory thread function
void* factory_thread(void* arg)
{
	int i = (intptr_t)arg;
	while(stop_thread == false) 
	{
		int randWaitTime = rand()%4;
		printf("\tFactory %d ship candy & wait %ds\n", i, randWaitTime);
		//Create new candy items, code taken directly from CMPT 300  prj3 suggestions
		candy_t* candy = malloc(sizeof(candy_t));
		candy->factory_number = i;
		candy->time_stamp_in_ms = current_time_in_ms();
		bbuff_blocking_insert(candy);
		stats_record_produced(i);
		sleep(randWaitTime);
	}
	//Thread finished
	printf("Candy-factory %d done\n", i);
	pthread_exit(NULL);
}



int main(int argc, char* argv[])
{	
	int i = 0;
	int factories = 0;
	int kids = 0;
	int seconds = 0;
	int *candyArray[3];
	candyArray[0] = &factories;
	candyArray[1] = &kids;
	candyArray[2] = &seconds;
	//Check if arguments ar epositive
	for(i=1; i<4; i++)
	{
		sscanf(argv[i], "%d", candyArray[i-1]);
	}
	if(*candyArray[0] <1)
	{
		printf("Error: All arguments must be positive.\n");
		exit(0);
	}
		if(*candyArray[1] <1)
	{
		printf("Error: All arguments must be positive.\n");
		exit(0);
	}
		if(*candyArray[2] <1)
	{
		printf("Error: All arguments must be positive.\n");
		exit(0);
	}
	//initialize 
	bbuff_init();
	stats_init(factories);

	//create threads
	pthread_t* factorythreads = malloc(factories *(sizeof(pthread_t)));
	pthread_t* kidthreads = malloc(kids *(sizeof(pthread_t)));
	//execute threads
	pthread_t factoryID;
	pthread_t kidID;
	for(i=0; i<factories; i++) 
	{
		pthread_create(&factoryID, NULL, factory_thread, (void*)(intptr_t)i);
		factorythreads[i] = factoryID;
	}
	for(i=0; i<kids; i++) 
	{
		pthread_create(&kidID, NULL, kid_thread, (void*)(intptr_t)i);
		kidthreads[i] = kidID;
	}
	//print the time for the seconds
	for(i=0; i<seconds; i++) 
	{
		printf("Time: %ds \n", i);
		sleep(1);
	}
	stop_thread = true;
	
	printf("Stopping candy factories...\n");
	for(i=0; i<factories; i++) 
	{
		pthread_join(factorythreads[i], NULL);
	}
	//Wait for the buffer to empty
	while(!bbuff_is_empty()) 
	{
		sleep(1);
	}
	//stop the kid threads
	printf("Stopping kids.\n");
	for( i=0; i<kids; i++) 
	{
		pthread_cancel(kidthreads[i]);
		pthread_join(kidthreads[i], NULL);
	}
	//print the stats and clean memory
	stats_display();
	stats_cleanup();
	free(factorythreads);
	free(kidthreads);
	return 0;
}



