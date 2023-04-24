#include <unistd.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <strings.h> 

/* 
 * In this program we are gonna have three processes. 
 * the main process send value x to process 1. 
 * process 1 adds 5 to it and then sends it to process 2. 
 * process 2 adds another 5 and sends x back to main. 
 * we need three pipes, since pipes are unidirectional. 
 * we will use fd[0] to send x from main to Process 1. 
 * we will use fd[1] to send x from Process 1 to Process 2. 
 * we will use fd[2] to send x from Process 2 back to main. 
 * There are gonna be 18 file descriptors, therefore 
 * we need to have 18 close calls. 
 */

int 
main(int arc, char *argv[]) 
{

	int fd[3][2] ; 
	
	int i ; 
	for (i = 0 ; i < 3 ; i++ ) 
	{
		if (pipe(fd[i]) < 0) 
		{ 
			printf("failed to create pipe") ;
			return 1 ; 
		}
	}

	// Process 1
	int pid1 = fork() ; 
	if ( pid1 < 0) 
	{
		printf("failed to fork") ; 
		return 2 ; 
	}

	if (pid1 == 0) 
	{
		//child process 
		close(fd[0][1]) ; 
		close(fd[1][0]) ; 
		close(fd[2][0]) ; 
		close(fd[2][1]) ; 

		// read x
		int x ; 
		if ( read(fd[0][0], &x, sizeof(int)) < 0 ) 
		{ 
			printf("Process 1 failed to read x") ; 
			return 3 ; 
		}

		x += 5 ; 

		if ( write(fd[1][1], &x, sizeof(int)) < 0 ) 
		{ 
			printf("Process 1 failed to write x to Process 2") ; 
			return 4 ; 
		}

		close(fd[0][0]) ; 
		close(fd[1][1]) ;
		return 0 ; 
	} 


	// Process 2 
	int pid2 = fork() ; 
	if ( pid2 < 0) 
	{
		printf("failed to fork") ; 
		return 5 ; 
	}

	if (pid2 == 0) 
	{
		//child process 
		close(fd[0][1]) ; 
		close(fd[2][0]) ; 
		close(fd[0][0]) ; 
		close(fd[1][1]) ;

		// read x
		int x ; 
		if ( read(fd[1][0], &x, sizeof(int)) < 0 ) 
		{ 
			printf("Process 2 failed to read x") ; 
			return 6 ; 
		}

		x += 5 ; 
		if ( write(fd[2][1], &x, sizeof(int)) < 0 ) 
		{ 
			printf("Process 2 failed to write x to main process") ; 
			return 7 ; 
		}

		close(fd[1][0]) ; 
		close(fd[2][1]) ; 
		return 0 ; 
	} 
	 
	
	close(fd[0][0]) ; 
	close(fd[1][1]) ;
	close(fd[1][0]) ; 
	close(fd[2][1]) ; 

	int x = 0 ;
	if ( write(fd[0][1], &x, sizeof(int)) < 0 ) 
	{
		printf("Parent failed to write x to Process 1") ; 
		return 8 ; 
	}

	if ( read(fd[2][0], &x, sizeof(int)) < 0 ) 
	{ 
		printf("Parent failed to read x from Process 2") ; 
		return 9 ; 
	}

	printf("Result if %d\n", x) ;

	close(fd[0][1]) ; 
	close(fd[2][0]) ; 

	return 0 ;
}

