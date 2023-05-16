#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>


#define OFFSET 12
#define BINS (2 * OFFSET) + 1 


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


int 
main(int argc, char *argv[]) { 

	int sampels ; 
	int hist[BINS] = {0} ; 

	srand(time(NULL)); 

	if (argc != 2) {
		printf(" invalid cli arguments\n Usage: ./normal [SAMPLES]\n") ;
		return -1 ; 
	}

	sampels = atoi(argv[1]) ; 

	clock_t begin = clock();


	int j ; 
	for (j = 0 ; j < sampels ; j ++ ) { 
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
		hist[counter] ++ ; 
	}

	clock_t end = clock();

	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("time: %f\n", time_spent); 
	printHistogram(hist); 
}
