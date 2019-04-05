#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>


int main(void){
    pid_t childpid;
    int fd[2];
    if ((pipe(fd) == -1) || ((childpid = fork()) == -1)) {
        perror("Failed to setup pipeline");
        return 1;
    }

    if (childpid == 0) { /* ls is the child */
        if (dup2(fd[1], STDOUT_FILENO) == -1)
            perror("Failed to redirect stdout of ls");
        else if ((close(fd[0]) == -1) || (close(fd[1]) == -1))
            perror("Failed to close extra pipe descriptors on ls");
        else {
            execl("/bin/ls", "ls", "-l", NULL);
            perror("Failed to exec ls");
        }
        return 1;
    }

    //lo mas probable es que la lectura se bloquee hasta que encuentre algo escrito
    if (dup2(fd[0], STDIN_FILENO) == -1) /* sort is the parent */
        perror("Failed to redirect stdin of sort");
    else if ((close(fd[0]) == -1) || (close(fd[1]) == -1))
        perror("Failed to close extra pipe file descriptors on sort");
    else {
        execlp("sort", "sort", "-n", "+4", NULL);
        perror("Failed to exec sort");
    }
    
    return 1;

}