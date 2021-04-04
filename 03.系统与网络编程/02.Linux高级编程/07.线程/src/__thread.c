#include <semaphore.h>
#include <sys/types.h>
#include <dirent.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <stdio.h>

__thread int POD;

void* task1(void* arg)
{
	POD = 1;
    printf("thread1 POD %d\n", POD);
	sleep(1);
	printf("thread1 POD %d\n", POD);
}

void* task2(void* arg)
{
	POD = 2;
    printf("thread2 POD %d\n", POD);
	sleep(2);
	printf("thread2 POD %d\n", POD);
}

int main(int argc, char *argv[])
{
    pthread_t thrd1, thrd2;

    pthread_create(&thrd1, NULL, (void*)task1, NULL);
    pthread_create(&thrd2, NULL, (void*)task2, NULL);

    pthread_join(thrd1, NULL);
	pthread_join(thrd2, NULL);
    printf("Main thread exit...\n");

    return 0;
}