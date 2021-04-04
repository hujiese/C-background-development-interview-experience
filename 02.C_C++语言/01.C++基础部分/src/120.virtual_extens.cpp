#include <iostream>
using namespace std;

class A{
public:
	void doIt(){ cout << "A ..." << endl;  }
};
class B :public virtual A{};
class C : public virtual A{};
class D :public B, public C{};

int main()
{
	int a, b, c, d;

	a = sizeof(A);
	b = sizeof(B);
	c = sizeof(C);
	d = sizeof(D);

	cout << a << b << c << d << endl; // 1448

	A aa;
	B bb;
	C cc;
	D dd;

	aa.doIt(); // A ...
	bb.doIt(); // A ...
	cc.doIt(); // A ...
	dd.doIt(); // A ...

	return 0;
}
