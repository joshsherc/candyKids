#include "stats.h"
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>


//Stats struct
typedef struct stats
{
	double minDelay;
	double avgDelay;
	double maxDelay;
	double totalDelay;
	int factoryNumber;
	int totalMade;
	int totalEaten;
} stats_t;

int factoryNumbers = 0;
sem_t mutex;
stats_t* statsArray;



void stats_record_produced(int factory_number)
{
	//Lock statsArray
	sem_wait(&mutex);
	statsArray[factory_number].totalMade += 1;
	//unLock statsArray
	sem_post(&mutex);
}

void stats_init(int num_producers)
{
	factoryNumbers = num_producers;
	sem_init(&mutex, SharedBetweenThreads, 1);
	statsArray = malloc(factoryNumbers*sizeof(stats_t));
	int i = 0;
	for(i = 0; i< factoryNumbers; i++) 
	{
		statsArray[i].minDelay = 0;
		statsArray[i].avgDelay = 0;
		statsArray[i].maxDelay = 0;
		statsArray[i].totalDelay = 0;
		statsArray[i].factoryNumber = i;
		statsArray[i].totalMade = 0;
		statsArray[i].totalEaten = 0;
	}
}
void stats_record_consumed(int factory_number, double delay_in_ms)
{
	//lock statsArray
	sem_wait(&mutex);
	statsArray[factory_number].totalEaten += 1;
	//recording first one
	if(statsArray[factory_number].totalEaten == 1) 
	{
		statsArray[factory_number].minDelay = delay_in_ms;
		statsArray[factory_number].avgDelay = delay_in_ms;
		statsArray[factory_number].maxDelay = delay_in_ms;
		statsArray[factory_number].totalDelay = delay_in_ms;
	}
	//beyond first
	else 
	{
		if(statsArray[factory_number].minDelay > delay_in_ms) 
		{
			statsArray[factory_number].minDelay = delay_in_ms;
		}
		if(statsArray[factory_number].maxDelay < delay_in_ms) 
		{
			statsArray[factory_number].maxDelay = delay_in_ms;
		}
		statsArray[factory_number].totalDelay += delay_in_ms;
		statsArray[factory_number].avgDelay = statsArray[factory_number].totalDelay/(double)statsArray[factory_number].totalEaten;
	}
	//unBlcok statsArray
	sem_post(&mutex);
}

void stats_display(void)
{
	printf("Statistics:\n");
	printf("%s%10s%10s%20s%20s%20s\n", "Factory #", "# Made", "# Eaten", "Min Delay [ms]", "Avg Delay [ms]", "Max Delay [ms]");
	for(int i=0; i<factoryNumbers; i++) 
	{
		printf("%9d%10d%10d%20.5f%20f%20.5f\n",
				statsArray[i].factoryNumber,
				statsArray[i].totalMade,
				statsArray[i].totalEaten,
				statsArray[i].minDelay,
				statsArray[i].avgDelay,
				statsArray[i].maxDelay);
		if(statsArray[i].totalMade != statsArray[i].totalEaten) {
			printf("\tERROR: Mismatch between number made and eaten.\n");
		}
	}
	printf("\n");
}

//Clean memory
void stats_cleanup(void)
{
	free(statsArray);
	statsArray = NULL;
}
