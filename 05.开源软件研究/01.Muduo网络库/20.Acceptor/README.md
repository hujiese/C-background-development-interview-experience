## Acceptor 分析

更多可见[muduo源码分析之Acceptor](https://github.com/hujiese/Large-concurrent-serve/blob/master/22_muduo_acceptor/muduo_acceptor.md)

套接字TCP服务器编程中，服务器需要一个监听套接字，该套接字用于监听客户端连接。

“监听套接字”也是一个文件描述符，一个文件描述符在muduo中应该和一个Channel联系。muduo网络库将专用于监听连接的套接字操作封装成了Acceptor。所以一个Acceptor中一定会有一个套接字socket和一个Channel，有了Channel一定会关联一个EventLoop：
```c
EventLoop* loop_;
Socket acceptSocket_;
Channel acceptChannel_;
```
其构造函数如下：
```c
Acceptor::Acceptor(EventLoop* loop, const InetAddress& listenAddr)
  : loop_(loop),
    acceptSocket_(sockets::createNonblockingOrDie()),
    acceptChannel_(loop, acceptSocket_.fd()),
    listenning_(false),
    idleFd_(::open("/dev/null", O_RDONLY | O_CLOEXEC))
{
  assert(idleFd_ >= 0);
  acceptSocket_.setReuseAddr(true);
  acceptSocket_.bindAddress(listenAddr);
  acceptChannel_.setReadCallback(
      boost::bind(&Acceptor::handleRead, this));
}
```
其中idleFd_的作用会在后面介绍。在构造函数中初始化了acceptSocket\_，并将该Socket管理的监听套接字和acceptChannel\_关联。然后在构造函数中设置了监听套接字地址复用，并绑定了传入参数的监听地址结构，最后设置了acceptChannel\_的读事件函数Acceptor::handleRead，当监听套接字上有连接事件时将会出发该回调函数，该回调函数将在后面详细介绍。

在设置完acceptChannel_的回调函数后，在构造函数初始化结束后，acceptChannel\_还是没有和EventLoop中的Poller关联，因为一个Channel如果要和Poller关联，那么必须要调用enablexxx函数，enablexxx函数内部又会调用update函数，这样才能达成关联（见[《15.muduo事件监听总结》](../15.muduo事件监听总结/README.md)）。所以如果要达成这个目的，还需要有该函数：
```c
void Acceptor::listen()
{
  loop_->assertInLoopThread();
  listenning_ = true;
  acceptSocket_.listen();
  acceptChannel_.enableReading();
}
```
创建套接字后还需要设置套接字监听，并和Poller关联，这样一来，如果有客户连接，那么将调用Acceptor::handleRead函数：
```c
void Acceptor::handleRead()
{
  loop_->assertInLoopThread();
  InetAddress peerAddr(0);
  //FIXME loop until no more
  int connfd = acceptSocket_.accept(&peerAddr);
  if (connfd >= 0)
  {
    // string hostport = peerAddr.toIpPort();
    // LOG_TRACE << "Accepts of " << hostport;
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
    // Read the section named "The special problem of
    // accept()ing when you can't" in libev's doc.
    // By Marc Lehmann, author of livev.
    if (errno == EMFILE)
    {
      ::close(idleFd_);
      idleFd_ = ::accept(acceptSocket_.fd(), NULL, NULL);
      ::close(idleFd_);
      idleFd_ = ::open("/dev/null", O_RDONLY | O_CLOEXEC);
    }
  }
  ```
在该函数中调用了accetp函数来接收连接，先看连接成功的情况（connfd >= 0），在该代码分支会将该连接客户端套接字及其地址传入newConnectionCallback_处理，newConnectionCallback_可以通过如下函数来设置：
```c
void setNewConnectionCallback(const NewConnectionCallback& cb){ newConnectionCallback_ = cb; }
```
可以理解，在setNewConnectionCallback中必然会处理客户端连接套接字，至于muduo如何处理，在TcpConnection中会介绍。如果该函数没有设置，也就是默认不处理该连接套接字，那么直接关闭该套接字。

接下来再看看else分支情况。else分支中会判断errno 是否为 EMFILE，如果发生该情况则说明系统文件描述符用尽了，需要处理方法，处理方法在前面的[《01.IO复用模型回顾/poll/README.md》](../01.IO复用模型回顾/poll/README.md)中有详细介绍，这里的方法和前文方法完全一样，这里就不在累述。

最后是析构函数，在该函数中将释放监听套接字资源，去除与其相关的Channel：
```c
Acceptor::~Acceptor()
{
  acceptChannel_.disableAll();
  acceptChannel_.remove();
  ::close(idleFd_);
}
```
到此，Acceptor就分析完了，这是为后面的TcpServer做准备。