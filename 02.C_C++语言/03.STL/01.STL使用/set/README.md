<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->

- [STL -- set和multiset](#stl----set%E5%92%8Cmultiset)
  - [1、介绍](#1%E4%BB%8B%E7%BB%8D)
  - [2、默认构造函数](#2%E9%BB%98%E8%AE%A4%E6%9E%84%E9%80%A0%E5%87%BD%E6%95%B0)
  - [3、迭代器与数据插入](#3%E8%BF%AD%E4%BB%A3%E5%99%A8%E4%B8%8E%E6%95%B0%E6%8D%AE%E6%8F%92%E5%85%A5)
  - [4、排序](#4%E6%8E%92%E5%BA%8F)
  - [5、拷贝构造与赋值](#5%E6%8B%B7%E8%B4%9D%E6%9E%84%E9%80%A0%E4%B8%8E%E8%B5%8B%E5%80%BC)
  - [6、set的大小](#6set%E7%9A%84%E5%A4%A7%E5%B0%8F)
  - [7、删除元素](#7%E5%88%A0%E9%99%A4%E5%85%83%E7%B4%A0)
  - [8、查找](#8%E6%9F%A5%E6%89%BE)
  - [9、使用案例](#9%E4%BD%BF%E7%94%A8%E6%A1%88%E4%BE%8B)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## STL -- set和multiset

### 1、介绍

* set是一个集合容器，其中所包含的元素是唯一的，集合中的元素按一定的顺序排列。元素插入过程是按排序规则插入，所以不能指定插入位置。
* set采用红黑树变体的数据结构实现，红黑树属于平衡二叉树。在插入操作和删除操作上比vector快。
* set不可以直接存取元素。（不可以使用at.(pos)与[]操作符）。
* multiset与set的区别：set支持唯一键值，每个元素值只能出现一次；而multiset中同一值可以出现多次。
* 不可以直接修改set或multiset容器中的元素值，因为该类容器是自动排序的。如果希望修改一个元素值，必须先删除原有的元素，再插入新的元素。

### 2、默认构造函数

构造函数示范如下：
```c
set<int> setInt;            //一个存放int的set容器。
set<float> setFloat;     //一个存放float的set容器。
set<string> setString;     //一个存放string的set容器。
multiset<int> mulsetInt;            //一个存放int的multi set容器。
multiset<float> multisetFloat;     //一个存放float的multi set容器。
multiset<string> multisetString;     //一个存放string的multi set容器。
```

### 3、迭代器与数据插入

相关操作函数示例如下：
```c
set.insert(elem);     //在容器中插入元素。
set.begin();  //返回容器中第一个数据的迭代器。
set.end();  //返回容器中最后一个数据之后的迭代器。
set.rbegin();  //返回容器中倒数第一个元素的迭代器。
set.rend();   //返回容器中倒数最后一个元素的后面的迭代器。
```

使用方法：
```c
set<int> setInt;
setInt.insert(3); 
setInt.insert(1);
setInt.insert(5);
setInt.insert(2);
for(set<int>::iterator it=setInt.begin(); it!=setInt.end(); ++it)
{
    int iItem = *it;
    cout << iItem;    //或直接使用cout << *it
}
//这样子便顺序输出  1 2 3 5。
```

### 4、排序

相关操作函数如下所示：
```c
set<int,less<int> >  setIntA;  //该容器是按升序方式排列元素。
set<int,greater<int>> setIntB;   //该容器是按降序方式排列元素。
```
set<int> 相当于 set<int,less<int>>。less<int>与greater<int>中的int可以改成其它类型，该类型主要要跟set容纳的数据类型一致。

使用方法如下：
```c
set<int,greater<int>> setIntB;   
setIntB.insert(3);
setIntB.insert(1);
setIntB.insert(5);
setIntB.insert(2);
// 此时容器setIntB就包含了按顺序的5,3,2,1元素
```

那么less<>与greater<>是什么？如果set<>不包含int类型，而是包含自定义类型，set容器如何排序？

要解决如上两个问题，需要了解容器的函数对象，也叫仿函数，英文名叫functor。下面将讲解什么是functor，以及functor的用法。

尽管函数指针被广泛用于实现函数回调，但C++还提供了一个重要的实现回调函数的方法，那就是函数对象。functor，翻译成函数对象，仿函数，算符，是重载了“()”操作符的普通类对象。从语法上讲，它与普通函数行为类似。greater<>与less<>就是函数对象。下面举出greater<int>的简易实现原理：

```c
struct greater
{
    bool operator() (const int& iLeft, const int& iRight)
    {
        return (iLeft>iRight);    //如果是实现less<int>的话，这边是写return (iLeft<iRight);
    }
}
```
具体使用案例如下：
```c
#include <iostream>
#include <set>
#include <string>
using namespace std;

class CStudent
{
public:
	CStudent(int iID, string strName)
	{
		m_iID = iID;
		m_strName = strName;
	}
	int m_iID;		//学号
	string m_strName; 	//姓名
};

struct StuFunctor
{
	bool operator()  (const CStudent &stu1, const CStudent &stu2)
	{
		return (stu1.m_iID<stu2.m_iID);
	}
};

void main()
{
	set<CStudent, StuFunctor> setStu;
	setStu.insert(CStudent(3, "小张"));
	setStu.insert(CStudent(1, "小李"));
	setStu.insert(CStudent(5, "小王"));
	setStu.insert(CStudent(2, "小刘"));
	//此时容器setStu包含了四个学生对象，分别是按姓名顺序的“小李”，“小刘”，“小张”，“小王” 
	for (set<CStudent, StuFunctor>::iterator it = setStu.begin(); it != setStu.end(); it++)
	{
		cout << it->m_iID << "-" << it->m_strName <<  endl;
	}
	
}
```
输出结果如下：
```
1-小李
2-小刘
3-小张
5-小王
```

### 5、拷贝构造与赋值

操作函数如下所示：
```c
set(const set &st);		     //拷贝构造函数
set& operator=(const set &st);	//重载等号操作符
set.swap(st);				//交换两个集合容器
```
使用方法：
```c
set<int> setIntA;
setIntA.insert(3);
setIntA.insert(1);
setIntA.insert(7);
setIntA.insert(5);
setIntA.insert(9);

set<int> setIntB(setIntA);  //1 3 5 7 9

set<int> setIntC;
setIntC = setIntA;		//1 3 5 7 9

setIntC.insert(6);
setIntC.swap(setIntA);	  //交换
```

### 6、set的大小

操作函数如下所示：
```c
set.size();	//返回容器中元素的数目
set.empty();//判断容器是否为空
```

使用方法：
```c
set<int> setIntA;
setIntA.insert(3);
setIntA.insert(1);
setIntA.insert(7);
setIntA.insert(5);
setIntA.insert(9);

if (!setIntA.empty())
{
    int iSize = setIntA.size();		//5
}
```

### 7、删除元素

操作函数如下所示：
```c
set.clear();		//清除所有元素
set.erase(pos);	//删除pos迭代器所指的元素，返回下一个元素的迭代器。
set.erase(beg,end);	    //删除区间[beg,end)的所有元素	，返回下一个元素的迭代器。
set.erase(elem);     //删除容器中值为elem的元素。
```

假设setInt是用set<int>声明的容器，现已包含按顺序的1,3,5,6,9,11元素，现进行如下操作：
```c
set<int>::iterator itBegin=setInt.begin();
++itBegin;
set<int>::iterator itEnd=setInt.begin();
++itEnd;
++itEnd;
++itEnd;
setInt.erase(itBegin,itEnd);//此时容器setInt包含按顺序的1,6,9,11四个元素。

// 删除容器中第一个元素
setInt.erase(setInt.begin());		//6,9,11

// 删除容器中值为9的元素
set.erase(9);    

// 删除setInt的所有元素
setInt.clear();			//容器为空
```

### 8、查找

操作函数如下所示：
```c
set.find(elem);   //查找elem元素，返回指向elem元素的迭代器。
set.count(elem);   //返回容器中值为elem的元素个数。对set来说，要么是0，要么是1。对multiset来说，值可能大于1。
set.lower_bound(elem);  //返回第一个>=elem元素的迭代器。
set.upper_bound(elem);	   //  返回第一个>elem元素的迭代器。
set.equal_range(elem);		//返回容器中与elem相等的上下限的两个迭代器。上限是闭区间，下限是开区间，如[beg,end),这两个迭代器被封装在pair中。
```

有必要在这里补充下pair：

* pair译为对组，可以将两个值视为一个单元。
* pair<T1,T2>存放的两个值的类型，可以不一样，如T1为int，T2为float。T1,T2也可以是自定义类型。
* pair.first是pair里面的第一个值，是T1类型。
* pair.second是pair里面的第二个值，是T2类型。

使用方法如下：
```c
#include <iostream>
#include <set>
#include <string>
using namespace std;

int main(void)
{
	set<int> setInt;
	setInt.insert(3);
	setInt.insert(1);
	setInt.insert(7);
	setInt.insert(5);
	setInt.insert(9);

	cout << "---------find-----------" << endl;
	set<int>::iterator itA = setInt.find(5);
	int iA = *itA;		//iA == 5
	cout << iA << endl;

	cout << "---------count-----------" << endl;
	int iCount = setInt.count(5);	//iCount == 1
	cout << iCount << endl;

	cout << "---------lower/upper_bound-----------" << endl;
	set<int>::iterator itB = setInt.lower_bound(5);// >= 5
	set<int>::iterator itC = setInt.upper_bound(5);// > 5
	int iB = *itB;	//iB == 5
	int iC = *itC; //iC == 7
	cout << iB << "------" << iC << endl;

	cout << "---------equal_range-----------" << endl;
	pair< set<int>::iterator, set<int>::iterator > pairIt = setInt.equal_range(5);// [beg, end)
	set<int>::iterator itBeg = pairIt.first;
	set<int>::iterator itEnd = pairIt.second;
	cout << *itBeg << "----" << *itEnd << endl;

	return 0;
}
```
输出：
```
---------find-----------
5
---------count-----------
1
---------lower/upper_bound-----------
5------7
---------equal_range-----------
5----7
```

### 9、使用案例

详见src下源码。