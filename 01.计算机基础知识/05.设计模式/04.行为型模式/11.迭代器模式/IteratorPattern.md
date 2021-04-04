## 迭代器模式 ##

Iterator模式也叫迭代模式，是行为模式之一，它把对容器中包含的内部对象的访问委让给外部类，使用Iterator（遍历）按顺序进行遍历访问的设计模式。

在应用Iterator模式之前，首先应该明白Iterator模式用来解决什么问题。或者说，如果不使用Iterator模式，会存在什么问题。

- 1、由容器自己实现顺序遍历。直接在容器类里直接添加顺序遍历方法 
- 2、让调用者自己实现遍历。直接暴露数据细节给外部。

以上方法1与方法2都可以实现对遍历，这样有问题呢？

- 1、容器类承担了太多功能：一方面需要提供添加删除等本身应有的功能；一方面还需要提供遍历访问功能。
- 2、往往容器在实现遍历的过程中，需要保存遍历状态，当跟元素的添加删除等功能夹杂在一起，很容易引起混乱和程序运行错误等。

Iterator模式就是为了有效地处理按顺序进行遍历访问的一种设计模式，简单地说，Iterator模式提供一种有效的方法，可以屏蔽聚集对象集合的容器类的实现细节，而能对容器内包含的对象元素按顺序进行有效的遍历访问。所以，Iterator模式的应用场景可以归纳为满足以下几个条件：

- 访问容器中包含的内部对象 
- 按顺序访问

### 一、角色和职责 ###

提供一种方法顺序访问一个聚敛对象的各个元素，而又不暴露该对象的内部表示。为遍历不同的聚集结构提供如开始，下一个，是否结束，当前一项等统一接口。

![](https://i.imgur.com/8XRrx9C.png)

- Iterator（迭代器接口）：该接口必须定义实现迭代功能的最小定义方法集
比如提供hasNext()和next()方法。
- ConcreteIterator（迭代器实现类）：迭代器接口Iterator的实现类。可以根据具体情况加以实现。
- Aggregate（容器接口）：定义基本功能以及提供类似Iterator iterator()的方法。
- concreteAggregate（容器实现类）：容器接口的实现类。必须实现Iterator iterator()方法。

结论：迭代器持有一个集合的引用，所以通过迭代器，就可以访问该集合。

### 二、案例 ###
```cpp
#include <iostream>
using namespace std;
typedef int Object;
#define SIZE 5 

class MyIterator
{
public:
	virtual void First() = 0;
	virtual void Next() = 0;
	virtual bool IsDone() = 0;
	virtual Object CurrentItem() = 0;
};

class Aggregate
{
public:
	virtual MyIterator *CreateIterator() = 0;
	virtual Object getItem(int index) = 0;
	virtual int getSize() = 0;
};

class ContreteIterator : public MyIterator
{
public:
	ContreteIterator(Aggregate *ag)
	{
		_ag = ag;
		_current_index = 0;
	}
	virtual void First()
	{
		_current_index = 0;  //让当前 游标 回到位置0
	}
	virtual void Next()
	{
		if (_current_index < _ag->getSize())
		{
			_current_index ++;
		}
	}
	virtual bool IsDone()
	{
		return  (_current_index ==  _ag->getSize());
	}
	virtual Object CurrentItem()
	{
		return _ag->getItem(_current_index);
	}
private:
	int			_current_index;
	Aggregate	 *_ag;
};


class ConcreteAggregate : public Aggregate
{
public:
	ConcreteAggregate()
	{
		for (int i=0; i<SIZE; i++)
		{
			object[i] = i + 100;
		}
	}
	virtual MyIterator *CreateIterator()
	{
		return new ContreteIterator(this); //让迭代器 持有一个 集合的 引用 
	}
	virtual Object getItem(int index)
	{
		return object[index];
	}
	virtual int getSize()
	{
		return SIZE;
	}
private:
	Object object[SIZE]; 
};

int main(void)
{
	Aggregate * ag = new ConcreteAggregate;

	MyIterator *it = ag->CreateIterator();

	for (; !(it->IsDone()); it->Next() )
	{
		cout << it->CurrentItem() << " ";
	}

	cout << endl;
	delete it;
	delete ag;

	return 0;
}
```
编译运行结果如下：

![](https://i.imgur.com/xKSOxh4.png)

