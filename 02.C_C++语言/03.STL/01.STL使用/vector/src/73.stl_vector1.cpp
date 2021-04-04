#include <iostream>
#include "vector"
using namespace std;

void test1(void)
{
	vector<int> v1;
	v1.push_back(2);
	v1.push_back(5);
	v1.push_back(7);

	cout << v1.front() << endl;
	while (v1.size() > 0)
	{
		cout << v1.back() << endl;
		v1.pop_back();
	}
}

//函数返回值当左值应该返回引用
void test2(void)
{
	vector<int> v1;
	v1.push_back(2);
	v1.push_back(5);
	v1.push_back(7);

	v1.front() = 10;
	v1.back() = 20;
	cout << v1.front() << endl;
	cout << v1.back() << endl;
}

void test3(void)
{
	vector<int> v1;
	v1.push_back(2);
	v1.push_back(5);
	v1.push_back(7);

	vector<int> v2 = v1;
	//一种赋值方法
	vector<int> v3(v1.begin(), v1.begin() + 2);
	cout << v3.front() << endl;
}

//函数模板vector做函数参数
void printVector(vector<int> &vect)
{
	for (int i = 0; i < 5; i++)
	{
		cout << vect[i] << " ";
	}
	cout << endl;
}

//用数组遍历容器
void test4(void)
{
	vector<int> temp(5);

	for (int i = 0; i < 5; i++)
	{
		temp[i] = i;
	}

	for (int i = 0; i < 5; i++)
	{
		cout << temp[i] << " ";
	}
	cout << endl;
	printVector(temp);

}

//用迭代器遍历容器vector
void test5(void)
{
	vector<int> v;
	v.push_back(1);
	v.push_back(3);
	v.push_back(5);

	//正向遍历
	for (vector<int>::iterator it = v.begin(); it != v.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;

	//逆向遍历
	for (vector<int>::reverse_iterator rit = v.rbegin(); rit != v.rend(); rit++)
	{
		cout << *rit << " ";
	}
	cout << endl;
}
int main(void)
{
	cout << "------------test1-----------" << endl;
	test1();
	cout << "------------test2-----------" << endl;
	test2();
	cout << "------------test3-----------" << endl;
	test3();
	cout << "------------test4-----------" << endl;
	test4();
	cout << "------------test5-----------" << endl;
	test5();

	return 0;
}