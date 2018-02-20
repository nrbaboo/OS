#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include <semaphore.h>

pthread_t thread_id[2];
int counter;
sem_t s;
//pthread_mutex_t lock;

void* doThing(void *arg)
{
	//pthread_mutex_lock(&lock);
	sem_wait(&s);
	
unsigned long i = 0;
	counter += 1;
	printf("\n Job %d started\n", counter);
	for(i=0; i<(0xFFFFFFFF);i++);
	printf("\n Job %d finished\n", counter);
	
	//pthread_mutex_unlock(&lock);
	sem_post(&s);

	return NULL;
}
int main(void)
{
	int i = 0;
	int err;
	/*if (pthread_mutex_init(&lock, NULL) != 0)
    	{
        	printf("\n mutex init failed\n");
        	return 1;
    	}*/
	if ( sem_init(&s, 0, 1) != 0 )
	{
		// Error: initialization failed
	}
	while(i < 2)
	{
		err = pthread_create(&(thread_id[i]), NULL, &doThing, NULL);
		if (err != 0)
			printf("\ncan't create thread :[%s]", strerror(err));
		i++;
	}
	
	pthread_join(thread_id[0], NULL);
	pthread_join(thread_id[1], NULL);
	//pthread_mutex_destroy(&lock);
	return 0;
}
