#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>

#define NUM_PHILSOPH 5 

sem_t room;
sem_t chopstick[NUM_PHILSOPH];

void *philosopher(void *);

void eat(int);

int main()
{
	int i,a[NUM_PHILSOPH];
	pthread_t tid[NUM_PHILSOPH];
	
	sem_init(&room,0,NUM_PHILSOPH - 1);
	
	for(i = 0 ; i < NUM_PHILSOPH ; i++)
	{
		sem_init(&chopstick[i], 0, 1);
	}
		
	for(i = 0 ; i < NUM_PHILSOPH ; i++)
	{
		a[i]=i;
		pthread_create(&tid[i], NULL, philosopher, (void *)&a[i]);
	}

	for(i = 0 ; i < NUM_PHILSOPH ; i++)
	{
		pthread_join(tid[i], NULL);
	}
}

void * philosopher(void * arg)
{
	int phil = *(int *) arg;

	sem_wait(&room);
	printf("Philosopher %d has entered room\n", phil);
	sem_wait(&chopstick[phil]);
	sem_wait(&chopstick[(phil + 1) % NUM_PHILSOPH]);

	eat(phil);
	sleep(2);
	printf("Philosopher %d has finished eating\n", phil);

	sem_post(&chopstick[(phil + 1) % NUM_PHILSOPH]);
	sem_post(&chopstick[phil]);
	sem_post(&room);
	
	return NULL ; 
}

void eat(int phil)
{
	printf("Philosopher %d is eating\n", phil);
}
