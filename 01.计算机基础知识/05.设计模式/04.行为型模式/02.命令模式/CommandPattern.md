## 命令模式 ##

Command模式也叫命令模式 ，是行为设计模式的一种。Command模式通过被称为
Command的类封装了对目标对象的调用行为以及调用参数。

在面向对象的程序设计中，一个对象调用另一个对象，一般情况下的调用过程是：创建目标对象实例；设置调用参数；调用目标对象的方法。但在有些情况下有必要使用一个专门的类对这种调用过程加以封装，我们把这种专门的类称作command类。整个调用过程比较繁杂，或者存在多处这种调用。这时，使用Command类对该调用加以封装，便于功能的再利用。调用前后需要对调用参数进行某些处理。调用前后需要进行某些额外处理，比如日志，缓存，记录历史操作等。

### 一、角色和职责 ###

![](https://i.imgur.com/gDbZO89.png)

- Command：Command命令的抽象类。
- ConcreteCommand：Command的具体实现类。
- Receiver：需要被调用的目标对象。
- Invorker：通过Invorker执行Command对象。

适用于：是将一个请求封装为一个对象，从而使你可用不同的请求对客户端进行参数化；对请求排队或记录请求日志，以及支持可撤销的操作。

### 二、案例 ###

推演一：
```cpp
#include <iostream>
using namespace std;

class Doctor
{
public:
	void treat_eye()
	{
		cout << "医生 治疗 眼科病" << endl;
	}

	void treat_nose()
	{
		cout << "医生 治疗 鼻科病" << endl;
	}
};


class CommandTreatEye
{
public:
	CommandTreatEye(Doctor *doctor) :m_doctor(doctor)
	{
	}
	void treat()
	{
		m_doctor->treat_eye();
	}
private:
	Doctor *m_doctor;
};


class CommandTreatNose
{
public:
	CommandTreatNose(Doctor *doctor) :m_doctor(doctor)
	{
	}
	void treat()
	{
		m_doctor->treat_nose();
	}
private:
	Doctor *m_doctor;
};



int main(void)
{
	//1 医生直接看病
	/*
	Doctor *doctor = new Doctor ;
	doctor->treat_eye();
	delete doctor;
	*/

	//2 通过一个命令 医生通过 命令 治疗 治病
	Doctor *doctor = new Doctor() ;
	CommandTreatEye * commandtreateye = new CommandTreatEye(doctor); //shift +u //转小写 shift+ctl + u转大写
	commandtreateye->treat();
	delete commandtreateye;
	delete doctor;
	return 0;
}
```
编译运行结果如下：

![](https://i.imgur.com/SEEflVe.png)

推演二：
```cpp
#include <iostream>
#include <list>
#define __HEAD_NURSE__
using namespace std;

class Doctor
{
public:
	void treat_eye()
	{
		cout << "医生 治疗 眼科病" << endl;
	}

	void treat_nose()
	{
		cout << "医生 治疗 鼻科病" << endl;
	}
};

class Command
{
public:
	virtual void treat() = 0;
};

class CommandTreatEye : public Command
{
public:
	CommandTreatEye(Doctor *doctor) :m_doctor(doctor)
	{
	}
	void treat()
	{
		m_doctor->treat_eye();
	}
private:
	Doctor *m_doctor;
};


class CommandTreatNose : public Command
{
public:
	CommandTreatNose(Doctor *doctor) :m_doctor(doctor)
	{
	}
	void treat()
	{
		m_doctor->treat_nose();
	}
private:
	Doctor *m_doctor;
};


class BeautyNurse
{
public:
	BeautyNurse(Command *command) :command(command)
	{
	}
public:
	void SubmittedCase() //提交病例 下单命令
	{
		command->treat();
	}
private:
	Command *command;
};

//护士长
class HeadNurse 
{
public:
	HeadNurse()
	{
		m_list.clear();
	}
	
public:
	void setCommand(Command *command)
	{
		m_list.push_back(command);
	}
	void SubmittedCase() //提交病例 下单命令
	{
		for (list<Command *>::iterator it=m_list.begin(); it!=m_list.end(); it++)
		{
			(*it)->treat();
		}
	}
private:
	list<Command *> m_list;
};

#ifndef __HEAD_NURSE__
int main(void)
{
	//护士提交简历 给以上看病
	BeautyNurse		*beautynurse = NULL;
	Doctor			*doctor = NULL;
	Command			*command  = NULL;

	doctor = new Doctor() ;

	//command = new CommandTreatEye(doctor); //shift +u //转小写 shift+ctl + u转大写
	command = new CommandTreatNose(doctor); //shift +u //转小写 shift+ctl + u转大写
	beautynurse = new BeautyNurse(command);
	beautynurse->SubmittedCase();
	
	delete doctor;
	delete command;
	delete beautynurse;
	return 0;
}
#else
// 通过护士长 批量的下单命令
int main(void)
{
	//护士提交简历 给以上看病
	HeadNurse		*headnurse = NULL;
	Doctor			*doctor = NULL;
	Command			*command1  = NULL;
	Command			*command2  = NULL;

	doctor = new Doctor() ;

	command1 = new CommandTreatEye(doctor); 
	command2 = new CommandTreatNose(doctor); 

	headnurse = new HeadNurse(); //new 护士长
	headnurse->setCommand(command1);
	headnurse->setCommand(command2);

	headnurse->SubmittedCase(); // 护士长 批量下单命令

	delete doctor;
	delete command1;
	delete command2;
	delete headnurse;
	return 0;
}
#endif
```
编译运行结果如下：

![](https://i.imgur.com/9b6cobT.png)

### 三、扩展案例 ###

首先创建作为命令的接口 Order，然后创建作为请求的 Stock 类。实体命令类 BuyStock 和 SellStock，实现了 Order 接口，将执行实际的命令处理。创建作为调用对象的类 Broker，它接受订单并能下订单。

Broker 对象使用命令模式，基于命令的类型确定哪个对象执行哪个命令。CommandPatternDemo，演示类使用 Broker 类来演示命令模式。

![](https://i.imgur.com/cWN3xsk.jpg)