<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->

- [STL -- stack](#stl----stack)
  - [1、介绍](#1%E4%BB%8B%E7%BB%8D)
  - [2、默认构造函数](#2%E9%BB%98%E8%AE%A4%E6%9E%84%E9%80%A0%E5%87%BD%E6%95%B0)
  - [3、出栈和入栈操作](#3%E5%87%BA%E6%A0%88%E5%92%8C%E5%85%A5%E6%A0%88%E6%93%8D%E4%BD%9C)
  - [4、获取栈顶元素](#4%E8%8E%B7%E5%8F%96%E6%A0%88%E9%A1%B6%E5%85%83%E7%B4%A0)
  - [5、stack的大小](#5stack%E7%9A%84%E5%A4%A7%E5%B0%8F)
  - [6、使用案例](#6%E4%BD%BF%E7%94%A8%E6%A1%88%E4%BE%8B)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## STL -- stack

### 1、介绍

* stack是堆栈容器，是一种“先进后出”的容器。
* stack是简单地装饰deque容器而成为另外的一种容器。

### 2、默认构造函数

stack采用模板类实现， stack对象的默认构造形式： 
```c
stack <T> stkT;  //尖括号内可以设置指针类型或自定义类型。
stack <int> stkInt;            //一个存放int的stack容器。
stack <float> stkFloat;     //一个存放float的stack容器。
stack <string> stkString;     //一个存放string的stack容器。
```

### 3、出栈和入栈操作

函数定义如下：
```c
stack.push(elem);   //往栈头添加元素
stack.pop();   //从栈头移除第一个元素
```
使用方法：
```c
stack<int> stkInt;  	
stkInt.push(1);
stkInt.push(3);
stkInt.pop();   
stkInt.push(5);
stkInt.push(7);  
stkInt.push(9);
stkInt.pop();   	
stkInt.pop();  
// 此时stkInt存放的元素是1,5  
```
### 4、获取栈顶元素

函数定义如下：
```c
stack.top();	  //返回最后一个压入栈元素
```
使用方法：
```c
stack<int> stkIntA;
stkIntA.push(1);
stkIntA.push(3);
stkIntA.push(5);
stkIntA.push(7);
stkIntA.push(9);

int iTop = stkIntA.top();		//9
stkIntA.top() = 19;			//19
```

### 5、stack的大小

可通过下面这组函数获取栈的大小相关信息：
```c
stack.empty();   //判断堆栈是否为空
stack.size(); 	     //返回堆栈的大小
```
使用方法：
```c
stack<int> stkIntA;
stkIntA.push(1);
stkIntA.push(3);
stkIntA.push(5);
stkIntA.push(7);
stkIntA.push(9);

if (!stkIntA.empty())
{
    int iSize = stkIntA.size();		//5
}
```
### 6、使用案例

详见src下源码。