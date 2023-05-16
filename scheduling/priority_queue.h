#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdlib.h>

typedef struct {
    void **array;
    int *priority;
    size_t size;
    size_t capacity;
} PriorityQueue;

PriorityQueue *createPriorityQueue(size_t capacity);
int isEmpty(PriorityQueue *queue);
int isFull(PriorityQueue *queue);
void enqueue(PriorityQueue *queue, void *item, int priority);
void *dequeue(PriorityQueue *queue);
void freePriorityQueue(PriorityQueue *queue);
void *peek(PriorityQueue *queue);

#endif
