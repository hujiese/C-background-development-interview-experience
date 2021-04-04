#include <iostream>
using namespace std;

class Base{
public:
	int x;
	char c;

	Base(){};               //构造函数
	virtual ~Base(){};      //析构函数

	virtual void func(void){
		cout << "virtual func" << endl;
	};
};

class Son1 : public Base
{

};

class Son2 : public Base
{
	virtual void func(void){
		cout << "son2 's virtual func" << endl;
	};
};

int main()
{
	Base* base = new Base;
	cout << "对象的起始地址为：" << base << endl; // 对象的起始地址为：00956848
	cout << "对象obj的大小为:  " << sizeof(*base) << endl; // 虚函数表指针地址：00956848

	typedef void(*Fun)(void);  //定义函数指针类型
	Fun pFun = NULL;           //定义函数指针
	cout << "虚函数表指针地址：" << (int*)base << endl;  // 虚函数表指针地址：00956848
	cout << "虚函数表 — 第一个函数地址：" << *((int*)*(int*)base + 1) << endl; // 虚函数表 — 第一个函数地址：2167456
	pFun = (Fun)*((int*)*(int*)base + 1);
	pFun(); // virtual func

	cout << "------------------------------------------" << endl;

	Son1* son1 = new Son1;
	cout << "对象的起始地址为：" << son1 << endl; // 对象的起始地址为：0095C658
	cout << "对象obj的大小为:  " << sizeof(*son1) << endl; // 对象obj的大小为 : 12

	typedef void(*Fun)(void);  //定义函数指针类型
	Fun pFun2 = NULL;           //定义函数指针
	cout << "虚函数表指针地址：" << (int*)son1 << endl; // 虚函数表指针地址：0095C658
	cout << "虚函数表 — 第一个函数地址：" << *((int*)*(int*)son1 + 1) << endl; // 虚函数表 — 第一个函数地址：2167456
	pFun2 = (Fun)*((int*)*(int*)son1 + 1);
	pFun2(); // virtual func

	cout << "------------------------------------------" << endl;

	base = son1;

	cout << "对象的起始地址为：" << base << endl; // 对象的起始地址为：0095C658
	cout << "对象obj的大小为:  " << sizeof(*base) << endl; // 对象obj的大小为 : 12

	typedef void(*Fun)(void);  //定义函数指针类型
	Fun pFun3 = NULL;           //定义函数指针
	cout << "虚函数表指针地址：" << (int*)base << endl; // 虚函数表指针地址：0095C658
	cout << "虚函数表 — 第一个函数地址：" << *((int*)*(int*)base + 1) << endl; // 虚函数表 — 第一个函数地址：2167456
	pFun3 = (Fun)*((int*)*(int*)base + 1);
	pFun3(); // virtual func

	cout << "------------------------------------------" << endl;

	Son2* son2 = new Son2;
	cout << "对象的起始地址为：" << son2 << endl; // 对象的起始地址为：0095C670
	cout << "对象obj的大小为:  " << sizeof(*son2) << endl; // 对象obj的大小为 : 12

	typedef void(*Fun)(void);  //定义函数指针类型
	Fun pFun4 = NULL;           //定义函数指针
	cout << "虚函数表指针地址：" << (int*)(&son2) << endl; // 虚函数表指针地址：0073FEE0
	cout << "虚函数表 — 第一个函数地址：" << *((int*)*(int*)son2 + 1) << endl; // 虚函数表 — 第一个函数地址：2167536
	pFun4 = (Fun)*((int*)*(int*)son2 + 1);
	pFun4(); // virtual func

	cout << "------------------------------------------" << endl;

	base = son2;

	cout << "对象的起始地址为：" << base << endl; // 对象的起始地址为：0095C670
	cout << "对象obj的大小为:  " << sizeof(*base) << endl; // 对象obj的大小为 : 12

	typedef void(*Fun)(void);  //定义函数指针类型
	Fun pFun5 = NULL;           //定义函数指针
	cout << "虚函数表指针地址：" << (int*)base << endl; // 虚函数表指针地址：0095C670
	cout << "虚函数表 — 第一个函数地址：" << *((int*)*(int*)base + 1) << endl; // 虚函数表 — 第一个函数地址：2167536
	pFun5 = (Fun)*((int*)*(int*)base + 1);

	pFun5(); // virtual func

	delete base;
	delete son1;
	return 0;
}