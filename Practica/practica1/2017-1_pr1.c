#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int main(void){
    pid_t pid;
    int status;

    if(waitpid(-1,&status,0) != -1){
        //printf("soy %d esperando a %d\n",getpid(),getppid());
        exit(0);
    }

    while(fork() + (pid = fork())){
        if(!pid){
            while(getppid() != 1);
            system("ps -l");
            exit(0);
        }
        execl("./2017-1_pr1","./2017-1_pr1",NULL);
    }
    system("ps -l");
    exit(7);
}