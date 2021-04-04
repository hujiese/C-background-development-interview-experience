#include <iostream>
#include "vector"
using namespace std;

//ע�⣺����erase��insert����vector����ʱλ��Ԫ��Ӧ���ǵ�����ָ��
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

	//ָ����Χɾ��Ԫ��
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
	//ɾ��ָ��λ��Ԫ��
	v.erase(v.begin());
	cout << " rease from begin to end " << endl;
	printVector(v);
}

//ɾ��ָ��Ԫ��
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
			it = v.erase(it);//ע�⣺�ô˷���ɾ��Ԫ�غ󷵻���һ��Ԫ��λ��iteratorָ�룬Ҫ�õ������ӹ��������ڴ�й©,������Ҫ�ر�ע��
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
	//����ָ��λ��Ԫ��
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