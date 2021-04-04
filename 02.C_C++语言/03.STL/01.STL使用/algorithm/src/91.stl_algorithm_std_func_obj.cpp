#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
using namespace std;

void main()
{
	plus<int> intAdd;
	int x = 10;
	int y = 20;
	int z = intAdd(x, y); //�ȼ��� x + y 
	cout << z << endl;

	plus<string> stringAdd;
	string myc = stringAdd("aaa", "bbb");
	cout << myc << endl;

	vector<string> v1;
	v1.push_back("bbb");
	v1.push_back("aaa");
	v1.push_back("ccc");
	v1.push_back("zzzz");

	//ȱʡ����£�sort()�õײ�Ԫ�����͵�С�ڲ���������������������Ԫ�ء�
	//Ϊ�˽��򣬿��Դ���Ԥ�������ģ��greater,�����õײ�Ԫ�����͵Ĵ��ڲ�������
	cout << "sort()��������" << endl;;
	sort(v1.begin(), v1.end(), greater<string>()); //�Ӵ�С
	for (vector<string>::iterator it = v1.begin(); it != v1.end(); it++)
	{
		cout << *it << endl;
	}
}
