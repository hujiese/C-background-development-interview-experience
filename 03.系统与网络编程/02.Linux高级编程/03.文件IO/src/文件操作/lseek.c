#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, const char* argv[])
{
    int fd = open("copy", O_RDWR);
    if(fd == -1)
    {
        perror("open error");
        exit(1);
    }

    // 拓展文件
    int len = lseek(fd, 0, SEEK_END);
    printf(" file len = %d\n", len);

    lseek(fd, 1000, SEEK_END);
    write(fd, "a", 1);

    close(fd);
    return 0;
}
