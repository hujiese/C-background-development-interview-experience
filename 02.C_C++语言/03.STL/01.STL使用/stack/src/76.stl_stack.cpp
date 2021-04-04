#include <iostream>
#include "stack"
using namespace std;

// teacher结点
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
	stack<int> st;

	for (int i = 0; i < 5; i++)
	{
		st.push(i);
	}

	while (!st.empty())
	{
		cout << st.top() << " ";
		st.pop();
	}
	cout << endl;
}



void test2(void)
{
	Teacher t1, t2, t3;
	t1.age = 31;
	t2.age = 32;
	t3.age = 33;

	stack<Teacher> s;
	s.push(t1);
	s.push(t2);
	s.push(t3);

	while (!s.empty())
	{
		Teacher tmp = s.top();
		tmp.printT();
		s.pop();
	}
}

//必须提供拷贝构造函数
void test3(void)
{
	Teacher t1, t2, t3;
	t1.age = 31;
	t2.age = 32;
	t3.age = 33;
	stack<Teacher *> s;
	s.push(&t1);
	s.push(&t2);
	s.push(&t3);

	while (!s.empty())
	{
		Teacher *p = s.top();
		p->printT();
		s.pop();
	}
}
int main(void)
{
	cout << "------------test1-----------" << endl;
	test1();
	cout << "------------test2-----------" << endl;
	test2();
	cout << "------------test3-----------" << endl;
	test3();
	return 0;
}