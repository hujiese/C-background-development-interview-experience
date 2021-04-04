#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>

void* myfunc(void* arg)
{
    printf("child pthread id: %lu\n", pthread_self());
    printf("child thread .....\n");
    for(int i=0; i<5; ++i)
    {
        printf("child i = %d\n", i);
    }
    return NULL;
}

int main(int argc, const char* argv[])
{
    // 创建子线程
    pthread_t thid;
    // 返回错误号
    int ret = pthread_create(&thid, NULL, myfunc, NULL);
    if(ret != 0)
    {
        printf("error number: %d\n", ret);
        // 根据错误号打印错误信息
        printf("error information: %s\n", strerror(ret));
    }
    printf("parent pthread id: %lu\n", pthread_self());

    // 退出主线程，子线程不受影响
    pthread_exit(NULL);

    printf("parent thread .....\n");
    for(int i=0; i<3; ++i)
    {
        printf("i = %d\n", i);
    }

    return 0;
}
