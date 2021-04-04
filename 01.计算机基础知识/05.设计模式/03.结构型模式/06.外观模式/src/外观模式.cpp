#include <iostream>
using namespace std;

class SubSystemA
{
public:
	void doThing()
	{
		cout << "SubSystemA run" << endl;
	}
};

class SubSystemB
{
public:
	void doThing()
	{
		cout << "SubSystemB run" << endl;
	}
};

class SubSystemC
{
public:
	void doThing()
	{
		cout << "SubSystemC run" << endl;
	}
};


class Facade
{
public:
	Facade()
	{
		sysA = new SubSystemA();
		sysB = new SubSystemB();
		sysC = new SubSystemC();
	}
	~Facade()
	{
		delete sysA;
		delete sysB;
		delete sysC;
	}
public:
	void doThing()
	{
		sysA->doThing();
		sysB->doThing();
		sysC->doThing();
	}
private:
	SubSystemA *sysA;
	SubSystemB *sysB;
	SubSystemC *sysC;
};

void main1801()
{
	SubSystemA *sysA = new SubSystemA();
	SubSystemB *sysB = new SubSystemB();
	SubSystemC *sysC = new SubSystemC();

	sysA->doThing();
	sysB->doThing();
	sysC->doThing();
	delete sysA;
	delete sysB;
	delete sysC;
	
	return ;
}

void main1802()
{
	Facade *f = new Facade();
	f->doThing();
	delete f;

}

int main(void)
{
	//main1801();
	main1802();

	return 0;
}