#include <iostream>
#include "vector"
using namespace std;

//注意：在用erase与insert操作vector容器时位置元素应该是迭代器指针
void printVector(vector<int> &vect)
{
	for (vector<int>::iterator it = vect.begin(); it != vect.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;
}

void test1(void)
{
	vector<int> v;
	v.push_back(2);
	v.push_back(4);
	v.push_back(6);

	printVector(v);

	//指定范围删除元素
	v.erase(v.begin(), v.end());
	cout << " rease from begin to end " << endl;
	printVector(v);
}

void test2(void)
{
	vector<int> v;
	v.push_back(2);
	v.push_back(4);
	v.push_back(6);

	printVector(v);
	//删除指点位置元素
	v.erase(v.begin());
	cout << " rease from begin to end " << endl;
	printVector(v);
}

//删除指定元素
void test3(void)
{
	vector<int> v;
	v.push_back(2);
	v.push_back(4);
	v.push_back(6);

	printVector(v);

	cout << " rease from begin to end " << endl;
	for (vector<int>::iterator it = v.begin(); it != v.end();)
	{
		if (*it == 4)
		{
			it = v.erase(it);//注意：用此方法删除元素后返回下一个元素位置iterator指针，要用迭代器接过，否则内存泄漏,遍历是要特别注意
		}
		else
		{
			it++;
		}
	}
	printVector(v);
}

void test4(void)
{
	vector<int> v;
	v.push_back(2);
	v.push_back(4);
	v.push_back(6);

	printVector(v);
	//插入指点位置元素
	v.insert(v.begin() + 1, 100);
	cout << " insert " << endl;
	printVector(v);
}
int main(void)
{
	cout << "---------test1---------" << endl;
	test1();
	cout << "---------test2---------" << endl;
	test2();
	cout << "---------test3---------" << endl;
	test3();
	cout << "---------test4---------" << endl;
	test4();

	return 0;
}