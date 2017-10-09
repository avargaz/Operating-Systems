#define _GNU_SOURCE
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <malloc.h>
#include <sys/ipc.h>
#define FIBER_STACK 65536


long long num_steps = 1000000000;
double step;
double piSeg[4]; //Segmento de PI


int hiloFunc(void *args){
  double x=0.0, sum=0.0;
  int counter = *((int *) args);//gets values from received args
  printf("%d\n",counter );
  step = 1./(double)num_steps;
  long range= counter*250000000;
  printf("%lu\n",range );
  for (int i=range; i<range+250000000; i++){
    x = (i + .5)*step;
    sum = sum + 4.0/(1.+ x*x);
  }

  piSeg[counter] = (sum*step);
  printf("%lf\n",piSeg[counter]);
}



int main(int argc, char* argv[]){

//Time variables and start of timer
long long start_ts;
long long stop_ts;
float elapsed_time;
long lElapsedTime;
struct timeval ts;
void* stack[4];
pid_t pid[4];
int status;
double piTotal;
int pmts[4];
gettimeofday(&ts, NULL);
start_ts = ts.tv_sec * 1000000 + ts.tv_usec; // Tiempo inicial


// Allocate the stack
for (int i =0 ; i<4 ; i++){
  piSeg[i] = 0.0;//used for writing in each different slot. one per process
  stack[i] = malloc( FIBER_STACK );
}

printf( "Creating child thread\n" );


// Call the clone system call to create the child thread
for(int i =0; i<4 ; i++){
  pmts[i]=i;
  pid[i] = clone( hiloFunc, (char*) stack[i]+FIBER_STACK,  CLONE_SIGHAND|CLONE_FS|CLONE_VM|SIGCHLD, &pmts[i]);
}

for(int i = 0; i < 4; i++){//waiting for pids of each child process to end
 		waitpid(pid[i], &status, 0);
}

//Sumar segmentos para el total
for(int i = 0; i < 4; i++) {
  piTotal += piSeg[i];

}

//time variables and stopping of timer
printf( "Child thread returned\n" );
gettimeofday(&ts, NULL);
stop_ts = ts.tv_sec * 1000000 + ts.tv_usec;
elapsed_time = (float) (stop_ts - start_ts)/1000000.0;

printf("El valor de PI es %1.12f\n",piTotal);
printf("Tiempo = %2.2f segundos\n",elapsed_time);

return 0;
}
