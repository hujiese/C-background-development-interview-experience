#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define MSG_TRY "try again\n"

// 非阻塞读终端
int main(void)
{
	char buf[10];
	int fd, n;
    // /dev/tty --> 当前打开的终端设备
	fd = open("/dev/tty", O_RDONLY | O_NONBLOCK);
	if(fd < 0) 
	{
		perror("open /dev/tty");
		exit(1);
	}
	tryagain:
	n = read(fd, buf, 10);
	if (n < 0) 
	{
        // 如果write为非阻塞，但是没有数据可读，此时全局变量 errno 被设置为 EAGAIN
		if (errno == EAGAIN) 
		{
			sleep(3);
			write(STDOUT_FILENO, MSG_TRY, strlen(MSG_TRY));
			goto tryagain;
		}
		perror("read /dev/tty");
		exit(1);
	}
	write(STDOUT_FILENO, buf, n);
	close(fd);
	return 0;
}
