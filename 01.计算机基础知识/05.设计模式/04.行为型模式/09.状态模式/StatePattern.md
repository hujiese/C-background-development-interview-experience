## 状态模式 ##

State模式也叫状态模式，是行为设计模式的一种。State模式允许通过改变对象的内部状态而改变对象的行为，这个对象表现得就好像修改了它的类一样。 
状态模式主要解决的是当控制一个对象状态转换的条件表达式过于复杂时的情况。把状态的判断逻辑转译到表现不同状态的一系列类当中，可以把复杂的判断逻辑简化。类似于“状态机”。

### 一、角色和职责 ###

![](https://i.imgur.com/T8vU6SX.png)

- Context：用户对象，拥有一个State类型的成员，以标识对象的当前状态； 
- State：接口或基类，封装与Context的特定状态相关的行为； 
- ConcreteState：接口实现类或子类，实现了一个与Context某个状态相关的行为。 

适用于：对象的行为，依赖于它所处的当前状态。行为随状态改变而改变的场景。通过用户的状态来改变对象的行为。

### 二、案例 ###
```cpp
#include <iostream>
using namespace std;

class Worker;

class State
{
public:
	virtual void doSomeThing(Worker *w) = 0;
};

class Worker
{
public:
	Worker();
	int getHour() const
	{
		return m_hour;
	}
	void setHour(int hour) //改变状态 7 
	{
		m_hour = hour;
	}
	State* getCurrentState() const
	{
		return m_currstate;
	}
	void setCurrentState(State* state)
	{
		m_currstate = state;
	}

	void doSomeThing() //
	{
		m_currstate->doSomeThing(this);
	}
private:
	int		m_hour;
	State	*m_currstate; //对象的当前状态
};

class State1 : public State
{
public:
	void doSomeThing(Worker *w);
};

class State2  : public State
{
public:
	void doSomeThing(Worker *w);
};

void State1::doSomeThing(Worker *w)
{
	if (w->getHour() == 7 || w->getHour()==8)
	{
		cout << "吃早饭" << endl; 
	}
	else
	{ 
		delete w->getCurrentState();  //状态1 不满足 要转到状态2
		w->setCurrentState(new State2 );
		w->getCurrentState()->doSomeThing(w); //
	}
}


void State2::doSomeThing(Worker *w)
{
	if (w->getHour() == 9 || w->getHour()==10)
	{
		cout << "工作" << endl; 
	}
	else
	{
		delete w->getCurrentState(); //状态2 不满足 要转到状态3 后者恢复到初始化状态
		w->setCurrentState(new State1); //恢复到当初状态
		cout << "当前时间点：" << w->getHour() << "未知状态" << endl;
	}
}

Worker::Worker()
{
	m_currstate = new State1();
}

int main(void)
{
	Worker *w1 = new Worker();
	w1->setHour(7);
	w1->doSomeThing();

	w1->setHour(9);
	w1->doSomeThing();

	delete w1;
	return 0;
}
```
编译运行结果如下：

![](https://i.imgur.com/SSrKrrt.png)

### 三、案例扩展 ###

创建一个 State 接口和实现了 State 接口的实体状态类。Context 是一个带有某个状态的类。
StatePatternDemo，演示类使用 Context 和状态对象来演示 Context 在状态改变时的行为变化。

![](https://i.imgur.com/KW2XPMi.png)