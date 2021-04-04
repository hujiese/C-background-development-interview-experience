## EventLoopThreadPool

更多可见 [muduo源码分析--TcpServer扩充 && EventLoopThreadPool](https://github.com/hujiese/Large-concurrent-serve/blob/master/25_muduo_TcpServer_EventLoopThreadPool/muduo_TcpServer_EventLoopThreadPool.md)

该类的作用就是“事先缓存多个EventLoopThread”，所以该类中需要有保存这些EventLoopThread及其EventLoop的数据结构：
```c
boost::ptr_vector<EventLoopThread> threads_;
std::vector<EventLoop*> loops_;
```
通过setThreadNum函数来设置需要创建多少个IO线程：
```c
void setThreadNum(int numThreads) { numThreads_ = numThreads; }
```
如果用户传入数量为0（默认构造函数中设置为0），那么该EventLoopThreadPool就会作为IO线程，EventLoopThreadPool内也持有一个EventLoop：
```c
EventLoop* baseLoop_;

EventLoopThreadPool::EventLoopThreadPool(EventLoop* baseLoop)
  : baseLoop_(baseLoop),
    started_(false),
    numThreads_(0),
    next_(0)
{
}
```
调用EventLoopThreadPool::start函数创建numThreads_个EventLoopThread。如果numThreads_为0且有线程初始化函数需要执行，那么就在EventLoopThreadPool所处线程中执行该函数。
```c
void EventLoopThreadPool::start(const ThreadInitCallback& cb)
{
  assert(!started_);
  baseLoop_->assertInLoopThread();

  started_ = true;

  for (int i = 0; i < numThreads_; ++i)
  {
    EventLoopThread* t = new EventLoopThread(cb);
    threads_.push_back(t);
    loops_.push_back(t->startLoop());
  }
  if (numThreads_ == 0 && cb)
  {
    cb(baseLoop_);
  }
}
```

考虑到负载均衡，该类还实现了一个基于轮询方法来选择要使用的EventLoop（IO线程）：
```c
EventLoop* EventLoopThreadPool::getNextLoop()
{
  baseLoop_->assertInLoopThread();
  EventLoop* loop = baseLoop_;

  if (!loops_.empty())
  {
    // round-robin
    loop = loops_[next_];
    ++next_;
    if (implicit_cast<size_t>(next_) >= loops_.size())
    {
      next_ = 0;
    }
  }
  return loop;
}
```

到此EventLoopThreadPool便分析完了。