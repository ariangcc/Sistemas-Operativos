#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

int main(void){
    int fd[2]; 
    int n = 2; int m;
    pipe(fd); 

    write(fd[1],&n,sizeof(int));

    read(fd[0],&m,sizeof(int));
    
    printf("m : %d  n: %d", m,n);
    return 0;
}