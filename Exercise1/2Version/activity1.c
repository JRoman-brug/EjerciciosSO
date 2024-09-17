#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
	pid_t pid1 = fork();
	char* args[] = {"./child1",NULL};
	if(pid1 == 0){
		execv(args[0],args);
		printf("error");
	}
	
	pid_t pid2 = fork();
	if(pid2 == 0){
		execlp("ls","ls,","/etc",NULL);
	}
	
	wait(NULL);
	wait(NULL);
	printf("Proceso padre termino.\n");
	return 0;
}

