<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->

- [lambda表达式](#lambda%E8%A1%A8%E8%BE%BE%E5%BC%8F)
  - [一、声明Lambda表达式](#%E4%B8%80%E5%A3%B0%E6%98%8Elambda%E8%A1%A8%E8%BE%BE%E5%BC%8F)
  - [二、捕获外部变量](#%E4%BA%8C%E6%8D%95%E8%8E%B7%E5%A4%96%E9%83%A8%E5%8F%98%E9%87%8F)
    - [1、值捕获](#1%E5%80%BC%E6%8D%95%E8%8E%B7)
    - [2、引用捕获](#2%E5%BC%95%E7%94%A8%E6%8D%95%E8%8E%B7)
    - [3、隐式捕获](#3%E9%9A%90%E5%BC%8F%E6%8D%95%E8%8E%B7)
    - [4、混合方式](#4%E6%B7%B7%E5%90%88%E6%96%B9%E5%BC%8F)
  - [三、修改捕获变量](#%E4%B8%89%E4%BF%AE%E6%94%B9%E6%8D%95%E8%8E%B7%E5%8F%98%E9%87%8F)
  - [四、Lambda表达式的参数](#%E5%9B%9Blambda%E8%A1%A8%E8%BE%BE%E5%BC%8F%E7%9A%84%E5%8F%82%E6%95%B0)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## lambda表达式

本文参考 [C++ 11 Lambda表达式](https://www.cnblogs.com/DswCnblog/p/5629165.html)

C++11引入了Lambda表达式。利用Lambda表达式，可以方便的定义和创建匿名函数。很多高级语言在很早以前就已经提供了Lambda表达式的功能。

### 一、声明Lambda表达式

Lambda表达式完整的声明格式如下：
```c
[capture list] (params list) mutable exception-> return_type { function body }
```
各项具体含义如下：

* capture list：捕获外部变量列表
* params list：形参列表
* mutable指示符：用来说用是否可以修改捕获的变量
* exception：异常设定
* return type：返回类型
* function body：函数体

此外，我们还可以省略其中的某些成分来声明“不完整”的Lambda表达式，常见的有以下几种：

序号|格式
---|---
1	| [capture list] (params list) -> return_type {function body} 
2	| [capture list] (params list) {function body}
3	| [capture list] {function body}

其中：

* 格式1声明了const类型的表达式，这种类型的表达式不能修改捕获列表中的值。
 * 格式2省略了返回值类型，但编译器可以根据以下规则推断出Lambda表达式的返回类型：

   * （1）如果function body中存在return语句，则该Lambda表达式的返回类型由return语句的返回类型确定； 
   * （2）如果function body中没有return语句，则返回值为void类型。
* 格式3中省略了参数列表，类似普通函数中的无参函数。

下面举一个实例：
```c
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

bool cmp(int a, int b)
{
	return  a < b;
}

int main()
{
	vector<int> myvec{ 3, 2, 5, 7, 3, 2 };
	vector<int> lbvec(myvec);

	sort(myvec.begin(), myvec.end(), cmp); // 旧式做法
	cout << "predicate function:" << endl;
	for (int it : myvec)
		cout << it << ' ';
	cout << endl;

	sort(lbvec.begin(), lbvec.end(), [](int a, int b) -> bool { return a < b; });   // Lambda表达式
	cout << "lambda expression:" << endl;
	for (int it : lbvec)
		cout << it << ' ';
	cout << endl;
}
```
运行结果如下：
```
predicate function:
2 2 3 3 5 7
lambda expression:
2 2 3 3 5 7
```
在C++11之前使用STL的sort函数，需要提供一个谓词函数。如果使用C++11的Lambda表达式，我们只需要传入一个匿名函数即可，方便简洁，而且代码的可读性也比旧式的做法好多了。

接下来将重点介绍一下Lambda表达式各项的具体用法。

### 二、捕获外部变量

Lambda表达式可以使用其可见范围内的外部变量，但必须明确声明（明确声明哪些外部变量可以被该Lambda表达式使用）。那么，在哪里指定这些外部变量呢？Lambda表达式通过在最前面的方括号[]来明确指明其内部可以访问的外部变量，这一过程也称过Lambda表达式“捕获”了外部变量：

```c
#include <iostream>
using namespace std;

int main()
{
	int a = 123;
	auto f = [a] { cout << a << endl; };
	f(); // 输出：123

	//或通过“函数体”后面的‘()’传入参数
	[](int a){cout << a << endl; }(1233435);
}
```
运行结果如下：
```
123
1233435
```
上面这个例子先声明了一个整型变量a，然后再创建Lambda表达式，该表达式“捕获”了a变量，这样在Lambda表达式函数体中就可以获得该变量的值。

类似参数传递方式（值传递、引入传递、指针传递），在Lambda表达式中，外部变量的捕获方式也有值捕获、引用捕获、隐式捕获。

#### 1、值捕获

值捕获和参数传递中的值传递类似，被捕获的变量的值在Lambda表达式创建时通过值拷贝的方式传入，因此随后对该变量的修改不会影响影响Lambda表达式中的值：
```c
#include <iostream>
using namespace std;

int main()
{
    int a = 123;
    auto f = [a] { cout << a << endl; }; 
    a = 321;
    f(); // 输出：123
}
```
这里需要注意的是，如果以传值方式捕获外部变量，则在Lambda表达式函数体中不能修改该外部变量的值。

#### 2、引用捕获

使用引用捕获一个外部变量，只需要在捕获列表变量前面加上一个引用说明符&。如下：

```c
#include <iostream>
using namespace std;

int main()
{
	int a = 123;
	auto f = [&a] { cout << a << endl; };
	a = 321;
	f(); // 输出：321
}
```
从示例中可以看出，引用捕获的变量使用的实际上就是该引用所绑定的对象。

#### 3、隐式捕获

上面的值捕获和引用捕获都需要我们在捕获列表中显示列出Lambda表达式中使用的外部变量。除此之外，我们还可以让编译器根据函数体中的代码来推断需要捕获哪些变量，这种方式称之为隐式捕获。隐式捕获有两种方式，分别是[=]和[&]。[=]表示以值捕获的方式捕获外部变量，[&]表示以引用捕获的方式捕获外部变量。

隐式值捕获示例：
```c
int main()
{
	int a = 123;
	auto f = [=] { cout << a << endl; };    // 值捕获
	f(); // 输出：123
}
```
隐式引用捕获示例：

```c
int main()
{
    int a = 123;
    auto f = [&] { cout << a << endl; };    // 引用捕获
    a = 321;
    f(); // 输出：321
}
```
#### 4、混合方式

上面的例子，要么是值捕获，要么是引用捕获，Lambda表达式还支持混合的方式捕获外部变量，这种方式主要是以上几种捕获方式的组合使用。

到这里，我们来总结一下：C++11中的Lambda表达式捕获外部变量主要有以下形式：

捕获形式|	说明
---|---
[]|	不捕获任何外部变量
[变量名, …] |	默认以值得形式捕获指定的多个外部变量（用逗号分隔），如果引用捕获，需要显示声明（使用&说明符）
[this]|	以值的形式捕获this指针
[=]|	以值的形式捕获所有外部变量
[&]	|以引用形式捕获所有外部变量
[=, &x]	|变量x以引用形式捕获，其余变量以传值形式捕获
[&, x]	|变量x以值的形式捕获，其余变量以引用形式捕获

### 三、修改捕获变量

前面提到过，在Lambda表达式中，如果以传值方式捕获外部变量，则函数体中不能修改该外部变量，否则会引发编译错误。那么有没有办法可以修改值捕获的外部变量呢？这是就需要使用mutable关键字，该关键字用以说明表达式体内的代码可以修改值捕获的变量：

```c
int main()
{
    int a = 123;
    auto f = [a]()mutable { cout << ++a; }; // 不会报错
    cout << a << endl; // 输出：123
    f(); // 输出：124
}
```

如果不想使用mutable关键字，可以采取引用方式捕获外部变量，而非传值。

### 四、Lambda表达式的参数

Lambda表达式的参数和普通函数的参数类似，那么这里为什么还要拿出来说一下呢？原因是在Lambda表达式中传递参数还有一些限制，主要有以下几点：

* 参数列表中不能有默认参数
* 不支持可变参数
* 所有参数必须有参数名