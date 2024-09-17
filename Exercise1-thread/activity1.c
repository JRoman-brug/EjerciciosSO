#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>


void* thread1(){
	int result = system("ls /etc");
	if(result == -1){
		perror("Error system");
	}
	return NULL;
}
void* thread2(){
	for(int i=1;i<=1000;i++){
		printf("Nro: %i\n",i);
	}
	return NULL;
}



int main(int argc, char **argv){
	pthread_t threadA1;
	int resultThread1 = pthread_create(&threadA1, NULL, thread1, NULL);
	if(resultThread1 != 0){
		perror("pthread_create");
		return 1;
	}
	
	pthread_t threadA2;
	int resultThread2 = pthread_create(&threadA2, NULL, thread2, NULL);
	if(resultThread2 != 0){
		perror("pthread_create");
		return 1;
	}
	
	if(pthread_join(threadA1,NULL) != 0){
		perror("pthread1_join");
		return 1;
	}
	
	if(pthread_join(threadA2,NULL) != 0){
		perror("pthread2_join");
		return 1;
	}
	
	return 0;
}

