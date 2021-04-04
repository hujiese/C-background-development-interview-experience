#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
using namespace std;

template <typename T>
void FuncShowElemt(T &t)  //普通函数 不能像 仿函数那样记录状态
{
	cout << t << " ";
};

void showChar(char &t)
{
	cout << t << " ";
}

//函数模板 重载 函数调用操作符
template <typename T>
class ShowElemt
{
public:
	ShowElemt()
	{
		n = 0;
	}
	void operator()(T &t)
	{
		n++;
		cout << t << " ";
	}
	void printCount()
	{
		cout << n << endl;
	}
public:
	int n;
};

//1 函数对象 基本使用
void main()
{
	int a = 100;
	FuncShowElemt<int>(a); //普通的函数调用
	ShowElemt<int> showElemt; //函数对象 
	showElemt(a); //函数对象调用 
}
