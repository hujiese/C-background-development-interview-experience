<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->

- [socket IPC -- 本地套接字](#socket-ipc----%E6%9C%AC%E5%9C%B0%E5%A5%97%E6%8E%A5%E5%AD%97)
  - [一、本地套接字通信](#%E4%B8%80%E6%9C%AC%E5%9C%B0%E5%A5%97%E6%8E%A5%E5%AD%97%E9%80%9A%E4%BF%A1)
  - [二、进程间发送文件描述符](#%E4%BA%8C%E8%BF%9B%E7%A8%8B%E9%97%B4%E5%8F%91%E9%80%81%E6%96%87%E4%BB%B6%E6%8F%8F%E8%BF%B0%E7%AC%A6)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## socket IPC -- 本地套接字

### 一、本地套接字通信

socket API 原本是为网络通讯设计的，但后来在socket 的框架上发展出一种IPC 机制，就是UNIX Domain
Socket。虽然网络socket 也可用于同一台主机的进程间通讯（通过loopback 地址127.0.0.1），但是UNIX
Domain Socket 用于IPC 更有效率：不需要经过网络协议栈，不需要打包拆包、计算校验和、维护序号和应答等，
只是将应用层数据从一个进程拷贝到另一个进程。这是因为，IPC 机制本质上是可靠的通讯，而网络协议是为不可
靠的通讯设计的。UNIX Domain Socket 也提供面向流和面向数据包两种API 接口，类似于TCP 和UDP，但是
面向消息的UNIX Domain Socket 也是可靠的，消息既不会丢失也不会顺序错乱。

UNIX Domain Socket 是全双工的，API 接口语义丰富，相比其它IPC 机制有明显的优越性，目前已成为使
用最广泛的IPC 机制，比如X Window 服务器和GUI 程序之间就是通过UNIX Domain Socket 通讯的。

使用UNIX Domain Socket 的过程和网络socket 十分相似，也要先调用socket()创建一个socket 文件描
述符，address family 指定为AF_UNIX，type 可以选择SOCK_DGRAM 或SOCK_STREAM，protocol 参数仍
然指定为0 即可。

UNIX Domain Socket 与网络socket 编程最明显的不同在于地址格式不同，用结构体sockaddr_un 表示，
网络编程的socket 地址是IP 地址加端口号，而UNIX Domain Socket 的地址是一个socket 类型的文件在文件
系统中的路径，这个socket 文件由bind()调用创建，如果调用bind()时该文件已存在，则bind()错误返回。

对比网络套接字地址结构和本地套接字地址结构：

```c
struct sockaddr_in {
    sa_family_t sin_family; /* Address family */ 地址结构类型
    unsigned short sin_port; /* Port number */ 端口号
    struct in_addr sin_addr; /* Internet address */ IP 地址
};

struct sockaddr_un {
    sa_family_t sun_family; /* AF_UNIX */ 地址结构类型
    char sun_path[UNIX_PATH_MAX]; /* pathname */ socket 文件名(含路径)
};
```
以下程序将UNIX Domain socket 绑定到一个地址：
```c
// 不同系统的sockaddr_un内部实现不同，所以要用该方法求结构体实际长度，而非使用sizeof方法
size = offsetof(struct sockaddr_un, sun_path) + strlen(un.sun_path);
// 假设该结构从地址0开始，此表达式求得成员起始地址的整型值
#define offsetof(type, member) ((int)&((type *)0)->member)
```
* 非常奇怪，我尝试使用sizeof获取sockaddr_un结构体的长度也没有问题。

src目录下有一个本地套接字通信的案例。本地套接字通信时会根据传入的“套接字地址”，生成一个文件来进行通信，在通信前需要保证该通信文件不存在，否则会出错。

src\local_socket 目录下使用UDP和TCP两种方法实现本地套接字，使用sizeof获取sockaddr_un结构体长度。

### 二、进程间发送文件描述符

请务必参考：

[Linux网络编程 -- socketpair的使用](./img/socketpair1.png)

> https://blog.csdn.net/y396397735/article/details/50684558

[通过UNIX域套接字传递描述符和 sendmsg/recvmsg 函数](./img/socketpair2.png)

> https://www.cnblogs.com/mickole/articles/3204406.html

问题：为什么不直接将文件描述符当成数据传递，而是使用sendmsg函数来传递复杂的结构体。