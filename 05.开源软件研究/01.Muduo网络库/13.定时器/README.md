<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->


- [定时器](#%E5%AE%9A%E6%97%B6%E5%99%A8)
  - [一、定时器选择](#%E4%B8%80%E5%AE%9A%E6%97%B6%E5%99%A8%E9%80%89%E6%8B%A9)
    - [1、timerfd_create](#1timerfd_create)
    - [2、timerfd_settime](#2timerfd_settime)
    - [3、timerfd_gettime](#3timerfd_gettime)
  - [二、Timer 分析](#%E4%BA%8Ctimer-%E5%88%86%E6%9E%90)
  - [三、TimerId 分析](#%E4%B8%89timerid-%E5%88%86%E6%9E%90)
  - [四、TimerQueue](#%E5%9B%9Btimerqueue)
    - [1、TimerQueue 设计原理](#1timerqueue-%E8%AE%BE%E8%AE%A1%E5%8E%9F%E7%90%86)
    - [2、向TimerQueue 中添加一个Timer](#2%E5%90%91timerqueue-%E4%B8%AD%E6%B7%BB%E5%8A%A0%E4%B8%80%E4%B8%AAtimer)
    - [3、获取超时Timer](#3%E8%8E%B7%E5%8F%96%E8%B6%85%E6%97%B6timer)
    - [4、向TimerQueue 中取消一个Timer](#4%E5%90%91timerqueue-%E4%B8%AD%E5%8F%96%E6%B6%88%E4%B8%80%E4%B8%AAtimer)
    - [5、重置超时Timer](#5%E9%87%8D%E7%BD%AE%E8%B6%85%E6%97%B6timer)
    - [6、析构函数与资源销毁](#6%E6%9E%90%E6%9E%84%E5%87%BD%E6%95%B0%E4%B8%8E%E8%B5%84%E6%BA%90%E9%94%80%E6%AF%81)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## 定时器

muduo的定时器由三个类实现，TimerId、Timer、TimerQueue，用户只能看到第一个类，其它两个都是内部实现细节

### 一、定时器选择

定时函数，用于让程序等待一段时间或安排计划任务，Linux有如下定时器方案可以选择：

* sleep 
* alarm 
* usleep 
* nanosleep 
* clock_nanosleep 
* getitimer / setitimer 
* timer_create / timer_settime / timer_gettime / timer_delete 
* timerfd_create / timerfd_gettime / timerfd_settime

muduo选择了timerfd_*系列作为定时器方案，下面是入选的几个原因：

* sleep / alarm / usleep 在实现时有可能用了信号 SIGALRM，在多线程程序中处理信号是个相当麻烦的事情，应当尽量避免
* nanosleep 和 clock_nanosleep 是线程安全的，但是在非阻塞网络编程中，绝对不能用让线程挂起的方式来等待一段时间，程序会失去响应。正确的做法是注册一个时间回调函数。 
* getitimer 和 timer_create 也是用信号来 deliver 超时，在多线程程序中也会有麻烦。
timer_create 可以指定信号的接收方是进程还是线程，算是一个进步，不过在信号处理函数(signal handler)能做的事情实在很受限。 
* timerfd_create 把时间变成了一个文件描述符，该“文件”在定时器超时的那一刻变得可读，这样就能很方便地融入到 select/poll 框架中，用统一的方式来处理 IO 事件和超时事件，这也正是 Reactor 模式的长处。

下面这组函数便是操作timerfd的系统调用：
```c
#include <sys/timerfd.h>

int timerfd_create(int clockid, int flags);

int timerfd_settime(int fd, int flags, const struct itimerspec *new_value, struct itimerspec *old_value);

int timerfd_gettime(int fd, struct itimerspec *curr_value)
```

#### 1、timerfd_create
```c
int timerfd_create(int clockid, int flags);
```
它是用来创建一个定时器描述符timerfd

* 第一个参数：clockid指定时间类型，有两个值：
    * CLOCK_REALTIME :Systemwide realtime clock. 系统范围内的实时时钟

    * CLOCK_MONOTONIC:以固定的速率运行，从不进行调整和复位 ,它不受任何系统time-of-day时钟修改的影响

* 第二个参数：flags可以是0或者O_CLOEXEC/O_NONBLOCK。

* 返回值：timerfd（文件描述符）

#### 2、timerfd_settime
```c
int timerfd_settime(int fd, int flags, const struct itimerspec *new_value, struct itimerspec *old_value);
```
该函数作用是用来启动或关闭有fd指定的定时器。在讲解该函数前，先理解两个相关结构体：
```c
struct timespec {
    time_t tv_sec;      /* Seconds */
    long   tv_nsec;     /* Nanoseconds */
};

struct itimerspec {
    struct timespec it_interval;  /* Interval for periodic timer */
    struct timespec it_value;     /* Initial expiration */
};
```
第二个结构体itimerspec就是timerfd要设置的超时结构体，它的成员it_value表示定时器第一次超时时间，it_interval表示之后的超时时间即每隔多长时间超时

下面正式分析该函数的参数和返回值：

参数：

* fd：timerfd，有timerfd_create函数返回
* flags：1代表设置的是绝对时间；为0代表相对时间
* fnew_value：指定新的超时时间，设定new_value.it_value非零则启动定时器，否则关闭定时器，如果new_value.it_interval为0，则定时器只定时一次，即初始那次，否则之后每隔设定时间超时一次

* old_value：不为null，则返回定时器这次设置之前的超时时间

#### 3、timerfd_gettime
```c
int timerfd_gettime(int fd, struct itimerspec *curr_value)
```
此函数用于获得定时器距离下次超时还剩下的时间。如果调用时定时器已经到期，并且该定时器处于循环模式（设置超时时间时struct itimerspec::it_interval不为0），那么调用此函数之后定时器重新开始计时。

### 二、Timer 分析

一个定时器会在某一个时刻执行某个任务，而且有可能是“定时执行”，所以，一个Timer里封装了该Timer的超时时间（定时时间）、任务的执行周期等信息：
```c
const TimerCallback callback_; //定时执行函数
Timestamp expiration_;  // 超时时间
const double interval_; // 执行间隔
const bool repeat_;  // 是否重复定时执行任务
const int64_t sequence_; // 序列号，每个Tmier的序列号都不一样
```
其中sequence_是通过对一个静态变量的原子增加来保证独一无二的：
```c
static AtomicInt64 s_numCreated_;
```
该原子操作会在Timer构造函数初始化列表中完成初始化：
```c
Timer(const TimerCallback& cb, Timestamp when, double interval)
  : callback_(cb),
    expiration_(when),
    interval_(interval),
    repeat_(interval > 0.0),
    sequence_(s_numCreated_.incrementAndGet())
{ }
```
Timer也提供了一些列函数来操作和获取这些变量值：
```c
Timestamp expiration() const  { return expiration_; }
bool repeat() const { return repeat_; }
int64_t sequence() const { return sequence_; }
static int64_t numCreated() { return s_numCreated_.get(); }

void run() const
{
  callback_();
}
```
如果要重新设置该Timer的超时时间，可以通过下面这个函数：
```c
void restart(Timestamp now);

void Timer::restart(Timestamp now)
{
  if (repeat_)
  {
    expiration_ = addTime(now, interval_);
  }
  else
  {
    expiration_ = Timestamp::invalid();
  }
}
```
到此，Timer分析结束。

### 三、TimerId 分析

TimerId 中保存了一个Timer，以及该TimerId的序列号，同时将TimerQueue 设置为友元类：
```c
class TimerId : public muduo::copyable
{
 public:
  TimerId()
    : timer_(NULL),
      sequence_(0)
  {
  }

  TimerId(Timer* timer, int64_t seq)
    : timer_(timer),
      sequence_(seq)
  {
  }

  // default copy-ctor, dtor and assignment are okay

  friend class TimerQueue;

 private:
  Timer* timer_;
  int64_t sequence_;
};
```

### 四、TimerQueue

#### 1、TimerQueue 设计原理

一个TimerQueue 会关联一个EventLoop，一个TimerQueue中会绑定一个文件描述符timerfd以及Channel。该TimerQueue中保存众多的Timer，并且将这些Timer根据其超时时间从小到大放入一个集合中，其中最近超时时间将设置为timerfd的超时时间，到了该时间，timerfd上会有读事件发生，然后定时器会将定时在该超时时间到真正处理该读事件之间的Timer获取到，执行这些Timer的定时函数。

所以有：
```c
typedef std::pair<Timestamp, Timer*> Entry;
typedef std::set<Entry> TimerList;
typedef std::pair<Timer*, int64_t> ActiveTimer;
typedef std::set<ActiveTimer> ActiveTimerSet;

EventLoop* loop_;
const int timerfd_;
Channel timerfdChannel_;
// Timer list sorted by expiration
TimerList timers_;
// for cancel()
ActiveTimerSet activeTimers_;
```
TimerQueue 的构造函数如下：
```c
TimerQueue::TimerQueue(EventLoop* loop)
  : loop_(loop),
    timerfd_(createTimerfd()),
    timerfdChannel_(loop, timerfd_),
    timers_(),
    callingExpiredTimers_(false)
{
  timerfdChannel_.setReadCallback(
      boost::bind(&TimerQueue::handleRead, this));
  // we are always reading the timerfd, we disarm it with timerfd_settime.
  timerfdChannel_.enableReading();
}
```
构造函数中使用了createTimerfd来创建一个定时器文件描述符，该函数定义如下：
```c
int createTimerfd()
{
  int timerfd = ::timerfd_create(CLOCK_MONOTONIC,
                                 TFD_NONBLOCK | TFD_CLOEXEC);
  if (timerfd < 0)
  {
    LOG_SYSFATAL << "Failed in timerfd_create";
  }
  return timerfd;
}
```
构造函数中Channel绑定的“读事件”回调函数是TimerQueue::handleRead，该函数定义如下：
```c
void TimerQueue::handleRead()
{
  loop_->assertInLoopThread();
  Timestamp now(Timestamp::now());
  readTimerfd(timerfd_, now);

  std::vector<Entry> expired = getExpired(now);

  callingExpiredTimers_ = true;
  cancelingTimers_.clear();
  // safe to callback outside critical section
  for (std::vector<Entry>::iterator it = expired.begin();
      it != expired.end(); ++it)
  {
    it->second->run();
  }
  callingExpiredTimers_ = false;

  reset(expired, now);
}
```
如果定时器超时，那么将执行该回调函数，该回调函数会调用readTimerfd函数先处理文件描述符事件，readTimerfd 函数定义如下：
```c
void readTimerfd(int timerfd, Timestamp now)
{
  uint64_t howmany;
  ssize_t n = ::read(timerfd, &howmany, sizeof howmany);
  LOG_TRACE << "TimerQueue::handleRead() " << howmany << " at " << now.toString();
  if (n != sizeof howmany)
  {
    LOG_ERROR << "TimerQueue::handleRead() reads " << n << " bytes instead of 8";
  }
}
```
在处理完文件描述符读事件后调用getExpired函数获取从文件描述符事件发生到当前时间内超时的Timer，然后遍历该集合，获取到Timer并执行其run函数（run函数中执行的是Timer真正的定时函数）。处理完这些超时的Timer后调用reset函数来重置这些Timer的定时任务，因为有些Timer是设置为循环执行的，所以还要重新设置它们的下一次执行时间。

#### 2、向TimerQueue 中添加一个Timer
TimerQueue使用addTimer函数来向队列中添加一个Timer：
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
```
该函数需要传入一个定时回调函数，一个超时时间戳和定时周期，TimerCallback的原型如下所示：
```c
typedef boost::function<void()> TimerCallback;
```
通过上面三个变量创建了一个Timer，然后通过EventLoop调用TimerQueue::addTimerInLoop函数，返回一个TimerId。runInLoop的实现细节在后面章节会解释，addTimerInLoop函数的定义如下：
```c
void TimerQueue::addTimerInLoop(Timer* timer)
{
  loop_->assertInLoopThread();
  bool earliestChanged = insert(timer);

  if (earliestChanged)
  {
    resetTimerfd(timerfd_, timer->expiration());
  }
}
```
该函数又调用了insert函数，insert定义如下：
```c
bool TimerQueue::insert(Timer* timer)
{
  loop_->assertInLoopThread();
  assert(timers_.size() == activeTimers_.size());
  bool earliestChanged = false;
  Timestamp when = timer->expiration();
  TimerList::iterator it = timers_.begin();
  if (it == timers_.end() || when < it->first)
  {
    earliestChanged = true;
  }
  {
    std::pair<TimerList::iterator, bool> result
      = timers_.insert(Entry(when, timer));
    assert(result.second); (void)result;
  }
  {
    std::pair<ActiveTimerSet::iterator, bool> result
      = activeTimers_.insert(ActiveTimer(timer, timer->sequence()));
    assert(result.second); (void)result;
  }

  assert(timers_.size() == activeTimers_.size());
  return earliestChanged;
}
```
insert函数会获取到该Timer的超时时间，如果该Timer的超时时间是最近的（比之前从小到大排列的超时集合最小的还小），说明该Timer的定时函数是最早执行的，所以设置标志earliestChanged = true，然后将该Timer加入到timers\_和activeTimers_中，然后返回earliestChanged。

回到addTimerInLoop函数，该函数在插入一个Timer后，判断earliestChanged，如果该项为true，那么也需要重新设置TimerQueue的timerfd_的定时时间，则调用resetTimerfd函数：
```c
void resetTimerfd(int timerfd, Timestamp expiration)
{
  // wake up loop by timerfd_settime()
  struct itimerspec newValue;
  struct itimerspec oldValue;
  bzero(&newValue, sizeof newValue);
  bzero(&oldValue, sizeof oldValue);
  newValue.it_value = howMuchTimeFromNow(expiration);
  int ret = ::timerfd_settime(timerfd, 0, &newValue, &oldValue);
  if (ret)
  {
    LOG_SYSERR << "timerfd_settime()";
  
}
```
这样一来又将最近超时时刻和timerfd_绑定了。

#### 3、获取超时Timer
```c
std::vector<TimerQueue::Entry> TimerQueue::getExpired(Timestamp now)
{
  assert(timers_.size() == activeTimers_.size());
  std::vector<Entry> expired;
  Entry sentry(now, reinterpret_cast<Timer*>(UINTPTR_MAX));
  TimerList::iterator end = timers_.lower_bound(sentry);
  assert(end == timers_.end() || now < end->first);
  std::copy(timers_.begin(), end, back_inserter(expired));
  timers_.erase(timers_.begin(), end);

  for (std::vector<Entry>::iterator it = expired.begin();
      it != expired.end(); ++it)
  {
    ActiveTimer timer(it->second, it->second->sequence());
    size_t n = activeTimers_.erase(timer);
    assert(n == 1); (void)n;
  }

  assert(timers_.size() == activeTimers_.size());
  return expired;
}
```
getExpired函数会获取到将小于传入时间戳now的Timer，将这些Timer的Entry保存到一个vector中并返回，同时删除timers_和activeTimers\_中的这些和Timer相关的部分。
#### 4、向TimerQueue 中取消一个Timer
muduo使用cancle来取消一个Timer：
```c
void TimerQueue::cancel(TimerId timerId)
{
  loop_->runInLoop(
      boost::bind(&TimerQueue::cancelInLoop, this, timerId));
}
```
该函数又调用了TimerQueue::cancelInLoop：
```c
void TimerQueue::cancelInLoop(TimerId timerId)
{
  loop_->assertInLoopThread();
  assert(timers_.size() == activeTimers_.size());
  ActiveTimer timer(timerId.timer_, timerId.sequence_);
  ActiveTimerSet::iterator it = activeTimers_.find(timer);
  if (it != activeTimers_.end())
  {
    size_t n = timers_.erase(Entry(it->first->expiration(), it->first));
    assert(n == 1); (void)n;
    delete it->first; // FIXME: no delete please
    activeTimers_.erase(it);
  }
  else if (callingExpiredTimers_)
  {
    cancelingTimers_.insert(timer);
  }
  assert(timers_.size() == activeTimers_.size());
}
```
该函数以TimerId的方式从TimerQueue中删除相关Timer内容，如果该当前TimerQueue已经有超时事件产生（handleRead中callingExpiredTimers_设置为true），那么还需要将该Timer保存到“待取消定时器队列”中：
```c
ActiveTimerSet cancelingTimers_;
```
至于为什么会有这样一个队列，在下面一节将会介绍。
#### 5、重置超时Timer
在handleRead函数中：
```c
void TimerQueue::handleRead()
{
  ...
  std::vector<Entry> expired = getExpired(now);

  callingExpiredTimers_ = true;
  cancelingTimers_.clear();
  ...
  callingExpiredTimers_ = false;

  reset(expired, now);
}
```
如果已经去除队列中的超时定时器集合，这些集合的定时任务也处理完了之后还需要reset这些集合，因为一个定时器Timer可能设置了repeat_和interval\_，需要重新设置下一次Timer的执行时间。reset函数定义如下：
```c
void TimerQueue::reset(const std::vector<Entry>& expired, Timestamp now)
{
  Timestamp nextExpire;

  for (std::vector<Entry>::const_iterator it = expired.begin();
      it != expired.end(); ++it)
  {
    ActiveTimer timer(it->second, it->second->sequence());
    if (it->second->repeat()
        && cancelingTimers_.find(timer) == cancelingTimers_.end())
    {
      it->second->restart(now);
      insert(it->second);
    }
    else
    {
      // FIXME move to a free list
      delete it->second; // FIXME: no delete please
    }
  }

  if (!timers_.empty())
  {
    nextExpire = timers_.begin()->second->expiration();
  }

  if (nextExpire.valid())
  {
    resetTimerfd(timerfd_, nextExpire);
  }
}
```
这个函数会检查这些超时的定时器Timer，如果是设置了repeat_为true并且不在“取消定时器队列”中则重置下一次定时器执行时间，否则（如一次性定时任务，或者取消了但未能避免执行）则直接删除。在执行handleRead时也会将cancelingTimers\_清空。最后设置TimerQueue的timerfd\_最近定时时间，准备下一次执行定时事件的到来。
#### 6、析构函数与资源销毁
```c
TimerQueue::~TimerQueue()
{
  ::close(timerfd_);
  // do not remove channel, since we're in EventLoop::dtor();
  for (TimerList::iterator it = timers_.begin();
      it != timers_.end(); ++it)
  {
    delete it->second;
  }
}
```
关闭timerfd_文件描述符，释放管理的所有Timer资源。

到此，定时器分析全部结束了。