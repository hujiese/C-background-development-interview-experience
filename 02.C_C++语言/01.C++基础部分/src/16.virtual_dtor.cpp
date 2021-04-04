#include<iostream>
using namespace std;
class Base{
public:
	Base(){ std::cout << "Base::Base()" << std::endl; }
	~Base(){ std::cout << "Base::~Base()" << std::endl; }
	//virtual ~Base(){ std::cout << "Base::~Base()" << std::endl; }
};
class Derive :public Base{
public:
	Derive(){ std::cout << "Derive::Derive()" << std::endl; }
	~Derive(){ std::cout << "Derive::~Derive()" << std::endl; }
};
int main(){
	Base* pBase = new Derive();
	/*这种base classed的设计目的是为了用来"通过base class接口处理derived class对象"*/
	delete pBase;
	return 0;
}
