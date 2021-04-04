#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
using namespace std;

template <typename T>
struct SumAdd
{
	T operator()(T &t1, T &t2)
	{
		return t1 + t2;
	}
};

template <typename T>
void printE(T &t)
{
	for (vector<int>::iterator it = t.begin(); it != t.end(); it++)
	{
		cout << *it << " ";
	}
}

void printVector(vector<int> &v)
{
	for (vector<int>::iterator it = v.begin(); it != v.end(); it++)
	{
		cout << *it << " ";
	}
}

void  main()
{
	vector<int> v1, v2;
	vector<int> v3;
	v1.push_back(1);
	v1.push_back(2);
	v1.push_back(3);

	v2.push_back(4);
	v2.push_back(5);
	v2.push_back(6);

	v3.resize(10);

	//transform(v1.begin(), v1.end(), v2.begin(),v3.begin(), SumAdd<int>());
	/*
	template<class _InIt1,
	class _InIt2,
	class _OutIt,
	class _Fn2> inline
	_OutIt transform(_InIt1 _First1, _InIt1 _Last1,
	_InIt2 _First2, _OutIt _Dest, _Fn2 _Func)
	*/
	vector<int>::iterator it = transform(v1.begin(), v1.end(), v2.begin(), v3.begin(), SumAdd<int>());
	cout << *it << endl;
	printE(v3);
}
