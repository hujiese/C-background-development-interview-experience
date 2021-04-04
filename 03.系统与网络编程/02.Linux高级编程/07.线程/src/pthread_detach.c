#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/types.h>

void* pthread_running(void *_val)
{ 
    printf("%s\n", (char*)_val);
    pthread_detach(pthread_self());
    //printf("%s\n", (char*)_val);
    return NULL;
}

int main()
{ 
    pthread_t tid;
    int ret = pthread_create(&tid, NULL, pthread_running, "main thread is running");

    if(ret != 0)
    { 
        printf("create thread error! info is:%s\n", strerror(ret));
        return ret;
    }

    int temp = 0;
    sleep(1);
    if(0 == pthread_join(tid, NULL))
    { 
        printf("pthread wait success!\n");
        temp = 0;
    }
    else
    { 
        printf("pthread wait failed!\n");
        temp = 1;
    }

    return temp;
}
