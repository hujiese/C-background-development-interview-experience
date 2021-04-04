## 聊天服务

### 具体可见reference目录下的《MuduoManual》的第66页。

这里主要补充下服务端的几个实现细节。

在服务端中需要保持客户端的TcpConnection，用于实现“群发”，如果是单线程的服务器还好办，不论是有新客户端连接还是实现群发都不需要加锁：
```c
void onConnection(const TcpConnectionPtr& conn)
{
  LOG_INFO << conn->localAddress().toIpPort() << " -> "
           << conn->peerAddress().toIpPort() << " is "
           << (conn->connected() ? "UP" : "DOWN");  if (conn->connected())
  {
    connections_.insert(conn);
  }
  else
  {
    connections_.erase(conn);
  }
}

void onStringMessage(const TcpConnectionPtr&,
                     const string& message,
                     Timestamp)
{
  for (ConnectionList::iterator it = connections_.begin();
      it != connections_.end();
      ++it)
  {
    codec_.send(get_pointer(*it), message);
  }
}
```
然而服务器大多是多线程的，为了提高性能往往会使用多个IO线程，server_threaded.cc中便是聊天服务器的多线程版本。所以服务端所维护的客户端TcpConnection就是共享资源，当有客户端连接时，在onConnection函数中会修改该资源，将新的TcpConnection加入到客户端连接集合中，所以需要加锁保护：
```c
void onConnection(const TcpConnectionPtr& conn)
{
  LOG_INFO << conn->localAddress().toIpPort() << " -> "
      << conn->peerAddress().toIpPort() << " is "
      << (conn->connected() ? "UP" : "DOWN");

  MutexLockGuard lock(mutex_);
  if (conn->connected())
  {
    connections_.insert(conn);
  }
  else
  {
    connections_.erase(conn);
  }
}
```
在调用onStringMessage函数时需要拿到所有客户端的连接TcpConnection集合，这里便又存在临界区（防止产生新的客户端连接改写这一集合元素，或者断开连接删除某些元素），需要加锁保护：
```c
void onStringMessage(const TcpConnectionPtr&,
                     const string& message,
                     Timestamp)
{
  MutexLockGuard lock(mutex_);
  for (ConnectionList::iterator it = connections_.begin();
      it != connections_.end();
      ++it)
  {
    codec_.send(get_pointer(*it), message);
  }
}
```
总所周知，加锁会导致服务器的性能降低，而且临界区的长度也不宜过大，上面的例子中只是为了获取所有客户端的连接TcpConnection集合，只有在该步骤才是真正的临界区，但是却将内容转发这块也锁住了，临界区不合理，server_threaded_efficient.cc便做出了改进：
```c
void onStringMessage(const TcpConnectionPtr&,
                     const string& message,
                     Timestamp)
{
  ConnectionListPtr connections = getConnectionList();
  for (ConnectionList::iterator it = connections->begin();
      it != connections->end();
      ++it)
  {
    codec_.send(get_pointer(*it), message);
  }
}

ConnectionListPtr getConnectionList()
{
  MutexLockGuard lock(mutex_);
  return connections_;
}
```
现在的这种做法是所有线程都共用一个客户端连接TcpConnection集合，所以有了共享资源，server_threaded_highperformance.cc中让每个线程都拥有一个自己的客户端连接TcpConnection集合：
```c
void threadInit(EventLoop* loop)
{
  assert(LocalConnections::pointer() == NULL);
  LocalConnections::instance();
  assert(LocalConnections::pointer() != NULL);
  MutexLockGuard lock(mutex_);
  loops_.insert(loop);
}

typedef ThreadLocalSingleton<ConnectionList> LocalConnections;
```
这样的话onConnection函数中就不需要加锁保护了，因为每个IO线程都有自己的局部单例客户端TcpConnection集合：
```c
void onConnection(const TcpConnectionPtr& conn)
{
  LOG_INFO << conn->localAddress().toIpPort() << " -> "
           << conn->peerAddress().toIpPort() << " is "
           << (conn->connected() ? "UP" : "DOWN");
  if (conn->connected())
  {
    LocalConnections::instance().insert(conn);
  }
  else
  {
    LocalConnections::instance().erase(conn);
  }
}
```
至于消息转发，则使用如下方法来实现，需要将消息转发到每个IO线程，然后让每个IO线程自己转发给管理的客户端连接：
```c
void onStringMessage(const TcpConnectionPtr&,
                     const string& message,
                     Timestamp)
{
  EventLoop::Functor f = boost::bind(&ChatServer::distributeMessage, this, message);
  LOG_DEBUG;
  MutexLockGuard lock(mutex_);
  for (std::set<EventLoop*>::iterator it = loops_.begin();
      it != loops_.end();
      ++it)
  {
    (*it)->queueInLoop(f);
  }
  LOG_DEBUG;
}

void distributeMessage(const string& message)
{
  LOG_DEBUG << "begin";
  for (ConnectionList::iterator it = LocalConnections::instance().begin();
      it != LocalConnections::instance().end();
      ++it)
  {
    codec_.send(get_pointer(*it), message);
  }
  LOG_DEBUG << "end";
}
```