#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h> 

#define OFFSET 12
#define BINS (2 * OFFSET) + 1 

pthread_mutex_t hist_mutex = PTHREAD_MUTEX_INITIALIZER;
int hist[BINS] = {0} ; 

void 
printHistogram(int hist[BINS]) 
{ 

	int i, j ; 
	for (i = 0 ; i < BINS ; i++) { 
		printf("%d\n", hist[i]); 
	}
}

int 
randInRange(int min, int max)
{
  return min + (int) (rand() / (double) (RAND_MAX) * (max - min + 1));
}

void *
handle_counter(void *arg) 
{

	int tmp[BINS] = {0} ; 
	int samples = *(int *) arg ; 

	int j ; 
	for (j = 0 ; j < samples ; j++) { 
		int counter  = OFFSET;  

		int i ; 
		for (i = 0 ; i < OFFSET ; i++) { 
			int r ;
			if( (r = randInRange(0, 1)) >= 0.5) {
				counter ++ ; 
			}  
			else { 
				counter -- ; 
			}
		}
		tmp[counter]++ ; 
	}

	pthread_mutex_lock(&hist_mutex) ; 
	int i; 
	for (i = 0 ; i < BINS ; i++ ) { 
		hist[i] += tmp[i] ; 
	}
	pthread_mutex_unlock(&hist_mutex) ; 

	return NULL; 
}

int 
main(int argc, char *argv[]) 
{ 

	int sampels, threads, work ; 

	srand(time(NULL)); 

	if (argc != 3) {
		printf(" invalid cli arguments\n Usage: ./parallel [SAMPLES] [THREADS]\n") ;
		return -1 ; 
	}

	sampels = atoi(argv[1]) ; 
	threads = atoi(argv[2]) ; 
	work = sampels / threads ; 

	pthread_t tid[threads];

	clock_t begin = clock();

	int j; 
	for (j = 0 ; j < threads ; j ++ ) { 
		pthread_create(&tid[j], NULL, &handle_counter, (void*)&work);		
	}
	for (j = 0 ; j < threads ; j ++ ) { 
		pthread_join(tid[j], NULL);  
	}

	clock_t end = clock();

	printHistogram(hist); 

	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("time: %f\n", time_spent); 

}
