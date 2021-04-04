#include <iostream>
using namespace std;

class Base
{
public:
	virtual void fun1()
	{
		cout << "Base func1...." << endl;
	}
	virtual void fun2()
	{
		cout << "Base func2...." << endl;
	}
	void func3()
	{
		cout << "Base func3...." << endl;
	}
};

class Son : public Base
{
public:
	virtual void fun1()
	{
		cout << "Son func1...." << endl;
	}
	virtual void fun2()
	{
		cout << "Son func2...." << endl;
	}
	void func3()
	{
		cout << "Son func3...." << endl;
	}
	void func4()
	{
		cout << "Son func4..." << endl;
	}
};

int main(void)
{
	Base* base = new Base;
	base->func3(); // Base func3....

	cout << "----------------" << endl;

	Son* son = new Son;
	son->fun1(); // Son func1....
	son->fun2(); // Son func2....
	son->func3(); // Son func3....
	son->func4(); // Son func4...

	cout << "------------------" << endl;

	base = son;
	base->fun1(); // Son func1....
	base->fun2(); // Son func2....
	base->func3(); // Base func3....
	//base->func4(); // error

	cout << "----------------" << endl;

	Son* son2 = dynamic_cast<Son*>(base);
	son2->fun1(); // Son func1....
	son2->fun2(); // Son func2....
	son2->func3(); // son func3....
	son2->func4(); // Son func4...

	cout << "----------------" << endl;

	Base* base2 = nullptr;
	base2 = dynamic_cast<Base*>(son);
	base2->fun1(); // Son func1....
	base2->fun2(); // Son func2....
	base2->func3(); // Base func3....
	//base2->func4(); // error

	return 0;
}