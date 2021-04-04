## 简单工厂模式 ##

简单工厂模式属于类的创建型模式,又叫做静态工厂方法模式。通过专门定义一个类来负责创建其他类的实例，被创建的实例通常都具有共同的父类。

### 一、模式中包含的角色及其职责 ###

- 1.工厂（Creator）角色
简单工厂模式的核心，它负责实现创建所有实例的内部逻辑。工厂类可以被外界直接调用，创建所需的产品对象。
- 2.抽象（Product）角色
简单工厂模式所创建的所有对象的父类，它负责描述所有实例所共有的公共接口。
- 3.具体产品（Concrete Product）角色
简单工厂模式所创建的具体实例对象

![](https://i.imgur.com/kFCIduY.png)

在这个模式中，工厂类是整个模式的关键所在。它包含必要的判断逻辑，能够根据外界给定的信息，决定究竟应该创建哪个具体类的对象。用户在使用时可以直接根据工厂类去创建所需的实例，而无需了解这些对象是如何创建以及如何组织的。有利于整个软件体系结构的优化。不难发现，简单工厂模式的缺点也正体现在其工厂类上，由于工厂类集中了所有实例的创建逻辑，所以“高内聚”方面做的并不好。另外，当系统中的具体产品类不断增多时，可能会出现要求工厂类也要做相应的修改，扩展性并不很好。

### 二、案例 ###

```cpp
#include <iostream>
using namespace std;

class Fruit 
{
public:
	virtual void getFruit() = 0;
};

class Banana : public Fruit
{
public:
	virtual void getFruit()
	{
		cout << "我是香蕉...." << endl;
	}
};

class Apple : public Fruit
{
public:
	virtual void getFruit()
	{
		cout << "我是苹果...." << endl;
	}
};


class Factory
{
public:
	Fruit *create(char *p)
	{

		if (strcmp(p, "banana") == 0)
		{
			return new Banana;	 
		}
		else if (strcmp(p, "apple") == 0)
		{
			return new Apple;
		}
		else
		{
			printf("不支持\n" ) ;
			return NULL;
		}
	}
};


int main(void)
{
	Factory *f = new Factory;

	Fruit *fruit = NULL;


	//工厂生产 香蕉
	fruit = f->create("banana");
	fruit->getFruit();
	delete fruit;


	fruit = f->create("apple");
	fruit->getFruit();
	delete fruit;

	delete f;
	return 0;
}
```
编译运行结果如下：

![](https://i.imgur.com/dXI56eB.png)

### 三、案例扩展 ###

创建一个 Shape 接口和实现 Shape 接口的实体类。下一步是定义工厂类 ShapeFactory。

FactoryPatternDemo，演示类使用 ShapeFactory 来获取 Shape 对象。它将向 ShapeFactory 传递信息（CIRCLE / RECTANGLE / SQUARE），以便获取它所需对象的类型。

![](https://i.imgur.com/Q4LDr7j.jpg)