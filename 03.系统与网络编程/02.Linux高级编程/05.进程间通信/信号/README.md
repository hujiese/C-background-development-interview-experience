<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->

- [信号](#%E4%BF%A1%E5%8F%B7)
  - [一、认识信号](#%E4%B8%80%E8%AE%A4%E8%AF%86%E4%BF%A1%E5%8F%B7)
  - [二、信号相关函数](#%E4%BA%8C%E4%BF%A1%E5%8F%B7%E7%9B%B8%E5%85%B3%E5%87%BD%E6%95%B0)
    - [1、kill函数](#1kill%E5%87%BD%E6%95%B0)
    - [2、raise函数](#2raise%E5%87%BD%E6%95%B0)
    - [3、abort函数](#3abort%E5%87%BD%E6%95%B0)
    - [4、闹钟（定时器）](#4%E9%97%B9%E9%92%9F%E5%AE%9A%E6%97%B6%E5%99%A8)
    - [5、setitimer 定时器](#5setitimer-%E5%AE%9A%E6%97%B6%E5%99%A8)
  - [三、信号集 && 信号集操作相关函数](#%E4%B8%89%E4%BF%A1%E5%8F%B7%E9%9B%86--%E4%BF%A1%E5%8F%B7%E9%9B%86%E6%93%8D%E4%BD%9C%E7%9B%B8%E5%85%B3%E5%87%BD%E6%95%B0)
  - [四、信号捕捉](#%E5%9B%9B%E4%BF%A1%E5%8F%B7%E6%8D%95%E6%8D%89)
    - [1、siganl 函数](#1siganl-%E5%87%BD%E6%95%B0)
    - [2、sigaction 函数](#2sigaction-%E5%87%BD%E6%95%B0)
  - [附录--信号参考](#%E9%99%84%E5%BD%95--%E4%BF%A1%E5%8F%B7%E5%8F%82%E8%80%83)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## 信号

### 一、认识信号

![](https://camo.githubusercontent.com/8cc0ef5f2ea54a7885224fd867b8e571ce7a442f/68747470733a2f2f692e696d6775722e636f6d2f584b636a6a35702e706e67)

信号产生图解：

![](https://camo.githubusercontent.com/844dcb64242663f664d73c9c1d203ec33669ad63/68747470733a2f2f692e696d6775722e636f6d2f67744b4a4c44482e706e67)

### 二、信号相关函数

#### 1、kill函数

![](https://camo.githubusercontent.com/c12d892f9d4b5dfbc2e8ba54532a639ef7400d09/68747470733a2f2f692e696d6775722e636f6d2f713247706651642e706e67)

使用kill -l 命令查看有哪些信号：

    $ kill -l
    1) SIGHUP	 2) SIGINT	 3) SIGQUIT	 4) SIGILL	 5) SIGTRAP
    6) SIGABRT	 7) SIGBUS	 8) SIGFPE	 9) SIGKILL	10) SIGUSR1
    11) SIGSEGV	12) SIGUSR2	13) SIGPIPE	14) SIGALRM	15) SIGTERM
    16) SIGSTKFLT	17) SIGCHLD	18) SIGCONT	19) SIGSTOP	20) SIGTSTP
    21) SIGTTIN	22) SIGTTOU	23) SIGURG	24) SIGXCPU	25) SIGXFSZ
    26) SIGVTALRM	27) SIGPROF	28) SIGWINCH	29) SIGIO	30) SIGPWR
    31) SIGSYS	34) SIGRTMIN	35) SIGRTMIN+1	36) SIGRTMIN+2	37) SIGRTMIN+3
    38) SIGRTMIN+4	39) SIGRTMIN+5	40) SIGRTMIN+6	41) SIGRTMIN+7	42) SIGRTMIN+8
    43) SIGRTMIN+9	44) SIGRTMIN+10	45) SIGRTMIN+11	46) SIGRTMIN+12	47) SIGRTMIN+13
    48) SIGRTMIN+14	49) SIGRTMIN+15	50) SIGRTMAX-14	51) SIGRTMAX-13	52) SIGRTMAX-12
    53) SIGRTMAX-11	54) SIGRTMAX-10	55) SIGRTMAX-9	56) SIGRTMAX-8	57) SIGRTMAX-7
    58) SIGRTMAX-6	59) SIGRTMAX-5	60) SIGRTMAX-4	61) SIGRTMAX-3	62) SIGRTMAX-2
    63) SIGRTMAX-1	64) SIGRTMAX	

下面这个案例演示了如何使用kill函数来发送信号。在该案例中父进程每隔1s打印自己的pid，子进程在创建2s后向父进程发送SIGKILL信号终结父进程。

```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>


int main(int argc, const char* argv[])
{
    pid_t pid = fork();
    if(pid == -1)
    {
        perror("fork error");
        exit(1);
    }

    if(pid > 0)
    {
        while(1)
        {
            printf("parent process pid = %d\n", getpid());
            sleep(1);
        }
    }
    else if(pid == 0)
    {
        sleep(2);
        // 弑父
        kill(getppid(), SIGKILL);
    }

    return 0;
}
```
运行结果如下：

    parent process pid = 83599
    parent process pid = 83599
    Killed

#### 2、raise函数

![](https://camo.githubusercontent.com/96903d08f9987537f2a54f94d89e129f0606c399/68747470733a2f2f692e696d6775722e636f6d2f72314d4a394f4b2e706e67)

rasise函数用于向该进程自己发送信号。下面这个例子中，父进程等待子进程退出并判断子进程是否由于信号退出，如果是，那么打印信号值。子进程创建后使用raise函数向自己发出了SIGQUIT退出信号。

```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>


int main(int argc, const char* argv[])
{
    pid_t pid = fork();

    if(pid > 0)
    {
        // parent
        int s;
        wait(&s);
        if(WIFSIGNALED(s))
        {
            printf("term by signal: %d\n", WTERMSIG(s));
        }
    }
    else if(pid == 0)
    {
        raise(SIGQUIT);
 //       abort();
    }

    return 0;
}
```

输出结果正是：

    term by signal: 3

#### 3、abort函数

![](https://camo.githubusercontent.com/9e34d91bcdf0d2fac7b9f78d845349a58a2a0d7c/68747470733a2f2f692e696d6775722e636f6d2f4b706e786e73632e706e67)

使用abort函数将给自己发送SIGABRT信号，该案例可参考前面raise案例。

#### 4、闹钟（定时器）

![](https://camo.githubusercontent.com/a29c1e18db2330012fd2ae33908675c8c89d6035/68747470733a2f2f692e696d6775722e636f6d2f4a4342626634772e706e67)

* alarm()函数的主要功能是设置信号传送闹钟，即用来设置信号SIGALRM在经过参数seconds秒数后发送给目前的进程。如果未设置信号SIGALARM的处理函数，那么alarm()默认处理终止进程。
* 如果在seconds秒内再次调用了alarm函数设置了新的闹钟，则后面定时器的设置将覆盖前面的设置，即之前设置的秒数被新的闹钟时间取代；当参数seconds为0时，之前设置的定时器闹钟将被取消，并将剩下的时间返回。

下面这个程序一开始创建了一个定时5s的定时器，由于此时是第一个定时器，所以alarm函数的返回值为0，休眠运行2s后再创建一个定时2s的定时器，覆盖之前的定时器，由于上一个定时器已经运行2s，所以返回距上一个定时器触发时间3s，最后每1s循环打印字符串，2s后退出。

alarm.c

```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>


int main(int argc, const char* argv[])
{
    int ret = alarm(5);
    printf("ret = %d\n", ret);

    sleep(2);

    ret = alarm(2);
    printf("ret = %d\n", ret);

    while(1)
    {
        printf("hello\n");
        sleep(1);
    }

    return 0;
}
```
运行结果如下：

    ret = 0
    ret = 3
    hello
    hello
    Alarm clock

#### 5、setitimer 定时器

![](https://camo.githubusercontent.com/083748b15c378d5be75aa7ceacd25fa7d35fc156/68747470733a2f2f692e696d6775722e636f6d2f3074726c6553562e706e67)

![](https://camo.githubusercontent.com/370f3a83d49728f3184086627d1132be947614ee/68747470733a2f2f692e696d6775722e636f6d2f496465674258662e706e67)

下面这个例子设置了第一次触发定时器时间为设置定时器2s以后，然后每1s触发定时器，由于这里没有设置定时器信号处理函数，所以2s后触发定时器之后程序会退出。

setitimer2.c

```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/time.h>
#include <signal.h>


int main(int argc, const char* argv[])
{
    struct itimerval new_value;
    new_value.it_interval.tv_sec = 1;
    new_value.it_interval.tv_usec = 0;

    new_value.it_value.tv_sec = 2;
    new_value.it_value.tv_usec = 0;

    setitimer(ITIMER_REAL, &new_value, NULL);
    while(1)
    {
        printf("Hello...\n");
        sleep(1);
    }
    return 0;
}
```
运行结果如下:

    Hello...
    Hello...
    Alarm clock

下面这个案例使用singal函数注册SIGALRM信号并设置信号处理函数，设置同样的定时方式：

setitimer.c

```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/time.h>
#include <signal.h>

void myfunc(int sig)
{
    printf("hello\n");
}

int main(int argc, const char* argv[])
{
    struct itimerval new_value;
    new_value.it_interval.tv_sec = 2;
    new_value.it_interval.tv_usec = 0;

    new_value.it_value.tv_sec = 2;
    new_value.it_value.tv_usec = 0;

    signal(SIGALRM, myfunc);
    setitimer(ITIMER_REAL, &new_value, NULL);
    while(1)
    {
    }
    return 0;
}
```
运行结果如下:

    hello
    hello
    hello
    hello
    hello
    hello
    hello
    hello
    ...

程序2s后开始发动定时器，定时器每1s触发，myfunc函数每隔1s运行一次，如果不手动停止，"hello"会一直循环打印下去。

### 三、信号集 && 信号集操作相关函数

![](https://camo.githubusercontent.com/1be6ed8060dc92b43ad82b7b585b3e1069153c30/68747470733a2f2f692e696d6775722e636f6d2f5765377932535a2e706e67)

![](https://camo.githubusercontent.com/d83d7d6893107f7dccfd57b361593a8d779e08a1/68747470733a2f2f692e696d6775722e636f6d2f556b6b457078772e706e67)

![](https://camo.githubusercontent.com/2294e7a41208c1028c7e1b88564b67203f88cdc2/68747470733a2f2f692e696d6775722e636f6d2f456d31546c4e342e706e67)

下面这个案例将SIGINT(ctrl + c)、SIGQUIT(ctrl + \)和SIGKILL加入了阻塞信号集。并没过1s打印内核中的未决信号集。当用户触发发前三个信号时，这三个信号便放入到未决信号集中，由于这三个信号被设置了阻塞，所以未决信号集中这几个信号的标志位变为1。

sigmask2.c

```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>

int main(int argc, const char* argv[])
{
    // 自定义信号集
    sigset_t myset, old;
    // 清空　－》　０
    sigemptyset(&myset);
    // 添加要阻塞的信号
    sigaddset(&myset, SIGINT);//ctrl + c
    sigaddset(&myset, SIGQUIT);//ctrl + \
    sigaddset(&myset, SIGKILL);

    // 自定义信号集设置到内核中的阻塞信号集
    sigprocmask(SIG_BLOCK, &myset, &old);

    sigset_t pend;
    while(1)
    {
        sleep(2);
        // 读内核中的未决信号集的状态
        sigpending(&pend);
        int i;
        for(i=1; i<32; ++i)
        {
            if(sigismember(&pend, i))
            {
                printf("1");
            }
            else if(sigismember(&pend, i) == 0)
            {
                printf("0");
            }
        }
        printf("\n");
    }

    return 0;
}
```
运行结果如下:

    0000000000000000000000000000000
    ^C0100000000000000000000000000000
    0100000000000000000000000000000
    0100000000000000000000000000000
    ^\0110000000000000000000000000000
    0110000000000000000000000000000
    0110000000000000000000000000000
    0110000000000000000000000000000
    ^Z
    [6]+  Stopped                 ./app

### 四、信号捕捉

#### 1、siganl 函数

![](https://camo.githubusercontent.com/edbfdbfa3fb051c92b6a30eb3c9d096ed4e81e66/68747470733a2f2f692e696d6775722e636f6d2f41424454784e4b2e706e67)

下面这个程序通过signal注册了一个信号ctrl + c并设置了信号处理的回调函数，在没有信号ctrl + c产生情况下每隔1s循环打印一串字符，当信号ctrl + c产生时调用myfunc函数打印信号值。

signal.c

```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>

void myfunc(int sig)
{
    printf("cathc you signal: %d\n", sig);
}

int main(int argc, const char* argv[])
{
    // 注册信号捕捉函数
    signal(SIGINT, myfunc);

    while(1)
    {
        printf("hello...\n");
        sleep(1);
    }
    return 0;
}
```
运行结果如下:

    hello...
    hello...
    ^Ccathc you signal: 2
    hello...
    ^Ccathc you signal: 2
    hello...
    ^Ccathc you signal: 2
    hello...
    ^Ccathc you signal: 2
    hello...
    ^Z
    [7]+  Stopped                 ./app

#### 2、sigaction 函数

![](https://camo.githubusercontent.com/c02fc9e3aa58ea5428fcf05b516b54286ec914b7/68747470733a2f2f692e696d6775722e636f6d2f685361317173432e706e67)

下面这个案例使用sigaction函数设置了SIGINT捕获函数并屏蔽了SIGQUIT信号。

sigaction.c

```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>

void myfunc(int sig)
{
    printf("hello signal: %d\n", sig);
}

int main(int argc, const char* argv[])
{
    // 注册信号捕捉函数
    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = myfunc;
    // 设置临时屏蔽的信号
    sigemptyset(&act.sa_mask);  // 0
    // ctrl + 反斜杠
    sigaddset(&act.sa_mask, SIGQUIT);// 该信号会被屏蔽
    sigprocmask(SIG_BLOCK, &act.sa_mask, NULL);
    sigaction(SIGINT, &act, NULL);

    while(1);

    return 0;
}
```
运行结果如下:

    ^Chello signal: 2
    ^Chello signal: 2
    ^\^\^\^\^Chello signal: 2
    ^\^Chello signal: 2
    ^Chello signal: 2
    ^Chello signal: 2
    ^\^\^\^\^\^\^Z
    [8]+  Stopped                 ./app

### 附录--信号参考

编号 | 信号名称 | 缺省动作 | 说明
---|---|---|---
1	| SIGHUP	| 终止|	终止控制终端或进程
2	| SIGINT	| 终止	| 键盘产生的中断(Ctrl-C)
3	| SIGQUIT	| dump	| 键盘产生的退出
4	| SIGILL	| dump	| 非法指令
5	| SIGTRAP	| dump	| debug中断
6	| SIGABRT／SIGIOT	| dump	| 异常中止
7	| SIGBUS／SIGEMT	| dump	| 总线异常/EMT指令
8	| SIGFPE	| dump	| 浮点运算溢出
9	| SIGKILL	| 终止	| 强制进程终止
10	| SIGUSR1	| 终止	| 用户信号，进程可自定义用途
11	| SIGSEGV	| dump	| 非法内存地址引用
12	| SIGUSR2	| 终止	| 用户信号，进程可自定义用途
13	| SIGPIPE	| 终止	| 向某个没有读取的管道中写入数据
14	| SIGALRM	| 终止	| 时钟中断(闹钟)
15	| SIGTERM	| 终止	| 进程终止
16	| SIGSTKFLT	| 终止	| 协处理器栈错误
17	| SIGCHLD	| 忽略	| 子进程退出或中断
18	| SIGCONT	| 继续	| 如进程停止状态则开始运行
19	| SIGSTOP	| 停止	| 停止进程运行
20	| SIGSTP	| 停止	| 键盘产生的停止
21	| SIGTTIN	| 停止	| 后台进程请求输入
22	| SIGTTOU	| 停止	| 后台进程请求输出
23	| SIGURG	| 忽略	| socket发生紧急情况
24	| SIGXCPU	| dump	| CPU时间限制被打破
25	| SIGXFSZ	| dump	| 文件大小限制被打破
26	| SIGVTALRM	| 终止	| 虚拟定时时钟
27	| SIGPROF	| 终止	| profile timer clock
28	| SIGWINCH	| 忽略	| 窗口尺寸调整
29	| SIGIO/SIGPOLL	| 终止|	I/O可用
30	| SIGPWR	|终止	|电源异常
31	| SIGSYS／SYSUNUSED	|dump	|系统调用异常