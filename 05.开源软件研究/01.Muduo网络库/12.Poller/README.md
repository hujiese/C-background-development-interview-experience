## Poller

Poller指的是muduo封装的PollPoller、EPollPoller及其父类Poller的总称。在muduo中定义了一个Poller类，该类中定义了一些PollPoller和EPollPoller必须要实现的函数：
```c
virtual Timestamp poll(int timeoutMs, ChannelList* activeChannels) = 0;
virtual void updateChannel(Channel* channel) = 0;
virtual void removeChannel(Channel* channel) = 0;
```

前面说到，一个Channel管理一个文件描述符fd的所有信息与操作，但如果要将文件描述符和poll/epoll关联和注册事件监听，Poller也需要关联Channel并提供相关操作的函数。但是由于poll/epoll的poll函数和操作的方法及其数据结构不同，所以这些具体的实现还是要放在PollPoller和EPollPoller中。

muduo提供了两种事件循环的类PollPoller和EPollPoller，同时也提供了该函数来选择使用哪一个：

```c
static Poller* newDefaultPoller(EventLoop* loop);
// DefaultPoller.cc
Poller* Poller::newDefaultPoller(EventLoop* loop)
{
  if (::getenv("MUDUO_USE_POLL"))
  {
    return new PollPoller(loop);
  }
  else
  {
    return new EPollPoller(loop);
  }
}
```
关于Poller的具体内容详见PollPoller和EPollPoller的讨论。