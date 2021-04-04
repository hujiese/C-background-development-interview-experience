#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	int fd = open("temp", O_RDWR | O_CREAT, 0664);
	if(fd == -1)
	{
		perror("open");
		exit(1);
	}

	// 复制文件描述符
	// int fd1 = dup(fd);
	int fd1 = fcntl(fd, F_DUPFD, 0);
	// 写文件
	char *p = "hello, world!!!";
	write(fd1, p, strlen(p)+1);
	close(fd1);

	// 读文件
	char buf[64]={0};
	lseek(fd, 0, SEEK_SET);
	read(fd, buf, sizeof(buf));
	printf("buf = %s\n", buf);
	close(fd);
}
