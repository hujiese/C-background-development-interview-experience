#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, const char* argv[])
{
    int fd = open("english.txt", O_RDONLY);
    if(fd == -1)
    {
        perror("open error");
        exit(1);
    }

    // 读文件
    char buf[1024];
    int len = read(fd, buf, sizeof(buf));

    // 写文件
    int fd1 = open("temp", O_WRONLY | O_CREAT, 0664);
    if(fd1 == -1)
    {
        perror("open error");
        exit(1);
    }

    while( len > 0 )
    {
        // 写文件
        int ret = write(fd1, buf, len);
        len = read(fd, buf, sizeof(buf));
    }
    close(fd);
    close(fd1);

    return 0;
}
