#ifndef _semaphore_h
#define _semaphore_h
#include <pthread.h>
#define MAXQUEUE 20

typedef struct _QUEUE {
    int elements[MAXQUEUE];
    int waitElem[MAXQUEUE];
    int head;
    int tail;
} QUEUE;

typedef struct _semaforo {
    int count;
    QUEUE *waiting_queue;
    pthread_mutex_t count_mutex;
    pthread_mutex_t count2_mutex;
} SEMAFORO;

void _initqueue(QUEUE *q);

void _enqueue(QUEUE *q, int val);

int _dequeue(QUEUE *q);

void waitsem(SEMAFORO *sem, int i);

void signalsem(SEMAFORO *sem);

SEMAFORO* initsem(int count);

#endif