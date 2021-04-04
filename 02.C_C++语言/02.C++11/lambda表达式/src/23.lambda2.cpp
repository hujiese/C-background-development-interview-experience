#include <iostream>
using namespace std;

//// 捕获外部变量
//int main()
//{
//	int a = 123;
//	auto f = [a] { cout << a << endl; };
//	f(); // 输出：123
//
//	//或通过“函数体”后面的‘()’传入参数
//	[](int a){cout << a << endl; }(1233435);
//}

//// 1、值捕获
//int main()
//{
//    int a = 123;
//    auto f = [a] { cout << a << endl; }; 
//    a = 321;
//    f(); // 输出：123
//}

////2、引用捕获
//int main()
//{
//	int a = 123;
//	auto f = [&a] { cout << a << endl; };
//	a = 321;
//	f(); // 输出：321
//}

////隐式值捕获
//int main()
//{
//	int a = 123;
//	auto f = [=] { cout << a << endl; };    // 值捕获
//	f(); // 输出：123
//}

////隐式引用捕获
//int main()
//{
//	int a = 123;
//	auto f = [&] { cout << a << endl; };    // 引用捕获
//	a = 321;
//	f(); // 输出：321
//}

// 修改捕获变量
int main()
{
	int a = 123;
	auto f = [a]()mutable { cout << ++a; }; // 不会报错
	cout << a << endl; // 输出：123
	f(); // 输出：124
}