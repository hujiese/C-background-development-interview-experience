#include<iostream>

class A {
public:
	int data;
};

int main()
{
	const A a = { 200 };
	//A a1 = const_cast<A>(a);    //错误，const_cast 目标类型只能是引用或者指针
	//a1.data = 100;

	A& a2 = const_cast<A&>(a);
	a2.data = 100;
	std::cout << a.data << ' ' << a2.data << std::endl;

	A* a3 = const_cast<A*>(&a);
	a3->data = 100;
	std::cout << a.data << ' ' << a3->data << std::endl;

	const int x = 3;

	int& x1 = const_cast<int&>(x);
	x1 = 200;
	std::cout << x << ' ' << x1 << std::endl;

	int* x2 = const_cast<int*>(&x);
	*x2 = 300;
	std::cout << x << ' ' << *x2 << std::endl;

	system("pause");
	return 0;
}