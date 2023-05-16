
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h>

#define MAX_PROC 10


struct proc { 

	int   pid ; 
	float bt ;  //burst time
	float rt ;  //running time, 
			    //eventually equals bt 
	float wt ;  //waiting time
	float tt ;  //turnaround time
	float at ;  //arrival time

} ; 

struct proc procs[MAX_PROC] ; 
float quantum ; 

void print_stats(int nprocs) { 

	float tt, wt; 
	tt = wt = 0; 
	 
	int i ; 
	for (i = 0 ; i < nprocs ; i++) { 
		printf("_______\n") ; 
		printf("Process: %d\nArrival: %.3f\nBurst: %.3f\nTurnaround: %.3f\nWaiting: %.3f\n",
		procs[i].pid, procs[i].at, procs[i].bt, procs[i].tt, procs[i].wt) ;
		tt += procs[i].tt ; 
		wt += procs[i].wt ; 
		printf("\n") ;
	}

	printf("Average Waiting: %f\nAverage Turnaround: %f", wt / nprocs, tt / nprocs) ; 
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
rr_scheduler(int nprocs) 
{ 
	sort_by_at(nprocs) ; 

	float elapsedtime = procs[0].at ; 
	int done = 0 ; 
	while ( done != nprocs ) {

		int j; 
		for ( j = 0, done = 0 ; j < nprocs ; j ++ ) { 
			if ( procs[j].at > elapsedtime ) { 
				break ; 
			}
			if ( procs[j].rt == procs[j].bt ) { 
				done += 1 ; 
				continue ; 
			}
			if ( procs[j].bt - procs[j].rt > quantum ) { 
				procs[j].rt += quantum ; 
				elapsedtime += quantum ; 
			} else { 
				elapsedtime += procs[j].bt - procs[j].rt ; 
				procs[j].rt = procs[j].bt ; 
				procs[j].tt = elapsedtime - procs[j].at ; 
				procs[j].wt = procs[j].tt - procs[j].bt ;  
			}
		}
	}
}

int   
main (int argc, char *argv[]) 
{ 

	if (argc != 3) { 
		printf("invalid number of arguments, expected: 2 got: %d", argc) ; 
		return 1 ; 
	}

	int nprocs = atoi(argv[1]) ; 
	quantum = atof(argv[2]) ; 

	int i ; 
	for(i = 0 ; i < nprocs ; i++) { 
		float at ;
		printf("arrival time: ") ;
		scanf("%f", &at) ; 

		float bt ; 
		printf("burst time: ") ; 
		scanf("%f", &bt) ; 

		procs[i] = (struct proc) { 
			.pid = i, 
			.bt = bt, 
			.at = at,
		} ; 
		printf("\n") ; 
	}

	rr_scheduler(nprocs) ; 
	print_stats(nprocs) ; 

	return 0 ; 

}

