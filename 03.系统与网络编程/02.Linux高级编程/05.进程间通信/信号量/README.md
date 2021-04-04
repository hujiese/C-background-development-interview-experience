<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->

- [System V 信号量](#system-v-%E4%BF%A1%E5%8F%B7%E9%87%8F)
  - [一、P && V](#%E4%B8%80p--v)
  - [二、System V 信号量操作函数](#%E4%BA%8Csystem-v-%E4%BF%A1%E5%8F%B7%E9%87%8F%E6%93%8D%E4%BD%9C%E5%87%BD%E6%95%B0)
    - [1、相关结构体](#1%E7%9B%B8%E5%85%B3%E7%BB%93%E6%9E%84%E4%BD%93)
    - [2、semget 创建/获取信号量](#2semget-%E5%88%9B%E5%BB%BA%E8%8E%B7%E5%8F%96%E4%BF%A1%E5%8F%B7%E9%87%8F)
    - [3、semop 信号量 P/V 操作](#3semop-%E4%BF%A1%E5%8F%B7%E9%87%8F-pv-%E6%93%8D%E4%BD%9C)
    - [4、semctl 控制信号量](#4semctl-%E6%8E%A7%E5%88%B6%E4%BF%A1%E5%8F%B7%E9%87%8F)
  - [三、使用案例](#%E4%B8%89%E4%BD%BF%E7%94%A8%E6%A1%88%E4%BE%8B)
    - [案例一--信号量操作函数合集，详见 src/sem.cpp](#%E6%A1%88%E4%BE%8B%E4%B8%80--%E4%BF%A1%E5%8F%B7%E9%87%8F%E6%93%8D%E4%BD%9C%E5%87%BD%E6%95%B0%E5%90%88%E9%9B%86%E8%AF%A6%E8%A7%81-srcsemcpp)
    - [案例二--父子进程通信](#%E6%A1%88%E4%BE%8B%E4%BA%8C--%E7%88%B6%E5%AD%90%E8%BF%9B%E7%A8%8B%E9%80%9A%E4%BF%A1)
    - [案例三--解决哲学家就餐问题](#%E6%A1%88%E4%BE%8B%E4%B8%89--%E8%A7%A3%E5%86%B3%E5%93%B2%E5%AD%A6%E5%AE%B6%E5%B0%B1%E9%A4%90%E9%97%AE%E9%A2%98)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## System V 信号量

参考 [Linux-信号量与P,V操作](https://www.cnblogs.com/goodswarm/p/10496715.html)

system v信号量又被称为system v信号量集。它的本质就是一个计数器，用来为多个进程共享的数据结构提供受控访问。

System V 信号量在内核中维护，其中包括二值信号量 、计数信号量、计数信号量集：

* 二值信号量 ： 其值只有0、1 两种选择，0表示资源被锁，1表示资源可用
* 计数信号量：其值在0 和某个限定值之间，不限定资源数只在0 1 之间
* 计数信号量集 ：多个信号量的集合组成信号量集

其中使用最广泛的信号量为二元信号量。

### 一、P && V

PV操作由P操作原语和V操作原语组成（原语是不可中断的过程），对信号量进行操作，具体定义如下：

* P（S）

    1、将信号量S的值减1，即S=S-1；

    2、如果S>=0，则该进程继续执行；否则该进程置为等待状态，排入等待队列。

* V（S）

    1、将信号量S的值加1，即S=S+1；

    2、如果S>0，则该进程继续执行；否则释放队列中第一个等待信号量的进程。

PV操作的意在于用信号量及PV操作来实现进程的同步和互斥。PV操作属于进程的低级通信。

信号量S>=0时，S表示可用资源的数量。执行一次P操作意味着请求分配一个单位资源，因此S的值减1；当S<0时，表示已经没有可用资源，请求者必须等待别的进程释放该类资源，它才能运行下去。而执行一个V操作意味着释放一个单位资源，因此S的值加1；若S<=0，表示有某些进程正在等待该资源，因此要唤醒一个等待状态的进程，使之运行下去。

利用信号量和PV操作实现进程互斥的一般模型是：

进程P1   |     进程P2     |    ……     |    进程Pn
---|---|---|---
 ……      |      ……    |       ……|
P（S）   |     P（S）   |      ……    |       P（S）
临界区   |    临界区  |     ……   |        临界区
V（S）  |     V（S） |    ……    |        V（S）
……     |        ……      |    ……    |        ……

### 二、System V 信号量操作函数

#### 1、相关结构体

内核维护的信号量集结构信息如，定义在头文件<sys/sem.h>中：

```cpp
struct semid_ds {
    struct     ipc_perm     sem_perm; // 权限
    struct     sem          *sem_base; // 队列第一个信号量
    ushort                  sem_nsems; // 信号量个数
    time_t                  sem_otime; // 最近semop时间
    time_t                  sem_ctime; // 最近修改时间
};
```

其中ipc_perm 结构是内核给每个进程间通信对象维护的一个信息结构，其成员包含所有者用户id，所有者组id、创建者及其组id，以及访问模式等；semid_ds结构体中的sem结构是内核用于维护某个给定信号量的一组值的内部结构，其结构定义：

```cpp
struct sem {
   int semval;     /* 当前信号量的值 */
   int sempid;     /* 最后一个成功操作该信号量的进程id（该结构体在内核以双向链表进行维护） */
   struct list_head sem_pending; /* pending single-sop operations */
 };
```
该结构体会在后续介绍如何使用。

 #### 2、semget 创建/获取信号量

使用semget创建一个信号量或访问一个已经存在的信号量集：
```cpp
int semget(key_t key, int nsems, int semflg);
```
参数说明：

* key：表示信号集的名字，一般由ftok函数产生
* nsems：表示信号集中信号量的个数
* semflg：用来标识信号量集合的权限，和创建文件类似，由九个权限标志为构成如0644，还可以和以下参数一起使用：

    * IPC_CREAT 表示如果key不存在就创建
    * IPC_EXCL 表示如果key存在就返回失败
    * IPC_NOWAIT 表示如果需要等待，则直接返回错误

返回值：成功返回一个非负整数即该信号量的标识码；失败返回-1。

示例：

```cpp
key_t key = ftok(".", 's');
int emid = semget(key, 1, IPC_CREAT | IPC_EXCL | 0666);
if (semid == -1)
{
    ERR_EXIT("semget");
}
```

#### 3、semop 信号量 P/V 操作

```cpp
int semop(int semid, struct sembuf *sops, unsigned nsops);
```
参数说明：

* semid：信号量标示符
* sops：一个结构体数组指针，结构体定义在sys/sem.h中，结构体如下：
    ```cpp
    struct sembuf
    {
        unsigned short sem_num;     /*semaphore index in array*/
        short sem_op;               /*semaphore operation*/
        short sem_flg;              /*operation flags*/
    };
    ```
    * sem_num标明它是信号量集的第几个元素，从0开始。
    * sem_op指定信号量采取的操作；<0相当于P操作，占有资源；>0相当于V操作，释放资源；=0进程睡眠直到信号量的值为0。
    * sem_flg指明操作的执行模式，两个标志位。一个是IPC_NOWAIT，指明以非阻塞方式操作信号量。一个是SEM_UNDO，指明内核为信号量操作保留恢复值。

* nops：第二个参数的操作数组（sops）的个数

返回值：正确返回0，错误时返回-1。

示例：

```cpp
// P 操作
struct sembuf sembuf;
sembuf.sem_num = 0;
sembuf.sem_op = -1;
sembuf.sem_flg = 0;
int ret;
ret = semop(semid, &sembuf, 1);
if (ret == -1)
{
    ERR_EXIT("sem_p")
}

// V 操作
struct sembuf sembuf;
sembuf.sem_num = 0;
sembuf.sem_op = 1;
sembuf.sem_flg = 0;
int ret;
ret = semop(semid, &sembuf, 1);
if (ret == -1)
{
    ERR_EXIT("sem_v")
}
```

#### 4、semctl 控制信号量

```cpp
int semctl(int semid ,int semnum ,int cmd ,union semun arg);
```

参数说明：

* semid：信号量集的标识符，由semget()得到
* semnum：指定semid信号量集的第几个信号量，在撤销信号量集时，此参数可缺省
* cmd：指定操作类型：

    取值|	含义
    ---|---
    GETVAL|	返回semnum指定的信号量的semval域值
    SETVAL|	指定semval域值为arg.val
    GETPID|	返回semnum指定信号量sempid
    GETNCNT|	返回semncnt
    GETZCNT|	返回semzcnt
    GETALL|	返回所有信号量的值，结果保存到arg.array中
    SETALL|	通过arg.array更新所有信号量的值
    IPC_STAT|	获取信号量集的arg.array，存入arg.buf
    IPC_SET|	将arg.buf数据结构的sem_perm.uid,sem_perm.gid,sem_perm.mode赋给sem_array，此操作仅限root、sem_perm.cuid或sem_perm.uid
    IPC_RMID|	删除指定信号量集。此操作仅限root、sem_perm.cuid或sem_perm.uid
    IPC_INFO|	获取信号量集的相关信息存放于arg.buf中

* arg：5中数据类型的共同体类型semnu，该类型在include/linux/sem.h中定义：

    ```cpp
    union senum
    {
        int val;                    /*value for setval*/
        struct semi_ds *buf;        /*buffer for IPC_STAT & IPC_SET*/
        unsigned short *array;      /*array for GETALL & SETALL*/
        struct seminfo *_buf;       /*buffer for IPC_INFO*/
        void *_pad
    };
    ```

示例：

```cpp
// 设置信号量值
union semun su;
su.val = val;
int ret;
ret = semctl(semid, 0, SETVAL, su);
if (ret == -1)
{
    ERR_EXIT("setval")
}

// 获取信号量值
int ret;
ret = semctl(semid, 0, GETVAL);
if (ret == -1)
{
    ERR_EXIT("getval")
}
printf("sem.val = %d\n", ret);

// 删除信号量
int ret;
ret = semctl(semid, 0, IPC_RMID, NULL);
if (ret == -1)
{
    ERR_EXIT("rm_sem")
}
```

### 三、使用案例

#### 案例一--信号量操作函数合集，详见 src/sem.cpp

运行结果如下：

```
$ ./app
semtool -c 创建信号量
semtool -d 删除信号量
semtool -g 获得值
semtool -s <val> 设置值
semtool -p p操作
semtool -v v操作 
semtool -f 查看权限
semtool -m <mode> 更改权限
$ ./app -c
$ ./app -g
sem.val = 0
$ ./app -s 1
$ ./app -g
sem.val = 1
$ ./app -p
sem.val = 0
$ ./app -v
sem.val = 1
$ ./app -d
$ ./app -g
semget: No such file or directory
```

#### 案例二--父子进程通信

代码见src/sem2.cpp

父子进程操作同一个信号量：
```cpp
if (pid > 0)
{
    sem_setval(sigmid, 1);
    print(sigmid, "o");
}
else
{
    print(sigmid, "x");
}
````

信号量一开始设置为0，父进程中先对信号量设置为一，然后执行print操作，子进程中同样也执行print操作，接下来看该函数：

```cpp
void print(int sigmid, char* s)
{
    int i = 0;
    int pause_time;
    for (int i = 0; i < 10; ++i)
    {
        sem_p(sigmid);

        printf("%s", s);
        fflush(stdout);
        pause_time = rand() % 3;
        sleep(pause_time);
        printf("%s", s);
        fflush(stdout);

        sem_v(sigmid);

        pause_time = rand() % 2;
        sleep(pause_time);
    }
}
```
在该函数中先对信号量取P操作，在这一步后，信号量的值为0，其他进程都会阻塞等待，取P之后，进行两次打印字符操作，在这之间休眠一段时间，最后执行V操作，唤醒阻塞进程。该信号量为二值信号量，非1即0，print函数P和V操作之间的过程是不能被打断的，一个进程一旦执行P操作，可以认为已经获取到临界区资源，只有通过V操作才能释放该资源。最后的演示效果如下：

```
ooxxooxxooxxooooxxooxxooxxooxxoooo
```
打印结果中o和x是两个两个出现的，sleep期间并没有被打断。

#### 案例三--解决哲学家就餐问题

代码在src/sem3.cpp中

思路是每个哲学家只能同时拥有两根筷子，不存在只获取一根筷子的情况。每个哲学家要双手同时抢筷子，要么抢到吃面，要么就不吃：

```cpp
void philosopher(int no)
{
    while (1)
    {
        printf("%d is thinking\n", no);
        sleep(rand() % 3);
        printf("%d is hungry\n", no);
        get2fork(no);
        printf("%d is eating\n", no);
        sleep(rand() % 3);
        put2fork(no);
    }
}

// 抢筷子
void get2fork(int pid)
{
    unsigned short int left = pid;
    unsigned short int right = (pid + 1) % 5;
    struct sembuf sembuf[2] = {{left, -1, 0}, {right, -1, 0}};
    int ret = semop(sigmid, sembuf, 2);
    if (ret == -1)
    {
        ERR_EXIT("get2fork")
    }
};

 // 放下筷子
void put2fork(int pid)
{
    unsigned short int left = pid;
    unsigned short int right = (pid + 1) % 5;
    struct sembuf sembuf[2] = {{left, 1, 0}, {right, 1, 0}};
    int ret = semop(sigmid, sembuf, 2);
    if (ret == -1)
    {
        ERR_EXIT("get2fork")
    }
};
```
效果如下：

```
1 is thinking
2 is thinking
3 is thinking
4 is thinking
0 is thinking
1 is hungry
1 is eating
2 is hungry
3 is hungry
3 is eating
4 is hungry
0 is hungry
3 is thinking
3 is hungry
1 is thinking
1 is hungry
2 is eating
4 is eating
2 is thinking

...
```