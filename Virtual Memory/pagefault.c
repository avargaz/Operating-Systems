#include <stdio.h>
#include <stdlib.h>
#include "mmu.h"

#define RESIDENTSETSIZE 24

extern char *base;
extern int framesbegin;
extern int idproc;
extern int systemframetablesize;
extern int ptlr;

extern struct SYSTEMFRAMETABLE *systemframetable;
extern struct PROCESSPAGETABLE *ptbr;
extern struct PROCESSPAGETABLE *gprocesspagetable;

int indiceLibre=0; //**************************************
int getfreeframe();
int memoriaSwap();

// Rutina de fallos de página

int pagefault(char *vaddress)
{
    int i;
    int frame;
    long pag_a_expulsar;
    long pag_del_proceso;

    // Calcula la página del proceso
    pag_del_proceso=(long) vaddress>>12;
    // Cuenta los marcos asignados al proceso
    i=countframesassigned();
  
    // Busca un marco libre en el sistema
    frame=getfreeframe(pag_del_proceso);  //Parametro +

    if(frame==-1)
    {
        return(-1); // Regresar indicando error de memoria insuficiente
    }


    (ptbr+pag_del_proceso)->presente=1;
    (ptbr+pag_del_proceso)->modificado=0; //***********
    (ptbr+pag_del_proceso)->framenumber=frame;


    return(1); // Regresar todo bien
}


int getfreeframe(long pag_del_proceso) //Parametro +
{
    int i;
    // Busca un marco libre en el sistema
    for(i=framesbegin;i<systemframetablesize+framesbegin;i++){
        if(!systemframetable[i].assigned)
        {
            systemframetable[i].assigned=1;
            break;
        }
    }
    if(i<systemframetablesize+framesbegin){
        systemframetable[i].assigned=1;
    }
    else{
        //i=-1;
        i=memoriaSwap(pag_del_proceso);//Parametro +
    }
    return i;
}

int memoriaSwap(long pag_del_proceso)
{
    //Abrir archivo swap
    FILE *archivoSwap;
    fopen("swap", "r+b");

    int i;

    indiceLibre++ % 12;
    
    // fwrite systemframetable[indiceLibre] > indiceLibre * 4k
    fseek(archivoSwap, indiceLibre<<12, SEEK_SET);
    fwrite(systemframetable[indiceLibre].paddress, 4096, 1, archivoSwap);


    // fread swap
    fseek(archivoSwap, indiceLibre<<12, SEEK_SET);
    char *swapIndiceLibre = (char *) malloc(4096);
    fread(swapIndiceLibre, 4096, 1, archivoSwap);
    
    
    // fread Swap Pagina Proceso
    fseek(archivoSwap, pag_del_proceso<<12, SEEK_SET);
    char *swapPaginaProceso = (char *) malloc(4096);
    fread(swapPaginaProceso, 4096, 1, archivoSwap);
    
    
    // fwrite swapIndiceLibre
    fseek(archivoSwap, pag_del_proceso<<12,SEEK_SET);
    fwrite(swapIndiceLibre, 4096,1,archivoSwap);

    
    // fwrite swapPaginaProceso
    fseek(archivoSwap, indiceLibre<<12,SEEK_SET);
    fwrite(swapPaginaProceso, 4096,1, archivoSwap);

    
    // fwrite indiceLibre * 4k > systemframetable[indiceLibre]
    fseek(archivoSwap, indiceLibre<<12, SEEK_SET);
    fwrite(systemframetable[indiceLibre].paddress, 4096,1 , archivoSwap);

    
    // Cerrar archivo
    fclose(archivoSwap);
    
    ///////////////////////////////////////////////////////////////////////
    
    for(i=0; i < ptlr; i++) 
    {
        if(ptbr[i].framenumber == indiceLibre) 
        {
            ptbr[i].modificado = 0;
            ptbr[i].presente = 0;
            break;
        }
    }


    
    return indiceLibre;
}