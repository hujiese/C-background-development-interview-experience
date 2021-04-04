## 责任链模式 ##

Chain of Responsibility（CoR）模式也叫职责链模式或者职责连锁模式，是行为模式之一，该模式构造一系列分别担当不同的职责的类的对象来共同完成一个任务，这些类的对象之间像链条一样紧密相连，所以被称作职责链模式。

- 例1：比如客户Client要完成一个任务，这个任务包括a,b,c,d四个部分。 
首先客户Client把任务交给A，A完成a部分之后，把任务交给B，B完成b部分，...，直到D完成d部分。
- 例2：比如政府部分的某项工作，县政府先完成自己能处理的部分，不能处理的部分交给省政府，省政府再完成自己职责范围内的部分，不能处理的部分交给中央政府，中央政府最后完成该项工作。
- 例3：软件窗口的消息传播。	
- 例4：SERVLET容器的过滤器（Filter）框架实现。

### 一、角色和职责 ###

![](https://i.imgur.com/HnsT0A8.png)

- Handler：处理类的抽象父类。
- Handler_X：具体的处理类。

优点：

- 1、责任的分担。每个类只需要处理自己该处理的工作（不该处理的传递给下一个对象完成），明确各类的责任范围，符合类的最小封装原则。
- 2、可以根据需要自由组合工作流程。如工作流程发生变化，可以通过重新分配对象链便可适应新的工作流程。
- 3、类与类之间可以以松耦合的形式加以组织。

缺点：
因为处理时以链的形式在对象间传递消息，根据实现方式不同，有可能会影响处理的速度。

适用于：
	链条式处理事情。工作流程化、消息处理流程化、事物流程化；


### 二、案例 ###

演变一（不使用责任链模式）：
```cpp
#include <iostream>
using namespace std;

class CarHandle
{
public:
	virtual void HandleCar() = 0;
};

class HeadCarHandle : public CarHandle
{
public:
	virtual void HandleCar()
	{
		cout << "造 车头" << endl;
	}
};

class BodyCarHandle : public CarHandle
{
public:
	virtual void HandleCar()
	{
		cout << "造 车身" << endl;
	}
};

class TailCarHandle : public CarHandle
{
public:
	virtual void HandleCar()
	{
		cout << "造 车尾" << endl;
	}
};


int main(void)
{
	CarHandle *headHandle = new HeadCarHandle();
	CarHandle *bodyHandle = new BodyCarHandle();
	CarHandle *tailHandle = new TailCarHandle();

	//业务逻辑 写死在客户端了..
	headHandle->HandleCar();
	bodyHandle->HandleCar();
	tailHandle->HandleCar();

	delete headHandle;
	delete bodyHandle;
	delete tailHandle;

	return 0;
}
```
编译运行结果如下：

![](https://i.imgur.com/jKyUdSR.png)

演变二（使用责任链模式）：
```cpp
#include <iostream>
using namespace std;

//造完车 以后,需要把任务 传递下去
class CarHandle
{
public:
	virtual void HandleCar() = 0;
	CarHandle *setNextHandle(CarHandle *handle)
	{
		m_handle = handle;
		return m_handle;
	}
	
protected:
	CarHandle *m_handle;  //下一个处理单元
};

class HeadCarHandle : public CarHandle
{
public:
	virtual void HandleCar()
	{
		cout << "造车头" << endl;
		//造 玩车头 以后 把任务递交给 下一个处理者
		if (m_handle != NULL) 
		{
			m_handle->HandleCar(); //
		}
	}
};

class BodyCarHandle : public CarHandle
{
public:
	virtual void HandleCar()
	{
		cout << "造 车身" << endl;
		//造 造 车身 以后 把任务递交给 下一个处理者
		if (m_handle != NULL) 
		{
			m_handle->HandleCar(); //
		}

	}
};

class TailCarHandle : public CarHandle
{
public:
	virtual void HandleCar()
	{
		cout << "造车尾" << endl;
		//造 造车尾 以后 把任务递交给 下一个处理者
		if (m_handle != NULL) 
		{
			m_handle->HandleCar(); //
		}
	}
};


int main(void)
{
	CarHandle *headHandle = new HeadCarHandle();
	CarHandle *bodyHandle = new BodyCarHandle();
	CarHandle *tailHandle = new TailCarHandle();
		
	headHandle->setNextHandle(tailHandle);
	tailHandle->setNextHandle(bodyHandle);
	bodyHandle->setNextHandle(NULL);
	
	headHandle->HandleCar();

	delete headHandle;
	delete bodyHandle;
	delete tailHandle;

	return 0;
}
```
编译运行结果如下：

![](https://i.imgur.com/hmtQvI1.png)


### 三、扩展案例 ###

创建抽象类 AbstractLogger，带有详细的日志记录级别。然后创建三种类型的记录器，都扩展了 AbstractLogger。每个记录器消息的级别是否属于自己的级别，如果是则相应地打印出来，否则将不打印并把消息传给下一个记录器。

![](https://i.imgur.com/9a9T0eK.jpg)