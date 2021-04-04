<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->

- [消息队列](#%E6%B6%88%E6%81%AF%E9%98%9F%E5%88%97)
  - [一、消息队列相关的函数](#%E4%B8%80%E6%B6%88%E6%81%AF%E9%98%9F%E5%88%97%E7%9B%B8%E5%85%B3%E7%9A%84%E5%87%BD%E6%95%B0)
    - [0、消息数据格式](#0%E6%B6%88%E6%81%AF%E6%95%B0%E6%8D%AE%E6%A0%BC%E5%BC%8F)
    - [1、ftok函数](#1ftok%E5%87%BD%E6%95%B0)
    - [2、msgget函数](#2msgget%E5%87%BD%E6%95%B0)
    - [3、msgsnd 函数](#3msgsnd-%E5%87%BD%E6%95%B0)
    - [4、msgrcv 函数](#4msgrcv-%E5%87%BD%E6%95%B0)
    - [5、msgctl函数](#5msgctl%E5%87%BD%E6%95%B0)
  - [二、使用案例](#%E4%BA%8C%E4%BD%BF%E7%94%A8%E6%A1%88%E4%BE%8B)
    - [1、两程序之间收发数据](#1%E4%B8%A4%E7%A8%8B%E5%BA%8F%E4%B9%8B%E9%97%B4%E6%94%B6%E5%8F%91%E6%95%B0%E6%8D%AE)
    - [2、从消息队列中取出数据](#2%E4%BB%8E%E6%B6%88%E6%81%AF%E9%98%9F%E5%88%97%E4%B8%AD%E5%8F%96%E5%87%BA%E6%95%B0%E6%8D%AE)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## System V 消息队列

> 参考 [Linux系统编程—消息队列](https://www.jianshu.com/p/7e3045cf1ab8)

消息队列本质上是位于内核空间的链表，链表的每个节点都是一条消息。每一条消息都有自己的消息类型，消息类型用整数来表示，而且必须大于 0。每种类型的消息都被对应的链表所维护：

![](https://upload-images.jianshu.io/upload_images/16823531-baf6c22c59b827fa.png?imageMogr2/auto-orient/strip|imageView2/2/w/685/format/webp)

消息类型为 0 的链表记录了所有消息加入队列的顺序，其中红色箭头表示消息加入的顺序。其中数字 1 表示类型为 1 的消息，数字2、3、4 类似。彩色块表示消息数据，它们被挂在对应类型的链表上。

### 一、消息队列相关的函数

#### 0、消息数据格式

消息的格式如下所示：
```c
struct Msg{
    long type; // 用于标识消息类型。这个是必须的，而且值必须 > 0，这个值被系统使用
    // 消息正文，多少字节随你而定
    // ...
};
```
只要保证首4字节(32 位 linux 下的 long)是一个整数就可以，例如：
```c
struct Msg {
    long type;
    char name[20];
    int age;
} msg;

struct Msg {
    long type;
    int start;
    int end;
} msg;
```
#### 1、ftok函数

功能：生成一个key(键值)

```c
key_t ftok(const char *pathname, int proj_id)
```
用法介绍：

```c
key_t key = ftok("/home/jack",'6');
int msgid = msgget(key,IPC_CREAT|O_WRONLY|0777);
```

* ftok根据路径名，提取文件信息，再根据这些文件信息及project ID合成key，该路径可以随便设置。
* 该路径是必须存在的，ftok只是根据文件inode在系统内的唯一性来取一个数值，和文件的权限无关。
* proj_id是可以根据自己的约定，随意设置。这个数字,有的称之为project ID; 在UNIX系统上,它的取值是1到255;

#### 2、msgget函数

```c
int msgget(key_t key, int msgflg);
```
* 功能：创建或取得一个消息队列对象
* 返回：消息队列对象的id 同一个key得到同一个对象
* 参数：msgget(key,flag|mode);
  * flag:可以是0或者IPC_CREAT(不存在就创建)
  * mode:同文件权限一样

#### 3、msgsnd 函数

```c
int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
```

* 功能：该函数用于将数据发送到消息队列。如果该函数被信号打断，会设置 errno 为 EINTR。
* 返回：0 表示成功，-1 失败并设置 errno。
* 参数：msgsnd(msqid, msgp, msgsz, msgflg);
    * msqid：ipc 内核对象 id
    * msgp：消息数据地址
    * msgsz：消息正文部分的大小（不包含消息类型）
    * msgflg：可选项
        * 该值为 0：如果消息队列空间不够，msgsnd 会阻塞。
        * IPC_NOWAIT：直接返回，如果空间不够，会设置 errno 为 EAGIN。

#### 4、msgrcv 函数

```c
ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);
```
* 功能：msgrcv 函数从消息队列取出消息后，并将其从消息队列里删除。
* 返回：调用成功时，该函数返回放到接收缓存区中的字节数，消息被复制到由msg_ptr指向的用户分配的缓存区中，然后删除消息队列中的对应消息。失败时返回-1。
* 参数：msgrcv(msqid, msgp, msgsz, msgtyp, msgflg);
    * msqid：ipc 内核对象 id
    * msgp：用来接收消息数据地址
    * msgsz：消息正文部分的大小（不包含消息类型）
    * msgtyp：指定获取哪种类型的消息
        * msgtyp = 0：获取消息队列中的第一条消息
        * msgtyp > 0：获取类型为 msgtyp 的第一条消息，除非指定了 msgflg 为MSG_EXCEPT，这表示获取除了 msgtyp 类* 型以外的第一条消息。
        * msgtyp < 0：获取类型 ≤|msgtyp|≤|msgtyp| 的第一条消息。
    * msgflg：可选项
        * 如果为 0 表示没有消息就阻塞。
        * IPC_NOWAIT：如果指定类型的消息不存在就立即返回，同时设置 errno 为 ENOMSG
        * MSG_EXCEPT：仅用于 msgtyp > 0 的情况。表示获取类型不为 msgtyp 的消息
        * MSG_NOERROR：如果消息数据正文内容大于 msgsz，就将消息数据截断为 msgsz

#### 5、msgctl函数

```c
int msgctl(int msgid, int command, struct msgid_ds *buf);
```
* 功能：该函数用来控制消息队列，它与共享内存的shmctl函数相似。
* 返回：
    * 成功：0
    * 出错：-1，错误原因存于error中
    * EACCESS：参数cmd为IPC_STAT，确无权限读取该消息队列
    * EFAULT：参数buf指向无效的内存地址
    * EIDRM：标识符为msqid的消息队列已被删除
    * EINVAL：无效的参数cmd或msqid
    * EPERM：参数cmd为IPC_SET或IPC_RMID，却无足够的权限执行
* 参数：msgctl(msgid, command, buf);
    * msqid：ipc 内核对象 id
    * command：是将要采取的动作，它可以取3个值
        * IPC_STAT：获得msgid的消息队列头数据到buf中
        * IPC_SET：设置消息队列的属性，要设置的属性需先存储在buf中
        * IPC_RMID：删除消息队列
    * buf：指向msgid_ds结构的指针，它指向消息队列模式和访问权限的结构。msgid_ds结构至少包括以下成员：
        ```c
        struct msgid_ds
        {
            uid_t shm_perm.uid;
            uid_t shm_perm.gid;
            mode_t shm_perm.mode;
        };
        ```

### 二、使用案例

#### 1、两程序之间收发数据

该案例中msgsend.c会阻塞等待用户向终端输入字符，然后向消息队列中发送用户的输入数据。msgreceive.c则从消息队列中取出msgsend.c中发送的数据，并打印，直到用户输入"end"为止。

msgsend.c

```c
#include <unistd.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <string.h>  
#include <sys/msg.h>  
#include <errno.h>  
#define MAX_TEXT 512  
struct msg_st{  
    long int msg_type;  
    char text[MAX_TEXT];  
};   
int main(){  
    int running = 1;  
    struct msg_st data;  
    char buffer[BUFSIZ];  
    int msgid = -1;   
    /*建立消息队列*/ 
    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);  
    if(msgid == -1){  
        fprintf(stderr, "msgget failed with error: %d\n", errno);  
        exit(EXIT_FAILURE);  
    }  
    /*向消息队列中写消息，直到写入end*/  
    while(running){  
        /*输入数据*/  
        printf("Enter some text: ");  
        fgets(buffer, BUFSIZ, stdin);  
        data.msg_type = 1;
        strcpy(data.text, buffer);  
        /*向队列发送数据*/
        if(msgsnd(msgid, (void*)&data, MAX_TEXT, 0) == -1){  
            fprintf(stderr, "msgsnd failed\n");  
            exit(EXIT_FAILURE);  
        }  
        /*输入end结束输入*/  
        if(strncmp(buffer, "end", 3) == 0)  
            running = 0;  
        sleep(1);  
    }  
    exit(EXIT_SUCCESS);  
}  
```

msgreceive.c

```c
#include <unistd.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <string.h>  
#include <errno.h>  
#include <sys/msg.h>  
struct msg_st{  
    long int msg_type;  
    char text[BUFSIZ];  
};   
int main(){
    int running = 1;  
    int msgid = -1;  
    struct msg_st data;  
    long int msgtype = 0; 
  
    /*建立消息队列*/ 
    msgid = msgget((key_t)1234, 0666 | IPC_CREAT);  
    if(msgid == -1){  
        fprintf(stderr, "msgget failed with error: %d\n", errno);  
        exit(EXIT_FAILURE);  
    }  
    /*从队列中获取消息，直到遇到end消息为止*/  
    while(running){  
        if(msgrcv(msgid, (void*)&data, BUFSIZ, msgtype, 0) == -1){  
           fprintf(stderr, "msgrcv failed with errno: %d\n", errno);  
            exit(EXIT_FAILURE);  
        }  
        printf("You wrote: %s\n",data.text);  
        /*遇到end结束*/  
        if(strncmp(data.text, "end", 3) == 0){  
            running = 0;  
	    }
    }  
    /*删除消息队列*/
    if(msgctl(msgid, IPC_RMID, 0) == -1){  
        fprintf(stderr, "msgctl(IPC_RMID) failed\n");  
        exit(EXIT_FAILURE);  
    }  
    exit(EXIT_SUCCESS);  
}  
```

#### 2、从消息队列中取出数据

msgsend2.c向消息队列中发送了不同type的消息，然后msgreceive2.c则从消息队列中取出不同type的消息。

msgsend2.c

```c
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
        char name[20];
        int age;
}Person;

typedef struct {
        long type;
        Person person;
}Msg;

int main(int argc, char *argv) 
{
    int id = msgget(0x8888, IPC_CREAT | 0664);
        
    Msg msg[10] = {
                {1, {"Luffy", 17}},
                {1, {"Zoro", 19}},
                {2, {"Nami", 18}},
                {2, {"Usopo", 17}},
                {1, {"Sanji", 19}},
                {3, {"Chopper", 15}},
                {4, {"Robin", 28}},
                {4, {"Franky", 34}},
                {5, {"Brook", 88}},
                {6, {"Sunny", 2}}
            
    };
        
    int i;
    for (i = 0; i < 10; ++i) 
    {
        int res = msgsnd(id, &msg[i], sizeof(Person), 0);
    }
    return 0;
}
```
上面程序第一次运行完后，内核中的消息队列大概像下面这样：

![](https://upload-images.jianshu.io/upload_images/16823531-f02e045b498ff1c0.png?imageMogr2/auto-orient/strip|imageView2/2/w/592/format/webp)

msgreceive2.c

```c
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

typedef struct {
    char name[20];
    int age;
}Person;

typedef struct {
    long type;
    Person person;
}Msg;

void printMsg(Msg *msg) {
    printf("{ type = %ld, name = %s, age = %d }\n",
           msg->type, msg->person.name, msg->person.age);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("usage: %s <type>\n", argv[0]);
        return -1;
    }
    
    // 要获取的消息类型
    long type = atol(argv[1]);
    
    // 获取 ipc 内核对象 id
    int id = msgget(0x8888, 0);
   
    
    Msg msg;
    int res;
    
    while(1) {
        // 以非阻塞的方式接收类型为 type 的消息
        res = msgrcv(id, &msg, sizeof(Person), type, IPC_NOWAIT);
        if (res < 0) {
            // 如果消息接收完毕就退出，否则报错并退出
            if (errno == ENOMSG) {
                printf("No message!\n");
                break;
            }

        }
        // 打印消息内容
        printMsg(&msg);
    }
    return 0;
}
```
效果如下所示：

    $ ./recv
    usage: ./recv <type>
    $ ./recv 1
    { type = 1, name = Luffy, age = 17 }
    { type = 1, name = Zoro, age = 19 }
    { type = 1, name = Sanji, age = 19 }
    No message!
    $ ./recv 2
    { type = 2, name = Nami, age = 18 }
    { type = 2, name = Usopo, age = 17 }
    No message!
    $ ./recv 3
    { type = 3, name = Chopper, age = 15 }
    No message!
    $ ./recv 4
    { type = 4, name = Robin, age = 28 }
    { type = 4, name = Franky, age = 34 }
    No message!
    $ ./recv 5
    { type = 5, name = Brook, age = 88 }
    No message!
    $ ./recv 6
    { type = 6, name = Sunny, age = 2 }
    No message!
    $ ./recv 7
    No message!
    $ ./recv 1
    No message!

可见，如果消息被取出，那么就会从队列中消失，下次再取同样类型的消息将返回空。

## POSIX 消息队列

来自 [Linux进程通信之POSIX消息队列](https://blog.csdn.net/anonymalias/article/details/9799645)

![](./img/mq.png)