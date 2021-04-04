#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>

int main(int argc, const char* argv[])
{
    // 自定义信号集
    sigset_t myset, old;
    // 清空　－》　０
    sigemptyset(&myset);
    // 添加要阻塞的信号
    sigaddset(&myset, SIGINT);//ctrl + c
    sigaddset(&myset, SIGQUIT);//ctrl + \
    sigaddset(&myset, SIGKILL);

    // 自定义信号集设置到内核中的阻塞信号集
    sigprocmask(SIG_BLOCK, &myset, &old);

    sigset_t pend;
    while(1)
    {
        sleep(2);
        // 读内核中的未决信号集的状态
        sigpending(&pend);
        int i;
        for(i=1; i<32; ++i)
        {
            if(sigismember(&pend, i))
            {
                printf("1");
            }
            else if(sigismember(&pend, i) == 0)
            {
                printf("0");
            }
        }
        printf("\n");
    }

    return 0;
}
