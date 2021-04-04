#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>

void* myfunc(void* arg)
{
    printf("hello, i am child thread , thid = %lu\n", pthread_self());
    return NULL;
}

int main(int argc, const char* argv[])
{
    pthread_attr_t arrt;
    pthread_attr_init(&arrt);
    pthread_attr_setdetachstate(&arrt, PTHREAD_CREATE_DETACHED);
    pthread_t thid;
    pthread_create(&thid, &arrt, myfunc, NULL);

    sleep(1);
    if(0 == pthread_join(thid, NULL))
    { 
        printf("pthread wait success!\n");
    }
    else
    { 
        printf("pthread wait failed!\n");
    }


    pthread_attr_destroy(&arrt);
     
    return 0;
}
