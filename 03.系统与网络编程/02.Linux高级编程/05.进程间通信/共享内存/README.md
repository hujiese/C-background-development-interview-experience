<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->


- [POSIX 共享内存](#posix-%E5%85%B1%E4%BA%AB%E5%86%85%E5%AD%98)
  - [一、mmap 和 munmap 使用](#%E4%B8%80mmap-%E5%92%8C-munmap-%E4%BD%BF%E7%94%A8)
    - [思考问题](#%E6%80%9D%E8%80%83%E9%97%AE%E9%A2%98)
  - [二、mmap用于进程间通信](#%E4%BA%8Cmmap%E7%94%A8%E4%BA%8E%E8%BF%9B%E7%A8%8B%E9%97%B4%E9%80%9A%E4%BF%A1)
    - [1、有血缘关系进程间通信](#1%E6%9C%89%E8%A1%80%E7%BC%98%E5%85%B3%E7%B3%BB%E8%BF%9B%E7%A8%8B%E9%97%B4%E9%80%9A%E4%BF%A1)
    - [2、无血缘关系进程间通信](#2%E6%97%A0%E8%A1%80%E7%BC%98%E5%85%B3%E7%B3%BB%E8%BF%9B%E7%A8%8B%E9%97%B4%E9%80%9A%E4%BF%A1)
  - [三、POSIX 共享内存](#%E4%B8%89posix-%E5%85%B1%E4%BA%AB%E5%86%85%E5%AD%98)
    - [1、shm_open 创建了共享内存区域](#1shm_open-%E5%88%9B%E5%BB%BA%E4%BA%86%E5%85%B1%E4%BA%AB%E5%86%85%E5%AD%98%E5%8C%BA%E5%9F%9F)
    - [2、ftruncate 改变创建共享内存的大小](#2ftruncate-%E6%94%B9%E5%8F%98%E5%88%9B%E5%BB%BA%E5%85%B1%E4%BA%AB%E5%86%85%E5%AD%98%E7%9A%84%E5%A4%A7%E5%B0%8F)
    - [3、mmap / munmap](#3mmap--munmap)
    - [4、shm_unlink 删除共享内存文件](#4shm_unlink-%E5%88%A0%E9%99%A4%E5%85%B1%E4%BA%AB%E5%86%85%E5%AD%98%E6%96%87%E4%BB%B6)
    - [5、使用案例](#5%E4%BD%BF%E7%94%A8%E6%A1%88%E4%BE%8B)
- [System V 共享内存](#system-v-%E5%85%B1%E4%BA%AB%E5%86%85%E5%AD%98)
  - [一、共享内存的操作流程](#%E4%B8%80%E5%85%B1%E4%BA%AB%E5%86%85%E5%AD%98%E7%9A%84%E6%93%8D%E4%BD%9C%E6%B5%81%E7%A8%8B)
  - [二、操作函数](#%E4%BA%8C%E6%93%8D%E4%BD%9C%E5%87%BD%E6%95%B0)
    - [1、shmget -- 获取一块共享内存](#1shmget----%E8%8E%B7%E5%8F%96%E4%B8%80%E5%9D%97%E5%85%B1%E4%BA%AB%E5%86%85%E5%AD%98)
    - [2、shmat -- 创建共享内存映射](#2shmat----%E5%88%9B%E5%BB%BA%E5%85%B1%E4%BA%AB%E5%86%85%E5%AD%98%E6%98%A0%E5%B0%84)
    - [3、shmdt -- 撤销共享内存与用户进程之间的映射](#3shmdt----%E6%92%A4%E9%94%80%E5%85%B1%E4%BA%AB%E5%86%85%E5%AD%98%E4%B8%8E%E7%94%A8%E6%88%B7%E8%BF%9B%E7%A8%8B%E4%B9%8B%E9%97%B4%E7%9A%84%E6%98%A0%E5%B0%84)
    - [4、shmctl -- 控制共享内存(删除)](#4shmctl----%E6%8E%A7%E5%88%B6%E5%85%B1%E4%BA%AB%E5%86%85%E5%AD%98%E5%88%A0%E9%99%A4)
  - [三、使用案例](#%E4%B8%89%E4%BD%BF%E7%94%A8%E6%A1%88%E4%BE%8B)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## POSIX 共享内存

### 一、mmap 和 munmap 使用
![](https://camo.githubusercontent.com/6f60edf36136bfbbfa2a779b2d6c970cba34b017/68747470733a2f2f692e696d6775722e636f6d2f33543155484d342e706e67)

![](https://camo.githubusercontent.com/3c0f99197f9f436edaddc227bccd673c7ad5f1c9/68747470733a2f2f692e696d6775722e636f6d2f42724d55486c4a2e706e67)

![](https://camo.githubusercontent.com/b3b67b2455a1a8de2722afe1b85e620be4027cb3/68747470733a2f2f692e696d6775722e636f6d2f42525a6a5957622e706e67)

为了使用这两个函数，首先在当前目录下创建一个文件"test.txt"，文件内容如下：

![](https://camo.githubusercontent.com/2926764fe42f3f046d3349456cbd638bf82530f0/68747470733a2f2f692e696d6775722e636f6d2f41536c784f39462e706e67)

mmap和munmap函数使用方法如下，该案例打开了test.txt文件，并将内存映射到该文件长度大小相同空间，设置共享内存内容同步到磁盘对应文件，然后向该内存映射区写入了一字符串。最后释放内存映射区：

mmap.c

```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>


int main(int argc, const char* argv[])
{
    int fd = open("test.txt", O_RDWR);
    if(fd == -1)
    {
        perror("open error");
        exit(1);
    }

    // get file length
    // len > 0
    int len = lseek(fd, 0, SEEK_END);

    void * ptr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(ptr == MAP_FAILED)
    {
        perror("mmap error");
        exit(1);
    }
    close(fd);
 
    char buf[4096];
    // 从内存中读数据
    printf("buf = %s\n", (char*)ptr);
    strcpy(ptr, "yyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyyaaaaa");

    int ret = munmap(ptr, len);
    if(ret == -1)
    {
        perror("munmap error");
        exit(1);
    }

    return 0;
}
```

#### 思考问题

![](https://camo.githubusercontent.com/185c67794541ad11e719987cec92f83952e04b15/68747470733a2f2f692e696d6775722e636f6d2f4b584b636f4b782e706e67)

![](https://camo.githubusercontent.com/1a32e97afd8d826d2064c5ab871b64b2c0cdec8f/68747470733a2f2f692e696d6775722e636f6d2f424f37464c41362e706e67)

### 二、mmap用于进程间通信

![](https://camo.githubusercontent.com/4f983c621d3cd7c8889b33ecb1609c89815990a8/68747470733a2f2f692e696d6775722e636f6d2f41664335314f4a2e706e67)

![](https://camo.githubusercontent.com/cf15a715c95d55ebfae2c76024b843b00d12a391/68747470733a2f2f692e696d6775722e636f6d2f75304c387379322e706e67)

![](https://camo.githubusercontent.com/5894c129ceceefcf3592f53ae2488ee2d35def48/68747470733a2f2f692e696d6775722e636f6d2f423475773449482e706e67)

#### 1、有血缘关系进程间通信

parent_child_mmap.c

```c
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, const char* argv[])
{
    // 打开一个文件
    int fd = open("english.txt", O_RDWR);
    int len = lseek(fd, 0, SEEK_END);

    // 创建内存映射区
    void *ptr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(ptr == MAP_FAILED)
    {
        perror("mmap error");
        exit(1);
    }
    close(fd);

    //printf("%s", (char*)ptr);

    // 创建子进程
    pid_t pid = fork();
    if(pid == -1)
    {
        perror("fork error");
        exit(1);
    }
    if(pid > 0)
    {
        // 写数据
        strcpy((char*)ptr, "你是我儿子吗？");
        // 回收
        wait(NULL);
    }
    else if(pid == 0)
    {
        // 读数据
        printf("%s\n", (char*)ptr);
    }


//    ptr++;
    // 释放内存映射区
    int ret = munmap(ptr, len);
    if(ret == -1)
    {
        perror("munmap error");
        exit(1);
    }

    return 0;
}
```
首先父进程创建共享内存区，在此之后创建一个子进程，父进程往共享内存区内写数据，子进程负责读数据。需要注意的是，共享内存区对应的磁盘文件大小不能为零，否则会出现错误。

#### 2、无血缘关系进程间通信

该部分演示分为读程序和写程序。其中读端程序首先打开一个名为"temp"的文件，如果文件不存在则创建，然后通过ftruncate函数将文件大小设置为4096，然后创建一个共享内存，每隔1s从内存偏移1024字节处开始读取内容并打印。写端程序同样地打开或创建"temp"文件，创建关于"temp"文件的内存映射区，然后每隔2s向共享内存映射区写入一串字符。

mmap_r_ipc.c

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>


int main(int argc, char *argv[])
{
	int fd = open("temp", O_RDWR | O_CREAT, 0664);
    ftruncate(fd, 4096);
    int len = lseek(fd, 0, SEEK_END);

	void* ptr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(ptr == MAP_FAILED)
	{
		perror("mmap");
		exit(1);
	}

	while(1)
	{
		sleep(1);
		printf("%s\n", (char*)ptr+1024);
	}

	// 释放
	int ret = munmap(ptr, len);
	if(ret == -1)
	{
		perror("munmap");
		exit(1);
	}

	return 0;
}
```
mmap_w_ipc.c

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>


int main(int argc, char *argv[])
{
	int fd = open("temp", O_RDWR | O_CREAT, 0664);

	void* ptr = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(ptr == MAP_FAILED)
	{
		perror("mmap");
		exit(1);
	}

	while(1)
	{
        char*p = (char*)ptr;
        p += 1024;
		strcpy(p, "hello parent, i am your 朋友！！！\n");
		sleep(2);
	}

	// 释放
	int ret = munmap(ptr, 8192);
	if(ret == -1)
	{
		perror("munmap");
		exit(1);
	}

	return 0;
}
```

### 三、POSIX 共享内存

整理于 [POSIX共享内存](https://blog.csdn.net/qq769651718/article/details/79393592)

在Linux中，POSIX共享内存对象驻留在tmpfs伪文件系统中。系统默认挂载在/dev/shm目录下。当调用shm_open函数创建或打开POSIX共享内存对象时，系统会将创建/打开的共享内存文件放到/dev/shm目录下。

#### 1、shm_open 创建了共享内存区域

程序执行shm_open函数创建了共享内存区域,此时会在/dev/shm/创建共享内存文件。

函数原型如下：

```cpp
#include <sys/mman.h>
int shm_open(const char *name, int oflag, mode_t mode);
```
该函数创建或打开一个共享内存，参数说明如下：

* name：共享内存区的名字
* oflag:  标志位，和open的标志一样，一般填写O_CREAT|O_TRUNC|O_RDWR
* mode:  权限位，和open的mode一样

返回值：成功返回一个整数的文件描述符；错误返回-1。

#### 2、ftruncate 改变创建共享内存的大小

通过ftruncate函数改变shm_open创建共享内存的大小为页大小sysconf(_SC_PAGE_SIZE)整数倍,如果不执ftruncate函数的话，会报Bus error的错误。

函数原型如下：

```cpp
#include <unistd.h>
int ftruncate(int fd, off_t length);
```

ftruncate()会将参数fd指定的文件大小改为参数length指定的大小。

函数说明：

fd：为已打开的文件描述词，而且必须是以写入模式打开的文件
length：文件大小，如果原来的文件大小比参数length大，则超过的部分会被删去。

返回 值：成功返回 0，错误返回 -1。

错误原因（errno）：

* EBADF     参数fd文件描述词为无效的或该文件已关闭
* EINVAL    参数fd为一socket并非文件，或是该文件并非以写入模式打开

#### 3、mmap / munmap

见前面介绍。

#### 4、shm_unlink 删除共享内存文件

```cpp
// name:内存共享文件
int shm_unlink(const char *name);
```
#### 5、使用案例

见src/shmen_write.c 和 src/shmen_read.c ,分析过程和前面mmap案例类似。

编译需要加上 -lrt选项：

```
gcc shmen_write.c -o write -lrt
gcc shmen_read.c -o read -lrt
```

## System V 共享内存

System V 共享内存的操作方法和System V 消息队列类似。

system V IPC 机制下的共享内存本质是一段特殊的内存区域，进程间需要共享的数据被放在该共享内存区域中，所有需要访问该共享区域的进程都要把该共享区域映射到本进程的地址空间中去。共享内存允许一个或多个进程通过同时出现在他们的虚拟地址空间的内存进行通信，而这块虚拟内存的页面被每个共享进程的页表条目所引用，同时并不需要再所有进程的虚拟内存都有相同的地址。

简而言之：

* System V共享内存是一种最为高效的进程间通信方式，进程可以直接读写内存，而不需要任何数据的拷贝。

* 为了在多个进程间交换信息，内核专门留出了一块内存区，可以由需要访问的进程将其映射到自己的私有地址空间。进程就可以直接读写这一块内存而不要进行数据的拷贝，从而大大提高效率。

* 由于多个进程共享一段内存，因此也需要依靠某种同步机制。

### 一、共享内存的操作流程

1、创建/打开共享内存

2、映射共享内存，即把指定的共享内存映射到进程的地址空间用于访问

3、撤销共享内存的映射

4、删除共享内存对象

### 二、操作函数

#### 1、shmget -- 获取一块共享内存

函数原型如下：
```cpp
#include<sys/ipc.h>
#include<sys/shm.h>

int shmget(key_t key, size_t size, int shmflg)
```
参数说明如下：

* key

    下面两种方式分配的共享内存，一般用来亲缘关系的进程间通信：

    * 当key取值为IPC_PRIVATE,则函数shmget()将创建一块新的共享内存
    * 当key取值为0,而参数shmflg中设置了IPC_CREATE这个标志，则同样创建一块新的共享内存

    可过ftok函数获取键值key：
    ```cpp
    #include<sys/types.h>
    #include<sys/ipc.h>

    key_t ftok(const char * pathname, int proj_id)

    /*pathname 就是指定的文件名（该文件必须是存在而且可以访问的）
    id是标示符，和pathname一起完成创建键值的参数，虽然为int，但只有8个比特被使用，一般我们写一个字符代替*/
    ```

* size

    是要建立共享内存的长度。所有的内存分配操作都是以页为单位的。所以如果一个进程只申请一块只有体格字节的内存，内存也会分配整整一页（在i386机器中一页的缺省大小PACE_SIZE = 4096B）。

* shmflg

    指定创建或打开的标志和读写的权限（ipc_perm中的mode成员）。有效的标志包括IPC_CREAT和IPC_EXCL，他们的功能与opene的O_CREAT和O_EXCL相当：

    * IPC_CREAT：如果共享内存不存在，则创建一个共享内存，否则直接打开已存在的。
    * IPC_EXCL：只有在共享内存不存在的时候，新的内存才建立，否则就产生错误。

返回值：调用成功返回一个shmid(类似打开一个或创建一个文件获得的文件描述符一样)；调用失败返回-1。

#### 2、shmat -- 创建共享内存映射

函数shmat将标示符为shmid共享内存映射到调用进程的地址空间中：
```cpp
#include<sys/types.h>
#include<sys/shm.h>

void * shmat(int shmid, const void *shmaddr, int shmflg);
```

参数说明：

* shmid：要映射的共享内存区标示符
* shmaddr：将共享内存映射到指定地址(若为NULL，则表示由系统自动完成映射)

* shmflg：SHM_RDONLY 共享内存只读；默认值为0，表示共享内存可读写

返回值：调用成功返回映射后的地址，出错返回(void *)-1。

#### 3、shmdt -- 撤销共享内存与用户进程之间的映射

```cpp
int shmdt(const void * shmadr);
```

参数shmaddr 是 shmat映射成功返回的地址。

注意：当一个进程不再需要共享内存段时，它将调用shmdt()系统调用取消这个段，但是这并不是从内核真正地删除这个段，而是把相关shmid_ds结构的shm_nattch域的值减1，当这个值为0时，内核才从物理上删除这个共享段。

#### 4、shmctl -- 控制共享内存(删除)
```cpp
#inlcude<sys/ipc.h>
#include<sys/shm.h>

int shmctl(int shmid, int cmd, struct shmid_ds *buf);
```
参数说明：

* shmid：共享内存标示符ID

* cmd：
    * IPC_STAT 得到共享内存的状态
    * IPC_SET 改变共享内存的状态
    * IPC_RMID 删除共享内存

* buf：是一个结构体指针。IPC_STAT的时候，取得的状态放在这个结构体中。如果要改变共享内存的状态，用这个结构体 struct shmid_ds 指定:
    ```cpp
    struct shmid_ds {
        struct ipc_perm shm_perm;    /* Ownership and permissions */
        size_t          shm_segsz;   /* Size of segment (bytes) */
        time_t          shm_atime;   /* Last attach time */
        time_t          shm_dtime;   /* Last detach time */
        time_t          shm_ctime;   /* Last change time */
        pid_t           shm_cpid;    /* PID of creator */
        pid_t           shm_lpid;    /* PID of last shmat(2)/shmdt(2) */
        shmatt_t        shm_nattch;  /* No. of current attaches */
        ...
    };
    
    The ipc_perm structure is defined as follows (the highlighted fields are settable using IPC_SET):
    struct ipc_perm {
        key_t          __key;    /* Key supplied to shmget(2) */
        uid_t          uid;      /* Effective UID of owner */
        gid_t          gid;      /* Effective GID of owner */
        uid_t          cuid;     /* Effective UID of creator */
        gid_t          cgid;     /* Effective GID of creator */
        unsigned short mode;     /* Permissions + SHM_DEST and
                                    SHM_LOCKED flags */
        unsigned short __seq;    /* Sequence number */
    };
    ```

注意：

* IPC_RMID命令实际上不从内核删除一个段，而是仅仅把这个段标记为删除，实际的删除发生最后一个进程离开这个共享段时。
* 当cmd为IPC_RMID时，第三个参数应为NULL。用这个函数“删除”共享内存。
* 如果在代码中没有手动删除，共享内存并不会随着程序的终止而自动清理。

### 三、使用案例

shmwrite 创建一块共享内存，并向其中写入一个结构体；shmread 则获取到该共享内存中结构体的内容，案例具体如下：

shmwrite.cpp：

```cpp
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <cstring>

using namespace std;

#define ERR_EXIT(m) \
        do \
        {   \
            perror(m);  \
            exit(EXIT_FAILURE); \
        } while (0);

struct student
{
    char name[32];
    int age;
};

int main(int argc, char** argv)
{
    int shmid;  // 共享内存标识符

    // 创建共享内存
    shmid = shmget((key_t)1234, sizeof(student), 0666 | IPC_CREAT);
    if (shmid == -1)
    {
        ERR_EXIT("shmget");
    }

    // 第一次创建完共享内存时，它还不能被任何进程访问，shmat()函数的作用就是用来启动对该共享内存的访问，并把共享内存连接到当前进程的地址空间
    // 将共享内存链接到当前进程的地址空间
    void *shm = shmat(shmid, NULL, 0);
    if (shm == (void *)-1)
    {
        ERR_EXIT("shmat");
    }

    // 设置共享内存
    student *shared = (struct student*) shm;
    strcpy(shared->name, "hello");
    shared->age = 20;

    while (true)
    {
        if (memcmp(shared->name, "quit", 4) == 0)
        {
            break;
        }
    }
    // 把共享内存从当前进程中分离
    if (shmdt(shm) == -1)
    {
        ERR_EXIT("shmdt");
    }

    shmctl(shmid, IPC_RMID, 0);
    return 0;
}
```

shmread.cpp：

```cpp
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <cstring>

using namespace std;

#define ERR_EXIT(m) \
        do \
        {   \
            perror(m);  \
            exit(EXIT_FAILURE); \
        } while (0);

struct student
{
    char name[32];
    int age;
};

int main(int argc, char** argv)
{
    int shmid;  // 共享内存标识符

    // 创建共享内存
    shmid = shmget((key_t)1234, 0, 0);
    if (shmid == -1)
    {
        ERR_EXIT("shmget");
    }

    // 第一次创建完共享内存时，它还不能被任何进程访问，shmat()函数的作用就是用来启动对该共享内存的访问，并把共享内存连接到当前进程的地址空间
    // 将共享内存链接到当前进程的地址空间
    void *shm = shmat(shmid, 0, 0);
    if (shm == (void *)-1)
    {
        ERR_EXIT("shmat");
    }

    // 设置共享内存
    student *shared = (struct student*) shm;
    printf("student name: %s, age: %d\n", shared->name, shared->age);

    memcpy(shared, "quit", 4);

    if (shmdt(shm) == -1)
    {
        ERR_EXIT("shmdt");
    }

    return 0;
}
```