#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
using namespace std;

//1元谓词 例子
template <typename T>
class Isdiv
{
public:
	Isdiv(const T &divisor) //
	{
		this->divisor = divisor;
	}
	bool operator()(T &t)
	{
		return (t%divisor == 0);
	}
protected:
private:
	T divisor;
};

void main()
{
	vector<int> v2;
	for (int i = 10; i<33; i++)
	{
		v2.push_back(i);
	}
	vector<int>::iterator it;
	int a = 4;
	Isdiv<int> mydiv(a);
	// _InIt find_if(_InIt _First, _InIt _Last, _Pr _Pred)   //返回的是迭代器
	it = find_if(v2.begin(), v2.end(), Isdiv<int>(4));
	if (it != v2.end())
	{
		cout << "第一个被4整除的数是：" << *it << endl;
	}
}
