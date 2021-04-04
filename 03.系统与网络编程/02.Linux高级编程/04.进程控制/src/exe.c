#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>


int main(int argc, const char* argv[])
{
    
    //父进程数
    int i;
    for(i = 0; i < 5;i++)
    {
        printf("parten i = %d\n", i);
    }
    pid_t pid = fork();
    if(pid == -1)
    {
        perror("fork error");
        exit(1);
    }

    //　子进程执行程序
    if(pid == 0)
    {
        execl("/bin/ls", "666", "-lah" ,NULL);
    }

    int j;
    for(j=0; j<3; ++j)
    {
        printf(" j = %d\n", j);
    }

    return 0;
}
