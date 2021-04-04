<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->

- [heap--堆](#heap--%E5%A0%86)
  - [一、堆与堆排序](#%E4%B8%80%E5%A0%86%E4%B8%8E%E5%A0%86%E6%8E%92%E5%BA%8F)
  - [二、STL堆操作函数](#%E4%BA%8Cstl%E5%A0%86%E6%93%8D%E4%BD%9C%E5%87%BD%E6%95%B0)
    - [1、概述](#1%E6%A6%82%E8%BF%B0)
    - [2、函数使用](#2%E5%87%BD%E6%95%B0%E4%BD%BF%E7%94%A8)
      - [（1）make_heap -- 是生成一个堆，大顶堆或小顶堆](#1make_heap----%E6%98%AF%E7%94%9F%E6%88%90%E4%B8%80%E4%B8%AA%E5%A0%86%E5%A4%A7%E9%A1%B6%E5%A0%86%E6%88%96%E5%B0%8F%E9%A1%B6%E5%A0%86)
      - [（2）push_heap -- 向堆中插入一个元素，并且使堆的规则依然成立](#2push_heap----%E5%90%91%E5%A0%86%E4%B8%AD%E6%8F%92%E5%85%A5%E4%B8%80%E4%B8%AA%E5%85%83%E7%B4%A0%E5%B9%B6%E4%B8%94%E4%BD%BF%E5%A0%86%E7%9A%84%E8%A7%84%E5%88%99%E4%BE%9D%E7%84%B6%E6%88%90%E7%AB%8B)
      - [（3）pop_heap -- 在堆的基础上弹出堆顶元素](#3pop_heap----%E5%9C%A8%E5%A0%86%E7%9A%84%E5%9F%BA%E7%A1%80%E4%B8%8A%E5%BC%B9%E5%87%BA%E5%A0%86%E9%A1%B6%E5%85%83%E7%B4%A0)
      - [（4）sort_heap -- 堆排序](#4sort_heap----%E5%A0%86%E6%8E%92%E5%BA%8F)
      - [（5）is_heap -- 判断数组是否是一个堆](#5is_heap----%E5%88%A4%E6%96%AD%E6%95%B0%E7%BB%84%E6%98%AF%E5%90%A6%E6%98%AF%E4%B8%80%E4%B8%AA%E5%A0%86)
      - [（6）is_heap_until -- 获取区间中第一个不满足heap条件的位置](#6is_heap_until----%E8%8E%B7%E5%8F%96%E5%8C%BA%E9%97%B4%E4%B8%AD%E7%AC%AC%E4%B8%80%E4%B8%AA%E4%B8%8D%E6%BB%A1%E8%B6%B3heap%E6%9D%A1%E4%BB%B6%E7%9A%84%E4%BD%8D%E7%BD%AE)
    - [3、综合案例：](#3%E7%BB%BC%E5%90%88%E6%A1%88%E4%BE%8B)
    - [4、总结](#4%E6%80%BB%E7%BB%93)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## heap--堆

### 一、堆与堆排序

关于堆和堆排序的概念可参考这篇文章：
[堆和堆排序：为什么说堆排序没有快速排序快](./img/heap.png)
> https://blog.csdn.net/every__day/article/details/86580883

### 二、STL堆操作函数

该部分参考:

 [make_heap(), pop_heap(), push_heap()用法](https://blog.csdn.net/qq_29630271/article/details/66478256) 

[STL中heap相关函数的用法：make_heap，push_heap，pop_heap，sort heap](https://blog.csdn.net/qq_34793133/article/details/81222919)

#### 1、概述

在STL中，heap是算法的形式提供给我们使用的。包括下面几个函数：

函数|功能|时间复杂度
---|---|---
make_heap|根据指定的迭代器区间以及一个可选的比较函数，来创建一个heap|O(N)
push_heap|把指定区间的最后一个元素插入到heap中|O(logN)
pop_heap |弹出heap顶元素, 将其放置于区间末尾|O(logN)
sort_heap|堆排序算法，通常通过反复调用pop_heap来实现|N*O(logN)


C++11加入了两个新成员：

函数|功能|时间复杂度
---|---|---
is_heap|判断给定区间是否是一个heap|O(N)
is_heap_until|找出区间中第一个不满足heap条件的位置|O(N)

因为heap以算法的形式提供，所以要使用这几个api需要包含 #include\<algorithm>

#### 2、函数使用

##### （1）make_heap -- 是生成一个堆，大顶堆或小顶堆

STL中的通过make_heap创建的堆，默认是大顶堆(max heap)，即每个非叶子节点元素的值均不”小于”(默认使用<作为比较准则)其左右孩子节点。要改变堆的建立准则，可以自己制定一个比较函数，如下第二个版本的make_heap声明：

```cpp

// 1 默认生成大顶堆
template< class RandomIt >
void make_heap( RandomIt first, RandomIt last );
 
// 2 _Compare有两种参数，一种是greater（生成小顶堆），一种是less（生成大顶堆）
template< class RandomIt, class Compare >
void make_heap( RandomIt first, RandomIt last, Compare comp );
```

案例：

```cpp
vector<int> vi{6, 1, 2, 5, 3, 4};
printContainer(vi, "vi: ");             // vi: 6 1 2 5 3 4
 
make_heap(vi.begin(), vi.end());
printContainer(vi, "vi: ");             // vi: 6 5 4 1 3 2

//-------------------------------------------------

vector<int> v2{6, 1, 2, 5, 3, 4};
printContainer(v2, "v2 before make_heap: ");// v2 before make_heap: 6 1 2 5 3 4
make_heap(v2.begin(), v2.end(), greater<int>());
printContainer(v2, "v2 after make_heap: ");// v2 after make_heap: 1 3 2 5 6 4
// 这里使用了greater<int>()来代替默认的less<int>()来创建int类型的heap。可以按层次遍历的顺序把这个heap画出来，可以看到它跟默认情况刚好相反，会是一个小顶堆。
```

##### （2）push_heap -- 向堆中插入一个元素，并且使堆的规则依然成立

```cpp
// 1 默认为大顶堆
template< class RandomIt >
void push_heap( RandomIt first, RandomIt last );
 
// 2  _Compare有两种参数，一种是greater（小顶堆），一种是less（大顶堆）
template< class RandomIt, class Compare >
void push_heap( RandomIt first, RandomIt last, Compare comp );
```

注意：

* 调用push_heap之前必须调用make_heap创建一个堆。
* 首先数组push_back插入元素，然后再调用push_heap，它会使最后一个元素插到合适位置。
* push_heap中的_Compare和make_heap中的_Compare参数必须是一致的，不然会插入堆失败，最后一个元素还是在最后位置，导致插入失败。

案例：

```cpp
vector<int> v1{6, 1, 2, 5, 3, 4};
make_heap(v1.begin(), v1.end());
 
v1.push_back(200);
printContainer(v1, "before push_heap: ");        // before push_heap: 6 5 4 1 3 2 200
push_heap(v1.begin(), v1.end());
printContainer(v1, "after push_heap: ");         // after push_heap: 200 5 6 1 3 2 4
```

先用make_heap来构造一个堆，然后在容器末尾追加元素之后，把新的迭代器区间传给push_heap，这样新尾部元素也被添加到堆中。

注意：在使用push_heap(f, l)前，调用者需要确保[f, l-1)已经是一个堆. push_heap(f, l)仅仅会把*(l-1)插入到[f, l-1)这个区间形成的堆中，时间复杂度是O(logN)。

下面演示一开始不用make_heap处理，直接push_heap的情况：

```cpp
vector<int> v2{6, 1, 2, 5, 3, 4};
v2.push_back(200);
printContainer(v2, "v2 before push_heap: ");// v2 before push_heap: 6 1 2 5 3 4 200
push_heap(v2.begin(), v2.end());
printContainer(v2, "v2 after push_heap: ");// v2 after push_heap: 200 1 6 5 3 4 2
```

可以看出直接调用push_heap的结果并不是一个heap. 下面的pop_heap也有同样。

##### （3）pop_heap -- 在堆的基础上弹出堆顶元素

```cpp
// 1 默认为大顶堆
template< class RandomIt >
void pop_heap( RandomIt first, RandomIt last );
 
// 2  _Compare有两种参数，一种是greater（小顶堆），一种是less（大顶堆）
template< class RandomIt, class Compare >
void pop_heap( RandomIt first, RandomIt last, Compare comp );
```
例如，pop_heap(nums.begin(), nums.end(),greater<int>())，它会将堆顶元素（即为数组第一个位置）和数组最后一个位置对调，然后你可以调用数组pop_back，删除这个元素，需要注意的是，调用者要保证，在调用pop_heap时[first, last)已经是一个堆(使用相同的排序准则)：

```cpp
vector<int> v1{6, 1, 2, 5, 3, 4};
make_heap(v1.begin(), v1.end());
printContainer(v1, "after make_heap: ");// after make_heap: 6 5 4 1 3 2
 
pop_heap(v1.begin(), v1.end());
printContainer(v1, "after pop_heap: ");// after pop_heap: 5 3 4 1 2 6

auto largest = v1.back();
psln(largest);// largest = 6
v1.pop_back();
printContainer(v1, "delete largest: ");// delete largest: 5 3 4 1 2
```

注意，pop_heap中的_Compare和make_heap中的_Compare参数必须是一致的，不然会失败。

##### （4）sort_heap -- 堆排序
```cpp
// 1
template< class RandomIt >
void sort_heap( RandomIt first, RandomIt last );
 
// 2
template< class RandomIt, class Compare >
void sort_heap( RandomIt first, RandomIt last, Compare comp );
```

sort_heap即经典的堆排序算法，通过每次弹出堆顶直到堆为空，依次被弹出的元素就组成了有序的序列了。STL中的priority_queue即使用heap的这个特性来实现。

使用sort_heap(f, l)处理过的区间因为已经有序，就不再是一个heap了：
```cpp
vector<int> v1{6, 1, 2, 5, 3, 4};
printContainer(v1, "before sort_heap: ");// before sort_heap: 6 1 2 5 3 4       
 
make_heap(v1.begin(), v1.end());
 
sort_heap(v1.begin(), v1.end());
printContainer(v1, "after sort_heap: ");// after sort_heap: 1 2 3 4 5 6
```
注意：

* 调用者仍需确保区间已经是一个堆。
* make_heap和sort_heap的Compare参数要一致。

##### （5）is_heap -- 判断数组是否是一个堆

```cpp
// (1)  (since C++11)
template< class RandomIt >
bool is_heap( RandomIt first, RandomIt last );
 
// (2)  (since C++11)
template< class RandomIt, class Compare >
bool is_heap( RandomIt first, RandomIt last, Compare comp );
```
使用方法：

```cpp
vector<int> v1{6, 1, 2, 5, 3, 4};
psln(is_heap(v1.begin(), v1.end()));// is_heap(v1.begin(), v1.end()) = 0
 
pln("after make_heap");// after make_heap
 
make_heap(v1.begin(), v1.end());
psln(is_heap(v1.begin(), v1.end()));// is_heap(v1.begin(), v1.end()) = 1
```

##### （6）is_heap_until -- 获取区间中第一个不满足heap条件的位置

```cpp
// (1)  (since C++11)
template< class RandomIt >
RandomIt is_heap_until( RandomIt first, RandomIt last );
 
// (2)  (since C++11)
template< class RandomIt, class Compare >
RandomIt is_heap_until( RandomIt first, RandomIt last, Compare comp );
```

使用方法：

```cpp
vector<int> v1{6, 1, 2, 5, 3, 4};
auto iter = is_heap_until(v1.begin(), v1.end());
psln(*iter);        // *iter = 5    5 是第一个不满足heap条件的位置。
 
make_heap(v1.begin(), v1.end());
iter = is_heap_until(v1.begin(), v1.end());
ASSERT_TRUE(iter == v1.end());
```
#### 3、综合案例：

```cpp
# include <iostream>
# include <functional>
# include <vector>
# include <algorithm>

using namespace std;

void printVec(vector<int> nums)
{
	for (int i = 0; i < nums.size(); ++i)
		cout << nums[i] << " ";
	cout << endl;
}
int main(void)
{
	int nums_temp[] = { 8, 3, 4, 8, 9, 2, 3, 4, 10 };
	vector<int> nums(nums_temp, nums_temp + 9);
	cout << "make_heap之前: ";
	printVec(nums);

	cout << "(默认(less))make_heap: ";
	make_heap(nums.begin(), nums.end());
	printVec(nums);

	cout << "(less)make_heap: ";
	make_heap(nums.begin(), nums.end(), less<int>());
	printVec(nums);

	cout << "(greater)make_heap: ";
	make_heap(nums.begin(), nums.end(), greater<int>());
	printVec(nums);

	cout << "此时，nums为小顶堆 greater" << endl;
	cout << "push_back(3)" << endl;
	nums.push_back(3);
	//cout << "默认(less)push_heap 此时push_heap失败: ";
	//push_heap(nums.begin(), nums.end());
	//printVec(nums);
	cout << "push_heap为greater 和make_heap一致，此时push_heap成功: ";
	push_heap(nums.begin(), nums.end(), greater<int>());
	printVec(nums);
	cout << "(greater,不然会失败)pop_heap: ";
	pop_heap(nums.begin(), nums.end(), greater<int>());
	printVec(nums);
	cout << "pop_back(): ";
	nums.pop_back();
	printVec(nums);


	cout << "--------------------------------sort---------------------------" << endl;
	vector<int> v1{ 6, 1, 2, 5, 3, 4 };
	cout << "before sort_heap: ";
	printVec(v1);

	make_heap(v1.begin(), v1.end(), greater<int>());

	// sort_heap(v1.begin(), v1.end());
	sort_heap(v1.begin(), v1.end(), greater<int>());
	cout << "after sort_heap: ";
	printVec(v1);
}
```

结果如下：

```
make_heap之前: 8 3 4 8 9 2 3 4 10
(默认(less))make_heap: 10 9 4 8 8 2 3 4 3
(less)make_heap: 10 9 4 8 8 2 3 4 3
(greater)make_heap: 2 3 3 4 8 4 10 9 8
此时，nums为小顶堆 greater
push_back(3)
push_heap为greater 和make_heap一致，此时push_heap成功: 2 3 3 4 3 4 10 9 8 8
(greater,不然会失败)pop_heap: 3 3 4 4 3 8 10 9 8 2
pop_back(): 3 3 4 4 3 8 10 9 8
--------------------------------sort---------------------------
before sort_heap: 6 1 2 5 3 4
after sort_heap: 6 5 4 3 2 1
```

#### 4、总结

* 建堆，make_heap

* 堆操作：增加元素(push_heap)，删除元素(pop_heap), 排序(sort_heap), 均要求区间已经是一个heap，并且是与当前操作使用相同的排序准则

* is_heap, is_heap_until当做辅助判断函数来用

* 所有的heap算法操作的区间都需要是随机迭代器组成