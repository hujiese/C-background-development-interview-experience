<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->

- [TcpConnection](#tcpconnection)
  - [1、设计思想](#1%E8%AE%BE%E8%AE%A1%E6%80%9D%E6%83%B3)
  - [2、TCP 网络编程最本质](#2tcp-%E7%BD%91%E7%BB%9C%E7%BC%96%E7%A8%8B%E6%9C%80%E6%9C%AC%E8%B4%A8)
  - [3、发送和接收缓冲区](#3%E5%8F%91%E9%80%81%E5%92%8C%E6%8E%A5%E6%94%B6%E7%BC%93%E5%86%B2%E5%8C%BA)
  - [4、连接事件处理](#4%E8%BF%9E%E6%8E%A5%E4%BA%8B%E4%BB%B6%E5%A4%84%E7%90%86)
  - [5、“读”事件处理](#5%E8%AF%BB%E4%BA%8B%E4%BB%B6%E5%A4%84%E7%90%86)
  - [6、发送数据](#6%E5%8F%91%E9%80%81%E6%95%B0%E6%8D%AE)
  - [7、“写”事件处理](#7%E5%86%99%E4%BA%8B%E4%BB%B6%E5%A4%84%E7%90%86)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## TcpConnection

如果说前面的Acceptor代表了监听连接的套机子listenfd，那么TcpConnection 就代表了已连接的客户端套接字connfd。

### 1、设计思想
TcpConnection 代表的是客户端连接套接字，所以TcpConnection 内部一定会管理一个文件描述符，以及该文件描述符对应的Channel，该Channel也需要关联一个EventLoop：
```cpp
EventLoop* loop_;
boost::scoped_ptr<Socket> socket_;
boost::scoped_ptr<Channel> channel_;
```
对于TcpConnection中的socket_，TcpConnection还使用了两个地址结构来分别保存本机IP地址和对等方IP地址：
```cpp
InetAddress localAddr_;
InetAddress peerAddr_;
```
使用一个状态flage来保存当前套接字的连接状态（未连接，连接还是断开状态）：
```c
StateE state_;  // FIXME: use atomic variable
```
这些变量都在构造函数中被初始化了：
```c
TcpConnection::TcpConnection(EventLoop* loop,
                             const string& nameArg,
                             int sockfd,
                             const InetAddress& localAddr,
                             const InetAddress& peerAddr)
  : loop_(CHECK_NOTNULL(loop)),
    name_(nameArg),
    state_(kConnecting),
    socket_(new Socket(sockfd)),
    channel_(new Channel(loop, sockfd)),
    localAddr_(localAddr),
    peerAddr_(peerAddr),
    highWaterMark_(64*1024*1024)
{
  channel_->setReadCallback(
      boost::bind(&TcpConnection::handleRead, this, _1));
  channel_->setWriteCallback(
      boost::bind(&TcpConnection::handleWrite, this));
  channel_->setCloseCallback(
      boost::bind(&TcpConnection::handleClose, this));
  channel_->setErrorCallback(
      boost::bind(&TcpConnection::handleError, this));
  LOG_DEBUG << "TcpConnection::ctor[" <<  name_ << "] at " << this
            << " fd=" << sockfd;
  socket_->setKeepAlive(true);
}
```
* 构造函数初始化列表中的name_指的是该TcpConnection的名字，需要传入。highWaterMark_“高水位”标识在后面会介绍。

* 构造函数将state_ 设置为了kConnecting状态，意思是还没有完成连接，当然还有kConnected和kDisconnected两种状态，这在后面会具体介绍。

* 在构造函数中设置了Channel的读、写、关闭和错误回调函数，这些回调函数的具体内容会在后续介绍。

* 构造函数中，设置了socket_->setKeepAlive(true)，设置套接字保持连接（TCP心跳），该选项作用详见《网络编程》的API介绍部分。

构造函数调用之后，该连接套接字还没有和EventLoop中的Poller监听，因为Channel还没有调用enableXXX函数，该函数会在建立连接回调函数TcpConnection::connectEstablished()调用，这在后续会详细介绍。

总之，一个TcpConnection代表了一个连接套接字及其Channel。

### 2、TCP 网络编程最本质

muduo的作者陈硕认为，TCP 网络编程最本质的是处理三个半事件：

* （1）连接的建立，包括服务端接受(accept) 新连接和客户端成功发起(connect) 连
接。TCP 连接一旦建立，客户端和服务端是平等的，可以各自收发数据。
* （2）连接的断开，包括主动断开(close 或shutdown) 和被动断开(read(2) 返回0)。
* （3）消息到达，文件描述符可读。这是最为重要的一个事件，对它的处理方式决定
了网络编程的风格（阻塞还是非阻塞，如何处理分包，应用层的缓冲如何设计
等等）。
* （3.5）消息发送完毕，这算半个。对于低流量的服务，可以不必关心这个事件；另外，
这里“发送完毕”是指将数据写入操作系统的缓冲区，将由TCP 协议栈负责数
据的发送与重传，不代表对方已经收到数据。

对于这“三个半事件”，在TcpConnection中体现如下：
```c
ConnectionCallback connectionCallback_;
MessageCallback messageCallback_;
WriteCompleteCallback writeCompleteCallback_;
CloseCallback closeCallback_;

typedef boost::shared_ptr<TcpConnection> TcpConnectionPtr;
typedef boost::function<void (const TcpConnectionPtr&)> ConnectionCallback;
typedef boost::function<void (const TcpConnectionPtr&)> CloseCallback;
typedef boost::function<void (const TcpConnectionPtr&)> WriteCompleteCallback;
// the data has been read to (buf, len)
typedef boost::function<void (const TcpConnectionPtr&,
                              Buffer*,
                              Timestamp)> MessageCallback;

void defaultConnectionCallback(const TcpConnectionPtr& conn);
void defaultMessageCallback(const TcpConnectionPtr& conn,
                            Buffer* buffer,
                            Timestamp receiveTime);
```
并提供了如下函数来设置这些回调函数：
```c
void setConnectionCallback(const ConnectionCallback& cb)
{ connectionCallback_ = cb; }

void setMessageCallback(const MessageCallback& cb)
{ messageCallback_ = cb; }

void setCloseCallback(const CloseCallback& cb)
{ closeCallback_ = cb; }

void setWriteCompleteCallback(const WriteCompleteCallback& cb)
{ writeCompleteCallback_ = cb; }
```
这几个函数都是在TcpServer中传递的，所以在后续的TcpServer分析中会具体介绍这些函数。

### 3、发送和接收缓冲区
TcpConnection使用Buffer作为发送和接收缓冲区：
```c
Buffer inputBuffer_;
Buffer outputBuffer_; // FIXME: use list<Buffer> as output buffer.
```

### 4、连接事件处理
TcpConnection 中有：
```c
void TcpConnection::connectEstablished()
{
  loop_->assertInLoopThread();
  assert(state_ == kConnecting);
  setState(kConnected);
  channel_->tie(shared_from_this());
  channel_->enableReading();

  connectionCallback_(shared_from_this());
}
```
该函数不是TcpConnection调用的，是在TcpServer设置调用的，现在只需要了解即可。该函数在建立连接时会被调用，该函数设置连接状态为kConnected（如果之前没有连接），然后会调用Channel的tie和enableReading函数，让该监听套接字及其Channel和EventLoop的Poller绑定。最后调用connectionCallback_。

### 5、“读”事件处理
在构造函数中做过如下设置：
```c
channel_->setReadCallback(boost::bind(&TcpConnection::handleRead, this, _1));
```
TcpConnection::handleRead定义如下：
```c
void TcpConnection::handleRead(Timestamp receiveTime)
{
  loop_->assertInLoopThread();
  int savedErrno = 0;
  ssize_t n = inputBuffer_.readFd(channel_->fd(), &savedErrno);
  if (n > 0)
  {
    messageCallback_(shared_from_this(), &inputBuffer_, receiveTime);
  }
  else if (n == 0)
  {
    handleClose();
  }
  else
  {
    errno = savedErrno;
    LOG_SYSERR << "TcpConnection::handleRead";
    handleError();
  }
}
```
具体读的过程会调用Buffer::readFd函数，该函数定义如下：
```c
ssize_t Buffer::readFd(int fd, int* savedErrno)
{
  // saved an ioctl()/FIONREAD call to tell how much to read
  char extrabuf[65536];
  struct iovec vec[2];
  const size_t writable = writableBytes();
  vec[0].iov_base = begin()+writerIndex_;
  vec[0].iov_len = writable;
  vec[1].iov_base = extrabuf;
  vec[1].iov_len = sizeof extrabuf;
  const ssize_t n = sockets::readv(fd, vec, 2);
  if (n < 0)
  {
    *savedErrno = errno;
  }
  else if (implicit_cast<size_t>(n) <= writable)
  {
    writerIndex_ += n;
  }
  else
  {
    writerIndex_ = buffer_.size();
    append(extrabuf, n - writable);
  }
  // if (n == writable + sizeof extrabuf)
  // {
  //   goto line_30;
  // }
  return n;
}
```
该函数利用了readv函数、iovec结构体和Buffer。vec[0]指向的是Buffer的写指针地址，vec[1]指向的是extrabuf的地址。这样做是为了双保险，如果读出的数据比较小，Buffer可以容纳，那么就全部读到Buffer中了，如果读出的数据很多，Buffer不够用了，那么还有65535字节的缓冲区可以接收这些数据，读出来的一些数据就保持在了Buffer中，剩下数据在extrabuf里，所以需要将在extrabuf中的数据再次append到Buffer中，Buffer也会适当扩容，最后所有数据都在Buffer中了。

现在回到TcpConnection::handleRead中，如果读取到数据，则调用messageCallback_；如果读取到数据为0，说明客户端已经断开连接，那么调用handleClose()函数来关闭连接：
```c
void TcpConnection::handleClose()
{
  loop_->assertInLoopThread();
  LOG_TRACE << "fd = " << channel_->fd() << " state = " << state_;
  assert(state_ == kConnected || state_ == kDisconnecting);
  // we don't close fd, leave it to dtor, so we can find leaks easily.
  setState(kDisconnected);
  channel_->disableAll();

  TcpConnectionPtr guardThis(shared_from_this());
  connectionCallback_(guardThis);
  // must be the last line
  closeCallback_(guardThis);
}
```
该函数设置state_为kDisconnected，将连接套接字及其Channel从EventLoop的Poller中解除监听，然后依次调用connectionCallback_和closeCallback_函数。断开连接也属于connectionCallback_。

如果读取出现错误，那么将调用handleError函数：
```c
void TcpConnection::handleError()
{
  int err = sockets::getSocketError(channel_->fd());
  LOG_ERROR << "TcpConnection::handleError [" << name_
            << "] - SO_ERROR = " << err << " " << strerror_tl(err);
}
```
将错误输出到日志中。

到此，“读事件处理”分析结束。

### 6、发送数据
在TcpConnection中提供了如下函数发送数据：
```c
// void send(string&& message); // C++11
void send(const void* message, size_t len);
void send(const StringPiece& message);
// void send(Buffer&& message); // C++11
void send(Buffer* message);  // this one will swap data
```
使用重载技术编写了三个send函数，这里只介绍参数为Buffer的那个，其余的大体类似：
```c
void TcpConnection::send(Buffer* buf)
{
  if (state_ == kConnected)
  {
    if (loop_->isInLoopThread())
    {
      sendInLoop(buf->peek(), buf->readableBytes());
      buf->retrieveAll();
    }
    else
    {
      loop_->runInLoop(
          boost::bind(&TcpConnection::sendInLoop,
                      this,     // FIXME
                      buf->retrieveAllAsString()));
                    //std::forward<string>(message)));
    }
  }
}
```
无论是在当前EventLoop线程还是跨线程调用，它们都最终调用了TcpConnection::sendInLoop函数：
```c
void TcpConnection::sendInLoop(const StringPiece& message)
{
  sendInLoop(message.data(), message.size());
}
```
进一步调用：
```c
void TcpConnection::sendInLoop(const void* data, size_t len)
{
  loop_->assertInLoopThread();
  ssize_t nwrote = 0;
  size_t remaining = len;
  bool error = false;
  if (state_ == kDisconnected)
  {
    LOG_WARN << "disconnected, give up writing";
    return;
  }
  // if no thing in output queue, try writing directly
  if (!channel_->isWriting() && outputBuffer_.readableBytes() == 0)
  {
    nwrote = sockets::write(channel_->fd(), data, len);
    if (nwrote >= 0)
    {
      remaining = len - nwrote;
      if (remaining == 0 && writeCompleteCallback_)
      {
        loop_->queueInLoop(boost::bind(writeCompleteCallback_, shared_from_this()));
      }
    }
    else // nwrote < 0
    {
      nwrote = 0;
      if (errno != EWOULDBLOCK)
      {
        LOG_SYSERR << "TcpConnection::sendInLoop";
        if (errno == EPIPE) // FIXME: any others?
        {
          error = true;
        }
      }
    }
  }

  assert(remaining <= len);
  if (!error && remaining > 0)
  {
    LOG_TRACE << "I am going to write more data";
    size_t oldLen = outputBuffer_.readableBytes();
    if (oldLen + remaining >= highWaterMark_
        && oldLen < highWaterMark_
        && highWaterMarkCallback_)
    {
      loop_->queueInLoop(boost::bind(highWaterMarkCallback_, shared_from_this(), oldLen + remaining));
    }
    outputBuffer_.append(static_cast<const char*>(data)+nwrote, remaining);
    if (!channel_->isWriting())
    {
      channel_->enableWriting();
    }
  }
}
```
在该函数中会调用：
```c
if (!channel_->isWriting() && outputBuffer_.readableBytes() == 0)
```
判断该Channel有没有使能“写”。在第一次调用该函数时是没有使能“写”的，前面只是使能了“读”，关注读事件。而且会判断Buffer的输出缓冲区内是否有数据，如果是第一次调用了该函数，那么发送缓冲区内肯定是没有有数据的，只不过第一次调用时还没有使能“写”，所以该判断成立，直接调用write函数写入数据，如果数据写入比较少，一次性就发送完了，那么直接调用
writeCompleteCallback_，结束该分支。如果一次性没有写完，那么代码会跳转到该分支执行：
```c
if (!error && remaining > 0)
{....}
````
它会继续判断，当前的Buffer缓冲区里的数据长度（oldLen）与需要发送数据长度（remaining）之和是否大于“高水位标志”highWaterMark_，而且Buffer缓冲区内的数据小于highWaterMark_，是否设置了“高水位标志”回调函数highWaterMarkCallback_，如果设置了，那么执行highWaterMarkCallback_函数。然后将发送数据中没有发送的部分拷贝到Buffer中，然后使能“写”。接下来的数据就交给Poller返回后调用TcpConnection::handleWrite函数处理了。

### 7、“写”事件处理
在构造函数中做过如下设置：
```c
channel_->setWriteCallback(boost::bind(&TcpConnection::handleWrite, this));
```
当需要向连接套接字中写入数据时，TcpConnection::handleWrite将被调用：
```c
void TcpConnection::handleWrite()
{
  loop_->assertInLoopThread();
  if (channel_->isWriting())
  {
    ssize_t n = sockets::write(channel_->fd(),
                               outputBuffer_.peek(),
                               outputBuffer_.readableBytes());
    if (n > 0)
    {
      outputBuffer_.retrieve(n);
      if (outputBuffer_.readableBytes() == 0)
      {
        channel_->disableWriting();
        if (writeCompleteCallback_)
        {
          loop_->queueInLoop(boost::bind(writeCompleteCallback_, shared_from_this()));
        }
        if (state_ == kDisconnecting)
        {
          shutdownInLoop();
        }
      }
      else
      {
        LOG_TRACE << "I am going to write more data";
      }
    }
    else
    {
      LOG_SYSERR << "TcpConnection::handleWrite";
      // if (state_ == kDisconnecting)
      // {
      //   shutdownInLoop();
      // }
    }
  }
  else
  {
    LOG_TRACE << "Connection fd = " << channel_->fd()
              << " is down, no more writing";
  }
}
```
当客户端套接字变得可写时，调用write往套接字中写入Buffer内容，如果写完了，那么设置Channel，客户端套接字停止“写行为”。如果设置了writeCompleteCallback_，则调用writeCompleteCallback_函数。

如果当前TcpConnection状态为kDisconnecting，说明没有连接，则调用shutdownInLoop函数，关闭该套接字的写端，但仍然可以读：
```c
void TcpConnection::shutdownInLoop()
{
  loop_->assertInLoopThread();
  if (!channel_->isWriting())
  {
    // we are not writing
    socket_->shutdownWrite();
  }
}
```
当调用如下函数后，状态为kDisconnecting：
```c
void TcpConnection::shutdown()
{
  // FIXME: use compare and swap
  if (state_ == kConnected)
  {
    setState(kDisconnecting);
    // FIXME: shared_from_this()?
    loop_->runInLoop(boost::bind(&TcpConnection::shutdownInLoop, this));
  }
}
```
使用该函数来断开连接。
