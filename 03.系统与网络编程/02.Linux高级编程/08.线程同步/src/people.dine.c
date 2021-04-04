#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <pthread.h>

pthread_mutex_t mutex[5];

void* dine(void* arg)
{
    int num = (int)arg;
    int left, right;

    if(num < 4)
    {
        // 前４个人，右手拿自己筷子
        right = num;
        left = num+1;
    }
    else if(num == 4)
    {
        // 最后一个人，右手拿别人筷子
        right = 0;
        left = num;
    }

    // 吃饭
    while(1)
    {
        // 右手加锁
        pthread_mutex_lock(&mutex[right]);
        // 尝试抢左手筷子
        if(pthread_mutex_trylock(&mutex[left]) == 0)
        {
            // 吃面。。。
            printf("%c 正在吃面。。。。。。\n", num+'A');
            // 吃完放筷子
            pthread_mutex_unlock(&mutex[left]);
        }
        // 解锁
        pthread_mutex_unlock(&mutex[right]);
        sleep(rand()%5);
    }
}

int main(int argc, const char* argv[])
{
    pthread_t p[5];

    for(int i=0; i<5; ++i)
    {
        pthread_mutex_init(&mutex[i], NULL);
    }

    for(int i=0; i<5; ++i)
    {
        pthread_create(&p[i], NULL, dine, (void*)i);
    }

    for(int i=0; i<5; ++i)
    {
        pthread_join(p[i], NULL);
    }
    
    for(int i=0; i<5; ++i)
    {
        pthread_mutex_destroy(&mutex[i]);
    }
    
    return 0;
}
