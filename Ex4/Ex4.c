#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
void *helper(void *arg) {
	printf("HELPER\n");
	return NULL;
}
int main() {
	pthread_t thread[1000];
	
	clock_t start = clock();
	
	for(int i=0;i<1000;i++)
	{
		pthread_create(&thread[i], NULL, &helper, NULL);
	}
	clock_t end = clock();
	float seconds = (float)(end - start) / CLOCKS_PER_SEC;

	printf("%f", seconds);
	
	//pthread_yield();
	//printf("MAIN\n");
	return 0;
}
