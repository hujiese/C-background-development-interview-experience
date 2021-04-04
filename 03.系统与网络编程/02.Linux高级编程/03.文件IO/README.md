<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->

- [文件IO](#%E6%96%87%E4%BB%B6io)
  - [一、预备知识](#%E4%B8%80%E9%A2%84%E5%A4%87%E7%9F%A5%E8%AF%86)
    - [1、C库函数与系统工作流程](#1c%E5%BA%93%E5%87%BD%E6%95%B0%E4%B8%8E%E7%B3%BB%E7%BB%9F%E5%B7%A5%E4%BD%9C%E6%B5%81%E7%A8%8B)
    - [2、C库函数与系统函数的关系](#2c%E5%BA%93%E5%87%BD%E6%95%B0%E4%B8%8E%E7%B3%BB%E7%BB%9F%E5%87%BD%E6%95%B0%E7%9A%84%E5%85%B3%E7%B3%BB)
    - [3、虚拟地址空间](#3%E8%99%9A%E6%8B%9F%E5%9C%B0%E5%9D%80%E7%A9%BA%E9%97%B4)
    - [4、pcb和文件描述符](#4pcb%E5%92%8C%E6%96%87%E4%BB%B6%E6%8F%8F%E8%BF%B0%E7%AC%A6)
  - [二、如何使用man](#%E4%BA%8C%E5%A6%82%E4%BD%95%E4%BD%BF%E7%94%A8man)
  - [三、文件操作](#%E4%B8%89%E6%96%87%E4%BB%B6%E6%93%8D%E4%BD%9C)
    - [1、普通文件读写案例read_write.c：](#1%E6%99%AE%E9%80%9A%E6%96%87%E4%BB%B6%E8%AF%BB%E5%86%99%E6%A1%88%E4%BE%8Bread_writec)
    - [2、阻塞读案例--block_read.c ：](#2%E9%98%BB%E5%A1%9E%E8%AF%BB%E6%A1%88%E4%BE%8B--block_readc-)
    - [3、非阻塞读案例--unblock_read.c ：](#3%E9%9D%9E%E9%98%BB%E5%A1%9E%E8%AF%BB%E6%A1%88%E4%BE%8B--unblock_readc-)
    - [4、超时非阻塞读--timeout_unblock_read.c :](#4%E8%B6%85%E6%97%B6%E9%9D%9E%E9%98%BB%E5%A1%9E%E8%AF%BB--timeout_unblock_readc-)
  - [四、文件属性](#%E5%9B%9B%E6%96%87%E4%BB%B6%E5%B1%9E%E6%80%A7)
    - [1、文件的权限](#1%E6%96%87%E4%BB%B6%E7%9A%84%E6%9D%83%E9%99%90)
    - [2、文件属性函数](#2%E6%96%87%E4%BB%B6%E5%B1%9E%E6%80%A7%E5%87%BD%E6%95%B0)
      - [（1）文件属性](#1%E6%96%87%E4%BB%B6%E5%B1%9E%E6%80%A7)
      - [（2）文件权限](#2%E6%96%87%E4%BB%B6%E6%9D%83%E9%99%90)
      - [（3）修改文件大小](#3%E4%BF%AE%E6%94%B9%E6%96%87%E4%BB%B6%E5%A4%A7%E5%B0%8F)
  - [3、目录操作相关函数](#3%E7%9B%AE%E5%BD%95%E6%93%8D%E4%BD%9C%E7%9B%B8%E5%85%B3%E5%87%BD%E6%95%B0)
    - [(1)文件重命名](#1%E6%96%87%E4%BB%B6%E9%87%8D%E5%91%BD%E5%90%8D)
    - [(2)切换目录](#2%E5%88%87%E6%8D%A2%E7%9B%AE%E5%BD%95)
    - [(3)创建和删除目录](#3%E5%88%9B%E5%BB%BA%E5%92%8C%E5%88%A0%E9%99%A4%E7%9B%AE%E5%BD%95)
    - [(4)遍历目录](#4%E9%81%8D%E5%8E%86%E7%9B%AE%E5%BD%95)
  - [四、dup-dup2-fcntl的使用](#%E5%9B%9Bdup-dup2-fcntl%E7%9A%84%E4%BD%BF%E7%94%A8)
    - [1、dup和dup2](#1dup%E5%92%8Cdup2)
    - [(1)dup用法](#1dup%E7%94%A8%E6%B3%95)
    - [(2)dup2用法](#2dup2%E7%94%A8%E6%B3%95)
    - [2、fcntl函数](#2fcntl%E5%87%BD%E6%95%B0)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## 文件IO

### 一、预备知识

#### 1、C库函数与系统工作流程

![C库函数与系统工作流程](https://camo.githubusercontent.com/326364c413a37d70d4a34ac3db502447168885f5/68747470733a2f2f692e696d6775722e636f6d2f6b5648326d33362e706e67)

#### 2、C库函数与系统函数的关系

![](https://camo.githubusercontent.com/c903367310923424c235b292d17ba319491e9413/68747470733a2f2f692e696d6775722e636f6d2f6e46564d6843322e706e67)

#### 3、虚拟地址空间

![](https://camo.githubusercontent.com/1195e558ce80fc9016b570cbb797778b80faef0e/68747470733a2f2f692e696d6775722e636f6d2f766757375a716d2e706e67)

#### 4、pcb和文件描述符

![](https://camo.githubusercontent.com/8aecd285a6d3837d85407673e69ab3d2446b691c/68747470733a2f2f692e696d6775722e636f6d2f724358797241482e706e67)

### 二、如何使用man

关于函数库的使用，可以查看文档，现在输入man man:

![](https://camo.githubusercontent.com/dcc3f5cd0d21b157916a8d9c541f49a657c37f05/68747470733a2f2f692e696d6775722e636f6d2f36393032704d612e706e67)

可以看到系统调用在第2章节，所以输入：man 2 open

![](https://camo.githubusercontent.com/a5eb2c7efa54b46bec767f1a677498ee9a84c9b7/68747470733a2f2f692e696d6775722e636f6d2f4e6f543974564f2e706e67)

通过这个方法可以查看open系统调用函数的内容。

### 三、文件操作

![](https://camo.githubusercontent.com/98830bc878d848ce2ee61c90740b45dab03c5fc8/68747470733a2f2f692e696d6775722e636f6d2f6a336d6f4275672e706e67)

#### 1、普通文件读写案例read_write.c：

```c
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>

int main(int argc, const char* argv[])
{
    int fd = open("english.txt", O_RDONLY);
    if(fd == -1)
    {
        perror("open error");
        exit(1);
    }

    // 读文件
    char buf[1024];
    int len = read(fd, buf, sizeof(buf));

    // 写文件
    int fd1 = open("temp", O_WRONLY | O_CREAT, 0664);
    if(fd1 == -1)
    {
        perror("open error");
        exit(1);
    }

    while( len > 0 )
    {
        // 写文件
        int ret = write(fd1, buf, len);
        len = read(fd, buf, sizeof(buf));
    }
    close(fd);
    close(fd1);

    return 0;
}
```
该程序从english.txt中读内容，并将内容写入到temp中，如果temp文件不存在则创建。

#### 2、阻塞读案例--block_read.c ：

```c
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// 阻塞读终端
int main(void)
{
	char buf[10];//字符缓存区，最大为10字节
	int n;//保存读出的值
	n = read(STDIN_FILENO, buf, 10);//从标准输入流中读最多十个字节字符串
	if (n < 0) //读取错误，退出``
	{
		perror("read STDIN_FILENO");
		exit(1);
	}
	write(STDOUT_FILENO, buf, n);//将读入的字符串写到标准输出流中
	return 0;
}
```
该程序将阻塞等待从标准输入流中读取最多十个字节，并从标准输出流中打印出来。如果输入流读取字符超过十个，那么只读取十个字节内容，其余内容将被丢弃。

#### 3、非阻塞读案例--unblock_read.c ：
```c
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#define MSG_TRY "try again\n"

// 非阻塞读终端
int main(void)
{
	char buf[10];
	int fd, n;
    // /dev/tty --> 当前打开的终端设备
	fd = open("/dev/tty", O_RDONLY | O_NONBLOCK);
	if(fd < 0) 
	{
		perror("open /dev/tty");
		exit(1);
	}
	tryagain:
	n = read(fd, buf, 10);
	if (n < 0) 
	{
        // 如果write为非阻塞，但是没有数据可读，此时全局变量 errno 被设置为 EAGAIN
		if (errno == EAGAIN) 
		{
			sleep(3);
			write(STDOUT_FILENO, MSG_TRY, strlen(MSG_TRY));
			goto tryagain;
		}
		perror("read /dev/tty");
		exit(1);
	}
	write(STDOUT_FILENO, buf, n);
	close(fd);
	return 0;
}
```

和前面阻塞读不同的是，这里的open函数还设置了O_NONBLOCK选项，设置为非阻塞。read函数不会阻塞下去，如果没有读取到来自终端的数据，那么直接运行下一行代码，这里设置sleep(3)，如果终端没有数据，每过3s就打印一条"try again\n"信息，提示用户输入并跳转继续尝试从标准输入读取数据，直到用户输入数据了，然后打印这些数据。

#### 4、超时非阻塞读--timeout_unblock_read.c :

```c
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#define MSG_TRY "try again\n"
#define MSG_TIMEOUT "timeout\n"

// 非阻塞读终端和等待超时
int main(void)
{
	char buf[10];
	int fd, n, i;
    // /dev/tty --> 当前终端设备
	fd = open("/dev/tty", O_RDONLY | O_NONBLOCK);
	if(fd<0) 
	{
		perror("open /dev/tty");
		exit(1);
	}
	for(i=0; i<5; i++) 
	{
		n = read(fd, buf, 10);
		if(n >= 0)
		break;
        // 如果write为非阻塞，但是没有数据可读，此时全局变量 errno 被设置为 EAGAIN
		if(errno != EAGAIN) 
		{
			perror("read /dev/tty");
			exit(1);
		}
		sleep(3);
		write(STDOUT_FILENO, MSG_TRY, strlen(MSG_TRY));
	}
	if(i==5)
	{
		write(STDOUT_FILENO, MSG_TIMEOUT, strlen(MSG_TIMEOUT));
	}
	else
	{
		write(STDOUT_FILENO, buf, n);
	}
	close(fd);
	return 0;
}
```
和前面程序不同，这个案例如果监测到终端没有数据输入，那么会过3s继续检测，但这种检测方式只会发生五次，每次会提示用户输入，如果五次后还是没有数据输入，那么将直接打印超时错误：

    try again
    try again
    try again
    try again
    try again
    timeout

### 四、文件属性

#### 1、文件的权限

![](https://camo.githubusercontent.com/181b3d8f220bc4ce874d11948cf75d78976e7bb7/68747470733a2f2f692e696d6775722e636f6d2f4a49315875704c2e706e67)

在Linux常见的文件类型有7种，分别如下所示：

文件属性|	文件类型
---|---
-|	常规文件，即file
d|	目录文件
b|	block device 即块设备文件，如硬盘;支持以block为单位进行随机访问
c|	character device 即字符设备文件，如键盘支持以character为单位进行线性访问
l|	symbolic link 即符号链接文件，又称软链接文件
p|	pipe 即命名管道文件
s|	socket 即套接字文件，用于实现两个进程进行通信

Linux使用以下结构获取某文件的属性：

![](https://camo.githubusercontent.com/f66f6b88151424bf6af65f128a8ee3186befe578/68747470733a2f2f692e696d6775722e636f6d2f4e4570675a43712e706e67)

![](https://camo.githubusercontent.com/adbaecd8aa9067d548e794c5360570858e462dbb/68747470733a2f2f692e696d6775722e636f6d2f514953487a6e6c2e706e67)

![](https://camo.githubusercontent.com/8bf34c6a8f62f4d701885f3563605fc98af9ef24/68747470733a2f2f692e696d6775722e636f6d2f573878576c6c412e706e67)

以之前文件操作的对象english.txt为例，使用

    stat english.txt

终端输出：

    File: ‘english.txt’
    Size: 109055    	Blocks: 213        IO Block: 1024   regular file
    Device: 1dh/29d	Inode: 41449       Links: 1
    Access: (0777/-rwxrwxrwx)  Uid: (    0/    root)   Gid: (    0/    root)
    Access: 2019-07-22 20:11:01.000000000 +0800
    Modify: 2019-02-23 10:05:10.000000000 +0800
    Change: 2019-02-23 10:05:10.000000000 +0800
    Birth: -

下面这个例子lstat查看文件的相关信息,stat.c:

```c
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>


int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("a.out filename\n");
        exit(1);
    }
    
    struct stat buf_st;
    int ret = lstat(argv[1], &buf_st);
    if(ret == -1)
    {
        perror("stat");
        exit(1);
    }

    printf("file size = %d\n", (int)buf_st.st_size);

    return 0;
}
```
输出：

    file size = 109055

该案例使用lstat获取english.txt的大小。

#### 2、文件属性函数

##### （1）文件属性

![](https://camo.githubusercontent.com/eab6760890ea5034fe5d22114719cb73b51565f0/68747470733a2f2f692e696d6775722e636f6d2f6c38373861774e2e706e67)

下面这个案例使用access来判断当前用户对某文件是否有写权限，access.c：

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("a.out filename\n");
        exit(1);
    }

    int ret = access(argv[1], W_OK);
    if(ret == -1)
    {
        perror("access");
        exit(1);
    }
    printf("you can write this file.\n");
    return 0;
}
```
##### （2）文件权限

![](https://camo.githubusercontent.com/6fa1284d6c529ce47ed32552a209e2102641d880/68747470733a2f2f692e696d6775722e636f6d2f5477576c6838412e706e67)

![](https://camo.githubusercontent.com/1bb76fb038dbed7167ebd29cb6a02bfd38979b91/68747470733a2f2f692e696d6775722e636f6d2f785279376570362e706e67)

使用方法如下：

```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>


int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        printf("a.out filename mode\n");
        exit(1);
    }
    int mode = strtol(argv[2], NULL, 8);
    int ret = chmod(argv[1], mode);
    if(ret == -1)
    {
        perror("chmod");
        exit(1);
    }

    ret = chown(argv[1], 1001, 1002);
    if(ret == -1)
    {
        perror("chown");
        exit(1);
    }
    return 0;
}
```
##### （3）修改文件大小

![](https://camo.githubusercontent.com/b4b7897efb68754aaa42e2300d303f333c094f2e/68747470733a2f2f692e696d6775722e636f6d2f38624d576962472e706e67)

使用案例truncate.c ：
```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        printf("a.out filename size\n");
        exit(1);
    }

    long int len = strtol(argv[2], NULL, 10); 
    int  aa = truncate(argv[1], len);
    if(aa == -1)
    {
        perror("truncate");
        exit(1);
    }
    return 0;
}
```

### 3、目录操作相关函数

#### (1)文件重命名

![](https://camo.githubusercontent.com/cfb316d21ee5a14bf5be15620eb5deb4c0d25aed/68747470733a2f2f692e696d6775722e636f6d2f4a727339634f502e706e67)

使用方法rename.c：
```c
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        printf("a.out oldName newName\n");
        exit(1);
    }

    int ret = rename(argv[1], argv[2]);
    if(ret == -1)
    {
        perror("rename");
        exit(1);
    }
    return 0;
}
```

#### (2)切换目录

![](https://camo.githubusercontent.com/29ef0fac142d33bc611bff92ecdd46e8e997d083/68747470733a2f2f692e696d6775722e636f6d2f67714f6d3347342e706e67)

![](https://camo.githubusercontent.com/26bf67a8368a767fb8053f7bc94a7645fd76956d/68747470733a2f2f692e696d6775722e636f6d2f33336f694a736b2e706e67)

使用方法chdir.c：

```c
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char* argv[])
{
    if(argc < 2)
    {
        printf("a.out dir\n");
        exit(1);
    }

    int ret = chdir(argv[1]);
    if(ret == -1)
    {
        perror("chdir");
        exit(1);
    }

    int fd = open("chdir.txt", O_CREAT | O_RDWR, 0777);
    if(fd == -1)
    {
        perror("open");
        exit(1);
    }
    close(fd);

    char buf[128];
    getcwd(buf, sizeof(buf));
    printf("current dir: %s\n", buf);

    return 0;
}
```

#### (3)创建和删除目录

![](https://camo.githubusercontent.com/51f7ecadb4e0f3a9e13d20617b7f90e201898589/68747470733a2f2f692e696d6775722e636f6d2f543032734663482e706e67)

![](https://camo.githubusercontent.com/5ed4341b7aa9faf81821b7339260cefef4610821/68747470733a2f2f692e696d6775722e636f6d2f705944794d59562e706e67)

案例--mkdir.c：

```c
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char* argv[])
{
    if(argc < 3)
    {
        printf("a.out newDir mode\n");
        exit(1);
    }

    int mode = strtol(argv[2], NULL, 8);
    int ret = mkdir(argv[1], mode);
    if(ret == -1)
    {
        perror("mkdir");
        exit(1);
    }
    return 0;
}
```
#### (4)遍历目录

![](https://camo.githubusercontent.com/e285db749291c337d09fa348526b692ae718432b/68747470733a2f2f692e696d6775722e636f6d2f49336f4f67454c2e706e67)

相关使用见opendir.c和readfileNum.c。

### 四、dup-dup2-fcntl的使用

#### 1、dup和dup2

![](https://camo.githubusercontent.com/f5e8ac327016e73a80a0424e55a6dce466359136/68747470733a2f2f692e696d6775722e636f6d2f4671315649674b2e706e67)

#### (1)dup用法

dup函数需要接收一个文件描述符，如果调用成功便返回一个新的文件描述符。下面这个案例会创建一个文件temp，获取到文件描述符fd，然后通过dup函数拷贝该文件描述符为fd2，通过fd2向文件temp写入一句话"让编程改变世界。。。。。。"，然后关闭fd2。然后通过fd读取temp文件内容并打印。

dup.c：

```c
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
	int fd = open("temp", O_RDWR | O_CREAT, 0664);
	if(fd == -1)
	{
		perror("open");
		exit(1);
	}

	// 复制文件描述符
	int fd2 = dup(fd);
//    int fd2 = fcntl(fd, F_DUPFD);
	// 写文件
	char* p = "让编程改变世界。。。。。。";
	write(fd2, p, strlen(p));
	close(fd2);

	char buf[1024];
	lseek(fd, 0, SEEK_SET);
	read(fd, buf, sizeof(buf));
	printf(" buf = %s\n", buf);
	close(fd);

	return 0;
}
```
#### (2)dup2用法

dup2的用法和dup类似。第一个参数是旧的文件描述符，第二个参数是新的文件描述符，该函数调用成功后，第二个参数表示的新的文件描述符将会和第一个参数旧的文件描述符指向同一个文件。下面这个案例会创建并打开两个文件temp和temp1，fd和fd2分别是这两个文件的文件描述符。然后代码调用dup2函数，让fd2和fd一样成为temp文件的文件描述符。通过fd2向temp写入一段话"change the world by programing。。。。。。"，然后关闭fd2，通过fd读取temp内容并打印。

dup2.c

```c
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>

int main(void)
{
	int fd = open("temp", O_RDWR | O_CREAT | O_APPEND, 0664);
	if(fd == -1)
	{
		perror("open");
		exit(1);
	}

	int fd2 = open("temp1", O_RDWR | O_CREAT | O_APPEND, 0664);
	if(fd2 == -1)
	{
		perror("open open");
		exit(1);
	}
	// 复制文件描述符
	dup2(fd, fd2);
	// 写文件
	char* p = "change the world by programing。。。。。。";
	write(fd2, p, strlen(p));
	close(fd2);

	char buf[1024];
	lseek(fd, 0, SEEK_SET);
	read(fd, buf, sizeof(buf));
	printf(" buf = %s\n", buf);
	close(fd);

	return 0;
}
```

#### 2、fcntl函数

![](https://camo.githubusercontent.com/aa80d04bcc299b07401edbfbf121e2542b0faf21/68747470733a2f2f692e696d6775722e636f6d2f6377333766586a2e706e67)


下面这个例子演示了如何使用fcntl来复制文件描述符。

fcntl.c：

```c
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{
	int fd = open("temp", O_RDWR | O_CREAT, 0664);
	if(fd == -1)
	{
		perror("open");
		exit(1);
	}

	// 复制文件描述符
	// int fd1 = dup(fd);
	int fd1 = fcntl(fd, F_DUPFD, 0);
	// 写文件
	char *p = "hello, world!!!";
	write(fd1, p, strlen(p)+1);
	close(fd1);

	// 读文件
	char buf[64]={0};
	lseek(fd, 0, SEEK_SET);
	read(fd, buf, sizeof(buf));
	printf("buf = %s\n", buf);
	close(fd);
}
```

下面这个例子演示了如何使用fcntl来修改文件的状态标识。这个程序以读写的方式打开了一个名为hello的文件并写入一句话，然后使用fcntl函数获取到该文件的读写权限并打印。然后给该文件添加O_APPEND权限，并再添加一句话。

fcntl_1.c

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <fcntl.h>

int main()
{
	int fd = open("hello", O_RDWR, 0664);
	if(fd == -1)
	{
		perror("open");
		exit(1);
	}
	char* t = "窗前明月光， 疑是地上霜。举头望明月，低头思故乡。";
	write(fd, t, strlen(t));

	// 获取文件状态标志
	int flag = fcntl(fd, F_GETFL, 0);
	switch(flag & O_ACCMODE)
	{
		case O_RDONLY:
			printf("read only\n");
			break;
		case O_WRONLY:
			printf("write only\n");
			break;
		case O_RDWR:
			printf("read and write\n");
			break;
		default:
			break;
	}
	if(flag & O_APPEND)
	{
		printf("append\n");
	}
	// 追加flag
	flag |= O_APPEND;
	fcntl(fd, F_SETFL, flag);
	char*p = "锄禾日当午， 编码真辛苦。";
	write(fd, p, strlen(p));
	close(fd);

	return 0;
}
```

到此，文件相关的操作就暂告一段落了。