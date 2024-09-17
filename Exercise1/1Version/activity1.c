#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
	//create a first child process, count 1 to 1000
	pid_t pid1 = fork();
	if(pid1 == 0){
		for(int i=1;i<=1000;i++){
			printf("Number: %i \n",i);
		}
		exit(0);
	}
	
	pid_t pid2 = fork();
	int result;
	//create a second child process, list directories of /etc
	if(pid2 == 0){
		result = system("ls /etc");
		exit(0);
	}
	
	//Wait for the children process
	wait(NULL);
	wait(NULL);
	printf("Termino el programa\n");
	return 0;
}

