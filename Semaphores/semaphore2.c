#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include "semaphore2.h"

#include <sys/wait.h>
#include <sys/time.h>


void _initqueue(QUEUE *q)
{
	q->head=0;
	q->tail=0;
}

void _enqueue(QUEUE *q,pid_t val)
{
	q->elements[q->head]=(int)val;
	// Incrementa al apuntador
	q->head++;
	q->head=q->head%MAXQUEUE;
}

pid_t _dequeue(QUEUE *q)
{
	int valret;
	valret=q->elements[q->tail];
	// Incrementa al apuntador
	q->tail++;
	q->tail=q->tail%MAXQUEUE;
	return((pid_t)valret);
}

void waitsem(SEMAFORO *sem) 
{
	//printf("hola 6\n");
	int c = sem->count;
	c--;
	
	sem->count = 0 + c;
	//printf("Move %d\n", sem->count);
	if(sem->count < 0)
	{
		//printf("Hola 3 \n");
		// agregar proceso a la cola de bloqueados
		// KILL PID SIGSTOP
		_enqueue(sem->waiting_queue, getpid());
		//printf("pid get %d\n",getpid());
		if(kill(getpid(), SIGSTOP))
		{
			printf("error kill\n");

			exit(0);
		}

	}
	//printf("Hola 2 \n");
	return;
}

void signalsem(SEMAFORO *sem) 
{

	sem->count++;
	if(sem->count <= 0)
	{
		pid_t next = _dequeue(sem->waiting_queue);
		if(kill(next, SIGCONT))
		{
			printf("error kill cont\n");
			
			exit(0);
		}
	}
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