#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

//el pstree no muestra pipes
int main(int argc, char*argv[]){
	pid_t childpid;
	int fd[2];
	int nprocs;
	int i;
	if ((argc != 2) || ((nprocs = atoi (argv[1])) <= 0)) {
		fprintf(stderr, "Usage %s nprocs\n", argv[0]);
		return 1;
	}
	if (pipe (fd) == -1){
		perror("Failed to create starting pipe");
		return 1;
	}
	if ((dup2(fd[0], STDIN_FILENO) == -1) || (dup2(fd[1], STDOUT_FILENO) == -1) ){
		perror("Failed to connect pipe");
		return 1;
	}
	if ((close(fd[0]) == -1) || (close(fd[1]) == -1)){
		perror("Failed to close extra descriptors");
		return 1;
	}
	for (i = 1; i < nprocs; i++){
		if (pipe (fd) == -1){
			fprintf(stderr, "[%ld]:failed to create pipe %d: %s\n", (long)getpid(), i, strerror(errno));
			return 1;
		}
		if ((childpid = fork()) == -1){
			fprintf(stderr, "[%ld]:failed to create child %d: %s\n", (long)getpid(), i, strerror(errno));
		}
		if (childpid > 0)
			errno = dup2(fd[1], STDOUT_FILENO);
		else
			errno = dup2(fd[0], STDIN_FILENO);
		if (errno == -1){
			fprintf(stderr, "[%ld]:failed to dup pipes for iteration  %d: %s\n", (long)getpid(), i, strerror(errno));
		}
		if ((close(fd[0]) == -1) || (close(fd[1]) == -1)){
			fprintf(stderr, "[%ld]:failed to close extra descriptors %d: %s\n", (long)getpid(), i, strerror(errno));
			return 1;
		}
		if (childpid) break;
	}
	fprintf(stderr, "This is process %d with ID %ld and parent id %ld\n", i, (long)getpid(), (long)getppid());
	return 0;
}

