## ThreadLocal线程特定数据

更多可见 [muduo源码分析--ThreadLocal](muduo源码分析--ThreadLocal)

### 1、线程特定数据

在单线程程序中，我们经常要用到"全局变量"以实现多个函数间共享数据。但是在多线程环境下，由于数据空间是共享的，因此全局变量也为所有线程所共有。 然而时应用程序设计中有必要提供线程私有的全局变量，仅在某个线程中有效，但却可以跨多个函数访问。例如聊天服务器案例中，如果是Reactor模式下的多线程服务器，每个线程都共享一个客户端Connection连接集合，如果需要回射客户端消息，则需要对该集合加锁操作，一个时刻只能由一个线程获取并操作该Connection连接集合，这样效率还有待提高。如果使用线程特定数据，每次连接到来时，让每个线程都拥有自己的Connection连接集合，那么这样就可以达到并行的效果，不需要加锁，提高了效率。

POSIX线程库通过维护一定的数据结构来解决这个问题，这个些数据称为（Thread-specific Data，或 TSD）。 线程特定数据也称为线程本地存储TLS（Thread-local storage），对于POD类型的线程本地存储，可以用__thread关键字。

下面说一下线程存储的具体用法。

* 创建一个类型为pthread_key_t类型的变量。

* 调用pthread_key_create()来创建该变量。该函数有两个参数，第一个参数就是上面声明的pthread_key_t变量，第二个参数是一个清理函数，用来在线程释放该线程存储的时候被调用。该函数指针可以设成 NULL，这样系统将调用默认的清理函数。该函数成功返回0.其他任何返回值都表示出现了错误。

* 当线程中需要存储特殊值的时候，可以调用 pthread_setspcific() 。该函数有两个参数，第一个为前面声明的pthread_key_t变量，第二个为void*变量，这样你可以存储任何类型的值。

* 如果需要取出所存储的值，调用pthread_getspecific()。该函数的参数为前面提到的pthread_key_t变量，该函数返回void *类型的值。

下面这几个函数的原型：

```c
int pthread_key_create(pthread_key_t *key, void (*destructor)(void*));

int pthread_setspecific(pthread_key_t key, const void *value);

void *pthread_getspecific(pthread_key_t key);
```

该组函数的使用案例如下：

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

pthread_key_t key; 

struct test_struct { // 用于测试的结构
    int i;
    float k;
};

void *child1(void *arg)
{
    struct test_struct struct_data; // 首先构建一个新的结构
    struct_data.i = 10;
    struct_data.k = 3.1415;
    pthread_setspecific(key, &struct_data); 
    printf("child1--address of struct_data is --> 0x%p\n", &(struct_data));
    printf("child1--from pthread_getspecific(key) get the pointer and it points to --> 0x%p\n", (struct test_struct *)pthread_getspecific(key));
    printf("child1--from pthread_getspecific(key) get the pointer and print it's content:\nstruct_data.i:%d\nstruct_data.k: %f\n", 
        ((struct test_struct *)pthread_getspecific(key))->i, ((struct test_struct *)pthread_getspecific(key))->k);
    printf("------------------------------------------------------\n");
}
void *child2(void *arg)
{
    int temp = 20;
    sleep(2);
    printf("child2--temp's address is 0x%p\n", &temp);
    pthread_setspecific(key, &temp); // 好吧，原来这个函数这么简单
    printf("child2--from pthread_getspecific(key) get the pointer and it points to --> 0x%p\n", (int *)pthread_getspecific(key));
    printf("child2--from pthread_getspecific(key) get the pointer and print it's content --> temp:%d\n", *((int *)pthread_getspecific(key)));
}
int main(void)
{
    pthread_t tid1, tid2;
    pthread_key_create(&key, NULL); // 这里是构建一个pthread_key_t类型，确实是相当于一个key
    pthread_create(&tid1, NULL, child1, NULL);
    pthread_create(&tid2, NULL, child2, NULL);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_key_delete(key);
    return (0);
}
```
运行结果：

    child1--address of struct_data is --> 0x0x7ffff77eff40
    child1--from pthread_getspecific(key) get the pointer and it points to --> 0x0x7ffff77eff40
    child1--from pthread_getspecific(key) get the pointer and print it's content:
    struct_data.i:10
    struct_data.k: 3.141500
    ------------------------------------------------------
    child2--temp's address is 0x0x7ffff6feef44
    child2--from pthread_getspecific(key) get the pointer and it points to --> 0x0x7ffff6feef44
    child2--from pthread_getspecific(key) get the pointer and print it's content --> temp:20

### 2、ThreadLocal

![](https://camo.githubusercontent.com/461a0783de990c11b91a3c65f416a8f475994824/68747470733a2f2f692e696d6775722e636f6d2f49596c4d5965562e706e67)

下面从一个ThreadLocal的创建开始说起，假设有一个类叫Test，可以通过该方法创建一个线程局部对象：
```c
muduo::ThreadLocal<Test> testObj;
```
创建该testObj对象会调用ThreadLocal的构造函数：

```c
ThreadLocal()
{
  pthread_key_create(&pkey_, &ThreadLocal::destructor);
}

private:
  static void destructor(void *x)
  {
    T* obj = static_cast<T*>(x);
    typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
    delete obj;
  }

private:
  pthread_key_t pkey_;
```
与前面测试不一样的是，这里的ThreadLocal提供了destructor函数用于销毁对象，这一点很重要，因为muduo的ThreadLocal对象都是动态创建的，如果要获取线程局部对象，可以使用如下方法：

```c
Test t = testObj.value();
```
该方法定义如下：
```c
T& value()
{
  T* perThreadValue = static_cast<T*>(pthread_getspecific(pkey_));
  if (!perThreadValue) {
    T* newObj = new T();
    pthread_setspecific(pkey_, newObj);
    perThreadValue = newObj;
  }
  return *perThreadValue;
}
```
该函数会使用pthread_getspecific函数，根据key获取该key对应的线程局部对象，如果该对象不存在，则动态创建，并使用pthread_setspecific函数绑定该对象，最后返回该对象的值（而非指针）。

同样地，该ThreadLocal也使用RAII技法，在析构函数中销毁key资源，而整个ThreadLocal也只有pthread_key_t pkey_一个局部成员：
```c
~ThreadLocal()
{
  pthread_key_delete(pkey_);
}
```

到此，ThreadLocal分析就结束了。