#include <iostream>

struct S {
	double operator()(char, int&); // ��������ķ��������� double
};

int main()
{

	std::result_of<S(char, int&)>::type foo = 3.14; // ʹ��������д�����Ƶ���ģ������к����ķ���ֵ����
	typedef std::result_of<S(char, int&)>::type MyType; // �� double ������?
	std::cout << "foo's type is double: " << std::is_same<double, MyType>::value << std::endl; // foo's type is double: 1
	return 0;
}