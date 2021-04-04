## 线程池服务器

### 一、线程池

线程池的设计思想大致如下：

1、一个线程池类

2、该线程池类需要有启动线程池函数：

    start(number)

该函数将创建number数量的线程，设置线程启动标志isRun=true。这些线程会运行一个函数：

    runInThreadPool()
    pthread(tid, NULL, runInThreadPool, this)

在该函数中runInThreadPool必须设置为static，这是pthread参数所决定的，在runInThreadPool中才运行真正的线程代码：

    takeTask()

从任务队列中取出任务执行。

3、该线程池需要一个任务队列，也需要操作该任务队列的函数

    putTask(taks)

和

    take()

putTask(task)传入的是需要线程执行的函数，task函数会被加入到任务队列中，所以这个需要加锁保护，可以使用条件变量。take()则是从该任务队列中取出task执行，在任务队列为空情况下会一直阻塞，一旦任务队列有任务了便取出执行，这个过程也可以使用条件变量来处理。

4、停止线程池

设置isRun=false，将队列清空。