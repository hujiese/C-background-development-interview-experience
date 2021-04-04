#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>


int main(int argc, char *argv[])
{
	int fd = open("temp", O_RDWR | O_CREAT, 0664);

	void* ptr = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(ptr == MAP_FAILED)
	{
		perror("mmap");
		exit(1);
	}

	while(1)
	{
        char*p = (char*)ptr;
        p += 1024;
		strcpy(p, "hello parent, i am your 朋友！！！\n");
		sleep(2);
	}

	// 释放
	int ret = munmap(ptr, 8192);
	if(ret == -1)
	{
		perror("munmap");
		exit(1);
	}

	return 0;
}

