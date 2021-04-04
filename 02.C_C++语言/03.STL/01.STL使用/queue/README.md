<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->

- [STL -- queue](#stl----queue)
  - [1、介绍](#1%E4%BB%8B%E7%BB%8D)
  - [2、默认构造函数](#2%E9%BB%98%E8%AE%A4%E6%9E%84%E9%80%A0%E5%87%BD%E6%95%B0)
  - [3、入队和出队操作](#3%E5%85%A5%E9%98%9F%E5%92%8C%E5%87%BA%E9%98%9F%E6%93%8D%E4%BD%9C)
  - [4、拷贝构造函数与赋值](#4%E6%8B%B7%E8%B4%9D%E6%9E%84%E9%80%A0%E5%87%BD%E6%95%B0%E4%B8%8E%E8%B5%8B%E5%80%BC)
  - [5、获取队首和队尾元素](#5%E8%8E%B7%E5%8F%96%E9%98%9F%E9%A6%96%E5%92%8C%E9%98%9F%E5%B0%BE%E5%85%83%E7%B4%A0)
  - [6、queue的大小](#6queue%E7%9A%84%E5%A4%A7%E5%B0%8F)
  - [7、使用案例](#7%E4%BD%BF%E7%94%A8%E6%A1%88%E4%BE%8B)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## STL -- queue

### 1、介绍

* queue是队列容器，是一种“先进先出”的容器。
* queue是简单地装饰deque容器而成为另外的一种容器。

### 2、默认构造函数

queue采用模板类实现，queue对象的默认构造形式：
```c
queue<T> queT;   //尖括号内可以设置指针类型或自定义类型。
queue<int> queInt;            //一个存放int的queue容器。
queue<float> queFloat;     //一个存放float的queue容器。
queue<string> queString;     //一个存放string的queue容器。
```

### 3、入队和出队操作

可使用下面两个函数对queue进行入队和出队操作：
```c
queue.push(elem);   //往队尾添加元素
queue.pop();   //从队头移除第一个元素
```
使用方法：
```c
queue<int> queInt;
queInt.push(1);
queInt.push(3);
queInt.push(5);
queInt.push(7);
queInt.push(9);
queInt.pop();
queInt.pop();
// 此时queInt存放的元素是5,7,9
```
### 4、拷贝构造函数与赋值

queue提供了拷贝构造函数和重载了“=”运算符来实现拷贝：
```c
queue(const queue &que);		     //拷贝构造函数
queue& operator=(const queue &que);	//重载等号操作符
```
使用方法：
```c
queue<int> queIntA;
queIntA.push(1);
queIntA.push(3);
queIntA.push(5);
queIntA.push(7);
queIntA.push(9);

queue<int> queIntB(queIntA);	//拷贝构造
queue<int> queIntC;
queIntC = queIntA;				//赋值
```

### 5、获取队首和队尾元素

操作函数如下：
```c
queue.back();   //返回最后一个元素
queue.front();   //返回第一个元素
```
使用方法：
```c
queue<int> queIntA;
queIntA.push(1);
queIntA.push(3);
queIntA.push(5);
queIntA.push(7);
queIntA.push(9);

int iFront = queIntA.front();		//1
int iBack = queIntA.back();		//9

queIntA.front() = 11;			//11
queIntA.back() = 19;			//19
```

### 6、queue的大小

相关函数如下：
```c
queue.empty();   //判断队列是否为空
queue.size(); 	     //返回队列的大小
```
使用方法：
```c
queue<int> queIntA; 	
queIntA.push(1);   	
queIntA.push(3);  		
queIntA.push(5);		
queIntA.push(7);		
queIntA.push(9);		

if (!queIntA.empty())
{
    int iSize = queIntA.size();		//5
}
```

### 7、使用案例

详见src下源码。