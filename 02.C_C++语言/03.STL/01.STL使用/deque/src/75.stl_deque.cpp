#include <iostream>
#include "deque"
using namespace std;

void printDque(deque<int> &deq)
{
	for (deque<int>::iterator it = deq.begin(); it != deq.end(); it++)
	{
		cout << *it << " ";
	}
	cout << endl;
}
void test1(void)
{
	deque<int> dq;
	dq.push_back(1);
	dq.push_back(3);
	dq.push_back(5);

	dq.push_front(-1);
	dq.push_front(-3);
	dq.push_front(-5);

	printDque(dq);

	cout << "start : " << dq.front() << endl;
	cout << "end : " << dq.back() << endl;

	cout << " ------------------------- " << endl;

	dq.pop_front();
	dq.pop_back();

	printDque(dq);
	cout << "start : " << dq.front() << endl;
	cout << "end : " << dq.back() << endl;


	//查找 -33 在数组下标的值
	deque<int>::iterator it = find(dq.begin(), dq.end(), -33);
	if (it != dq.end())
	{
		cout << "-33数组下标是" << distance(dq.begin(), it) << endl;
	}
	else
	{
		cout << "没有找到值为-33的元素" << endl;
	}
}
int main(void)
{
	test1();
	return 0;
}