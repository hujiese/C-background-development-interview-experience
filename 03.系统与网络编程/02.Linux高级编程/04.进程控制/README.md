<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->


- [进程控制](#%E8%BF%9B%E7%A8%8B%E6%8E%A7%E5%88%B6)
  - [一、进程创建](#%E4%B8%80%E8%BF%9B%E7%A8%8B%E5%88%9B%E5%BB%BA)
    - [1、进程控制](#1%E8%BF%9B%E7%A8%8B%E6%8E%A7%E5%88%B6)
    - [2、进程创建误区](#2%E8%BF%9B%E7%A8%8B%E5%88%9B%E5%BB%BA%E8%AF%AF%E5%8C%BA)
    - [3、进程相关命令](#3%E8%BF%9B%E7%A8%8B%E7%9B%B8%E5%85%B3%E5%91%BD%E4%BB%A4)
    - [4、进程共享](#4%E8%BF%9B%E7%A8%8B%E5%85%B1%E4%BA%AB)
  - [二、exec函数族](#%E4%BA%8Cexec%E5%87%BD%E6%95%B0%E6%97%8F)
    - [更多的exec函数族用法详见demo_exec.c。](#%E6%9B%B4%E5%A4%9A%E7%9A%84exec%E5%87%BD%E6%95%B0%E6%97%8F%E7%94%A8%E6%B3%95%E8%AF%A6%E8%A7%81demo_execc)
    - [1、execl 和 execv](#1execl-%E5%92%8C-execv)
    - [2、execlp](#2execlp)
    - [3、execle和execve](#3execle%E5%92%8Cexecve)
  - [三、进程回收](#%E4%B8%89%E8%BF%9B%E7%A8%8B%E5%9B%9E%E6%94%B6)
    - [1、孤儿进程](#1%E5%AD%A4%E5%84%BF%E8%BF%9B%E7%A8%8B)
    - [2、僵尸进程](#2%E5%83%B5%E5%B0%B8%E8%BF%9B%E7%A8%8B)
    - [3、进程回收](#3%E8%BF%9B%E7%A8%8B%E5%9B%9E%E6%94%B6)
    - [(1)wait函数](#1wait%E5%87%BD%E6%95%B0)
    - [(2)waitpid函数](#2waitpid%E5%87%BD%E6%95%B0)
  - [三、进程相关函数总结](#%E4%B8%89%E8%BF%9B%E7%A8%8B%E7%9B%B8%E5%85%B3%E5%87%BD%E6%95%B0%E6%80%BB%E7%BB%93)
  - [补充--写时复制](#%E8%A1%A5%E5%85%85--%E5%86%99%E6%97%B6%E5%A4%8D%E5%88%B6)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## 进程控制

### 一、进程创建

#### 1、进程控制

![](https://camo.githubusercontent.com/23c70becd9485c33ef8eb8c86b2becebc6459f96/68747470733a2f2f692e696d6775722e636f6d2f615a6575336d552e706e67)

* getpid()得到当前进程的PID
* getppid()得到当前进程父进程的PID

下面的这个程序将在一个主进程中创建子进程，主程序一开始开始打印j的值五次，然后创建一个子进程，fork返回值大于零为父进程，等于零为子进程，然后父子各自打印自己的进程ID和父进程ID，之后再各自循环打印i的值。需要注意的是，在父进程中使用了sleep(1)，这样让父进程有意地休眠是有原因的，由于父子进程哪个先执行是为知道的，如果父进程运行较快，先运行结束了，子进程将成为“孤儿进程”，所以有意地让父进程休眠一会儿。

fork.c：

```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>


int main(int argc, const char* argv[])
{

    for(int j=0; j<5; ++j)
    {
        printf(" j = %d\n", j);
    }
    
    pid_t pid = fork();
    // 父进程
    if(pid > 0)
    {
        printf("parent process, pid = %d, ppid = %d\n", getpid(), getppid());
        sleep(1);
    }
    // 子进程
    else if(pid == 0)
    {
        printf("child process, pid = %d, ppid = %d\n", getpid(), getppid());
    }

    for(int i=0; i<5; ++i)
    {
        printf(" i = %d\n", i);
    }

    return 0;
}
```

运行结果如下：

    j = 0
    j = 1
    j = 2
    j = 3
    j = 4
    child process, pid = 77723, ppid = 77722
    i = 0
    i = 1
    i = 2
    i = 3
    i = 4
    parent process, pid = 77722, ppid = 74423
    i = 0
    i = 1
    i = 2
    i = 3
    i = 4

结论：

* 1.fork返回值大于0，则为父进程的返回值，等于0为子进程的返回值；

* 2.子进程创建成功后，代码执行位置：父进执行到哪里，子进程就从哪里开始执行;

* 3.父子进程没有执行顺序，谁先抢到CPU谁先执行；

* 4.可以通过fork的返回值来区分父子进程。

#### 2、进程创建误区

先看看下面这个程序，判断这个程序打印了多少次：

loop_fork1.c

```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>


int main(int argc, const char* argv[])
{
    pid_t pid;

    int i=0;
    for(i=0; i<3; i++)
    {
        pid = fork();
    }
    
    printf("Proess %d is run ...\n", getpid());
    return 0;
}
```

运行结果如下:

    Proess 78002 is run ...
    Proess 78005 is run ...
    Proess 78003 is run ...
    Proess 78007 is run ...
    Proess 78006 is run ...
    Proess 78008 is run ...
    Proess 78004 is run ...
    Proess 78009 is run ...

一共打印了八次，所以一共有八个进程，而不是我们想要的三个。下面一张图说明原因：

![](https://camo.githubusercontent.com/d7a47d288ce90532a27b40bd0415cb37f53997c6/68747470733a2f2f692e696d6775722e636f6d2f42434b585834712e706e67)


#### 3、进程相关命令

* 1.查看某个xxx进程：ps aux | grep "xxx"

* 2.查看某个xxx进程：ps ajx | grep "xxx"

* 3.查看信号： kill -l

* 4.杀死某个进程：kill -9 (SIGKILL) PID

更多内容在Linux相关篇章会有介绍。

#### 4、进程共享

![](https://camo.githubusercontent.com/264557a55140fa9959b729746d98fe7c5a1177b5/68747470733a2f2f692e696d6775722e636f6d2f68515a64696e322e706e67)

下面这个例子将演示父子进程是否共享全局变量：

copy_on_write.c

```c
#include<stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

char str[6] = "hello";

int main(int argc, char* argv[])
{
   pid_t pid;

   pid = fork();
   
   if(pid == 0)//son
   {
       str[0] = 's';
       printf("son's str is %s . the addr of str is %x.\n", str, (unsigned int)str);
   }
   if(pid > 0)
   {
       sleep(1);
       str[1] = 'f';
       printf("son's str is %s . the addr of str is %x.\n", str, (unsigned int)str);
   }
   return 0; 
}
```

输出结果如下:

    son's str is sello . the addr of str is 601050.
    father's str is fello . the addr of str is 601050.

父子进程的str地址都是一样的，但是对应的值却不一样，这便是Linux进程的“写时拷贝”，这部分内容具体会在Linux后续详细介绍。fork时子进程获得父进程数据空间、堆和栈的复制，所以变量的地址（当然是虚拟地址）也是一样的，但是由于“写时拷贝”，str的物理地址又不一样，所以才有了上面的现象。

### 二、exec函数族

* 1.让父子进程执行不相干的操作;

* 2.能够替换进程地址空间中的源码中的.txt段（代码段）;

* 3.当前程序中调用另外一个应用程序，在使用exec之前需要先fork子进程；

* 4.如果函数执行成功，不返回；执行失败可以打印错误信息，然后退出当前进程。

![](https://camo.githubusercontent.com/2bf5e02cc8cedbb5b48cdb6752b0c897e8f72105/68747470733a2f2f692e696d6775722e636f6d2f7876744f477a652e706e67)

#### 更多的exec函数族用法详见demo_exec.c。

#### 1、execl 和 execv

![](https://camo.githubusercontent.com/9865b6d69b9cb20d3b4eacbd69150b2c6da1250f/68747470733a2f2f692e696d6775722e636f6d2f6f454d556954722e706e67)

![](https://camo.githubusercontent.com/c4435b6be718bcecfee0dfadeacfc65a8d462c04/68747470733a2f2f692e696d6775722e636f6d2f354f32536652352e706e67)

下面这个例子演示fork和execl如何结合使用：

execl.c：

```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>


int main(int argc, const char* argv[])
{
    printf("hello, world\n");

    pid_t pid = fork();
    if(pid == -1)
    {
        perror("fork error");
        exit(1);
    }
    if(pid >0)
    {
        sleep(1);
    }
    //　子进程执行程序
    if(pid == 0)
    {
        execl("./hello", "xxxx",  NULL);
        //execl("/home/kevin/hello", "xxxx",  NULL);
        perror("execl");
        exit(1);
    }

    for(int i=0; i<3; ++i)
    {
        printf(" i = %d\n", i);
    }

    return 0;
}
```
其中./hello可执行文件的源代码如下;

hello.c：

```c
#include<stdio.h>

int main(void)
{
    printf("Hello ,this is a simple demo !\n");
    return 0;
}
```

运行结果如下:

    hello, world
    Hello ,this is a simple demo !
    i = 0
    i = 1
    i = 2

理论上子进程也应该要打印三次i的值，但是由于子进程的程序空间已经被execl替换为了hello可执行文件，所以只执行hello的内容。src目录下还有用execl执行系统环境中的程序案例exe.c：
```c
    execl("/bin/ls", "666", "-lah" ,NULL);
```
具体内容见exe.c。

#### 2、execlp

![](https://camo.githubusercontent.com/713ea745e87430588f98f5bb58888c2e61acb72a/68747470733a2f2f692e696d6775722e636f6d2f56456d3352374b2e706e67)

```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>


int main(int argc, const char* argv[])
{
    printf("hello, world\n");

    pid_t pid = fork();
    if(pid == -1)
    {
        perror("fork error");
        exit(1);
    }

    //　子进程执行程序
    if(pid == 0)
    {
        execlp("ps", "pssdfsdf", "aux", NULL);
        perror("execlp");
        exit(1);
    }

    for(int i=0; i<3; ++i)
    {
        printf(" i = %d\n", i);
    }

    return 0;
}
```

这里让execlp执行ps aux命令，结果如下：

    hello, world
    i = 0
    i = 1
    i = 2
    USER        PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND
    root          1  0.0  0.1  33840  2768 ?        Ss   11月05   0:06 /sbin/init
    root          2  0.0  0.0      0     0 ?        S    11月05   0:00 [kthreadd]
    root          3  0.0  0.0      0     0 ?        S    11月05   0:03 [ksoftirqd/0]
    root          5  0.0  0.0      0     0 ?        S<   11月05   0:00 [kworker/0:0H]
    root          7  0.0  0.0      0     0 ?        S    11月05   1:01 [rcu_sched]
    root          8  0.0  0.0      0     0 ?        S    11月05   0:00 [rcu_bh]
    root          9  0.0  0.0      0     0 ?        S    11月05   0:01 [migration/0]
    root         10  0.0  0.0      0     0 ?        S    11月05   0:02 [watchdog/0]
    root         11  0.0  0.0      0     0 ?        S    11月05   0:02 [watchdog/1]
    ....
    
    root      79318  0.0  0.0      0     0 ?        S    20:53   0:00 [kworker/u256:2]
    jack      79332  0.0  0.0   4200   628 pts/3    S+   20:54   0:00 ./app
    jack      79333  0.0  0.1  22644  2828 pts/3    R+   20:54   0:00 pssdfsdf aux

#### 3、execle和execve

![](https://camo.githubusercontent.com/95c58e5109f6bc42a75694b56ec2be99c04a9e34/68747470733a2f2f692e696d6775722e636f6d2f434e504e334c4a2e706e67)

![](https://camo.githubusercontent.com/2bfa3d59deb5563ba73a3b10c93f4d6f93ae411b/68747470733a2f2f692e696d6775722e636f6d2f553137706663582e706e67)

### 三、进程回收

![](https://camo.githubusercontent.com/c3f9056a03ea64a3b39bac8b2c67413f16878a16/68747470733a2f2f692e696d6775722e636f6d2f314f79535837712e706e67)

#### 1、孤儿进程

下面程序创建了一个子进程，子进程休眠1s，然而当子进程继续执行时，父进程已经退出了，由此来构建孤儿进程案例。

orphan.c

```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>


int main(int argc, const char* argv[])
{
    pid_t pid = fork();

    // 父进程
    if(pid > 0)
    {
        printf("parent process, pid = %d, ppid = %d\n", getpid(), getppid());
    }
    // 子进程
    else if(pid == 0)
    {
        sleep(1);
        printf("child process, pid = %d, ppid = %d\n", getpid(), getppid());
    }

    return 0;
}
```
运行结果如下：

    parent process, pid = 79817, ppid = 74423
    jack@jack-virtual-machine:/mnt/hgfs/vmshared/LinuxCodeMaster/进程控制/basic/5Day$ child process, pid = 79818, ppid = 32606

从子进程打印的ppid=32606看，并不是父进程的pid，这里可以查看下这个32606进程号对应的是哪个进程。

    $ps -aux|grep 32606
    jack      32606  0.0  0.1  40328  3252 ?        Ss   11月07   0:01 init --user
    jack      79837  0.0  0.1  15948  2404 pts/3    S+   08:47   0:00 grep --color=auto 32606

这个进程是init进程，init进程负责回收孤儿进程。


#### 2、僵尸进程

下面这个程序在主进程中创建子进程，之后，父进程中先打印pid和ppid，然后进入while死循环，每隔1s打印一句话。子进程中则只是打印自己的pid和ppid，然后打印五次i的值。由于父进程陷入while循环，所以将不会打印i的值。当子进程运行结束后，父进程还忙于while循环，将不会回收子进程，由此构建了僵尸进程案例。

```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>


int main(int argc, const char* argv[])
{
    pid_t pid = fork();

    // 父进程
    if(pid > 0)
    {
        printf("parent process, pid = %d, ppid = %d\n", getpid(), getppid());
        while(1)
        {
            printf(" i am live\n");
            sleep(1);
        }
    }
    // 子进程
    else if(pid == 0)
    {
        printf("child process, pid = %d, ppid = %d\n", getpid(), getppid());
    }

    for(int i=0; i<5; ++i)
    {
        printf(" i = %d\n", i);
    }

    return 0;
}
```

运行结果如下:

    parent process, pid = 79888, ppid = 74423
    i am live
    child process, pid = 79889, ppid = 79888
    i = 0
    i = 1
    i = 2
    i = 3
    i = 4
    i am live
    i am live
    i am live
    ...

查看下子进程的状态:

    $ps -aux|grep 79889
    jack      79889  0.0  0.0      0     0 pts/3    Z+   08:56   0:00 [app] <defunct>
    jack      79921  0.0  0.1  15944  2352 pts/10   S+   08:58   0:00 grep --color=auto 79889

子进程处于Z+状态，即僵尸进程。

**僵尸进程用kill命令是无法杀掉的，但是我们可以结果掉僵尸进程的爸爸，僵尸daddy挂了之后，僵尸进程就成了孤儿进程，孤儿进程不会占用系统资源，会被init程序收养，然后init程序将其回收。**

#### 3、进程回收

#### (1)wait函数
![](https://camo.githubusercontent.com/be99975f8b84b1f2e5babf520b784a805802fe67/68747470733a2f2f692e696d6775722e636f6d2f6d4a686e77446a2e706e67)

下面的程序在父进程中使用wait等待子进程运行结束并判断子进程是如何退出的。

wait.c
```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>


int main(int argc, const char* argv[])
{
    pid_t pid = fork();

    // 父进程
    if(pid > 0)
    {
        printf("parent process, pid = %d, ppid = %d\n", getpid(), getppid());
        // 回收资源
        int status;
        pid_t wpid = wait(&status);
        printf("child died pid = %d\n", wpid);
        // 通过返回值退出
        if(WIFEXITED(status))
        {
            printf("child process exit value: %d\n", WEXITSTATUS(status));
        }
        // 子进程被信号杀死
        else if(WIFSIGNALED(status))
        {
            printf("child process died by signal: %d\n", WTERMSIG(status));
        }

    }
    // 子进程
    else if(pid == 0)
    {
        printf("child process, pid = %d, ppid = %d\n", getpid(), getppid());
        sleep(200);
    }

    for(int i=0; i<5; ++i)
    {
        printf(" i = %d\n", i);
    }

//    return 10;
    exit(10);
}
```

#### (2)waitpid函数

![](https://camo.githubusercontent.com/6d44b201b48cfd8acc73ef415171c7e7cf171b4c/68747470733a2f2f692e696d6775722e636f6d2f677057715347742e706e67)

wait函数只能等待一个子进程退出，但waitpid可以等待多个退出。下面这个程序fork了十个子进程，然后通过在父进程中使用waitpid等待这些子进程全部退出。

waitpid.c

```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, const char* argv[])
{
    pid_t pid;

    for(int i=0; i<10; ++i)
    {
        pid = fork();
        if(pid == 0)
        {
            break;
        }
    }

    if(pid > 0)
    {
        // 父进程
        printf("parent process, pid = %d, ppid = %d\n", getpid(), getppid());
        // 回收子进程
        pid_t wpid;
        int status;
        while( ( wpid = waitpid(-1, &status, WNOHANG) ) != -1 )
        {
            if(wpid == 0)
                continue;
            printf("died process pid = %d\n", wpid);
            // 判断文件退出方式
            if(WIFEXITED(status))
            {
                printf("------ exit value: %d\n", WEXITSTATUS(status));
            }
            if(WIFSIGNALED(status))
            {
                printf("=======exit by signal: %d\n", WTERMSIG(status));
            }
        }
    }
    else
    {
        // 子进程
        printf("child process, pid = %d, ppid = %d\n", getpid(), getppid());
    }
    return 9;
}
```

### 三、进程相关函数总结

![](https://camo.githubusercontent.com/82b1115aa9a354e70167a1571c39ab25b9cd4f8a/68747470733a2f2f692e696d6775722e636f6d2f666b377a4b6a672e706e67)

![](https://camo.githubusercontent.com/6d2c0086b82914139eb464f50a0c3e56594d13ba/68747470733a2f2f692e696d6775722e636f6d2f5259593938436e2e706e67)

![](https://camo.githubusercontent.com/83115ff844971d7d93ff4c9cf19067394209226b/68747470733a2f2f692e696d6775722e636f6d2f5a4362706f6d322e706e67)

![](https://camo.githubusercontent.com/364ecba36c3060f0f69880dd5fae4983ebdc9f70/68747470733a2f2f692e696d6775722e636f6d2f764a7a39564e302e706e67)

### 补充--写时复制

Linux采用了写时复制的方法，以减少fork时对父进程空间进程整体复制带来的开销。

写时复制是一种采取了惰性优化方法来避免复制时的系统开销。它的前提很简单：如果有多个进程要读取它们自己的那部门资源的副本，那么复制是不必要的。每个进程只要保存一个指向这个资源的指针就可以了。只要没有进程要去修改自己的“副本”，就存在着这样的幻觉：每个进程好像独占那个资源。从而就避免了复制带来的负担。如果一个进程要修改自己的那份资源“副本”，那么就会复制那份资源，并把复制的那份提供给进程。不过其中的复制对进程来说是透明的。这个进程就可以修改复制后的资源了，同时其他的进程仍然共享那份没有修改过的资源。所以这就是名称的由来：在写入时进行复制。

写时复制的主要好处在于：如果进程从来就不需要修改资源，则不需要进行复制。惰性算法的好处就在于它们尽量推迟代价高昂的操作，直到必要的时刻才会去执行。

在使用虚拟内存的情况下，写时复制（Copy-On-Write）是以页为基础进行的。所以，只要进程不修改它全部的地址空间，那么就不必复制整个地址空间。在fork( )调用结束后，父进程和子进程都相信它们有一个自己的地址空间，但实际上它们共享父进程的原始页，接下来这些页又可以被其他的父进程或子进程共享。

写时复制在内核中的实现非常简单。与内核页相关的数据结构可以被标记为只读和写时复制。如果有进程试图修改一个页，就会产生一个缺页中断。内核处理缺页中断的方式就是对该页进行一次透明复制。这时会清除页面的COW属性，表示着它不再被共享。

现代的计算机系统结构中都在内存管理单元（MMU）提供了硬件级别的写时复制支持，所以实现是很容易的。

在调用fork( )时，写时复制是有很大优势的。因为大量的fork之后都会跟着执行exec，那么复制整个父进程地址空间中的内容到子进程的地址空间完全是在浪费时间：如果子进程立刻执行一个新的二进制可执行文件的映像，它先前的地址空间就会被交换出去。写时复制可以对这种情况进行优化。

在实现写时复制之前，Unix的设计者们就一直很关注在fork后立刻执行exec所造成的地址空间的浪费。BSD的开发者们在3.0的BSD系统中引入了vfork( )系统调用。
```cpp
#include <sys/types.h>

#include <unistd.h>

pid_t vfork(void);
```
除了子进程必须要立刻执行一次对exec的系统调用，或者调用_exit( )退出，对vfork( )的成功调用所产生的结果和fork( )是一样的。vfork( )会挂起父进程直到子进程终止或者运行了一个新的可执行文件的映像。通过这样的方式，vfork( )避免了地址空间的按页复制。在这个过程中，父进程和子进程共享相同的地址空间和页表项。实际上vfork( )只完成了一件事：复制内部的内核数据结构。因此，子进程也就不能修改地址空间中的任何内存。

vfork( )是一个历史遗留产物，Linux本不应该实现它。需要注意的是，即使增加了写时复制，vfork( )也要比fork( )快，因为它没有进行页表项的复制。然而，写时复制的出现减少了对于替换fork( )争论。实际上，直到2.2.0内核，vfork( )只是一个封装过的fork( )。因为对vfork( )的需求要小于fork( )，所以vfork( )的这种实现方式是可行的。

fork和vfork的区别：

1、fork( )的子进程拷贝父进程的数据段和代码段；vfork( )的子进程与父进程共享数据段

2、fork( )的父子进程的执行次序不确定；vfork( )保证子进程先运行，在调用exec或exit之前与父进程数据是共享的，在它调用exec或exit之后父进程才可能被调度运行。

3、vfork( )保证子进程先运行，在它调用exec或exit之后父进程才可能被调度运行。如果在调用这两个函数之前子进程依赖于父进程的进一步动作，则会导致死锁。

4、当需要改变共享数据段中变量的值，则拷贝父进程。

fork和vfork的对比可见 [fork与vfork的区别](./img/vfork.png)

> https://blog.csdn.net/jianchi88/article/details/6985326