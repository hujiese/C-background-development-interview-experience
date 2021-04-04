#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>

// 全局变量
int number;
pthread_rwlock_t rwlock;

void* write_func(void* arg)
{
    while(1)
    {
        pthread_rwlock_wrlock(&rwlock);
        number++;
        printf("+++ write number: %d\n", number);
        pthread_rwlock_unlock(&rwlock);
        usleep(1000);
    }
    return NULL;
}

void* read_func(void* arg)
{
    while(1)
    {
        pthread_rwlock_rdlock(&rwlock);
        printf("--- read number: %d\n", number);
        pthread_rwlock_unlock(&rwlock);
        usleep(500);
    }
    return NULL;
}

int main(int argc, const char* argv[])
{
    pthread_t thid[8];

    pthread_rwlock_init(&rwlock, NULL);

    // 创建线程
    int i;
    for(i=0; i<3; ++i)
    {
        pthread_create(&thid[i], NULL, write_func, NULL);
    }
    for(i=3; i<8; ++i)
    {
        pthread_create(&thid[i], NULL, read_func, NULL);
    }

    // 回收
    int j;
    for(j=0; j<8; ++j)
    {
        pthread_join(thid[j], NULL);
    }

    pthread_rwlock_destroy(&rwlock);

    return 0;
}
