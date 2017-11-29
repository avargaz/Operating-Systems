#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/sem.h>
#include <wait.h>

#define CICLOS 10


char *pais[3] = {"Peru", "Bolivia", "Colombia"};

int semaforo;

void waitS(int semid);
void signalS(int semid);
void proceso(int n);

int main()
{
	int i;
	int pid;

	srand(getpid());

	semaforo=semget((key_t)0x1234,1,0666|IPC_CREAT); // Pedir al SO que me de un semáforo
	semctl(semaforo,0,SETVAL,1);	// Inicializa el elemento 0 del arreglo de semáforos con 1

	for(i=0;i<3;i++)
	{
		pid=fork();
		if(pid==0)
			proceso(i);
	}

	for(i=0;i<3;i++)
		wait(NULL);

	semctl(semaforo,0,IPC_RMID,0);
	printf("FIN\n");
}

void waitS(int semid)
{
	struct sembuf s;
	s.sem_num=0;
	s.sem_op=-1;	// Wait
	s.sem_flg=0;
	semop(semid,&s,1);
	return;
}

void signalS(int semid)
{
	struct sembuf s;
	s.sem_num=0;
	s.sem_op=1;	// Signal
	s.sem_flg=0;
	semop(semid,&s,1);
	return;
}


void proceso(int n)
{
	int i;

	for(i=0;i<10;i++)
	{
		waitS(semaforo);
		printf("%s entra a CS -> ",pais[n]);
		sleep(1+rand()%3);
		printf("%s sale de CS. Vuelta: %d \n",pais[n], i);
		signalS(semaforo);
		sleep(1+rand()%3);
	}
	exit(0);
}

//Referencia
//José Luis Elvira Valenzuela
