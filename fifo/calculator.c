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

	//opening file descriptor
	int fd ; 
	if ( (fd = open(FIFO_NAME, O_RDONLY)) < 0 ) 
	{		
		printf("Failed to open pipe") ;
		return 1 ; 
	}

	if (read(fd, numbers, sizeof(int) * NUM) < 0)	
	{ 
		printf("Failed to read from pipe") ; 
		return 2 ; 
	}

	close(fd) ;
	
	if ( (fd = open(FIFO_NAME, O_WRONLY)) < 0) 
	{ 
		printf("Failed to open pipe") ; 
		return 1 ;
	}

	//calculating sum 
	
	int sum, i ; 
    for (i = 0 ; i < NUM ; i++ ) 
	{
		sum += numbers[i] ; 
	}	

	if (write(fd, &sum, sizeof(int)) < 0) 
	{ 
		printf("Failed to write to pipe") ;
		return 3 ; 
	}

	close(fd) ; 
	return 0 ;
}


