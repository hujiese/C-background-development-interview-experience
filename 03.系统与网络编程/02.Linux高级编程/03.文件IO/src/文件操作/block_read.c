#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// �������ն�
int main(void)
{
	char buf[10];//�ַ������������Ϊ10�ֽ�
	int n;//���������ֵ
	n = read(STDIN_FILENO, buf, 10);//�ӱ�׼�������ж����ʮ���ֽ��ַ���
	if (n < 0) //��ȡ�����˳�``
	{
		perror("read STDIN_FILENO");
		exit(1);
	}
	write(STDOUT_FILENO, buf, n);//��������ַ���д����׼�������
	return 0;
}
