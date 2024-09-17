#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <pthread.h>

//Semaphores
#include <semaphore.h>

sem_t semaphore;
FILE* file;


//Thread 1
void* thread1(){
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
	return NULL;
}
//Thread 2
void* thread2(){
	int result;
	
	for(int i=0;i<1000;i++){
		sem_wait(&semaphore);
		result = system("ls .");
		fprintf(file,"Estoy mirando el directorio actual\n");	
		sem_post(&semaphore);
	}
	
	return NULL;
}
//Thread 3
void* thread3(){
	for(int i=0;i<20000;i++){
		sem_wait(&semaphore);
		fprintf(file,"El resultado es: %i\n",i);	
		sem_post(&semaphore);
	}
	return NULL;
}


int main(int argc, char **argv){
	//Manage file
	file = fopen("salida.txt","w");
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
	sem_init(&semaphore,0,1);
	pthread_t threadA1,threadA2,threadA3;	
	if(pthread_create(&threadA1,NULL, thread1,NULL) !=0){
		perror("pthread_create thread1");
		return 1;
	}

	if(pthread_create(&threadA2,NULL, thread2,NULL) !=0){
		perror("pthread_create thread2");
		return 1;
	}
	
	if(pthread_create(&threadA3,NULL, thread3,NULL) !=0){
		perror("pthread_create thread3");
		return 1;
	}
	
	
	if(pthread_join(threadA1,NULL) != 0){
		perror("pthread_join thread1");
		return 1;
	}
	if(pthread_join(threadA2,NULL) != 0){
		perror("pthread_join thread2");
		return 1;
	}
	if(pthread_join(threadA3,NULL) != 0){
		perror("pthread_join thread2");
		return 1;
	}
	fclose(file);
	sem_destroy(&semaphore);
	printf("termino de escribir\n");
	return 0;
}

