#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define MSG_TRY "try again\n"

// ���������ն�
int main(void)
{
	char buf[10];
	int fd, n;
    // /dev/tty --> ��ǰ�򿪵��ն��豸
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
        // ���writeΪ������������û�����ݿɶ�����ʱȫ�ֱ��� errno ������Ϊ EAGAIN
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
