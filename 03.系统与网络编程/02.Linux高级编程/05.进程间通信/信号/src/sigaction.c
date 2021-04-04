#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>

void myfunc(int sig)
{
    printf("hello signal: %d\n", sig);
    //sleep(5);
    //printf("wake up .....\n");
}

int main(int argc, const char* argv[])
{
    // 注册信号捕捉函数
    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = myfunc;
    // 设置临时屏蔽的信号
    sigemptyset(&act.sa_mask);  // 0
    // ctrl + 反斜杠
    sigaddset(&act.sa_mask, SIGQUIT);
    sigprocmask(SIG_BLOCK, &act.sa_mask, NULL);
    sigaction(SIGINT, &act, NULL);

    while(1);

    return 0;
}
