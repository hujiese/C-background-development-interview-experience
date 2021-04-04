#include <iostream>
using namespace std;

class Base{
public:
	int x;
	char c;

	Base(){};               //���캯��
	virtual ~Base(){};      //��������

	virtual void func(void){
		cout << "virtual func" << endl;
	};
};

class Son1 : public Base
{

};

class Son2 : public Base
{
	virtual void func(void){
		cout << "son2 's virtual func" << endl;
	};
};

int main()
{
	Base* base = new Base;
	cout << "�������ʼ��ַΪ��" << base << endl; // �������ʼ��ַΪ��00956848
	cout << "����obj�Ĵ�СΪ:  " << sizeof(*base) << endl; // �麯����ָ���ַ��00956848

	typedef void(*Fun)(void);  //���庯��ָ������
	Fun pFun = NULL;           //���庯��ָ��
	cout << "�麯����ָ���ַ��" << (int*)base << endl;  // �麯����ָ���ַ��00956848
	cout << "�麯���� �� ��һ��������ַ��" << *((int*)*(int*)base + 1) << endl; // �麯���� �� ��һ��������ַ��2167456
	pFun = (Fun)*((int*)*(int*)base + 1);
	pFun(); // virtual func

	cout << "------------------------------------------" << endl;

	Son1* son1 = new Son1;
	cout << "�������ʼ��ַΪ��" << son1 << endl; // �������ʼ��ַΪ��0095C658
	cout << "����obj�Ĵ�СΪ:  " << sizeof(*son1) << endl; // ����obj�Ĵ�СΪ : 12

	typedef void(*Fun)(void);  //���庯��ָ������
	Fun pFun2 = NULL;           //���庯��ָ��
	cout << "�麯����ָ���ַ��" << (int*)son1 << endl; // �麯����ָ���ַ��0095C658
	cout << "�麯���� �� ��һ��������ַ��" << *((int*)*(int*)son1 + 1) << endl; // �麯���� �� ��һ��������ַ��2167456
	pFun2 = (Fun)*((int*)*(int*)son1 + 1);
	pFun2(); // virtual func

	cout << "------------------------------------------" << endl;

	base = son1;

	cout << "�������ʼ��ַΪ��" << base << endl; // �������ʼ��ַΪ��0095C658
	cout << "����obj�Ĵ�СΪ:  " << sizeof(*base) << endl; // ����obj�Ĵ�СΪ : 12

	typedef void(*Fun)(void);  //���庯��ָ������
	Fun pFun3 = NULL;           //���庯��ָ��
	cout << "�麯����ָ���ַ��" << (int*)base << endl; // �麯����ָ���ַ��0095C658
	cout << "�麯���� �� ��һ��������ַ��" << *((int*)*(int*)base + 1) << endl; // �麯���� �� ��һ��������ַ��2167456
	pFun3 = (Fun)*((int*)*(int*)base + 1);
	pFun3(); // virtual func

	cout << "------------------------------------------" << endl;

	Son2* son2 = new Son2;
	cout << "�������ʼ��ַΪ��" << son2 << endl; // �������ʼ��ַΪ��0095C670
	cout << "����obj�Ĵ�СΪ:  " << sizeof(*son2) << endl; // ����obj�Ĵ�СΪ : 12

	typedef void(*Fun)(void);  //���庯��ָ������
	Fun pFun4 = NULL;           //���庯��ָ��
	cout << "�麯����ָ���ַ��" << (int*)(&son2) << endl; // �麯����ָ���ַ��0073FEE0
	cout << "�麯���� �� ��һ��������ַ��" << *((int*)*(int*)son2 + 1) << endl; // �麯���� �� ��һ��������ַ��2167536
	pFun4 = (Fun)*((int*)*(int*)son2 + 1);
	pFun4(); // virtual func

	cout << "------------------------------------------" << endl;

	base = son2;

	cout << "�������ʼ��ַΪ��" << base << endl; // �������ʼ��ַΪ��0095C670
	cout << "����obj�Ĵ�СΪ:  " << sizeof(*base) << endl; // ����obj�Ĵ�СΪ : 12

	typedef void(*Fun)(void);  //���庯��ָ������
	Fun pFun5 = NULL;           //���庯��ָ��
	cout << "�麯����ָ���ַ��" << (int*)base << endl; // �麯����ָ���ַ��0095C670
	cout << "�麯���� �� ��һ��������ַ��" << *((int*)*(int*)base + 1) << endl; // �麯���� �� ��һ��������ַ��2167536
	pFun5 = (Fun)*((int*)*(int*)base + 1);

	pFun5(); // virtual func

	delete base;
	delete son1;
	return 0;
}