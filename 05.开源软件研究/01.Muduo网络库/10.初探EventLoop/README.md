## 初探EventLoop

更多可见 [muduo源码分析--EventLoop](https://github.com/hujiese/Large-concurrent-serve/blob/master/16_muduo_EventLoop/muduo_EventLoop.md)

### 1、什么都不做的EventLoop

一个EventLoop就是一个事件循环，下面将通过一个“什么都不做的EventLoop”来大致描述下muduo中EventLoop的功能。

“什么都不做的EventLoop”有如下几个特点：

* one loop per thread意思是说每个线程最多只能有一个EventLoop对象。
* EventLoop对象构造的时候，会检查当前线程是否已经创建了其他EventLoop对象，如果已创建，终止程序（LOG_FATAL）
* EventLoop构造函数会记住本对象所属线程（threadId_）。
* 创建了EventLoop对象的线程称为IO线程，其功能是运行事件循环（EventLoop::loop）

具体代码可在src下看到。

### 2、one loop per thread

这里指的是一个EventLoop只属于一个线程（但一个线程可以拥有多个EventLoop），在muduo中，如果EventLoop一旦被创建，EventLoop会保持所属线程的一份tid拷贝，作为标识。如果该EventLoop被其他线程调用则会报错，muduo中使用assertInLoopThread函数来判断该EventLoop对象是否是在所属线程中执行。相关代码如下：

```c
EventLoop::EventLoop()
  : looping_(false),
    threadId_(CurrentThread::tid())
{
  LOG_TRACE << "EventLoop created " << this << " in thread " << threadId_;
  // 如果当前线程已经创建了EventLoop对象，终止(LOG_FATAL)
  if (t_loopInThisThread)
  {
    LOG_FATAL << "Another EventLoop " << t_loopInThisThread
              << " exists in this thread " << threadId_;
  }
  else
  {
    t_loopInThisThread = this;
  }
}

void assertInLoopThread()
{
  if (!isInLoopThread())
  {
    abortNotInLoopThread();
  }
}

bool isInLoopThread() const { return threadId_ == CurrentThread::tid(); }

void EventLoop::abortNotInLoopThread()
{
  LOG_FATAL << "EventLoop::abortNotInLoopThread - EventLoop " << this
            << " was created in threadId_ = " << threadId_
            << ", current thread id = " <<  CurrentThread::tid();
} 
```

### 3、事件循环

一个EventLoop里其实是调用了poll/epoll来跟踪所关注的文件描述符的，当有文件描述符上有事件发生时，EventLoop会拿到那些发生事件的文件描述符信息，这里的只是简单示例，更具体的内容会在后续章节不断展开。

```c
void EventLoop::loop()
{
  assert(!looping_);
  // 断言当前处于创建该对象的线程中
  assertInLoopThread();
  looping_ = true;
  LOG_TRACE << "EventLoop " << this << " start looping";

  ::poll(NULL, 0, 5*1000);

  LOG_TRACE << "EventLoop " << this << " stop looping";
  looping_ = false;
}
```