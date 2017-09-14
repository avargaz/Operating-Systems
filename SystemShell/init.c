#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUMVENTANAS 6

int main(){
  int status;
  int arr[10]; //Numero maximo de ventanas = 10

  //Ciclo for para crear la cantidad de procesos/ventanas necesarias
  for(int i=0;i<NUMVENTANAS;i++){
    arr[i]=fork();
    if (arr[i]==0){
      execlp("/usr/bin/xterm","/usr/bin/xterm", "-e", "./getty",NULL);
    }
  };
    wait(&status);

  }
