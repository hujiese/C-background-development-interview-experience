## 代理模式 ##

Proxy模式又叫做代理模式，是构造型的设计模式之一，它可以为其他对象提供一种代理（Proxy）以控制对这个对象的访问。

所谓代理，是指具有与代理元（被代理的对象）具有相同的接口的类，客户端必须通过代理与被代理的目标类交互，而代理一般在交互的过程中（交互前后），进行某些特别的处理。

### 一、类图角色和职责 ###

![](https://i.imgur.com/rouV7a4.png)

- Abstract（抽象主题角色）：真实主题与代理主题的共同接口。
- RealSubject（真实主题角色）：定义了代理角色所代表的真实对象。 
- Proxy（代理主题角色）：含有对真实主题角色的引用，代理角色通常在将客户端调用传递给真是主题对象之前或者之后执行某些操作，而不是单纯返回真实的对象。

适合于：为其他对象提供一种代理以控制对这个对象的访问。

### 二、案例 ###
```cpp
#include <iostream>
using namespace std;

class Subject
{
public:
	virtual void salebook() = 0;
};

class RealSubjectBook : public Subject
{
public:
	virtual void salebook()
	{
		cout << "卖书" << endl;
	}
};

//a中包含b类；a、b类实现协议类protocol 
class dangdangProxy : public Subject
{
public:
	virtual void salebook()
	{
		RealSubjectBook *rsb = new RealSubjectBook();
		rsb->salebook();
		discount();
	}
public:
	void discount()
	{
		cout << "双十一打折" << endl;
	}
private:
	Subject *m_subject;
};

int main(void)
{
	Subject *s = new dangdangProxy();
	s->salebook();
	delete s;

	return 0;
}
```
编译运行结果如下：

![](https://i.imgur.com/pcLfNcy.png)

### 三、扩展案例 ###
创建一个Image接口和实现了Image接口的实体类。ProxyImage是一个代理类，减少RealImage对象加载的内存占用。

ProxyPatternDemo，演示类使用ProxyImage来获取要加载的Image对象，并按照需求进行显示。

![](https://i.imgur.com/xindaK8.jpg)


