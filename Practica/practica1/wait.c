#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <unistd.h>

int main(void){
    int wai = wait(NULL);
    printf("wait : %d",wai);
    return 0; 
}