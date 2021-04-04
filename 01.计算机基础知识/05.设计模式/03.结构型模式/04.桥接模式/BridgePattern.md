## 桥接模式 ##

Bridge 模式又叫做桥接模式，是构造型的设计模式之一。Bridge模式基于类的最小设计原则，通过使用封装，聚合以及继承等行为来让不同的类承担不同的责任。它的主要特点是把抽象（abstraction）与行为实现（implementation）分离开来，从而可以保持各部分的独立性以及应对它们的功能扩展。

### 一、角色和职责 ###

![](https://i.imgur.com/M4YID1Q.png)

- Abstraction：抽象类接口（接口或抽象类）维护对行为实现（Implementor）的引用
- Concrete：Abstraction子类
- Implementation：行为实现类接口 (Abstraction接口定义了基于Implementation接口的更高层次的操作)
- Imp：Implementation子类

桥接模式（Bridge Pattern）是将抽象部分与实现部分分离（解耦合），使它们都可以独立的变化。

	车 安装 发动机 | 不同型号的车，安装不同型号的发动机
	图形 填 颜色   | 不同形状的图形，填充上不同的颜色
	
将“车 安装 发动机”这个抽象 和 实现进行分离；两个名字 就设计两个类

将“图形 填 颜色”这个抽象 和 实现 进行分离，两个名字，就设计两个类

### 二、案例 ###
```cpp
#include <iostream>
using namespace std;

class Engine
{
public:
	virtual void InstallEngine() = 0;
};

class Engine4400cc : public Engine
{
public:
	virtual void InstallEngine()
	{
		cout << "我是 4400cc 发动机 安装完毕 " << endl;
	}
};


class Engine4500cc : public Engine
{
public:
	virtual void InstallEngine()
	{
		cout << "我是 4500cc 发动机 安装完毕 " << endl;
	}
};

class Car
{
public:
	Car(Engine *engine) :m_engine(engine)
	{
	}
	virtual void installEngine() = 0;

protected:
	Engine *m_engine;
};

class WBM5 : public Car
{
public:
	WBM5(Engine *engine) : Car(engine)
	{
	}

	virtual void installEngine()
	{
		m_engine->InstallEngine();
	}
};

class WBM6 : public Car
{
public:
	WBM6(Engine *engine) : Car(engine)
	{
	}

	virtual void installEngine()
	{
		cout << "我是WBM6 " << endl;
		m_engine->InstallEngine();
	}
};

int main(void)
{
	Engine	*engine = new Engine4400cc();
	WBM6	*wbm6 = new WBM6(engine);
	wbm6->installEngine();

	delete wbm6;
	delete engine;

	return 0;
}
```
编译运行结果如下：

![](https://i.imgur.com/Y7XvJQe.png)

### 三、扩展案例 ###

有一个作为桥接实现的 DrawAPI 接口和实现了 DrawAPI 接口的实体类 RedCircle、GreenCircle。Shape 是一个抽象类，将使用 DrawAPI 的对象。BridgePatternDemo，演示类使用 Shape 类来画出不同颜色的圆。

![](https://i.imgur.com/CpkTMSL.jpg)

