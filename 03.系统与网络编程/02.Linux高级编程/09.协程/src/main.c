#include "coroutine.h"
#include <stdio.h>

struct args {
	int n;
};

// 协程需要执行的用户函数
static void
foo(struct schedule * S, void *ud) {
	struct args * arg = ud;// 用户传入数据
	int start = arg->n;// 获取args结构体中n
	int i;
	// 交替获取当前运行协程的索引号，并打印相关信息
	for (i=0;i<5;i++) {
		printf("coroutine %d : %d\n",coroutine_running(S) , start + i);
		coroutine_yield(S);
	}
}

static void
test(struct schedule *S) {
    // 传递给协程的参数
	struct args arg1 = { 0 };
	struct args arg2 = { 100 };

    // 创建协程，返回值即为协程在调度器协程数组中的索引
	int co1 = coroutine_new(S, foo, &arg1);
	int co2 = coroutine_new(S, foo, &arg2);
	printf("main start\n");
	// 保证两个协程都创建成功
	while (coroutine_status(S,co1) && coroutine_status(S,co2)) {
		coroutine_resume(S,co1);
		coroutine_resume(S,co2);
	} 
	printf("main end\n");
}

int 
main() {
	struct schedule * S = coroutine_open();// 创建协程调度器
	test(S);// 执行任务
	coroutine_close(S);// 关闭所有协程
	
	return 0;
}

