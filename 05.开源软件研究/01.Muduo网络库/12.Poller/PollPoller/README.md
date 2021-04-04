## PollPoller分析

### 1、PollPoller和EventLoop关系

一个EventLoop关联一个PollPoller，前面章节中说到Channel的update会调用EventLoop的update函数，而EventLoop又调用Poller相关的函数。EventLoop之所以能够“事件循环”也是其内部调用Poller的poll函数，所以有：
```c
PollPoller(EventLoop* loop);

PollPoller::PollPoller(EventLoop* loop)
  : Poller(loop)
{
}

 private:
  EventLoop* ownerLoop_;

void assertInLoopThread()
{
  ownerLoop_->assertInLoopThread();
}
```
这一系列函数和变量的存在。

### 2、PollPoller和Channel的关系

Channel管理了一个文件描述符，在muduo中，一个Channel可以看作是一个文件描述符的“代表”，如果要操作一个文件描述符，则必须是通过该文件描述符对应的Channel。PollPoller需要监听和返回这些文件描述符上注册和实际发生的事件，所以必须提供操作Channel的函数和数据结构。下面这两个数据结构用于保存文件描述符及其Channel。
```c
typedef std::vector<struct pollfd> PollFdList;
typedef std::map<int, Channel*> ChannelMap;
PollFdList pollfds_;
ChannelMap channels_;
```
PollFdList pollfds_的作用非常明显，因为poll函数需要一个struct pollfd的数组地址，所以该结构是用于poll函数参数。ChannelMap channels_则是用于管理注册的Channel的，key是Channel对应的文件描述符fd，value就是该Channel的地址，使用map数据结构可以很方便地对Channel进行查找和删除。

### 3、update 和 remove Channel

#### （1）updateChannel 函数

updateChannel函数定义如下，由于需要更新一个Channel，所以该函数的参数只有Channel的一份指针：

```c
virtual void updateChannel(Channel* channel);

void PollPoller::updateChannel(Channel* channel)
{
  Poller::assertInLoopThread();
  LOG_TRACE << "fd = " << channel->fd() << " events = " << channel->events();
  if (channel->index() < 0)
  {
    // a new one, add to pollfds_
    assert(channels_.find(channel->fd()) == channels_.end());
    struct pollfd pfd;
    pfd.fd = channel->fd();
    pfd.events = static_cast<short>(channel->events());
    pfd.revents = 0;
    pollfds_.push_back(pfd);
    int idx = static_cast<int>(pollfds_.size())-1;
    channel->set_index(idx);
    channels_[pfd.fd] = channel;
  }
  else
  {
    // update existing one
    assert(channels_.find(channel->fd()) != channels_.end());
    assert(channels_[channel->fd()] == channel);
    int idx = channel->index();
    assert(0 <= idx && idx < static_cast<int>(pollfds_.size()));
    struct pollfd& pfd = pollfds_[idx];
    assert(pfd.fd == channel->fd() || pfd.fd == -channel->fd()-1);
    pfd.events = static_cast<short>(channel->events());
    pfd.revents = 0;
    if (channel->isNoneEvent())
    {
      // ignore this pollfd
      pfd.fd = -channel->fd()-1;
    }
  }
}
```
首先看if (channel->index() < 0)情况下做的工作。当一个Channel创建后默认设置自身的index_为-1：
```c
Channel::Channel(EventLoop* loop, int fd__)
  : loop_(loop),
    ...
    index_(-1),
    ...
{
}
```
所以对于新创建的Channel如果被更新，那么一定是走该分支的。既然该Channel是刚创建并且是第一次和PollPoller关联，那么PollPoller中一定不会存在该Channel的信息，所以使用了该断言：
```c
assert(channels_.find(channel->fd()) == channels_.end());
```
再接下来就是构造一个pollfd结构体并将该结构体该Channel保存了，供下次poll函数调用：
```C
struct pollfd pfd;
pfd.fd = channel->fd();
pfd.events = static_cast<short>(channel->events());
pfd.revents = 0;
pollfds_.push_back(pfd);
int idx = static_cast<int>(pollfds_.size())-1;
channel->set_index(idx);
channels_[pfd.fd] = channel;
```
需要注意的是，上面Channel的index_被设置为当前pollfds_的实际长度减一，这也是为了方便快速获取到pollfds_向量中的对应的文件描述符，有了该文件描述又可以很快从channels_中获取到该Channel，这个过程的代价很小，几乎不需要遍历。

接下来分析channel->index() > 0 的情况，发生这种情况也意味着该Channel之前已经注册到该PollPoller中了，但是由于一些原因需要修改该文件描述符的关注事件，对于这种情况的Channel将调用else分支代码：
```c
else
{
  // update existing one
  assert(channels_.find(channel->fd()) != channels_.end());
  assert(channels_[channel->fd()] == channel);
  int idx = channel->index();
  assert(0 <= idx && idx < static_cast<int>(pollfds_.size()));
  struct pollfd& pfd = pollfds_[idx];
  assert(pfd.fd == channel->fd() || pfd.fd == -channel->fd()-1);
  pfd.events = static_cast<short>(channel->events());
  pfd.revents = 0;
  if (channel->isNoneEvent())
  {
    // ignore this pollfd
    pfd.fd = -channel->fd()-1;
  }
}
```
代码中两个assert断言该Channel是否已经和PollPoller关联，如果关联则取出该pollfd数组中的该Channel对应的文件描述符及其结构体，更新该结构体中文件描述符监听事件。如果不再关注该Channel中文件描述符事件，则直接将该文件描述符赋值为其相反数减一。

#### （2）removeChannel 函数
使用该函数之前一般需要调用 updateChannel 函数设置不再关注该Channel对应的文件描述符上的事件。

该函数定义如下：
```c
void PollPoller::removeChannel(Channel* channel)
{
  Poller::assertInLoopThread();
  LOG_TRACE << "fd = " << channel->fd();
  assert(channels_.find(channel->fd()) != channels_.end());
  assert(channels_[channel->fd()] == channel);
  assert(channel->isNoneEvent());
  int idx = channel->index();
  assert(0 <= idx && idx < static_cast<int>(pollfds_.size()));
  const struct pollfd& pfd = pollfds_[idx]; (void)pfd;
  assert(pfd.fd == -channel->fd()-1 && pfd.events == channel->events());
  size_t n = channels_.erase(channel->fd());
  assert(n == 1); (void)n;
  if (implicit_cast<size_t>(idx) == pollfds_.size()-1)
  {
    pollfds_.pop_back();
  }
  else
  {
    int channelAtEnd = pollfds_.back().fd;
    iter_swap(pollfds_.begin()+idx, pollfds_.end()-1);
    if (channelAtEnd < 0)
    {
      channelAtEnd = -channelAtEnd-1;
    }
    channels_[channelAtEnd]->set_index(idx);
    pollfds_.pop_back();
  }
}
```
首先三个断言确认该Channel已经和PollPoller关联而且确认该Channel上的文件描述符事件不再关注，然后找到该Channel文件描述符在pollfds_数组中的位置，将该Channel从Channels_中去除，将该文件描述符对应的pollfd从pollfds_数组中去除。从pollfs_数组中去除一个pollfd，Muduo使用了一个技巧，如果要去除的pollfd结构是数组中的最后一个元素，则调用pop_back函数直接弹出，否则将该元素和数组中最后一个元素交换位置，然后弹出最后一个元素，这样保证了pollfds_数组元素是连续的，不存在中间缺失的情况。

### 3、poll

poll函数是在EventLoop中调用的，EventLoop希望通过该函数获取到当前的活动Channel(文件描述符上有事件发生)集合，所以会传入一个ChannelList* activeChannels作为poll的参数，该结构定义如下：
```c
typedef std::vector<Channel*> ChannelList;
```
也是一个vector集合，保存Channel的地址。poll函数定义如下：
```c
Timestamp PollPoller::poll(int timeoutMs, ChannelList* activeChannels)
{
  // XXX pollfds_ shouldn't change
  int numEvents = ::poll(&*pollfds_.begin(), pollfds_.size(), timeoutMs);
  Timestamp now(Timestamp::now());
  if (numEvents > 0)
  {
    LOG_TRACE << numEvents << " events happended";
    fillActiveChannels(numEvents, activeChannels);
  }
  else if (numEvents == 0)
  {
    LOG_TRACE << " nothing happended";
  }
  else
  {
    LOG_SYSERR << "PollPoller::poll()";
  }
  return now;
}
```
该函数的第一个参数是超时时间，第二个参数是EventLoop中需要的活动通道集合。该函数的内部也是调用了poll函数，当poll返回时，该函数会获取当前时间戳，作为函数返回值使用。如果poll返回为0，则说明poll超时但没有发生任何事件；如果poll为负值，则说明poll系统调用失败；如果poll正常返回一个整数，则说明当前有文件描述符活动，需要获取这些文件描述符对应的Channel，并返回给EventLoop，这里使用了fillActiveChannels来获取这些活跃的通道：
```c
void PollPoller::fillActiveChannels(int numEvents, ChannelList* activeChannels) const
{
  for (PollFdList::const_iterator pfd = pollfds_.begin();
      pfd != pollfds_.end() && numEvents > 0; ++pfd)
  {
    if (pfd->revents > 0)
    {
      --numEvents;
      ChannelMap::const_iterator ch = channels_.find(pfd->fd);
      assert(ch != channels_.end());
      Channel* channel = ch->second;
      assert(channel->fd() == pfd->fd);
      channel->set_revents(pfd->revents);
      // pfd->revents = 0;
      activeChannels->push_back(channel);
    }
  }
}
```

到这里PollPoller就分析完了，需要注意的是Channel的index_，在PollPoller中，如果index_为-1，则说明该Channel是新的需要加入的通道；如果index_>0，则说明该Channel已经和PollPoller关联了，index_的值用于在pollfds_数组中查找文件描述符对应的pollfd如果index_为其他负值，则说明该文件描述符将不被关注，该Channel也将被移除。

index_在EPollPoller中的作用也类似，但又是一种新的处理方法，这在EPollPoller中将具体分析。
