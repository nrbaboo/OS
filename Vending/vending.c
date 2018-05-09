#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

//define number of supplier and customer
#define CUSTOMER_NUM 8
#define SUPPLIER_NUM 5

//define goods
int goods[SUPPLIER_NUM];


//initial pthread mutex and condition
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

//function to return timestamp 
char *time_stamp()
{
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    static char s[64];
    strftime(s, sizeof(s), "%c", tm);
    //printf("%s\n", s);
    return s;
}

void *customer_thread(void *id)
{

    /********************************/
    /*          READ FILE           */
    /********************************/

    //get value
    const int myid = (long)id;
    const int myNumber = myid+1;
    const int myProduct = myNumber-1;

    //value from file
    char name[500];
    int interval = 0;
    int repeat = 0;
    int currentRepeat = 0;
    int currentInterval = 0;

    //concate string to make configuration file name
    char fileName[100];
    sprintf(fileName, "customer%d.txt", myNumber); // puts string into buffer

    //read file
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int state = 0;

    fp = fopen(fileName, "r");
    if (fp == NULL){
        printf("EXIT_FAILURE");
        exit(EXIT_FAILURE);
    }
        

    while ((read = getline(&line, &len, fp)) != -1)
    {

        /*  because fgets() get '\n' in to string.so that  
            we need to remove it */
        if (line[strlen(line) - 1] == '\n')
            line[strlen(line) - 1] = '\0';

        switch (state)
        {
        case 0:
            strcpy(name, line);
            break;
        case 1:
            interval = atoi(line);
            break;
        case 2:
            repeat = atoi(line);
            break;
        default:
            break;
        }
        state++;
    }

    //close FILE and free
    fclose(fp);
    if (line)
        free(line);

    //printf("Customer thread :%d thread number :%d\n",myid,myNumber);
    //printf("Customer thread :%2d thread number :%2d variable  name:%s interval:%d repeate:%d\n", myid, myNumber, name, interval, repeat);


    /********************************/
    /*          OPERATION           */
    /********************************/

    //setup
    currentInterval=interval;
    currentRepeat=0;

    while(1){
        //printf("in while thread id:%d\n",myid);

        sleep(currentInterval);
        //usleep(currentInterval*1000000000);

        pthread_mutex_lock( &mutex );

        if(goods[myProduct]>0){
            goods[myProduct]--;
            printf("%s %s consumed 1 unit. stock after = %d\n",time_stamp(),name,goods[myProduct]);
            currentInterval=interval;
            currentRepeat=0;
        }else{
            printf("%s %s consumer going to wait.\n",time_stamp(),name);
            currentRepeat++;
            if(currentRepeat==repeat){
                currentRepeat=0;
                currentInterval*=2;
                if(currentInterval>60)
                    currentInterval=60;
            }
        }

        pthread_mutex_unlock( & mutex );
    }
}

void *supplier_thread(void *id)
{
    /********************************/
    /*          READ FILE           */
    /********************************/

    //get value
    const int myid = (long)id;
    const int myNumber = myid-CUSTOMER_NUM+1;
    const int myProduct = myNumber-1;

    //value from file
    char name[500];
    int interval = 0;
    int repeat = 0;
    int currentRepeat = 0;
    int currentInterval = 0;

    //concate string to make configuration file name
    char fileName[100];
    sprintf(fileName, "supplier%d.txt", myNumber); // puts string into buffer
    //printf("\n\n--> %d\n",myNumber);

    //read file
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    int state = 0;

    fp = fopen(fileName, "r");
    if (fp == NULL){
        printf("EXIT_FAILER");
        exit(EXIT_FAILURE);
    }
        

    while ((read = getline(&line, &len, fp)) != -1)
    {

        /*  because fgets() get '\n' in to string.so that  
            we need to remove it */
        if (line[strlen(line) - 1] == '\n')
            line[strlen(line) - 1] = '\0';

        switch (state)
        {
        case 0:
            strcpy(name, line);
            break;
        case 1:
            interval = atoi(line);
            break;
        case 2:
            repeat = atoi(line);
            break;
        default:
            break;
        }
        state++;
    }

    //close FILE and free
    fclose(fp);
    if (line)
        free(line);
    //exit(EXIT_SUCCESS);

    //printf("Supplier thread :%d thread number :%d\n",myid,myNumber);
    //printf("Supplier thread :%2d thread number :%2d variable  name:%s interval:%d repeate:%d\n", myid, myNumber, name, interval, repeat);

    /********************************/
    /*          OPERATION           */
    /********************************/

    //setup
    currentInterval=interval;
    currentRepeat=0;

    while(1){
        //printf("in while thread id:%d\n",myid);

        sleep(currentInterval);
        //usleep(currentInterval*1000000000);

        pthread_mutex_lock( &mutex );

        if(goods[myProduct]<100){
            goods[myProduct]++;
            printf("%s %s supplied 1 unit. stock after = %d\n",time_stamp(),name,goods[myProduct]);
            currentInterval=interval;
            currentRepeat=0;
        }else{
            printf("%s %s supplier going to wait.\n",time_stamp(),name);
            currentRepeat++;
            if(currentRepeat==repeat){
                currentRepeat=0;
                currentInterval*=2;
                if(currentInterval>60)
                    currentInterval=60;
            }
        }

        pthread_mutex_unlock( & mutex );
    }
}

int main(int argc, char **argv)
{
    printf("[thread main] starting\n");

    //test time stamp
    printf(" Timestamp: %s\n",time_stamp());

    //init all goods to be 10
    for (int i = 0; i < SUPPLIER_NUM; i++)
    {
        goods[i] = 1;
        //printf("good: %d have: %d\n", i, goods[i]);
    }

    //init thread pool
    int thread_num = CUSTOMER_NUM + SUPPLIER_NUM;
    //printf("thread_num :%d\n", thread_num);
    pthread_t threads[thread_num];

    //init threads for customer and supplier
    for (int i = 0; i < thread_num; i++)
    {

        //create customer and supplier
        if (i < CUSTOMER_NUM)
        {   
            pthread_create( &threads[i], NULL, customer_thread, (void*)(long)i );
        }
        else
        {
            pthread_create( &threads[i], NULL, supplier_thread, (void*)(long)i );
        }
        
    }

    while(1){
    }
}