#include <iostream>
using namespace std;

//// �����ⲿ����
//int main()
//{
//	int a = 123;
//	auto f = [a] { cout << a << endl; };
//	f(); // �����123
//
//	//��ͨ���������塱����ġ�()���������
//	[](int a){cout << a << endl; }(1233435);
//}

//// 1��ֵ����
//int main()
//{
//    int a = 123;
//    auto f = [a] { cout << a << endl; }; 
//    a = 321;
//    f(); // �����123
//}

////2�����ò���
//int main()
//{
//	int a = 123;
//	auto f = [&a] { cout << a << endl; };
//	a = 321;
//	f(); // �����321
//}

////��ʽֵ����
//int main()
//{
//	int a = 123;
//	auto f = [=] { cout << a << endl; };    // ֵ����
//	f(); // �����123
//}

////��ʽ���ò���
//int main()
//{
//	int a = 123;
//	auto f = [&] { cout << a << endl; };    // ���ò���
//	a = 321;
//	f(); // �����321
//}

// �޸Ĳ������
int main()
{
	int a = 123;
	auto f = [a]()mutable { cout << ++a; }; // ���ᱨ��
	cout << a << endl; // �����123
	f(); // �����124
}