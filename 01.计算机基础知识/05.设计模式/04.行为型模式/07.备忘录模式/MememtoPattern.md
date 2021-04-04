## 备忘录模式 ##

Memento模式也叫备忘录模式，是行为模式之一，它的作用是保存对象的内部状态，并在需要的时候（undo/rollback）恢复对象以前的状态。

应用场景:

如果一个对象需要保存状态并可通过undo或rollback等操作恢复到以前的状态时，可以使用Memento模式。

- 1）一个类需要保存它的对象的状态（相当于Originator角色）
- 2）设计一个类，该类只是用来保存上述对象的状态（相当于Memento角色）
- 3）需要的时候，Caretaker角色要求Originator返回一个Memento并加以保存
- 4）undo或rollback操作时，通过Caretaker保存的Memento恢复Originator对象的状态


### 一、角色和职责 ###

![](https://i.imgur.com/70yIbM4.png)

- Originator（原生者）：需要被保存状态以便恢复的那个对象。
- Memento（备忘录）：该对象由Originator创建，主要用来保存Originator的内部状态。
- Caretaker（管理者）：负责在适当的时间保存/恢复Originator对象的状态。

适用于：

在不破坏封装性的前提下，捕获一个对象的内部状态，并在该对象之外保存这个状态，这样就可以将以后的对象状态恢复到先前保存的状态。
适用于功能比较复杂的，但需要记录或维护属性历史的类；或者需要保存的属性只是众多属性中的一小部分时Originator可以根据保存的Memo还原到前一状态。

### 二、案例 ###
```cpp
#include <iostream>
#include <string>
#define __CARETAKER__
using namespace std;

//Caretaker 管理者
// MememTo  备忘录

class MememTo
{
public:
	MememTo(string name, int age) :m_name(name), m_age(age)
	{
	}
	string getName() const
	{
		return m_name;
	}
	int getAge() const
	{
		return m_age;
	}

	void setName(string name)
	{
		this->m_name = name;
	}
	void setAge(int age)
	{
		this->m_age = age;
	}
private:
	string	m_name;
	int		m_age;
};

class  Person
{
public:
	Person(string name, int age) :m_name(name), m_age(age)
	{
	}
	string getName() const
	{
		return m_name;
	}
	int getAge() const
	{
		return m_age;
	}

	void setName(string name)
	{
		this->m_name = name;
	}
	void setAge(int age)
	{
		this->m_age = age;
	}

	//保存
	MememTo* createMememTo()
	{
		return new MememTo(m_name, m_age);
	}
	//还原 
	void setMememTo(MememTo* memto)
	{
		this->m_age = memto->getAge();
		this->m_name = memto->getName();
	}
public:
	void printT()
	{
		cout << "m_name:" << m_name << " m_age:" << m_age << endl;
	}
private:
	string	m_name;
	int		m_age;
};

class Caretaker
{
public:
	Caretaker(MememTo *memto) :memto(memto)
	{
	}
	MememTo *getMememTo() const
	{
		return memto;
	}
	void setMememTo(MememTo *memto)
	{
		this->memto = memto;
	}
private:
	MememTo *memto;
};

#ifdef __CARETAKER__
int main(void)
{
	Caretaker *caretaker = NULL;
	Person *p = new Person("张三", 32);
	p->printT();

	//创建 Person对象的一个状态
	printf("---------\n");
	caretaker = new Caretaker( p->createMememTo());
	p->setAge(42);
	p->printT();

	printf("还原旧的状态\n" );
	p->setMememTo(caretaker->getMememTo());
	p->printT();

	delete caretaker;
	delete p;

	return 0;
}

#else
int main(void)
{
	MememTo *memto = NULL;
	Person *p = new Person("张三", 32);
	p->printT();

	//创建 Person对象的一个状态
	printf("---------\n");
	memto = p->createMememTo();
	p->setAge(42);
	p->printT();

	printf("还原旧的状态\n" );
	p->setMememTo(memto);
	p->printT();

	delete memto;
	delete p;

	return 0;
}
#endif
```
编译运行结果如下：

![](https://i.imgur.com/5SU2yrw.png)

### 三、案例扩展 ###

备忘录模式使用三个类 Memento、Originator 和 CareTaker。Memento 包含了要被恢复的对象的状态。Originator 创建并在 Memento 对象中存储状态。Caretaker 对象负责从 Memento 中恢复对象的状态。

MementoPatternDemo，演示类使用 CareTaker 和 Originator 对象来显示对象的状态恢复。

![](https://i.imgur.com/OPVy0PT.jpg)