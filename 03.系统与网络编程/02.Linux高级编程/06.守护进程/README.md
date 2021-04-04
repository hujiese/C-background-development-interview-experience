<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->

- [守护进程](#%E5%AE%88%E6%8A%A4%E8%BF%9B%E7%A8%8B)
  - [进程，进程组，会话，控制终端之间的关系](#%E8%BF%9B%E7%A8%8B%E8%BF%9B%E7%A8%8B%E7%BB%84%E4%BC%9A%E8%AF%9D%E6%8E%A7%E5%88%B6%E7%BB%88%E7%AB%AF%E4%B9%8B%E9%97%B4%E7%9A%84%E5%85%B3%E7%B3%BB)
  - [一、创建守护进程](#%E4%B8%80%E5%88%9B%E5%BB%BA%E5%AE%88%E6%8A%A4%E8%BF%9B%E7%A8%8B)
  - [二、其他三种种创建守护进程的方法](#%E4%BA%8C%E5%85%B6%E4%BB%96%E4%B8%89%E7%A7%8D%E7%A7%8D%E5%88%9B%E5%BB%BA%E5%AE%88%E6%8A%A4%E8%BF%9B%E7%A8%8B%E7%9A%84%E6%96%B9%E6%B3%95)
    - [（1）daemon 函数](#1daemon-%E5%87%BD%E6%95%B0)
    - [（2）nohup](#2nohup)
    - [（3）&](#3)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## 守护进程

 Linux daemon是运行于后台常驻内存的一种特殊进程，周期性的执行或者等待trigger执行某个任务，与用户交互断开，独立于控制终端。一个守护进程的父进程是init进程，它是一个孤儿进程，没有控制终端，所以任何输出，无论是向标准输出设备stdout还是标准出错设备stderr的输出都被丢到了/dev/null中。守护进程一般用作服务器进程，如httpd，syslogd等。

 ### 进程，进程组，会话，控制终端之间的关系

 ![](https://images2015.cnblogs.com/blog/1081185/201705/1081185-20170518101834807-1254422679.png)

* 进程组: 它是由一个或多个进程组成，进程组号(GID)就是这些进程中的进程组长的PID。
* 会话: 其实叫做会话期(session)，它包括了期间所有的进程组，一般一个会话期开始于用户login，一般login的是shell终端，所以shell终端又是此次会话期的首进程，会话一般结束于logout。对于非进程组长，它可以调用setsid()创建一个新的会话。
* 控制终端(tty): 一般就是指shell终端，它在会话期中可有也可以没有。

其中有必要提下，setsid 函数用于创建一个新的会话，并担任该会话组的组长。调用setsid有3个作用：

* 让进程摆脱原会话的控制
* 让进程摆脱原进程组的控制
* 让进程摆脱原控制终端的控制

### 一、创建守护进程

![](https://camo.githubusercontent.com/7e5cebafcd5bd66f83dbce563cea2caaac924edf/68747470733a2f2f692e696d6775722e636f6d2f66445857576b4e2e706e67)

![](https://camo.githubusercontent.com/7443ccdf413efcfafafeb100f3fec7c3ab0f8b03/68747470733a2f2f692e696d6775722e636f6d2f50497a687159512e706e67)

下面这个程序将创建一个守护进程，守护进程在后台定时向一个文件写入系统时间。

deamon.c

```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>
#include <fcntl.h>

/*
 * time_t rawtime;
 * time ( &rawtime  ); --- 获取时间，以秒计，从1970年1月一日起算，存于rawtime 
 * localtime ( &rawtime  ); //转为当地时间，tm 时间结构 
 * asctime() // 转为标准ASCII时间格式：
 */
void write_time(int num)
{
    time_t rawtime;  
    struct tm * timeinfo;  
    // 获取时间
    time(&rawtime);  
#if 0
    // 转为本地时间
    timeinfo = localtime(&rawtime);  
    // 转为标准ASCII时间格式
    char *cur = asctime(timeinfo);
#else
    char* cur = ctime(&rawtime);
#endif
    
    // 将得到的时间写入文件中
    int fd = open("/home/jack/timelog.txt", O_RDWR | O_CREAT | O_APPEND, 0664);
    if(fd == -1)
    {
        perror("open error");
        exit(1);
    }
    // 写文件
    int ret = write(fd, cur, strlen(cur)+1);
    if(ret == -1)
    {
        perror("write error");
        exit(1);
    }
    // 关闭文件
    close(fd);
}

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
        // 父进程退出
        exit(1);
    }
    else if(pid == 0)
    {
        // 子进程
        // 提升为会长，同时也是新进程组的组长
        setsid();
        // 忽略掉SIGCHLD信号，忽略SIGHUP信号
       // signal(SIGCHLD,SIG_IGN);
        //signal(SIGHUP,SIG_IGN);
            
        // 更改进程的执行目录
        chdir("/home/jack");
        // 更改掩码
        umask(0022);
        // 关闭文件描述符
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
       
        /* 重定向标准的3个文件描述符 */
        /*if(fd = open("/dev/null", O_RDWR) < 0)
            ERROR_EXIT("open /dev/null failed!");
        for(i=0; i<3; i++)
            dup2(fd, i);*/

        // 注册信号捕捉函数
        //　××××××××××××××××　　先注册，再定时　　　×××××××××××××××××××××××××××
        struct sigaction sigact;
        sigact.sa_flags = 0;
        sigemptyset(&sigact.sa_mask);
        sigact.sa_handler = write_time;
        sigaction(SIGALRM, &sigact, NULL);

        // 设置定时器
        struct itimerval act;
        // 定时周期
        act.it_interval.tv_sec = 1;
        act.it_interval.tv_usec = 0;
        // 设置第一次触发定时器时间
        act.it_value.tv_sec = 2;
        act.it_value.tv_usec = 0;
        // 开始计时
        setitimer(ITIMER_REAL, &act, NULL);

        // 防止子进程退出
        while(1);
    }

    return 0;
}
```

### 二、其他三种种创建守护进程的方法

#### （1）daemon 函数

```c
#include <unistd.h>
int daemon(int nochdir, int noclose);

//nochdir为0表示将工作目录变为"/"
//noclose为0表示将文件描述符重定向至"/dev/null"
```

案例：

```c
#include <stdio.h>
#include <unistd.h>

int main()
{
    daemon(0,0);

    while(1)
    {   
        sleep(1);
    }   
    return 0;
}
```

#### （2）nohup

用途：不挂断地运行命令，即使终端ssh关闭了也一直运行（nohup可以使程序能够忽略挂起信号，继续运行）。

语法：nohup Command [ Arg … ] [　& ]

例：

    nohup start.sh &

* 无论是否将 nohup 命令的输出重定向到终端，输出都将附加到当前目录的 nohup.out 文件中。nohup.out相当于日志
* 如果当前目录的 nohup.out 文件不可写，输出重定向到 $HOME/nohup.out 文件中。
* 如果没有文件能创建或打开以用于追加，那么 Command 参数指定的命令不可调用。

**如果不加 & 当前程序则会阻塞终端。**

#### （3）&

用途：在后台运行，当关闭了ssh终端，程序也会关闭

例： 

    start.sh &