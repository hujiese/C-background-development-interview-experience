## ThreadPool 线程池

更多可见 [muduo源码分析--ThreadPool线程池](https://github.com/hujiese/Large-concurrent-serve/blob/master/13_muduo_ThreadPool/muduo_ThreadPool.md)

muduo的线程池如下所示：

![](https://camo.githubusercontent.com/68e3c524a531532fbb6d439df1e24217c3ead3ce/68747470733a2f2f692e696d6775722e636f6d2f6a65483231516b2e706e67)

该线程池使用boost::ptr_vector来保持动态创建的线程，线程start时会根据使用者传入的参数来确定要创建线程的数量，threads_保存的正是这些动态创建的线程：
```c
boost::ptr_vector<muduo::Thread> threads_;
```
该线程池使用deque数据结构维持一个任务队列：

```c
std::deque<Task> queue_;
typedef boost::function<void ()> Task;
```

所以，到这里很明显，线程池会提供往该任务队列里添加任务的函数，而这些线程池里的线程则会去抢占任务队列里的任务，线程池模型本质上就是一个“生产者-消费者”模型。由于是“生产者-消费者”模型，所以该线程池也持有成员变量：

```c
MutexLock mutex_;
Condition cond_;
```
muduo的线程池使用条件变量来完成同步。

### 1、创建线程池

该线程池创建时需要做一些初始化操作：
```c
ThreadPool::ThreadPool(const string& name)
  : mutex_(),
    cond_(mutex_),
    name_(name),
    running_(false)
{
}
```
传入参数name设置线程池名字，初始化互斥锁和条件变量，设置运行标准running_为false。

### 2、启动线程池

```c
void ThreadPool::start(int numThreads)
{
  assert(threads_.empty());
  running_ = true;
  threads_.reserve(numThreads);
  for (int i = 0; i < numThreads; ++i)
  {
    char id[32];
    snprintf(id, sizeof id, "%d", i);
    threads_.push_back(new muduo::Thread(
          boost::bind(&ThreadPool::runInThread, this), name_+id));
    threads_[i].start();
  }
}
```
在调用strat函数时需要向该函数传入需要线程的数量。然后该函数会断言判断线程池向量里是否有线程，正常情况下，在这一时刻是没有任何线程创建的，如果threads_不为空，则说明程序存在问题。在检查完threads_后便设置线程池运行标志位running_为true，然后给threads_设置长度，创建numThreads个线程并送入threads_中管理。

### 3、线程执行函数

从代码：
```c
threads_.push_back(new muduo::Thread(boost::bind(&ThreadPool::runInThread, this), name_+id));
```
中可以看到，线程的任务是ThreadPool::runInThread函数，该函数定义如下：

```c
void ThreadPool::runInThread()
{
  try
  {
    while (running_)
    {
      Task task(take());
      if (task)
      {
        task();
      }
    }
  }
  catch (const Exception& ex)
  {
    fprintf(stderr, "exception caught in ThreadPool %s\n", name_.c_str());
    fprintf(stderr, "reason: %s\n", ex.what());
    fprintf(stderr, "stack trace: %s\n", ex.stackTrace());
    abort();
  }
  catch (const std::exception& ex)
  {
    fprintf(stderr, "exception caught in ThreadPool %s\n", name_.c_str());
    fprintf(stderr, "reason: %s\n", ex.what());
    abort();
  }
  catch (...)
  {
    fprintf(stderr, "unknown exception caught in ThreadPool %s\n", name_.c_str());
    throw; // rethrow
  }
}
```

该函数如果在线程启动的情况下会去执行take()函数，获取一个任务，如果任务为空则不执行，否则执行任务。take()函数定义如下：

```c
ThreadPool::Task ThreadPool::take()
{
  MutexLockGuard lock(mutex_);
  // always use a while-loop, due to spurious wakeup
  while (queue_.empty() && running_)
  {
    cond_.wait();
  }
  Task task;
  if(!queue_.empty())
  {
    task = queue_.front();
    queue_.pop_front();
  }
  return task;
}
```

很明显，take()函数是扮演“消费者”角色，该函数内部会尝试从任务队列中取任务，如果任务队列为空则阻塞。

### 4、给线程池任务队列添加任务

```c
void ThreadPool::run(const Task& task)
{
  if (threads_.empty())
  {
    task();
  }
  else
  {
    MutexLockGuard lock(mutex_);
    queue_.push_back(task);
    cond_.notify();
  }
}
```
使用run函数可以向任务队列中添加任务，很明显，该函数扮演的是“生产者”。在该函数中，如果创建线程数量为0，那么就在当前线程中直接运行该任务，否则将该任务添加到任务队列中，并通知工作线程解除阻塞，让线程池里的空闲线程抢占该任务。

到此，muduo的线程池便分析结束了。