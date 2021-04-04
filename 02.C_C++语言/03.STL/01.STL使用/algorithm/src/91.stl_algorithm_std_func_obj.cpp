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
	int z = intAdd(x, y); //等价于 x + y 
	cout << z << endl;

	plus<string> stringAdd;
	string myc = stringAdd("aaa", "bbb");
	cout << myc << endl;

	vector<string> v1;
	v1.push_back("bbb");
	v1.push_back("aaa");
	v1.push_back("ccc");
	v1.push_back("zzzz");

	//缺省情况下，sort()用底层元素类型的小于操作符以升序排列容器的元素。
	//为了降序，可以传递预定义的类模板greater,它调用底层元素类型的大于操作符：
	cout << "sort()函数排序" << endl;;
	sort(v1.begin(), v1.end(), greater<string>()); //从大到小
	for (vector<string>::iterator it = v1.begin(); it != v1.end(); it++)
	{
		cout << *it << endl;
	}
}
