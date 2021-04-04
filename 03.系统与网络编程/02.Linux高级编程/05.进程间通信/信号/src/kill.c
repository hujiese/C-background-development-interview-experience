#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>


int main(int argc, const char* argv[])
{
    pid_t pid = fork();
    if(pid == -1)
    {
        perror("fork error");
        exit(1);
    }

    if(pid > 0)
    {
        while(1)
        {
            printf("parent process pid = %d\n", getpid());
            sleep(1);
        }
    }
    else if(pid == 0)
    {
        sleep(2);
        // 弑父
        kill(getppid(), SIGKILL);
    }

    return 0;
}
