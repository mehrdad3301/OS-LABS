
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h>

#define MAX_PROC 10


struct proc { 

	int pid ; 
	int bt ;  //burst time
	int wt ;  //waiting time
	int tt ;  //turnaround time
	int at ;  //arrival time

} ; 

struct proc procs[MAX_PROC] ; 

void print_stats(int nprocs) { 

	int tt, wt; 
	tt = wt = 0 ; 
	 
	int i ; 
	for (i = 0 ; i < nprocs ; i++) { 
		printf("_______\n") ; 
		printf("Process: %d\nArrival: %d\nBurst: %d\nTurnaround: %d\nWaiting: %d\n",
		procs[i].pid, procs[i].at, procs[i].bt, procs[i].tt, procs[i].wt) ;
		tt += procs[i].tt ; 
		wt += procs[i].wt ; 
		printf("\n") ;
	}

	printf("Average Waiting: %d\nAverage Turnaround: %d", tt / nprocs, wt / nprocs) ; 
}

void 
sort_by_at(int nprocs) {

    int i;
    for (i = 1; i < nprocs; i++) {
		struct proc key = procs[i] ; 
 
		int j ; 
        for (j = i - 1; j >= 0 && procs[j].at > key.at ; j--) {
            procs[j + 1] = procs[j] ;
        }
        procs[j + 1] = key;
    }
}

void 
fcfs_scheduler(int nprocs) 
{ 
	sort_by_at(nprocs) ; 

	int elapsedtime = procs[0].at ; 

	int i ; 
	for (i = 0 ; i < nprocs ; i++) { 
		procs[i].wt = elapsedtime - procs[i].at ; 
		procs[i].tt = procs[i].wt + procs[i].bt ; 
		elapsedtime += procs[i].bt ; 
	}
}

int   
main (int argc, char *argv[]) 
{ 

	if (argc != 2) { 
		printf("invalid number of arguments, expected: 1 got: %d", argc) ; 
		return 1 ; 
	}

	int nprocs = atoi(argv[1]) ; 

	int i ; 
	for(i = 0 ; i < nprocs ; i++) { 
		int at ;
		printf("arrival time: ") ;
		scanf("%d", &at) ; 

		int bt ; 
		printf("burst time: ") ; 
		scanf("%d", &bt) ; 

		procs[i] = (struct proc) { 
			.pid = i, 
			.bt = bt, 
			.at = at,
		} ; 
		printf("\n") ; 
	}

	fcfs_scheduler(nprocs) ; 
	print_stats(nprocs) ; 

	return 0 ; 

}

