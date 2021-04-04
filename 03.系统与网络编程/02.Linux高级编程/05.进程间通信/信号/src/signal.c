#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>

void myfunc(int sig)
{
    printf("cathc you signal: %d\n", sig);
}

int main(int argc, const char* argv[])
{
    // 注册信号捕捉函数
    signal(SIGINT, myfunc);

    while(1)
    {
        printf("hello...\n");
        sleep(1);
    }
    return 0;
}
