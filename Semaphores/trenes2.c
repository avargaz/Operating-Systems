#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "semaphore2.h"

#define CICLOS 10

char *pais[3] = {"Peru", "Bolivia", "Colombia"};

void proceso(int i, int segment_id) {
	SEMAFORO *sem;
	//printf("segment_id 2 %d\n",segment_id);
	sem = shmat(segment_id,NULL,0);

	//printf("count   %d\n",sem->count);
	int k;
	int c = 0;
	
	for(k=0;k<CICLOS;k++)
	{
		c = sem->count;
		//printf("j %d, %d \n",c,getpid());
		waitsem((sem));
		c = sem->count;
		printf("Entra %s  %d",pais[i], (int)getpid());
		fflush(stdout);
		//sleep(rand() % 3);
		printf("- %s Sale %d %d\n",pais[i], (int) getpid(), k);
		// Llamada waitsignal implementada en la parte 3
		pthread_mutex_lock(&((sem)->count2_mutex));
		signalsem((sem));
		c = (sem)->count;
		//printf("k %d, %d\n",c, getpid());
		pthread_mutex_unlock(&((sem)->count2_mutex));
		// Espera aleatoria fuera de la sección crítica
		sleep(1);
		//*sem = sem2;
	}
	exit(0);
	// Termina el proceso
}

int main(int argc, char const *argv[])
{
	SEMAFORO *sem;
	int segment_id = shmget(IPC_PRIVATE, sizeof(sem), S_IRUSR | S_IWUSR);
	pid_t tid[3];
	printf("segment_id %d\n",segment_id);
	int i;
	int status;
	
	sem = shmat(segment_id, NULL, 0);
	(*sem) = (*initsem(1));
	for(i=0;i<3;i++)
	{
		pid_t t;
		t = fork();
		if(t > 0) {
			proceso(i, segment_id);
		}
	}
	for(i = 0; i < 3; i++){
    	wait(&status);
    }
	return 0;
}