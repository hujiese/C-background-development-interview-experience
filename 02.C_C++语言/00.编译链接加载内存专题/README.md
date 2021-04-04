<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->


- [底层知识](#%E5%BA%95%E5%B1%82%E7%9F%A5%E8%AF%86)
  - [程序虚拟地址空间、栈、堆](#%E7%A8%8B%E5%BA%8F%E8%99%9A%E6%8B%9F%E5%9C%B0%E5%9D%80%E7%A9%BA%E9%97%B4%E6%A0%88%E5%A0%86)
  - [段错误](#%E6%AE%B5%E9%94%99%E8%AF%AF)
  - [编译链接](#%E7%BC%96%E8%AF%91%E9%93%BE%E6%8E%A5)
    - [1、编译链接过程](#1%E7%BC%96%E8%AF%91%E9%93%BE%E6%8E%A5%E8%BF%87%E7%A8%8B)
    - [2、目标文件](#2%E7%9B%AE%E6%A0%87%E6%96%87%E4%BB%B6)
    - [3、链接的接口—符号](#3%E9%93%BE%E6%8E%A5%E7%9A%84%E6%8E%A5%E5%8F%A3%E7%AC%A6%E5%8F%B7)
  - [Linux 的共享库（Shared Library）](#linux-%E7%9A%84%E5%85%B1%E4%BA%AB%E5%BA%93shared-library)
  - [ELF 文件格式](#elf-%E6%96%87%E4%BB%B6%E6%A0%BC%E5%BC%8F)
  - [.bss段的意义](#bss%E6%AE%B5%E7%9A%84%E6%84%8F%E4%B9%89)
- [内存泄漏与内存溢出](#%E5%86%85%E5%AD%98%E6%B3%84%E6%BC%8F%E4%B8%8E%E5%86%85%E5%AD%98%E6%BA%A2%E5%87%BA)
    - [1、概念和区别](#1%E6%A6%82%E5%BF%B5%E5%92%8C%E5%8C%BA%E5%88%AB)
    - [2、内存泄漏的分类](#2%E5%86%85%E5%AD%98%E6%B3%84%E6%BC%8F%E7%9A%84%E5%88%86%E7%B1%BB)
    - [检测内存泄漏](#%E6%A3%80%E6%B5%8B%E5%86%85%E5%AD%98%E6%B3%84%E6%BC%8F)
      - [（1）检查内存泄漏](#1%E6%A3%80%E6%9F%A5%E5%86%85%E5%AD%98%E6%B3%84%E6%BC%8F)
      - [（2）检查使用未初始化内存](#2%E6%A3%80%E6%9F%A5%E4%BD%BF%E7%94%A8%E6%9C%AA%E5%88%9D%E5%A7%8B%E5%8C%96%E5%86%85%E5%AD%98)
      - [（3）其他](#3%E5%85%B6%E4%BB%96)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## 底层知识

参考 
> https://github.com/hujiese/interview#%EF%B8%8F-%E9%93%BE%E6%8E%A5%E8%A3%85%E8%BD%BD%E5%BA%93

> https://www.nowcoder.com/tutorial/93/8f140fa03c084299a77459dc4be31c95

### 程序虚拟地址空间、栈、堆

程序的虚拟地址空间分布如下

![](https://camo.githubusercontent.com/1195e558ce80fc9016b570cbb797778b80faef0e/68747470733a2f2f692e696d6775722e636f6d2f766757375a716d2e706e67)

32bitCPU可寻址4G线性空间，每个进程都有各自独立的4G逻辑地址，其中0~3G是用户态空间，3~4G是内核空间（3G用户空间和1G内核空间）不同进程相同的逻辑地址会映射到不同的物理地址中。其逻辑地址其划分如下：

* 静态区域

    text segment(代码段):包括只读存储区和文本区，其中只读存储区存储字符串常量，文本区存储程序的机器代码。

    data segment(数据段)：存储程序中已初始化的全局变量和静态变量。

    bss segment：存储未初始化的全局变量和静态变量（局部+全局），以及所有被初始化为0的全局变量和静态变量，对于未初始化的全局变量和静态变量，程序运行main之前时会统一清零。即未初始化的全局变量编译器会初始化为0。

* 动态区域：

    heap（堆）：一般由程序员分配释放，若程序员不释放，程序结束时可能由操作系统回收，用来容纳应用程序动态分配的内存区域。当进程未调用malloc时是没有堆段的，只有调用malloc时采用分配一个堆，并且在程序运行过程中可以动态增加堆大小(移动break指针)，从低地址向高地址增长。分配小内存时使用该区域。堆的起始地址由mm_struct 结构体中的start_brk标识，结束地址由brk标识。堆分配算法有：空闲链表（Free List）、位图（Bitmap）和对象池。

    memory mapping segment(映射区)：存储动态链接库等文件映射、申请大内存（malloc时调用mmap函数）

    stack（栈）：栈保存了一个函数调用所需要的维护信息，常被称为堆栈帧（Stack Frame）或活动记录（Activate Record），使用栈空间存储函数的返回地址、参数、局部变量、临时变量（包括函数的非静态局部变量以及编译器自动生成的其他临时变量）、返回值、保存上下文（包括函数调用前后需要保持不变的寄存器）。栈从高地址向低地址增长。在创建进程时会有一个最大栈大小，Linux可以通过ulimit命令指定。

### 段错误

典型的非法指针解引用造成的错误。当指针指向一个不允许读写的内存地址，而程序却试图利用指针来读或写该地址时，会出现这个错误。

普遍原因：

* 试图修改字符串常量的内容
* 将指针初始化为 NULL，之后没有给它一个合理的值就开始使用指针
* 没用初始化栈中的指针，指针的值一般会是随机数，之后就直接开始使用指针（野指针，虽然释放了动态分配的内存，但指针的内容却没有置为nullptr，指向一个不存在的空间）

### 编译链接

#### 1、编译链接过程

各平台文件格式：

平台|	可执行文件|	目标文件|	动态库/共享对象|	静态库
---|---|---|---|---
Windows|exe|	obj|	dll	|lib
Unix/Linux|	ELF、out|	o|	so|	a

编译链接过程：

* 1、预编译（预编译器处理如 #include、#define 等预编译指令，生成 .i 或 .ii 文件）
* 2、编译（编译器进行词法分析、语法分析、语义分析、中间代码生成、目标代码生成、优化，生成 .s 文件）
* 3、汇编（汇编器把汇编码翻译成机器码，生成 .o 文件）
* 4、链接（连接器进行地址和空间分配、符号决议、重定位，生成 .out 文件）

现在版本 GCC 把预编译和编译合成一步，预编译编译程序 cc1、汇编器 as、连接器 ld。MSVC 编译环境，编译器 cl、连接器 link、可执行文件查看器 dumpbin。

#### 2、目标文件

编译器编译源代码后生成的文件叫做目标文件。目标文件从结构上讲，它是已经编译后的可执行文件格式，只是还没有经过链接的过程，其中可能有些符号或有些地址还没有被调整。

可执行文件（Windows 的 .exe 和 Linux 的 ELF）、动态链接库（Windows 的 .dll 和 Linux 的 .so）、静态链接库（Windows 的 .lib 和 Linux 的 .a）都是按照可执行文件格式存储（Windows 按照 PE-COFF，Linux 按照 ELF）。

目标文件存储结构：

段|	功能
---|---
File Header|	文件头，描述整个文件的文件属性（包括文件是否可执行、是静态链接或动态连接及入口地址、目标硬件、目标操作系统等）
.text section|	代码段，执行语句编译成的机器代码
.data section|	数据段，已初始化的全局变量和局部静态变量
.bss section|	BSS 段（Block Started by Symbol），未初始化的全局变量和局部静态变量（因为默认值为 0，所以只是在此预留位置，不占空间）
.rodata section|	只读数据段，存放只读数据，一般是程序里面的只读变量（如 const 修饰的变量）和字符串常量
.comment section|	注释信息段，存放编译器版本信息
.note.GNU-stack section|	堆栈提示段

#### 3、链接的接口—符号

在链接中，目标文件之间相互拼合实际上是目标文件之间对地址的引用，即对函数和变量的地址的引用。我们将函数和变量统称为符号（Symbol），函数名或变量名就是符号名（Symbol Name）。

如下符号表（Symbol Table）：

Symbol（符号名）|	Symbol Value （地址）
---|---
main|	0x100
Add|	0x123
...|	...

### Linux 的共享库（Shared Library）

Linux 下的共享库就是普通的 ELF 共享对象。

共享库版本更新应该保证二进制接口 ABI（Application Binary Interface）的兼容

* 命名

    例如：libname.so.x.y.z
    * x：主版本号，不同主版本号的库之间不兼容，需要重新编译
    * y：次版本号，高版本号向后兼容低版本号
    * z：发布版本号，不对接口进行更改，完全兼容

* 路径

    大部分包括 Linux 在内的开源系统遵循 FHS（File Hierarchy Standard）的标准，这标准规定了系统文件如何存放，包括各个目录结构、组织和作用。

    * /lib：存放系统最关键和最基础的共享库，如动态链接器、C 语言运行库、数学库等
    * /usr/lib：存放非系统运行时所需要的关键性的库，主要是开发库
    * /usr/local/lib：存放跟操作系统本身并不十分相关的库，主要是一些第三方应用程序的库

    动态链接器会在 /lib、/usr/lib 和由 /etc/ld.so.conf 配置文件指定的，目录中查找共享库

* 环境变量

    * LD_LIBRARY_PATH：临时改变某个应用程序的共享库查找路径，而不会影响其他应用程序
    * LD_PRELOAD：指定预先装载的一些共享库甚至是目标文件
    * LD_DEBUG：打开动态链接器的调试功能

### ELF 文件格式

参考 [ELF文件格式](./img/elf.png)

> https://www.cnblogs.com/gatsby123/p/9750187.html

### .bss段的意义

**.bss不占据实际的磁盘空间，只在段表中记录大小，在符号表中记录符号。当文件加载运行时，才分配空间以及初始化。**

早期的计算机存储设备是很贵的，而很多时候，数据段里的全局变量都是0（或者没有初始值），那么存储这么多的0到目标文件里其实是没有必要的。所以为了节约空间，在生成目标文件的时候，就把没有初始值（实际就是0）的数据段里的变量都放到BSS段里，这样目标文件就不需要那么大的体积里（节约磁盘空间）。**只有当目标文件被载入的时候，加载器负责把BSS段清零（一个循环就可以搞定）**。 之后，这个规则慢慢的成为一个标准配置，大多数编译器也就都支持了BSS段。

.data节中定义的数据项与.bss节中定义的数据项之间存在至关重要的区别：**.data节中的数据项增加了可执行文件的大小。.bss部分中的数据项没有**。可以在.bss中定义一个占用16,000字节（或更多，有时更多的字节）的缓冲区，并且几乎不增加任何内容（描述中大约50字节）。

**总之，.bss段目的是为了节省空间，因为如果所有的未初始化全局变量都放在.data段里，.data段必定很大，占用空间，这是没有必要的浪费，只需要在加载运行程序时来初始化这些变量即可。**

## 内存泄漏与内存溢出

#### 1、概念和区别

内存泄露(memory leak)是指程序在申请内存后，无法释放已申请的内存空间就造成了内存泄漏，一次内存泄漏似乎不会有大的影响，但内存泄漏堆积后的后果就是内存溢出。内存泄漏并非指内存在物理上的消失，而是应用程序分配某段内存后，由于设计错误，失去了对该段内存的控制，因而造成了内存的浪费。

内存溢出(out of memory)指程序申请内存时，没有足够的内存供申请者使用，或者说，给了你一块存储int类型数据的存储空间，但是你却存储long类型的数据，那么结果就是内存不够用，此时就会报错OOM,即所谓的内存溢出，简单来说就是自己所需要使用的空间比我们拥有的内存大内存不够使用所造成的内存溢出。

#### 2、内存泄漏的分类

* 堆内存泄漏 （Heap leak）

    对内存指的是程序运行中根据需要分配通过malloc,realloc new等从堆中分配的一块内存，再是完成后必须通过调用对应的 free或者delete 删掉。如果程序的设计的错误导致这部分内存没有被释放，那么此后这块内存将不会被使用，就会产生Heap Leak。

* 系统资源泄露（Resource Leak）

    主要指程序使用系统分配的资源比如 Bitmap,handle ,SOCKET等没有使用相应的函数释放掉，导致系统资源的浪费，严重可导致系统效能降低，系统运行不稳定。

* 没有将基类的析构函数定义为虚函数。

    当基类指针指向子类对象时，如果基类的析构函数不是virtual，那么子类的析构函数将不会被调用，子类的资源没有正确是释放，因此造成内存泄露。

#### 检测内存泄漏

##### （1）检查内存泄漏

可以使用linux环境下的内存泄漏检查工具Valgrind。这个工具可以使用apt-get安装。

下面这个代码：

```cpp
#include<iostream>
#include<stdlib.h>
using namespace std;
void func(){
    int *x=(int *)malloc( 10 * sizeof ( int ) ) ;
    x[10]=0;
}
int main(){
    func();
    cout<<"done"<<endl;
    return 0;
}
```

在编译这个程序的时候注意加上-g选项，便于查看调试信息。

这个程序犯了两大问题：

* 一个是数组越界，func函数只分配了10个int空间，x[10]是不能被访问的
* malloc动态分配内存没有释放，内存泄漏

可以使用valgrind查看诊断信息：

```shell
$ valgrind ./a.out
==28428== Memcheck, a memory error detector
==28428== Copyright (C) 2002-2013, and GNU GPL'd, by Julian Seward et al.
==28428== Using Valgrind-3.10.1 and LibVEX; rerun with -h for copyright info
==28428== Command: ./a.out
==28428== 
==28428== Invalid write of size 4
==28428==    at 0x4008BE: func() (0508test.cpp:6)
==28428==    by 0x4008CE: main (0508test.cpp:9)
==28428==  Address 0x5a1c068 is 0 bytes after a block of size 40 alloc'd
==28428==    at 0x4C2AB80: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==28428==    by 0x4008B1: func() (0508test.cpp:5)
==28428==    by 0x4008CE: main (0508test.cpp:9)
==28428== 
done
==28428== 
==28428== HEAP SUMMARY:
==28428==     in use at exit: 40 bytes in 1 blocks
==28428==   total heap usage: 1 allocs, 0 frees, 40 bytes allocated
==28428== 
==28428== LEAK SUMMARY:
==28428==    definitely lost: 40 bytes in 1 blocks
==28428==    indirectly lost: 0 bytes in 0 blocks
==28428==      possibly lost: 0 bytes in 0 blocks
==28428==    still reachable: 0 bytes in 0 blocks
==28428==         suppressed: 0 bytes in 0 blocks
==28428== Rerun with --leak-check=full to see details of leaked memory
==28428== 
==28428== For counts of detected and suppressed errors, rerun with: -v
==28428== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
```

其中：

*==28428== Invalid write of size 4* 部分显示了非法访问地址，*by 0x4008B1: func() (0508test.cpp:5)* 行号也打印出来了。此外*==28428==    definitely lost: 40 bytes in 1 blocks* 显示有40Bytes内存泄漏。

##### （2）检查使用未初始化内存

```cpp
#include<iostream>
using namespace std;
int main(){
    int a[5];
    int i,s=0;
    a[0]=a[1]=a[3]=a[4]=0;
    for(i=0;i<5;i++)
            s=s+a[i];
    if(s==33)
            cout<<"sum is 33"<<endl;
    else
            cout<<"sum is not 33"<<endl; // 输出该句
    return 0;
}
```

下面这个代码中a[2]是没有初始化的，所以才会导致这个问题，下面用valgrind查看下：

```shell
$ valgrind ./a.out
==28514== Memcheck, a memory error detector
==28514== Copyright (C) 2002-2013, and GNU GPL'd, by Julian Seward et al.
==28514== Using Valgrind-3.10.1 and LibVEX; rerun with -h for copyright info
==28514== Command: ./a.out
==28514== 
==28514== Conditional jump or move depends on uninitialised value(s)
==28514==    at 0x4008A8: main (0509test.cpp:9)
==28514== 
sum is not 33
==28514== 
==28514== HEAP SUMMARY:
==28514==     in use at exit: 0 bytes in 0 blocks
==28514==   total heap usage: 0 allocs, 0 frees, 0 bytes allocated
==28514== 
==28514== All heap blocks were freed -- no leaks are possible
==28514== 
==28514== For counts of detected and suppressed errors, rerun with: -v
==28514== Use --track-origins=yes to see where uninitialised values come from
==28514== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)
```

*==28514== Conditional jump or move depends on uninitialised value(s)*
*==28514==    at 0x4008A8: main (0509test.cpp:9)*

该句将问题诊断了。

##### （3）其他

valgrind 也可以检测内存覆盖和动态内存管理错误等问题，具体可见《后台开发 核心技术与应用实践》第5章内容。