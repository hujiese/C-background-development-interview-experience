## EventLoopThread

更多可见 [muduo源码分析之EventLoop（扩充）&& EventLoopThread](https://github.com/hujiese/Large-concurrent-serve/blob/master/24_muduo_EventLoop_EventLoopThread/muduo_EventLoop_EventLoopThread.md)

任何一个线程，只要创建并运行了EventLoop，都称之为IO线程，所以IO线程不一定是主线程。

muduo并发模型one loop per thread + threadpool，为了方便今后使用，定义了EventLoopThread类，该类封装了IO线程。EventLoopThread创建了一个线程，然后在线程函数中创建了一个EvenLoop对象并调用EventLoop::loop。

由于该EventLoopThread类会在一个线程中启动一个EventLoop，所以持有如下变量：
```c
EventLoop* loop_;
Thread thread_;
```
构造函数：
```c
EventLoopThread::EventLoopThread(const ThreadInitCallback& cb)
  : loop_(NULL),
    exiting_(false),
    thread_(boost::bind(&EventLoopThread::threadFunc, this)),
    mutex_(),
    cond_(mutex_),
    callback_(cb)
{
}
```
从构造函数中可以看出，该EventLoop默认是没有初始化的，线程绑定的执行函数是EventLoopThread::threadFunc。除此之外，构造函数中还初始化了互斥锁和条件变量，它们作用会在后面分析。

构造函数中还传入了一个ThreadInitCallback类型的函数对象：
```c
ThreadInitCallback callback_;
typedef boost::function<void(EventLoop*)> ThreadInitCallback;
```
这是用作线程启动初始化执行的用户函数，如果用户想在线程初始化时执行一些函数，那么就可以在EventLoopThread构造函数中传入该函数，默认情况下EventLoopThread会创建一个空函数作为线程初始化函数：
```c
EventLoopThread(const ThreadInitCallback& cb = ThreadInitCallback());
```
那么，如何启动一个IO线程呢？使用EventLoopThread::startLoop便可以“启动”一个IO线程，这里的“启动”只的是启动IO线程，而不是线程，因为构造函数在已经初始化一个线程了，光启动该线程还不能称为“IO线程”，因为EventLoop还没有初始化，EventLoopThread::startLoop会通过条件变量一直等待，知道EventLoop被初始化，条件满足时才返回该EventLoop地址：
```c
EventLoop* EventLoopThread::startLoop()
{
  assert(!thread_.started());
  thread_.start();

  {
    MutexLockGuard lock(mutex_);
    while (loop_ == NULL)
    {
      cond_.wait();
    }
  }

  return loop_;
}
```
那么EventLoop在哪里初始化，条件变量又如何被通知呢？这些问题都在线程执行函数中完成：
```c
void EventLoopThread::threadFunc()
{
  EventLoop loop;

  if (callback_)
  {
    callback_(&loop);
  }

  {
    MutexLockGuard lock(mutex_);
    loop_ = &loop;
    cond_.notify();
  }

  loop.loop();
  //assert(exiting_);
}
```
当EventLoopThread类创建后，会在构造函数阶段创建一个线程，而该线程便执行threadFunc函数，如果用户传入了“线程初始化函数”，那么在这里便会优先执行该函数，然后初始化所持有的EventLoop，并发让条件满足（loop_!=NULL），然后启动EventLoop的事件循环。

这一过程下来，当一个EventLoopThread 被创建后，该EventLoopThread 对象内部也创建并启动了一个EventLoop对象，用户代码需要startLoop函数来获取该EventLoop对象，然后让该EventLoop对象和一些Channel绑定，让Channel中文件描述符加入事件监听中。所以该EventLoop可以在其他线程里被调用，这就是为什么EventLoop中还提供了runInLoop函数的原因，也为后续的EventLoopThreadPool作出铺垫。

最后的析构函数则做了些收尾工作：
```c
EventLoopThread::~EventLoopThread()
{
  exiting_ = true;
  loop_->quit();
  thread_.join();
}
```
下面是一个简单的使用案例，可以帮助理解EventLoopThread的使用方法：
```c
#include <muduo/net/EventLoop.h>
#include <muduo/net/EventLoopThread.h>

#include <stdio.h>

using namespace muduo;
using namespace muduo::net;

void runInThread()
{
  printf("runInThread(): pid = %d, tid = %d\n",
	 getpid(), CurrentThread::tid());
}

int main()
{
  printf("main(): pid = %d, tid = %d\n",
	 getpid(), CurrentThread::tid());

  EventLoopThread loopThread;
  EventLoop* loop = loopThread.startLoop();
  // 异步调用runInThread，即将runInThread添加到loop对象所在IO线程，让该IO线程执行
  loop->runInLoop(runInThread);
  sleep(1);
  // runAfter内部也调用了runInLoop，所以这里也是异步调用
  loop->runAfter(2, runInThread);
  sleep(3);
  loop->quit();

  printf("exit main().\n");
}
```