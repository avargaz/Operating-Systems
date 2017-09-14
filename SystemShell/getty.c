#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

int main(){
  char linea[30];
  char usuario[30];
  char contra[30];
	char usuarioEnTexto[30];
	char contraEnTexto[30];
  int flag = 1;
  int status;
  int pid;
  FILE* archivo;

while(flag) {
  printf("Usuario: ");
  scanf("%s",usuario);

  printf("Contraseña: ");
  scanf("%s",contra);

  archivo = fopen("passwd.txt","rw+");

  while (!feof(archivo))
  {
    if (fscanf(archivo,"%s : %s",usuarioEnTexto, contraEnTexto)) //fscanf(archivo,regex,variables)
    {
      if(strcmp(usuarioEnTexto, usuario) == 0)
      {               //0 si str1==str2
        if(strcmp(contraEnTexto, contra) == 0)
        {
          printf("\nLogin exitoso!\n\n");
          flag = 0;
        }
        else{
        }
      }
    }
  }
  fclose(archivo);
  if(!flag){
    break;
  }
  printf("\nLogin no valido. Vuelva a intentar.\n\n");
}

pid = fork();

if(pid == 0) {
  execlp("./sh","./sh", NULL);
}
else {
  wait(&status);
  }
}
