#include <iostream>
#include <string>
#include <algorithm>
#include <functional>
using namespace std;

template <typename T>
void FuncShowElemt(T &t)  //��ͨ���� ������ �º���������¼״̬
{
	cout << t << " ";
};

void showChar(char &t)
{
	cout << t << " ";
}

//����ģ�� ���� �������ò�����
template <typename T>
class ShowElemt
{
public:
	ShowElemt()
	{
		n = 0;
	}
	void operator()(T &t)
	{
		n++;
		cout << t << " ";
	}
	void printCount()
	{
		cout << n << endl;
	}
public:
	int n;
};

//1 �������� ����ʹ��
void main()
{
	int a = 100;
	FuncShowElemt<int>(a); //��ͨ�ĺ�������
	ShowElemt<int> showElemt; //�������� 
	showElemt(a); //����������� 
}
