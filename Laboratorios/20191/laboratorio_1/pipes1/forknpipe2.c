#define BUFSIZE 64 

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/wait.h>

int main(void){
    char  buffer1[BUFSIZE],buffer2[BUFSIZE];
    int   p_this, p_prev, p_no1, p_no2;
    int   fd_pipe[4][2], ii, jj;
    pid_t mypid;
    
    memset(buffer1, 0, BUFSIZE);
    memset(buffer2, 0, BUFSIZE);
    
    for (ii=0;ii<4;ii++){
        pipe(fd_pipe[ii]);
    }
    
    ii = (fork() != 0 ) ? 0 : 2;
    jj = (fork() != 0 ) ? 0 : 1;
    
    p_this = ii + jj;
    close(fd_pipe[p_this][0]);
    
    p_prev = (p_this + 3) % 4;
    close(fd_pipe[p_prev][1]);
        
    p_no1  = (p_this + 1) % 4;
    close(fd_pipe[p_no1][0]);
    close(fd_pipe[p_no1][1]);
    
    p_no2  = (p_this + 2) % 4;
    close(fd_pipe[p_no2][0]);
    close(fd_pipe[p_no2][1]);
    
    mypid  = getpid();
    //mensaje para el siguiente en la lista , que pasa si write BUFSIZE nada mas
    sprintf(buffer1,"A message from PID[%d] with PPID[%d].\n", (int) mypid, getppid());
    write(fd_pipe[p_this][1], buffer1, BUFSIZE-1);
    close(fd_pipe[p_this][1]);
    //no se como explicar porqué el primer hijo no espera a su hijo
    while ((read(fd_pipe[p_prev][0], buffer2,BUFSIZE-1)) != 0) {
        waitpid(1,NULL,0);
        printf("PID[%d] with PPID[%d] IS WAITING:\n%s\n", (int) mypid,getppid(), buffer2);
    }
    close(fd_pipe[p_prev][0]);
    
    exit(0);
}