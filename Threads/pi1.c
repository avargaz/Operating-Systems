#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

long long num_steps = 1000000000;//total steps
double step;
double pi;
int NO_PROC = 4;//numer of processors
int counter = 0;//starting on cero

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *hiloFunc(void *args){
  double x, sum=0.0;
  pthread_mutex_lock(&mutex);
  long range= counter*250000000;
  counter++;
  pthread_mutex_unlock(&mutex);
  printf("%lu\n",range);
  for (int i=range; i<range+250000000; i++){
    x = (i + .5)*step;
    sum = sum + 4.0/(1.+ x*x);
  }
  pthread_mutex_lock(&mutex);
  pi = pi+(sum*step);
  pthread_mutex_unlock(&mutex);
}

int main(int argc, char* argv[]){

//Time variables and starters of timer
long long start_ts;
long long stop_ts;
float elapsed_time;
long lElapsedTime;
struct timeval ts;
gettimeofday(&ts, NULL);
start_ts = ts.tv_sec * 1000000 + ts.tv_usec;

step = 1./(double)num_steps;


pthread_t idhilo[4]; //creates a variable to store the return value


for(int i=0;i<4;i++){
  pthread_create(&idhilo[i],NULL,hiloFunc,NULL); //create threads
}

for(int i=0;i<4;i++){
  pthread_join(idhilo[i],NULL); //waits for all threads to finish
}

//time variables and stop timer
gettimeofday(&ts, NULL);
stop_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Tiempo final
elapsed_time = (float) (stop_ts - start_ts)/1000000.0;

printf("El valor de PI es %1.12f\n",pi);
printf("Tiempo = %2.2f segundos\n",elapsed_time);

return 0;
}
