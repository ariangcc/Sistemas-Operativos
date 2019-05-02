#define BUFSIZE1 10
#define BUFSIZE2 13
#define WLOOP 5

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/wait.h>

int main(void){

    int fd[2];
    pid_t haschild;

    pipe(fd); /* pipe a */
    dup2(fd[0], STDIN_FILENO);
    dup2(fd[1], STDOUT_FILENO);

    close(fd[0]);
    close(fd[1]);

    pipe(fd); /* pipe b */
    haschild = fork();

    if (haschild > 0)
        dup2(fd[1], STDOUT_FILENO); /* parent(A) redirects std output */
    else if (!haschild)
        dup2(fd[0], STDIN_FILENO); /* child(B) redirects std input */

    close(fd[0]);
    close(fd[1]);

    /*hasta este punto basicamente esta : 
        padre lee de pipe 1 escribe en pipe 2 
        hijo lee de pipe 2 escribe en pipe 1 
    */

    //luego de tener las cañerias unidireccionales 
    char message1[] = "Hello son";
    char message2[] = "Hello father";

    char respuesta1[BUFSIZE2];
    char respuesta2[BUFSIZE1];
    if (haschild > 0){
        int wpipe, rpipe;
        
   
        
        wpipe = write(STDOUT_FILENO,message1,10);
            
        
        
        //close(STDOUT_FILENO);
        
        //wait(NULL);
        //rpipe = read(STDIN_FILENO,respuesta1,13); 
        //close(STDIN_FILENO);
        

        fprintf(stderr,"respuesta de mi hijo: %s\n",respuesta1);
        fprintf(stderr,"estado lectura del pipe 2 por parte del hijo: %d\n",rpipe);
        fprintf(stderr,"estado escritura del pipe 2 por parte del padre: %d\n",wpipe);
    }
    else{
        int wpipe, rpipe;
        //wpipe = write(STDOUT_FILENO,message2,13);
        //close(STDOUT_FILENO);
        
        //rpipe = read(STDIN_FILENO,respuesta2,10);
        close(STDIN_FILENO);
        
        fprintf(stderr,"respuesta de mi padre: %s\n",respuesta2);
        fprintf(stderr,"estado lectura del pipe 2 por parte del hijo: %d\n",rpipe);
        fprintf(stderr,"estado escritura del pipe 2 por parte del hijo: %d\n",wpipe);
    }

    return 0;     
}