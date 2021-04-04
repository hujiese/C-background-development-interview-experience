<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->


- [GDB调试](#gdb%E8%B0%83%E8%AF%95)
  - [一、预热](#%E4%B8%80%E9%A2%84%E7%83%AD)
  - [二、正式开始](#%E4%BA%8C%E6%AD%A3%E5%BC%8F%E5%BC%80%E5%A7%8B)
    - [1、显示文件内容命令](#1%E6%98%BE%E7%A4%BA%E6%96%87%E4%BB%B6%E5%86%85%E5%AE%B9%E5%91%BD%E4%BB%A4)
    - [2、设置断点](#2%E8%AE%BE%E7%BD%AE%E6%96%AD%E7%82%B9)
    - [3、调试命令](#3%E8%B0%83%E8%AF%95%E5%91%BD%E4%BB%A4)
  - [三、调试进程和线程](#%E4%B8%89%E8%B0%83%E8%AF%95%E8%BF%9B%E7%A8%8B%E5%92%8C%E7%BA%BF%E7%A8%8B)
  - [四、用gdb调试coredump](#%E5%9B%9B%E7%94%A8gdb%E8%B0%83%E8%AF%95coredump)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## GDB调试

**断点调试**

>  有一个程序出错了，大致猜出可能在某处会出错，就在那里下一个断点。调试程序，执行到那里，程序会停下来，这时可以检查各种变量的值，然后按步调试运行，观察程序的流向及各个变量的变化，便于快速排错。
>
> 断点是一个信号，它通知调试器，在某个特定点上暂时将程序执行挂起。当执行在某个断点处挂起时，我们称程序处于中断模式， 进入中断模式并不会终止或结束程序的执行，执行可以在任何时候继续。
>
> 软件断点在X86系统中就是指令INT 3，它的二进制代码opcode是0xCC。当程序执行到INT 3指令时，会引发软件中断。操作系统的INT 3中断处理器会寻找注册在该进程上的调试处理程序。从而像Windbg和VS等等调试器就有了上下其手的机会。

### 一、预热

首先准备好一个需要调试的文件test.c，其内容十分简单：

```c
#include <stdio.h>

int main(int argc, const char* argv[])
{
    printf("args num = %d\n", argc);
    for(int i=0; i<argc; ++i)
    {
        printf("arg%d: %s\n", i, argv[i]);
    }
    return 0;
}
```
这个文件的功能就是接受用户输入的命令行参数及其数量，并将他们全部打印出来。

现在编译这个文件，加入调试信息-g：

    gcc test.c -g -o app -std=c99
    gdb app


输出如下：

```cpp
GNU gdb (Ubuntu 7.7.1-0ubuntu5~14.04.2) 7.7.1
Copyright (C) 2014 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "x86_64-linux-gnu".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
<http://www.gnu.org/software/gdb/documentation/>.
For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from app...done.
(gdb) 
```

到此就进入 gdb 调试状态，等待参数的输入，这里我们输入参数如下：

    set args sss xxx 444

然后再输入命令

    r

是的，就一个字符'r'，意思是run，运行程序。程序运行结果如下:

```shell
(gdb) set args sss xxx 444
(gdb) r
Starting program: /mnt/hgfs/vmshared/LinuxCodeMaster/基础部分/GDB/args/app sss xxx 444
args num = 4
arg0: /mnt/hgfs/vmshared/LinuxCodeMaster/基础部分/GDB/args/app
arg1: sss
arg2: xxx
arg3: 444
[Inferior 1 (process 74564) exited normally]
(gdb) 
```

将我们的命令行参数全部打印出来了。

### 二、正式开始

接下来以一个具体的例子学习 gdb 的调试方法。首先要准备的文件都在src下，这里仅仅展示需要测试的main.c文件：

```cpp
#include <stdio.h>
#include "sort.h"

void main()
{
	int i;
	//定义整型数组
	int array[] = { 12, 5, 33, 6, 10, 35, 67, 89, 87, 65, 54, 24, 58, 92, 100, 24, 46, 78, 99, 200, 55, 44, 33, 22, 11, 71, 2, 4, 86, 8, 9 };
	int array2[] = { 12, 5, 33, 6, 10, 35, 67, 89, 87, 65, 54, 24, 58, 92, 100, 24, 46, 78, 99, 200, 55, 44, 33, 22, 11, 71, 2, 4, 86, 8, 9 };

	//计算数组长度
	int len = sizeof(array) / sizeof(int);
	//遍历数组
	printf("Sort Array:\n");
	for (i = 0; i < len; ++i)
	{
		printf("%d\t", array[i]);
	}
	printf("\n");

	// selectionSort
	selectionSort(array, len);
	// printf
	printf("Selection Sort:\n");
	for (i = 0; i < len; ++i)
	{
		printf("%d  ", array[i]);
	}
	
	// insertionSort
	insertionSort(array2, len);
	// printf
	printf("\n==========Gorgeous Split Line==========\n");
	printf("Insertion Sort:	\n");
	for (i = 0; i < len; ++i)
	{
		printf("%d  ", array2[i]);
	}
	printf("\n");
}
```
该文件定义了两个数组，然后分别先后调用sort.h中的选择排序法和插入排序法进行排序，排序结束后打印排序好的数组元素。使用Makefile编译该文件，最后得到app可执行文件，于是：

    gdb app

输出如下：

```shell
GNU gdb (Ubuntu 7.7.1-0ubuntu5~14.04.2) 7.7.1
Copyright (C) 2014 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "x86_64-linux-gnu".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
<http://www.gnu.org/software/gdb/documentation/>.
For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from app...done.
(gdb) 
```

#### 1、显示文件内容命令

输入命令'l'，gdb将显示被调试文件的内容：

```cpp
(gdb) l
1	#include <stdio.h>
2	#include "sort.h"
3	
4	void main()
5	{
6		int i;
7		//�����������
8		int array[] = { 12, 5, 33, 6, 10, 35, 67, 89, 87, 65, 54, 24, 58, 92, 100, 24, 46, 78, 99, 200, 55, 44, 33, 22, 11, 71, 2, 4, 86, 8, 9 };
9		int array2[] = { 12, 5, 33, 6, 10, 35, 67, 89, 87, 65, 54, 24, 58, 92, 100, 24, 46, 78, 99, 200, 55, 44, 33, 22, 11, 71, 2, 4, 86, 8, 9 };
10	
(gdb) 
```

输入：

    show listsize

显示gdb默认显示文件内容的行数，默认是10行：

    (gdb) show listsize
    Number of source lines gdb will list by default is 10.
    (gdb) 

于是，这里可以使用

    set listsize

命令来设置每次输出的行数：

```cpp
(gdb) set listsize 20
(gdb) l
11		//�������鳤�
12		int len = sizeof(array) / sizeof(int);
13		//�������
14		printf("Sort Array:\n");
15		for (i = 0; i < len; ++i)
16		{
17			printf("%d\t", array[i]);
18		}
19		printf("\n");
20	
21		// selectionSort
22		selectionSort(array, len);
23		// printf
24		printf("Selection Sort:\n");
25		for (i = 0; i < len; ++i)
26		{
27			printf("%d  ", array[i]);
28		}
29		
30		// insertionSort
(gdb) show listsize
Number of source lines gdb will list by default is 20.
(gdb) 
```

上面设置了输出20行内容。

接下来使用l n命令来显示某行的上下文内容，其中n是行号：

    (gdb) l 4
    1	#include <stdio.h>
    2	#include "sort.h"
    3	
    4	void main()
    5	{
    6		int i;
    7		//�����������
    8		int array[] = { 12, 5, 33, 6, 10, 35, 67, 89, 87, 65, 54, 24, 58, 92, 100, 24, 46, 78, 99, 200, 55, 44, 33, 22, 11, 71, 2, 4, 86, 8, 9 };
    9		int array2[] = { 12, 5, 33, 6, 10, 35, 67, 89, 87, 65, 54, 24, 58, 92, 100, 24, 46, 78, 99, 200, 55, 44, 33, 22, 11, 71, 2, 4, 86, 8, 9 };
    10	
    11		//�������鳤�
    12		int len = sizeof(array) / sizeof(int);
    13		//�������
    14		printf("Sort Array:\n");
    15		for (i = 0; i < len; ++i)
    16		{
    17			printf("%d\t", array[i]);
    18		}
    19		printf("\n");
    20	
    (gdb) 

如果需要查看其他文件，则：l filename:n。这里以insert_sort.c文件的第15为例：

    (gdb) l insert_sort.c:15
    5	
    6		ÿ�δ������ǽ��������еĵ�һ��Ԫ�����������
    7		��Ԫ�شӺ���ǰ�������бȽϣ��ҳ�����λ�ã���
    8		��Ԫ�ز��뵽�������еĺ���λ���С�
    9	
    10		�ȶ���:�����������ȶ��
    11	
    12	***********************************************/
    13	
    14	//���������㷨(��������)
    15	void insertionSort(int *array, int len)
    16	{
    17		int tmp = 0;	// �洢��׼�
    18		int index = 0;	// �ӵ�λ�
    19		// �����������
    20		for (int i = 1; i < len; ++i)
    21		{
    22			index = i;
    23			tmp = array[i];
    24			// ������������(�Ӻ���ǰ)
    (gdb) 

或者直接使用l filename:function_name来显示某函数的上下文内容：

    (gdb) l insert_sort.c:insertionSort
    6		ÿ�δ������ǽ��������еĵ�һ��Ԫ�����������
    7		��Ԫ�شӺ���ǰ�������бȽϣ��ҳ�����λ�ã���
    8		��Ԫ�ز��뵽�������еĺ���λ���С�
    9	
    10		�ȶ���:�����������ȶ��
    11	
    12	***********************************************/
    13	
    14	//���������㷨(��������)
    15	void insertionSort(int *array, int len)
    16	{
    17		int tmp = 0;	// �洢��׼�
    18		int index = 0;	// �ӵ�λ�
    19		// �����������
    20		for (int i = 1; i < len; ++i)
    21		{
    22			index = i;
    23			tmp = array[i];
    24			// ������������(�Ӻ���ǰ)
    25			for (int j = i - 1; j >= 0; --j)
    (gdb) 

#### 2、设置断点

输入：b n即可设置断点：

    (gdb) b 12
    Breakpoint 1 at 0x4008b0: file main.c, line 12.
    (gdb) b 14
    Breakpoint 2 at 0x4008ba: file main.c, line 14.
    (gdb) b 15
    Breakpoint 3 at 0x4008c4: file main.c, line 15.
    (gdb) b 17
    Breakpoint 4 at 0x4008d0: file main.c, line 17.
    (gdb) b 19
    Breakpoint 5 at 0x400905: file main.c, line 19.
    (gdb) b 24
    Breakpoint 6 at 0x400926: file main.c, line 24.
    (gdb) b 27
    Breakpoint 7 at 0x40093c: file main.c, line 27.
    (gdb) 

上面的操作设置了多个断点，接下来输入i(info) b(break)可以查看设置的断点：

    (gdb) i b
    Num     Type           Disp Enb Address            What
    1       breakpoint     keep y   0x00000000004008b0 in main at main.c:12
    2       breakpoint     keep y   0x00000000004008ba in main at main.c:14
    3       breakpoint     keep y   0x00000000004008c4 in main at main.c:15
    4       breakpoint     keep y   0x00000000004008d0 in main at main.c:17
    5       breakpoint     keep y   0x0000000000400905 in main at main.c:19
    6       breakpoint     keep y   0x0000000000400926 in main at main.c:24
    7       breakpoint     keep y   0x000000000040093c in main at main.c:27
    (gdb) 

接下来用 d(del)命令删除断点：

    (gdb) d 1
    (gdb) i b
    Num     Type           Disp Enb Address            What
    2       breakpoint     keep y   0x00000000004008ba in main at main.c:14
    3       breakpoint     keep y   0x00000000004008c4 in main at main.c:15
    4       breakpoint     keep y   0x00000000004008d0 in main at main.c:17
    5       breakpoint     keep y   0x0000000000400905 in main at main.c:19
    6       breakpoint     keep y   0x0000000000400926 in main at main.c:24
    7       breakpoint     keep y   0x000000000040093c in main at main.c:27
    (gdb) d 2 3
    (gdb) i b
    Num     Type           Disp Enb Address            What
    4       breakpoint     keep y   0x00000000004008d0 in main at main.c:17
    5       breakpoint     keep y   0x0000000000400905 in main at main.c:19
    6       breakpoint     keep y   0x0000000000400926 in main at main.c:24
    7       breakpoint     keep y   0x000000000040093c in main at main.c:27
    (gdb) 

可以来用 dis 命令无效化断点：

    (gdb) dis 4 5
    (gdb) i b
    Num     Type           Disp Enb Address            What
    4       breakpoint     keep n   0x00000000004008d0 in main at main.c:17
    5       breakpoint     keep n   0x0000000000400905 in main at main.c:19
    6       breakpoint     keep y   0x0000000000400926 in main at main.c:24
    7       breakpoint     keep y   0x000000000040093c in main at main.c:27
    (gdb) 

用 ena 命令有效化断点：

    (gdb) ena 4 5
    (gdb) i b
    Num     Type           Disp Enb Address            What
    4       breakpoint     keep y   0x00000000004008d0 in main at main.c:17
    5       breakpoint     keep y   0x0000000000400905 in main at main.c:19
    6       breakpoint     keep y   0x0000000000400926 in main at main.c:24
    7       breakpoint     keep y   0x000000000040093c in main at main.c:27
    (gdb) 

至此，关于断点的一些操作就告一段落。

#### 3、调试命令

退出前面的测试，重新对app进行gdb调试，重新打断点，这里的断点是第17行：

    (gdb) l 17
    12		int len = sizeof(array) / sizeof(int);
    13		//�������
    14		printf("Sort Array:\n");
    15		for (i = 0; i < len; ++i)
    16		{
    17			printf("%d\t", array[i]);
    18		}
    19		printf("\n");
    20	
    21		// selectionSort
    (gdb) b 17
    Breakpoint 1 at 0x4008d0: file main.c, line 17.

接着使用r命令运行程序，接着 p i 命令查看 i 的当前值，使用p array[i]查看array的当前元素是哪个：

    (gdb) r
    Starting program: /mnt/hgfs/vmshared/LinuxCodeMaster/基础部分/GDB/app 
    Sort Array:
    
    Breakpoint 1, main () at main.c:17
    17			printf("%d\t", array[i]);
    (gdb) p i
    $1 = 0
    (gdb) p array[i]
    $2 = 12
    (gdb) 

条件断点是当满足一定条件时才会触发的断点，比较适合进行异常排查。所以可以设置i的值到循环到某一特定值时程序的状态，这里是b 17 if i == 10：

    (gdb) b 17 if i == 10
    Breakpoint 1 at 0x4008d0: file main.c, line 17.
    (gdb) r
    Starting program: /mnt/hgfs/vmshared/LinuxCodeMaster/基础部分/GDB/app 
    Sort Array:
    
    Breakpoint 1, main () at main.c:17
    17			printf("%d\t", array[i]);
    (gdb) c
    Continuing.
    12	5	33	6	10	35	67	89	87	65	54	24	58	92	100	24	46	78	99	200	55	44	33	22	11	71	24	86	8	9	
    Selection Sort:
    2  4  5  6  8  9  10  11  12  22  24  24  33  33  35  44  46  54  55  58  65  67  71  78  86  87  89  92  99  100  200  
    ==========Gorgeous Split Line==========
    Insertion Sort:	
    2  4  5  6  8  9  10  11  12  22  24  24  33  33  35  44  46  54  55  58  65  67  71  78  86  87  89  92  99  100  200  
    [Inferior 1 (process 75153) exited with code 012]
    (gdb) p i
    No symbol "i" in current context.
    (gdb) r
    Starting program: /mnt/hgfs/vmshared/LinuxCodeMaster/基础部分/GDB/app 
    Sort Array:
    
    Breakpoint 1, main () at main.c:17
    17			printf("%d\t", array[i]);
    (gdb) p i
    $1 = 10
    (gdb) p array[i]
    $2 = 54
    (gdb) 

可以使用使用 ptype 查看变量类型：

    (gdb) ptype i
    type = int
    (gdb) ptype array
    type = int [31]
    (gdb) 

使用 n 和 display 命令单步执行程序并查看变量内容：

    (gdb) n
    15		for (i = 0; i < len; ++i)
    (gdb) n
    
    Breakpoint 1, main () at main.c:17
    17			printf("%d\t", array[i]);
    (gdb) display i
    1: i = 1
    (gdb) n
    15		for (i = 0; i < len; ++i)
    1: i = 1
    (gdb) n
    
    Breakpoint 1, main () at main.c:17
    17			printf("%d\t", array[i]);
    1: i = 2
    (gdb) display i
    2: i = 2
    (gdb) display array[i]
    3: array[i] = 33
    (gdb) 

用 i display 查看打印变量：

    (gdb) i display
    Auto-display expressions now in effect:
    Num Enb Expression
    3:   y  array[i]
    2:   y  i
    1:   y  i

用 undisplay 取消打印:

    (gdb) undisplay 1
    (gdb) undisplay 2
    (gdb) n
    15		for (i = 0; i < len; ++i)
    3: array[i] = 33
    (gdb) i display
    Auto-display expressions now in effect:
    Num Enb Expression
    3:   y  array[i]
    (gdb) 

这样每次都只打印array[i]的值，i没有被打印。

接下来使用 c 命令 continue 下去：

    (gdb) c
    Continuing.
    
    Breakpoint 1, main () at main.c:17
    17			printf("%d\t", array[i]);
    3: array[i] = 6


使用 s(step)跳到调到函数内部，finish 跳转回去：

    (gdb) s
    __printf (format=0x400b64 "%d\t") at printf.c:28
    28	printf.c: No such file or directory.
    (gdb) finish
    Run till exit from #0  __printf (format=0x400b64 "%d\t") at printf.c:28
    main () at main.c:15
    15		for (i = 0; i < len; ++i)
    3: array[i] = 6
    Value returned is $1 = 2
    (gdb) 


至此，gdb调试就告一段落了。主要是介绍了如何设置断点、单步运行和查看变量值。

### 三、调试进程和线程

来自 [gdb调试多进程与多线程](https://blog.csdn.net/snow_5288/article/details/72982594?depth_1-utm_source=distribute.pc_relevant.none-task&utm_source=distribute.pc_relevant.none-task)

![](./img/gdb.png)

### 四、用gdb调试coredump

下面这个程序向一个未初始化的指针指向的地址空间（该地址空间未知）中写入一个数，于是发生了coredump：

```c
#include<stdio.h>
int main(void)
{
    int* num;
    printf("---------------\n");
    *num = 10;
    printf("%d/n", *num);
    return 0;
}
```

执行结果如下：

```shell
Segmentation fault (core dumped)
```

使用gdb调试没有用：

```shell
Type "apropos word" to search for commands related to "word"...
Reading symbols from app...(no debugging symbols found)...done.
(gdb) l
No symbol table is loaded.  Use the "file" command.
(gdb) 
```

无法加载符号表，无法调试该程序。

这里需要使用**coredump文件来结合gdb**调试，首先要开启生成coredump文件：

```shell
ulimit -c unlimited
```

默认情况下是在当前运行目录下生成该文件。接下来就是编译了，上面那个程序需要加入-g调试选项，不然接下来调试时看不到信息：

```shell
gcc coredump.c -o app -g
```

最后执行gdb：

```shell
$ gdb app core
GNU gdb (Ubuntu 7.7.1-0ubuntu5~14.04.2) 7.7.1
Copyright (C) 2014 Free Software Foundation, Inc.
...
Type "apropos word" to search for commands related to "word"...
Reading symbols from app...done.

warning: exec file is newer than core file.
[New LWP 25008]
Core was generated by `./app'.
Program terminated with signal SIGSEGV, Segmentation fault.
#0  0x0000000000400593 in main () at coredump.c:14
14	    *num = 10;
(gdb) 
```

这里一下就定位到了第14行出错，非常厉害。

**除此之外，还有一种core dump错误只使用上面的方法是检查不到的： *munmap_chunk(): invalid pointer: 0x0000000000a3a018***

看下面这段代码：

```cpp
class Foo
{
public:
    Foo(){ cout << "call ctor" << endl; }
    ~Foo(){ cout << "call dtor" << endl; }
};

int main(void)
{
    Foo* array = new Foo[10];
    delete array;
    return 0;
}
```

使用了delete释放一个动态数组的内存，这将导致core dump。下面如果直接使用上面方法调试输出结果如下：

```shell
warning: exec file is newer than core file.
[New LWP 49729]
Core was generated by `./del'.
Program terminated with signal SIGABRT, Aborted.
#0  0x00007f3d4e036c37 in __GI_raise (sig=sig@entry=6) at ../nptl/sysdeps/unix/sysv/linux/raise.c:56
56	../nptl/sysdeps/unix/sysv/linux/raise.c: No such file or directory.
```

这时需要使用bt命令来打印调用栈：

```shell
(gdb) bt
#0  0x00007f3d4e036c37 in __GI_raise (sig=sig@entry=6) at ../nptl/sysdeps/unix/sysv/linux/raise.c:56
#1  0x00007f3d4e03a028 in __GI_abort () at abort.c:89
#2  0x00007f3d4e0732a4 in __libc_message (do_abort=do_abort@entry=1, 
    fmt=fmt@entry=0x7f3d4e1816b0 "*** Error in `%s': %s: 0x%s ***\n") at ../sysdeps/posix/libc_fatal.c:175
#3  0x00007f3d4e07dff7 in malloc_printerr (action=<optimized out>, 
    str=0x7f3d4e181a30 "munmap_chunk(): invalid pointer", ptr=<optimized out>) at malloc.c:4996
#4  0x0000000000400a96 in main () at delete2.cpp:23
```

错误便可定位。像这类内存相关的错误，最好还是使用valgrind工具来检测。

除此之外，请参考《后台开发 核心技术与事件》该书相关章节。