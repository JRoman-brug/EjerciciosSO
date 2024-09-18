#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

//Semaphores
#include <semaphore.h>

sem_t semaphore;

int main(int argc, char **argv){
	//Manage file
	FILE* file = fopen("salida.txt","w");
	if(file == NULL){
			perror("Error open file");
			return 1;
	}
	
	char ch[100];
	char* aux;
	char empty[] = "";

	
	//params: struc sem_t, 
	//processShared (0 to shared with threads and nonz-zero shared with process),
	//initial value
	sem_init(&semaphore,1,1);
	fflush(file);
	pid_t pid1 = fork();
	if(pid1 == 0){
		int x=2,y=3,z=0;
		printf("Process 1\n");
		
		for(int i=0;i<100000;i++){
			sem_wait(&semaphore);
			z=x+y;
			z=x-y;
			z=x*y;
			z=x/y;
			
			fprintf(file,"Operaciones aritmeticas\n");	
			sem_post(&semaphore);
		}
		
		fclose(file);
		exit(0);
	}
	pid_t pid2 = fork();
	if(pid2 == 0){
		printf("Process 2\n");
		int result;
		
		for(int i=0;i<1000;i++){
			sem_wait(&semaphore);
			result = system("ls .");
			fprintf(file,"Estoy mirando el directorio actual\n");	
			sem_post(&semaphore);
		}
		
		
		fclose(file);
		exit(0);
	}	
	pid_t pid3 = fork();
	if(pid3 == 0){
		printf("Process 3\n");
		for(int i=0;i<20000;i++){
			sem_wait(&semaphore);
			fprintf(file,"El resultado es: %i\n",i);	
			sem_post(&semaphore);
		}
		
		fclose(file);
		exit(0);
	}
	
	printf("parent\n");
	wait(NULL);
	wait(NULL);
	wait(NULL);
	
	fprintf(file,"Termino de escribir \n");	
	fclose(file);
	sem_destroy(&semaphore);
	printf("termino de escribir\n");
	return 0;
}

