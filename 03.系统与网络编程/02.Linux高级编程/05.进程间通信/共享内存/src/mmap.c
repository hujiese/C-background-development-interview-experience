#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>


int main(int argc, const char* argv[])
{
    int fd = open("test.txt", O_RDWR);
    if(fd == -1)
    {
        perror("open error");
        exit(1);
    }

    // get file length
    // len > 0
    int len = lseek(fd, 0, SEEK_END);

    void * ptr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(ptr == MAP_FAILED)
    {
        perror("mmap error");
        exit(1);
    }
    close(fd);
 
    char buf[4096];
    // 从内存中读数据
    printf("buf = %s\n", (char*)ptr);
    strcpy(ptr, "yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyaaaaa");

    int ret = munmap(ptr, len);
    if(ret == -1)
    {
        perror("munmap error");
        exit(1);
    }

    return 0;
}
