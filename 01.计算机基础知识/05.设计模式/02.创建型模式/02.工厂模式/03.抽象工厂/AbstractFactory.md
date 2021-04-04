## 抽象工厂 ##

抽象工厂模式是所有形态的工厂模式中最为抽象和最其一般性的。抽象工厂模式可以向客户端提供一个接口，使得客户端在不必指定产品的具体类型的情况下，能够创建多个产品族的产品对象

工厂模式只能生产一个产品（要么香蕉、要么苹果）；
抽象工厂可以一下生产一个产品族（里面有很多产品组成）。

具体工厂在开闭原则下,	能生产香蕉/苹果/梨子;  (产品等级结构)。抽象工厂:在开闭原则下,	能生产：南方香蕉/苹果/梨子 (产品族)、北方香蕉/苹果/梨子。

### 一、模式中包含的角色及其职责 ###

1. 抽象工厂（Creator）角色 ：抽象工厂模式的核心，包含对多个产品结构的声明，任何工厂类都必须实现这个接口。
2. 具体工厂（ Concrete  Creator）角色：具体工厂类是抽象工厂的一个实现，负责实例化某个产品族中的产品对象。
3. 抽象（Product）角色：抽象模式所创建的所有对象的父类，它负责描述所有实例所共有的公共接口。
4. 具体产品（Concrete Product）角色：抽象模式所创建的具体实例对象

![](https://i.imgur.com/KAY72eg.png)


### 二、案例 ###

```cpp
#include <iostream>
using namespace std;

class Fruit
{
public:
	virtual void SayName() = 0;
};

class AbstractFactory
{
public:
	virtual Fruit* CreateBanana() = 0;
	virtual Fruit* CreateApple() = 0;
};

class NorthBanana : public Fruit
{
public:
	virtual void SayName()
	{
		cout << "我是北方香蕉" << endl;
	}
};

class NorthApple : public Fruit
{
public:
	virtual void SayName()
	{
		cout << "我是北方苹果" << endl;
	}
};


class SourthBanana : public Fruit
{
public:
	virtual void SayName()
	{
		cout << "我是南方香蕉" << endl;
	}
};


class SourthApple : public Fruit
{
public:
	virtual void SayName()
	{
		cout << "我是南方苹果" << endl;
	}
};

class NorthFacorty : public AbstractFactory
{
	virtual Fruit * CreateBanana()
	{
		return new NorthBanana();
	}
	virtual Fruit * CreateApple()
	{
		return new NorthApple();
	}
};

class SourthFacorty : public AbstractFactory
{
	virtual Fruit * CreateBanana()
	{
		return new SourthBanana();
	}
	virtual Fruit * CreateApple()
	{
		return new SourthApple();
	}
};


int main(void)
{
	Fruit			*fruit = NULL;
	AbstractFactory *af = NULL;

	af = new SourthFacorty;
	fruit = af->CreateApple();
	fruit->SayName();
	delete fruit;
	fruit = af->CreateBanana();
	fruit->SayName();
	delete fruit;

	af = new NorthFacorty;
	fruit = af->CreateApple();
	fruit->SayName();
	delete fruit;
	fruit = af->CreateBanana();
	fruit->SayName();
	delete fruit;

	return 0;
}
```
编译运行结果如下：

![](https://i.imgur.com/37W29C6.png)


### 三、案例扩展 ###

创建 Shape 和 Color 接口和实现这些接口的实体类。下一步是创建抽象工厂类 AbstractFactory。接着定义工厂类 ShapeFactory 和 ColorFactory，这两个工厂类都是扩展了 AbstractFactory。然后创建一个工厂创造器/生成器类 FactoryProducer。

AbstractFactoryPatternDemo，演示类使用 FactoryProducer 来获取 AbstractFactory 对象。它将向 AbstractFactory 传递形状信息 Shape（CIRCLE / RECTANGLE / SQUARE），以便获取它所需对象的类型。同时它还向 AbstractFactory 传递颜色信息 Color（RED / GREEN / BLUE），以便获取它所需对象的类型。

![](https://i.imgur.com/ebEU4T7.jpg)


