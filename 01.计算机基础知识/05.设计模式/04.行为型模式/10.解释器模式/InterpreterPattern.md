## 解释模式 ##

解释器模式（Interpreter Pattern）提供了评估语言的语法或表达式的方式，它属于行为型模式。这种模式实现了一个表达式接口，该接口解释一个特定的上下文。这种模式被用在 SQL 解析、符号处理引擎等。

### 一、角色和职责 ###

![](https://i.imgur.com/DrGFyHt.png)

- Context：解释器上下文环境类。用来存储解释器的上下文环境，比如需要解释的文法等。 
- AbstractExpression：解释器抽象类。
- ConcreteExpression：解释器具体实现类。

### 二、案例 ###
```cpp
#include <iostream>
using namespace std;

// Context 

// Expression 

// PlusExpression   MinusExpression 

class Context
{
public:
	Context(int num) :m_num(num)
	{
	}

	int getNum() const
	{
		return m_num;
	}

	int getRes() const
	{
		return m_res;
	}

	void setNum(int num)
	{
		this->m_num = num;
	}

	void  setRes(int res)
	{
		this->m_res = res;
	}

private:
	int m_num;
	int m_res;
};

class Expression
{
public:
	virtual void  interpreter(Context *context) = 0;
private:
	Context		*m_context;
};

//加法
class PlusExpression : public Expression
{
public:
	PlusExpression()
	{
		this->context = NULL;
	}
	virtual void  interpreter(Context *context)
	{
		int num = context->getNum();
		num ++;
		context->setNum(num);
		context->setRes(num);
	}
private:
	Context *context;
};


// 减 法
class MinusExpression : public Expression
{
public:
	MinusExpression()
	{
		this->context = NULL;
	}
	virtual void  interpreter(Context *context)
	{
		int num = context->getNum();
		num -- ;
		context->setNum(num);
		context->setRes(num);
	}
private:
	Context *context;
};



int main(void)
{
	Expression		*expression = NULL;
	Context			*context = NULL;

	Expression		*expression2 = NULL;

	context = new Context(10);
	cout << context->getNum() << endl;

	expression = new PlusExpression();
	expression->interpreter(context);

	cout << context->getRes() << endl;

	//////////////////////////////////////////////////////////////////////////
	expression2 = new MinusExpression();
	expression2->interpreter(context);
	cout << context->getRes() << endl;

	return 0;
}
```
编译运行结果如下：

![](https://i.imgur.com/Cs9segk.png)

### 三、案例扩展 ###

创建一个接口 Expression 和实现了 Expression 接口的实体类。定义作为上下文中主要解释器的 TerminalExpression 类。其他的类 OrExpression、AndExpression 用于创建组合式表达式。

InterpreterPatternDemo，演示类使用 Expression 类创建规则和演示表达式的解析。

![](https://i.imgur.com/geO1tqS.jpg)