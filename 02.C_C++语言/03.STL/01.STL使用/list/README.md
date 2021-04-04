<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->

- [STL -- list](#stl----list)
  - [1、介绍](#1%E4%BB%8B%E7%BB%8D)
  - [2、默认构造函数](#2%E9%BB%98%E8%AE%A4%E6%9E%84%E9%80%A0%E5%87%BD%E6%95%B0)
  - [3、在列表头尾操作元素](#3%E5%9C%A8%E5%88%97%E8%A1%A8%E5%A4%B4%E5%B0%BE%E6%93%8D%E4%BD%9C%E5%85%83%E7%B4%A0)
  - [4、获取列表头尾数据](#4%E8%8E%B7%E5%8F%96%E5%88%97%E8%A1%A8%E5%A4%B4%E5%B0%BE%E6%95%B0%E6%8D%AE)
  - [5、列表与迭代器](#5%E5%88%97%E8%A1%A8%E4%B8%8E%E8%BF%AD%E4%BB%A3%E5%99%A8)
  - [6、带参构造函数](#6%E5%B8%A6%E5%8F%82%E6%9E%84%E9%80%A0%E5%87%BD%E6%95%B0)
  - [7、赋值函数](#7%E8%B5%8B%E5%80%BC%E5%87%BD%E6%95%B0)
  - [8、列表大小](#8%E5%88%97%E8%A1%A8%E5%A4%A7%E5%B0%8F)
  - [9、插入元素](#9%E6%8F%92%E5%85%A5%E5%85%83%E7%B4%A0)
  - [10、删除元素](#10%E5%88%A0%E9%99%A4%E5%85%83%E7%B4%A0)
  - [11、列表反转](#11%E5%88%97%E8%A1%A8%E5%8F%8D%E8%BD%AC)
  - [12、使用案例](#12%E4%BD%BF%E7%94%A8%E6%A1%88%E4%BE%8B)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## STL -- list

### 1、介绍

* list是一个双向链表容器，可高效地进行插入删除元素。
* list不可以随机存取元素，所以不支持at.(pos)函数与[]操作符（只能使用迭代器it++,it+5这样随意获取是不被允许的，只能按照顺序访问容器中的元素）。

### 2、默认构造函数

list采用采用模板类实现,对象的默认构造形式：
```c
list<T> lstT;  //尖括号内可以设置指针类型或自定义类型。
list<int> lstInt;            //定义一个存放int的list容器。
list<float> lstFloat;     //定义一个存放float的list容器。
list<string> lstString;     //定义一个存放string的list容器。
```

### 3、在列表头尾操作元素

操作函数如下所示：
```c
list.push_back(elem);	   //在容器尾部加入一个元素
list.pop_back();              //删除容器中最后一个元素
list.push_front(elem);     //在容器开头插入一个元素
list.pop_front();              //从容器开头移除第一个元素
```
使用方法：
```c
list<int> lstInt;
lstInt.push_back(1);
lstInt.push_back(3);
lstInt.push_back(5);
lstInt.push_back(7);
lstInt.push_back(9);
lstInt.pop_front();
lstInt.pop_front();
lstInt.push_front(11);
lstInt.push_front(13);
lstInt.pop_back();
lstInt.pop_back();
// lstInt    {13,11,5}
```

### 4、获取列表头尾数据

操作函数如下：
```c
list.front();   //返回第一个元素。
list.back();  //返回最后一个元素。
```
使用方法：
```c
list<int> lstInt;
lstInt.push_back(1);
lstInt.push_back(3);
lstInt.push_back(5);
lstInt.push_back(7);
lstInt.push_back(9);

int iFront = lstInt.front();	//1
int iBack = lstInt.back();		//9
lstInt.front() = 11;			//11
lstInt.back() = 19;			//19
```

### 5、列表与迭代器

相关函数如下所示：
```c
list.begin();        //返回容器中第一个元素的迭代器。
list.end();          //返回容器中最后一个元素之后的迭代器。
list.rbegin();       //返回容器中倒数第一个元素的迭代器。
list.rend();         //返回容器中倒数最后一个元素的后面的迭代器。
```
使用方法：
```c
list<int> lstInt;
lstInt.push_back(1);
lstInt.push_back(3);
lstInt.push_back(5);
lstInt.push_back(7);
lstInt.push_back(9);

for (list<int>::iterator it=lstInt.begin(); it!=lstInt.end(); ++it)
{
    cout << *it;
    cout << " ";
}

for (list<int>::reverse_iterator rit=lstInt.rbegin(); rit!=lstInt.rend(); ++rit)
{
    cout << *rit;
    cout << " ";
}
```

### 6、带参构造函数

函数定义如下：
```c
list(beg,end);    //构造函数将[beg, end)区间中的元素拷贝给本身。注意该区间是左闭右开的区间。
list(n,elem);   //构造函数将n个elem拷贝给本身。
list(const list &lst);  //拷贝构造函数。
```
使用方法：
```c
list<int> lstIntA;
lstIntA.push_back(1);
lstIntA.push_back(3);
lstIntA.push_back(5);
lstIntA.push_back(7);
lstIntA.push_back(9);

list<int> lstIntB(lstIntA.begin(),lstIntA.end());		//1 3 5 7 9
list<int> lstIntC(5,8);							//8 8 8 8 8 
list<int> lstIntD(lstIntA);						//1 3 5 7 9
```

### 7、赋值函数

操作函数如下所示：
```c
list.assign(beg,end);    //将[beg, end)区间中的数据拷贝赋值给本身。注意该区间是左闭右开的区间。
list.assign(n,elem);  //将n个elem拷贝赋值给本身。
list& operator=(const list &lst);	//重载等号操作符
list.swap(lst);  // 将lst与本身的元素互换。
```
使用方法：
```c
list<int> lstIntA,lstIntB,lstIntC,lstIntD;
lstIntA.push_back(1);
lstIntA.push_back(3);
lstIntA.push_back(5);
lstIntA.push_back(7);
lstIntA.push_back(9);

lstIntB.assign(lstIntA.begin(),lstIntA.end());		//1 3 5 7 9
lstIntC.assign(5,8);							//8 8 8 8 8
lstIntD = lstIntA;							//1 3 5 7 9
lstIntC.swap(lstIntD);						//互换
```
### 8、列表大小

操作函数定义如下：
```c
list.size();	   //返回容器中元素的个数
list.empty();	   //判断容器是否为空
list.resize(num);   //重新指定容器的长度为num，若容器变长，则以默认值填充新位置。如果容器变短，则末尾超出容器长度的元素被删除。
list.resize(num, elem);  //重新指定容器的长度为num，若容器变长，则以elem值填充新位置。如果容器变短，则末尾超出容器长度的元素被删除。
```
使用方法：
```c
list<int> lstIntA;
lstIntA.push_back(1);
lstIntA.push_back(3);
lstIntA.push_back(5);

if (!lstIntA.empty())
{
    int iSize = lstIntA.size();		//3
    lstIntA.resize(5);			//1 3 5 0 0
    lstIntA.resize(7,1);			//1 3 5 0 0 1 1
    lstIntA.resize(2);			//1 3
}
```

### 9、插入元素

操作函数如下所示：
```c
list.insert(pos,elem);   //在pos位置插入一个elem元素的拷贝，返回新数据的位置。
list.insert(pos,n,elem);   //在pos位置插入n个elem数据，无返回值。
list.insert(pos,beg,end);   //在pos位置插入[beg,end)区间的数据，无返回值。
```

使用方法：
```c
list<int> lstA;
list<int> lstB;

lstA.push_back(1);
lstA.push_back(3);
lstA.push_back(5);
lstA.push_back(7);
lstA.push_back(9);

lstB.push_back(2);
lstB.push_back(4);
lstB.push_back(6);
lstB.push_back(8);

lstA.insert(lstA.begin(), 11);		//{11, 1, 3, 5, 7, 9}
lstA.insert(++lstA.begin(),2,33);		//{11,33,33,1,3,5,7,9}
lstA.insert(lstA.begin() , lstB.begin() , lstB.end() );	//{2,4,6,8,11,33,33,1,3,5,7,9}
```

### 10、删除元素

操作函数如下所示：
```c
list.clear();		//移除容器的所有数据
list.erase(beg,end);  //删除[beg,end)区间的数据，返回下一个数据的位置。
list.erase(pos);    //删除pos位置的数据，返回下一个数据的位置。
lst.remove(elem);   //删除容器中所有与elem值匹配的元素。
```

假设lstInt是用list<int>声明的容器，现已包含按顺序的1,3,5,6,9元素，然后进行如下操作：
```c
list<int>::iterator itBegin=lstInt.begin();
++itBegin;
list<int>::iterator itEnd=lstInt.begin();
++itEnd;
++itEnd;
++itEnd;
lstInt.erase(itBegin,itEnd);
//此时容器lstInt包含按顺序的1,6,9三个元素。
```

再假设 lstInt 包含1,3,2,3,3,3,4,3,5,3，删除容器中等于3的元素的方法一：
```c
for(list<int>::iterator it=lstInt.being(); it!=lstInt.end(); )    //小括号里不需写  ++it
{
   if(*it == 3)
   {
        it  =  lstInt.erase(it);       //以迭代器为参数，删除元素3，并把数据删除后的下一个元素位置返回给迭代器，此时不执行  ++it；  
   }
   else
   {
       ++it;
   }
}
```

删除容器中等于3的元素的方法二:
```c
lstInt.remove(3);
```

删除lstInt的所有元素:
```c
lstInt.clear();			//容器为空
```

### 11、列表反转

操作函数如下：
```c
lst.reverse();     //反转链表，比如lst包含1,3,5元素，运行此方法后，lst就包含5,3,1元素。
```

使用方法：
```c
list<int> lstA;

lstA.push_back(1);
lstA.push_back(3);
lstA.push_back(5);
lstA.push_back(7);
lstA.push_back(9);

lstA.reverse();			//9 7 5 3 1
```

### 12、使用案例

详见src下源码。