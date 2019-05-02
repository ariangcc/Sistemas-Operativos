#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#define MAXFILESD 512


int main(void){
    int num = 0;
    int i;
    int fd[2];
    int error;
    for(i = 0; i < MAXFILESD; i++)
    {
        if((error = pipe(fd)) == -1) break;
        num++; num++;
    }
    printf("numero de pipes realizados %d\n",i);
    //se suma +3 debido a los descriptores standar
    printf("numero de directorios abiertos (fd) %d\n",num + 3);


/*
    haciendo la comparacion entre los limites mostrados 
    se concluye que efectivamente el numero maximo de  open files
    es 1023, pues contando con los tres descriptores referentes 
    a el estandar input y output y el error, se llega a dicho 
    numero. 

    esto es logico pues se entiende que los tres descriptores 
    standar estan activos al crear un nuevo proceso, por ende 
    al llegar al bucle 510 ya no tiene el 'espacio' suficiente
    para crear mas file directories entonces sale del bucle.  


    
*/
    //close(fd[0]);
    //close(fd[1]);

    return 0;
}