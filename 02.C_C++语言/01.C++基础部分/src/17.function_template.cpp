#include <iostream>
using namespace std;

int Max(int a, int b)
{
	cout << "int Max(int a, int b)" << endl;
	return a > b ? a : b;
}

template<typename T>
T Max(T a, T b)
{
	cout << "T Max(T a, T b)" << endl;
	return a > b ? a : b;
}

template<typename T>
T Max(T a, T b, T c)
{
	cout << "T Max(T a, T b, T c)" << endl;
	return Max(Max(a, b), c);
}


void main()
{
	int a = 1;
	int b = 2;

	cout << Max(a, b) << endl; //������ģ�����ͨ���������ϵ���ʱ,����ѡ����ͨ���� int Max(int a, int b)
	cout << Max<>(a, b) << endl; //����ʾʹ�ú���ģ��,��ʹ��<> �����б� T Max(T a, T b)

	cout << Max(3.0, 4.0) << endl; //��� ����ģ��������õ�ƥ�� ʹ�ú���ģ�� T Max(T a, T b)

	cout << Max(5.0, 6.0, 7.0) << endl; //���� T Max(T a, T b, T c)

	cout << Max('a', 100) << endl;  //������ͨ���� ������ʽ����ת��  int Max(int a, int b)
	system("pause");
	return;
}
