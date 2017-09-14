#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <string.h>


void agregarVariable(char * entrada);
int esVariable(char * argumento);
const char * susbstituirVariable(char * varNombre);

int main()
{
	int pid;
	int status;
	int flag = 1;
	int hayArgumento = 0;
	int containsVar = 0;
	char entrada[30];
	char comando[30];
	char argumento[30];

	printf("SHELL\n\n");

  while(flag) {
		printf("sh> ");
		fgets(entrada, 30, stdin);

		if(sscanf(entrada, "%s %s", &comando, &argumento) == 2)
		{
	    hayArgumento = 1;

	    if(esVariable(comando))
			{
	    	if(susbstituirVariable(comando) != NULL)
				{
	    			strcpy(comando, susbstituirVariable(comando)); //remplazar variable por comando
	    	}
				else {
						printf("Error... No existe esa variable\n");
				}
      }

			/*if(esVariable(argumento) && strcmp(comando,"echo") && strcmp(comando,"export"))
			{
				if(susbstituirVariable(argumento) != NULL)
				{
		    	strcpy(argumento, susbstituirVariable(argumento));
		    }
				else {
						printf("Error... No existe esa variable\n");
				}

		   */
		 }

		 else if (sscanf(entrada, "%s", &comando) == 1)
		 {
			 hayArgumento=0; //Resetear variable
			 if(esVariable(comando))
			 {
				 if(susbstituirVariable(comando) != NULL)
				 {
					 strcpy(comando, susbstituirVariable(comando));
		     }
				 else
				 {
					 printf("Error... No existe esa variable\n");
				 }
			 }
		 }

		 else
		 {
		   printf("Entrada invalida\n");
			 hayArgumento=0; //Resetear variable
		   continue; //Para no romper del loop
		 }

		if(strcmp("export", comando) == 0)
    {
			agregarVariable(argumento);
		}

		else if(strcmp("echo", comando) == 0)
    {
			if(susbstituirVariable(argumento))
			{
				printf("%s\n", susbstituirVariable(argumento));
			}
			else
      {
				printf("Error... No existe esa variable\n");
			}
		}

		else if(strcmp("exit", comando) == 0)
    {
			return 0;
		}

		else {
			pid = fork();

			if(pid == 0) {
				if(hayArgumento)
					execlp(comando, comando, argumento, (char *)NULL);
				else
					execlp(comando, comando, (char *)NULL);
			}
			else {
				wait(&status);
			}
		}
	}
}

void agregarVariable(char * entrada)
{
	char varTag[30];
	char varValor[30];
	FILE *archivo;

	sscanf(entrada, "%[^=]%*c%s", varTag, varValor);  //entrada: $TAG=VALOR . [^] not sotre =

	archivo = fopen("variables.txt", "a");

	fprintf(archivo, "%s : %s\n", varTag, varValor);

	fclose(archivo);


}

int esVariable(char * argumento)
{
	const char* comparar[30];
	strcpy(comparar, argumento);
	if(strchr(comparar, '$') == NULL){ //checar si tiene el signo de peso
		return 0;
	}
	else
	{
		return 1;
	}
}

const char * susbstituirVariable(char * varNombre)
{
	FILE* archivo;
	char varTag[30];
	static char varValor[30];

	archivo = fopen("variables.txt","rw+");

	while (!feof(archivo))
	{
		if (fscanf(archivo,"%s : %s",&varTag, &varValor))
		{
			if(strcmp(varTag, varNombre) == 0) { //regresa 0 si son iguales
				fclose(archivo);
				return varValor;
			}
		}
	}
	fclose(archivo);
	return NULL;
}
