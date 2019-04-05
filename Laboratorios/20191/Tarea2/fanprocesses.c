#include <stdio.h> // printf()
#include <sys/types.h> // para usar pid_t 
#include <sys/wait.h> // wait() , waitpid()
#include <unistd.h> //getpid
#include <stdlib.h> //contiene a system

int main(void){

    int i, status; 
    pid_t child, pid_padre; 

    pid_padre = getpid();

    for(i = 1; i < 4; i++)
    {
        //si estoy en el proceso hijo o hay error salgo
        if((child = fork()) <= 0) break;
        //si estoy en el proceso padre
        else{
            fprintf(stderr,"Este es el ciclo Nro %d soy el proceso %d y se esta creando el proceso %d\n",i,getpid(),child);
        }
    }  


    /*
    2.  Modifique el programa fanprocesses.c para que se genere el archivo fanprocesses.txt que
        contendrá la salida del programa pstree mostrando el árbol de procesos creados por el proceso padre.


    recordar que cuando se realiza el fork y se crea un proceso hijo,el padre e hijo se ejecutan simultaneamente  
    
    dicho esto una vez creados los hijos, estos y el padre se encuentran solicitando atencion del kernel 
    por ello  si soy el padre y ya estan creados todos los hijos solo tengo que imprimir donde estoy y luego esperar 
    a que los procesos acaben o simplemente dejar que continuen su ejecucion para acabar!
    
    */
 
    //si me encuentro en el proceso padre 
    if(pid_padre == getpid()){
        
        char comando[100];
        sprintf(comando,"pstree -s %d > fanarbol.txt",getpid());
        system(comando);

        //espero a que acaben los hijos que tengo para continuar
        for(i = 1; i < 4; i++)
            wait(&status);
    }

    //se imprimen los hijos conforme se van creando 
    fprintf(stderr, "Este es el proceso %d con padre %d\n",getpid(),getppid());


    return 0; 
}