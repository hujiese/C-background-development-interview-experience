#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, const char* argv[])
{
    int fd[2];
    int ret  = pipe(fd);
    if(ret == -1)
    {
        perror("pipe error");
        exit(1);
    }

    printf("fd[0] = %d\n", fd[0]);
    printf("fd[1] = %d\n", fd[1]);

    //pid_t pid = fork();
    close(fd[0]);
    close(fd[1]);

    return 0;
}
