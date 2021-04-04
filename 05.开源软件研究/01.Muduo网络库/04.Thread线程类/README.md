## Thread 线程类

更多可见 [muduo源码分析--Thread线程](https://github.com/hujiese/Large-concurrent-serve/blob/master/08_muduo_Thread/muduo_Thread.md)

### 一、线程标识符

linux中，每个进程有一个pid，类型pid_t，由getpid()取得。Linux下的POSIX线程也有一个id，类型pthread_t，由pthread_self()取得，该id由线程库维护，其id空间是各个进程独立的（即不同进程中的线程可能有相同的id）。Linux中的POSIX线程库实现的线程其实也是一个进程（LWP），只是该进程与主进程（启动线程的进程）共享一些资源而已，比如代码段，数据段等。

有时候我们可能需要知道线程的真实pid。比如进程P1要向另外一个进程P2中的某个线程发送信号时，既不能使用P2的pid，更不能使用线程的pthread id，而只能使用该线程的真实pid，称为tid。

glibc的Pthreads实现实际上把pthread_t用作一个结构体指针（它的类型是unsigned long),指向一块动态分配的内存，而且这块内存是反复使用的。这就造成pthread_t的值很容易重复。**Pthreads只保证同一进程之内，同一时刻的各个线程的id不同；不能保证同一进程先后多个线程具有不同的id，更不要说一台机器上多个进程之间的id唯一性了**：

下面这段代码中先后两个线程的标识符是相同的：
```cpp
int main()
{
	pthread_t t1, t2;
	pthread_create(&t1, NULL, threadFun, NULL);
	printf("%lx\n", t1);
	pthread_join(t1, NULL);

	pthread_create(&t2, NULL, threadFun, NULL);
	printf("%lx\n", t2);
	pthread_join(t2, NULL);
}
```
有一个函数gettid()可以得到tid，但glibc并没有实现该函数，只能通过Linux的系统调用syscall来获取。muduo的线程类中是这样使用的：
```c
pid_t gettid()
{
  return static_cast<pid_t>(::syscall(SYS_gettid));
}
```

### 二、__thread 关键字

在Thread.cc中有如下代码：
```c
namespace CurrentThread
{
  __thread int t_cachedTid = 0;
  __thread char t_tidString[32];
  __thread const char* t_threadName = "unknown";
  const bool sameType = boost::is_same<int, pid_t>::value;
  BOOST_STATIC_ASSERT(sameType);
}
```
其中有__thread修饰，这个修饰是gcc内置的线程局部存储设施，__thread只能修饰POD类型。

* __thread是GCC内置的线程局部存储设施，存取效率可以和全局变量相比。
* __thread变量每一个线程有一份独立实体，各个线程的值互不干扰。可以用来修饰那些带有全局性且值可能变，但是又不值得用全局变量保护的变量。

__thread只能修饰POD类型（plain old data，类似整型指针的标量，不带自定义的构造、拷贝、赋值、析构的类型，二进制内容可以任意复制memset,memcpy,且内容可以复原），与C兼容的原始数据，例如，结构和整型等C语言中的类型是 POD 类型，但带有用户定义的构造函数或虚函数的类则不是，例如：
```c
__thread string t_obj1(“cppcourse”);	// 错误，不能调用对象的构造函数
__thread string* t_obj2 = new string;	// 错误，初始化只能是编译期常量
__thread string* t_obj3 = NULL;	// 正确
```

__thread 不能修饰class类型，因为无法自动调用构造函数和析构函数，可以用于修饰全局变量，函数内的静态变量，不能修饰函数的局部变量或者class的普通成员变量，且__thread变量值只能初始化为编译期常量，即编译期间就能确定值。

### 三、pthread_atfork 函数

该函数的原型如下：

```c
#include <pthread.h>
int pthread_atfork(void (*prepare)(void), void (*parent)(void), void (*child)(void));
```

调用fork时，内部创建子进程前在父进程中会调用prepare，内部创建子进程成功后，父进程会调用parent ，子进程会调用child。用这个方法可以及时在fork子进程后关闭一些文件描述符，因为子进程会获取一份父进程的打开的文件描述符。

当然，多用于解决多线程多进程死锁问题。最好不要同时使用多线程多进程，两者择其一。比如在多线程程序中调用fork容易出现死锁。如果在父进程中先创建了一个线程，该线程中加了互斥锁，在此同时父进程也创建了一个子进程，子进程也尝试加锁。这里就出问题了，子进程会复制父进程中锁的状态，也就是说子进程当前处理临界区之外，而且在子进程中无法解锁，这时候子进程就死锁了。解决方法是使用pthread_atfork函数，在prepare时解锁，在parent时加锁。可参考该文 [pthread_atfork函数解决多线程多进程死锁问题](https://murfyexp.github.io/2018/05/20/linux网络编程/pthread_atfork函数解决多线程多进程死锁问题/)

案例如下：

```c
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>

void prepare(void)
{
	printf("pid = %d prepare ...\n", static_cast<int>(getpid()));
}

void parent(void)
{
	printf("pid = %d parent ...\n", static_cast<int>(getpid()));
}

void child(void)
{
	printf("pid = %d child ...\n", static_cast<int>(getpid()));
}


int main(void)
{
	printf("pid = %d Entering main ...\n", static_cast<int>(getpid()));

	pthread_atfork(prepare, parent, child);

	fork();

	printf("pid = %d Exiting main ...\n",static_cast<int>(getpid()));

	return 0;
}
```
编译运行结果如下：

![](https://camo.githubusercontent.com/7fa3cac01e734d5cfcf647853e652048b3d01177/68747470733a2f2f692e696d6775722e636f6d2f757a5772504f4e2e706e67)

### 四、Thread实现

![](https://camo.githubusercontent.com/563c1948852f7c76916a8a115517ccf499c318ad/68747470733a2f2f692e696d6775722e636f6d2f446d62454276792e706e67)

该线程类的封装和前面的“基于对象线程类封装”原理是一样的。