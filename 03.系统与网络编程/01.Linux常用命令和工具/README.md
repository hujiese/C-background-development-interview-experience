<!-- START doctoc generated TOC please keep comment here to allow auto update -->
<!-- DON'T EDIT THIS SECTION, INSTEAD RE-RUN doctoc TO UPDATE -->


- [Linux常用工具和命令](#linux%E5%B8%B8%E7%94%A8%E5%B7%A5%E5%85%B7%E5%92%8C%E5%91%BD%E4%BB%A4)
  - [1、回到上一个目录](#1%E5%9B%9E%E5%88%B0%E4%B8%8A%E4%B8%80%E4%B8%AA%E7%9B%AE%E5%BD%95)
  - [2、find 文件查找](#2find-%E6%96%87%E4%BB%B6%E6%9F%A5%E6%89%BE)
    - [查找名为timelog.txt的文件：](#%E6%9F%A5%E6%89%BE%E5%90%8D%E4%B8%BAtimelogtxt%E7%9A%84%E6%96%87%E4%BB%B6)
    - [查找叫http的目录](#%E6%9F%A5%E6%89%BE%E5%8F%ABhttp%E7%9A%84%E7%9B%AE%E5%BD%95)
    - [查找名为test的目录或文件](#%E6%9F%A5%E6%89%BE%E5%90%8D%E4%B8%BAtest%E7%9A%84%E7%9B%AE%E5%BD%95%E6%88%96%E6%96%87%E4%BB%B6)
    - [查找大于20M的文件](#%E6%9F%A5%E6%89%BE%E5%A4%A7%E4%BA%8E20m%E7%9A%84%E6%96%87%E4%BB%B6)
    - [根据文件名查找文件，忽略大小写](#%E6%A0%B9%E6%8D%AE%E6%96%87%E4%BB%B6%E5%90%8D%E6%9F%A5%E6%89%BE%E6%96%87%E4%BB%B6%E5%BF%BD%E7%95%A5%E5%A4%A7%E5%B0%8F%E5%86%99)
  - [3、grep 命令](#3grep-%E5%91%BD%E4%BB%A4)
    - [查找"node"字符串](#%E6%9F%A5%E6%89%BEnode%E5%AD%97%E7%AC%A6%E4%B8%B2)
    - [查找"main"字符串前后行内容](#%E6%9F%A5%E6%89%BEmain%E5%AD%97%E7%AC%A6%E4%B8%B2%E5%89%8D%E5%90%8E%E8%A1%8C%E5%86%85%E5%AE%B9)
  - [4、awk 命令](#4awk-%E5%91%BD%E4%BB%A4)
    - [语法](#%E8%AF%AD%E6%B3%95)
    - [每行按空格或TAB分割，输出文本中的1、4项](#%E6%AF%8F%E8%A1%8C%E6%8C%89%E7%A9%BA%E6%A0%BC%E6%88%96tab%E5%88%86%E5%89%B2%E8%BE%93%E5%87%BA%E6%96%87%E6%9C%AC%E4%B8%AD%E7%9A%8414%E9%A1%B9)
    - [使用","分割](#%E4%BD%BF%E7%94%A8%E5%88%86%E5%89%B2)
    - [使用多个分隔符.先使用空格分割，然后对分割结果再使用","分割](#%E4%BD%BF%E7%94%A8%E5%A4%9A%E4%B8%AA%E5%88%86%E9%9A%94%E7%AC%A6%E5%85%88%E4%BD%BF%E7%94%A8%E7%A9%BA%E6%A0%BC%E5%88%86%E5%89%B2%E7%84%B6%E5%90%8E%E5%AF%B9%E5%88%86%E5%89%B2%E7%BB%93%E6%9E%9C%E5%86%8D%E4%BD%BF%E7%94%A8%E5%88%86%E5%89%B2)
    - [awk -v  # 设置变量](#awk--v---%E8%AE%BE%E7%BD%AE%E5%8F%98%E9%87%8F)
  - [5、sed命令](#5sed%E5%91%BD%E4%BB%A4)
    - [语法](#%E8%AF%AD%E6%B3%95-1)
    - [在testfile文件的第四行后添加一行](#%E5%9C%A8testfile%E6%96%87%E4%BB%B6%E7%9A%84%E7%AC%AC%E5%9B%9B%E8%A1%8C%E5%90%8E%E6%B7%BB%E5%8A%A0%E4%B8%80%E8%A1%8C)
  - [6、ps 命令](#6ps-%E5%91%BD%E4%BB%A4)
    - [语法](#%E8%AF%AD%E6%B3%95-2)
    - [ps -ef](#ps--ef)
    - [ps aux](#ps-aux)
    - [ps 结合管道](#ps-%E7%BB%93%E5%90%88%E7%AE%A1%E9%81%93)
    - [按照内存使用大小对进程排序](#%E6%8C%89%E7%85%A7%E5%86%85%E5%AD%98%E4%BD%BF%E7%94%A8%E5%A4%A7%E5%B0%8F%E5%AF%B9%E8%BF%9B%E7%A8%8B%E6%8E%92%E5%BA%8F)
  - [7、lsof 查看端口占用情况](#7lsof-%E6%9F%A5%E7%9C%8B%E7%AB%AF%E5%8F%A3%E5%8D%A0%E7%94%A8%E6%83%85%E5%86%B5)
  - [8、netstat 命令](#8netstat-%E5%91%BD%E4%BB%A4)
  - [9、GDB调试程序的使用](#9gdb%E8%B0%83%E8%AF%95%E7%A8%8B%E5%BA%8F%E7%9A%84%E4%BD%BF%E7%94%A8)
  - [10、查看程序运行所需的共享库：ldd命令](#10%E6%9F%A5%E7%9C%8B%E7%A8%8B%E5%BA%8F%E8%BF%90%E8%A1%8C%E6%89%80%E9%9C%80%E7%9A%84%E5%85%B1%E4%BA%AB%E5%BA%93ldd%E5%91%BD%E4%BB%A4)
  - [11、查看cpu使用情况：top命令](#11%E6%9F%A5%E7%9C%8Bcpu%E4%BD%BF%E7%94%A8%E6%83%85%E5%86%B5top%E5%91%BD%E4%BB%A4)
  - [12、查看内存空间、磁盘空间(free，df -h)](#12%E6%9F%A5%E7%9C%8B%E5%86%85%E5%AD%98%E7%A9%BA%E9%97%B4%E7%A3%81%E7%9B%98%E7%A9%BA%E9%97%B4freedf--h)
  - [13、ethtool 和 iperf 探查网络数据带宽](#13ethtool-%E5%92%8C-iperf-%E6%8E%A2%E6%9F%A5%E7%BD%91%E7%BB%9C%E6%95%B0%E6%8D%AE%E5%B8%A6%E5%AE%BD)
  - [14、pkill 杀死指定名称的进程](#14pkill-%E6%9D%80%E6%AD%BB%E6%8C%87%E5%AE%9A%E5%90%8D%E7%A7%B0%E7%9A%84%E8%BF%9B%E7%A8%8B)
  - [15、strace 命令](#15strace-%E5%91%BD%E4%BB%A4)
  - [16、ulimit 命令](#16ulimit-%E5%91%BD%E4%BB%A4)
  - [17、valgrind 工具](#17valgrind-%E5%B7%A5%E5%85%B7)
  - [18、raceroute的过程](#18raceroute%E7%9A%84%E8%BF%87%E7%A8%8B)
  - [19、dstat 工具](#19dstat-%E5%B7%A5%E5%85%B7)
  - [20、ipcs/ipcrm 命令](#20ipcsipcrm-%E5%91%BD%E4%BB%A4)
  - [21、Linux 性能分析工具gprof](#21linux-%E6%80%A7%E8%83%BD%E5%88%86%E6%9E%90%E5%B7%A5%E5%85%B7gprof)
  - [22、pstack检测死锁](#22pstack%E6%A3%80%E6%B5%8B%E6%AD%BB%E9%94%81)

<!-- END doctoc generated TOC please keep comment here to allow auto update -->

## Linux常用工具和命令

更多参考 [Linux 命令大全](https://www.runoob.com/linux/linux-command-manual.html)

### 1、回到上一个目录

```shell
cd -
```

### 2、find 文件查找

参考 [linux find命令使用](https://www.cnblogs.com/hb91/p/9984655.html)

命令格式：find  (目录)   [-type d | f]  (文件夹 | 文件)   -name   （名称，可使用正则表达式）

假设当前目录结构如下：

```shell
$ ls -l
total 9896
drwxrwxr-x 15 jack jack    4096  3月 23  2019 cmake-3.11.3
-rw-rw-r--  1 jack jack 7954155  6月  1  2018 cmake-3.11.3.tar.gz
drwxr-xr-x  2 jack jack    4096  8月 31  2018 Desktop
drwxr-xr-x  2 jack jack    4096  8月 31  2018 Documents
drwxr-xr-x  2 jack jack    4096  2月  3  2019 Downloads
-rw-r--r--  1 jack jack    8980  8月 31  2018 examples.desktop
drwxrwxr-x  4 jack jack    4096  7月 17 21:56 http
-rw-rw-r--  1 jack jack     652 10月  7 21:36 test.c
drwxrwxr-x  2 jack jack    4096  9月 30  2018 test_muduo
-rw-r--r--  1 jack jack    3848 11月 10 14:54 timelog.txt
drwxr-xr-x  2 jack jack    4096  8月 31  2018 Videos
drwxrwxr-x 23 jack jack    4096 11月  8 14:33 workspace
drwxrwxr-x 10 jack jack    4096 10月  7 21:33 zeromq-3.2.5
-rwxrwxr-x  1 jack jack 2082960 10月  7 21:33 zeromq-3.2.5.tar.gz
```

#### 查找名为timelog.txt的文件：

```shell
$ sudo find . -type f -name timelog.txt
./timelog.txt
```

#### 查找叫http的目录

```shell
$ sudo find . -type d -name http
./http
./http/httpserver/http
./http/http_build/debug/http
./workspace/httpserver/http
./workspace/hurricane/deps/meshy/src/http
```

#### 查找名为test的目录或文件
```shell
$ sudo find . -name test
./.vim/bundle/vundle/test
./.vim/bundle/ctrlp-funky/test
./.vim/ftplugin/python/pyflakes/pyflakes/test
./workspace/LinuxCodeMaster/网络编程/02_mul_sock/mult_process_concurrent/test
./workspace/LinuxCodeMaster/网络编程/01_tcp_socket/test
```

#### 查找大于20M的文件

```shell
$ sudo find . -size +20M
./software/hadoop-2.6.5.tar.gz
./software/jdk-8u191-linux-x64.tar.gz
./.vim/bundle/command-t/data/benchmark.yml
./cmake-3.11.3/Source/libCMakeLib.a
```

#### 根据文件名查找文件，忽略大小写

```shell
$ sudo find . -type d -iname test
./.vim/bundle/vundle/test
./.vim/bundle/ctrlp-funky/test
./.vim/ftplugin/python/pyflakes/pyflakes/test
./workspace/hurricane/deps/logging/test
./workspace/recipes-master/thread/test
./cmake-3.11.3/Tests/FindGTest/Test
./cmake-3.11.3/Utilities/KWIML/test
```

### 3、grep 命令

参考 [linux中grep命令的用法](https://www.cnblogs.com/flyor/p/6411140.html)

grep家族总共有三个：grep，egrep，fgrep，其常用选项有：

* -E ：开启扩展（Extend）的正则表达式。
* -i ：忽略大小写（ignore case）。
* -v ：反过来（invert），只打印没有匹配的，而匹配的反而不打印。
* -n ：显示行号
* -w ：被匹配的文本只能是单词，而不能是单词中的某一部分，如文本中有liker，而我搜寻的只是like，就可以使用-w选项来避免匹配* liker
* -c ：显示总共有多少行被匹配到了，而不是显示被匹配到的内容，注意如果同时使用-cv选项是显示有多少行没有被匹配到。
* -o ：只显示被模式匹配到的字符串。
* --color :将匹配到的内容以颜色高亮显示。
* -A  n：显示匹配到的字符串所在的行及其后n行，after
* -B  n：显示匹配到的字符串所在的行及其前n行，before
* -C  n：显示匹配到的字符串所在的行及其前后各n行，context

后续将以condition.c为例来使用这些命令。

#### 查找"node"字符串

```shell
$ grep "node" ./conditon.c 
typedef struct node
    struct node* next;
```

#### 查找"main"字符串前后行内容

```shell
$ grep -A 2 "main" ./conditon.c 
int main(int argc, const char* argv[])
{
    pthread_t p1, p2;
$ grep -B 2 "main" ./conditon.c 
}

int main(int argc, const char* argv[])
$ grep -C 2 "main" ./conditon.c 
}

int main(int argc, const char* argv[])
{
    pthread_t p1, p2;
```

### 4、awk 命令

参考 [Linux awk 命令](https://www.runoob.com/linux/linux-comm-awk.html)

AWK是一种处理文本文件的语言，是一个强大的文本分析工具。

#### 语法

```shell
awk [选项参数] 'script' var=value file(s)
或
awk [选项参数] -f scriptfile var=value file(s)
```

选项参数说明：

* -F fs or --field-separator fs 

    指定输入文件折分隔符，fs是一个字符串或者是一个正则表达式，如-F:
* -v var=value or --asign var=value

    赋值一个用户定义变量。
* -f scripfile or --file scriptfile

    从脚本文件中读取awk命令。
* -mf nnn and -mr nnn

    对nnn值设置内在限制，-mf选项限制分配给nnn的最大块数目；-mr选项限制记录的最大数目。这两个功能是Bell实验室版awk的扩展功* 能，在标准awk中不适用。
* -W compact or --compat, -W traditional or --traditional

    在兼容模式下运行awk。所以gawk的行为和标准的awk完全一样，所有的awk扩展都被忽略。
* -W copyleft or --copyleft, -W copyright or --copyright

    打印简短的版权信息。
* -W help or --help, -W usage or --usage

    打印全部awk选项和每个选项的简短说明。
* -W lint or --lint

    打印不能向传统unix平台移植的结构的警告。
* -W lint-old or --lint-old

    打印关于不能向传统unix平台移植的结构的警告。
* -W posix

    打开兼容模式。但有以下限制，不识别：/x、函数关键字、func、换码序列以及当fs是一个空格时，将新行作为一个域分隔符；操作符* **和**=不能代替^和^=；fflush无效。
* -W re-interval or --re-inerval

    允许间隔正则表达式的使用，参考(grep中的Posix字符类)，如括号表达式[[:alpha:]]。
* -W source program-text or --source program-text

    使用program-text作为源代码，可与-f命令混用。
* -W version or --version

    打印bug报告信息的版本。

#### 每行按空格或TAB分割，输出文本中的1、4项

文本log.txt内容如下：

```shell
2 is a test
3 Are you like awk
This's a test
10 There are orange,apple,mongo
```

使用方法：

```shell
$ awk '{print $1, $4}' log.txt
2 test
3 like
This's 
10 orange,apple,mongo
```

#### 使用","分割

```shell
$ awk -F, '{print $1,$2}' log.txt
2 is a test 
3 Are you like awk 
This's a test 
10 There are orange apple
```

或者

```shell
$ awk 'BEGIN{FS=","} {print $1,$2}' log.txt
2 is a test 
3 Are you like awk 
This's a test 
10 There are orange apple
```

#### 使用多个分隔符.先使用空格分割，然后对分割结果再使用","分割

```shell
$ awk -F '[ ,]'  '{print $1,$2,$5}'   log.txt
2 is 
3 Are awk
This's a 
10 There apple
```

#### awk -v  # 设置变量

```shell
awk -va=1 '{print $1,$1+a}' log.txt
2 3
3 4
This's 1
10 11

$ awk -va=1 -vb=s '{print $1,$1+a,$1b}' log.txt
2 3 2s
3 4 3s
This's 1 This'ss
10 11 10s
```

### 5、sed命令

参考 [Linux sed 命令](https://www.runoob.com/linux/linux-comm-sed.html)

sed 可依照脚本的指令来处理、编辑文本文件。sed 主要用来自动编辑一个或多个文件、简化对文件的反复操作、编写转换程序等。

#### 语法

```shell
sed [-hnV][-e<script>][-f<script文件>][文本文件]
```

参数说明：

```shell
-e<script>或--expression=<script> 以选项中指定的script来处理输入的文本文件。
-f<script文件>或--file=<script文件> 以选项中指定的script文件来处理输入的文本文件。
-h或--help 显示帮助。
-n或--quiet或--silent 仅显示script处理后的结果。
-V或--version 显示版本信息。
```
动作说明：

* a ：新增， a 的后面可以接字串，而这些字串会在新的一行出现(目前的下一行)
* c ：取代， c 的后面可以接字串，这些字串可以取代 n1,n2 之间的行
* d ：删除，因为是删除，所以 d 后面通常不接任何东西
* i ：插入， i 的后面可以接字串，而这些字串会在新的一行出现(目前的上一行)
* p ：打印，即将某个选择的数据印出。通常 p 会与参数 sed -n 一起运行
* s ：取代，可以直接进行取代的工作,通常这个 s 的动作可以搭配正规表示法,例如 1,20s/old/new/g

#### 在testfile文件的第四行后添加一行

testfile内容如下：

```shell
LINUX!  
Linux is a free unix-type opterating system.  
This is a linux testfile!  
Linux test
```

使用sed命令后，输出结果如下：

```shell
$ sed -e 4a\newline testfile
LINUX!  
Linux is a free unix-type opterating system.  
This is a linux testfile!  
Linux test
newline
```
然而newline并没有写入到testfile文件中

```shell
$ cat testfile
LINUX!  
Linux is a free unix-type opterating system.  
This is a linux testfile!  
Linux test
```

### 6、ps 命令

参考 [Linux ps命令](https://www.runoob.com/linux/linux-comm-ps.html)、[LINUX下PS -ef 和PS aux 的区别及格式详解](https://www.linuxidc.com/Linux/2016-07/133515.htm)

Linux ps命令用于显示当前进程 (process) 的状态。

#### 语法

```shell
ps [options] [--help]
```

参数：

* -A 列出所有的行程
* -w 显示加宽可以显示较多的资讯
* -au 显示较详细的资讯
* -aux 显示所有包含其他使用者的进程
* au(x) 输出格式 :
  * USER PID %CPU %MEM VSZ RSS TTY STAT START TIME COMMAND
  * USER: 进程拥有者
  * PID: pid
  * %CPU: 占用的 CPU 使用率
  * %MEM: 占用的记忆体使用率
  * VSZ: 占用的虚拟记忆体大小
  * RSS: 占用的记忆体大小
  * TTY: 终端的次要装置号码 (minor device number of tty)
  * STAT: 该行程的状态:
    * D: 无法中断的休眠状态 (通常 IO 的进程)
    * R: 正在执行中
    * S: 静止状态
    * T: 暂停执行
    * Z: 不存在但暂时无法消除（僵尸）
    * W: 没有足够的记忆体分页可分配
    * <: 高优先序的行程
    * N: 低优先序的行程
    * L: 有记忆体分页分配并锁在记忆体内 (实时系统或捱A I/O)
  * START: 行程开始时间
  * TIME: 执行的时间
  * COMMAND:所执行的指令

#### ps -ef

ps -ef 是用标准的格式显示进程的、其格式如下:

```shell
$ ps -ef
UID         PID   PPID  C STIME TTY          TIME CMD
root          1      0  0 11月10 ?      00:00:07 /sbin/init
root          2      0  0 11月10 ?      00:00:00 [kthreadd]
root          3      2  0 11月10 ?      00:00:03 [ksoftirqd/0]
root          5      2  0 11月10 ?      00:00:00 [kworker/0:0H]
root          7      2  0 11月10 ?      00:01:12 [rcu_sched]
```

其中各列的内容意思如下：
* UID：用户ID、但输出的是用户名 
* PID：进程的ID 
* PPID：父进程ID 
* C：进程占用CPU的百分比 
* STIME：进程启动到现在的时间 
* TTY：该进程在那个终端上运行，若与终端无关，则显示? 若为pts/0等，则表示由网络连接主机进程。 
* CMD：命令的名称和参数

#### ps aux

ps aux 是用BSD的格式来显示、其格式如下:

```shell
$ ps aux
USER        PID %CPU %MEM    VSZ   RSS TTY      STAT START   TIME COMMAND
root          1  0.0  0.1  33840  2716 ?        Ss   11月10   0:07 /sbin/init
root          2  0.0  0.0      0     0 ?        S    11月10   0:00 [kthreadd]
root          3  0.0  0.0      0     0 ?        S    11月10   0:03 [ksoftirqd/0]
root          5  0.0  0.0      0     0 ?        S<   11月10   0:00 [kworker/0:0H]
root          7  0.0  0.0      0     0 ?        S    11月10   1:12 [rcu_sched]
```

同ps -ef 不同的有列有：
* USER: 用户名 
* %CPU: 进程占用的CPU百分比 
* %MEM: 占用内存的百分比 
* VSZ : 该进程使用的虚拟內存量（KB） 
* RSS : 该进程占用的固定內存量（KB）（驻留中页的数量） 
* STAT: 进程的状态 
* START: 该进程被触发启动时间 
* TIME: 该进程实际使用CPU运行的时间

其中STAT状态位常见的状态字符有:
* D：无法中断的休眠状态（通常 IO 的进程）
* R：正在运行可中在队列中可过行的
* S：处于休眠状态
* T：停止或被追踪
* W：进入内存交换 （从内核2.6开始无效）
* X：死掉的进程 （基本很少见）
* Z：僵尸进程
* <：优先级高的进程 
* N：优先级较低的进程 
* L：有些页被锁进内存
* s：进程的领导者（在它之下有子进程）
* l：多线程，克隆线程（使用 CLONE_THREAD, 类似 NPTL pthreads） 
* +：位于后台的进程组

#### ps 结合管道

```shell
$ ps -ef|grep init
root          1      0  0 11月10 ?      00:00:07 /sbin/init
jack      32606  32592  0 11月10 ?      00:00:02 init --user
jack      88621  81252  0 20:10 pts/10   00:00:00 grep --color=auto init
$ ps aux|grep init
root          1  0.0  0.1  33840  2716 ?        Ss   11月10   0:07 /sbin/init
jack      32606  0.0  0.1  40328  3236 ?        Ss   11月10   0:02 init --user
jack      88626  0.0  0.1  15948  2384 pts/10   S+   20:10   0:00 grep --color=auto init
```

#### 按照内存使用大小对进程排序

```shell
ps aux --sort -rss
```

### 7、lsof 查看端口占用情况

lsof(list open files)是一个列出当前系统打开文件的工具。lsof 查看端口占用语法格式：

```shell
lsof -i:端口号
```

查看服务器 50070 端口的占用情况：

```shell
$ sudo lsof -i:50070
COMMAND   PID   USER   FD   TYPE DEVICE SIZE/OFF NODE NAME
java    26339 hadoop  190u  IPv4  79831      0t0  TCP *:50070 (LISTEN)
```

lsof -i查看所有端口占用情况：

```shell
sudo lsof -i
[sudo] password for jack: 
COMMAND     PID   USER   FD   TYPE DEVICE SIZE/OFF NODE NAME
avahi-dae   794  avahi   12u  IPv4  10869      0t0  UDP *:mdns 
avahi-dae   794  avahi   13u  IPv6  10870      0t0  UDP *:mdns 
avahi-dae   794  avahi   14u  IPv4  10871      0t0  UDP *:55947 
avahi-dae   794  avahi   15u  IPv6  10872      0t0  UDP *:46806 
sshd       1044   root    3u  IPv4  12342      0t0  TCP *:ssh (LISTEN)
sshd       1044   root    4u  IPv6  12344      0t0  TCP *:ssh (LISTEN)
cups-brow  1087   root    6u  IPv6  14536      0t0  TCP ip6-localhost:59116->ip6-localhost:ipp (CLOSE_WAIT)
cups-brow  1087   root    8u  IPv4  14540      0t0  UDP *:ipp 
dnsmasq    2633 nobody    4u  IPv4  20281      0t0  UDP jack-virtual-machine:domain 
dnsmasq    2633 nobody    5u  IPv4  20282      0t0  TCP jack-virtual-machine:domain (LISTEN)
cupsd      3238   root   10u  IPv6  21850      0t0  TCP ip6-localhost:ipp (LISTEN)
cupsd      3238   root   11u  IPv4  21851      0t0  TCP localhost:ipp (LISTEN)
java      26339 hadoop  190u  IPv4  79831      0t0  TCP *:50070 (LISTEN)
java      26339 hadoop  202u  IPv4  80888      0t0  TCP localhost:9000 (LISTEN)
java      26339 hadoop  212u  IPv4  81797      0t0  TCP localhost:9000->localhost:51224 (ESTABLISHED)
java      26480 hadoop  190u  IPv4  81486      0t0  TCP *:50010 (LISTEN)
java      26480 hadoop  194u  IPv4  81922      0t0  TCP localhost:45942 (LISTEN)
java      26480 hadoop  225u  IPv4  82148      0t0  TCP *:50075 (LISTEN)
java      26480 hadoop  227u  IPv4  81754      0t0  TCP *:50020 (LISTEN)
java      26480 hadoop  238u  IPv4  82226      0t0  TCP localhost:51224->localhost:9000 (ESTABLISHED)
java      26682 hadoop  196u  IPv4  82784      0t0  TCP *:50090 (LISTEN)
dhclient  85039   root    6u  IPv4 403616      0t0  UDP *:bootpc 
dhclient  85039   root   20u  IPv4 403596      0t0  UDP *:32886 
dhclient  85039   root   21u  IPv6 403597      0t0  UDP *:26153 
```

### 8、netstat 命令

netstat -tunlp 用于显示 tcp，udp 的端口和进程等相关情况。netstat 查看端口占用语法格式：

```shell
netstat -tunlp | grep 端口号
```

* -t (tcp) 仅显示tcp相关选项
* -u (udp)仅显示udp相关选项
* -n 拒绝显示别名，能显示数字的全部转化为数字
* -l 仅列出在Listen(监听)的服务状态
* -p 显示建立相关链接的程序名

例如查看 50070 端口的情况，使用以下命令：

```shell
$ sudo netstat -tunlp|grep 50070
[sudo] password for jack: 
tcp        0      0 0.0.0.0:50070           0.0.0.0:*               LISTEN      26339/java
```
也可以使用

```shell
netstat -pnt | grep :80 | wc -l
```

来查看当前端口的用户数量，其中netstat需要占用一个。

### 9、GDB调试程序的使用

见《二、开发相关》中的《高级编程》部分。

### 10、查看程序运行所需的共享库：ldd命令

见《二、开发相关》中的《高级编程》中动静态库部分。

### 11、查看cpu使用情况：top命令

top命令内容如下：

```shell
top - 16:15:36 up 1 day, 58 min,  4 users,  load average: 0.04, 0.07, 0.03
Tasks: 258 total,   1 running, 257 sleeping,   0 stopped,   0 zombie
%Cpu(s):  0.5 us,  1.7 sy,  0.0 ni, 97.8 id,  0.0 wa,  0.0 hi,  0.0 si,  0.0 st
KiB Mem:   2030820 total,  1701424 used,   329396 free,   133684 buffers
KiB Swap:  1046524 total,     7804 used,  1038720 free.   735580 cached Mem
```

除了显示当前cpu和内存使用情况，程序情况外，还会显示当前占用cpu较多的进程信息。

其中，top命令右上角的三个数字：

load average: 0.04, 0.07, 0.03 — load average后面的三个数分别是1分钟、5分钟、15分钟的负载情况。

load average数据是每隔5秒钟检查一次活跃的进程数，然后按特定算法计算出的数值。一般来说，负载在0-1.00是正常的，超过了1.00就要注意了，在多核处理器中系统均值不应该高于处理核心总数。

### 12、查看内存空间、磁盘空间(free，df -h)

```powershell
jack@jack-virtual-machine:~/workspace/sed$ free
             total       used       free     shared    buffers     cached
Mem:       2030820    1705664     325156       9024     134548     735692
-/+ buffers/cache:     835424    1195396
Swap:      1046524       7796    1038728

jack@jack-virtual-machine:~/workspace/sed$ df -h
Filesystem      Size  Used Avail Use% Mounted on
udev            981M  4.0K  981M   1% /dev
tmpfs           199M  1.5M  197M   1% /run
/dev/sda1        19G  9.1G  8.6G  52% /
none            4.0K     0  4.0K   0% /sys/fs/cgroup
none            5.0M     0  5.0M   0% /run/lock
none            992M  152K  992M   1% /run/shm
none            100M   48K  100M   1% /run/user
vmhgfs-fuse     331G  284G   47G  86% /mnt/hgfs
/dev/sr0        1.1G  1.1G     0 100% /media/jack/Ubuntu 14.04.5 LTS amd64
```

### 13、ethtool 和 iperf 探查网络数据带宽

100M带宽，就是100M bit,而平常我们下载软件所显示的单位都是byte，1byte=8bit，所以100M带宽的最大传输速率为100/8M Byte，但这个速度是不可能达到的。

可通过下面命令查看网卡：

```shell
$ ethtool eth0 | grep Speed
	Speed: 1000Mb/s
```

通过iperf工具可以做到在不占满情况下测试网络数据带宽。具体可参考：

 [使用iPerf进行网络吞吐量测试](./img/1.6.png)

> https://www.jianshu.com/p/15f888309c72

例如，在一个终端中输入（服务端）：

```shell
$ iperf -s
------------------------------------------------------------
Server listening on TCP port 5001
TCP window size: 85.3 KByte (default)
------------------------------------------------------------
[  4] local 127.0.0.1 port 5001 connected with 127.0.0.1 port 37120
[ ID] Interval       Transfer     Bandwidth
[  4]  0.0-10.0 sec  46.7 GBytes  40.1 Gbits/sec
```

重新打开一个终端作为客户端，测试本地回环：

```shell
$ iperf -c 127.0.0.1
------------------------------------------------------------
Client connecting to 127.0.0.1, TCP port 5001
TCP window size: 2.50 MByte (default)
------------------------------------------------------------
[  3] local 127.0.0.1 port 37120 connected with 127.0.0.1 port 5001
[ ID] Interval       Transfer     Bandwidth
[  3]  0.0-10.0 sec  46.7 GBytes  40.1 Gbits/sec
```

### 14、pkill 杀死指定名称的进程

linux下可使用pkill命令，这里以前面的iperf程序为例：

```shell
pkill -9 iperf
```

当然也可以使用awk：

```shell
sudo kill `(ps -ef|grep iperf | awk {'if(NR == 1){print $2}'})`
```

### 15、strace 命令

strace 可以用来查看程序系统调用过程、查看系统调用次数、跟踪信号传递过程。

看下面这个程序：

```cpp
#include <iostream>
#include <unistd.h>
using namespace std;

int main(void)
{
    cout << " test main ..... " << endl;
    return 0;

}
```

很简单的一个程序，只是调用了cout输出一句话，接下来编译并使用strace命令追踪：

```shell
$ g++ strace.cpp -o app -g
$ strace ./app
execve("./app", ["./app"], [/* 76 vars */]) = 0
brk(0)                                  = 0x12dd000
access("/etc/ld.so.nohwcap", F_OK)      = -1 ENOENT (No such file or directory)
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7ff27262b000
access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)
open("/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
fstat(3, {st_mode=S_IFREG|0644, st_size=86198, ...}) = 0
mmap(NULL, 86198, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7ff272615000
close(3)                                = 0
access("/etc/ld.so.nohwcap", F_OK)      = -1 ENOENT (No such file or directory)
open("/usr/lib/x86_64-linux-gnu/libstdc++.so.6", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0 \266\5\0\0\0\0\0"..., 832) = 832
fstat(3, {st_mode=S_IFREG|0644, st_size=979056, ...}) = 0
mmap(NULL, 3159040, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7ff272107000
mprotect(0x7ff2721ed000, 2093056, PROT_NONE) = 0
mmap(0x7ff2723ec000, 40960, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0xe5000) = 0x7ff2723ec000
mmap(0x7ff2723f6000, 82944, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7ff2723f6000
close(3)                                = 0
access("/etc/ld.so.nohwcap", F_OK)      = -1 ENOENT (No such file or directory)
open("/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0P \2\0\0\0\0\0"..., 832) = 832
fstat(3, {st_mode=S_IFREG|0755, st_size=1840928, ...}) = 0
mmap(NULL, 3949248, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7ff271d42000
mprotect(0x7ff271efc000, 2097152, PROT_NONE) = 0
mmap(0x7ff2720fc000, 24576, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1ba000) = 0x7ff2720fc000
mmap(0x7ff272102000, 17088, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7ff272102000
close(3)                                = 0
access("/etc/ld.so.nohwcap", F_OK)      = -1 ENOENT (No such file or directory)
open("/lib/x86_64-linux-gnu/libm.so.6", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\20V\0\0\0\0\0\0"..., 832) = 832
fstat(3, {st_mode=S_IFREG|0644, st_size=1071552, ...}) = 0
mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7ff272614000
mmap(NULL, 3166568, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7ff271a3c000
mprotect(0x7ff271b41000, 2093056, PROT_NONE) = 0
mmap(0x7ff271d40000, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x104000) = 0x7ff271d40000
close(3)                                = 0
access("/etc/ld.so.nohwcap", F_OK)      = -1 ENOENT (No such file or directory)
open("/lib/x86_64-linux-gnu/libgcc_s.so.1", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\0\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\260*\0\0\0\0\0\0"..., 832) = 832
fstat(3, {st_mode=S_IFREG|0644, st_size=90160, ...}) = 0
mmap(NULL, 2186016, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7ff271826000
mprotect(0x7ff27183c000, 2093056, PROT_NONE) = 0
mmap(0x7ff271a3b000, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x15000) = 0x7ff271a3b000
close(3)                                = 0
mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7ff272613000
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7ff272611000
arch_prctl(ARCH_SET_FS, 0x7ff272611780) = 0
mprotect(0x7ff2720fc000, 16384, PROT_READ) = 0
mprotect(0x7ff271d40000, 4096, PROT_READ) = 0
mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7ff272610000
mprotect(0x7ff2723ec000, 32768, PROT_READ) = 0
mprotect(0x600000, 4096, PROT_READ)     = 0
mprotect(0x7ff27262d000, 4096, PROT_READ) = 0
munmap(0x7ff272615000, 86198)           = 0
fstat(1, {st_mode=S_IFCHR|0620, st_rdev=makedev(136, 12), ...}) = 0
mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7ff27262a000
write(1, " test main ..... \n", 18 test main ..... 
)     = 18
exit_group(0)                           = ?
+++ exited with 0 +++
```

整个程序运行的系统调用全部依次打印出来了，最后的write才是我们的输出真正调用的，但再次之前做了相当多的内存映射和动态库链接等工作。

接下来查看系统调用的次数：

```shell
$ strace -c ./app
 test main ..... 
% time     seconds  usecs/call     calls    errors syscall
------ ----------- ----------- --------- --------- ----------------
  0.00    0.000000           0         4           read
  0.00    0.000000           0         1           write
  0.00    0.000000           0         5           open
  0.00    0.000000           0         5           close
  0.00    0.000000           0         6           fstat
  0.00    0.000000           0        17           mmap
  0.00    0.000000           0         9           mprotect
  0.00    0.000000           0         1           munmap
  0.00    0.000000           0         1           brk
  0.00    0.000000           0         6         6 access
  0.00    0.000000           0         1           execve
  0.00    0.000000           0         1           arch_prctl
------ ----------- ----------- --------- --------- ----------------
100.00    0.000000                    57         6 total
```

接下来修改程序，打印输出后休眠3s：

```cpp
cout << " test main ..... " << endl;
sleep(3);
return 0;
```

然后执行strace  ./app过程中，开启另一个终端执行pkill app，部分内容如下：

```shell
write(1, " test main ..... \n", 18 test main ..... 
)     = 18
rt_sigprocmask(SIG_BLOCK, [CHLD], [], 8) = 0
rt_sigaction(SIGCHLD, NULL, {SIG_DFL, [], 0}, 8) = 0
rt_sigprocmask(SIG_SETMASK, [], NULL, 8) = 0
nanosleep({3, 0}, {2, 94424855})        = ? ERESTART_RESTARTBLOCK (Interrupted by signal)
--- SIGTERM {si_signo=SIGTERM, si_code=SI_USER, si_pid=51263, si_uid=1000} ---
+++ killed by SIGTERM +++
```

从上面可以看到，***+++ killed by SIGTERM +++***，这个pkill信号终止进程被捕获了。

### 16、ulimit 命令

ulimit -a 查看当前会话中资源数量

ulimit -n 用于查看或修改当前会话中文件描述符个数

ulimit -s 用于查看或修改当前会话中栈的大小

ulimit -c unlimited 用于生产coredump文件

### 17、valgrind 工具

valgrind的有很多工具组成，默认是使用memcheck，在使用时也可加上--tool=tool name来指定工具。

该工具可用于：

* 检查非法内存使用，例如越界数组写入数据
* 检查内存溢出情况
* 检查内存泄漏
* 检查是否使用未初始化的内存，例如int a，a没有初始化，所以里面的值是不确定的
* 检测内存覆盖，例如两段数组出现交集
* 动态内存错误
  *  申请和释放内存不一致 ，例如使用了new申请内存，但使用free释放内存
   * 申请和释放内存次数不一致，例如new了一次，却delete多次
   * 在释放的内存上进行读写操作，free掉后指针指向空间是未知的，不可访问的

**详细内容请参考 《三、C_C++语言》中“编译链接加载内存专题”。**

### 18、traceroute的过程

通过traceroute我们可以知道信息从你的计算机到互联网另一端的主机是走的什么路径。当然每次数据包由某一同样的出发点（source）到达某一同样的目的地(destination)走的路径可能会不一样，但基本上来说大部分时候所走的路由是相同的。linux系统中，我们称之为traceroute,在MS Windows中为tracert。 traceroute通过发送小的数据包到目的设备直到其返回，来测量其需要多长时间。一条路径上的每个设备traceroute要测3次。输出结果中包括每次测试的时间(ms)和设备的名称（如有的话）及其IP地址。

在大多数情况下，我们会在linux主机系统下，直接执行命令行：

```shell
traceroute hostname
```

如果是在windows下，可以这样：

```powershell
> tracert www.baidu.com

通过最多 30 个跃点跟踪
到 www.a.shifen.com [36.152.44.95] 的路由:

  1     3 ms     3 ms     1 ms  192.168.0.1
  2     5 ms     1 ms     2 ms  YHTC_GW.bbrouter [192.168.1.1]
  3     4 ms     4 ms     4 ms  10.84.64.1
  4    23 ms     7 ms     9 ms  HW-123-61.yic.jx.chinamobile.com [211.141.123.61]
  5    42 ms     9 ms     9 ms  221.183.48.133
  6     *        *      160 ms  221.183.42.145
  7    38 ms    23 ms    27 ms  221.183.59.54
  8    47 ms    32 ms    27 ms  162.23.207.183.static.js.chinamobile.com [183.207.23.162]
  9    39 ms    23 ms    27 ms  182.61.253.214
 10     *        *        *     请求超时。
 11    24 ms    26 ms    36 ms  36.152.44.95

跟踪完成。
```

**有时我们traceroute 一台主机时，会看到有一些行是以星号表示的。出现这样的情况，可能是防火墙封掉了ICMP的返回信息，所以我们得不到什么相关的数据包返回数据。**
		**Traceroute程序的设计是利用ICMP及IP header的TTL（Time To Live）栏位（field）**。首先，traceroute送出一个TTL是1的IP datagram（其实，每次送出的为3个40字节的包，包括源地址，目的地址和包发出的时间标签）到目的地，探测包是UDP格式，由于我们不想目的主机处理它们，因此目的端口设置为一个不可能的值。当路径上的第一个路由器（router）收到这个datagram时，它将TTL减1。此时，TTL变为0了，所以该路由器会将此datagram丢掉，并送回一个「ICMP time exceeded」消息（包括发IP包的源地址，IP包的所有内容及路由器的IP地址），traceroute 收到这个消息后，便知道这个路由器存在于这个路径上，接着traceroute 再送出另一个TTL是2 的datagram，发现第2 个路由器...... traceroute 每次将送出的datagram的TTL 加1来发现另一个路由器，这个重复的动作一直持续到某个datagram 抵达目的地。当datagram到达目的地后，该主机并不会送回ICMP time exceeded消息，因为它已是目的地了，那么traceroute如何得知目的地到达了呢？

Traceroute在送出UDP datagrams到目的地时，它所选择送达的port number 是一个一般应用程序都不会用的号码（30000 以上），所以当此UDP datagram 到达目的地后该主机会送回一个「ICMP port unreachable」的消息，而当traceroute 收到这个消息时，便知道目的地已经到达了。所以traceroute 在Server端也是没有所谓的Daemon 程式。

Traceroute提取发 ICMP TTL到期消息设备的IP地址并作域名解析。每次 ，Traceroute都打印出一系列数据,包括所经过的路由设备的域名及 IP地址,三个包每次来回所花时间。

Traceroute有一个固定的时间等待响应(ICMP TTL到期消息)。如果这个时间过了，它将打印出一系列的*号表明：在这个路径上，这个设备不能在给定的时间内发出ICMP TTL到期消息的响应。然后，Traceroute给TTL记数器加1并继续进行。

### 19、dstat 工具

dstat 非常强大，可以实时的监控 cpu、磁盘、网络、IO、内存等使用情况。更多可参考[dstat命令](./img/dstat.png)  http://lnmp.ailinux.net/dstat

使用工具dstat：

```shell
$ dstat
You did not select any stats, using -cdngy by default.
----total-cpu-usage---- -dsk/total- -net/total- ---paging-- ---system--
usr sys idl wai hiq siq| read  writ| recv  send|  in   out | int   csw 
  0   0  99   0   0   0|8424B 6724B|   0     0 |  17B   70B|  69   147 
  0   0 100   0   0   0|   0     0 | 432B    0 |   0     0 |  93   232 
  0   0 100   0   0   0|   0     0 |  92B    0 |   0     0 |  66   186 
  0   0 100   0   0   0|   0     0 |  92B    0 |   0     0 |  85   208 
  0   0 100   0   0   0|   0   236k|   0     0 |   0     0 |  87   210 
  1   0  99   0   0   0|   0     0 |   0     0 |   0     0 |  69   181 
  0   0 100   0   0   0|   0     0 |   0     0 |   0     0 |  79   219 
  0   1  99   0   0   0|   0     0 |   0     0 |   0     0 | 111   410 
  1   1  99   0   0   0|   0     0 |   0     0 |   0     0 |  89   267 
```

### 20、ipcs/ipcrm 命令

​        信号量、消息队列和共享内存三种System v IPC进程间通信方式都使用一个全局唯一的键值(key)来描述多个共享资源。当程序调用semget、shmget或者msgget时，就创建了这些共享资源的一个实例·。Linux提供了ipcs命令观察当前系统上拥有哪些共享资源实例：

```shell
$ ipcs

------ Shared Memory Segments --------
key        shmid      owner      perms      bytes      nattch     status      
0x00000000 7110656    jack       600        524288     2          dest         
0x00000000 5079041    jack       600        524288     2          dest         
0x00000000 5111810    jack       600        524288     2          dest         
0x00000000 5373955    jack       600        524288     2          dest         
0x00000000 5767172    jack       600        524288     2          dest         
0x00000000 5570565    jack       600        524288     2          dest                

------ Semaphore Arrays --------
key        semid      owner      perms      nsems     
0x00000000 32768      jack       666        1         
0x00000000 65537      jack       666        5         

------ Message Queues --------
key        msqid      owner      perms      used-bytes   messages    
0x000004d2 0          jack       666        0            0           
```

**ipcs 命令示例：**

* ipcs -a或ipc ：显示当前系统中共享内存段、信号量集、消息队列的使用情况

* ipcs -m ：显示共享内存段的使用情况

* ipcs -s ：显示信号量集的使用情况

* ipcs -q ：显示消息队列的使用情况

**ipcrm可用来删除对应的共享内存段、信号量、消息队列：**

* ipcrm -s semid ：删除对应的信号量集
* ipcrm -m shmid ：删除对应的共享内存段
* ipcrm -q msqid ：删除对应的消息队列

### 21、Linux 性能分析工具gprof

参考 [linux 下性能分析工具 Gprof](https://blog.csdn.net/datase/article/details/78404313)

gprof 是GNU工具之一，它在编译的时候在每个函数的出入口加入了profiling的代码，运行时统计程序在用户态的 执行信息，可以得到每个函数的调用次数，执行时间，调用关系等信息，简单易懂。适合于查找用户级程序的性能瓶颈，对于很多时间都在内核态执行的程序，gprof不适合。

gprof 通过在编译和链接程序的时候（使用 -pg 编译和链接选项），gcc 在你应用程序的每个函数中都加入了一个名为mcount ( or “_mcount” , or “__mcount” , 依赖于编译器或操作系统)的函数，也就是说你的应用程序里的每一个函数都会调用mcount, 而mcount 会在内存中保存一张函数调用图，并通过函数调用堆栈的形式查找子函数和父函数的地址。这张调用图也保存了所有与函数相关的调用时间，调用次数等等的所有信息。

需要注意的是，程序的累积执行时间只是包括gprof能够监控到的函数。工作在内核态的函数和没有加-pg编译的第三方库函数是无法被gprof能够监控到的（如sleep等）。

对于下面这个程序：

```cpp
#include <iostream>
using namespace std;

void fun1()
{
	int i = 0;
	while(i<100)
	{
		i++;        
		cout << "fun1" << endl;
	}        
}

void fun2()
{
	int i = 0;
	int b;
	while(i<50)
	{
		i++;
		cout << "fun2" << endl;
		b+=i;        
	}        
}

void fun()
{
	fun1();
	fun2();
}

int main(void)
{
	fun();
	return 0;
}
```

编译：

```shell
$ g++ gprof.cpp -pg -lc -o app
```

运行 ./app文件，便于后面生成 gmon.out文件：

```shell
$ ./app
```

生成报告文件：

```shell
gprof -b ./app gmon.out >report.txt
```

查看报告文件：

```shell
$ cat report.txt 
Flat profile:

Each sample counts as 0.01 seconds.
 no time accumulated

  %   cumulative   self              self     total           
 time   seconds   seconds    calls  Ts/call  Ts/call  name    
  0.00      0.00     0.00        1     0.00     0.00  _GLOBAL__sub_I__Z4fun1v
  0.00      0.00     0.00        1     0.00     0.00  fun()
  0.00      0.00     0.00        1     0.00     0.00  __static_initialization_and_destruction_0(int, int)
  0.00      0.00     0.00        1     0.00     0.00  fun1()
  0.00      0.00     0.00        1     0.00     0.00  fun2()


			Call graph


granularity: each sample hit covers 2 byte(s) no time propagated

index % time    self  children    called     name
                0.00    0.00       1/1           __libc_csu_init [18]
[8]      0.0    0.00    0.00       1         _GLOBAL__sub_I__Z4fun1v [8]
                0.00    0.00       1/1           __static_initialization_and_destruction_0(int, int) [10]
-----------------------------------------------
                0.00    0.00       1/1           main [6]
[9]      0.0    0.00    0.00       1         fun() [9]
                0.00    0.00       1/1           fun2() [12]
                0.00    0.00       1/1           fun1() [11]
-----------------------------------------------
                0.00    0.00       1/1           _GLOBAL__sub_I__Z4fun1v [8]
[10]     0.0    0.00    0.00       1         __static_initialization_and_destruction_0(int, int) [10]
-----------------------------------------------
                0.00    0.00       1/1           fun() [9]
[11]     0.0    0.00    0.00       1         fun1() [11]
-----------------------------------------------
                0.00    0.00       1/1           fun() [9]
[12]     0.0    0.00    0.00       1         fun2() [12]
-----------------------------------------------


Index by function name

   [8] _GLOBAL__sub_I__Z4fun1v [10] __static_initialization_and_destruction_0(int, int) [12] fun2()
   [9] fun()                  [11] fun1()
```

### 22、pstack检测死锁

有这个样一个产生死锁的程序 [](./src/deathLock.c)：

```cpp
#include <unistd.h> 
#include <pthread.h> 
#include <string.h> 
 
pthread_mutex_t mutexA = PTHREAD_MUTEX_INITIALIZER; 
pthread_mutex_t mutexB = PTHREAD_MUTEX_INITIALIZER;
 
static int counterA = 0; 
static int counterB = 0; 
 
int func1() 
{ 
   pthread_mutex_lock(&mutexA); 
   ++counterA; 
   sleep(1); 
   pthread_mutex_lock(&mutexB); 
   ++counterB; 
   pthread_mutex_unlock(&mutexB); 
   pthread_mutex_unlock(&mutexA); 
 
   return counterA; 
} 
 
int func2() 
{ 
   pthread_mutex_lock(&mutexB); 
   ++counterB; 
   sleep(1); 
   pthread_mutex_lock(&mutexA); 
   ++counterA; 
   pthread_mutex_unlock(&mutexA); 
   pthread_mutex_unlock(&mutexB); 
 
   return counterB; 
} 
 
void* start_routine1(void* arg) 
{ 
       int iRetValue = func1(); 
} 
 
void* start_routine2(void* arg) 
{ 
       int iRetValue = func2(); 
} 
 
int main() 
{ 
   pthread_t tid[2]; 
   if (pthread_create(&tid[0], NULL, &start_routine1, NULL) != 0) 
   { 
       _exit(1); 
   } 
   if (pthread_create(&tid[1], NULL, &start_routine2, NULL) != 0) 
   { 
       _exit(1); 
   } 
   sleep(5); 
 
   pthread_join(tid[0], NULL); 
   pthread_join(tid[1], NULL); 
 
   pthread_mutex_destroy(&mutexA); 
   pthread_mutex_destroy(&mutexB); 
 
   return 0; 
}
```

下面将通过一个pstack脚本来分析这个程序，该脚本为[](./src/mypstack.sh)

1、编译并运行程序

```shell
$ gcc -g deathLock.c -o app -lpthread
$ ./app
```

2、查看进程号

```shell
$ ps -aux|grep ./app
jack      47794  0.0  0.0  22888   864 pts/3    Sl+  15:44   0:00 ./app
jack      47798  0.0  0.1  15944  2512 pts/12   S+   15:44   0:00 grep --color=auto ./app
```

2、使用mypstack调试程序（务必加sudo命令）

```shell
$ sudo ./mypstack 47794
Thread 3 (Thread 0x7f0531d82700 (LWP 47795)):
#0  __lll_lock_wait () at ../nptl/sysdeps/unix/sysv/linux/x86_64/lowlevellock.S:135
#1  0x00007f0532152649 in _L_lock_909 () from /lib/x86_64-linux-gnu/libpthread.so.0
#2  0x00007f0532152470 in __GI___pthread_mutex_lock (mutex=0x6010e0 <mutex>) at ../nptl/pthread_mutex_lock.c:79
#3  0x000000000040081e in func1 () at deathLock.c:16
#4  0x00000000004008bb in start_routine1 (arg=0x0) at deathLock.c:39
#5  0x00007f0532150184 in start_thread (arg=0x7f0531d82700) at pthread_create.c:312
#6  0x00007f0531e7d37d in clone () at ../sysdeps/unix/sysv/linux/x86_64/clone.S:111
Thread 2 (Thread 0x7f0531581700 (LWP 47796)):
#0  __lll_lock_wait () at ../nptl/sysdeps/unix/sysv/linux/x86_64/lowlevellock.S:135
#1  0x00007f0532152649 in _L_lock_909 () from /lib/x86_64-linux-gnu/libpthread.so.0
#2  0x00007f0532152470 in __GI___pthread_mutex_lock (mutex=0x6010a0 <mutexA>) at ../nptl/pthread_mutex_lock.c:79
#3  0x000000000040087a in func2 () at deathLock.c:29
#4  0x00000000004008d6 in start_routine2 (arg=0x0) at deathLock.c:44
#5  0x00007f0532150184 in start_thread (arg=0x7f0531581700) at pthread_create.c:312
#6  0x00007f0531e7d37d in clone () at ../sysdeps/unix/sysv/linux/x86_64/clone.S:111
Thread 1 (Thread 0x7f053256d740 (LWP 47794)):
#0  0x00007f053215165b in pthread_join (threadid=139660287813376, thread_return=0x0) at pthread_join.c:92
#1  0x0000000000400954 in main () at deathLock.c:60
```

可以看到两个线程当前处于 __lll_lock_wait 状态。

3、gdb调试程序（这里只截取片段）

```shell
(gdb) info thread
  Id   Target Id         Frame 
  3    Thread 0x7ffff6ff5700 (LWP 47831) "app" __lll_lock_wait () at ../nptl/sysdeps/unix/sysv/linux/x86_64/lowlevellock.S:135
  2    Thread 0x7ffff77f6700 (LWP 47830) "app" __lll_lock_wait () at ../nptl/sysdeps/unix/sysv/linux/x86_64/lowlevellock.S:135
* 1    Thread 0x7ffff7fdd740 (LWP 47826) "app" 0x00007ffff78b7dfd in nanosleep () at ../sysdeps/unix/syscall-template.S:81
(gdb) thread 2
[Switching to thread 2 (Thread 0x7ffff77f6700 (LWP 47830))]
#0  __lll_lock_wait () at ../nptl/sysdeps/unix/sysv/linux/x86_64/lowlevellock.S:135
135	../nptl/sysdeps/unix/sysv/linux/x86_64/lowlevellock.S: No such file or directory.
(gdb) thread 3
[Switching to thread 3 (Thread 0x7ffff6ff5700 (LWP 47831))]
#0  __lll_lock_wait () at ../nptl/sysdeps/unix/sysv/linux/x86_64/lowlevellock.S:135
135	in ../nptl/sysdeps/unix/sysv/linux/x86_64/lowlevellock.S
```

可以看到thread2和thread3一直处于__lll_lock_wait 状态，这便是死锁了。