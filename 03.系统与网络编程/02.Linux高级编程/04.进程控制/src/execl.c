#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>


int main(int argc, const char* argv[])
{
    printf("hello, world\n");

    pid_t pid = fork();
    if(pid == -1)
    {
        perror("fork error");
        exit(1);
    }
    if(pid >0)
    {
        sleep(1);
    }
    //　子进程执行程序
    if(pid == 0)
    {
        execl("./hello", "xxxx",  NULL);
        //execl("/home/kevin/hello", "xxxx",  NULL);
        perror("execl");
        exit(1);
    }

    for(int i=0; i<3; ++i)
    {
        printf(" i = %d\n", i);
    }

    return 0;
}
