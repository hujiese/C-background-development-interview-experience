## Condition条件变量

更多可见 [muduo源码分析--Condition条件变量](https://github.com/hujiese/Large-concurrent-serve/blob/master/10_muduo_Condition/muduo_Condition.md)

### 一、condition条件变量回顾

![](https://camo.githubusercontent.com/e98fef3ca8cbb5440b09868900a58dc775d21159/68747470733a2f2f692e696d6775722e636f6d2f4937724569764a2e706e67)

条件变量比较关键的地方就是while循环中的等待，如果条件不满足，那么条件变量将解锁，然后在while循环中一直等待条件变量满足，于此同时，其他线程通过某种方式改变了条件，然后发出信号通知等待线程条件满足，然后等待线程加锁推出while循环。

### 二、实现

![](https://camo.githubusercontent.com/e99ee2c7fa9db0bf9413cbd0bc9c6b36f1da79db/68747470733a2f2f692e696d6775722e636f6d2f6236454350494f2e706e67)

该类只是很简单地对pthread_cond_t进行封装。

### 三、CountDownLatch

![](https://camo.githubusercontent.com/68536a33b06cba7424d2c8c2d5c0c181ac452069/68747470733a2f2f692e696d6775722e636f6d2f466e776f6b41742e706e67)

该类是muduo的互斥锁与条件变量使用的案例典范：

```c
void CountDownLatch::wait()
{
  MutexLockGuard lock(mutex_);
  while (count_ > 0) {
    condition_.wait();
  }
}

void CountDownLatch::countDown()
{
  MutexLockGuard lock(mutex_);
  --count_;
  if (count_ == 0) {
    condition_.notifyAll();
  }
}
```
由此可以实现生产者消费者模型，达到线程同步的目的。

### 四、BlockinngQueue

![](https://camo.githubusercontent.com/a69edb21659c74d84ab067bad500000d8bf4f718/68747470733a2f2f692e696d6775722e636f6d2f716c546c3131672e706e67)

muduo_BlockinngQueue利用了条件变量来进行阻塞队列的操作，是一个典型的“生产者消费者”模型：

```c
void put(const T& x)
{
  MutexLockGuard lock(mutex_);
  queue_.push_back(x);
  notEmpty_.notify(); // TODO: move outside of lock
}

T take()
{
  MutexLockGuard lock(mutex_);
  // always use a while-loop, due to spurious wakeup
  while (queue_.empty())
  {
    notEmpty_.wait();
  }
  assert(!queue_.empty());
  T front(queue_.front());
  queue_.pop_front();
  return front;
}
  ```

  ### 五、BoundedBlockingQueue

![](https://camo.githubusercontent.com/d3fd3d5ee1fb0ce87b0ce112070536d4c2c9bc2f/68747470733a2f2f692e696d6775722e636f6d2f454e484d3446782e706e67)  

BoundedBlockingQueue与BlockinngQueue相比最大的区别在于该队列是有长度限制的，如果生产者生产的东西过多导致队列已满，那么生产者将被阻塞生产任务：

```c
void put(const T& x)
{
  MutexLockGuard lock(mutex_);
  while (queue_.full())
  {
    notFull_.wait();
  }
  assert(!queue_.full());
  queue_.push_back(x);
  notEmpty_.notify(); // TODO: move outside of lock
}

T take()
{
  MutexLockGuard lock(mutex_);
  while (queue_.empty())
  {
    notEmpty_.wait();
  }
  assert(!queue_.empty());
  T front(queue_.front());
  queue_.pop_front();
  notFull_.notify(); // TODO: move outside of lock
  return front;
}
```
