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

    //　子进程执行程序
    if(pid == 0)
    {
        execlp("ps", "pssdfsdf", "aux", NULL);
        perror("execlp");
        exit(1);
    }

    for(int i=0; i<3; ++i)
    {
        printf(" i = %d\n", i);
    }
    sleep(1);

    return 0;
}
