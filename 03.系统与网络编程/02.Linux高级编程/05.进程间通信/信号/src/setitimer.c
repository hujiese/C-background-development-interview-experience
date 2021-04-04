#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/time.h>
#include <signal.h>

void myfunc(int sig)
{
    printf("hello\n");
}

int main(int argc, const char* argv[])
{
    struct itimerval new_value;
    new_value.it_interval.tv_sec = 2;
    new_value.it_interval.tv_usec = 0;

    new_value.it_value.tv_sec = 2;
    new_value.it_value.tv_usec = 0;

    signal(SIGALRM, myfunc);
    setitimer(ITIMER_REAL, &new_value, NULL);
    while(1)
    {
    }
    return 0;
}
