<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->

- [STL -- string](#stl----string)
  - [1、概念](#1%E6%A6%82%E5%BF%B5)
  - [2、构造函数](#2%E6%9E%84%E9%80%A0%E5%87%BD%E6%95%B0)
  - [3、存取字符操作](#3%E5%AD%98%E5%8F%96%E5%AD%97%E7%AC%A6%E6%93%8D%E4%BD%9C)
  - [4、string 转 const char*](#4string-%E8%BD%AC-const-char)
  - [5、把string拷贝到char*指向的内存空间](#5%E6%8A%8Astring%E6%8B%B7%E8%B4%9D%E5%88%B0char%E6%8C%87%E5%90%91%E7%9A%84%E5%86%85%E5%AD%98%E7%A9%BA%E9%97%B4)
  - [6、获取string 长度](#6%E8%8E%B7%E5%8F%96string-%E9%95%BF%E5%BA%A6)
  - [7、赋值操作](#7%E8%B5%8B%E5%80%BC%E6%93%8D%E4%BD%9C)
  - [8、字符串拼接](#8%E5%AD%97%E7%AC%A6%E4%B8%B2%E6%8B%BC%E6%8E%A5)
  - [9、字符串比较](#9%E5%AD%97%E7%AC%A6%E4%B8%B2%E6%AF%94%E8%BE%83)
  - [10、获取字串](#10%E8%8E%B7%E5%8F%96%E5%AD%97%E4%B8%B2)
  - [11、查找 和 替换](#11%E6%9F%A5%E6%89%BE-%E5%92%8C-%E6%9B%BF%E6%8D%A2)
  - [12、区间删除和插入](#12%E5%8C%BA%E9%97%B4%E5%88%A0%E9%99%A4%E5%92%8C%E6%8F%92%E5%85%A5)
  - [13、算法相关](#13%E7%AE%97%E6%B3%95%E7%9B%B8%E5%85%B3)
  - [14、使用案例](#14%E4%BD%BF%E7%94%A8%E6%A1%88%E4%BE%8B)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## STL -- string

### 1、概念

string是STL的字符串类型，通常用来表示字符串。而在使用string之前，字符串通常是用char*表示的。string与char*都可以用来表示字符串，那么二者有什么区别呢。

下面是string和char*的比较：

* string是一个类, char*是一个指向字符的指针。
* string封装了char*，管理这个字符串，是一个char*型的容器。
* string不用考虑内存释放和越界。
* string管理char*所分配的内存。每一次string的复制，取值都由string类负责维护，不用担心复制越界和取值越界等。
* string提供了一系列的字符串操作函数（这个等下会详讲），例如查找find，拷贝copy，删除erase，替换replace，插入insert。

### 2、构造函数

* 默认构造函数：
    ```c
    string();	   //构造一个空的字符串string s1。
    ```
* 拷贝构造函数：
    ```c
    string(const string &str);	//构造一个与str一样的string。如string s1(s2)。
    ```
* 带参数的构造函数:
    ```c
    string(const char *s);    //用字符串s初始化
    string(int n,char c);    //用n个字符c初始化
    ```

### 3、存取字符操作

* string类的字符操作：
    ```c
    const char &operator[] (int n) const;
    const char &at(int n) const;
    char &operator[] (int n);
    char &at(int n);
    ```
其中operator[]和at()均返回当前字符串中第n个字符，但二者是有区别的。主要区别在于at()在越界时会抛出异常，[]在刚好越界时会返回(char)0，再继续越界时，编译器直接出错。如果你的程序希望可以通过try,catch捕获异常，建议采用at()。

### 4、string 转 const char*
```c
const char *c_str() const;   //返回一个以'\0'结尾的字符串的首地址
```

### 5、把string拷贝到char*指向的内存空间
```c
int copy(char *s, int n, int pos=0) const;  
```
该函数把当前串中以pos开始的n个字符拷贝到以s为起始位置的字符数组中，返回实际拷贝的数目。注意要保证s所指向的空间足够大以容纳当前字符串，不然会越界。

### 6、获取string 长度

```c
int length() const;   //返回当前字符串的长度。长度不包括字符串结尾的'\0'。
bool empty() const;     //当前字符串是否为空
```

### 7、赋值操作

```c
string &operator=(const string &s);//把字符串s赋给当前的字符串
string &assign(const char *s); //把字符串s赋给当前的字符串
string &assign(const char *s, int n); //把字符串s的前n个字符赋给当前的字符串
string &assign(const string &s);  //把字符串s赋给当前字符串
string &assign(int n,char c);  //用n个字符c赋给当前字符串
string &assign(const string &s,int start, int n);  //把字符串s中从start开始的n个字符赋给当前字符串
```

### 8、字符串拼接
```c
string &operator+=(const string &s);  //把字符串s连接到当前字符串结尾
string &operator+=(const char *s);//把字符串s连接到当前字符串结尾
string &append(const char *s);    //把字符串s连接到当前字符串结尾
string &append(const char *s,int n);  //把字符串s的前n个字符连接到当前字符串结尾
string &append(const string &s);   //同operator+=()
string &append(const string &s,int pos, int n);//把字符串s中从pos开始的n个字符连接到当前字符串结尾
string &append(int n, char c);   //在当前字符串结尾添加n个字符c
```
### 9、字符串比较
```c
int compare(const string &s) const;  //与字符串s比较
int compare(const char *s) const;   //与字符串s比较
```
compare函数在>时返回 1，<时返回 -1，==时返回 0。比较区分大小写，比较时参考字典顺序，排越前面的越小。大写的A比小写的a小。

### 10、获取字串

```c
string substr(int pos=0, int n=npos) const;    //返回由pos开始的n个字符组成的子字符串
```

### 11、查找 和 替换

* 查找

```c
int find(char c,int pos=0) const;  //从pos开始查找字符c在当前字符串的位置 
int find(const char *s, int pos=0) const;  //从pos开始查找字符串s在当前字符串的位置
int find(const string &s, int pos=0) const;  //从pos开始查找字符串s在当前字符串中的位置
find函数如果查找不到，就返回-1
int rfind(char c, int pos=npos) const;   //从pos开始从后向前查找字符c在当前字符串中的位置 
int rfind(const char *s, int pos=npos) const;
int rfind(const string &s, int pos=npos) const;
//rfind是反向查找的意思，如果查找不到， 返回-1
```

* 替换

```c
string &replace(int pos, int n, const char *s);//删除从pos开始的n个字符，然后在pos处插入串s
string &replace(int pos, int n, const string &s);  //删除从pos开始的n个字符，然后在pos处插入串s
void swap(string &s2);    //交换当前字符串与s2的值
```

### 12、区间删除和插入

```c
string &insert(int pos, const char *s);
string &insert(int pos, const string &s);
//前两个函数在pos位置插入字符串s
string &insert(int pos, int n, char c);  //在pos位置 插入n个字符c
string &erase(int pos=0, int n=npos);  //删除pos开始的n个字符，返回修改后的字符串
```

### 13、算法相关

```c
string s2 = "AAAbbb";
transform(s2.begin(), s2.end(), s2.begin(), toupper);
cout << s2 << endl;

string s3 = "AAAbbb";
transform(s3.begin(), s3.end(), s3.begin(), tolower);
cout << s3 << endl;
```

### 14、使用案例

详见src下源码。