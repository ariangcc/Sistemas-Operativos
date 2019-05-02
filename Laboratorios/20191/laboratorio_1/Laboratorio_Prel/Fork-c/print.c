#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

int main(void){
	
	//imprimiendo en base al proceso 
	printf("ID del proceso actual: %d\n",getpid());
	printf("ID del padre del proceso: %d\n",getppid());
	printf("ID del usuario propietario: %d\n",getuid());
	

	printf("\n");
	

	//demostrando 
	int pid; 
    pid = fork(); 
    if (pid == 0) 
    { 
		printf("me encuentro en el proceso hijo: \n");
		printf("pid exitoso: %d\n", pid);
        printf("id del proceso actual : %d\n" ,getpid());	
        printf("id del proceso padre : %d\n", getppid()); 
    } 
	else if(pid > 0){
		printf("me encuentro en el proceso padre: \n");
		printf("pid del hijo: %d\n", pid);
        printf("id del proceso actual : %d\n" ,getpid());		
        printf("id del proceso padre : %d\n", getppid()); 
	}	


	printf("\n");


	exit(0);
}

