## Atomic

更多内容可见 [muduo源码分析--Automic原子操作](https://github.com/hujiese/Large-concurrent-serve/blob/master/06_muduo_Atomic/muduo_Atomic.md)

在多线程环境中访问临界区需要做到同步，同步可以通过加锁实现，然而锁是高并发服务器的一大杀手，所以这里可以考虑使用更加高效的方法，就是原子操作。

### 一、gcc原子性操作

```c
// 原子自增操作
type __sync_fetch_and_add (type *ptr, type value)

// 原子比较和交换（设置）操作
type __sync_val_compare_and_swap (type *ptr, type oldval, type newval)
bool __sync_bool_compare_and_swap (type *ptr, type oldval, type newval)

// 原子赋值操作
type __sync_lock_test_and_set (type *ptr, type value)
```

使用这些原子性操作函数，编译的时候需要加-march=cpu-type选项。

### 二、volatile 关键字

volatile的作用： 作为指令关键字，确保本条指令不会因编译器的优化而省略，且要求每次直接读值。简单地说就是防止编译器对代码进行优化

当要求使用volatile 声明的变量的值的时候，系统总是重新从它所在的内存读取数据，而不是使用保存在寄存器中的备份。即使它前面的指令刚刚从该处读取过数据。而且读取的数据立刻被保存。

多线程下寄存器内的值可能为多个线程共享，可能会发生改变。

### 三、muduo编译选项

* -Wall // 大部分警告
* -Wextra // 一些额外的警告
* -Werror // 当出现警告时转为错误，停止编译
* -Wconversion // 一些可能改变值的隐式转换，给出警告。
* -Wno-unused-parameter // 函数中出现未使用的参数，不给出警告。
* -Wold-style-cast // C风格的转换，给出警告
* -Woverloaded-virtual // 如果函数的声明隐藏住了基类的虚函数，就给出警告。
* -Wpointer-arith // 对函数指针或者void *类型的指针进行算术操作时给出警告
* -Wshadow // 当一个局部变量遮盖住了另一个局部变量，或者全局变量时，给出警告。
* -Wwrite-strings // 规定字符串常量的类型是const char[length],因此,把这样的地址复制给 non-const char* 指针将产生警告.这些警告能够帮助你在编译期间发现企图写入字符串常量 的代码
* -march=native // 指定cpu体系结构为本地平台

### 四、AtomicIntegerT类

该类是muduo的原子操作类，该类封装了一系列原子操作函数，该文件与base/Atomic.h中定义，其主要内容如下：

![](https://camo.githubusercontent.com/27b42484104a67cf0b32f312ffd4d7ba5320e9b7/68747470733a2f2f692e696d6775722e636f6d2f4b536f704c74742e706e67)


