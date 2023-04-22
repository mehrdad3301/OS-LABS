#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#define NUM_THREAD 6

pthread_mutex_t mutex_fuel;
pthread_cond_t cond_fuel;
int fuel = 0;


void* 
fuel_filling(void* arg) 
{
    for (int i = 0; i < 5; i++) {
        pthread_mutex_lock(&mutex_fuel);
        fuel += 30;
        printf("Filled fuel... %d\n", fuel);
        pthread_mutex_unlock(&mutex_fuel);
        pthread_cond_signal(&cond_fuel);
		//pthread_cond_broadcast(&cond_fuel); 
        sleep(1);
    }
	return NULL; 
}

void* 
car(void* arg) 
{
    pthread_mutex_lock(&mutex_fuel);
    while (fuel < 40) {
        printf("No fuel. Waiting...\n");
        pthread_cond_wait(&cond_fuel, &mutex_fuel);
    }
    fuel -= 40;
    printf("Got fuel. Now left: %d\n", fuel);
    pthread_mutex_unlock(&mutex_fuel);

	return NULL;
}


int 
main(int argc, char *argv[]) 
{

	pthread_t th[NUM_THREAD];
    pthread_mutex_init(&mutex_fuel, NULL);
    pthread_cond_init(&cond_fuel, NULL);
    for (int i = 0; i < NUM_THREAD; i++) {
        if (i % (NUM_THREAD / 2) == 0) {
            if (pthread_create(&th[i], NULL, &fuel_filling, NULL) != 0) {
                perror("Failed to create thread");
            }
        } else {
            if (pthread_create(&th[i], NULL, &car, NULL) != 0) {
                perror("Failed to create thread");
            }
        }
    }

    for (int i = 0; i < NUM_THREAD; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }
    pthread_mutex_destroy(&mutex_fuel);
    pthread_cond_destroy(&cond_fuel);



	return 0; 
}

