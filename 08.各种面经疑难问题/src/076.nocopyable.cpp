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
	A(){} // "Base<T>::Base() [���� T=nocopyable]" (������ ��������:10) ���ɷ���

};

class nocopyable2 final 
{
public:
	nocopyable2();
	~nocopyable2();
};

class B : public nocopyable2 // ���ܽ���final����������������
{

};