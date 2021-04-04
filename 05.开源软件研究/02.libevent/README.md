## libevent学习 ##

#### 一、安装步骤与参考资料

一些关于学习libevent的记录,如果要想运行这些例子，你得安装好libevent,这里的测试环境为Ubuntu 14.04 LTS，libevnet版本为libevent-2.0.20-stable.

libevent官网： http://libevent.org/

libevent文档：http://www.wangafu.net/~nickm/libevent-book/

第三方学习文档：https://aceld.gitbooks.io/libevent/content/

源码分析可见《Linux 高性能服务器编程》第12章。

安装步骤：

```shell
$ tar xzf libevent-2.0.20-stable.tar.gz
$ cd libevent-2.0.20-stable/
$ ./configure
$ make
$ sudo make install
```

#### 二、使用案例

在src/three_simple_demo里有三个不断提高的libevent使用案例

在src/http_server_deom中有一个使用libevnet构建的简易Http服务器案例