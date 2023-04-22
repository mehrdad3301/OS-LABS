#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define NUM_THREAD 5
#define QUEUE_SIZE 256


typedef struct Task {
    void (*task_function)(int, int);
    int arg1, arg2;
} Task;


Task task_queue[QUEUE_SIZE];
int task_count = 0;

pthread_mutex_t mutex_queue;
pthread_cond_t cond_queue;

void 
sum(int a, int b) 
{
	// simulating work 
    usleep(50000);
    int sum = a + b;
    printf("Sum of %d and %d is %d\n", a, b, sum);
}

void 
product(int a, int b) 
{
	// simulating work
    usleep(50000);
    int prod = a * b;
    printf("Product of %d and %d is %d\n", a, b, prod);
}


void 
execute_task(Task* task) 
{
    task->task_function(task->arg1, task->arg2);
}


void 
submit_task(Task task) 
{ 
	pthread_mutex_lock(&mutex_queue);
	task_queue[task_count++] = task; 
	pthread_mutex_unlock(&mutex_queue);
	pthread_cond_signal(&cond_queue); 
}

void* 
start_thread(void* args) 
{
    while (1) {
        Task task;

        pthread_mutex_lock(&mutex_queue);
        while (task_count == 0) {
            pthread_cond_wait(&cond_queue, &mutex_queue);
        }

        task = task_queue[0];
        int i;
        for (i = 0; i < task_count - 1; i++) {
            task_queue[i] = task_queue[i + 1];
        }
        task_count--;
        pthread_mutex_unlock(&mutex_queue);
        execute_task(&task);
    }
}


int 
main(int argc, char* argv[]) 
{
    pthread_t th[NUM_THREAD];
    pthread_mutex_init(&mutex_queue, NULL);
    pthread_cond_init(&cond_queue, NULL);
    int i;
    for (i = 0; i < NUM_THREAD; i++) {
        if (pthread_create(&th[i], NULL, &start_thread, NULL) != 0) {
            perror("Failed to create the thread");
        }
    }

    srand(time(NULL));
    for (i = 0; i < 100; i++) {
        Task t = {
            .task_function = i % 2 == 0 ? &sum : &product,
            .arg1 = rand() % 100,
            .arg2 = rand() % 100
        };
        submit_task(t);
    }

    for (i = 0; i < NUM_THREAD; i++) {
        if (pthread_join(th[i], NULL) != 0) {
            perror("Failed to join the thread");
        }
    }
    pthread_mutex_destroy(&mutex_queue);
    pthread_cond_destroy(&cond_queue);
    return 0;
}




