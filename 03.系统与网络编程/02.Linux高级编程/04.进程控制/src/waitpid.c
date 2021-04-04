#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, const char* argv[])
{
    pid_t pid;

    for(int i=0; i<10; ++i)
    {
        pid = fork();
        if(pid == 0)
        {
            break;
        }
    }

    if(pid > 0)
    {
        // 父进程
        printf("parent process, pid = %d, ppid = %d\n", getpid(), getppid());
        // 回收子进程
        pid_t wpid;
        int status;
        while( ( wpid = waitpid(-1, &status, WNOHANG) ) != -1 )
        {
            if(wpid == 0)
                continue;
            printf("died process pid = %d\n", wpid);
            // 判断文件退出方式
            if(WIFEXITED(status))
            {
                printf("------ exit value: %d\n", WEXITSTATUS(status));
            }
            if(WIFSIGNALED(status))
            {
                printf("=======exit by signal: %d\n", WTERMSIG(status));
            }
        }
    }
    else
    {
        // 子进程
        printf("child process, pid = %d, ppid = %d\n", getpid(), getppid());
    }
    return 9;
}
