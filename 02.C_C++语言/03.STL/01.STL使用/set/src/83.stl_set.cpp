#define  _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <set>
#include <functional>
using namespace std;

//set默认是按照从小到大顺序排序的
void test1(void)
{
	set<int> st;

	for (int i = 0; i < 5; i++)
	{
		int temp = rand();
		st.insert(temp);
	}
	st.insert(100);
	st.insert(200);
	st.insert(50);

	for (set<int>::iterator it = st.begin(); it != st.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;
}

void test2(void)
{
	set<int> st;

	for (int i = 0; i < 5; i++)
	{
		int temp = rand();
		st.insert(temp);
	}
	st.insert(100);
	st.insert(200);
	st.insert(50);

	set<int>::iterator it = st.begin();
	for (; it != st.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;

	while (!st.empty())
	{
		it = st.begin();
		st.erase(it);
	}
	cout << st.size() << endl;

}

void test3(void)
{
	set<int, greater<int>> st;

	for (int i = 0; i < 5; i++)
	{
		int temp = rand();
		st.insert(temp);
	}
	st.insert(100);
	st.insert(200);
	st.insert(50);

	for (set<int, greater<int>>::iterator it = st.begin(); it != st.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;
}
int main(void)
{
	cout << "---------test1---------" << endl;
	test1();
	cout << "---------test2---------" << endl;
	test2();
	cout << "---------test3---------" << endl;
	test3();

	return 0;
}