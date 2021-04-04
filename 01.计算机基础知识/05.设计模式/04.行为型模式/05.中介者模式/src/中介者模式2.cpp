#include <iostream>
#include <string>
using namespace std;

class Mediator;
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
			cout << "ͬ���� ֮�� �����Ҷ��� " << endl;
		}
		if (pWoman->getCondi() == pMan->getCondi())
		{
			cout << pWoman->getName() << " �� " << pMan->getName() << "���� " << endl;
		}
		else
		{
			cout << pWoman->getName() << " �� " << pMan->getName() << "���� " << endl;
		}
	}
private:
	Person	*pMan;
	Person	*pWoman;
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
		mediator->getParter(); //�Ҷ��� 
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
		mediator->getParter(); //�Ҷ��� 
	}
};

int main(void)
{
	//string name, int sex, int condi
	Mediator *m = new Mediator();
	Person *xiaofang = new Woman("С��", 2, 5, m);

	Person *zhangsan = new Man("����", 1, 4, m);

	Person *lisi = new Man("����", 1, 5, m);
	xiaofang->getParter(zhangsan);

	xiaofang->getParter(lisi);

	return 0;
}