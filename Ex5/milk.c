#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>

/* Compile like this:
gcc --std=c99 -lpthread cond.c -o cond
*/
const size_t NUMTHREADS = 5;

/* a global count of the number of threads finished working. It will
be protected by mutex and changes to it will be signalled to the
main thread via cond */
int milk = 1;
int r;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* drinkMilkAndBuyIfNeed(void* id)
{
	const int myid = (long)id;

	while(1) {
		pthread_mutex_lock( &mutex );
		r = rand() % 5;
		if(r == myid) {
			milk--;
			printf("Drink milk by roommate[%d]\n", myid);
		
			printf("Remaining bottles of milk: %d\n", milk);
			sleep(1);

			if (milk == 0) {
				r = rand() % 5 +2;
				milk += r;
				printf("Buy %d bottles of milk by roommate[%d]\n",r,myid);
				printf("Remaining bottles of milk: %d\n", milk);
				sleep(1);
			}
		}
		else {
			printf("Do nothing by roommate[%d], rand = %d\n", myid, r);
			sleep(1);
		}
		pthread_mutex_unlock( & mutex );
		sleep(1);
	}
}

int main( int argc, char** argv )
{
	srand(time(NULL));

	puts( "[thread main] starting" );
	pthread_t threads[NUMTHREADS];
	for( int t=0; t<NUMTHREADS; t++ ){
		pthread_create( &threads[t], NULL, drinkMilkAndBuyIfNeed, (void*)(long)t );
	}
	// we're going to test "done" so we need the mutex for safety
	// are the other threads still busy?
	while( 1 )
	{
		
	}
	printf( "[thread main] done == %d so everyone is done\n", (int)NUMTHREADS);

	return 0;
}
