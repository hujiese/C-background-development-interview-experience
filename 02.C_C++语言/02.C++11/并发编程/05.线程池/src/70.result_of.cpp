#include <iostream>

struct S {
	double operator()(char, int&); // 这个函数的返回类型是 double
};

int main()
{

	std::result_of<S(char, int&)>::type foo = 3.14; // 使用这样的写法会推导出模板参数中函数的返回值类型
	typedef std::result_of<S(char, int&)>::type MyType; // 是 double 类型吗?
	std::cout << "foo's type is double: " << std::is_same<double, MyType>::value << std::endl; // foo's type is double: 1
	return 0;
}