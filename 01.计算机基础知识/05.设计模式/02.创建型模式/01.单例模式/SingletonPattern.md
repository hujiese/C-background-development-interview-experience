## 单例模式 ##

### 一、概念 ###

单例模式是一种对象创建型模式，使用单例模式，可以保证为一个类只生成唯一的实例对象。也就是说，在整个程序空间中，该类只存在一个实例对象。    
GoF对单例模式的定义是：保证一个类、只有一个实例存在，同时提供能对该实例加以访问的全局访问方法。

![](https://i.imgur.com/OCKocw4.png)

### 二、单例模式应用场景 ###

在应用系统开发中，我们常常有以下需求：

- 在多个线程之间，比如初始化一次socket资源；比如servlet环境，共享同一个资源或者操作同一个对象
- 在整个程序空间使用全局变量，共享资源
- 大规模系统中，为了性能的考虑，需要节省对象的创建时间等等。

因为Singleton模式可以保证为一个类只生成唯一的实例对象，所以这些情况，Singleton模式就派上用场了。


### 三、实现步骤 ###

- 1、构造函数私有化
- 2、提供一个全局的静态方法（全局访问点）
- 3、在类中定义一个静态指针，指向本类的变量的静态变量指针


### 四、具体实现 ###

#### （1）懒汉式 ####
```cpp
#include <iostream>
using namespace std;

//懒汉式
class Singelton
{
private:
	Singelton()
	{
		cout << "Singelton 构造函数执行" << endl;
	}
public:
	static Singelton *getInstance()
	{
		if (m_psl == NULL)
		{
			m_psl = new Singelton();
		}
		return m_psl;
	}

	static void FreeInstance()
	{
		if (m_psl != NULL)
		{
			delete m_psl;
			m_psl = NULL; 
		}
	}

private:
	static Singelton *m_psl;
};

Singelton *Singelton::m_psl = NULL;


int main(void)
{
	
	Singelton *p1 = Singelton::getInstance();
	Singelton *p2 = Singelton::getInstance();

	if (p1 == p2)
	{
		cout << "是同一个对象" << endl;
	}
	else
	{
		cout << "不是同一个对象" << endl;
	}
	Singelton::FreeInstance();

	return 0;
}
```
运行结果如下：

![](https://i.imgur.com/VrDmKvL.png)

#### （2）饿汉式 ####
```cpp
#include <iostream>
using namespace std;


//懒汉式
class Singelton
{
private:
	Singelton()
	{
		cout << "Singelton 构造函数执行" << endl;
	}
public:
	static Singelton *getInstance()
	{
		return m_psl;
	}

	static void FreeInstance()
	{
		if (m_psl != NULL)
		{
			delete m_psl;
			m_psl = NULL; 
		}
	}

private:
	static Singelton *m_psl;
};

//int g_count = 0;
//饿汉式
Singelton *Singelton::m_psl = new Singelton();


int main(void)
{
	Singelton *p1 = Singelton::getInstance();
	Singelton *p2 = Singelton::getInstance();

	if (p1 == p2)
	{
		cout << "是同一个对象" << endl;
	}
	else
	{
		cout << "不是同一个对象" << endl;
	}
	Singelton::FreeInstance();

	return 0;
}
```
运行结果如下：

![](https://i.imgur.com/VrDmKvL.png)

### 五、多线程下的懒汉式单例和饿汉式 ###

"懒汉"模式虽然有优点，但是每次调用GetInstance()静态方法时，必须判断NULL == m_instance，使程序相对开销增大。多线程中会导致多个实例的产生，从而导致运行代码不正确以及内存的泄露。

C++中构造函数并不是线程安全的，C++中的构造函数简单来说分两步：

- 第一步：内存分配
- 第二步：初始化成员变量
        由于多线程的关系，可能当我们在分配内存好了以后，还没来得急初始化成员变量，就进行线程切换，另外一个线程拿到所有权后，由于内存已经分配了，但是变量初始化还        没进行，因此打印成员变量的相关值会发生不一致现象。

所以多线程下建议使用饿汉式，如果使用懒汉示则需要加锁同步。

### 六、案例扩展 ###

创建一个 SingleObject 类。SingleObject 类有它的私有构造函数和本身的一个静态实例。

SingleObject 类提供了一个静态方法，供外界获取它的静态实例。SingletonPatternDemo，我们的演示类使用 SingleObject 类来获取 SingleObject 对象。

![](https://i.imgur.com/AqfIkb8.jpg)