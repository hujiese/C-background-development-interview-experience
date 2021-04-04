#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
using namespace std;

class CMyShow
{
public:
	CMyShow()
	{
		num = 0;
	}
	void operator()(const int &iItem)
	{
		num++;
		cout << iItem;
	}

	void printCount()
	{
		cout << "num:" << num << endl;
	}

private:
	int num;
};

void show(const int &iItem)
{
	cout << iItem;
}
void main()
{
	int iArray[] = { 0, 1, 2, 3, 4 };
	vector<int> vecInt(iArray, iArray + sizeof(iArray) / sizeof(iArray[0]));
	for_each(vecInt.begin(), vecInt.end(), show);

	//结果打印出0 1 2 3 4
	CMyShow show1 = for_each(vecInt.begin(), vecInt.end(), CMyShow());
	cout << endl;
	show1.printCount(); //显示对象被调用的次数
}
