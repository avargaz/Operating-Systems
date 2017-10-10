#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "semaphore.h"

#include <sys/wait.h>
#include <sys/time.h>

#define SISCONT 1
#define SISSTOP 0


void _initqueue(QUEUE *q)
{
	q->head=0;
	q->tail=0;
	q->waitElem[0] = SISCONT;
	q->waitElem[1] = SISCONT;
	q->waitElem[2] = SISCONT;
}

void _enqueue(QUEUE *q,int val)
{
	//Poner nuevo valor en head
	q->elements[q->head] = val;
	//parar elemento
	q->waitElem[val]= SISTOP;
	//preparar queue para recibir nuevo valor
	q->head++;
	q->head=q->head%MAXQUEUE;
}

int _dequeue(QUEUE *q)
{
	int next;
	//sacar de la queue el proximo valor
	next=q->elements[q->tail];
	//poner en marcha al siguiente elemento
	q->waitElem[next] = SISCONT;
	//preparar al queue para sacar proximo elemento
	q->tail++;
	q->tail=q->tail%MAXQUEUE;
	return next;
}

void waitsem(SEMAFORO *sem, int i) 
{
	sem->count = sem->count - 1;
	if(sem->count < 0)
		_enqueue(sem->waiting_queue, i);

	while(sem->waiting_queue->waitElem[i] == SISSTOP){
		sleep(1);

	return;
}

void signalsem(SEMAFORO *sem) 
{
	sem->count++;
	if(sem->count <= 0)
		int i = _dequeue(sem->waiting_queue);
		
	return;
}

SEMAFORO* initsem(int count)
{	
	SEMAFORO *sem;
	sem = (SEMAFORO *)malloc(sizeof(SEMAFORO));
	sem->count = count;
	printf("initsem count %d\n",sem->count);
	sem->waiting_queue = (QUEUE *)malloc(sizeof(QUEUE));
	_initqueue(sem->waiting_queue);

	return sem;
}