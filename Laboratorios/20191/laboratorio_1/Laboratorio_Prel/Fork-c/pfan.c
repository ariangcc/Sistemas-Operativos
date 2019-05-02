#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define	N 8
int main(void){
	int i, status;
	pid_t child,pid_padre;
	pid_padre=getpid();
	
	//los procesos se crean despues del for
	//for devuelve cosas
	//al padre devuelve el numero del hijo
	//y al hijo le devuelve 0
	//tambien puede devolver un error, con -1: por ejemplo cuando no hay espacio en la tabla
	for(i=0;i<N;++i)
		if((child=fork())<=0) break;
		else fprintf(stderr, "Ciclo Nro %d \n", i);
	fprintf(stderr, "Proceso con pid=%d y pid de padre= %d\n", getpid(), getppid());
	if(pid_padre==getpid()) for(i=0;i<N;++i) wait(&status);
	for(;;)
	return 0;	

}
