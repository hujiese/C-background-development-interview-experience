<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->


- [C++11 多线程编程](#c11-%E5%A4%9A%E7%BA%BF%E7%A8%8B%E7%BC%96%E7%A8%8B)
  - [一、创建线程](#%E4%B8%80%E5%88%9B%E5%BB%BA%E7%BA%BF%E7%A8%8B)
    - [1、线程分离](#1%E7%BA%BF%E7%A8%8B%E5%88%86%E7%A6%BB)
    - [2、线程函数是仿函数](#2%E7%BA%BF%E7%A8%8B%E5%87%BD%E6%95%B0%E6%98%AF%E4%BB%BF%E5%87%BD%E6%95%B0)
    - [3、线程函数是lambda表达式](#3%E7%BA%BF%E7%A8%8B%E5%87%BD%E6%95%B0%E6%98%AFlambda%E8%A1%A8%E8%BE%BE%E5%BC%8F)
  - [二、线程传递参数](#%E4%BA%8C%E7%BA%BF%E7%A8%8B%E4%BC%A0%E9%80%92%E5%8F%82%E6%95%B0)
    - [1、普通传参](#1%E6%99%AE%E9%80%9A%E4%BC%A0%E5%8F%82)
    - [2、传递引用](#2%E4%BC%A0%E9%80%92%E5%BC%95%E7%94%A8)
    - [3、使用move函数实现零拷贝传参](#3%E4%BD%BF%E7%94%A8move%E5%87%BD%E6%95%B0%E5%AE%9E%E7%8E%B0%E9%9B%B6%E6%8B%B7%E8%B4%9D%E4%BC%A0%E5%8F%82)
  - [三、线程拷贝](#%E4%B8%89%E7%BA%BF%E7%A8%8B%E6%8B%B7%E8%B4%9D)
  - [四、线程信息](#%E5%9B%9B%E7%BA%BF%E7%A8%8B%E4%BF%A1%E6%81%AF)
    - [1、线程ID](#1%E7%BA%BF%E7%A8%8Bid)
    - [2、查看CPU有多少线程](#2%E6%9F%A5%E7%9C%8Bcpu%E6%9C%89%E5%A4%9A%E5%B0%91%E7%BA%BF%E7%A8%8B)
  - [五、多线程与lambda表达式](#%E4%BA%94%E5%A4%9A%E7%BA%BF%E7%A8%8B%E4%B8%8Elambda%E8%A1%A8%E8%BE%BE%E5%BC%8F)
    - [1、创建五个lambda线程](#1%E5%88%9B%E5%BB%BA%E4%BA%94%E4%B8%AAlambda%E7%BA%BF%E7%A8%8B)
    - [2、打印循环创建的线程编号](#2%E6%89%93%E5%8D%B0%E5%BE%AA%E7%8E%AF%E5%88%9B%E5%BB%BA%E7%9A%84%E7%BA%BF%E7%A8%8B%E7%BC%96%E5%8F%B7)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## C++11 多线程编程

本文参考 [C++11/C++14 THREAD 1. CREATING THREADS](https://www.bogotobogo.com/cplusplus/C11/1_C11_creating_thread.php)

在看这一些列文章前最好有过Linux多线程编程经验，对于很多常识性的东西（例如join和线程分离，这里不会做过多介绍）。

### 一、创建线程

创建一个线程在C++11中是个简单的事，下面这个例子演示了如何创建线程：
```c
#include <iostream>
#include <thread>

void thread_function()
{
	std::cout << "thread function\n";
}

int main()
{
	std::thread t(&thread_function);   // t starts running
	//std::thread t(thread_function);
	std::cout << "main thread\n";
	t.join();   // main thread waits for the thread t to finish
	return 0;
}
```
运行结果如下：
```
thread function
main thread
```

#### 1、线程分离

线程分离代码如下：
```c
#include <iostream>
#include <thread>

void thread_function()
{
	std::cout << "thread function\n";
}

int main()
{
	std::thread t(&thread_function);
	std::cout << "main thread\n";
	// t.join();
	t.detach();
	return 0;
}
```
使用detach函数实现线程分离，分离后的现场会成为一个后台程序，和之前的主线程没有任何关系。需要注意的是，一个线程一旦被设置为“分离”，那么就不能在主线程中使用join等待该子线程退出。判断一个线程是否能join，可以使用函数joinable，下面是使用的案例：
```c
#include <iostream>
#include <thread>

void thread_function()
{
	std::cout << "thread function\n";
}

int main()
{
	std::thread t(&thread_function);
	std::cout << "main thread\n";
	t.detach();
	if (t.joinable())
	{
		std::cout << "joinable" << std::endl;
	}
	else
	{
		std::cout << "no join" << std::endl;
	}
	return 0;
}
```
运行结果如下：
```
thread function
main thread
no join
```

#### 2、线程函数是仿函数

前面的std::thread传入的参数是一个函数地址，当然也可以传入一个仿函数：
```c
#include <iostream>
#include <thread>

class MyFunctor
{
public:
	void operator()() {
		std::cout << "functor\n";
	}
};

int main()
{
	MyFunctor fnctor;
	std::thread t(fnctor);
	std::cout << "main thread\n";
	t.join();
	return 0;
}
```
有可能会将一个临时仿函数对象传给一个线程：
```c
std::thread t(MyFunctor());
```
但这样做编译会报错，应该这样写：
```c
std::thread t((MyFunctor()));
```

#### 3、线程函数是lambda表达式

```c
#include <iostream>
#include <thread>

int main()
{
	std::thread t([]()
	{
		std::cout << "thread function\n";
	}
	);
	std::cout << "main thread\n";
	t.join();     // main thread waits for t to finish
	return 0;
}
```

### 二、线程传递参数

#### 1、普通传参

如果向一个线程传递一个普通的变量，可以参考下面这种做法：

```c
#include <iostream>
#include <thread>
#include <string>

void thread_function(std::string s)
{
	std::cout << "thread function ";
	std::cout << "message is = " << s << std::endl;
}

int main()
{
	std::string s = "Kathy Perry";
	std::thread t(&thread_function, s);
	std::cout << "main thread message = " << s << std::endl;
	t.join();
	return 0;
}
```
#### 2、传递引用

有时候也需要向线程传递一个引用，很多人可能会采用如下方法来传递引用：
```c
void thread_function(std::string &s)
{
    std::cout << "thread function ";
    std::cout << "message is = " << s << std::endl;
    s = "Justin Beaver";
}

...
std::thread t(&thread_function, s);
```
但这种做法是错误的，它不能达到想要的结果--改变s的值并在main函数中打印这个修改过后的值。

如果想要真的传递引用，还得使用如下方法来创建线程：
```c
std::thread t(&thread;_function, std::ref(s));
```
完整代码如下所示：
```c
#include <iostream>
#include <thread>
#include <string>

void thread_function(std::string &s)
{
	std::cout << "thread function ";
	std::cout << "message is = " << s << std::endl;
	s = "Justin Beaver";
}

int main()
{
	std::string s = "Kathy Perry";
	/*std::thread t(&thread_function, s);*/
	std::thread t(&thread_function, std::ref(s));
	t.join();
	std::cout << "main thread message = " << s << std::endl;
	return 0;
}
```
输出结果如下：
```
thread function message is = Kathy Perry
main thread message = Justin Beaver
```

#### 3、使用move函数实现零拷贝传参

除前面说的一些传递参数方法之外，也可以通过move函数来传递参数，这种方式将实现参数传递的“零拷贝”：
```c
#include <iostream>
#include <thread>
#include <string>

void thread_function(std::string s)
{
	std::cout << "thread function ";
	std::cout << "message is = " << s << std::endl;
}

int main()
{
	std::string s = "Kathy Perry";
	std::thread t(&thread_function, std::move(s));
	t.join();
	std::cout << "main thread message = " << s << std::endl;
	return 0;
}
```
运行结果如下：
```
thread function message is = Kathy Perry
main thread message =
```

###  三、线程拷贝

使用=赋值方式拷贝线程是不允许的：
```c
#include <iostream>
#include <thread>

void thread_function()
{
	std::cout << "thread function\n";
}

int main()
{
	std::thread t(&thread_function);
	std::cout << "main thread\n";
	std::thread t2 = t;// 错误	1	error C2280: “std::thread::thread(const std::thread &)”: 尝试引用已删除的函数
	// std::thread t2 = move(t);
	t2.join();

	return 0;
}
```
如果非得拷贝或者转移一个线程，还是得使用move函数：
```c
std::thread t2 = move(t);
```
### 四、线程信息

#### 1、线程ID

可以通过下面方法来获取线程id：
```c
#include <iostream>
#include <thread>

void thread_function()
{
	std::cout << "thread function id is " << std::this_thread::get_id() << std::endl;;
}
int main()
{
	std::thread t(&thread_function);
	std::cout << "child thread id = " << t.get_id() << std::endl;

	t.join();
	std::cout << "main thread id = " << std::this_thread::get_id() << std::endl;
	return 0;
}
```
运行结果如下：
```
thread function id is child thread id = 3096
3096
main thread id = 8228
```

#### 2、查看CPU有多少线程

```c
#include <iostream>
#include <thread>

int main()
{
	std::cout << "A Number of threads = " << std::thread::hardware_concurrency() << std::endl;
	return 0;
}
```
运行结果如下：
```
A Number of threads = 8
```
我的CPU是四核八线程，所以打印结果是8。

### 五、多线程与lambda表达式

之前通过lambda创建过多线程程序，但只创建了一个线程而已，下面将深入lambda和多线程程序的结合。

#### 1、创建五个lambda线程

下面将循环创建五个线程，这几个线程会保存到一个vector数组中：
```c
#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>

int main()
{
	// vector container stores threads
	std::vector<std::thread> workers;
	for (int i = 0; i < 5; i++) {
		workers.push_back(std::thread([]()
		{
			std::cout << "thread function\n";
		}));
	}
	std::cout << "main thread\n";

	// Looping every thread via for_each
	// The 3rd argument assigns a task
	// It tells the compiler we're using lambda ([])
	// The lambda function takes its argument as a reference to a thread, t
	// Then, joins one by one, and this works like barrier
	std::for_each(workers.begin(), workers.end(), [](std::thread &t)
	{
		t.join();
	});

	return 0;
}
```
运行结果如下：
```
thread function
thread function
thread function
thread function
thread function
main thread
```
#### 2、打印循环创建的线程编号

上面的例子的确创建了五个线程，而且运行函数是lambda表达式，但是我们并不知道哪个线程会先运行，如果想要知道当前运行的是哪个线程，可以传递创建线程是i的值，以此标记线程：
```c
for (int i = 0; i < 5; i++) {
	workers.push_back(std::thread([i]()
	{
		std::cout << "thread function " << i << "\n";
	}));
}
```
运行结果如下：
```thread function thread function 1
thread function 4
thread function 2
0
main thread
thread function 3
```