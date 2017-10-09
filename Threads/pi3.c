#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

long long num_steps = 1000000000;
double step;
double pi=0;
double counter=0;

int main(int argc, char* argv[]){

//Time variables and beginning of timer
long long start_ts;
long long stop_ts;
float elapsed_time;
long lElapsedTime;
struct timeval ts;
int status;
gettimeofday(&ts, NULL);
start_ts = ts.tv_sec * 1000000 + ts.tv_usec;
step = 1./(double)num_steps;


int shmid;
int shmid2;

double *shm;
double *shm2;

//Crear shared memory
shmid= shmget(IPC_PRIVATE,(1024*64),IPC_CREAT | 0666);
shmid2= shmget(IPC_PRIVATE,(1024*64),IPC_CREAT | 0667);


shm = shmat(shmid, NULL,0);
shm2 = shmat(shmid2, NULL,0);

char piString[(50)];
char counterString[(50)];

snprintf(piString, (50), "%1.12f", pi);
snprintf(counterString, (50), "%1.12f", counter);

memcpy(shm, piString,sizeof(piString));
memcpy(shm2, counterString,sizeof(counterString));

//here we create many else ifs to assign numbre ranges to each process. only the parent process will enter the last else
if (fork()){
  printf("Hola,soy el hijo1\n");
  double x, sum=0.0;
  char piString[(50)];
  char counterString[(50)];
  double pi,counter;
  sscanf(shm2, "%lf", &counter);
  long range=counter*250000000;
  counter++;
  snprintf(counterString, (50), "%1.12f", counter);
  memcpy(shm2, counterString,sizeof(counterString));
  for (int i=range; i<range+250000000; i++){
    x = (i + .5)*step;
    sum = sum + 4.0/(1.+ x*x);
  }
    sscanf(shm, "%lf", &pi);
    pi = pi+(sum*step);
    snprintf(piString, (50), "%1.12f", pi);
    memcpy(shm, piString,sizeof(piString));
    exit(0);
}

else if (fork()){
  sleep(1);
  printf("Hola,soy el hijo2\n");
  double x, sum=0.0;
  char piString[(50)];
  char counterString[(50)];
  double pi,counter;
  sscanf(shm2, "%lf", &counter);
  long range=counter*250000000;
  counter++;
  snprintf(counterString, (50), "%1.12f", counter);
  memcpy(shm2, counterString,sizeof(counterString));
  for (int i=range; i<range+250000000; i++){
    x = (i + .5)*step;
    sum = sum + 4.0/(1.+ x*x);
  }
    sscanf(shm, "%lf", &pi);
    pi = pi+(sum*step);
    snprintf(piString, (50), "%1.12f", pi);
    memcpy(shm, piString,sizeof(piString));
    exit(0);

}

else if (fork()){
  sleep(2);
  printf("Hola,soy el hijo3\n");
  double x, sum=0.0;
  char piString[(50)];
  char counterString[(50)];
  double pi,counter;
  sleep(0.5);
  sscanf(shm2, "%lf", &counter);
  long range=counter*250000000;
  counter++;
  snprintf(counterString, (50), "%1.12f", counter);
  memcpy(shm2, counterString,sizeof(counterString));
  for (int i=range; i<range+250000000; i++){
    x = (i + .5)*step;
    sum = sum + 4.0/(1.+ x*x);
  }
    sscanf(shm, "%lf", &pi);
    pi = pi+(sum*step);
    snprintf(piString, (50), "%1.12f", pi);
    memcpy(shm, piString,sizeof(piString));
    exit(0);
}


else {
  sleep(3);
  printf("Hola,soy el hijo4\n");
  double x, sum=0.0;
  char piString[(50)];
  char counterString[(50)];
  double pi,counter;
  sleep(0.7);
  sscanf(shm2, "%lf", &counter);
  long range=counter*250000000;
  counter++;
  snprintf(counterString, (50), "%1.12f", counter);
  memcpy(shm2, counterString,sizeof(counterString));
  for (int i=range; i<range+250000000; i++){
    x = (i + .5)*step;
    sum = sum + 4.0/(1.+ x*x);
  }
    sscanf(shm, "%lf", &pi);
    pi = pi+(sum*step);
    snprintf(piString, (50), "%1.12f", pi);
    memcpy(shm, piString,sizeof(piString));

}


for(int i = 0; i < 4; i++){
    	wait(&status);
    }


sscanf(shm, "%lf", &pi);
//Time variables and stopping of timer
gettimeofday(&ts, NULL);
stop_ts = ts.tv_sec * 1000000 + ts.tv_usec;
elapsed_time = (float) (stop_ts - start_ts)/1000000.0;

printf("\nEl valor de PI es %1.12f\n",pi);
printf("Tiempo = %2.2f segundos\n",elapsed_time);

return 0;
}
