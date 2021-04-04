<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->

- [Channel 分析](#channel-%E5%88%86%E6%9E%90)
  - [1、Channel与文件描述符](#1channel%E4%B8%8E%E6%96%87%E4%BB%B6%E6%8F%8F%E8%BF%B0%E7%AC%A6)
  - [2、设置监听回调函数](#2%E8%AE%BE%E7%BD%AE%E7%9B%91%E5%90%AC%E5%9B%9E%E8%B0%83%E5%87%BD%E6%95%B0)
  - [3、Channel与EventLoop关系](#3channel%E4%B8%8Eeventloop%E5%85%B3%E7%B3%BB)
  - [4、响应事件](#4%E5%93%8D%E5%BA%94%E4%BA%8B%E4%BB%B6)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## Channel 分析

Channel，你可以理解为是一个“通道”，该“通道”中绑定了一个文件描述符及其所关注事件、注册的读写事件等信息。

### 1、Channel与文件描述符

一个Channel管理一个文件描述符，在创建Channel时需要指定：
```c
const int  fd_;

Channel::Channel(EventLoop* loop, int fd__)
  : loop_(loop),
    fd_(fd__),
    ...
{
}
```
该文件描述符的关注事件可以用如下代码表示：
```c
private:
    const int Channel::kNoneEvent = 0;
    const int Channel::kReadEvent = POLLIN | POLLPRI;
    const int Channel::kWriteEvent = POLLOUT;
```
在Channel内部也定义了events_和revents_来标记文件描述符所关注的事件以及实际发生的事件，该方法和struct pollfd 结构体（见网络编程IO复用poll部分）类似：
```c
  int        events_;
  int        revents_;
```
muduo提供了下面这些函数来设置文件描述符关注事件：
```c
void enableReading() { events_ |= kReadEvent; update(); }
void enableWriting() { events_ |= kWriteEvent; update(); }
void disableWriting() { events_ &= ~kWriteEvent; update(); }
void disableAll() { events_ = kNoneEvent; update(); }
```
其中update函数定义如下：
```c
void Channel::update()
{
  loop_->updateChannel(this);
}
```
该函数的具体实现会在后续章节解释，现在只需要明白update的作用就是将该Channel及其绑定的文件描述符和EventLoop中的poll/epoll关联即可。

muduo也提供了下面函数来获取和设置文件描述符及其事件的状态：
```c
int fd() const { return fd_; }
int events() const { return events_; }
void set_revents(int revt) { revents_ = revt; } // used by pollers
// int revents() const { return revents_; }
bool isNoneEvent() const { return events_ == kNoneEvent; }
bool isWriting() const { return events_ & kWriteEvent; }
```
其中需要注意的是set_revents函数，该函数是被poll/epoll类中调用的，它的使用会在poll/epoll相关章节给出。

### 2、设置监听回调函数

Channel中可以设置读、写、错误和关闭事件的回调函数：

```c
void setReadCallback(const ReadEventCallback& cb)
{ readCallback_ = cb; }
void setWriteCallback(const EventCallback& cb)
{ writeCallback_ = cb; }
void setCloseCallback(const EventCallback& cb)
{ closeCallback_ = cb; }
void setErrorCallback(const EventCallback& cb)
{ errorCallback_ = cb; }
```
其中：
```c
ReadEventCallback readCallback_;
EventCallback writeCallback_;
EventCallback closeCallback_;
EventCallback errorCallback_;

typedef boost::function<void()> EventCallback;
typedef boost::function<void(Timestamp)> ReadEventCallback;
```
这里也使用了函数模板。

### 3、Channel与EventLoop关系

一个Channel一定会关联一个EventLoop，和文件描述符一样，在构造函数中需要传入。一旦关联该EventLoop，EventLoop就可对该Channel操作，相关内容定义如下：

```c
EventLoop* loop_;

EventLoop* ownerLoop() { return loop_; }

void Channel::update()
{
  loop_->updateChannel(this);
}

void Channel::remove()
{
  assert(isNoneEvent());
  loop_->removeChannel(this);
}
```
这些函数在后续章节使用时会在深入介绍。

### 4、响应事件

muduo中定义了该函数来响应Channel所绑定的文件描述符发生事件及其回调函数：
```c
void Channel::handleEvent(Timestamp receiveTime)
{
  boost::shared_ptr<void> guard;
  if (tied_)
  {
    guard = tie_.lock();
    if (guard)
    {
      handleEventWithGuard(receiveTime);
    }
  }
  else
  {
    handleEventWithGuard(receiveTime);
  }
}
```
该函数中主要调用了handleEventWithGuard，handleEventWithGuard定义如下：

```c
void Channel::handleEventWithGuard(Timestamp receiveTime)
{
  eventHandling_ = true;
  if ((revents_ & POLLHUP) && !(revents_ & POLLIN))
  {
    if (logHup_)
    {
      LOG_WARN << "Channel::handle_event() POLLHUP";
    }
    if (closeCallback_) closeCallback_();
  }

  if (revents_ & POLLNVAL)
  {
    LOG_WARN << "Channel::handle_event() POLLNVAL";
  }

  if (revents_ & (POLLERR | POLLNVAL))
  {
    if (errorCallback_) errorCallback_();
  }
  if (revents_ & (POLLIN | POLLPRI | POLLRDHUP))
  {
    if (readCallback_) readCallback_(receiveTime);
  }
  if (revents_ & POLLOUT)
  {
    if (writeCallback_) writeCallback_();
  }
  eventHandling_ = false;
}
```
该函数会根据revents_判断该文件描述符上实际发生的事件类型，然后调用相关的注册的回调函数。例如如果是有POLLIN（读事件）产生，那么将调用readCallback_回调函数

那么什么时候handleEvent函数会执行呢，这里可以先提前窥探一下：

在EventLoop源码中定义了该函数：
```c
void EventLoop::loop()
{
...
  while (!quit_)
  {
    activeChannels_.clear();
    pollReturnTime_ = poller_->poll(kPollTimeMs, &activeChannels_);
    ++iteration_;
    if (Logger::logLevel() <= Logger::TRACE)
    {
      printActiveChannels();
    }
    // TODO sort channel by priority
    eventHandling_ = true;
    for (ChannelList::iterator it = activeChannels_.begin();
        it != activeChannels_.end(); ++it)
    {
      currentActiveChannel_ = *it;
      currentActiveChannel_->handleEvent(pollReturnTime_);
    }
...
  }
...
}
```

所以在poll/epoll返回时，EventLoop会拿到有事件发生的Channel集合，并逐一执行它们的handleEvent函数。

到此，Channel的主要代码就分析完了，一些次要的，或者没有介绍的将在后面分析EventLoop、poll/epoll时介绍。