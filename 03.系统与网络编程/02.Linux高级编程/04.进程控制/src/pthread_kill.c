#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <errno.h>

void *func1()/*1秒钟之后退出*/
{
	sleep(1);
	printf("线程1（ID：0x%x）退出。\n",(unsigned int)pthread_self());
	pthread_exit((void *)0);
}

void *func2()/*5秒钟之后退出*/
{
	sleep(5);
	printf("线程2（ID：0x%x）退出。\n",(unsigned int)pthread_self());
	pthread_exit((void *)0);
}

void test_pthread(pthread_t tid) /*pthread_kill的返回值：成功（0） 线程不存在（ESRCH） 信号不合法（EINVAL）*/
{
	int pthread_kill_err;
	pthread_kill_err = pthread_kill(tid,0);
	
	if(pthread_kill_err == ESRCH)
		printf("ID为0x%x的线程不存在或者已经退出。\n",(unsigned int)tid);
	else if(pthread_kill_err == EINVAL)
		printf("发送信号非法。\n");
	else
		printf("ID为0x%x的线程目前仍然存活。\n",(unsigned int)tid);
}

int main()
{
	int ret;
	pthread_t tid1,tid2;
	pthread_create(&tid1,NULL,func1,NULL);
	pthread_create(&tid2,NULL,func2,NULL);
	sleep(3);/*创建两个线程3秒钟之后，分别测试一下它们是否还活着*/
	test_pthread(tid1);/*测试ID为tid1的线程是否存在*/
	test_pthread(tid2);/*测试ID为tid2的线程是否存在*/
	exit(0);
}
