#include <iostream>
#include <vector>
using namespace std;

class T1
{
public:
	T1() = delete;
};

class T2
{
private:
	T2();
};

int main(void)
{
	vector<T1> vec;
	T1 array[10]; // 	�޷����� "T1" ��Ĭ�Ϲ��캯�� -- ������ɾ���ĺ���

	vector<T2> vec;
	T2 array[10]; // 	"T2::T2()" (������ ��������:14) ���ɷ���

	return 0;
}