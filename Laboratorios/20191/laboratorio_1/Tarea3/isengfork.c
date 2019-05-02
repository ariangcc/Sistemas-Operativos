#include <sys/types.h> // para usar pid_t 
#include <sys/wait.h> // wait() , waitpid()
#include <stdio.h> // printf()
#include <stdlib.h> //contiene a system
#include <unistd.h> //getpid

int main(void){
    int ii = 0; 

    printf("Pid primer padre: %d\n" , getpid());

    if(fork() == 0) ii++;     
    waitpid(-1,NULL,0);

    if(fork() == 0) ii++;     
    waitpid(-1,NULL,0);

    if(fork() == 0) ii++;     
    waitpid(-1,NULL,0);

    /*
    Tarea 3
    c) Haciendo uso de los datos proporcionados por el programa isengfork.c elabore un esquema que
        represente el árbol de procesos generados por el proceso padre.

        el analisis esta en el cuaderno rojo 

    d) En el programa isengfork.c cuál es la interpretación de su salida.
        El analisis para esta pregunta es muy parecido al de multifork, nuevamente se crean procesos hijos 
        los cuales van a avanzar en la ejecucion mientras los procesos padres se quedan a esperar, si un proceso
        padre llega a otra instruccion fork() tiene que crear nuevamente un hijo y esperar a que este termine 
        su ejecucion en return 0; 
    
    */
    printf("Pid actual : %d  Valor ii = %3.3d \n",getpid() ,ii);

    return 0;
}