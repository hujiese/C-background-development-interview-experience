#include <iostream>
#include <list>
#define __MANAGER__
using namespace std;

// Visitor  ParkElement
class ParkElement;

class Visitor
{
public:
	virtual void visit(ParkElement *parkelement) = 0;
};

class ParkElement
{
public:
	virtual void accept(Visitor *visit) = 0;
};

class  ParkA : public ParkElement
{
public:
	virtual void accept(Visitor *v)
	{
		v->visit(this); //��԰���ܷ����߷��� �÷�����������
	}
};

class  ParkB : public ParkElement
{
public:
	virtual void accept(Visitor *v)
	{
		v->visit(this); //��԰���ܷ����߷��� �÷�����������
	}

};

//������԰ 
class Park : public ParkElement
{
public:
	Park()
	{
		m_list.clear();
	}
	void setParkElement(ParkElement *pe)
	{
		m_list.push_back(pe);
	}

public:
	virtual void accept(Visitor *v)
	{
		//v->visit(this); //��԰���ܷ����߷��� �÷�����������

		for (list<ParkElement *>::iterator it = m_list.begin(); it!=m_list.end(); it++ )
		{
			(*it)->accept(v);  //��԰A ��԰B ���� ������v����
		}
	}

private:
	list<ParkElement *> m_list; //��԰��ÿһ����  //Ӧ���ù�԰��ÿһ������ ���� �����߷���
};

class VisitorA : public Visitor
{
public:
	virtual void visit(ParkElement *parkelement)
	{
		cout << "��๤A ��� ��԰A���ֵ� ��ɨ " << endl; //parkelement->getName();
	}
};

class VisitorB : public Visitor
{
public:
	virtual void visit(ParkElement *parkelement)
	{
		cout << "��๤B ��� ��԰B���ֵ� ��ɨ " << endl; //parkelement->getName();
	}
};

class ManagerVisitor : public Visitor

{
public:
	virtual void visit(ParkElement *parkelement)
	{
		cout << "������ ���ʹ�԰ �� �������� " << endl; //parkelement->getName();
	}
};

#ifdef __MANAGER__
int main(void)
{
	Visitor *vManager = new  ManagerVisitor();
	Park *park = new Park();

	ParkElement *parkA = new ParkA();
	ParkElement *parkB = new ParkB();

	park->setParkElement(parkA);
	park->setParkElement(parkB);

	//������԰ ���� �����߷���
	park->accept(vManager);

	delete parkA;
	delete parkB;
	delete park;
	delete vManager;

	return 0;
}

#else
int main(void)
{
	Visitor *vA = new  VisitorA();
	Visitor *vB = new  VisitorB();

	ParkA *parkA = new ParkA();
	ParkB *parkB = new ParkB();

	parkA->accept(vA);
	parkB->accept(vB);

	delete vA;
	delete vB;
	delete parkA;
	delete parkB;

	return 0;
}
#endif