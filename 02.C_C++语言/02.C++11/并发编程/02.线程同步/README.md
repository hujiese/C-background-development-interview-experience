<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->


- [线程同步](#%E7%BA%BF%E7%A8%8B%E5%90%8C%E6%AD%A5)
  - [一、互斥锁](#%E4%B8%80%E4%BA%92%E6%96%A5%E9%94%81)
    - [1、背景](#1%E8%83%8C%E6%99%AF)
    - [2、互斥锁](#2%E4%BA%92%E6%96%A5%E9%94%81)
    - [3、std::lock_guard](#3stdlock_guard)
  - [二、条件变量](#%E4%BA%8C%E6%9D%A1%E4%BB%B6%E5%8F%98%E9%87%8F)
    - [1、std::condition_variable](#1stdcondition_variable)
    - [2、std::condition_variable::wait](#2stdcondition_variablewait)
    - [3、std::condition_variable::wait_for](#3stdcondition_variablewait_for)
    - [4、std::condition_variable::notify_one](#4stdcondition_variablenotify_one)
    - [5、std::condition_variable::notify_all](#5stdcondition_variablenotify_all)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## 线程同步

### 一、互斥锁

#### 1、背景

下面是一个典型的临界资源访问的案例，该案例中产生了竞态，在该案例中创建了三个线程，两个线程往一个全局的list中写入数据，第三个线程从该list中读取数据并打印：
```c
#include <iostream>
#include <thread>
#include <list>
#include <algorithm>
using namespace std;

std::list<int> myList;

void addToList(int max, int interval)
{
	for (int i = 0; i < max; i++) {
		if ((i % interval) == 0) myList.push_back(i);
	}
}

void printList()
{
	for (auto itr = myList.begin(), end_itr = myList.end(); itr != end_itr; ++itr) {
		cout << *itr << ",";
	}
}

int main()
{
	int max = 100;

	std::thread t1(addToList, max, 1);
	std::thread t2(addToList, max, 10);
	std::thread t3(printList);

	t1.join();
	t2.join();
	t3.join();

	return 0;
}
```
运行结果可能如下：
```
0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,
30,31,32,33,34,0,10,20,30,40,50,60,70,80,90,42,43,44,45,46,47,48,49,50,51,52,53,
54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80
,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,
```

线程t1将0-100数字送入到list中，t2将0-100中能被10整除的数字送入到list中，t3则打印list中的数据。但是很明显，数据没有完全打印出来，35-41这部分数据没有被打印。

#### 2、互斥锁

这是线程同步的问题，可以使用互斥锁来解决，C++11中提供了互斥锁函数使用：
```c
#include <iostream>
#include <thread>
#include <list>
#include <algorithm>
#include <mutex>
using namespace std;

std::list<int> myList;
std::mutex mu;

void addToList(int max, int interval)
{
	
	for (int i = 0; i < max; i++) 
	{
		mu.lock();
		if ((i % interval) == 0)
		{
			myList.push_back(i);
		}
		mu.unlock();
	}
}

void printList()
{
	mu.lock();
	for (auto itr = myList.begin(), end_itr = myList.end(); itr != end_itr; ++itr) {
		cout << *itr << ",";
	}
	mu.unlock();
}

int main()
{
	int max = 100;

	std::thread t1(addToList, max, 1);
	std::thread t2(addToList, max, 10);
	std::thread t3(printList);

	t1.join();
	t2.join();
	t3.join();

	return 0;
}
```
输出结果如下：
```
0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,
30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56
,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,8
3,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,0,10,20,30,40,50,60,70,80,90,
```

#### 3、std::lock_guard

使用std::mutex需要lock和unlock，如果忘记了unlock，那就麻烦大了。鉴于此，C++11提供了类似于muduo的MutexLockGuard的std::lock_guard来方便使用互斥锁，作用域区间内一旦结束便自动unlock，内部猜想也是使用了RAII设计的：
```c
#include <iostream>
#include <thread>
#include <list>
#include <algorithm>
#include <mutex>

using namespace std;

// a global variable
std::list<int>myList;

// a global instance of std::mutex to protect global variable
std::mutex myMutex;

void addToList(int max, int interval)
{
	// the access to this function is mutually exclusive
	std::lock_guard<std::mutex> guard(myMutex);
	for (int i = 0; i < max; i++) {
		if ((i % interval) == 0) myList.push_back(i);
	}
}

void printList()
{
	// the access to this function is mutually exclusive
	std::lock_guard<std::mutex> guard(myMutex);
	for (auto itr = myList.begin(), end_itr = myList.end(); itr != end_itr; ++itr) {
		cout << *itr << ",";
	}
}

int main()
{
	int max = 100;

	std::thread t1(addToList, max, 1);
	std::thread t2(addToList, max, 10);
	std::thread t3(printList);

	t1.join();
	t2.join();
	t3.join();

	return 0;
}
```
#### 4、unique_lock

该部分来自 [多线程编程(五)——unique_lock](https://www.jianshu.com/p/34d219380d90)

互斥锁保证了线程间的同步，但是却将并行操作变成了串行操作，这对性能有很大的影响，所以我们要尽可能的减小锁定的区域，也就是使用细粒度锁。

这一点lock_guard做的不好，不够灵活，lock_guard只能保证在析构的时候执行解锁操作，lock_guard本身并没有提供加锁和解锁的接口，但是有些时候会有这种需求。看下面的例子：

```cpp
class LogFile {
    std::mutex _mu;
    ofstream f;
public:
    LogFile() {
        f.open("log.txt");
    }
    ~LogFile() {
        f.close();
    }
    void shared_print(string msg, int id) {
        {
            std::lock_guard<std::mutex> guard(_mu);
            //do something 1
        }
        //do something 2
        {
            std::lock_guard<std::mutex> guard(_mu);
            // do something 3
            f << msg << id << endl;
            cout << msg << id << endl;
        }
    }

};
```
上面的代码中，一个函数内部有两段代码需要进行保护，这个时候使用lock_guard就需要创建两个局部对象来管理同一个互斥锁（其实也可以只创建一个，但是**锁的力度太大，效率不行**），修改方法是使用unique_lock。它提供了lock()和unlock()接口，能记录现在处于上锁还是没上锁状态，在析构的时候，会根据当前状态来决定是否要进行解锁（lock_guard就一定会解锁）。上面的代码修改如下：

```cpp
class LogFile {
    std::mutex _mu;
    ofstream f;
public:
    LogFile() {
        f.open("log.txt");
    }
    ~LogFile() {
        f.close();
    }
    void shared_print(string msg, int id) {

        std::unique_lock<std::mutex> guard(_mu);
        //do something 1
        guard.unlock(); //临时解锁

        //do something 2

        guard.lock(); //继续上锁
        // do something 3
        f << msg << id << endl;
        cout << msg << id << endl;
        // 结束时析构guard会临时解锁
        // 这句话可要可不要，不写，析构的时候也会自动执行
        // guard.ulock();
    }

};
```
上面的代码可以看到，在无需加锁的操作时，可以先临时释放锁，然后需要继续保护的时候，可以继续上锁，这样就无需重复的实例化lock_guard对象，还能减少锁的区域。同样，可以使用std::defer_lock设置初始化的时候不进行默认的上锁操作：

```cpp
void shared_print(string msg, int id) {
    std::unique_lock<std::mutex> guard(_mu, std::defer_lock);
    //do something 1

    guard.lock();
    // do something protected
    guard.unlock(); //临时解锁

    //do something 2

    guard.lock(); //继续上锁
    // do something 3
    f << msg << id << endl;
    cout << msg << id << endl;
    // 结束时析构guard会临时解锁
}
```
这样使用起来就比lock_guard更加灵活！然后这也是有代价的，因为**它内部需要维护锁的状态，所以效率要比lock_guard低**一点，在lock_guard能解决问题的时候，就是用lock_guard，反之，使用unique_lock。

后面在学习条件变量的时候，还会有unique_lock的用武之地。

另外，请注意，**unique_lock和lock_guard都不能复制，lock_guard不能移动，但是unique_lock可以**：

```cpp
// unique_lock 可以移动，不能复制
std::unique_lock<std::mutex> guard1(_mu);
std::unique_lock<std::mutex> guard2 = guard1;  // error
std::unique_lock<std::mutex> guard2 = std::move(guard1); // ok

// lock_guard 不能移动，不能复制
std::lock_guard<std::mutex> guard1(_mu);
std::lock_guard<std::mutex> guard2 = guard1;  // error
std::lock_guard<std::mutex> guard2 = std::move(guard1); // error
```


### 二、条件变量

该部分参考 [C++11并行编程-条件变量(condition_variable)详细说明](https://www.cnblogs.com/bhlsheji/p/5035018.html)

#include <condition_variable>头文件主要包含有类和函数相关的条件变量。

包括相关类 std::condition_variable和 std::condition_variable_any，还有枚举类型std::cv_status。另外还包含函数 std::notify_all_at_thread_exit()，以下分别介绍一下以上几种类型。

#### 1、std::condition_variable

std::condition_variable即条件变量。Linux下使用 Pthread库中的 pthread_cond_t及其相关函数提供了与条件变量相关的功能。C++11提供的条件变量用法和Pthrad库类似，都需要一个互斥锁配合使用，原理也是一样的。

下面是一个整体使用案例：
```c
#include <iostream>                // std::cout
#include <thread>                // std::thread
#include <mutex>                // std::mutex, std::unique_lock
#include <condition_variable>    // std::condition_variable

std::mutex mtx; // 全局相互排斥锁.
std::condition_variable cv; // 全局条件变量.
bool ready = false; // 全局标志位.

void do_print_id(int id)
{
	std::unique_lock <std::mutex> lck(mtx);
	while (!ready) // 假设标志位不为 true, 则等待...
		cv.wait(lck); // 当前线程被堵塞, 当全局标志位变为 true 之后,
	// 线程被唤醒, 继续往下运行打印线程编号id.
	std::cout << "thread " << id << '\n';
}

void go()
{
	std::unique_lock <std::mutex> lck(mtx);
	ready = true; // 设置全局标志位为 true.
	cv.notify_all(); // 唤醒全部线程.
}

int main()
{
	std::thread threads[10];
	// spawn 10 threads:
	for (int i = 0; i < 10; ++i)
		threads[i] = std::thread(do_print_id, i);

	std::cout << "10 threads ready to race...\n";
	go(); // go!

	for (auto & th : threads)
		th.join();

	return 0;
}
```
运行结果如下：
```
10 threads ready to race...
thread 9
thread 7
thread 5
thread 3
thread 1
thread 0
thread 2
thread 6
thread 4
thread 8
```
需要注意的是，std::condition_variable 的拷贝构造函数被禁用，仅仅提供了默认构造函数。

#### 2、std::condition_variable::wait

函数原型如下：
```c
oid wait (unique_lock<mutex>& lck);
template <class Predicate>
  void wait (unique_lock<mutex>& lck, Predicate pred)
```

std::condition_variable提供了两种 wait() 函数。

当前线程调用 wait() 后将被堵塞(此时当前线程应该获得了锁（mutex），最好还是设获得锁 lck)，直到另外某个线程调用 notify_* 唤醒了当前线程。

在线程被堵塞时，该函数会自己主动调用 lck.unlock() 释放锁，使得其它被堵塞在锁竞争上的线程得以继续运行。另外，一旦当前线程获得通知(notified，一般是另外某个线程调用 notify_* 唤醒了当前线程)，wait()函数也是自己主动调用 lck.lock()。

在另外一种情况下（即设置了 Predicate）。仅仅有当 pred 条件为false 时调用 wait() 才会堵塞当前线程。而且在收到其它线程的通知后仅仅有当 pred 为 true 时才会被解除堵塞。

案例如下：
```c
#include <iostream>                // std::cout
#include <thread>                // std::thread, std::this_thread::yield
#include <mutex>                // std::mutex, std::unique_lock
#include <condition_variable>    // std::condition_variable

std::mutex mtx;
std::condition_variable cv;

int cargo = 0;
bool shipment_available()
{
	return cargo != 0;
}

// 消费者线程.
void consume(int n)
{
	for (int i = 0; i < n; ++i) {
		std::unique_lock <std::mutex> lck(mtx);
		cv.wait(lck, shipment_available);
		std::cout << cargo << '\n';
		cargo = 0;
	}
}

int main()
{
	std::thread consumer_thread(consume, 10); // 消费者线程.

	// 主线程为生产者线程, 生产 10 个物品.
	for (int i = 0; i < 10; ++i) {
		while (shipment_available())
			std::this_thread::yield();
		std::unique_lock <std::mutex> lck(mtx);
		cargo = i + 1;
		cv.notify_one();
	}

	consumer_thread.join();

	return 0;
}
```
运行结果如下：
```
1
2
3
4
5
6
7
8
9
10
```
#### 3、std::condition_variable::wait_for

函数原型如下：
```c
template <class Rep, class Period>
cv_status wait_for (unique_lock<mutex>& lck, const chrono::duration<Rep,Period>& rel_time);

template <class Rep, class Period, class Predicate>
bool wait_for (unique_lock<mutex>& lck, const chrono::duration<Rep,Period>& rel_time, Predicate pred);
```

与std::condition_variable::wait() 相似，只是 wait_for能够指定一个时间段，在当前线程收到通知或者指定的时间 rel_time 超时之前。该线程都会处于堵塞状态。而一旦超时或者收到了其它线程的通知，wait_for返回，剩下的处理步骤和 wait()相似。

请看以下的样例（參考），以下的样例中，主线程等待th线程输入一个值。然后将th线程从终端接收的值打印出来。在th线程接受到值之前，主线程一直等待。每一个一秒超时一次，并打印一个 "."：

```cpp
#include <iostream>           // std::cout
#include <thread>             // std::thread
#include <chrono>             // std::chrono::seconds
#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable, std::cv_status

std::condition_variable cv;

int value;

void do_read_value()
{
	std::cin >> value;
	cv.notify_one();
}

int main()
{
	std::cout << "Please, enter an integer (I'll be printing dots): \n";
	std::thread th(do_read_value);

	std::mutex mtx;
	std::unique_lock<std::mutex> lck(mtx);
	while (cv.wait_for(lck, std::chrono::seconds(1)) == std::cv_status::timeout) {
		std::cout << '.';
		std::cout.flush();
	}

	std::cout << "You entered: " << value << '\n';

	th.join();
	return 0;
}
```
运行结果如下：
```
Please, enter an integer (I'll be printing dots):
...6.
You entered: 6
```
#### 4、std::condition_variable::notify_one

唤醒某个等待(wait)线程。假设当前没有等待线程，则该函数什么也不做，假设同一时候存在多个等待线程，则唤醒某个线程是不确定的(unspecified)：

```C
#include <iostream>                // std::cout
#include <thread>                // std::thread
#include <mutex>                // std::mutex, std::unique_lock
#include <condition_variable>    // std::condition_variable

std::mutex mtx;
std::condition_variable cv;

int cargo = 0; // shared value by producers and consumers

void consumer()
{
	std::unique_lock < std::mutex > lck(mtx);
	while (cargo == 0)
		cv.wait(lck);
	std::cout << cargo << '\n';
	cargo = 0;
}

void producer(int id)
{
	std::unique_lock < std::mutex > lck(mtx);
	cargo = id;
	cv.notify_one();
}

int main()
{
	std::thread consumers[10], producers[10];

	// spawn 10 consumers and 10 producers:
	for (int i = 0; i < 10; ++i) {
		consumers[i] = std::thread(consumer);
		producers[i] = std::thread(producer, i + 1);
	}

	// join them back:
	for (int i = 0; i < 10; ++i) {
		producers[i].join();
		consumers[i].join();
	}

	return 0;
}
```
#### 5、std::condition_variable::notify_all

唤醒全部的等待(wait)线程。假设当前没有等待线程，则该函数什么也不做。请看以下的样例：

```c
#include <iostream>                // std::cout
#include <thread>                // std::thread
#include <mutex>                // std::mutex, std::unique_lock
#include <condition_variable>    // std::condition_variable

std::mutex mtx; // 全局相互排斥锁.
std::condition_variable cv; // 全局条件变量.
bool ready = false; // 全局标志位.

void do_print_id(int id)
{
	std::unique_lock <std::mutex> lck(mtx);
	while (!ready) // 假设标志位不为 true, 则等待...
		cv.wait(lck); // 当前线程被堵塞, 当全局标志位变为 true 之后,
	// 线程被唤醒, 继续往下运行打印线程编号id.
	std::cout << "thread " << id << '\n';
}

void go()
{
	std::unique_lock <std::mutex> lck(mtx);
	ready = true; // 设置全局标志位为 true.
	cv.notify_all(); // 唤醒全部线程.
}

int main()
{
	std::thread threads[10];
	// spawn 10 threads:
	for (int i = 0; i < 10; ++i)
		threads[i] = std::thread(do_print_id, i);

	std::cout << "10 threads ready to race...\n";
	go(); // go!

	for (auto & th : threads)
		th.join();

	return 0;
}
```