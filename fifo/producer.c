#include <stdio.h> 
#include <unistd.h> 
#include <stdlib.h> 

#include <sys/stat.h> 
#include <sys/types.h> 
#include <errno.h>
#include <fcntl.h>
#include <time.h>

#define FIFO_NAME "sum"
#define NUM		  5 

int 
main(int argc, char *argv[]) 
{


	int numbers[NUM] ; 
	srand(time(NULL)) ;

	int i ; 
	for (i = 0 ; i < NUM ; i ++) 
	{ 
		numbers[i] = rand() % 100 ; 
	}


	// creating fifo 
	// alternatively you can craete fifo 
	// using cli with mkfifo 
	if (mkfifo(FIFO_NAME, 0777) < 0) 
	{
		if (errno != EEXIST) 
		{
			printf("Couldn't create file") ;
			return 1 ;
		}
	}

	//opening file descriptor
	int fd ; 
	if ( (fd = open(FIFO_NAME, O_WRONLY)) < 0 )
	{
		printf("Failed to open pipe") ; 	
		return 2 ; 
	}

	if (write(fd, numbers, sizeof(int) * NUM) < 0) 
	{ 
		printf("Failed to write to pipe") ; 
		return 3 ;
	}

	close(fd) ; 

	//opening file for reading
	if ( (fd = open(FIFO_NAME, O_RDONLY)) < 0 ) 
	{ 
		printf("Failed to open pipe") ; 
		return 2 ; 
	}

	//reading sum from other process
	int sum ; 
	if ( read(fd, &sum, sizeof(int)) < 0 )
	{
		printf("Failed to read from pipe") ; 
		return 3 ;
	}

	printf("Sum is %d", sum) ;
	close(fd) ; 

	return 0 ;
}

