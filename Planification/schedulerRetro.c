#include "scheduler.h"

extern THANDLER threads[MAXTHREAD];
extern int currthread;
extern int blockevent;
extern int unblockevent;

QUEUE ready;
QUEUE waitinginevent[MAXTHREAD];
int priorityArray[20][10]={{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},
{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1},{-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}};
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
		putInArray(callingthread,0);
		printf("NEWTHREAD\n");
	}

	if(event==BLOCKTHREAD)
	{

		threads[callingthread].status=BLOCKED;
		_enqueue(&waitinginevent[blockevent],callingthread);
		changethread=1;
		printf("BLOCKTHREAD\n");
	}

	if(event==ENDTHREAD)
	{
		threads[callingthread].status=END;
		changethread=1;
		printf("ENDTHREAD\n" );
	}

	if(event==UNBLOCKTHREAD)
	{
			callingthread = getFirstProcess();
			threads[callingthread].status=READY;
			_enqueue(&ready,callingthread);
			printf("UNBLOCK\n" );

	}
	if(threads[currthread].status=RUNNING){
		old = getFirstProcess();
		threads[old].status=READY;
		next=_dequeue(&ready);
		threads[next].status=RUNNING;
		_swapthreads(old,next);
	}

	if(changethread)
	{
		old=currthread;
		next=_dequeue(&ready);
		threads[next].status=RUNNING;
		_swapthreads(old,next);
	}
}

int isInArray(int process){
	for(int i=0;i<20;i++){
		for(int j=0;j<20;j++){
			if(priorityArray[i][j] == process)
			return 1;
		}
	}
	return 0;
}
void putInArray(int process,int priority){
	for(int j=0;j<20;j++){
		if(priorityArray[priority][j]== -1){
			priorityArray[priority][j] = process;
			return;
		}
	}
	printf("INSERTED IN %d\n",priority );
	return;
}
void putInNextPriority(int process, int index){
	for(int j=0;j<20;j++){
		if(priorityArray[index][j] == -1){
			priorityArray[index][j] = process;
		}
	}
}
void changeQueue(int index){
	_emptyq(&ready);
	for(int j=0;priorityArray[index][j]!=-1;j++){
		_enqueue(&ready,priorityArray[index][j]);
	}
}
int getFirstProcess(){
	for(int i=0;i<20;i++){
		for(int j=0;j<20;j++){
			if(priorityArray[i][j] != -1){
				putInNextPriority(priorityArray[i][j],i+1);
				changeQueue(i);
				printf("Prioridad:%d \n",i );
				return priorityArray[i][j];
			}
		}
	}
	return 0;
}
