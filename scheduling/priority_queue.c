#include <stdio.h>
#include <stdlib.h>
#include "priority_queue.h"

PriorityQueue *
createPriorityQueue(size_t capacity) 
{
    PriorityQueue *queue = malloc(sizeof(PriorityQueue));
    queue->array = malloc(capacity * sizeof(void *));
    queue->priority = malloc(capacity * sizeof(int));
    queue->size = 0;
    queue->capacity = capacity;
    return queue;
}

int 
isEmpty(PriorityQueue *queue) 
{
    return queue->size == 0;
}

int isFull(PriorityQueue *queue) {
    return queue->size == queue->capacity;
}

void 
swap(void **array, int *priority, int i, int j) 
{
    void *temp = array[i];
    array[i] = array[j];
    array[j] = temp;
    int temp_priority = priority[i];
    priority[i] = priority[j];
    priority[j] = temp_priority;
}

void 
bubbleUp(void **array, int *priority, int index) 
{
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (priority[index] <= priority[parent]) {
            break;
        }
        swap(array, priority, index, parent);
        index = parent;
    }
}

void 
bubbleDown(void **array, int *priority, int index, int size) 
{
    while (index * 2 + 1 < size) {
        int leftChild = index * 2 + 1;
        int rightChild = index * 2 + 2;
        int maxChild = leftChild;
        if (rightChild < size && priority[rightChild] > priority[leftChild]) {
            maxChild = rightChild;
        }
        if (priority[index] >= priority[maxChild]) {
            break;
        }
        swap(array, priority, index, maxChild);
        index = maxChild;
    }
}

void 
enqueue(PriorityQueue *queue, void *item, int priority) 
{
    if (isFull(queue)) {
        printf("Priority queue is full.\n");
        return;
    }
    queue->array[queue->size] = item;
    queue->priority[queue->size] = priority;
    queue->size++;
    bubbleUp(queue->array, queue->priority, queue->size - 1);
}

void *
dequeue(PriorityQueue *queue) 
{
    if (isEmpty(queue)) {
        printf("Priority queue is empty.\n");
        return NULL;
    }
    void *item = queue->array[0];
    queue->size--;
    queue->array[0] = queue->array[queue->size];
    queue->priority[0] = queue->priority[queue->size];
    bubbleDown(queue->array, queue->priority, 0, queue->size);
    return item;
}

void 
freePriorityQueue(PriorityQueue *queue) 
{
    free(queue->array);
    free(queue->priority);
    free(queue);
}
void 
*peek(PriorityQueue *queue) 
{
    if (isEmpty(queue)) {
        printf("Priority queue is empty.\n");
        return NULL;
    }
    return queue->array[0];
}
