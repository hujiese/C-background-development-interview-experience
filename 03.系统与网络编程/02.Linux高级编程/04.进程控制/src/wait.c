#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>


int main(int argc, const char* argv[])
{
    pid_t pid = fork();

    // 父进程
    if(pid > 0)
    {
        printf("parent process, pid = %d, ppid = %d\n", getpid(), getppid());
        // 回收资源
        int status;
        pid_t wpid = wait(&status);
        printf("child died pid = %d\n", wpid);
        // 通过返回值退出
        if(WIFEXITED(status))
        {
            printf("child process exit value: %d\n", WEXITSTATUS(status));
        }
        // 子进程被信号杀死
        else if(WIFSIGNALED(status))
        {
            printf("child process died by signal: %d\n", WTERMSIG(status));
        }

    }
    // 子进程
    else if(pid == 0)
    {
        printf("child process, pid = %d, ppid = %d\n", getpid(), getppid());
        sleep(200);
    }

    for(int i=0; i<5; ++i)
    {
        printf(" i = %d\n", i);
    }

//    return 10;
    exit(10);
}
