#include<iostream>
#include<functional>
using namespace std;

//定义好了一个函数包装器
template < typename F, typename T>
T function_wrapper(F fun, T t)
{
	return fun(t);
}
//定义好了一个函数包装器
template < typename F, typename T>
T function_wrapper(F fun, T t1, T t2)
{
	static int count = 0;
	count++;
	cout << "函数包装器执行" << count << "次数" << endl;

	return fun(t1, t2);
}
int main()
{
	double d_num = 1.1;
	//double(double)是声明函数类型 
	//允许函数内嵌
	//本质是函数指针
	// [] 标识要开辟一个函数
	function<double(double)> square_fun = [](double in_data)
	{
		return in_data*in_data;
	};
	function<double(double)> cube_fun = [](double in_data)
	{
		return in_data*in_data*in_data;
	};
	function<int(int, int)> add_fun = [](int in_data1, int in_data2)
	{
		return in_data1 + in_data2;
	};
	cout << function_wrapper(square_fun, d_num) << " " << function_wrapper(cube_fun, d_num) << endl;
	cout << function_wrapper(add_fun, 1, 2) << endl;
	
	return 0;
}