/* 
A dead simple implementation of priority based scheduling. 
For simplicity we don't consider floating points. Everything 
including process duration, arrival time, .. are considered  
to be integers. 
*/

#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h>
#include "priority_queue.h"

#define MAX_PROC 10


struct proc { 

	int pid ; 
	int pr ;  //priority 
	int bt ;  //burst time
	int rt ;  //running time, 
			  //eventually equals bt 
	int wt ;  //waiting time
	int tt ;  //turnaround time
	int at ;  //arrival time

} ; 

struct proc procs[MAX_PROC] ; 

void 
print_stats(int nprocs) 
{ 
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
	printf("Average Waiting: %d\nAverage Turnaround: %d", tt / nprocs , wt / nprocs ) ; 
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
pr_scheduling(int nprocs) 
{ 

	sort_by_at(nprocs) ; 

	PriorityQueue *queue = createPriorityQueue(nprocs);

	int elapsed_time = procs[0].at ; 

	int i = 0 ; 
	while (1) { 

		int j ; 
		for (j = i ; procs[j].at == procs[i].at ; j++) { 
			enqueue(queue, (void *) &procs[j], procs[j].pr) ;
			if (j == nprocs) { 
				goto enqueued ; 
			}
		}

		while( elapsed_time < procs[j].at ) { 
			struct proc *p = peek(queue) ; 
			p->rt += 1 ;  
			if (p->rt == p->bt) { 
				p->tt = elapsed_time - p->at ; 
				p->wt = p->tt - p->bt ; 
				dequeue(queue) ; 
			}
			elapsed_time += 1 ; //TODO: omit incrementing by 1
		}
		i = j; 
	}

	enqueued :
	while (! isEmpty(queue)) { 
		struct proc *p = dequeue(queue) ; 
		elapsed_time += p->bt - p->rt ; 
		p->rt = p->bt ; 
		p->tt = elapsed_time - p->at ; 
		p->wt = p->tt - p->bt ; 
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

		int pr ; 
		printf("priority: ") ; 
		scanf("%d", &pr) ; 

		procs[i] = (struct proc) { 
			.pid = i, 
			.bt = bt, 
			.at = at,
			.pr = pr, 
		} ; 
		printf("\n") ; 
	}

	pr_scheduling(nprocs) ; 
	print_stats(nprocs) ; 

	return 0 ; 

}

