<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->

- [管道](#%E7%AE%A1%E9%81%93)
  - [一、无名（匿名）管道](#%E4%B8%80%E6%97%A0%E5%90%8D%E5%8C%BF%E5%90%8D%E7%AE%A1%E9%81%93)
  - [二、有名管道](#%E4%BA%8C%E6%9C%89%E5%90%8D%E7%AE%A1%E9%81%93)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## 管道

### 一、无名（匿名）管道

![](https://camo.githubusercontent.com/24f7058ade9d60bf4936ac4a5ca9dfd2f98d991a/68747470733a2f2f692e696d6775722e636f6d2f427a486f6e76752e706e67)

![](https://camo.githubusercontent.com/5a8eefb4077493351475aaeb547358d0f2160dad/68747470733a2f2f692e696d6775722e636f6d2f49496b6e7656522e706e67)

下面这个例子展示了如何创建管道：

pipe.c
```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>

int main(int argc, const char* argv[])
{
    int fd[2];
    int ret  = pipe(fd);
    if(ret == -1)
    {
        perror("pipe error");
        exit(1);
    }

    printf("fd[0] = %d\n", fd[0]);
    printf("fd[1] = %d\n", fd[1]);

    //pid_t pid = fork();
    close(fd[0]);
    close(fd[1]);

    return 0;
}
```

下面这个例子演示了父子进程如何使用无名管道通信。子进程关闭了自己的读端管道，父进程关闭了自己的写端管道，子进程向自己的写管道写入一段话，父进程则从自己的读管道读取这部分内容并打印：

pipe2.c

```c
#include<unistd.h> 
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define INPUT 0 
#define OUTPUT 1 
int main() {
	int fd[2];
	/*定义子进程号 */ 
	pid_t pid; 
	char buf[256]; 
	int returned_count; 
	/*创建无名管道*/ 
	pipe(fd);
	/*创建子进程*/
    pid=fork();	
	if(pid<0) {
		printf("Error in fork\n");
		exit(1); 
	}else if(pid == 0) { /*执行子进程*/
	    printf("in the child process...\n"); 
		/*子进程向父进程写数据，关闭管道的读端*/ 
		close(fd[INPUT]); 
		write(fd[OUTPUT], "hello world", strlen("hello world"));
		exit(0); 
	}else { /*执行父进程*/ 	    
		printf("in the parent process...\n"); 
		/*父进程从管道读取子进程写的数据，关闭管道的写端*/
		close(fd[OUTPUT]); 
		returned_count = read(fd[INPUT], buf, sizeof(buf)); 
		printf("%d bytes of data received from child process: %s\n", returned_count, buf);
	}
	return 0;
}
```

输出结果如下：

    in the parent process...
    in the child process...
    11 bytes of data received from child process: hello worldW�

管道的一些其他内容：

![](https://camo.githubusercontent.com/50edbc03cc6cface45bf5ef3f584341e50a9ce59/68747470733a2f2f692e696d6775722e636f6d2f535630396f54342e706e67)

![](https://camo.githubusercontent.com/39903067565cd74822082b63aa1925da0aaebd84/68747470733a2f2f692e696d6775722e636f6d2f6a7932567963672e706e67)

### 二、有名管道

前面的无名管道适合用于有血缘间进程通信，有名管道则适用于无血缘关系进程间通信。

![](https://camo.githubusercontent.com/0fc823d410ab56abaff3684b37e2a78078af5ead/68747470733a2f2f692e696d6775722e636f6d2f34665a444d6f6c2e706e67)

下面这个案例将演示如何使用有名管道。演示案例由两部分代码，write_fifo.c中会让用户输入管道的名字，如果该管道不存在，将自动创建，然后每过1s便向该管道中写入一句"hello, world"。read_fifo.c则是不断阻塞等待并读取管道中的内容。

write_fifo.c

```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, const char* argv[])
{
    if(argc < 2)
    {
        printf("./a.out fifoname\n");
        exit(1);
    }

    // 判断文件是否存在
    int ret = access(argv[1], F_OK);
    if(ret == -1)
    {
        int r = mkfifo(argv[1], 0664);
        if(r == -1)
        {
            perror("mkfifo error");
            exit(1);
        }
        printf("有名管道%s创建成功\n", argv[1]);
    }

    int fd = open(argv[1], O_WRONLY);
    if(fd == -1)
    {
        perror("open error");
        exit(1);
    }
    
    char *p = "hello, world";
    while(1)
    {
        sleep(1);
        int len = write(fd, p, strlen(p)+1);
    }

    close(fd);
    
    return 0;
}
```

read_fifo.c

```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

int main(int argc, const char* argv[])
{
    if(argc < 2)
    {
        printf("./a.out fifoname\n");
        exit(1);
    }

    // 判断文件是否存在
    int ret = access(argv[1], F_OK);
    if(ret == -1)
    {
        int r = mkfifo(argv[1], 0664);
        if(r == -1)
        {
            perror("mkfifo error");
            exit(1);
        }
        printf("有名管道%s创建成功\n", argv[1]);
    }

    int fd = open(argv[1], O_RDONLY);
    if(fd == -1)
    {
        perror("open error");
        exit(1);
    }
    
    char buf[512];
    while(1)
    {
        int len = read(fd, buf, sizeof(buf));
        buf[len] = 0;
        printf("buf = %s\n, len = %d", buf, len);
    }

    close(fd);
    
    return 0;
}
```

