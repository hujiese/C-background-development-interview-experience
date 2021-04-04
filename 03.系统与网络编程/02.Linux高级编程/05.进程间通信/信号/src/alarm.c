#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>


int main(int argc, const char* argv[])
{
    int ret = alarm(5);
    printf("ret = %d\n", ret);

    sleep(2);

    ret = alarm(2);
    printf("ret = %d\n", ret);

    while(1)
    {
        printf("hello\n");
        sleep(1);
    }

    return 0;
}
