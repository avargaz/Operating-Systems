#include "scheduler.h"
#include <stdio.h>

extern THANDLER threads[MAXTHREAD];
extern int currthread;
extern int blockevent;
extern int unblockevent;

QUEUE ready;
QUEUE waitinginevent[MAXTHREAD];

void scheduler(int arguments)
{
	int old,next;
	int changethread=0;
	int waitingthread=0;

	int event=arguments & 0xFF00;
	int callingthread=arguments & 0xFF;

	if(event==NEWTHREAD)
	{
		// Un nuevo hilo va a la cola de listos
		threads[callingthread].status=READY;
		_enqueue(&ready,callingthread);
		_enqueue(&ready,callingthread);
		//printf("\nNEWTHREAD%d",callingthread);
	}

	if(event==BLOCKTHREAD)
	{

		threads[callingthread].status=READY;
		_enqueue(&waitinginevent[blockevent],callingthread);
		changethread=1;
		//printf("\nBLOCKTHREAD%d\n",callingthread);
	}

	if(event==ENDTHREAD)
	{
		threads[callingthread].status=END;
		changethread=1;
		//printf("\nENDTHREAD%d",callingthread);
	}

	if(event==UNBLOCKTHREAD)
	{
			threads[callingthread].status=READY;
			_enqueue(&ready,callingthread);
			//threads[callingthread].status=;
			//changethread=1;
			//printf("\nUNBLOCKTHREAD%d",callingthread);
	}

	if(threads[currthread].status=RUNNING){
		//printf("\nUNBLOCKTHREAD");
		old = currthread;
		threads[old].status=READY;
		_enqueue(&ready,old);
		next=_dequeue(&ready);
		threads[next].status=RUNNING;
		_swapthreads(old,next);
	}


	if(changethread)
	{
		//validate if the next one is equal to the current one
		old=currthread;
		next=_dequeue(&ready);
		if(currthread == next){//it's equal
				next=_dequeue(&ready);
		}
		threads[next].status=RUNNING;
		_swapthreads(old,next);

		//printf("\nchangethread%d\n",next);
	}

}
