#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>


int main(int argc, const char* argv[])
{
    pid_t pid = fork();

    // 父进程
    if(pid > 0)
    {
        printf("parent process, pid = %d, ppid = %d\n", getpid(), getppid());
        while(1)
        {
            printf(" i am live\n");
            sleep(1);
        }
    }
    // 子进程
    else if(pid == 0)
    {
        printf("child process, pid = %d, ppid = %d\n", getpid(), getppid());
    }

    for(int i=0; i<5; ++i)
    {
        printf(" i = %d\n", i);
    }

    return 0;
}
