#include<iostream>
using namespace std;
class A{
};
class B{
};
class C :public A{
};
class D :public virtual B{
};
class E :public A, public B{
};
int main(){
	A a;
	B b;
	C c;
	D d;
	E e;
	cout << "sizeof(a):" << sizeof(a) << endl;
	cout << "sizeof(b):" << sizeof(b) << endl;
	cout << "sizeof(c):" << sizeof(c) << endl;
	cout << "sizeof(d):" << sizeof(d) << endl;
	cout << "sizeof(e):" << sizeof(e) << endl;
	return 0;
}
