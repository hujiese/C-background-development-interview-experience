## 网络套接字相关

### 一、sockets
该类定义于SocketsOps.h中，它提供了众多对socket套接字操作的函数：
```c
int createNonblockingOrDie();

int  connect(int sockfd, const struct sockaddr_in& addr);
void bindOrDie(int sockfd, const struct sockaddr_in& addr);
void listenOrDie(int sockfd);
int  accept(int sockfd, struct sockaddr_in* addr);
ssize_t read(int sockfd, void *buf, size_t count);
ssize_t readv(int sockfd, const struct iovec *iov, int iovcnt);
ssize_t write(int sockfd, const void *buf, size_t count);
void close(int sockfd);
void shutdownWrite(int sockfd);

void toIpPort(char* buf, size_t size,
              const struct sockaddr_in& addr);
void toIp(char* buf, size_t size,
          const struct sockaddr_in& addr);
void fromIpPort(const char* ip, uint16_t port,
                  struct sockaddr_in* addr);

int getSocketError(int sockfd);

struct sockaddr_in getLocalAddr(int sockfd);
struct sockaddr_in getPeerAddr(int sockfd);
bool isSelfConnect(int sockfd);
```
该类比较简单，各个函数之间也很少互相调用，基本就是将网络套接字API封装了一遍，具体的分析可见 [muduo源码分析之SocketsOps](https://github.com/hujiese/Large-concurrent-serve/blob/master/19_muduo_SocketsOps/19_muduo_SocketsOps.md)

### 二、InetAddress 

该类主要提供了一些地址转换的函数（例如从网络字节序到本地字节序），该类非常简单，更多内容可见 [muduo源码分析之InetAddress](https://github.com/hujiese/Large-concurrent-serve/blob/master/20_muduo_InetAddress/20_muduo_InetAddress.md)

### 三、Socket

该类使用了前面两个类中的函数，主要是对socket套接字操作的进一步封装，还包括一些setsockopt的选项操作封装：
```c
/// abort if address in use
void bindAddress(const InetAddress& localaddr);
/// abort if address in use
void listen();
/// On success, returns a non-negative integer that is
/// a descriptor for the accepted socket, which has been
/// set to non-blocking and close-on-exec. *peeraddr is assigned.
/// On error, -1 is returned, and *peeraddr is untouched.
int accept(InetAddress* peeraddr);
void shutdownWrite();
///
/// Enable/disable TCP_NODELAY (disable/enable Nagle's algorithm).
///
void setTcpNoDelay(bool on);
///
/// Enable/disable SO_REUSEADDR
///
void setReuseAddr(bool on);
///
/// Enable/disable SO_KEEPALIVE
///
void setKeepAlive(bool on);
```
关于套接字选项可见网络编程部分的“套接字API”相关内容，更多内容可见 [muduo 源码分析之socket](https://github.com/hujiese/Large-concurrent-serve/blob/master/21_muduo_socket/muduo_socket.md)