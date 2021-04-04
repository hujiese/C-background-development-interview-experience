#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>
using namespace std;

template<class T>
struct Mygreater : public binary_function<T, T, bool> // 一定要继承该类： public binary_function<T, T, bool>
{	
	bool operator()(const T& _Left, const T& _Right) const
	{	
		return (_Left > _Right);
	}
};

int main(void)
{
	vector<int>  v1;
	for (int i = 0; i<5; i++)
	{
		v1.push_back(i + 1);
	}
	// 打印数组内容
	for (vector<int>::iterator it = v1.begin(); it != v1.end(); it++)
	{
		cout << *it << " "; // 1 2 3 4 5
	}
	cout << endl;

	int num1 = count_if(v1.begin(), v1.end(), bind1st(Mygreater<int>(), 2));// 绑定Mygreater函数的第一个参数（2 > v1[i]），结果是得到比2小的数量
	cout << "bind1st result :" << num1 << endl; // bind1st result : 1

	int num2 = count_if(v1.begin(), v1.end(), bind2nd(Mygreater<int>(), 2));// 绑定Mygreater函数的第二个参数（v1[i] > 2），结果是得到比2大的数量  
	cout << "bind2nd result :" << num2 << endl; // bind2nd result : 3

	return 0;
}