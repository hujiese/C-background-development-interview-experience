#include <iostream>
#include <queue>
#include <functional>
using namespace std;

void main()
{
	priority_queue<int> p1; //Ĭ���� ���ֵ���ȼ����� 
	//priority_queue<int, vector<int>, less<int> > p1; //�൱������д
	priority_queue<int, vector<int>, greater<int>> p2; //��Сֵ���ȼ�����

	p1.push(33);
	p1.push(11);
	p1.push(55);
	p1.push(22);
	cout << "���д�С" << p1.size() << endl;
	cout << "��ͷ" << p1.top() << endl;

	while (p1.size() > 0)
	{
		cout << p1.top() << " ";
		p1.pop();
	}
	cout << endl;

	cout << "���� ��Сֵ���ȼ�����" << endl;
	p2.push(33);
	p2.push(11);
	p2.push(55);
	p2.push(22);
	while (p2.size() > 0)
	{
		cout << p2.top() << " ";
		p2.pop();

	}
}
