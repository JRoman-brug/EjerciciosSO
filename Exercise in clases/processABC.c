#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

//Semaphores
#include <semaphore.h>

#define MAX 5

sem_t semA,semB,semC;
int main(int argc, char **argv){
	//Initilize semaphores
	sem_init(&semA,1,0);
	sem_init(&semB,1,0);
	sem_init(&semC,1,0);
	
	//Process A
	pid_t processA = fork();
	if(processA == 0){
		for(int i=0;i<MAX;i++){
			sem_wait(&semB);
			printf("A\n");
			sem_post(&semA);
		}
	}
	//Process B
	pid_t processB = fork();
	if(processB == 0){
		for(int i=0;i<MAX;i++){
			sem_wait(&semB);
			printf("B\n");
			sem_post(&semA);
		}
	}
	//Process C
	pid_t processC = fork();
	if(0){
		for(int i=0;i<MAX;i++){
			sem_wait(&semC);
			printf("C\n");
			sem_post(&semA);
		}
	}
	
	wait(NULL);
	wait(NULL);
	wait(NULL);
	sem_destroy(&semA);
	sem_destroy(&semB);
	sem_destroy(&semC);
	return 0;
}

