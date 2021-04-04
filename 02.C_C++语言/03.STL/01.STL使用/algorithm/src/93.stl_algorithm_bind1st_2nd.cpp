#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
using namespace std;

class IsGreat
{
public:
	IsGreat(int i)
	{
		m_num = i;
	}
	bool operator()(int &num)
	{
		if (num > m_num)
		{
			return true;
		}
		return false;
	}
protected:
private:
	int m_num;
};

void main()
{
	vector<int>  v1;
	for (int i = 0; i<5; i++)
	{
		v1.push_back(i + 1);
	}

	for (vector<int>::iterator it = v1.begin(); it != v1.end(); it++)
	{
		cout << *it << " ";
	}

	int num1 = count(v1.begin(), v1.end(), 3);
	cout << "num1:" << num1 << endl;

	//ͨ��ν�������2�ĸ���
	int num2 = count_if(v1.begin(), v1.end(), IsGreat(2));
	cout << "num2:" << num2 << endl;

	//ͨ��Ԥ���庯�����������2�ĸ���   greater<int>() ��2������ 
	//												param > 2
	int num3 = count_if(v1.begin(), v1.end(), bind2nd(greater<int>(), 2));
	cout << "num3:" << num3 << endl;

	int num3x = count_if(v1.begin(), v1.end(), bind1st(greater<int>(), 2));
	cout << "num3x:" << num3x << endl;

	//ȡģ �ܱ�2�������� ������
	int num4 = count_if(v1.begin(), v1.end(), bind2nd(modulus <int>(), 2));
	cout << "����num4:" << num4 << endl;

	int num5 = count_if(v1.begin(), v1.end(), not1(bind2nd(modulus <int>(), 2)));
	cout << "ż��num5:" << num5 << endl;
}
