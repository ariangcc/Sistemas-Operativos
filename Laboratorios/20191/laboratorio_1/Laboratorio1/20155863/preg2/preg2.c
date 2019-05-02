#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#define MAXPROC 31367

int main(void){
    int num = 1;
    int i;
    int error;

    for(i = 0; i < MAXPROC; i++)
    {
        if((error = fork()) > 0){
            num++;
        }
        else if (error == 0){
            usleep(MAXPROC);
            exit(EXIT_SUCCESS);
        }
        else if(error < 0){
            break;
        }

    }
    printf("numero de procesos creados %d\n",num);
    
    for(i = 0; i < num; i++)   
        wait(NULL);
    
    /*
        el resultado es 10096 lo cual es distinto a lo que esta en la descripcion
        
    
    */
    
    return 0;
}