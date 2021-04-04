## 模板模式 ##

Template Method模式也叫模板方法模式，是行为模式之一，它把具有特定步骤算法中的某些必要的处理委让给抽象方法，通过子类继承对抽象方法的不同实现改变整个算法的行为。

### 一、角色和职责 ###

![](https://i.imgur.com/PXEFjtZ.png)

- AbstractTemplate：抽象类的父类
- ConcreteClass：具体的实现子类
- templateMethod()：模板方法
- operation1()与operation2()：具体步骤方法  

Template Method模式一般应用在具有以下条件的应用中：

- 具有统一的操作步骤或操作过程
- 具有不同的操作细节
- 存在多个具有同样操作步骤的应用场景，但某些具体的操作细节却各不相同

总结：在抽象类中统一操作步骤，并规定好接口；让子类实现接口。这样可以把各个具体的子类和操作步骤接耦合。

### 二、案例 ###
```cpp
#include <iostream>
using namespace std;

class MakeCar
{
public:
	virtual void MakeHead() = 0;
	virtual void MakeBody() = 0;
	virtual void MakeTail() = 0;

public:
	void Make() //模板函数 把业务逻辑给做好
	{
		MakeTail();
		MakeBody();
		MakeHead();
	}
};

class Jeep : public MakeCar
{
public:
	virtual void MakeHead()
	{
		cout << "jeep head" << endl;
	}

	virtual void MakeBody()
	{
		cout << "jeep body" << endl;
	}

	virtual void MakeTail()
	{
		cout << "jeep tail" << endl;
	}
};

class Bus : public MakeCar
{
public:
	virtual void MakeHead()
	{
		cout << "Bus head" << endl;
	}

	virtual void MakeBody()
	{
		cout << "Bus body" << endl;
	}

	virtual void MakeTail()
	{
		cout << "Bus tail" << endl;
	}
};

int main(void)
{
	MakeCar *car = new Bus();
	car->Make();
	delete car;

	MakeCar *car2 = new Jeep();
	car2->Make();
	delete car2;

	return 0;
}
```
编译运行结果如下：

![](https://i.imgur.com/L2LckRG.png)


### 三、案例扩展 ###

创建一个定义操作的 Game 抽象类，其中，模板方法设置为 final，这样它就不会被重写。Cricket 和 Football 是扩展了 Game 的实体类，它们重写了抽象类的方法。

TemplatePatternDemo，演示类使用 Game 来演示模板模式的用法。

![](https://i.imgur.com/kfw7ZMR.jpg)
