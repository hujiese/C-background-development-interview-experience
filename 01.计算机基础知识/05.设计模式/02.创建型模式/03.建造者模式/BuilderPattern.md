## 建造者模式 ##

Builder模式也叫建造者模式或者生成器模式，是由GoF提出的23种设计模式中的一种。Builder模式是一种对象创建型模式之一，用来隐藏复合对象的创建过程，它把复合对象的创建过程加以抽象，通过子类继承和重载的方式，动态地创建具有复合属性的对象。

对象的创建：Builder模式是为对象的创建而设计的模式- 创建的是一个复合对象：被创建的对象为一个具有复合属性的复合对象- 关注对象创建的各部分的创建过程：不同的工厂（这里指builder生成器）对产品属性有不同的创建方法。

### 一、角色和职责 ###

![](https://i.imgur.com/aODEpJo.png)

- 1) Builder：为创建产品各个部分，统一抽象接口。
- 2) ConcreteBuilder：具体的创建产品的各个部分，部分A， 部分B，部分C。
- 3) Director：构造一个使用Builder接口的对象。

ConcreteBuilder创建该产品的内部表示并定义它的装配过程，包含定义组成部件的类，包括将这些部件装配成最终产品的接口。

适用情况：一个对象的构建比较复杂，将一个对象的构建和对象的表示进行分离。

### 二、案例 ###
```cpp
#include <iostream>
using namespace std;

#include "string"

class House
{
public:
	void setDoor(string door)
	{
		this->m_door = door;
	}

	void setWall(string wall)
	{
		this->m_wall = wall;
	}

	void setWindow(string window)
	{
		this->m_window = window;
	}

	string getDoor()
	{
		cout << m_door << endl;
		return this->m_door ;
	}

	string getWall()
	{
		cout << m_wall << endl;
		return this->m_wall;
	}

	string getWindow()
	{
		cout << m_window << endl;
		return m_window;
	}

private:
	string	m_door;
	string	m_wall;
	string	m_window;
};

class  Builder
{
public:
	virtual void buildWall() = 0;
	virtual void buildDoor() = 0;
	virtual void buildWindow() = 0;
	virtual House* getHouse() = 0;
};

//公寓工程队
class  FlatBuilder : public Builder
{
public:
	FlatBuilder()
	{
		m_house = new House();
	}
	virtual void buildWall()
	{
		m_house->setWall(" flat wall");
	}

	virtual void buildDoor()
	{
		m_house->setDoor("flat door");
	}

	virtual void buildWindow()
	{
		m_house->setWindow("flat window");
	}

	virtual House* getHouse()
	{
		return m_house;
	}
private:
	House *m_house;
};

//别墅 villa 工程队
class  VillaBuilder : public Builder
{
public:
	VillaBuilder()
	{
		m_house = new House();
	}
	virtual void buildWall()
	{
		m_house->setWall(" villa wall");
	}

	virtual void buildDoor()
	{
		m_house->setDoor("villa door");
	}

	virtual void buildWindow()
	{
		m_house->setWindow("villa window");
	}

	virtual House* getHouse()
	{
		return m_house;
	}
private:
	House *m_house;
};

//设计师(指挥者) 负责建造逻辑  
//建筑队 干具体的活
class Director 
{
public:
	Director( Builder * build)
	{
		m_build = build;
	}
	void Construct()
	{
		m_build->buildWall();
		m_build->buildWindow();
		m_build->buildDoor();
	}
private:
		Builder * m_build;
};


int main(void)
{
	House		*house  = NULL;
	Builder		*builder = NULL;
	Director	*director = NULL;

	// 请一个建造别墅的工程队
	builder = new VillaBuilder();

	//设计师 指挥 工程队 干活
	director = new Director(builder);
	director->Construct(); 
	house  =  builder->getHouse();
	house->getWindow();
	house->getDoor();
	delete house;
	delete builder;

	//请 FlatBuilder 公寓
	builder = new FlatBuilder;
	director = new Director(builder);
	director->Construct(); 
	house  =  builder->getHouse();
	house->getWindow();
	house->getDoor();
	delete house;
	delete builder;


	delete director;
	return 0;
}
```
编译运行结果如下：

![](https://i.imgur.com/G0PTS28.png)

