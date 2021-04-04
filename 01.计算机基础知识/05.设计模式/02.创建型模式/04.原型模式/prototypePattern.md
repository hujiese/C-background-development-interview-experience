## 原型模式 ##

Prototype模式是一种对象创建型模式，它采取复制原型对象的方法来创建对象的实例。使用Prototype模式创建的实例，具有与原型一样的数据。

- 1）由原型对象自身创建目标对象。也就是说，对象创建这一动作发自原型对象本身。
- 2）目标对象是原型对象的一个克隆。也就是说，通过Prototype模式创建的对象，不仅仅与原型对象具有相同的结构，还与原型对象具有相同的值。
- 3）根据对象克隆深度层次的不同，有浅度克隆与深度克隆。

### 一、角色和职责 ###

原型模式主要面对的问题是：“某些结构复杂的对象”的创建工作；由于需求的变化，这些对象经常面临着剧烈的变化，但是他们却拥有比较稳定一致的接口。

适用情况：一个复杂对象，具有自我复制功能，统一一套接口。

![](https://i.imgur.com/8zR7dbx.png)

### 二、案例 ###
```cpp
#define _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <string>
using namespace std;


class Person
{
public:
	virtual Person* clone() = 0;
	virtual void printT() = 0;
};

class  CPlusPlusProgrammer : public Person
{
public:
	CPlusPlusProgrammer() :m_name(""), m_age(0), m_resume(NULL)
	{
		setResume("aaaa");
	}

	CPlusPlusProgrammer(string name, int age) :m_name(name), m_age(age), m_resume(NULL)
	{
		setResume("aaaa");
	}

	void setResume(char *p)
	{
		if (m_resume != NULL)
		{
			delete m_resume;
		}
		m_resume = new char[strlen(p) + 1];
		strcpy(m_resume, p);
	}


	virtual void printT()
	{
		cout << "m_name" << m_name << " m_age" << m_age << "m_resume:" << m_resume <<  endl;
	}
	virtual Person* clone()
	{
		CPlusPlusProgrammer *tmp = new CPlusPlusProgrammer;
		//tmp->m_name = this->m_name;
		*tmp = *this; // =  浅拷贝
		return tmp;
	}
private:
	string	m_name;
	int		m_age ;
	char	*m_resume;
		
};

int main(void)
{
	Person *c1 = new CPlusPlusProgrammer("张三", 32);
	c1->printT();


	Person *c2 = c1->clone();
	c2->printT();
	return 0;
}
```
编译运行结果如下：

![](https://i.imgur.com/O1djN2v.png)

