#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>

void* myfunc(void* arg)
{
    int num = *(int*)arg;
    printf("%dth child pthread id: %lu\n", num, pthread_self());
    return NULL;
}

int main(int argc, const char* argv[])
{
    // 创建子线程
    pthread_t thid[5];
    // 返回错误号
    for(int i=0; i<5; ++i)
    {
        int ret = pthread_create(&thid[i], NULL, myfunc, (void*)&i);
        if(ret != 0)
        {
            printf("error number: %d\n", ret);
            // 根据错误号打印错误信息
            printf("error information: %s\n", strerror(ret));
        }
    }
    printf("parent pthread id: %lu\n", pthread_self());

    sleep(1);

    return 0;
}
