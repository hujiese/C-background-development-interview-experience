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
	/*����base classed�����Ŀ����Ϊ������"ͨ��base class�ӿڴ���derived class����"*/
	delete pBase;
	return 0;
}
