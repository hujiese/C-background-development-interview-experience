<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->

- [深入 EventLoop](#%E6%B7%B1%E5%85%A5-eventloop)
  - [1、EventLoop 与 Channel](#1eventloop-%E4%B8%8E-channel)
  - [2、EventLoop 与 TimerQueue](#2eventloop-%E4%B8%8E-timerqueue)
  - [3、EventLoop 与 Poller](#3eventloop-%E4%B8%8E-poller)
  - [4、跨线程执行函数](#4%E8%B7%A8%E7%BA%BF%E7%A8%8B%E6%89%A7%E8%A1%8C%E5%87%BD%E6%95%B0)
  - [5、析构函数与资源销毁](#5%E6%9E%90%E6%9E%84%E5%87%BD%E6%95%B0%E4%B8%8E%E8%B5%84%E6%BA%90%E9%94%80%E6%AF%81)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## 深入 EventLoop
前面章节已经分析过一个“什么都不做的EventLoop”，muduo中的EventLoop有如下几个特点：

* one loop per thread意思是说每个线程最多只能有一个EventLoop对象。
* EventLoop对象构造的时候，会检查当前线程是否已经创建了其他EventLoop对象，如果已创建，终止程序（LOG_FATAL）
* EventLoop构造函数会记住本对象所属线程（threadId_）。
* 创建了EventLoop对象的线程称为IO线程，其功能是运行事件循环（EventLoop::loop）

下面将深入具体分析muduo中的EventLoop的实现。

### 1、EventLoop 与 Channel
前面在分析Channel时说到过，“一个Channel会关联一个EventLoop”，当Channel设置了监听文件描述符关注事件类型后会调用update函数，Channel中的update函数也是间接调用EventLoop的updateChannel函数：
```c
void Channel::update()
{
  loop_->updateChannel(this);
}

void EventLoop::updateChannel(Channel* channel)
{
  assert(channel->ownerLoop() == this);
  assertInLoopThread();
  poller_->updateChannel(channel);
}
```
然后又间接调用Poller的updateChannel函数，该函数在前面分析过，该函数将会将该Channel及其相关文件描述符和Poller的事件监听联系。

同样地，Channel的remove函数也是间接调用EventLoop的removeChannel函数来删除自身：
```c
void Channel::remove()
{
  assert(isNoneEvent());
  loop_->removeChannel(this);
}

void EventLoop::removeChannel(Channel* channel)
{
  assert(channel->ownerLoop() == this);
  assertInLoopThread();
  if (eventHandling_)
  {
    assert(currentActiveChannel_ == channel ||
        std::find(activeChannels_.begin(), activeChannels_.end(), channel) == activeChannels_.end());
  }
  poller_->removeChannel(channel);
}
```
Poller的removeChannel函数会取消该Channel及其文件描述符的事件监听，并不再保存该Channel及其文件描述符任何信息。
### 2、EventLoop 与 TimerQueue
EventLoop中持有一个TimerQueue，并在构造函数中完成了该TimerQueue的初始化：
```c
boost::scoped_ptr<TimerQueue> timerQueue_;

EventLoop::EventLoop()
  : looping_(false),
    ...
    timerQueue_(new TimerQueue(this)),
    ...
{
    ...
}
````
接下来是一组定时器操作函数，用于添加定时器任务：
```c
  ///
  /// Runs callback at 'time'.
  /// Safe to call from other threads.
  ///
  TimerId runAt(const Timestamp& time, const TimerCallback& cb);
  ///
  /// Runs callback after @c delay seconds.
  /// Safe to call from other threads.
  ///
  TimerId runAfter(double delay, const TimerCallback& cb);
  ///
  /// Runs callback every @c interval seconds.
  /// Safe to call from other threads.
  ///
  TimerId runEvery(double interval, const TimerCallback& cb);
  ///
  /// Cancels the timer.
  /// Safe to call from other threads.
  ///
  void cancel(TimerId timerId);

  TimerId EventLoop::runAt(const Timestamp& time, const TimerCallback& cb)
  {
    return timerQueue_->addTimer(cb, time, 0.0);
  }
  
  TimerId EventLoop::runAfter(double delay, const TimerCallback& cb)
  {
    Timestamp time(addTime(Timestamp::now(), delay));
    return runAt(time, cb);
  }
  
  TimerId EventLoop::runEvery(double interval, const TimerCallback& cb)
  {
    Timestamp time(addTime(Timestamp::now(), interval));
    return timerQueue_->addTimer(cb, time, interval);
  }
  
  void EventLoop::cancel(TimerId timerId)
  {
    return timerQueue_->cancel(timerId);
  }
```
本质上这些函数也是调用了TimerQueue的addTimer和cancel函数，这两个函数在前面已经分析过了，不再分析，但是需要关注的是addTimer和cancel函数内部都是通过EventLoop::runInLoop函数调用的：
```c
TimerId TimerQueue::addTimer(const TimerCallback& cb,
                             Timestamp when,
                             double interval)
{
  Timer* timer = new Timer(cb, when, interval);
  loop_->runInLoop(
      boost::bind(&TimerQueue::addTimerInLoop, this, timer));
  return TimerId(timer, timer->sequence());
}

void TimerQueue::cancel(TimerId timerId)
{
  loop_->runInLoop(
      boost::bind(&TimerQueue::cancelInLoop, this, timerId));
}
```
使用EventLoop::runInLoop能够保证该bind函数是在EventLoop所在线程内执行的，哪怕这个函数是在别的线程里被调用，关于EventLoop::runInLoop函数，我们在第四节会重点介绍，分析它是如何实现跨线程安全调用的。
### 3、EventLoop 与 Poller
一个EventLoop持有一个poller：
```c
boost::scoped_ptr<Poller> poller_;

EventLoop::EventLoop()
  : looping_(false),
    ...
    poller_(Poller::newDefaultPoller(this)),
    ...
{
    ...
}
```
Poller::newDefaultPoller会根据系统环境来选择是使用PollPoller还是EPollPoller：
```c
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
EventLoop的核心函数loop便是调用了Poller的loop函数：
```c
void EventLoop::loop()
{
  assert(!looping_);
  assertInLoopThread();
  looping_ = true;
  quit_ = false;
  LOG_TRACE << "EventLoop " << this << " start looping";

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
    currentActiveChannel_ = NULL;
    eventHandling_ = false;
    doPendingFunctors();
  }

  LOG_TRACE << "EventLoop " << this << " stop looping";
  looping_ = false;
}
```
该函数通过调用Poller的poll函数，将活跃Channel保存到activeChannels_中，然后遍历这些Channel，执行它们的handleEvent函数，最后执行doPendingFunctors函数，该函数将在下一节介绍。

### 4、跨线程执行函数
为了实现跨线程调用函数，muduo使用了如下数据结构：
```c
  bool callingPendingFunctors_; /* atomic */
  int wakeupFd_;
  // unlike in TimerQueue, which is an internal class,
  // we don't expose Channel to client.
  boost::scoped_ptr<Channel> wakeupChannel_;
  std::vector<Functor> pendingFunctors_; // @BuardedBy mutex_
```
前面说到，TimerQueue的更新和删除Timer会用到EventLoop::runInLoop函数，该函数的实现如下：
```c
void EventLoop::runInLoop(const Functor& cb)
{
  if (isInLoopThread())
  {
    cb();
  }
  else
  {
    queueInLoop(cb);
  }
}
```
以更新Timer为例：
```c
loop_->runInLoop(boost::bind(&TimerQueue::addTimerInLoop, this, timer));
```
如果该代码时是在EventLoop所在线程中执行，那么TimerQueue::addTimerInLoop会立刻执行，否则TimerQueue::addTimerInLoop将通过queueInLoop函数送入到待执行函数队列中，queueInLoop定义如下：
```c
void EventLoop::queueInLoop(const Functor& cb)
{
  {
  MutexLockGuard lock(mutex_);
  pendingFunctors_.push_back(cb);
  }

  if (!isInLoopThread() || callingPendingFunctors_)
  {
    wakeup();
  }
}
```
该函数会判断是否是在EventLoop所处线程中执行，如果不是的话则调用wakeup()函数。到这里先解释下跨线程调用的原理。一个EventLoop中有一个wakeupFd_文件描述符（eventfd 是一个比 pipe 更高效的线程间事件通知机制，一方面它比 pipe 少用一个 file descripor，节省了资源；另一方面，eventfd 的缓冲区管理也简单得多，全部“buffer” 只有定长8 bytes，不像 pipe 那样可能有不定长的真正 buffer），该文件描述符在构造函数初始化列表中完成初始化：
```c
EventLoop::EventLoop()
  : looping_(false),
  ...
    wakeupFd_(createEventfd()),
  ...
{
  ...
}

int createEventfd()
{
  int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
  if (evtfd < 0)
  {
    LOG_SYSERR << "Failed in eventfd";
    abort();
  }
  return evtfd;
}
```
而且EventLoop也为该wakeuFd_文件描述符提供了一个Channel：
```c
boost::scoped_ptr<Channel> wakeupChannel_;
```
该Channel也在构造函数中完成了初始化工作：
```c
EventLoop::EventLoop()
  : looping_(false),
    ...
    wakeupFd_(createEventfd()),
    wakeupChannel_(new Channel(this, wakeupFd_)),
    ...
{
  ...
  wakeupChannel_->setReadCallback(
      boost::bind(&EventLoop::handleRead, this));
  // we are always reading the wakeupfd
  wakeupChannel_->enableReading();
}
```
该Channel也加入到了Poller事件监听中，并且设置了“读事件”回调函数为EventLoop::handleRead：
```c
void EventLoop::handleRead()
{
  uint64_t one = 1;
  ssize_t n = sockets::read(wakeupFd_, &one, sizeof one);
  if (n != sizeof one)
  {
    LOG_ERROR << "EventLoop::handleRead() reads " << n << " bytes instead of 8";
  }
}
```
现在回到queueInLoop函数中，可以正式介绍wake()函数了：
```c
void EventLoop::wakeup()
{
  uint64_t one = 1;
  ssize_t n = sockets::write(wakeupFd_, &one, sizeof one);
  if (n != sizeof one)
  {
    LOG_ERROR << "EventLoop::wakeup() writes " << n << " bytes instead of 8";
  }
}
```
wakeup函数功能就是向wakeupFd_写入东西，然后促使Poller的poll函数返回：
```c
void EventLoop::loop()
{
  ...
  while (!quit_)
  {
    activeChannels_.clear();
    pollReturnTime_ = poller_->poll(kPollTimeMs, &activeChannels_);
    ...
    doPendingFunctors();
  }
  ...
}
```
然后进而调用doPendingFunctors函数，在该函数中执行这些跨线程调用的“未决函数”：
```c
void EventLoop::doPendingFunctors()
{
  std::vector<Functor> functors;
  callingPendingFunctors_ = true;

  {
  MutexLockGuard lock(mutex_);
  functors.swap(pendingFunctors_);
  }

  for (size_t i = 0; i < functors.size(); ++i)
  {
    functors[i]();
  }
  callingPendingFunctors_ = false;
}
```
这样即使是跨线程调用某些函数，这些函数也不会立刻执行，而是存入该pendingFunctors_集合中等待wake，然后在EventLoop线程中“打包”执行。

需要注意的是：
* 不是简单地在临界区内依次调用Functor，而是把回调列表swap到functors中，这样一方面减小了临界区的长度（意味着不会阻塞其它线程的queueInLoop()），另一方面，也避免了死锁（因为Functor可能再次调用queueInLoop()）
* 由于doPendingFunctors()调用的Functor可能再次调用queueInLoop(cb)，这时，queueInLoop()就必须wakeup()，否则新增的cb可能就不能及时调用了
* muduo没有反复执行doPendingFunctors()直到pendingFunctors为空，这是有意的，否则IO线程可能陷入死循环，无法处理IO事件。

### 5、析构函数与资源销毁

整个EventLoop中创建了如下资源：
```c
EventLoop::EventLoop()
  : looping_(false),
    ...
    poller_(Poller::newDefaultPoller(this)),
    timerQueue_(new TimerQueue(this)),
    wakeupFd_(createEventfd()),
    wakeupChannel_(new Channel(this, wakeupFd_)),
    ...
{
    ...
}
```
然而这些资源除了wakeupFd_以外都是使用智能指针来管理的：
```c
boost::scoped_ptr<Poller> poller_;
boost::scoped_ptr<TimerQueue> timerQueue_;
int wakeupFd_;
// unlike in TimerQueue, which is an internal class,
// we don't expose Channel to client.
boost::scoped_ptr<Channel> wakeupChannel_;
```
所以析构函数只需要释放wakeupFd_文件描述符即可：
```c
EventLoop::~EventLoop()
{
  ::close(wakeupFd_);
  t_loopInThisThread = NULL;
}
```
到此，EventLoop便分析完了。