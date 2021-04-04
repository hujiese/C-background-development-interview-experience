#include <semaphore.h>
#include <sys/types.h>
#include <dirent.h>
#include <pthread.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <stdio.h>

pthread_once_t once = PTHREAD_ONCE_INIT;

void once_run(void)
{
    printf("once_run in thread %u\n", pthread_self());
}

void* task1(void* arg)
{
    int tid = pthread_self();
    printf("thread1 enter %u\n", tid);
    pthread_once(&once, once_run);
    printf("thread1 returns %u\n", tid);
}

void* task2(void* arg)
{
    int tid = pthread_self();
    printf("thread2 enter %u\n", tid);
    pthread_once(&once, once_run);
    printf("thread2 returns %u\n", tid);
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