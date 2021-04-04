<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->

- [Connector](#connector)
  - [1、设计思想](#1%E8%AE%BE%E8%AE%A1%E6%80%9D%E6%83%B3)
  - [2、连接服务端](#2%E8%BF%9E%E6%8E%A5%E6%9C%8D%E5%8A%A1%E7%AB%AF)
  - [3、错误处理与重连](#3%E9%94%99%E8%AF%AF%E5%A4%84%E7%90%86%E4%B8%8E%E9%87%8D%E8%BF%9E)
  - [4、处理“写”事件](#4%E5%A4%84%E7%90%86%E5%86%99%E4%BA%8B%E4%BB%B6)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## Connector

Connector 是为TcpClient所调用的，一个客户端需要一个套接字来与服务端通信，muduo 使用Connector 来封装这个与服务端通信的套接字（如同Acceptor 封装了客户端连接套接字）。

### 1、设计思想
前面说到，该Connector 封装了一个客户端用于连接服务器的套接字，本着muduo中 Every Fd is Channel 的精神，Connector也同样遵循这样的准测：
```c
EventLoop* loop_;
boost::scoped_ptr<Channel> channel_;
```
但是成员变量中却没有套接字，该套接字是在建立连接时才创建的，待创建成功才与channel_绑定：
```c
void Connector::connect()
{
  int sockfd = sockets::createNonblockingOrDie();
  int ret = sockets::connect(sockfd, serverAddr_.getSockAddrInet());
  ...
}
```
而且从构造函数来看，该Channel也没有和EventLoop绑定，至于为什么这样做，在后面会详细介绍。
```c
Connector::Connector(EventLoop* loop, const InetAddress& serverAddr)
  : loop_(loop),
    serverAddr_(serverAddr),
    connect_(false),
    state_(kDisconnected),
    retryDelayMs_(kInitRetryDelayMs)
{
  LOG_DEBUG << "ctor[" << this << "]";
}
```
构造函数中需要传入一个EventLoop和一个服务端地址结构，至于初始化列表中的后三个变量，在本文后续会详细介绍。
### 2、连接服务端
调用Connector::start函数可启动Connector：
```c
void Connector::start()
{
  connect_ = true;
  loop_->runInLoop(boost::bind(&Connector::startInLoop, this)); // FIXME: unsafe
}
```
该函数又进而调用Connector::startInLoop函数：
```c
void Connector::startInLoop()
{
  loop_->assertInLoopThread();
  assert(state_ == kDisconnected);
  if (connect_)
  {
    connect();
  }
  else
  {
    LOG_DEBUG << "do not connect";
  }
}
```
由于一开始构造函数对state_的设置，assert(state_ == kDisconnected)成立。在Connector::start中已经将connect_设置为true，所以直接调用connect()函数，该函数定义如下：
```c
void Connector::connect()
{
  int sockfd = sockets::createNonblockingOrDie();
  int ret = sockets::connect(sockfd, serverAddr_.getSockAddrInet());
  int savedErrno = (ret == 0) ? 0 : errno;
  switch (savedErrno)
  {
    case 0:
    case EINPROGRESS:
    case EINTR:
    case EISCONN:
      connecting(sockfd);
      break;

    case EAGAIN:
    case EADDRINUSE:
    case EADDRNOTAVAIL:
    case ECONNREFUSED:
    case ENETUNREACH:
      retry(sockfd);
      break;

    case EACCES:
    case EPERM:
    case EAFNOSUPPORT:
    case EALREADY:
    case EBADF:
    case EFAULT:
    case ENOTSOCK:
      LOG_SYSERR << "connect error in Connector::startInLoop " << savedErrno;
      sockets::close(sockfd);
      break;

    default:
      LOG_SYSERR << "Unexpected error in Connector::startInLoop " << savedErrno;
      sockets::close(sockfd);
      // connectErrorCallback_();
      break;
  }
}
```
在该函数中会创建一个非阻塞的套接字，并调用sockets::connect函数，如果连接成功则调用connecting函数，如果是连接超时，非阻塞套接字返回，那么调用retry函数，其他情况下都是出现错误，直接关闭该套接字。retry这种情况会在下一节内容中介绍，这部分主要关注connecting函数的调用：
```c
void Connector::connecting(int sockfd)
{
  setState(kConnecting);
  assert(!channel_);
  channel_.reset(new Channel(loop_, sockfd));
  channel_->setWriteCallback(
      boost::bind(&Connector::handleWrite, this)); // FIXME: unsafe
  channel_->setErrorCallback(
      boost::bind(&Connector::handleError, this)); // FIXME: unsafe

  // channel_->tie(shared_from_this()); is not working,
  // as channel_ is not managed by shared_ptr
  channel_->enableWriting();
}
```
该函数将state_设置为kConnecting状态，重置channel_的值，绑定EventLoop和连接成功的套接字sockfd，设置Channel的“写”和“错误”回调函数，并使能“读”，这样一来，Channel及其连接套接字就在EventLoop的Poller中了。
Connector::handleWrite和Connector::handleError两个函数会在下几节介绍。
### 3、错误处理与重连
当连接套接字上出现错误时将调用该函数：
```c
void Connector::handleError()
{
  LOG_ERROR << "Connector::handleError";
  assert(state_ == kConnecting);

  int sockfd = removeAndResetChannel();
  int err = sockets::getSocketError(sockfd);
  LOG_TRACE << "SO_ERROR = " << err << " " << strerror_tl(err);
  retry(sockfd);
}
```
在该函数中会调用removeAndResetChannel函数，removeAndResetChannel会清除该连接套接字Channel和EventLoop的Poller绑定，释放channel_智能指针所管理的内存，并将该连接套接字返回：
```c
int Connector::removeAndResetChannel()
{
  channel_->disableAll();
  channel_->remove();
  int sockfd = channel_->fd();
  // Can't reset channel_ here, because we are inside Channel::handleEvent
  loop_->queueInLoop(boost::bind(&Connector::resetChannel, this)); // FIXME: unsafe
  return sockfd;
}

void Connector::resetChannel()
{
  channel_.reset();
}
```
回到Connector::handleError函数，在清理资源并获取到产生错误的sockfd后，该函数会再调用retry函数尝试连接：
```c
void Connector::retry(int sockfd)
{
  sockets::close(sockfd);
  setState(kDisconnected);
  if (connect_)
  {
    LOG_INFO << "Connector::retry - Retry connecting to " << serverAddr_.toIpPort()
             << " in " << retryDelayMs_ << " milliseconds. ";
    loop_->runAfter(retryDelayMs_/1000.0,
                    boost::bind(&Connector::startInLoop, shared_from_this()));
    retryDelayMs_ = std::min(retryDelayMs_ * 2, kMaxRetryDelayMs);
  }
  else
  {
    LOG_DEBUG << "do not connect";
  }
}
```
在介绍retry函数之前需要了解这两个常量：
```c
static const int kMaxRetryDelayMs = 30*1000;
static const int kInitRetryDelayMs = 500;
```
kInitRetryDelayMs指的是过500ms便发动一次连接，尝试连接到服务器；kMaxRetryDelayMs指的是尝试超时连接总时间为30*1000ms。在retry函数中调用了runAfter定时函数来尝试连接。
### 4、处理“写”事件

改函数定义如下：
```c
void Connector::handleWrite()
{
  LOG_TRACE << "Connector::handleWrite " << state_;

  if (state_ == kConnecting)
  {
    int sockfd = removeAndResetChannel();
    int err = sockets::getSocketError(sockfd);
    if (err)
    {
      LOG_WARN << "Connector::handleWrite - SO_ERROR = "
               << err << " " << strerror_tl(err);
      retry(sockfd);
    }
    else if (sockets::isSelfConnect(sockfd))
    {
      LOG_WARN << "Connector::handleWrite - Self connect";
      retry(sockfd);
    }
    else
    {
      setState(kConnected);
      if (connect_)
      {
        newConnectionCallback_(sockfd);
      }
      else
      {
        sockets::close(sockfd);
      }
    }
  }
  else
  {
    // what happened?
    assert(state_ == kDisconnected);
  }
}
```
在该函数中只是获取到连接成功套接字的文件描述符sockfd，并且将Channel清除，然后调用newConnectionCallback_函数，该函数在TcpClient中传入。为什么要将该连接Channel移除是有原因的，客户端连接套接字只有一个，而Connector中已经将该套接字与自己的Channel绑定了，只是负责连接而已，connect函数调用成功该套接字则变得可写，这时的Acceptor已经完成任务了，需要将该套接字的所有权转让给TcpClient，所以调用了newConnectionCallback_函数，而该函数在TcpClient中给该套接字绑定了一个新的Channel，该Channel和Acceptor中的Channel同属于一个EventLoop，所以转移所有权需要彻底，Acceptor中的Channel必须要被移除，否则将导致“串台”。

### 5、重置连接
“断线重连”和“重置连接”是两个概念，“短线重连”指的是在已经连接成功的情况下又断开连接，再多次尝试连接；“重置连接”指的是放弃之前的连接，从零开始连接：
```c
void Connector::restart()
{
  loop_->assertInLoopThread();
  setState(kDisconnected);
  retryDelayMs_ = kInitRetryDelayMs;
  connect_ = true;
  startInLoop();
}
```