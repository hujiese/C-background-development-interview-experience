#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// 阻塞读终端
int main(void)
{
	char buf[10];//字符缓存区，最大为10字节
	int n;//保存读出的值
	n = read(STDIN_FILENO, buf, 10);//从标准输入流中读最多十个字节字符串
	if (n < 0) //读取错误，退出``
	{
		perror("read STDIN_FILENO");
		exit(1);
	}
	write(STDOUT_FILENO, buf, n);//将读入的字符串写到标准输出流中
	return 0;
}
