#include <iostream>
#include <string>
#define __CARETAKER__
using namespace std;

//Caretaker ������
// MememTo  ����¼

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

	//����
	MememTo* createMememTo()
	{
		return new MememTo(m_name, m_age);
	}
	//��ԭ 
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
	Person *p = new Person("����", 32);
	p->printT();

	//���� Person�����һ��״̬
	printf("---------\n");
	caretaker = new Caretaker( p->createMememTo());
	p->setAge(42);
	p->printT();

	printf("��ԭ�ɵ�״̬\n" );
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
	Person *p = new Person("����", 32);
	p->printT();

	//���� Person�����һ��״̬
	printf("---------\n");
	memto = p->createMememTo();
	p->setAge(42);
	p->printT();

	printf("��ԭ�ɵ�״̬\n" );
	p->setMememTo(memto);
	p->printT();

	delete memto;
	delete p;

	return 0;
}
#endif