#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#define NUM_OF_RESOURCES  3
#define NUM_OF_CUSTOMERS  3


int available[NUM_OF_RESOURCES] ; 						
int allocation[NUM_OF_CUSTOMERS][NUM_OF_RESOURCES] ;     
int max[NUM_OF_CUSTOMERS][NUM_OF_RESOURCES] ;
int need[NUM_OF_CUSTOMERS][NUM_OF_RESOURCES] ;

pthread_mutex_t available_lock;
pthread_cond_t cond_available;

int 
rand_in_range(int min, int max)
{
  return min + (int) (rand() / (double) (RAND_MAX) * (max - min + 1));
}

int
is_safe() 
{ 
	int done[NUM_OF_CUSTOMERS] = {0} ; 
	int resources[NUM_OF_RESOURCES] ; 

	int i ; 
	for (i = 0 ; i < NUM_OF_RESOURCES ; i++) { 
		resources[i] = available[i] ; 
	} 

	int finished = 0 ; 
	while (finished != NUM_OF_CUSTOMERS) { 

		for (i = 0 ; i < NUM_OF_CUSTOMERS ; i ++) { 
			int j, allowed = 1; 
			for (j = 0 ; j < NUM_OF_RESOURCES ; j++) { 
				if (need[i][j] > resources[j]) { 
					allowed = 0 ; 	
				}
			}
			if (allowed) { 
				done[i] = 1 ; 
				int j ; 
				for (j = 0 ; j < NUM_OF_RESOURCES ; j++) { 
					resources[j] += allocation[i][j] ; 
				}
			}
		}

		int cnt = 0 ; 
		for(i = 0 ; i < NUM_OF_CUSTOMERS ; i++) { 
			if (done[i]) { 
				cnt ++ ; 
			}	
		}

		if (cnt == finished) { 
			return 0 ; 
		}
		finished = cnt ; 
	}
	return 1; 
}

int 
validate_request(int customer_id, int request[]) 
{ 
	int allowed = 1 ; 

	int j ; 
	for (j = 0 ; j < NUM_OF_RESOURCES ; j++) { 
		if (request[j] > available[j] ) { 
			allowed = 0 ; 
		}
	}

	return allowed ;
}

int 
request_resources(int id, int request[])  
{

	pthread_mutex_lock(&available_lock) ; 
	while (validate_request(id, request) == 0) { 
		printf("client %d is waiting for resources to become available\n", id) ; 
		pthread_cond_wait(&cond_available, &available_lock) ; 
	} 

	int j ; 
	for(j = 0 ; j < NUM_OF_RESOURCES ; j++) { 
		available[j] -= request[j] ; 
		need[id][j] -= request[j] ; 
		allocation[id][j] += request[j] ; 
	}

	if (!is_safe()) { 
		available[j] += request[j] ; 
		need[id][j] += request[j] ; 
		allocation[id][j] -= request[j] ; 
		pthread_mutex_unlock(&available_lock) ; 
		return -1 ; 
	} 

	pthread_mutex_unlock(&available_lock) ; 
	return 0;
}

int 
release_resources(int id, int request[])
{
	pthread_mutex_lock(&available_lock) ; 
	int j ; 
	for(j = 0 ; j < NUM_OF_RESOURCES ; j++) { 
		available[j] += request[j] ; 
	}
	pthread_mutex_unlock(&available_lock) ; 
    pthread_cond_signal(&cond_available);
	return 1 ;
}

void 
print_1d(int len, int arr[])
{ 
	int i ; 
	for (i = 0 ; i < len ; i++ ) { 
		printf("%d ", arr[i]) ; 
	}
	printf("\n") ; 
}

void * 
handle_customer(void *arg) 
{

	int id = *(int *) arg ; 
	while( 1 ) {

		printf("client %d needs these resources: ", id) ;
		print_1d(NUM_OF_RESOURCES, need[id]) ; 

		int request[NUM_OF_RESOURCES] ; 

		int i ; 
		for (i = 0 ; i < NUM_OF_RESOURCES ; i++) { 
			request[i] = rand_in_range(0, need[id][i]) ; 
		}

		printf("client %d requested for these resources: ", id) ; 
		print_1d(NUM_OF_RESOURCES, request) ; 

		if (request_resources(id, request) < 0) { 
			printf("denied access to requested resources for client %d\n", id) ; 
		} else {
			printf("client %d request was accepted\n", id) ; 
		} 

		sleep(1) ; 

		release_resources(id, request) ; 
		printf("client %d released acquired resources\n", id) ; 
		
		int done = 1 ; 
		for (i = 0 ; i < NUM_OF_RESOURCES ; i++) { 
			if (need[id][i] != 0) { 
				done = 0 ; 
			}
		}

		if (done) { 
			break ; 
		}
	}

	return NULL ; 
}

int 
main(int argc, char *argv[]) 
{
	
	if (argc != NUM_OF_RESOURCES + 1) {
		printf("invalid number of arguments, expected: %d, got: %d", NUM_OF_RESOURCES, argc) ; 
		return -1 ; 
	}

	int i ; 
	for (i = 0 ; i < NUM_OF_RESOURCES ; i++) { 
		available[i] = atoi(argv[i]) ; 
	}	

	for(i = 0 ; i < NUM_OF_CUSTOMERS ; i++) { 
		int j ; 
		for (j = 0 ; j < NUM_OF_RESOURCES ; j++) { 
			need[i][j] = max[i][j] = rand_in_range(0, available[j]) ; 
			allocation[i][j] = 0 ; 
		}
	}

	pthread_t customers[NUM_OF_CUSTOMERS] ;
    pthread_mutex_init(&available_lock, NULL);
    pthread_cond_init(&cond_available, NULL);

	for (i = 0 ; i < NUM_OF_CUSTOMERS ; i ++ ) { 
		int *id = malloc(sizeof(int*)) ; 
		*id = i ; 
		pthread_create(&customers[i], NULL, &handle_customer, id); 
	}

	for (i = 0 ; i < NUM_OF_CUSTOMERS ; i ++ ) { 
		pthread_join(customers[i], NULL); 
	}

    pthread_mutex_destroy(&available_lock);
    pthread_cond_destroy(&cond_available);

	return 0 ; 
}


