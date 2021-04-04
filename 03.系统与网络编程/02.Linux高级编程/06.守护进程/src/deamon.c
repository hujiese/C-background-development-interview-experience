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
        signal(SIGCHLD,SIG_IGN);
        signal(SIGHUP,SIG_IGN);
            
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
