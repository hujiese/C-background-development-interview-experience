## 函数绑定

看完下面这个案例，函数绑定的使用便基本掌握了：
```c
#include <iostream>
#include <functional>
using namespace std;

typedef function<void(int, int)> Fun;
typedef function<void(int)> Fun2;

class Foo
{
public:
	void memberFunc(double d, int i, int j)
	{
		cout << d << endl;
		cout << i << endl;      
		cout << j << endl;
	}
};

void memberFunc(double d, int i, int j)
{
	cout << d << endl;
	cout << i << endl;     
	cout << j << endl;
}

int Func(int i, int j)
{
	return i + j;
}

int main()
{
	Foo foo;
	function<void(int, int)> fp = bind(&Foo::memberFunc, &foo, 0.5, placeholders::_1, placeholders::_2);
	fp(100, 200);
	cout << "---------------------------" << endl;

	function<void(int, int)> fp2 = bind(&Foo::memberFunc, ref(foo), 0.5, placeholders::_1, placeholders::_2);
	fp2(55, 66);
	cout << "---------------------------" << endl;

	Fun fun = bind(memberFunc, 0.3, placeholders::_1, placeholders::_2);
	fun(123, 456);
	cout << "---------------------------" << endl;

	Fun2 fun2 = bind(memberFunc, 0.5, 666, placeholders::_1);
	fun2(77);
	cout << "---------------------------" << endl;

	Fun2 fun3 = bind(memberFunc, 0.5, placeholders::_1, 666);
	fun3(77);
	cout << "---------------------------" << endl;

	function<int(int, int)> fun4 = bind(Func, placeholders::_1, placeholders::_2);
	int temp = fun4(1, 2);
	cout << temp << endl;

	return 0;
}
```
该案例运行结果如下：
```
0.5
100
200
---------------------------
0.5
55
66
---------------------------
0.3
123
456
---------------------------
0.5
666
77
---------------------------
0.5
77
666
---------------------------
3
```