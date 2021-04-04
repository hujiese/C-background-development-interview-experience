<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->


- [std::future 详解](#stdfuture-%E8%AF%A6%E8%A7%A3)
  - [一、std::promise](#%E4%B8%80stdpromise)
    - [1、std::promise 构造函数](#1stdpromise-%E6%9E%84%E9%80%A0%E5%87%BD%E6%95%B0)
    - [2、std::promise::get_future](#2stdpromiseget_future)
    - [3、std::promise::set_value](#3stdpromiseset_value)
    - [4、std::promise::set_exception](#4stdpromiseset_exception)
    - [5、std::promise::set_value_at_thread_exit](#5stdpromiseset_value_at_thread_exit)
    - [6、std::promise::swap](#6stdpromiseswap)
  - [二、std::packaged_task](#%E4%BA%8Cstdpackaged_task)
    - [1、std::packaged_task 构造函数](#1stdpackaged_task-%E6%9E%84%E9%80%A0%E5%87%BD%E6%95%B0)
    - [2、std::packaged_task::valid](#2stdpackaged_taskvalid)
    - [3、std::packaged_task::get_future](#3stdpackaged_taskget_future)
    - [4、std::packaged_task::operator()(Args... args)](#4stdpackaged_taskoperatorargs-args)
    - [5、std::packaged_task::make_ready_at_thread_exit](#5stdpackaged_taskmake_ready_at_thread_exit)
    - [6、std::packaged_task::reset](#6stdpackaged_taskreset)
    - [7、std::packaged_task::swap](#7stdpackaged_taskswap)
  - [三、std::future](#%E4%B8%89stdfuture)
    - [1、std::future 构造函数](#1stdfuture-%E6%9E%84%E9%80%A0%E5%87%BD%E6%95%B0)
    - [2、std::future::share](#2stdfutureshare)
    - [3、std::future::get](#3stdfutureget)
    - [4、std::future::valid](#4stdfuturevalid)
    - [5、std::future::wait](#5stdfuturewait)
    - [6、std::future::wait_for](#6stdfuturewait_for)
    - [7、std::async](#7stdasync)
    - [8、std::future 相关的枚举类](#8stdfuture-%E7%9B%B8%E5%85%B3%E7%9A%84%E6%9E%9A%E4%B8%BE%E7%B1%BB)
      - [（1）std::future_errc](#1stdfuture_errc)
      - [（2）std::future_status](#2stdfuture_status)
      - [（3）std::launch](#3stdlaunch)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## std::future 详解

本文参考 [C++11 并发指南四(<future> 详解一 std::promise 介绍)](https://www.cnblogs.com/haippy/p/3239248.html)

<future> 头文件中包含了以下几个类和函数：

* Providers 类：std::promise, std::package_task
* Futures 类：std::future, shared_future.
* Providers 函数：std::async()
* 其他类型：std::future_error, std::future_errc, std::future_status, std::launch.

这一头文件是异步编程sync API的调用基础。

### 一、std::promise

promise 对象可以保存某一类型 T 的值，该值可被 future 对象读取（可能在另外一个线程中），因此 promise 也提供了一种线程同步的手段。在 promise 对象构造时可以和一个共享状态（通常是std::future）相关联，并可以在相关联的共享状态(std::future)上保存一个类型为 T 的值。

可以通过 get_future 来获取与该 promise 对象相关联的 future 对象，调用该函数之后，两个对象共享相同的共享状态(shared state)

* promise 对象是异步 Provider，它可以在某一时刻设置共享状态的值。
* future 对象可以异步返回共享状态的值，或者在必要的情况下阻塞调用者并等待共享状态标志变为 ready，然后才能获取共享状态的值。

下面以一个简单的例子来说明上述关系：
```c
#include <iostream>       // std::cout
#include <functional>     // std::ref
#include <thread>         // std::thread
#include <future>         // std::promise, std::future
//#include <Windows.h>
void print_int(std::future<int>& fut) {
	int x = fut.get(); // 获取共享状态的值.
	std::cout << "value: " << x << '\n'; // 打印 value: 10.
}

int main()
{
	std::promise<int> prom; // 生成一个 std::promise<int> 对象.
	std::future<int> fut = prom.get_future(); // 和 future 关联.
	std::thread t(print_int, std::ref(fut)); // 将 future 交给另外一个线程t.
	/*Sleep(1000);*/
	prom.set_value(10); // 设置共享状态的值, 此处和线程t保持同步.
	t.join();
	return 0;
}
```
该例子传递了一个std::future变量fut给另一个线程，注意，我前面的代码特地注释掉了Sleep(1000)，如果去掉该注释，线程t需要等待1s时间，主线程中将future的值设置为10后才会输出结果。这便是异步调用的思想。

#### 1、std::promise 构造函数

类型|函数格式|说明
---|---|---
default (1)	|promise();|默认构造函数，初始化一个空的共享状态
with allocator (2)	|template <class Alloc> promise (allocator_arg_t aa, const Alloc& alloc);|带自定义内存分配器的构造函数，与默认构造函数类似，但是使用自定义分配器来分配共享状态
copy [deleted] (3)	|promise (const promise&) = delete;|拷贝构造函数，被禁用
move (4)	|promise (promise&& x) noexcept;|移动构造函数

另外，std::promise 的 operator= **没有拷贝语义**，即 std::promise 普通的赋值操作被禁用，operator= **只有 move 语义**，所以 std::promise 对象是禁止拷贝的。

案例:
```c
#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <future>         // std::promise, std::future

std::promise<int> prom;

void print_global_promise() {
	std::future<int> fut = prom.get_future();
	int x = fut.get();
	std::cout << "value: " << x << '\n';
}

int main()
{
	std::thread th1(print_global_promise);
	prom.set_value(10);
	th1.join();

	prom = std::promise<int>();    // prom 被move赋值为一个新的 promise 对象.

	std::thread th2(print_global_promise);
	prom.set_value(20);
	th2.join();

	return 0;
}
```
#### 2、std::promise::get_future

该函数返回一个与 promise 共享状态相关联的 future 。返回的 future 对象可以访问由 promise 对象设置在共享状态上的值或者某个异常对象。只能从 promise 共享状态获取一个 future 对象。在调用该函数之后，promise 对象通常会在某个时间点准备好(设置一个值或者一个异常对象)，如果不设置值或者异常，promise 对象在析构时会自动地设置一个 future_error 异常(broken_promise)来设置其自身的准备状态。上面的例子中已经提到了 get_future，此处不再重复。

#### 3、std::promise::set_value


* generic template (1)
    ```c
    void set_value (const T& val); 
    void set_value (T&& val);
    ```
* specializations (2)
    ```c
    void promise<R&>::set_value (R& val);   // when T is a reference type (R&)  
    void promise<void>::set_value (void);   // when T is void
    ```

设置共享状态的值，此后 promise 的共享状态标志变为 ready。

#### 4、std::promise::set_exception

下面这个案例中，线程1从终端接收一个整数，线程2将该整数打印出来，如果线程1接收一个非整数，则为promise设置一个异常(failbit) ，线程2通过std::future::get 中抛出该异常：
```c
#include <iostream>       // std::cin, std::cout, std::ios
#include <functional>     // std::ref
#include <thread>         // std::thread
#include <future>         // std::promise, std::future
#include <exception>      // std::exception, std::current_exception

void get_int(std::promise<int>& prom) {
	int x;
	std::cout << "Please, enter an integer value: ";
	std::cin.exceptions(std::ios::failbit);   // throw on failbit
	try {
		std::cin >> x;                         // sets failbit if input is not int
		prom.set_value(x);
	}
	catch (std::exception&) {
		prom.set_exception(std::current_exception());
	}
}

void print_int(std::future<int>& fut) {
	try {
		int x = fut.get();
		std::cout << "value: " << x << '\n';
	}
	catch (std::exception& e) {
		std::cout << "[exception caught: " << e.what() << "]\n";
	}
}

int main()
{
	std::promise<int> prom;
	std::future<int> fut = prom.get_future();

	std::thread th1(get_int, std::ref(prom));
	std::thread th2(print_int, std::ref(fut));

	th1.join();
	th2.join();
	return 0;
}
```
如果正常输入整数，结果如下：
```
Please, enter an integer value: 6
value: 6
```
如果输入的是字符或者其他东西，那么：
```
Please, enter an integer value: a
[exception caught: ios_base::failbit set: iostream stream error]
```

#### 5、std::promise::set_value_at_thread_exit

设置共享状态的值，但是不将共享状态的标志设置为 ready，当线程退出时该 promise 对象会自动设置为 ready。如果某个 std::future 对象与该 promise 对象的共享状态相关联，并且该 future 正在调用 get，则调用get的线程会被阻塞，当线程退出时，调用future::get 的线程解除阻塞，同时 get 返回 set_value_at_thread_exit 所设置的值。注意，该函数已经设置了 promise 共享状态的值，如果在线程结束之前有其他设置或者修改共享状态的值的操作，则会抛出 future_error( promise_already_satisfied )。

#### 6、std::promise::swap

交换 promise 的共享状态。

### 二、std::packaged_task

std::packaged_task 包装一个可调用的对象，并且允许异步获取该可调用对象产生的结果，从包装可调用对象意义上来讲，std::packaged_task 与 std::function 类似，只不过 std::packaged_task 将其包装的可调用对象的执行结果传递给一个 std::future 对象（该对象通常在另外一个线程中获取 std::packaged_task 任务的执行结果）。

std::packaged_task 对象内部包含了两个最基本元素：
* 一、被包装的任务(stored task)，任务(task)是一个可调用的对象，如函数指针、成员函数指针或者函数对象
* 二、共享状态(shared state)，用于保存任务的返回值，可以通过 std::future 对象来达到异步访问共享状态的效果

可以通过 std::packged_task::get_future 来获取与共享状态相关联的 std::future 对象。在调用该函数之后，两个对象共享相同的共享状态，具体解释如下：

* std::packaged_task 对象是异步 Provider，它在某一时刻通过调用被包装的任务来设置共享状态的值。
* std::future 对象是一个异步返回对象，通过它可以获得共享状态的值，当然在必要的时候需要等待共享状态标志变为 ready。
* std::packaged_task 的共享状态的生命周期一直持续到最后一个与之相关联的对象被释放或者销毁为止。

下面一个小例子大致讲了 std::packaged_task 的用法：
```c
#include <iostream>     // std::cout
#include <future>       // std::packaged_task, std::future
#include <chrono>       // std::chrono::seconds
#include <thread>       // std::thread, std::this_thread::sleep_for

// count down taking a second for each value:
int countdown(int from, int to) {
	for (int i = from; i != to; --i) {
		std::cout << i << '\n';
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	std::cout << "Finished!\n";
	return from - to;
}

int main()
{
	std::packaged_task<int(int, int)> task(countdown); // 设置 packaged_task
	std::future<int> ret = task.get_future(); // 获得与 packaged_task 共享状态相关联的 future 对象.

	std::thread th(std::move(task), 10, 0);   //创建一个新线程完成计数任务.

	int value = ret.get();                    // 等待任务完成并获取结果.

	std::cout << "The countdown lasted for " << value << " seconds.\n";

	th.join();
	return 0;
}
```
输出结果如下：
```
10
9
8
7
6
5
4
3
2
1
Finished!
The countdown lasted for 10 seconds.
```
程序会从10开始每过1s递减打印数字。

#### 1、std::packaged_task 构造函数

类型|函数格式|说明
---|---|---
default (1)	|packaged_task() noexcept;|默认构造函数，初始化一个空的共享状态，并且该 packaged_task 对象无包装任务
initialization (2)	|template <class Fn> explicit packaged_task (Fn&& fn);|初始化一个共享状态，并且被包装任务由参数 fn 指定
with allocator (3)	|template <class Fn, class Alloc> explicit packaged_task (allocator_arg_t aa, const Alloc& alloc, Fn&& fn);|带自定义内存分配器的构造函数，与默认构造函数类似，但是使用自定义分配器来分配共享状态
copy [deleted] (4)	|packaged_task (const packaged_task&) = delete;|拷贝构造函数，被禁用
move (5)	|packaged_task (packaged_task&& x) noexcept;|移动构造函数

下面这个例子介绍了各类构造函数的用法：
```c
#include <iostream>     // std::cout
#include <utility>      // std::move
#include <future>       // std::packaged_task, std::future
#include <thread>       // std::thread

int main()
{
	std::packaged_task<int(int)> foo; // 默认构造函数.

	// 使用 lambda 表达式初始化一个 packaged_task 对象.
	std::packaged_task<int(int)> bar([](int x){return x * 2; });

	foo = std::move(bar); // move-赋值操作，也是 C++11 中的新特性.

	// 获取与 packaged_task 共享状态相关联的 future 对象.
	std::future<int> ret = foo.get_future();

	std::thread(std::move(foo), 10).detach(); // 产生线程，调用被包装的任务.

	int value = ret.get(); // 等待任务完成并获取结果.
	std::cout << "The double of 10 is " << value << ".\n";

	return 0;
}
```
与 std::promise 类似， std::packaged_task 也禁用了普通的赋值操作运算，只允许 move 赋值运算。

#### 2、std::packaged_task::valid

检查当前 packaged_task 是否和一个有效的共享状态相关联，对于由默认构造函数生成的 packaged_task 对象，该函数返回 false，除非中间进行了 move 赋值操作或者 swap 操作。

使用案例如下：
```c
#include <iostream>     // std::cout
#include <utility>      // std::move
#include <future>       // std::packaged_task, std::future
#include <thread>       // std::thread

// 在新线程中启动一个 int(int) packaged_task.
std::future<int> launcher(std::packaged_task<int(int)>& tsk, int arg)
{
	if (tsk.valid()) {
		std::future<int> ret = tsk.get_future();
		std::thread(std::move(tsk), arg).detach();
		return ret;
	}
	else return std::future<int>();
}

int main()
{
	std::packaged_task<int(int)> tsk([](int x){return x * 2; });

	std::future<int> fut = launcher(tsk, 25);

	std::cout << "The double of 25 is " << fut.get() << ".\n";

	return 0;
}
```

#### 3、std::packaged_task::get_future

返回一个与 packaged_task 对象共享状态相关的 future 对象。返回的 future 对象可以获得由另外一个线程在该 packaged_task 对象的共享状态上设置的某个值或者异常。

前面有很多使用案例可供参考。

#### 4、std::packaged_task::operator()(Args... args)

调用该 packaged_task 对象所包装的对象(通常为函数指针，函数对象，lambda 表达式等)，传入的参数为 args. 调用该函数一般会发生两种情况：

* 如果成功调用 packaged_task 所包装的对象，则返回值（如果被包装的对象有返回值的话）被保存在 packaged_task 的共享状态中。
* 如果调用 packaged_task 所包装的对象失败，并且抛出了异常，则异常也会被保存在 packaged_task 的共享状态中。
以上两种情况都使共享状态的标志变为 ready，因此其他等待该共享状态的线程可以获取共享状态的值或者异常并继续执行下去。

共享状态的值可以通过在 future 对象(由 get_future获得)上调用 get 来获得。

由于被包装的任务在 packaged_task 构造时指定，因此调用 operator() 的效果由 packaged_task 对象构造时所指定的可调用对象来决定：

* 如果被包装的任务是函数指针或者函数对象，调用 std::packaged_task::operator() 只是将参数传递给被包装的对象。
* 如果被包装的任务是指向类的非静态成员函数的指针，那么 std::packaged_task::operator() 的第一个参数应该指定为成员函数被调用的那个对象，剩余的参数作为该成员函数的参数。
* 如果被包装的任务是指向类的非静态成员变量，那么 std::packaged_task::operator() 只允许单个参数。

#### 5、std::packaged_task::make_ready_at_thread_exit

该函数会调用被包装的任务，并向任务传递参数，类似 std::packaged_task 的 operator() 成员函数。但是与 operator() 函数不同的是，make_ready_at_thread_exit 并不会立即设置共享状态的标志为 ready，而是在线程退出时设置共享状态的标志。

如果与该 packaged_task 共享状态相关联的 future 对象在 future::get 处等待，则当前的 future::get 调用会被阻塞，直到线程退出。而一旦线程退出，future::get 调用继续执行，或者抛出异常。

注意，该函数已经设置了 promise 共享状态的值，如果在线程结束之前有其他设置或者修改共享状态的值的操作，则会抛出 future_error( promise_already_satisfied )。

#### 6、std::packaged_task::reset

重置 packaged_task 的共享状态，但是保留之前的被包装的任务。请看例子，该例子中，packaged_task 被重用了多次：

```c
#include <iostream>     // std::cout
#include <utility>      // std::move
#include <future>       // std::packaged_task, std::future
#include <thread>       // std::thread

// a simple task:
int triple(int x) { return x * 3; }

int main()
{
	std::packaged_task<int(int)> tsk(triple); // package task


	std::future<int> fut = tsk.get_future();
	std::thread(std::move(tsk), 100).detach();
	std::cout << "The triple of 100 is " << fut.get() << ".\n";


	// re-use same task object:
	tsk.reset();
	fut = tsk.get_future();
	std::thread(std::move(tsk), 200).detach();
	std::cout << "Thre triple of 200 is " << fut.get() << ".\n";

	return 0;
}
```

#### 7、std::packaged_task::swap

交换 packaged_task 的共享状态。

### 三、std::future

前面已经多次提到过 std::future，那么 std::future 究竟是什么呢？简单地说，std::future 可以用来获取异步任务的结果，因此可以把它当成一种简单的线程间同步的手段。std::future 通常由某个 Provider 创建，你可以把 Provider 想象成一个异步任务的提供者，Provider 在某个线程中设置共享状态的值，与该共享状态相关联的 std::future 对象调用 get（通常在另外一个线程中） 获取该值，如果共享状态的标志不为 ready，则调用 std::future::get 会阻塞当前的调用者，直到 Provider 设置了共享状态的值（此时共享状态的标志变为 ready），std::future::get 返回异步任务的值或异常（如果发生了异常）。

一个有效(valid)的 std::future 对象通常由以下三种 Provider 创建，并和某个共享状态相关联。Provider 可以是函数或者类，其实我们前面都已经提到了，他们分别是：

* std::async 函数，本文后面会介绍 std::async() 函数
* std::promise::get_future，get_future 为 promise 类的成员函数
* std::packaged_task::get_future，此时 get_future为 packaged_task 的成员函数

一个 std::future 对象只有在有效(valid)的情况下才有用(useful)，由 std::future 默认构造函数创建的 future 对象不是有效的（除非当前非有效的 future 对象被 move 赋值另一个有效的 future 对象）。

 在一个有效的 future 对象上调用 get 会阻塞当前的调用者，直到 Provider 设置了共享状态的值或异常（此时共享状态的标志变为 ready），std::future::get 将返回异步任务的值或异常（如果发生了异常）。

 ```c
 // future example
#include <iostream>             // std::cout
#include <future>               // std::async, std::future
#include <chrono>               // std::chrono::milliseconds

// a non-optimized way of checking for prime numbers:
bool is_prime(int x)
{
	for (int i = 2; i < x; ++i)
	if (x % i == 0)
		return false;
	return true;
}

int main()
{
	// call function asynchronously:
	std::future < bool > fut = std::async(is_prime, 444444443);

	// do something while waiting for function to set future:
	std::cout << "checking, please wait";
	std::chrono::milliseconds span(100);
	while (fut.wait_for(span) == std::future_status::timeout)
		std::cout << '.';

	bool x = fut.get();         // retrieve return value

	std::cout << "\n444444443 " << (x ? "is" : "is not") << " prime.\n";

	return 0;
}
 ```
运行结果如下：
```
checking, please wait............
444444443 is prime.
```
#### 1、std::future 构造函数

std::future 一般由 std::async, std::promise::get_future, std::packaged_task::get_future 创建，不过也提供了构造函数，如下表所示：

类型|函数
---|---
default (1)	|future() noexcept;
copy [deleted] (2)	|future (const future&) = delete;
move (3)	|future (future&& x) noexcept;

不过 std::future 的拷贝构造函数是被禁用的，只提供了默认的构造函数和 move 构造函数（注：C++ 新特新）。另外，std::future 的普通赋值操作也被禁用，只提供了 move 赋值操作。如下代码所示：
```c
std::future<int> fut;           // 默认构造函数
fut = std::async(do_some_task);   // move-赋值操作。
```

#### 2、std::future::share

返回一个 std::shared_future 对象（本文后续内容将介绍 std::shared_future ），调用该函数之后，该 std::future 对象本身已经不和任何共享状态相关联，因此该 std::future 的状态不再是 valid 的了：
```c
#include <iostream>       // std::cout
#include <future>         // std::async, std::future, std::shared_future

int do_get_value() { return 10; }

int main()
{
	std::future<int> fut = std::async(do_get_value);
	std::shared_future<int> shared_fut = fut.share();

	// 共享的 future 对象可以被多次访问.
	std::cout << "value: " << shared_fut.get() << '\n';
	std::cout << "its double: " << shared_fut.get() * 2 << '\n';

	return 0;
}
```
运行结果如下：
```
value: 10
its double: 20
```
#### 3、std::future::get

std::future::get 一共有三种形式，如下表所示：

函数|类型
---|---
generic template (1)	|T get();
reference specialization (2)	|R& future<R&>::get();  // when T is a reference type (R&)
void specialization (3)	|void future<void>::get();   // when T is void

当与该 std::future 对象相关联的共享状态标志变为 ready 后，调用该函数将返回保存在共享状态中的值，如果共享状态的标志不为 ready，则调用该函数会阻塞当前的调用者，而此后一旦共享状态的标志变为 ready，get 返回 Provider 所设置的共享状态的值或者异常（如果抛出了异常）:

```c
#include <iostream>       // std::cin, std::cout, std::ios
#include <functional>     // std::ref
#include <thread>         // std::thread
#include <future>         // std::promise, std::future
#include <exception>      // std::exception, std::current_exception

void get_int(std::promise<int>& prom) {
	int x;
	std::cout << "Please, enter an integer value: ";
	std::cin.exceptions(std::ios::failbit);   // throw on failbit
	try {
		std::cin >> x;                         // sets failbit if input is not int
		prom.set_value(x);
	}
	catch (std::exception&) {
		prom.set_exception(std::current_exception());
	}
}

void print_int(std::future<int>& fut) {
	try {
		int x = fut.get();
		std::cout << "value: " << x << '\n';
	}
	catch (std::exception& e) {
		std::cout << "[exception caught: " << e.what() << "]\n";
	}
}

int main()
{
	std::promise<int> prom;
	std::future<int> fut = prom.get_future();

	std::thread th1(get_int, std::ref(prom));
	std::thread th2(print_int, std::ref(fut));

	th1.join();
	th2.join();
	return 0;
}
```

#### 4、std::future::valid

检查当前的 std::future 对象是否有效，即释放与某个共享状态相关联。一个有效的 std::future 对象只能通过 std::async(), std::future::get_future 或者 std::packaged_task::get_future 来初始化。另外由 std::future 默认构造函数创建的 std::future 对象是无效(invalid)的，当然通过 std::future 的 move 赋值后该 std::future 对象也可以变为 valid。

```c
#include <iostream>       // std::cout
#include <future>         // std::async, std::future
#include <utility>        // std::move

int do_get_value() { return 11; }

int main()
{
	// 由默认构造函数创建的 std::future 对象,
	// 初始化时该 std::future 对象处于为 invalid 状态.
	std::future<int> foo, bar;
	foo = std::async(do_get_value); // move 赋值, foo 变为 valid.
	bar = std::move(foo); // move 赋值, bar 变为 valid, 而 move 赋值以后 foo 变为 invalid.

	if (foo.valid())
		std::cout << "foo's value: " << foo.get() << '\n';
	else
		std::cout << "foo is not valid\n";

	if (bar.valid())
		std::cout << "bar's value: " << bar.get() << '\n';
	else
		std::cout << "bar is not valid\n";

	return 0;
}
```
运行结果如下：
```
foo is not valid
bar's value: 11
```

#### 5、std::future::wait

等待与当前std::future 对象相关联的共享状态的标志变为 ready.

如果共享状态的标志不是 ready（此时 Provider 没有在共享状态上设置值（或者异常）），调用该函数会被阻塞当前线程，直到共享状态的标志变为 ready。
一旦共享状态的标志变为 ready，wait() 函数返回，当前线程被解除阻塞，但是 wait() 并不读取共享状态的值或者异常。下面的代码说明了 std::future::wait() 的用法：
```c
#include <iostream>                // std::cout
#include <future>                // std::async, std::future
#include <chrono>                // std::chrono::milliseconds

// a non-optimized way of checking for prime numbers:
bool do_check_prime(int x) // 为了体现效果, 该函数故意没有优化.
{
	for (int i = 2; i < x; ++i)
	if (x % i == 0)
		return false;
	return true;
}

int main()
{
	// call function asynchronously:
	std::future < bool > fut = std::async(do_check_prime, 194232491);

	std::cout << "Checking...\n";
	fut.wait();

	std::cout << "\n194232491 ";
	if (fut.get()) // guaranteed to be ready (and not block) after wait returns
		std::cout << "is prime.\n";
	else
		std::cout << "is not prime.\n";

	return 0;
}
```
运行结果如下：
```
Checking...

194232491 is prime.
```

#### 6、std::future::wait_for

与 std::future::wait() 的功能类似，即等待与该 std::future 对象相关联的共享状态的标志变为 ready，该函数原型如下：

```c
template <class Rep, class Period>
  future_status wait_for (const chrono::duration<Rep,Period>& rel_time) const;
```

而与 std::future::wait() 不同的是，wait_for() 可以设置一个时间段 rel_time，如果共享状态的标志在该时间段结束之前没有被 Provider 设置为 ready，则调用 wait_for 的线程被阻塞，在等待了 rel_time 的时间长度后 wait_until() 返回，返回值如下：

返回值|描述
---|---
future_status::ready	|共享状态的标志已经变为 ready，即 Provider 在共享状态上设置了值或者异常。
future_status::timeout	|超时，即在规定的时间内共享状态的标志没有变为 ready。
future_status::deferred	|共享状态包含一个 deferred 函数。

案例如下：
```c
#include <iostream>                // std::cout
#include <future>                // std::async, std::future
#include <chrono>                // std::chrono::milliseconds

// a non-optimized way of checking for prime numbers:
bool do_check_prime(int x) // 为了体现效果, 该函数故意没有优化.
{
    for (int i = 2; i < x; ++i)
        if (x % i == 0)
            return false;
    return true;
}

int main()
{
    // call function asynchronously:
    std::future < bool > fut = std::async(do_check_prime, 194232491);

    std::cout << "Checking...\n";
    std::chrono::milliseconds span(500); // 设置超时间隔.

    // 如果超时，则输出"."，继续等待
    while (fut.wait_for(span) == std::future_status::timeout)
        std::cout << '.';

    std::cout << "\n194232491 ";
    if (fut.get()) // guaranteed to be ready (and not block) after wait returns
        std::cout << "is prime.\n";
    else
        std::cout << "is not prime.\n";

    return 0;
}
```
运行结果如下：
```
Checking...
.
194232491 is prime.
```

#### 7、std::async

与 std::future 相关的函数主要是 std::async()，原型如下：

* unspecified policy (1)	
```c
template <class Fn, class... Args>
future<typename result_of<Fn(Args...)>::type> async(Fn&& fn, Args&&... args);
```
* specific policy (2)
```c
template <class Fn, class... Args>
future<typename result_of<Fn(Args...)>::type> async(launch policy, Fn&& fn, Args&&... args);
```

上面两组 std::async() 的不同之处是第一类 std::async 没有指定异步任务（即执行某一函数）的启动策略(launch policy)，而第二类函数指定了启动策略，详见 std::launch 枚举类型，指定启动策略的函数的 policy 参数可以是launch::async，launch::deferred，以及两者的按位或( | )。

std::async() 的 fn 和 args 参数用来指定异步任务及其参数。另外，std::async() 返回一个 std::future 对象，通过该对象可以获取异步任务的值或异常（如果异步任务抛出了异常）。

下面介绍一下 std::async 的用法：
```c
#include <stdio.h>
#include <stdlib.h>

#include <cmath>
#include <chrono>
#include <future>
#include <iostream>

double ThreadTask(int n) {
	std::cout << std::this_thread::get_id()
		<< " start computing..." << std::endl;

	double ret = 0;
	for (int i = 0; i <= n; i++) {
		ret += std::sin(i);
	}

	std::cout << std::this_thread::get_id()
		<< " finished computing..." << std::endl;
	return ret;
}

int main(int argc, const char *argv[])
{
	std::future<double> f(std::async(std::launch::async, ThreadTask, 100000000));

#if 0
	while (f.wait_until(std::chrono::system_clock::now() + std::chrono::seconds(1))
		!= std::future_status::ready) {
		std::cout << "task is running...\n";
	}
#else
	while (f.wait_for(std::chrono::seconds(1))
		!= std::future_status::ready) {
		std::cout << "task is running...\n";
	}
#endif

	std::cout << f.get() << std::endl;

	return EXIT_SUCCESS;
}
```
运行结果如下：
```
8980 start computing...
task is running...
task is running...
task is running...
task is running...
task is running...
task is running...
8980 finished computing...
1.71365
```
#### 8、std::future 相关的枚举类

面介绍与 std::future 相关的枚举类型。与 std::future 相关的枚举类型包括：
```c
enum class future_errc;
enum class future_status;
enum class launch;
```

##### （1）std::future_errc

std::future_errc 类型描述如下：

类型|取值|描述
---|---|---
broken_promise	|0	|与该 std::future 共享状态相关联的 std::promise 对象在设置值或者异常之前一被销毁。
future_already_retrieved	|1	|与该 std::future 对象相关联的共享状态的值已经被当前 Provider 获取了，即调用了 std::future::get 函数。
promise_already_satisfied	|2	|std::promise 对象已经对共享状态设置了某一值或者异常。
no_state	|3	|无共享状态。

##### （2）std::future_status

std::future_status 类型主要用在 std::future(或std::shared_future)中的 wait_for 和 wait_until 两个函数中：

类型|取值|描述
---|---|---
future_status::ready	|0|	wait_for(或wait_until) 因为共享状态的标志变为 ready 而返回。
future_status::timeout	|1|	超时，即 wait_for(或wait_until) 因为在指定的时间段（或时刻）内共享状态的标志依然没有变为 ready 而返回。
future_status::deferred	|2|	共享状态包含了 deferred 函数。

##### （3）std::launch

该枚举类型主要是在调用 std::async 设置异步任务的启动策略的：

类型|描述
---|---
launch::async	|Asynchronous: 异步任务会在另外一个线程中调用，并通过共享状态返回异步任务的结果（一般是调用 std::future::get() 获取异步任务的结果）。
launch::deferred	|Deferred: 异步任务将会在共享状态被访问时调用，相当与按需调用（即延迟(deferred)调用）。

使用案例如下：
```c
#include <iostream>                // std::cout
#include <future>                // std::async, std::future, std::launch
#include <chrono>                // std::chrono::milliseconds
#include <thread>                // std::this_thread::sleep_for

void do_print_ten(char c, int ms)
{
	for (int i = 0; i < 10; ++i) {
		std::this_thread::sleep_for(std::chrono::milliseconds(ms));
		std::cout << c;
	}
}

int main()
{
	std::cout << "with launch::async:\n";
	std::future < void >foo = std::async(std::launch::async, do_print_ten, '*', 100);
	std::future < void >bar = std::async(std::launch::async, do_print_ten, '@', 200);
	// async "get" (wait for foo and bar to be ready):
	foo.get();
	bar.get();
	std::cout << "\n\n";

	std::cout << "with launch::deferred:\n";
	foo = std::async(std::launch::deferred, do_print_ten, '*', 100);
	bar = std::async(std::launch::deferred, do_print_ten, '@', 200);
	// deferred "get" (perform the actual calls):
	foo.get();
	bar.get();
	std::cout << '\n';

	return 0;
}
```
运行结果如下：
```
with launch::async:
**@*@***@**@*@*@@@@@

with launch::deferred:
**********@@@@@@@@@@
```
