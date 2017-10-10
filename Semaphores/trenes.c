#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "semaphore.h"

#define CICLOS 10

char *pais[3] = {"Peru", "Bolivia", "Colombia"};

pthread_t tid[3];

SEMAFORO *sem;

void *proceso(void *arg) {

	int i = *((int *)arg);
	int k;
	//printf("hola 1\n");
	//printf("hola 5 %ld, %ld, %d, %d\n",pthread_self(), tid[i], i, getpid());
	for(k=0;k<CICLOS;k++)
	{
		// Llamada waitsem implementada en la parte 3
		
		//sleep(1);
		pthread_mutex_lock(&(sem->count_mutex));
		waitsem(sem, i);
		pthread_mutex_unlock(&(sem->count_mutex));
		//printf("hola 2\n");
		printf("Entra %s ",pais[i]);
		fflush(stdout);
		//sleep(rand()%3);
		printf("- %s Sale %d\n",pais[i], k);
		// Llamada waitsignal implementada en la parte 3
		//pthread_mutex_lock(&(sem->count_mutex));
		signalsem(sem);
		//pthread_mutex_unlock(&(sem->count_mutex));
		// Espera aleatoria fuera de la sección crítica
		sleep(1);
	}
	//printf("hola 3\n");
	//exit(0);
	// Termina el proceso
}

int main(int argc, char const *argv[])
{
	sem = initsem(1);
	pthread_t tid[3];
	int res;
	int args[3];
	int i;
	void *thread_result;
	srand(getpid());
	//Implementación de Semáforos.
	//Sistemas Operativos, Primavera 2017
	// Crea los hilos
	//printf("hola pid2. %d\n",getpid());
	for(i=0;i<3;i++)
	{
		//printf("hola pid3. %d\n",getpid());
		args[i]=i;
		pthread_create(&tid[i],NULL,proceso ,(void *) &args[i]);
	}
	// Espera que terminen los hilos
	//printf("debug 0\n");
	for(i=0;i<3;i++)
	{
		//printf("debug 2\n");
		res = pthread_join(tid[i], &thread_result);
	}
	//printf("debug 1\n");
	return 0;
}