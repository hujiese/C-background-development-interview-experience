## 策略模式 ##

Strategy模式也叫策略模式是行为模式之一，它对一系列的算法加以封装，为所有算法定义一个抽象的算法接口，并通过继承该抽象算法接口对所有的算法加以封装和实现，具体的算法选择交由客户端决定（策略）。Strategy模式主要用来平滑地处理算法的切换 。

### 一、角色和职责 ###

![](https://i.imgur.com/ldkAmhv.png)

- Strategy:策略（算法）抽象。
- StrategyX：各种策略（算法）的具体实现。
- Context：策略的外部封装类，或者说策略的容器类。根据不同策略执行不同的行为。策略由外部环境决定。 

适用于：准备一组算法，并将每一个算法封装起来，使得它们可以互换。

它的优点有：

- 1、策略模式提供了管理相关的算法族的办法。策略类的等级结构定义了一个算法或行为族。恰当使用继承可以把公共的代码移到父类里面，从而避免重复的代码。
- 2、策略模式提供了可以替换继承关系的办法。继承可以处理多种算法或行为。如果不是用策略模式，那么使用算法或行为的环境类就可能会有一些子类，每一个子类提供一个不同的算法或行为。但是，这样一来算法或行为的使用者就和算法或行为本身混在一起。决定使用哪一种算法或采取哪一种行为的逻辑就和算法或行为的逻辑混合在一起，从而不可能再独立演化。继承使得动态改变算法或行为变得不可能。
- 3、使用策略模式可以避免使用多重条件转移语句。多重转移语句不易维护，它把采取哪一种算法或采取哪一种行为的逻辑与算法或行为的逻辑混合在一起，统统列在一个多重转移语句里面，比使用继承的办法还要原始和落后。

策略模式的缺点有：

- 1、客户端必须知道所有的策略类，并自行决定使用哪一个策略类。这就意味着客户端必须理解这些算法的区别，以便适时选择恰当的算法类。换言之，策略模式只适用于客户端知道所有的算法或行为的情况。
- 2、策略模式造成很多的策略类。有时候可以通过把依赖于环境的状态保存到客户端里面，而将策略类设计成可共享的，这样策略类实例可以被不同客户端使用。换言之，可以使用享元模式来减少对象的数量。

### 二、案例 ###
```cpp
#include <iostream>
using namespace std;

class Strategy
{
public:
	virtual void crypt() = 0;
};

//对称加密  速度快 加密大数据块文件 特点:加密密钥和解密密钥是一样的.
//非对称加密 加密速度慢 加密强度高 高安全性高 ;特点: 加密密钥和解密密钥不一样  密钥对(公钥 和 私钥)

class AES :  public Strategy
{
public:
	virtual void crypt()
	{
		cout << "AES加密算法" << endl;
	}
};

class DES :  public Strategy
{
public:
	virtual void crypt()
	{
		cout << "DES 加密算法" << endl;
	}
};


class Context
{
public:
	void setStrategy(Strategy *strategy)
	{
		this->strategy = strategy;
	}
	void myoperator()
	{
		strategy->crypt();
	}
private:
	Strategy *strategy;
};


int main(void)
{
	Strategy *strategy = NULL;

	//strategy = new DES();
	strategy = new AES();
	Context *context = new Context();
	context->setStrategy(strategy);
	context->myoperator();
	
	delete  strategy;
	delete  context;
			
	return 0;
}
```
编译运行结果如下：

![](https://i.imgur.com/jkAdUAZ.png)

### 三、案例扩展 ###

创建一个定义活动的 Strategy 接口和实现了 Strategy 接口的实体策略类。Context 是一个使用了某种策略的类。

StrategyPatternDemo，演示类使用 Context 和策略对象来演示 Context 在它所配置或使用的策略改变时的行为变化。

![](https://i.imgur.com/ykREs9e.jpg)
