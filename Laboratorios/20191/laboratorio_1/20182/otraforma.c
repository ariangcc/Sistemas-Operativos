#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#define MAX 100

    /*
     a)  la salida muestra un resumen de el uso de los recursos del programa, el resultado de programa lo envia a
     un 'agujero negro' mientras que el resumen szale por pantalla, esto se interpreta como: 

     el primer dato mostrado es el tiempoi desde el momento que se presiona enter hasta que se 
     completa la ejecucion de lo que sigue

     el segundo es la cantidad de tiempo de uso del CPU usado en modo usuario 

     el tercero es la cantidad de tiempo usado por la CPU en modo kernel
     
     b) codigo

     c) con time llegas a la conclusion xd
    */
int main(void){
    long i; 
    pid_t pid_padre = getpid();
    pid_t val;
    int nChild;
    long *arreglo; 
    arreglo = (long *)malloc(MAX*sizeof(long));
    srand(time(NULL));
    int fd[2];
    memset(arreglo,0,MAX);

    pipe(fd);
    
    for(nChild = 0; nChild < 4; nChild++)
        if((val = fork()) == 0) 
            break;
        else
            wait(NULL);  
    
    //para sacar el numero de elementos
    //sizeof(arr)/sizeof(arr[0])
    if(nChild == 0){
        for(i = 0; i < MAX; i++)
            arreglo[i] = rand() % 20;
            //tambien fun ciona con sizeof(arrreglo)
        write(fd[1],arreglo,sizeof(arreglo));     
    }
    else if(nChild == 1){
        read(fd[0],arreglo,MAX*sizeof(long));
        for(i = 0; i < MAX; i++)
            printf("%3li ",arreglo[i]);
        printf("\n");
        write(fd[1],arreglo,MAX*sizeof(long));        
    }
    else if(nChild == 2){
        read(fd[0],arreglo,MAX*sizeof(long));
        for(i = 0; i < MAX; i++)
            if(arreglo[i] % 2 == 0)
                arreglo[i] = 1;
            else 
                arreglo[i] = 0;
        write(fd[1],arreglo,MAX*sizeof(long));
    }
    else if(nChild == 3){
        read(fd[0],arreglo,MAX*sizeof(long));
        for(i = 0; i < MAX; i++)
            printf("%3li ",arreglo[i]);   
        printf("\n");
    }   

    return 0;
}