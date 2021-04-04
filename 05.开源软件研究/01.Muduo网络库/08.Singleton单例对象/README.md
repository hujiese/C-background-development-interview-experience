## Singleton单例对象

更多可见 [muduo源码分析之 Singleton单例对象](https://github.com/hujiese/Large-concurrent-serve/blob/master/14_muduo_Singleton/muduo_Singleton.md)

![](https://camo.githubusercontent.com/0c3eab502adc01e1ba0ba396180dbd81310f722d/68747470733a2f2f692e696d6775722e636f6d2f6b7a43346a4e672e706e67)

### 1、pthread_once函数

该函数的原型如下：
```c
int pthread_once(pthread_once_t *once_control, void (*init_routine) (void))
```
函数使用初值为PTHREAD_ONCE_INIT的once_control变量保证init_routine()函数在本进程执行序列中仅执行一次。

LinuxThreads使用互斥锁和条件变量保证由pthread_once()指定的函数执行且仅执行一次，而once_control则表征是否执行过。如果once_control的初值不是PTHREAD_ONCE_INIT（LinuxThreads定义为0），pthread_once() 的行为就会不正常。在LinuxThreads中，实际"一次性函数"的执行状态有三种：NEVER（0）、IN_PROGRESS（1）、DONE （2），如果once初值设为1，则由于所有pthread_once()都必须等待其中一个激发"已执行一次"信号，因此所有pthread_once ()都会陷入永久的等待中；如果设为2，则表示该函数已执行过一次，从而所有pthread_once()都会立即返回0。

简而言之，如果once_control的值是PTHREAD_ONCE_INIT，那么init_routine只会执行一次，如果在init_routine函数里执行一个对象的创建，那么即使以后在调用init_routine创建该对象，那么该对象也不会再创建，因为init_routine只执行一次，该对象在全局是唯一的，也就是“单例”。

### 2、muduo实现单例对象

muduo是使用pthread_once函数来实现单例对象的：

```c
private:
  static pthread_once_t ponce_;
  static T*             value_;

template<typename T>
pthread_once_t Singleton<T>::ponce_ = PTHREAD_ONCE_INIT;

template<typename T>
T* Singleton<T>::value_ = NULL;

 static T& instance()
 {
   pthread_once(&ponce_, &Singleton::init);
   return *value_;
 }

 static void init()
 {
   value_ = new T();
   ::atexit(destroy);
 }
 ```

 假设现在有一个类叫Test，初始化单例对象Test：

 ```c
 Test t = muduo::Singleton<Test>::instance();
 ```

 init()函数只会执行一次。muduo的单例对象是“懒汉式”创建的。

 其中用到了atexit函数，atexit函数是一个特殊的函数，它是在正常程序退出时调用的函数，称之为登记函数，其函数原型如下：
 ```cpp
 int atexit (void (*)(void));
 ```