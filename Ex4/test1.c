#include<stdio.h>
#include<pthread.h>
#include<stdlib.h>

void thread1fn(void *arg) { 
int i=0;
	while(1)
	{
		if(i%10000000==0)
		{
			printf(".");
                        i = 0;
		}
		i++;
	}
	return NULL;
}

void thread2fn(void *arg) { 

	while(1){
		char ch[1000];
		scanf("%s",&ch);
		printf("Thank you for your input.\n");
	}
}
int main() {
	pthread_t thread1;
	pthread_t thread2;
	pthread_create(&thread1, NULL, &thread1fn,NULL);
	pthread_create(&thread2, NULL, &thread2fn,NULL);
	pthread_join(thread1, NULL);
    	pthread_join(thread2, NULL); 
	return 0;
}
