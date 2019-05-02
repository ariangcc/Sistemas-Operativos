#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

/* Este programa crea procesos hijos en forma horizontal.          */
/* Es decir el padre crea N hijos.                                 */
/* Ejm 2.6 del libro UNIX Programacion Practica - Kay Robbins      */
/*                                                Steve Robbins    */
/* Modificado por Alejandro Bello Ruiz - Informática PUCP          */

#define   N    4

int main(void)
{
  int i,status;
  pid_t child,pid_padre;
  

  pid_padre=getpid(); 
  for(i=0;i<N; ++i)
     if((child=fork())<=0) break;
     else fprintf(stderr,"Ciclo Nro %d \n",i);
  fprintf(stderr,"Proceso con pid=%d y pid de padre= %d\n",getpid(),getppid());
  if(pid_padre==getpid()) for(i=0;i<N;++i) wait(&status);
  return 0;
}

