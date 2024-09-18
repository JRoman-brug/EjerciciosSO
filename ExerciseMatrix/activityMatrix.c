#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <pthread.h>
#include <time.h>

#define MAX 20

int matrix1[MAX][MAX];
int matrix2[MAX][MAX];
int matrixRel[MAX][MAX];

typedef struct{
	int initRow;
	int initCol;
	int finalRow;
	int finalCol;
	int n;
}subMatrix_t;

void initializeParams(int n, subMatrix_t* params){
	int middle = (n/2);
	
	for(int i=0;i<2;i++){
		for(int j=0;j<2;j++){
			if(i==0){
				(*params).initCol = (i*middle);			
			}else{
				(*params).initCol = (i*middle)-1;			
			}	
			(*params).finalCol = ((i+1)*middle)-1;
			
			if(j==0){
				(*params).initRow = (j*middle);
			}else{
				(*params).initRow = (j*middle);
			}
			
			(*params).finalRow = ((j+1)*middle)-1;
			(*params).n = n;
			params++;
		}
	}
}

void initalizeMatrixZero(int n,int *matrix){
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			*((matrix+i*n)+j)=0;
		}
	}	
}

void initalizeMatrix(int n,int *matrix){
	int random;
	
	for(int i=0;i<n;i++){
		for(int j=0;j<n;j++){
			random = rand() % 5;
			*((matrix+i*n)+j)=random;
		}
	}	
}

void* subMatrix(void* arg){	
	subMatrix_t* param = (subMatrix_t*)arg;
	int initRow, initCol, finalRow ,finalCol, n;

	initRow = param->initRow; 
	initCol = param->initCol; 
	finalRow = param->finalRow;
	finalCol = param->finalCol;
	n = param->n;
	
	int elemMatrix1;
	int elemMatrix2;
	for(int i=initCol;i<=finalCol;i++){
		for(int j=initRow;j<=finalRow;j++){
			elemMatrix1 = matrix1[i][j];
			elemMatrix2 = matrix2[i][j];
			matrixRel[i][j] = elemMatrix1+elemMatrix2;
		}
	}

}
void printMatrix(int* matrix){
	for(int i=0;i<MAX;i++){
		for(int j=0;j<MAX;j++){
			printf("%i-",*(matrix++));
		}
		printf("\n");
	}
	printf("\n");
}
void printParams(subMatrix_t* params){
	for(int i=0;i<4;i++){
		printf("initRow: %i, finalRow %i, initCol %i, finalCol %i, n: %i\n",
			params->initRow,
			params->finalRow,
			params->initCol,
			params->finalCol,
			params->n
		);
		params++;
	}
}

void benchmark(){
	clock_t start_time = clock();
	
	int elemMatrix1;
	int elemMatrix2;
	for(int i=0;i<MAX;i++){
		for(int j=0;j<MAX;j++){
			elemMatrix1 = matrix1[i][j];
			elemMatrix2 = matrix2[i][j];
			matrixRel[i][j] = elemMatrix1+elemMatrix2;
		}
	}
	
	clock_t end_time = clock();
	double exec_time = (double)(end_time-start_time)/CLOCKS_PER_SEC*1000;
	printf("\n\n time with main process: %f\n",exec_time);
}
int main(int argc, char **argv){
	
	clock_t start_time = clock();
	
	initalizeMatrix(MAX,(int*)matrix1);
	initalizeMatrix(MAX,(int*)matrix2);
	initalizeMatrixZero(MAX,(int*)matrixRel);
	
	//-------------------------------------
	/*subMatrix_t paramEx;
	paramEx.matrix1 = (int*)matrix1;
	paramEx.matrix2 = (int*)matrix2;
	paramEx.matrixRel = (int*)matrixRel;
	paramEx.initRow = 0;
	paramEx.initCol = 0;
	paramEx.finalRow = 10;
	paramEx.finalCol = 10;
	paramEx.n = MAX;
	subMatrix(paramEx);*/
	//-------------------------------------
	
	
	subMatrix_t params[4];	
	initializeParams(MAX,params);
	//printParams(params);
	pthread_t threadMatrix[4];
	
	for(int i=0;i<4;i++){
		if(pthread_create(&threadMatrix[i], NULL, subMatrix,&params[i]) != 0){
			perror("pthead_create: \n");
			return 1;
		}
	}
	
	for(int i=0;i<4;i++){
		if(pthread_join(threadMatrix[i],NULL) != 0){
			perror("Fail ptherad_join: \n");
			return 1;
		}
	}
	
	clock_t end_time = clock();
	
	printMatrix((int*)matrix1);
	printMatrix((int*)matrix2);
	printMatrix((int*)matrixRel);
	double exec_time = (double)(end_time-start_time)/CLOCKS_PER_SEC*1000;
	printf("\n\n time with threads: %f\n",exec_time);
	
	
	initalizeMatrixZero(MAX,(int*)matrixRel);
	benchmark((int*)matrix1,(int*)matrix2,(int*)matrixRel);
	return 0;
}

