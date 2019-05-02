#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
int main(void){
	for(;;) printf("%d\n", getpid());
	fflush(stdout);
	return 0;
}
