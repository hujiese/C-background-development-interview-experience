#include<iostream>
using namespace std;

template <typename T>
class Base
{
	friend T;
private:
	Base();
	~Base();
};

class nocopyable : virtual public Base<nocopyable>
{
public:
	nocopyable();
	~nocopyable();
};

class A : public nocopyable
{
public:
	A(){} // "Base<T>::Base() [其中 T=nocopyable]" (已声明 所在行数:10) 不可访问

};

class nocopyable2 final 
{
public:
	nocopyable2();
	~nocopyable2();
};

class B : public nocopyable2 // 不能将“final”类类型用作基类
{

};