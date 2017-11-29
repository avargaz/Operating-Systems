#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/msg.h>
#include <wait.h>

// Definir una estructura que se va a usar para mandar los mensajes
typedef struct {
	long msg_type;		// Tipo de mensaje, debe ser long
	char mensaje[100];	// Contenido
} MSGTYPE;

int msgqid;

char *pais[3] = {"Peru", "Bolivia", "Colombia"};

void emisor();
void receptor();

int main()
{
	int pid;

	// Crear un buzón o cola de mensajes
	msgqid=msgget(0x1234,0666|IPC_CREAT);

	// Crear un nuevo proceso
	for(int i=0;i<3;i++)
	{
		//pid_t t;
		//hacemos 3 forks
		pid = fork();
		if(pid == 0){
			emisor(i); // Se ejecuta el emisor en el proceso hijo
		}
	}
	if(pid!=0){
		receptor();	// El padre es el receptor
		wait(NULL);
		wait(NULL);
		wait(NULL);
		wait(NULL);
	}
	printf("Procesos terminados\n");
	msgctl(msgqid,IPC_RMID,NULL);
	return(0);
}

void emisor(int paisIndex)
{
	//printf("Se crea emisor\n");
	int i;
	char *text;
	MSGTYPE m;
	for(i=0;i<10;i++)
	{
		m.msg_type=1;
		sprintf(m.mensaje,"Entra %s vuelta: %d", pais[paisIndex], i);
		msgsnd(msgqid,&m,sizeof(MSGTYPE)-sizeof(long),0);
		sleep(3);
	}
	m.msg_type=1;
	strcpy(m.mensaje,"Fin");
	msgsnd(msgqid,&m,sizeof(MSGTYPE)-sizeof(long),0);

	exit(0);	// Termina el proceso
}

void receptor()
{
	int total = 0;
	//printf("Se crea receptor\n");
	MSGTYPE m;	// Donde voy a recibir el mensaje
	int retval;

	do
	{
		retval=msgrcv(msgqid,&m,sizeof(MSGTYPE)-sizeof(long),1,0);
		printf("Recibido: %s",m.mensaje);
		sleep(2);
		if(strcmp(m.mensaje,"Fin")==0){
			total++;
		}
		printf(" -> Sale de CS \n");
	}
	while(total!=3); // Hasta que el mensaje sea "Fin"
	return;
}

//Referencia
//José Luis Elvira Valenzuela
