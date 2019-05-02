#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>

int main(void){
    int fd; 
    if((fd = open("salida", O_CREAT | O_RDWR, 0770))<0) perror("open failed"); 
    if(dup2(fd,1)<0) perror("dup2 failed");
    printf("hello world\n");
    //fflush(stdout);
    if(execlp("ls","ls","-l",(char*)NULL) < 0) perror("ecex failed");     
    //close(fd);
    exit(EXIT_SUCCESS);
}