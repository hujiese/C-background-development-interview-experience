#include <iostream>
#include <list>
using namespace std;
//ע��list���ݽṹΪ�����������������Ԫ��
class Teacher
{
public:
	int		age;
	char	name[32];
public:
	void printT()
	{
		cout << "age:" << age << endl;
	}
};

void test1(void)
{
	list<int> li;
	li.push_back(2);
	li.push_back(4);
	li.push_back(6);

	for (list<int>::iterator it = li.begin(); it != li.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;
}

void test2(void)
{
	list<int> li;
	li.push_back(2);
	li.push_back(4);
	li.push_back(6);

	list<int>::iterator it = li.begin();

	it++;
	it++;
	it++;
	li.insert(it, 10);//����Ԫ���ڵ�����ָ��֮ǰ

	for (it = li.begin(); it != li.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;
}

void test3(void)
{
	list<int> l;
	cout << "list�Ĵ�С:" << l.size() << endl;
	for (int i = 0; i < 10; i++)
	{
		l.push_back(i); //β������Ԫ�� β�巨
	}
	cout << "list�Ĵ�С:" << l.size() << endl;

	for (list<int>::iterator it = l.begin(); it != l.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;

	//0	 1	2	3	4	5
	//          ��
	list<int>::iterator it1 = l.begin();
	list<int>::iterator it2 = l.begin();
	it2++;
	it2++;
	it2++;

	l.erase(it1, it2);//ɾ��ʱ����������Ϊ3��Ԫ��

	for (list<int>::iterator it = l.begin(); it != l.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;

	l.insert(l.begin(), 100);
	l.insert(l.begin(), 100);
	l.insert(l.begin(), 100);
	for (list<int>::iterator it = l.begin(); it != l.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;

	l.erase(l.begin());
	l.remove(100);
	for (list<int>::iterator it = l.begin(); it != l.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;
}
int main(void)
{
	cout << "--------------test1------------" << endl;
	test1();
	cout << "--------------test2------------" << endl;
	test2();
	cout << "--------------test3------------" << endl;
	test3();
	return 0;
}
