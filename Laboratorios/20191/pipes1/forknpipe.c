#define BUFSIZE 64 
#define WLOOP 5

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/wait.h>

int main(void){
    char buffer[BUFSIZE];
    char message[] = "Hello\n";
    int i , fd[2];

    pipe(fd); 
    if(fork() == 0){
        close(fd[0]);
        printf("I am PID[%d] (child) fd[0]: %d fd[1] : %d\n", (int)getpid(),fd[0],fd[1]);
        for(i = 0; i < WLOOP; i++)
        {
            write(fd[1],message,sizeof(message) - 1);
        }
        close(fd[1]);
    }
    else{
        close(fd[1]);
        printf("I am PID[%d] (parent) fd[0]: %d fd[1] : %d\n", (int)getpid(),fd[0],fd[1]);
        memset(buffer,0,sizeof(buffer));
        while((i=read(fd[0],buffer,BUFSIZE-1)) != 0){
            printf("PARENT READ [%d]: \n%s<-\n",(int)i, buffer);
            memset(buffer,0,sizeof(buffer));
        }
        close(fd[0]);
    }
    return 0;
}