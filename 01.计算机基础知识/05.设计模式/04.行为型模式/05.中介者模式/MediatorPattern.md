## 中介者模式 ##

Mediator模式也叫中介者模式，是由GoF提出的23种软件设计模式的一种。Mediator模式是行为模式之一，在Mediator模式中，类之间的交互行为被统一放在Mediator的对象中，对象通过Mediator对象同其他对象交互，Mediator对象起着控制器的作用。

### 一、角色和职责 ###

![](https://i.imgur.com/mY4gKnu.png)

- Mediator抽象中介者：中介者类的抽象父类。
- concreteMediator：具体的中介者类。
- Colleague：关联类的抽象父类。
- concreteColleague：具体的关联类。

适用于：用一个中介对象，封装一些列对象（同事）的交换，中介者是各个对象不需要显示的相互作用，从而实现了耦合松散，而且可以独立的改变他们之间的交换。

优点：

- 1、将系统按功能分割成更小的对象，符合类的最小设计原则
- 2、对关联对象的集中控制
- 3、减小类的耦合程度，明确类之间的相互关系：当类之间的关系过于复杂时，其中任何一个类的修改都会影响到其他类，不符合类的设计的开闭原则 ，而Mediator模式将原来相互依存的多对多的类之间的关系简化为Mediator控制类与其他关联类的一对多的关系，当其中一个类修改时，可以对其他关联类不产生影响（即使有修改，也集中在Mediator控制类）。
- 4、有利于提高类的重用性

### 二、案例 ###

演变一（不使用中介者模式）：
```cpp
#include <iostream>
#include <string>
using namespace std;

class Person
{
public:
	Person(string name, int sex, int condi) :m_name(name), m_sex(sex), m_condi(condi)
	{
	}
	string getName() const
	{
		return m_name;
	}
	int getSex() const
	{
		return m_sex;
	}
	int getCondi() const
	{
		return m_condi;
	}
	virtual void getParter(Person *p) = 0;

protected:
	string	m_name;
	int		m_sex;
	int		m_condi;
};


class Woman : public Person
{
public:
	Woman(string name, int sex, int condi) : Person(name, sex, condi)
	{
	}
	virtual void getParter(Person *p)
	{
		if (this->m_sex == p->getSex())
		{
			cout << "我不是同性恋..." << endl;
		}
		if (this->getCondi() == p->getCondi() )
		{
			cout << this->getName() << " 和 " << p->getName() << "绝配 " <<endl;
		}
		else
		{
			cout << this->getName() << " 和 " << p->getName() << "不配 " <<endl;
		}
	}
};


class Man : public Person
{
public:
	Man(string name, int sex, int condi) : Person(name, sex, condi)
	{
	}
	virtual void getParter(Person *p)
	{
		if (this->m_sex == p->getSex())
		{
			cout << "我不是同性恋..." << endl;
		}
		if (this->getCondi() == p->getCondi() )
		{
			cout << this->getName() << " 和 " << p->getName() << "绝配 " <<endl;
		}
		else
		{
			cout << this->getName() << " 和 " << p->getName() << "不配 " <<endl;
		}
	}
};

int main(void)
{
	//string name, int sex, int condi
	Person *xiaofang = new Woman("小芳", 2, 5);

	Person *zhangsan = new Man("张三", 1, 4);

	Person *lisi = new Man("李四", 1, 5);
	xiaofang->getParter(zhangsan);

	xiaofang->getParter(lisi);

	delete xiaofang;
	delete zhangsan;
	delete lisi;
	return 0;
}
```
编译运行结果如下：

![](https://i.imgur.com/m3qmOWr.png)

演变二（使用中介者模式）：
```cpp
#include <iostream>
#include <string>
using namespace std;

class Mediator;
class Mediator
{
public:
	void setMan(Person *man)
	{
		pMan = man;
	}
	void setWoman(Person *woman)
	{
		pWoman = woman;
	}

public:
	virtual void getParter()
	{

		if (pWoman->getSex() == pMan->getSex())
		{
			cout << "同性恋 之间 不能找对象 " << endl;
		}
		if (pWoman->getCondi() == pMan->getCondi())
		{
			cout << pWoman->getName() << " 和 " << pMan->getName() << "绝配 " << endl;
		}
		else
		{
			cout << pWoman->getName() << " 和 " << pMan->getName() << "不配 " << endl;
		}
	}
private:
	Person	*pMan;
	Person	*pWoman; 
};

class Person
{
public:
	Person(string name, int sex, int condi, Mediator *m) :m_name(name), m_sex(sex), m_condi(condi), mediator(m)
	{
	}

	string getName() const 
	{
		return m_name;
	}
	int getSex() const
	{
		return m_sex;
	}
	int getCondi() const
	{
		return m_condi;
	}
	virtual void getParter(Person *p) = 0;

protected:
	string	m_name;
	int		m_sex;
	int		m_condi;
	Mediator *mediator;
};


class Woman : public Person
{
public:
	Woman(string name, int sex, int condi, Mediator *m) : Person(name, sex, condi, m)
	{

	}
	virtual void getParter(Person *p)
	{
		mediator->setMan(p);
		mediator->setWoman(this);
		mediator->getParter(); //找对象 
	}
};


class Man : public Person
{
public:
	Man(string name, int sex, int condi, Mediator *m) : Person(name, sex, condi, m)
	{

	}
	virtual void getParter(Person *p)
	{
		mediator->setMan(this);
		mediator->setWoman(p);
		mediator->getParter(); //找对象 
	}
};

int main(void)
{
	//string name, int sex, int condi
	Mediator *m = new Mediator();
	Person *xiaofang = new Woman("小芳", 2, 5, m);

	Person *zhangsan = new Man("张三", 1, 4, m);

	Person *lisi = new Man("李四", 1, 5, m);
	xiaofang->getParter(zhangsan);

	xiaofang->getParter(lisi);

	return 0;
}
```
编译运行结果如下：

![](https://i.imgur.com/9ptiU7Z.png)

### 三、案例扩展 ###

通过聊天室实例来演示中介者模式。实例中，多个用户可以向聊天室发送消息，聊天室向所有的用户显示消息。创建两个类 ChatRoom 和 User。User 对象使用 ChatRoom 方法来分享他们的消息。

MediatorPatternDemo，演示类使用 User 对象来显示他们之间的通信。

![](https://i.imgur.com/xcNuyKt.jpg)