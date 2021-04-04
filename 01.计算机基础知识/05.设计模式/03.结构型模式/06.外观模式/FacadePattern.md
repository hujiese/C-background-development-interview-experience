## 外观模式 ##

Facade模式也叫外观模式，是由GoF提出的23种设计模式中的一种。Facade模式为一组具有类似功能的类群，比如类库，子系统等等，提供一个一致的简单的界面。这个一致的简单的界面被称作facade。

### 一、角色和职责 ###

![](https://i.imgur.com/Ue73SFA.png)

- Façade：为调用方, 定义简单的调用接口。
- Client：调用者。通过Facade接口调用提供某功能的内部类群。

适用于：为子系统中统一一套接口，让子系统更加容易使用。

### 二、案例 ###
```cpp
#include <iostream>
using namespace std;

class SubSystemA
{
public:
	void doThing()
	{
		cout << "SubSystemA run" << endl;
	}
};

class SubSystemB
{
public:
	void doThing()
	{
		cout << "SubSystemB run" << endl;
	}
};

class SubSystemC
{
public:
	void doThing()
	{
		cout << "SubSystemC run" << endl;
	}
};


class Facade
{
public:
	Facade()
	{
		sysA = new SubSystemA();
		sysB = new SubSystemB();
		sysC = new SubSystemC();
	}
	~Facade()
	{
		delete sysA;
		delete sysB;
		delete sysC;
	}
public:
	void doThing()
	{
		sysA->doThing();
		sysB->doThing();
		sysC->doThing();
	}
private:
	SubSystemA *sysA;
	SubSystemB *sysB;
	SubSystemC *sysC;
};

void main1801()
{
	SubSystemA *sysA = new SubSystemA();
	SubSystemB *sysB = new SubSystemB();
	SubSystemC *sysC = new SubSystemC();

	sysA->doThing();
	sysB->doThing();
	sysC->doThing();
	delete sysA;
	delete sysB;
	delete sysC;
	
	return ;
}

void main1802()
{
	Facade *f = new Facade();
	f->doThing();
	delete f;

}

int main(void)
{
	//main1801();
	main1802();

	return 0;
}
```
编译运行结果如下：

![](https://i.imgur.com/cy1hiew.png)

### 三、案例扩展 ###

创建一个 Shape 接口和实现了 Shape 接口的实体类。下一步是定义一个外观类 ShapeMaker。

ShapeMaker 类使用实体类来代表用户对这些类的调用。FacadePatternDemo，演示类使用 ShapeMaker 类来显示结果。

![](https://i.imgur.com/25CAtfT.jpg)