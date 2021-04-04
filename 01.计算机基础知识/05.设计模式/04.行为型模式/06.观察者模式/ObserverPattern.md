## 观察者模式 ##

Observer模式是行为模式之一，它的作用是当一个对象的状态发生变化时，能够自动通知其他关联对象，自动刷新对象状态。

Observer模式提供给关联对象一种同步通信的手段，使某个对象与依赖它的其他对象之间保持状态同步。

### 一、角色和职责 ###

![](https://i.imgur.com/xqwkbh8.png)

- Subject（被观察者）：被观察的对象。当需要被观察的状态发生变化时，需要通知队列中所有观察者对象。- --  Subject需要维持（添加，删除，通知）一个观察者对象的队列列表。
- Observer（观察者）：接口或抽象类。当Subject的状态发生变化时，Observer对象将通过一个callback函数得到通知。
- ObserverX：观察者的具体实现。得到通知后将完成一些具体的业务逻辑处理。

典型应用：

- 侦听事件驱动程序设计中的外部事件
- 侦听/监视某个对象的状态变化
- 发布者/订阅者(publisher/subscriber)模型中，当一个外部事件（新的产品，消息的出现等等）被触发时，通知邮件列表中的订阅者

适用于：定义对象间一种一对多的依赖关系，使得每一个对象改变状态，则所有依赖于他们的对象都会得到通知。

使用场景：定义了一种一对多的关系，让多个观察对象（公司员工）同时监听一个主题对象（秘书），主题对象状态发生变化时，会通知所有的观察者，使它们能够更新自己。

### 二、案例 ###
```cpp
#include <iostream>
#include <string>
#include <list>
using namespace std;

//观察者 
class PlayserObserver
{
public:
	void update(string action)
	{
		cout << "action:" << action << endl;
		cout << "老板来了 我很害怕啊..." << endl;
		
	}
};

class Secretary
{
public:
	Secretary()
	{
		m_list.clear();
	}
	void Notify(string info)
	{
		//给所有的 观察者 发送 情报
		for ( list<PlayserObserver *>::iterator it=m_list.begin(); it!=m_list.end(); it++)
		{
			(*it)->update(info);
		}
	}

	void setPlayserObserver(PlayserObserver *o)
	{
		m_list.push_back(o);
	}

private:
	list<PlayserObserver *> m_list;
};
int main(void)
{
	Secretary			*secretary = new Secretary();
	PlayserObserver		*po1 = new PlayserObserver();
	PlayserObserver		*po2 = new PlayserObserver();

	secretary->setPlayserObserver(po1);
	secretary->setPlayserObserver(po2);

	secretary->Notify("老板来了") ;
	secretary->Notify("老板走了");
	delete secretary ;
	delete po1 ;
	delete po2 ;

	return 0;
}
```
编译运行结果如下：

![](https://i.imgur.com/12WdZUB.png)

### 三、案例扩展 ###

观察者模式使用三个类 Subject、Observer 和 Client。Subject 对象带有绑定观察者到 Client 对象和从 Client 对象解绑观察者的方法。创建 Subject 类、Observer 抽象类和扩展了抽象类 Observer 的实体类。

ObserverPatternDemo，演示类使用 Subject 和实体类对象来演示观察者模式。

![](https://i.imgur.com/uFExhmw.jpg)