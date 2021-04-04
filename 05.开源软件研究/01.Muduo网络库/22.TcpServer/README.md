<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->

- [TcpServer](#tcpserver)
  - [1、I/O 线程](#1io-%E7%BA%BF%E7%A8%8B)
  - [2、Acceptor](#2acceptor)
  - [3、启动服务器](#3%E5%90%AF%E5%8A%A8%E6%9C%8D%E5%8A%A1%E5%99%A8)
  - [4、处理客户端连接](#4%E5%A4%84%E7%90%86%E5%AE%A2%E6%88%B7%E7%AB%AF%E8%BF%9E%E6%8E%A5)
  - [5、释放资源](#5%E9%87%8A%E6%94%BE%E8%B5%84%E6%BA%90)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## TcpServer

TcpServer中融合了前面讲过的几乎所有东西。

### 1、I/O 线程
TcpServer 正如其名，是一个服务器，而且是一个可以支持多线程Reactor的服务器，所以该服务器必须要有诸多IO线程：
```c
EventLoop* loop_;  // the acceptor loop
boost::scoped_ptr<EventLoopThreadPool> threadPool_;
````
它们在构造函数中被初始化：
```c
TcpServer::TcpServer(EventLoop* loop,
                     const InetAddress& listenAddr,
                     const string& nameArg)
  : loop_(CHECK_NOTNULL(loop)),
    ...
    threadPool_(new EventLoopThreadPool(loop)),
    ...
    nextConnId_(1)
{
...
}
```
可通过该函数来创建多少个IO线程：
```c
void TcpServer::setThreadNum(int numThreads)
{
  assert(0 <= numThreads);
  threadPool_->setThreadNum(numThreads);
}
```
### 2、Acceptor

TcpServer使用Acceptor来操作监听套接字，而且需要一个监听套接字地址和端口：
```c
boost::scoped_ptr<Acceptor> acceptor_; // avoid revealing Acceptor
const string hostport_;

```
构造函数中初始化如下：
```c
TcpServer::TcpServer(EventLoop* loop,
                     const InetAddress& listenAddr,
                     const string& nameArg)
  : loop_(CHECK_NOTNULL(loop)),
    hostport_(listenAddr.toIpPort()),
    ...
    acceptor_(new Acceptor(loop, listenAddr)),
    ...
{
  acceptor_->setNewConnectionCallback(
      boost::bind(&TcpServer::newConnection, this, _1, _2));
}
```
此时的Acceptor还没有加入事件监听中（没有调用Acceptor::listen()）。Acceptor需要TcpServer传入一个函数来处理连接事件（Acceptor只负责处理连接，所以只关注读事件）：
```c
acceptChannel_.setReadCallback(
      boost::bind(&Acceptor::handleRead, this));
...
void Acceptor::handleRead()
{
    ...
  if (connfd >= 0)
  {
    if (newConnectionCallback_)
    {
      newConnectionCallback_(connfd, peerAddr);
    }
    else
    {
      sockets::close(connfd);
    }
  }
  else
  {
      ...
  }
}
```
而该函数便是TcpServer::newConnection，该函数在下面详细介绍。

### 3、启动服务器
调用该函数即可启动服务器：
```c
void TcpServer::start()
{
  if (!started_)
  {
    started_ = true;
    threadPool_->start(threadInitCallback_);
  }

  if (!acceptor_->listenning())
  {
    loop_->runInLoop(
        boost::bind(&Acceptor::listen, get_pointer(acceptor_)));
  }
}
```
该函数首先启动了IO线程池，并传入线程初始化函数，该函数作用在前面章节介绍过，TcpServer中threadInitCallback_定义如下：
```c
ThreadInitCallback threadInitCallback_;
typedef boost::function<void(EventLoop*)> ThreadInitCallback;
```
可通过该函数设置这一回调函数：
```c
void setThreadInitCallback(const ThreadInitCallback& cb){ threadInitCallback_ = cb; }
```
在初始化IO后便启动了Acceptor，Acceptor加入了事件循环。值得一提的是，如果IO线程池设置线程数量为0，那么全局只有一个EventLoop，threadPool_中的EventLoop也是指向TcpServer中的EventLoop。

### 4、处理客户端连接
Acceptor 绑定的“读事件”回调函数是newConnection，当有客户端连接时该函数将会被调用：
```c
void TcpServer::newConnection(int sockfd, const InetAddress& peerAddr)
{
  loop_->assertInLoopThread();
  EventLoop* ioLoop = threadPool_->getNextLoop();
  char buf[32];
  snprintf(buf, sizeof buf, ":%s#%d", hostport_.c_str(), nextConnId_);
  ++nextConnId_;
  string connName = name_ + buf;

  LOG_INFO << "TcpServer::newConnection [" << name_
           << "] - new connection [" << connName
           << "] from " << peerAddr.toIpPort();
  InetAddress localAddr(sockets::getLocalAddr(sockfd));
  // FIXME poll with zero timeout to double confirm the new connection
  // FIXME use make_shared if necessary
  TcpConnectionPtr conn(new TcpConnection(ioLoop,
                                          connName,
                                          sockfd,
                                          localAddr,
                                          peerAddr));
  connections_[connName] = conn;
  conn->setConnectionCallback(connectionCallback_);
  conn->setMessageCallback(messageCallback_);
  conn->setWriteCompleteCallback(writeCompleteCallback_);
  conn->setCloseCallback(
      boost::bind(&TcpServer::removeConnection, this, _1)); // FIXME: unsafe
  ioLoop->runInLoop(boost::bind(&TcpConnection::connectEstablished, conn));
}
```
该函数调用后，TcpServer将从threadPool_轮询方式获取一个EventLoop，让该EventLoop来处理新连接客户端套接字事件。然后将该TcpConnection保存到connections_中，该成员变量定义如下：
```c
typedef std::map<string, TcpConnectionPtr> ConnectionMap;
ConnectionMap connections_;
```
接下来会创建一个TcpConnection，设置该TcpConnection的对应的回调事件，其中connectionCallback_和messageCallback_在构造函数中设置了默认处理函数，这两个默认函数里没有做任何实事：
```c
connectionCallback_(defaultConnectionCallback)
messageCallback_(defaultMessageCallback)
```
可以通过如下函数来设置这些回调函数：
```c
/// Set connection callback.
/// Not thread safe.
void setConnectionCallback(const ConnectionCallback& cb)
{ connectionCallback_ = cb; }

/// Set message callback.
/// Not thread safe.
void setMessageCallback(const MessageCallback& cb)
{ messageCallback_ = cb; }

/// Set write complete callback.
/// Not thread safe.
void setWriteCompleteCallback(const WriteCompleteCallback& cb)
{ writeCompleteCallback_ = cb; }
```
需要注意的是，连接关闭回调函数绑定的是TcpServer::removeConnection：
```c
void TcpServer::removeConnection(const TcpConnectionPtr& conn)
{
  // FIXME: unsafe
  loop_->runInLoop(boost::bind(&TcpServer::removeConnectionInLoop, this, conn));
}

void TcpServer::removeConnectionInLoop(const TcpConnectionPtr& conn)
{
  loop_->assertInLoopThread();
  LOG_INFO << "TcpServer::removeConnectionInLoop [" << name_
           << "] - connection " << conn->name();
  size_t n = connections_.erase(conn->name());
  (void)n;
  assert(n == 1);
  EventLoop* ioLoop = conn->getLoop();
  ioLoop->queueInLoop(
      boost::bind(&TcpConnection::connectDestroyed, conn));
}
```
这一串函数将该TcpConnection从TcpServer的connections_中删除，TcpServer与该TcpConnection没有任何关系了，然后调用TcpConnection::connectDestroyed将该TcpConnection的连接套接字及其Channel与EventLoop及其Poller解除联系，这样一来就彻底除掉该TcpConnection了。

### 5、释放资源
将connections_保存的所有TcpConnection全部去除，去除方法和前面TcpServer::removeConnectionInLoop去除单个TcpConnection的方法是一样的。
```c
TcpServer::~TcpServer()
{
  loop_->assertInLoopThread();
  LOG_TRACE << "TcpServer::~TcpServer [" << name_ << "] destructing";

  for (ConnectionMap::iterator it(connections_.begin());
      it != connections_.end(); ++it)
  {
    TcpConnectionPtr conn = it->second;
    it->second.reset();
    conn->getLoop()->runInLoop(
      boost::bind(&TcpConnection::connectDestroyed, conn));
    conn.reset();
  }
}
```

到此，TcpServer便分析完了。