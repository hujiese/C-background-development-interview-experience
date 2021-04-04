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
		v->visit(this); //公园接受访问者访问 让访问者做操作
	}
};

class  ParkB : public ParkElement
{
public:
	virtual void accept(Visitor *v)
	{
		v->visit(this); //公园接受访问者访问 让访问者做操作
	}

};

//整个公园 
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
		//v->visit(this); //公园接受访问者访问 让访问者做操作

		for (list<ParkElement *>::iterator it = m_list.begin(); it!=m_list.end(); it++ )
		{
			(*it)->accept(v);  //公园A 公园B 接受 管理者v访问
		}
	}

private:
	list<ParkElement *> m_list; //公园的每一部分  //应该让公园的每一个部分 都让 管理者访问
};

class VisitorA : public Visitor
{
public:
	virtual void visit(ParkElement *parkelement)
	{
		cout << "清洁工A 完成 公园A部分的 打扫 " << endl; //parkelement->getName();
	}
};

class VisitorB : public Visitor
{
public:
	virtual void visit(ParkElement *parkelement)
	{
		cout << "清洁工B 完成 公园B部分的 打扫 " << endl; //parkelement->getName();
	}
};

class ManagerVisitor : public Visitor

{
public:
	virtual void visit(ParkElement *parkelement)
	{
		cout << "管理者 访问公园 的 各个部分 " << endl; //parkelement->getName();
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

	//整个公园 接受 管理者访问
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