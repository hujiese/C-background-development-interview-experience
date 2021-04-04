## EPollPoller 分析

### 1、PollPoller和EventLoop关系

一个EventLoop关联一个EPollPoller，前面章节中说到Channel的update会调用EventLoop的update函数，而EventLoop又调用Poller相关的函数。EventLoop之所以能够“事件循环”也是其内部调用Poller的poll函数，所以有：
```c
EPollPoller(EventLoop* loop);

EPollPoller::EPollPoller(EventLoop* loop)
  : Poller(loop),
    epollfd_(::epoll_create1(EPOLL_CLOEXEC)),
    events_(kInitEventListSize)
{
  if (epollfd_ < 0)
  {
    LOG_SYSFATAL << "EPollPoller::EPollPoller";
  }
}

 private:
  EventLoop* ownerLoop_;

void assertInLoopThread()
{
  ownerLoop_->assertInLoopThread();
}
```
这一系列函数和变量的存在。

### 2、EPollPoller和Channel的关系

Channel管理了一个文件描述符，在muduo中，一个Channel可以看作是一个文件描述符的“代表”，如果要操作一个文件描述符，则必须是通过该文件描述符对应的Channel。EPollPoller需要监听和返回这些文件描述符上注册和实际发生的事件，所以必须提供操作Channel的函数和数据结构。下面这两个数据结构用于保存文件描述符及其Channel。
```c
typedef std::vector<struct epoll_event> EventList;
typedef std::map<int, Channel*> ChannelMap;
EventList events_;
ChannelMap channels_;
```
EventList events_的作用非常明显，因为epoll函数需要一个struct epoll_event的数组地址，所以该结构是用于epoll_wait函数参数。ChannelMap channels_则是用于管理注册的Channel的，key是Channel对应的文件描述符fd，value就是该Channel的地址，使用map数据结构可以很方便的对Channel进行查找和操作。

### 3、update 和 remove Channel

#### （1）updateChannel 函数
updateChannel函数定义如下：
```c
void EPollPoller::updateChannel(Channel* channel)
{
  Poller::assertInLoopThread();
  LOG_TRACE << "fd = " << channel->fd() << " events = " << channel->events();
  const int index = channel->index();
  if (index == kNew || index == kDeleted)
  {
    // a new one, add with EPOLL_CTL_ADD
    int fd = channel->fd();
    if (index == kNew)
    {
      assert(channels_.find(fd) == channels_.end());
      channels_[fd] = channel;
    }
    else // index == kDeleted
    {
      assert(channels_.find(fd) != channels_.end());
      assert(channels_[fd] == channel);
    }
    channel->set_index(kAdded);
    update(EPOLL_CTL_ADD, channel);
  }
  else
  {
    // update existing one with EPOLL_CTL_MOD/DEL
    int fd = channel->fd();
    (void)fd;
    assert(channels_.find(fd) != channels_.end());
    assert(channels_[fd] == channel);
    assert(index == kAdded);
    if (channel->isNoneEvent())
    {
      update(EPOLL_CTL_DEL, channel);
      channel->set_index(kDeleted);
    }
    else
    {
      update(EPOLL_CTL_MOD, channel);
    }
  }
}
```
这个函数的作用和PollPoller函数作用是一样的。当一个Channel的index_为-1时则说明这个Channel并没有和EPollPoller关联，如果index_为2，则说明该通道被取消过关注，如果为1则说明该Channel已经和EPollPoller关联，需要更新相关文件描述符的一些监听事件：
```c
namespace
{
const int kNew = -1;
const int kAdded = 1;
const int kDeleted = 2;
}
```
如果index_是kNew或者kDeleted，则说明需要将该通道和该EPollPoller关联，设置index_为kAdded，然后调用update函数将该通道和EPollPoller关联：
```c
void EPollPoller::update(int operation, Channel* channel)
{
  struct epoll_event event;
  bzero(&event, sizeof event);
  event.events = channel->events();
  event.data.ptr = channel;
  int fd = channel->fd();
  if (::epoll_ctl(epollfd_, operation, fd, &event) < 0)
  {
    if (operation == EPOLL_CTL_DEL)
    {
      LOG_SYSERR << "epoll_ctl op=" << operation << " fd=" << fd;
    }
    else
    {
      LOG_SYSFATAL << "epoll_ctl op=" << operation << " fd=" << fd;
    }
  }
}
```
update函数内部也是调用了epoll_ctl函数（其用法在网络编程部分有介绍）。需要注意的是，epoll_event里并没有设置文件描述符，而是用了event.data.ptr指针保存了Channel，毕竟Channel中包含的文件描述符信息更加丰富。

如果index_已经是added状态，那么判断该Channel中文件描述符是否被设置为“不关注”如果是的话，直接调用updata函数将该文件描述符移除epoll事件监听，否则更新该文件描述符结构的events监听事件。

#### （2）removeChannel 函数
```c
void EPollPoller::removeChannel(Channel* channel)
{
  Poller::assertInLoopThread();
  int fd = channel->fd();
  LOG_TRACE << "fd = " << fd;
  assert(channels_.find(fd) != channels_.end());
  assert(channels_[fd] == channel);
  assert(channel->isNoneEvent());
  int index = channel->index();
  assert(index == kAdded || index == kDeleted);
  size_t n = channels_.erase(fd);
  (void)n;
  assert(n == 1);

  if (index == kAdded)
  {
    update(EPOLL_CTL_DEL, channel);
  }
  channel->set_index(kNew);
}
```
该函数将取消对Channel对应的文件描述的事件监听，然后将该Channel从channels_中删除。

### 4、poll 
poll函数是在EventLoop中调用的，EventLoop希望通过该函数获取到当前的活动Channel(文件描述符上有事件发生)集合，所以会传入一个ChannelList* activeChannels作为poll的参数，该结构定义如下：
```c
typedef std::vector<Channel*> ChannelList;
```
也是一个vector集合，保存Channel的地址。poll函数定义如下：
```c
Timestamp EPollPoller::poll(int timeoutMs, ChannelList* activeChannels)
{
  int numEvents = ::epoll_wait(epollfd_,
                               &*events_.begin(),
                               static_cast<int>(events_.size()),
                               timeoutMs);
  Timestamp now(Timestamp::now());
  if (numEvents > 0)
  {
    LOG_TRACE << numEvents << " events happended";
    fillActiveChannels(numEvents, activeChannels);
    if (implicit_cast<size_t>(numEvents) == events_.size())
    {
      events_.resize(events_.size()*2);
    }
  }
  else if (numEvents == 0)
  {
    LOG_TRACE << " nothing happended";
  }
  else
  {
    LOG_SYSERR << "EPollPoller::poll()";
  }
  return now;
}
```
该函数的第一个参数是超时时间，第二个参数是EventLoop中需要的活动通道集合。该函数的内部也是调用了epoll_wait函数，该函数需要一个额外的文件描述符作为epollfd_：
```c
int epollfd_;
```
该文件描述符在构造函数初始化参数中中已经初始化了：
```c
EPollPoller::EPollPoller(EventLoop* loop)
  : Poller(loop),
    epollfd_(::epoll_create1(EPOLL_CLOEXEC)),
    events_(kInitEventListSize)
{
  if (epollfd_ < 0)
  {
    LOG_SYSFATAL << "EPollPoller::EPollPoller";
  }
}
```
当epoll_wait返回时，该函数会获取当前时间戳，作为函数返回值使用。如果poll返回为0，则说明poll超时但没有发生任何事件；如果poll为负值，则说明poll系统调用失败；如果poll正常返回一个整数，则说明当前有文件描述符活动，需要获取这些文件描述符对应的Channel，并返回给EventLoop，这里使用了fillActiveChannels来获取这些活跃的通道，当活跃的文件描述达到events_数组大小时，该数组将会扩容一倍，以满足更多需求，也减少了vector动态扩张的次数。fillActiveChannels函数定义如下：
```c
void EPollPoller::fillActiveChannels(int numEvents,
                                     ChannelList* activeChannels) const
{
  assert(implicit_cast<size_t>(numEvents) <= events_.size());
  for (int i = 0; i < numEvents; ++i)
  {
    Channel* channel = static_cast<Channel*>(events_[i].data.ptr);
#ifndef NDEBUG
    int fd = channel->fd();
    ChannelMap::const_iterator it = channels_.find(fd);
    assert(it != channels_.end());
    assert(it->second == channel);
#endif
    channel->set_revents(events_[i].events);
    activeChannels->push_back(channel);
  }
}
```
需要注意的是EPollPoller的文件描述符epollfd_，由于EPollPoller是采用RAII技法编写的，在构造函数中创建了文件描述符epollfd_，那么在析构函数中也应该关闭epollfd_并释放资源：
```c
EPollPoller::~EPollPoller()
{
  ::close(epollfd_);
}
```

到此，EPollPoller就分析结束了。
