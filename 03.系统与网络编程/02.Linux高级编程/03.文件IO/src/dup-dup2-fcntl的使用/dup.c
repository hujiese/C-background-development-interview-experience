#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
	int fd = open("temp", O_RDWR | O_CREAT, 0664);
	if(fd == -1)
	{
		perror("open");
		exit(1);
	}

	// 复制文件描述符
	int fd2 = dup(fd);
//    int fd2 = fcntl(fd, F_DUPFD);
	// 写文件
	char* p = "让编程改变世界。。。。。。";
	write(fd2, p, strlen(p));
	close(fd2);

	char buf[1024];
	lseek(fd, 0, SEEK_SET);
	read(fd, buf, sizeof(buf));
	printf(" buf = %s\n", buf);
	close(fd);

	return 0;
}
