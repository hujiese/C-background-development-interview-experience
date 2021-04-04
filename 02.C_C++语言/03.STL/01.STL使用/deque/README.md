<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->

- [STL -- deque](#stl----deque)
  - [1、介绍](#1%E4%BB%8B%E7%BB%8D)
  - [2、默认构造函数](#2%E9%BB%98%E8%AE%A4%E6%9E%84%E9%80%A0%E5%87%BD%E6%95%B0)
  - [3、添加移除操作](#3%E6%B7%BB%E5%8A%A0%E7%A7%BB%E9%99%A4%E6%93%8D%E4%BD%9C)
  - [4、deque的数据存取](#4deque%E7%9A%84%E6%95%B0%E6%8D%AE%E5%AD%98%E5%8F%96)
  - [5、deque与迭代器](#5deque%E4%B8%8E%E8%BF%AD%E4%BB%A3%E5%99%A8)
  - [6、deque的带参构造函数](#6deque%E7%9A%84%E5%B8%A6%E5%8F%82%E6%9E%84%E9%80%A0%E5%87%BD%E6%95%B0)
  - [7、deque的赋值](#7deque%E7%9A%84%E8%B5%8B%E5%80%BC)
  - [8、deque的大小](#8deque%E7%9A%84%E5%A4%A7%E5%B0%8F)
  - [9、deque插入元素](#9deque%E6%8F%92%E5%85%A5%E5%85%83%E7%B4%A0)
  - [10、deque删除元素](#10deque%E5%88%A0%E9%99%A4%E5%85%83%E7%B4%A0)
  - [11、使用案例](#11%E4%BD%BF%E7%94%A8%E6%A1%88%E4%BE%8B)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## STL -- deque

### 1、介绍

* deque是“double-ended queue”的缩写，和vector一样都是STL的容器，deque是双端数组，而vector是单端的。
* deque在接口上和vector非常相似，在许多操作的地方可以直接替换。
* deque可以随机存取元素（支持索引值直接存取， 用[]操作符或at()方法，这个等下会详讲）。
* deque头部和尾部添加或移除元素都非常快速。但是在中部安插元素或移除元素比较费时。

### 2、默认构造函数

deque采用模板类实现，deque对象的默认构造形式：
```c
deque<T> deqT; // 尖括号内可以设置指针类型或自定义类型。
deque<int> deqInt;            //一个存放int的deque容器。
deque<float> deqFloat;     //一个存放float的deque容器。
deque<string> deqString;     //一个存放string的deque容器。
```

### 3、添加移除操作

操作函数：
```c
deque.push_back(elem);	//在容器尾部添加一个数据
deque.push_front(elem);	//在容器头部插入一个数据
deque.pop_back();    		//删除容器最后一个数据
deque.pop_front();		//删除容器第一个数据
```

使用方法如下：
```c
deque<int> deqInt;
deqInt.push_back(1);
deqInt.push_back(3);
deqInt.push_back(5);
deqInt.pop_front();
deqInt.pop_front();
deqInt.push_front(11);
deqInt.push_front(13);
deqInt.pop_back();
deqInt.pop_back();
//最终结果 deqInt  { 13,11,5}
```

### 4、deque的数据存取

可通过下面这些函数来获取相关元素：
```c
deque.at(idx);  //返回索引idx所指的数据，如果idx越界，抛出out_of_range。
deque[idx];  //返回索引idx所指的数据，如果idx越界，不抛出异常，直接出错。
deque.front();   //返回第一个数据。
deque.back();  //返回最后一个数据
```
使用方法：
```c
deque<int> deqInt;
deqInt.push_back(1);
deqInt.push_back(3);
deqInt.push_back(5);
deqInt.push_back(7);
deqInt.push_back(9);

int iA = deqInt.at(0);		//1
int iB = deqInt[1];			//3
deqInt.at(0) = 99;			//99
deqInt[1] = 88;			//88

int iFront = deqInt.front();	//99
int iBack = deqInt.back();	//9
deqInt.front() = 77;			//77
deqInt.back() = 66;			//66
```

### 5、deque与迭代器

可通过下面这组函数获取deque的首尾位置迭代器：
```c
deque.begin();  //返回容器中第一个元素的迭代器。
deque.end();  //返回容器中最后一个元素之后的迭代器。
deque.rbegin();  //返回容器中倒数第一个元素的迭代器。
deque.rend();   //返回容器中倒数最后一个元素之后的迭代器。
```

使用方法如下：
```c
deque<int> deqInt;
deqInt.push_back(1);
deqInt.push_back(3);
deqInt.push_back(5);
deqInt.push_back(7);
deqInt.push_back(9);

for (deque<int>::iterator it=deqInt.begin(); it!=deqInt.end(); ++it)
{
    cout << *it;
    cout << "";
}
// 1 3 5 7 9

for (deque<int>::reverse_iterator rit=deqInt.rbegin(); rit!=deqInt.rend(); ++rit)
{
    cout << *rit;
    cout << "";
}
//9 7 5 3 1
```

### 6、deque的带参构造函数

函数原型如下：
```c
deque(beg,end);    //构造函数将[beg, end)区间中的元素拷贝给本身。注意该区间是左闭右开的区间。
deque(n,elem);   //构造函数将n个elem拷贝给本身。
deque(const deque  &deq);  //拷贝构造函数。
```
使用方法：
```c
deque<int> deqIntA;
deqIntA.push_back(1);
deqIntA.push_back(3);
deqIntA.push_back(5);
deqIntA.push_back(7);
deqIntA.push_back(9);

deque<int> deqIntB(deqIntA.begin(),deqIntA.end());		//1 3 5 7 9
deque<int> deqIntC(5,8);							//8 8 8 8 8
deque<int> deqIntD(deqIntA);						//1 3 5 7 9
```

### 7、deque的赋值

可以通过下面这些函数对deque对象进行赋值：
```c
deque.assign(beg,end);    //将[beg, end)区间中的数据拷贝赋值给本身。注意该区间是左闭右开的区间。
deque.assign(n,elem);  //将n个elem拷贝赋值给本身。
deque& operator=(const deque &deq);	//重载等号操作符 
deque.swap(deq);  // 将vec与本身的元素互换
```

使用方法如下：
```c
deque<int> deqIntA,deqIntB,deqIntC,deqIntD;
deqIntA.push_back(1);
deqIntA.push_back(3);
deqIntA.push_back(5);
deqIntA.push_back(7);
deqIntA.push_back(9);

deqIntB.assign(deqIntA.begin(),deqIntA.end());	// 1 3 5 7 9

deqIntC.assign(5,8);						//8 8 8 8 8

deqIntD = deqIntA;							//1 3 5 7 9

deqIntC.swap(deqIntD);						//互换
```

### 8、deque的大小

和vector类似：
```c
deque.size();	   //返回容器中元素的个数
deque.empty();	   //判断容器是否为空
deque.resize(num);   //重新指定容器的长度为num，若容器变长，则以默认值填充新位置。如果容器变短，则末尾超出容器长度的元素被删除。
deque.resize(num, elem);  //重新指定容器的长度为num，若容器变长，则以elem值填充新位置。如果容器变短，则末尾超出容器长度的元素被删除。
```

使用方法：
```c
deque<int> deqIntA;
deqIntA.push_back(1);
deqIntA.push_back(3);
deqIntA.push_back(5);

int iSize = deqIntA.size();  //3

if (!deqIntA.empty())
{
    deqIntA.resize(5);		//1 3 5 0 0
    deqIntA.resize(7,1);	//1 3 5 0 0 1 1
    deqIntA.resize(2);		//1 3
}
```

### 9、deque插入元素

可通过下面这组函数对deque对象进行插入操作：
```c
deque.insert(pos,elem);   //在pos位置插入一个elem元素的拷贝，返回新数据的位置。
deque.insert(pos,n,elem);   //在pos位置插入n个elem数据，无返回值。
deque.insert(pos,beg,end);   //在pos位置插入[beg,end)区间的数据，无返回值。
```
使用方法：
```c
deque<int> deqA;
deque<int> deqB;

deqA.push_back(1);
deqA.push_back(3);
deqA.push_back(5);
deqA.push_back(7);
deqA.push_back(9);

deqB.push_back(2);
deqB.push_back(4);
deqB.push_back(6);
deqB.push_back(8);

deqA.insert(deqA.begin(), 11);		//{11, 1, 3, 5, 7, 9}
deqA.insert(deqA.begin()+1,2,33);		//{11,33,33,1,3,5,7,9}
deqA.insert(deqA.begin() , deqB.begin() , deqB.end() );	//{2,4,6,8,11,33,33,1,3,5,7,9}
```

### 10、deque删除元素

操作函数如下：
```c
deque.clear();	//移除容器的所有数据
deque.erase(beg,end);  //删除[beg,end)区间的数据，返回下一个数据的位置。
deque.erase(pos);    //删除pos位置的数据，返回下一个数据的位置。
```

假设deqInt是用deque<int>声明的容器，现已包含按顺序的1,3,5,6,9元素，然后进行如下操作：
```c
deque<int>::iterator itBegin=deqInt.begin()+1;
deque<int>::iterator itEnd=deqInt.begin()+3;
deqInt.erase(itBegin,itEnd);
```
此时容器deqInt包含按顺序的1,6,9三个元素。

再次假设 deqInt 包含1,3,2,3,3,3,4,3,5,3，删除容器中等于3的元素：
```c
for(deque<int>::iterator it=deqInt.being(); it!=deqInt.end(); )    //小括号里不需写  ++it
{
   if(*it == 3)
   {
        it  =  deqInt.erase(it);       //以迭代器为参数，删除元素3，并把数据删除后的下一个元素位置返回给迭代器。此时不执行  ++it；  
   }
   else
   {
       ++it;
   }
}
```

删除deqInt的所有元素：
```c
deqInt.clear();			//容器为空
```

### 11、使用案例

见src下相关代码文件。