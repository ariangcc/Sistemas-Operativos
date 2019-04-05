#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

#define MSGSIZE 64

int main(int argc, char *argv[ ]) {
    pid_t childpid; /* indicates process should spawn another */
    int error; /* return value from dup2 call */
    int fd[2]; /* file descriptors returned by pipe */
    int i; /* number of this process (starting with 1) */
    int nprocs; /* total number of processes in ring */

    int pid_padre = getpid();
    
    /* check command line for a valid number of processes to generate */
    if ( (argc != 2) || ((nprocs = atoi (argv[1])) <= 0) ) {
        fprintf (stderr, "Usage: %s nprocs\n", argv[0]);
        return 1;
    }

    if (pipe (fd) == -1) { /* connect std input to std output via a pipe */
        perror("Failed to create starting pipe");
        return 1;
    }
    
    if ((dup2(fd[0], STDIN_FILENO) == -1) || (dup2(fd[1], STDOUT_FILENO) == -1)) {
        perror("Failed to connect pipe");
        return 1;
    }

    
    if ((close(fd[0]) == -1) || (close(fd[1]) == -1)) {
        perror("Failed to close extra descriptors");
        return 1;
    }

    //
    char message[MSGSIZE];
    sprintf(message,"hola hijito");
    write(STDOUT_FILENO,message,MSGSIZE);            
    
    //pipe coloca su 0 y 1 en 3 y 4 siempre? o en los disponibles 
    //close(STDOUT_FILENO);

    for (i = 1; i < nprocs; i++) { /* create the remaining processes */
        if (pipe (fd) == -1) {
            fprintf(stderr, "[%ld]:failed to create pipe %d: %s\n",
            (long)getpid(), i, strerror(errno));
            return 1;
        }

        if ((childpid = fork()) == -1) {
            fprintf(stderr, "[%ld]:failed to create child %d: %s\n",(long)getpid(), i, strerror(errno));
            return 1;
        }

        if (childpid > 0){ /* for parent process, reassign stdout */
            error = dup2(fd[1], STDOUT_FILENO);
            
        }
        else{ /* for child process, reassign stdin */
            error = dup2(fd[0], STDIN_FILENO);
        }

        if (error == -1) {
            fprintf(stderr, "[%ld]:failed to dup pipes for iteration %d: %s\n",
            (long)getpid(), i, strerror(errno));
            return 1;
        }
        

        if ((close(fd[0]) == -1) || (close(fd[1]) == -1)) {
            fprintf(stderr, "[%ld]:failed to close extra descriptors %d: %s\n",(long)getpid(), i, strerror(errno));
            return 1;
        }

        if(childpid > 0){
            char messagePrev[MSGSIZE];
            read(STDIN_FILENO,messagePrev,MSGSIZE); 
            //close(STDIN_FILENO);
            
            write(STDOUT_FILENO,messagePrev,MSGSIZE);
            //close(STDOUT_FILENO);       
        }

        if(i == (nprocs-1) && childpid == 0){
            char messagePrev[MSGSIZE];
            read(STDIN_FILENO,messagePrev,MSGSIZE); 
            //close(STDIN_FILENO);
            
            write(STDOUT_FILENO,messagePrev,MSGSIZE);
            //close(STDOUT_FILENO);   
        }
 

        //el padre no hace más hijos, deja que el hijo tenga su hijo
        if (childpid)
            break;
    } /* say hello to the world */

    //el papu de los papus 
    if(pid_padre == getpid()){
        char messagePrev[MSGSIZE];
        read(STDIN_FILENO,messagePrev,MSGSIZE);
        fprintf(stderr,"El mensaje final es : %s\n",messagePrev);
        close(STDIN_FILENO);
    }
    fprintf(stderr, "This is process %d with ID %ld and parent id %ld\n",i, (long)getpid(), (long)getppid());
    return 0;
}