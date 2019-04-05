#include <stdio.h> // printf()
#include <sys/types.h> // para usar pid_t 
#include <sys/wait.h> // wait() , waitpid()
#include <unistd.h> //getpid
#include <stdlib.h> //contiene a system

int main(void){
    int i, status; 
    pid_t child; 

	//imprimiendo en base al proceso 
	printf("ID del proceso actual: %d\n",getpid());
	printf("ID del padre del proceso: %d\n",getppid());
	printf("ID del usuario propietario: %d\n",getuid());
	

	printf("\n");  

    //obviamente el primer proceso que sale es el mimo del de arriba 

    /*

    revisar analisis de codigo en cuaderno  
    basicamente crea un proceso hijo y el padre lo imprime 
    mientras que el hijo se queda en el bucle produciendo el 
    siguiente hijo, se imprimen los padres conforme pasa 
    los bucles, el ultimo en imprimirse sera el ultimo hijo. 


    */
    for(i = 0; i < 4; i++)
    {
        //si estoy en el proceso padre salgo a imprimirme
        //si estoy en proceso hijo me quedo a hacer mas hijos
        if(child = fork())
            break;
    }
    
    //si soy proceso padre o se acabó el bucle de creacion de hijos ando por aca
    fprintf(stderr,"Esta es la vuelta Nro %d\n", i);
    fprintf(stderr,"Recibi de fork el valor de %d\n", child);
    fprintf(stderr,"Soy el proceso %d con padre %d\n\n",getpid(),getppid());

    /*
    
    1. Modifique el programa chainprocesses.c para que se genere el archivo chainprocesses.txt que
        contendrá la salida del programa pstree mostrando el árbol de procesos creados por el proceso padre.

    hago que un proceso padre espere a que acabe su hijo, el cual se encuentra en el bucle creando mas 
    hijos, esto se repite para el hijo de su hijo al final el ultimo no tiene hijos que esperar, ese es el que 
    toma la foto el cual coincide con i=4 pues es el ultimo hijo de la cadena, por restriccion del bucle 
    
    */
    wait(&status);
    if(i == 4){
        char comando[100];
        sprintf(comando,"pstree -s %d > chainarbol.txt",getpid());
        system(comando);
    }

    return 0;
}