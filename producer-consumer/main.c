#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <unistd.h> 
#include <string.h>

#define BUFF_SIZE 10
	
int idx = 0; 
int buffer[BUFF_SIZE]; 
pthread_mutex_t bufferMutex; 

sem_t semEmpty ; 
sem_t semFull ;

void *produce(void *arg)
{

	int r; 
	while (1) 
	{ 
		r = rand() % 10 ; 

		sem_wait(&semEmpty) ; 
		pthread_mutex_lock(&bufferMutex);
		buffer[idx] = r ; 
		idx ++ ; 
		printf("producer %d\n", r) ;
		pthread_mutex_unlock(&bufferMutex);
		sem_post(&semFull) ; 
	}
	return NULL ; 
}
void *consume(void *arg)
{   

	while (1) 
	{ 
		sem_wait(&semFull) ; 
		pthread_mutex_lock(&bufferMutex);
		idx -- ;
		printf("consumer %d\n", buffer[idx]) ; 
		pthread_mutex_unlock(&bufferMutex);
		sem_post(&semEmpty) ; 
	}

	return NULL ; 
}

int main(int argc, char *argv[])
{   

	if (argc != 3) 
	{ 
		printf("Invalid arguments, Usage: ./a.out [PRODUCERS] [CONSUMERS]") ; 
		return -1 ; 
	}

	srand(time(NULL));
	
	int num_producers, num_consumers ;
	num_producers = atoi(argv[1]) ; 
	num_consumers = atoi(argv[2]) ; 
	
    pthread_t consumers[num_consumers], producers[num_producers];
	sem_init(&semEmpty, 0, 10) ; 
	sem_init(&semFull, 0, 0) ; 

	int i; 
	for (i = 0 ; i < num_producers ; i ++ ) 
	{ 
        pthread_create(&producers[i], NULL, &produce, NULL);
	}

	for (i = 0 ; i < num_consumers ; i ++ ) 
	{ 
        pthread_create(&consumers[i], NULL, &consume, NULL);
	}

	for (i = 0 ; i < num_consumers ; i ++ ) 
	{ 
        pthread_join(consumers[i], NULL);
	}

	for (i = 0 ; i < num_producers ; i ++ ) 
	{ 
        pthread_join(producers[i], NULL);
	}


    pthread_mutex_destroy(&bufferMutex);
	sem_destroy(&semEmpty) ; 
	sem_destroy(&semFull) ; 

    return 0;
    
}
