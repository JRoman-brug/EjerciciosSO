#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv)
{
	for(int i=1;i<=1000;i++){
		printf("Nro: %i\n",i);
	}
	return 0;
}

