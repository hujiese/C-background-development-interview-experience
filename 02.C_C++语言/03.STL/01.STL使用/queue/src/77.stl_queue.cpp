#include <iostream>
#include "queue"
using namespace std;

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
	queue<int> qe;
	qe.push(1);
	qe.push(3);
	qe.push(5);

	cout << qe.size() << endl;
	while (!qe.empty())
	{
		cout << qe.front() << " ";
		qe.pop();
	}
	cout << endl;

	cout << qe.size() << endl;
}

void test2(void)
{
	queue<Teacher> qe;

	Teacher t1, t2, t3;
	t1.age = 10;
	t2.age = 20;
	t3.age = 30;
	qe.push(t1);
	qe.push(t2);
	qe.push(t3);

	while (!qe.empty())
	{
		Teacher temp = qe.front();
		temp.printT();
		qe.pop();
	}
}

void test3(void)
{
	queue<Teacher *> qe;

	Teacher t1, t2, t3;
	t1.age = 10;
	t2.age = 20;
	t3.age = 30;
	qe.push(&t1);
	qe.push(&t2);
	qe.push(&t3);

	while (!qe.empty())
	{
		Teacher *temp = qe.front();
		temp->printT();
		qe.pop();
	}
}
int main(void)
{
	cout << "-------------test1-----------" << endl;
	test1();
	cout << "-------------test2-----------" << endl;
	test2();
	cout << "-------------test3-----------" << endl;
	test3();
	return 0;
}