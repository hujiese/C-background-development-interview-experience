<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->


- [线程池--C++11仿muduo](#%E7%BA%BF%E7%A8%8B%E6%B1%A0--c11%E4%BB%BFmuduo)
- [线程池--复杂版](#%E7%BA%BF%E7%A8%8B%E6%B1%A0--%E5%A4%8D%E6%9D%82%E7%89%88)
  - [1、std::result_of](#1stdresult_of)
  - [2、C++函数包装器](#2c%E5%87%BD%E6%95%B0%E5%8C%85%E8%A3%85%E5%99%A8)
  - [3、std::future](#3stdfuture)
  - [4、线程池实现](#4%E7%BA%BF%E7%A8%8B%E6%B1%A0%E5%AE%9E%E7%8E%B0)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->
## 线程池--C++11仿muduo

见 src/mythread

## 线程池--复杂版

参考 [C++11的简单线程池代码阅读](https://www.cnblogs.com/oloroso/p/5881863.html)

原理就是管理一个任务队列和一个工作线程队列。

工作线程不断的从任务队列取任务，然后执行。如果没有任务就等待新任务的到来。添加新任务的时候先添加到任务队列，然后通知任意(条件变量notify_one)一个线程有新的任务来了。

### 1、std::result_of

std::result_of可以在编译的时候推导出一个函数表达式的返回值类型：

```c
#include <iostream>

struct S {
	double operator()(char, int&); // 这个函数的返回类型是 double
};

int main()
{

	std::result_of<S(char, int&)>::type foo = 3.14; // 使用这样的写法会推导出模板参数中函数的返回值类型
	typedef std::result_of<S(char, int&)>::type MyType; // 是 double 类型吗?
	std::cout << "foo's type is double: " << std::is_same<double, MyType>::value << std::endl; // foo's type is double: 1
	return 0;
}
```

### 2、C++函数包装器
该部分参考 [C++函数包装器](https://blog.csdn.net/huangshanchun/article/details/45647911)

函数包装器把函数包装起来：有以下特点
* 设计通用的函数执行接口，可以设置计数（函数执行次数）和关卡
* 函数包装器依赖于函数模板，实现通用泛型
* 函数代码可以内嵌在函数中
* 原理是函数指针实现的

以下给出C++简单的函数包装器案例：

```c
#include<iostream>
#include<functional>
using namespace std;

//定义好了一个函数包装器
template < typename F, typename T>
T function_wrapper(F fun, T t)
{
	return fun(t);
}
//定义好了一个函数包装器
template < typename F, typename T>
T function_wrapper(F fun, T t1, T t2)
{
	static int count = 0;
	count++;
	cout << "函数包装器执行" << count << "次数" << endl;

	return fun(t1, t2);
}
int main()
{
	double d_num = 1.1;
	//double(double)是声明函数类型 
	//允许函数内嵌
	//本质是函数指针
	// [] 标识要开辟一个函数
	function<double(double)> square_fun = [](double in_data)
	{
		return in_data*in_data;
	};
	function<double(double)> cube_fun = [](double in_data)
	{
		return in_data*in_data*in_data;
	};
	function<int(int, int)> add_fun = [](int in_data1, int in_data2)
	{
		return in_data1 + in_data2;
	};
	cout << function_wrapper(square_fun, d_num) << " " << function_wrapper(cube_fun, d_num) << endl;
	cout << function_wrapper(add_fun, 1, 2) << endl;
	
	return 0;
}
```
###  3、std::future

详见相关章节。

### 4、线程池实现

见附录代码，代码中有大量注解。