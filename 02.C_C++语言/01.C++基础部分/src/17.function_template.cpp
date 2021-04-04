#include <iostream>
using namespace std;

int Max(int a, int b)
{
	cout << "int Max(int a, int b)" << endl;
	return a > b ? a : b;
}

template<typename T>
T Max(T a, T b)
{
	cout << "T Max(T a, T b)" << endl;
	return a > b ? a : b;
}

template<typename T>
T Max(T a, T b, T c)
{
	cout << "T Max(T a, T b, T c)" << endl;
	return Max(Max(a, b), c);
}


void main()
{
	int a = 1;
	int b = 2;

	cout << Max(a, b) << endl; //当函数模板和普通函数都符合调用时,优先选择普通函数 int Max(int a, int b)
	cout << Max<>(a, b) << endl; //若显示使用函数模板,则使用<> 类型列表 T Max(T a, T b)

	cout << Max(3.0, 4.0) << endl; //如果 函数模板产生更好的匹配 使用函数模板 T Max(T a, T b)

	cout << Max(5.0, 6.0, 7.0) << endl; //重载 T Max(T a, T b, T c)

	cout << Max('a', 100) << endl;  //调用普通函数 可以隐式类型转换  int Max(int a, int b)
	system("pause");
	return;
}
