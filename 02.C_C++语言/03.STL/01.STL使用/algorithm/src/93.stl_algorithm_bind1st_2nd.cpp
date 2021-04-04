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

	//通过谓词求大于2的个数
	int num2 = count_if(v1.begin(), v1.end(), IsGreat(2));
	cout << "num2:" << num2 << endl;

	//通过预定义函数对象求大于2的个数   greater<int>() 有2个参数 
	//												param > 2
	int num3 = count_if(v1.begin(), v1.end(), bind2nd(greater<int>(), 2));
	cout << "num3:" << num3 << endl;

	int num3x = count_if(v1.begin(), v1.end(), bind1st(greater<int>(), 2));
	cout << "num3x:" << num3x << endl;

	//取模 能被2整除的数 求奇数
	int num4 = count_if(v1.begin(), v1.end(), bind2nd(modulus <int>(), 2));
	cout << "奇数num4:" << num4 << endl;

	int num5 = count_if(v1.begin(), v1.end(), not1(bind2nd(modulus <int>(), 2)));
	cout << "偶数num5:" << num5 << endl;
}
