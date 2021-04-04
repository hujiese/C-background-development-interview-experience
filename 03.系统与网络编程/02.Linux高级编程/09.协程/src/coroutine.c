#include "coroutine.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>

#if __APPLE__ && __MACH__
	#include <sys/ucontext.h>
#else 
	#include <ucontext.h>
#endif 

#define STACK_SIZE (1024*1024) // 默认栈大小
#define DEFAULT_COROUTINE 16 // 默认初始化创建协程数量

struct coroutine;

// 协程调度器结构
struct schedule {
	char stack[STACK_SIZE]; // 调度器栈
	ucontext_t main; // 主进程上下文
	int nco; // 当前程序中协程数量
	int cap; // 容量
	int running; // 运行状态
	struct coroutine **co; // 二级指针，保存并指向创建的协程结构
};

// 协程结构
struct coroutine {
	coroutine_func func; // 协程运行函数，typedef void (*coroutine_func)(struct schedule *, void *ud);
	void *ud; // 协程用户数据
	ucontext_t ctx; // 协程上下文
	struct schedule * sch; // 协程所属调度器结构
	ptrdiff_t cap; // 协程的私有栈的空间大小
	ptrdiff_t size;// 协程的私有栈的已用空间
	int status; // 协程状态
	char *stack; // 协程程序栈
};

// 创建并初始化一个协程结构，需要传入调度器地址、协程运行函数和用户数据
struct coroutine * 
_co_new(struct schedule *S , coroutine_func func, void *ud) {
	struct coroutine * co = malloc(sizeof(*co)); // 创建协程结构
	co->func = func; // 设置协程运行函数
	co->ud = ud; // 设置用户传入数据
	co->sch = S; // 设置协程所属调度器
	co->cap = 0;
	co->size = 0;
	co->status = COROUTINE_READY;// 设置协程当前状态为ready
	co->stack = NULL;// 协程当前分配栈空间为空
	return co;// 返回创建的协程结构
}

// 清除一个协程结构中的数据
void
_co_delete(struct coroutine *co) {
	free(co->stack);
	free(co);
}

// 创建一个调度器
struct schedule * 
coroutine_open(void) {
	struct schedule *S = malloc(sizeof(*S)); // 创建schedule结构
	S->nco = 0; // 设置当前调度器保存协程数量为0
	S->cap = DEFAULT_COROUTINE; // 设置默认创建协程容量为16
	S->running = -1;// 设置运行状态为 -1
	S->co = malloc(sizeof(struct coroutine *) * S->cap);// 创建一个用于保存coroutine指针的数组，数组初始化大小为16
	memset(S->co, 0, sizeof(struct coroutine *) * S->cap);// 将上一步创建数组内容清空（数组内不保存任何coroutine地址）
	return S; // 返回创建的调度器结构
}

// 关闭协程，也就是将协程调度器中的所有协程相关内容清空，然后释放调度器自身
void 
coroutine_close(struct schedule *S) {
	int i;
	for (i=0;i<S->cap;i++) {
		struct coroutine * co = S->co[i];
		if (co) {
			_co_delete(co);
		}
	}
	free(S->co);
	S->co = NULL;
	free(S);
}

// 创建一个协程，需要传入调度器地址、协程运行函数和用户数据
int 
coroutine_new(struct schedule *S, coroutine_func func, void *ud) {
	struct coroutine *co = _co_new(S, func , ud);// 调用__co_new创建一个协程结构
	// 检查当前调度器中协程数量是否大于调度器可保存协程容量
	if (S->nco >= S->cap) { // 如果当前创建的协程数量等于或者超出调度器容量
		int id = S->cap; // 获取当前调度器的容量大小
		S->co = realloc(S->co, S->cap * 2 * sizeof(struct coroutine *));// 重新分配调度器协程数组大小，为之前容量的两倍
		memset(S->co + S->cap , 0 , sizeof(struct coroutine *) * S->cap);// 清空新分配的协程数组
		S->co[S->cap] = co; // 将前面创建的协程放入调度器协程数组中
		S->cap *= 2; // 设置调度器协程容量为之前的两倍
		++S->nco; // 调度器中协程数量增加
		return id; // 返回先前调度器容量大小，该大小也是新创建协程在调度器协程数组中的下标位置
	} else {// 如果当前调度器中协程数量小于协程数组容量
		int i;
		// 在调度器协程数组中找到一块没有被使用的位置，然后将该位置保存为新创建的协程
		for (i=0;i<S->cap;i++) {
			int id = (i+S->nco) % S->cap;
			if (S->co[id] == NULL) {
				S->co[id] = co;
				++S->nco; // 调度器协程数量自增
				return id; // 返回新创建协程在调度器协程数组中的索引位置
			}
		}
	}
	// 前面要是失败了，程序运行到这里说明出现异常错误，正常情况下程序不可能运行到这一步
	assert(0);
	return -1;
}

// 协程调度运行函数
static void
mainfunc(uint32_t low32, uint32_t hi32) {
	uintptr_t ptr = (uintptr_t)low32 | ((uintptr_t)hi32 << 32);
	struct schedule *S = (struct schedule *)ptr;// 获取到协程调度器
	int id = S->running;// 获取当前运行协程id
	struct coroutine *C = S->co[id];
	C->func(S,C->ud);// 执行当前运行协程中保存的用户函数
	_co_delete(C);// 用户函数执行完毕，清除该协程运行栈等信息
	S->co[id] = NULL;// 将调度器协程数组中该协程位置清空
	--S->nco;// 调度器中活跃协程数目减一
	S->running = -1;// 调度器当前没有运行任何协程
}

void 
coroutine_resume(struct schedule * S, int id) {
	assert(S->running == -1);// 如果调度器当前在不处于运行状态（断言成立，没有协程在运行）
	assert(id >=0 && id < S->cap);// 确保该协程是合法正确的
	struct coroutine *C = S->co[id];// 获取到该协程结构
	if (C == NULL) // 确保该协程确实存在
		return;
	int status = C->status; // 获取到该协程的运行状态
	switch(status) {
	case COROUTINE_READY:// 第一次创建运行
		getcontext(&C->ctx);// 获取协程运行上下文
		C->ctx.uc_stack.ss_sp = S->stack;// 设置协程运行栈
		C->ctx.uc_stack.ss_size = STACK_SIZE;// 设置协程运行栈大小，默认1MB大小
		C->ctx.uc_link = &S->main;// 设置协程调度返回函数
		S->running = id;// 设置当前调度器中运行协程id
		C->status = COROUTINE_RUNNING;// 设置协程运行状态为“正在运行”
		uintptr_t ptr = (uintptr_t)S;// 获取调度器结构体
		// 设置协程运行函数为mainfunc，将调度器结构体传入协程运行函数mainfunc中
		makecontext(&C->ctx, (void (*)(void)) mainfunc, 2, (uint32_t)ptr, (uint32_t)(ptr>>32));
		swapcontext(&S->main, &C->ctx);// 替换协程映像，将函数程序空间main替换为当前协程运行函数，也就是mainfunc函数
		break;
	case COROUTINE_SUSPEND:// 如果当前协程处于挂起状态
		memcpy(S->stack + STACK_SIZE - C->size, C->stack, C->size);// 恢复之前运行现场
		S->running = id;// 设置当前运行
		C->status = COROUTINE_RUNNING; // 设置当前协程运行状态为“运行态”
		swapcontext(&S->main, &C->ctx);// 替换协程映像，将函数程序空间main替换为当前协程运行函数，也就是mainfunc函数
		break;
	default:
		assert(0);
	}
}

// 保存当前调度器运行main函数运行上下文到协程栈中
static void
_save_stack(struct coroutine *C, char *top) {
	char dummy = 0;
	assert(top - &dummy <= STACK_SIZE);
	if (C->cap < top - &dummy) {
		free(C->stack);
		C->cap = top-&dummy;
		C->stack = malloc(C->cap);
	}
	C->size = top - &dummy;
	memcpy(C->stack, &dummy, C->size);
}
// 协程“礼让”，切换上下文
void
coroutine_yield(struct schedule * S) {
	int id = S->running; // 获取当前运行的协程
	assert(id >= 0);
	struct coroutine * C = S->co[id];
	assert((char *)&C > S->stack);
	_save_stack(C,S->stack + STACK_SIZE);// 保存当前运行现场
	C->status = COROUTINE_SUSPEND;// 将当前运行协程设置为挂起状态
	S->running = -1;// 当前没有处于运行状态的协程
	swapcontext(&C->ctx , &S->main); // 切换上下文空间，运行main函数（main函数上下文已经保存在某协程栈中，可以切换恢复）
}
// 获取调度器中索引为id的协程状态
int 
coroutine_status(struct schedule * S, int id) {
	// 协程必须在调度器中能找到
	assert(id>=0 && id < S->cap);
	// 说明当前调度器协程数组中该位置没有指向有效协程地址，也就是说标号为id的协程不存在
	if (S->co[id] == NULL) {
		return COROUTINE_DEAD;
	}
	return S->co[id]->status;
}

// 获取当前调度器中运行协程的索引号
int 
coroutine_running(struct schedule * S) {
	return S->running;
}

