#include <iostream>
using namespace std;

class DataOnly 
{
public:
	void fun(int a) {}
	void fun(float a) = delete;
};

int main(int argc, char *argv[]) 
{
	DataOnly data1;
	data1.fun(1); // OK
	data1.fun(0.5); // 1	IntelliSense:  �ж�� ���غ��� "DataOnly::fun" ʵ��������б�ƥ��
	return 0;
}

