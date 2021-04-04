## TcpClient

在上一章的Connector中说到“在该函数中只是获取到连接成功套接字的文件描述符sockfd，并且将Channel清除，然后调用newConnectionCallback_函数，该函数在TcpClient中传入”，Connector::handleWrite函数是被TcpClient调用的，那么我们先一窥newConnectionCallback_函数究竟。

在此之前，先看看TcpClient中的Connector：
```c
EventLoop* loop_;
ConnectorPtr connector_; // avoid revealing Connector
typedef boost::shared_ptr<Connector> ConnectorPtr;

TcpClient::TcpClient(EventLoop* loop,
                     const InetAddress& serverAddr,
                     const string& name)
  : loop_(CHECK_NOTNULL(loop)),
    connector_(new Connector(loop, serverAddr)),
    name_(name),
    ...
{
  connector_->setNewConnectionCallback(
      boost::bind(&TcpClient::newConnection, this, _1));
  // FIXME setConnectFailedCallback
  LOG_INFO << "TcpClient::TcpClient[" << name_
           << "] - connector " << get_pointer(connector_);
}
```
在构造函数中，Connector被设置了新连接回调函数TcpClient::newConnection，也就是前面的newConnectionCallback_，该函数定义如下：
```c
void TcpClient::newConnection(int sockfd)
{
  loop_->assertInLoopThread();
  InetAddress peerAddr(sockets::getPeerAddr(sockfd));
  char buf[32];
  snprintf(buf, sizeof buf, ":%s#%d", peerAddr.toIpPort().c_str(), nextConnId_);
  ++nextConnId_;
  string connName = name_ + buf;

  InetAddress localAddr(sockets::getLocalAddr(sockfd));
  // FIXME poll with zero timeout to double confirm the new connection
  // FIXME use make_shared if necessary
  TcpConnectionPtr conn(new TcpConnection(loop_,
                                          connName,
                                          sockfd,
                                          localAddr,
                                          peerAddr));

  conn->setConnectionCallback(connectionCallback_);
  conn->setMessageCallback(messageCallback_);
  conn->setWriteCompleteCallback(writeCompleteCallback_);
  conn->setCloseCallback(
      boost::bind(&TcpClient::removeConnection, this, _1)); // FIXME: unsafe
  {
    MutexLockGuard lock(mutex_);
    connection_ = conn;
  }
  conn->connectEstablished();
}
```
该函数将连接成功的socfd封装为一个Connection，并设置“三个半”回调事件函数。好的，到了这里就解释了前面Acceptor为什么不自己持有一个sockfd、为什么要在连接成功时清空Channel等诸多问题。拿到连接成功套接字后，创建TcpConnection与服务器进行数据交换才是真主角。这些TcpConnection相关的代码就不做具体介绍了，可自行参考TcpClient源码。

TcpClient的其他部分代码都比较简单，可自行分析。