#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>


int main(int argc, const char* argv[])
{
    pid_t pid;

    int i=0;
    for(i=0; i<3; i++)
    {
        pid = fork();
    }
    
    printf("Proess %d is run ...\n", getpid());
    return 0;
}
