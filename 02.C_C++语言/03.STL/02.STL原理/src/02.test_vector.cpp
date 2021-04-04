#define _CRT_SECURE_NO_WARNINGS
#include <vector>
#include <stdexcept>
#include <string>
#include <cstdlib> //abort()
#include <cstdio>  //snprintf()
#include <iostream>
#include <ctime> 
#include <algorithm> 	//sort()
using namespace std;

namespace jj02
{
	const long ASIZE = 50000L;//随机生成字符数个数

	//用户输入需要查找的string类型元素
	string get_a_target_string()
	{
		long target = 0;
		char buf[10];

		cout << "target (0~" << RAND_MAX << "): ";
		cin >> target;
		sprintf(buf, "%d", target);
		return string(buf);
	}

	//字符串比较
	int compareStrings(const void* a, const void* b)
	{
		if (*(string*)a > *(string*)b)
			return 1;
		else if (*(string*)a < *(string*)b)
			return -1;
		else
			return 0;
	}

	void test_vector(const long& value)
	{
		cout << "\ntest_vector().......... \n";

		vector<string> c;//定义一个vector容器存放string类型数据
		char buf[10];

		clock_t timeStart = clock();//开始计时
		//将value个string送入vector中
		for (long i = 0; i< value; ++i)
		{
			try {//这里使用try...catch是为了避免value太大导致内存不够出现异常
				sprintf(buf, "%d", rand());
				c.push_back(string(buf));//将string送入vector中
			}
			catch (exception& p) {
				cout << "i=" << i << " " << p.what() << endl;
				//曾經最高 i=58389486 then std::bad_alloc
				abort();
			}
		}
		cout << "milli-seconds : " << (clock() - timeStart) << endl;//打印初始化array时间
		cout << "vector.max_size()= " << c.max_size() << endl;	//1073747823
		cout << "vector.size()= " << c.size() << endl; // 打印vector长度
		cout << "vector.front()= " << c.front() << endl;//打印vector首元素
		cout << "vector.back()= " << c.back() << endl;//打印vector最后元素
		cout << "vector.data()= " << c.data() << endl;//打印vector在内存的首地址
		cout << "vector.capacity()= " << c.capacity() << endl << endl;//打印vector容量


		string target = get_a_target_string();//获取用户输入字符串
		//该代码块测试stl的find函数，查找用户输入字符串，并打印查找所花费时间
		{
			timeStart = clock();
			auto pItem = find(c.begin(), c.end(), target);
			cout << "std::find(), milli-seconds : " << (clock() - timeStart) << endl;

			if (pItem != c.end())
				cout << "found, " << *pItem << endl << endl;
			else
				cout << "not found! " << endl << endl;
		}
		//该代码块测试c标准库的二分查找bsearch函数，查找用户输入字符串，并打印查找所花费时间
		{
			timeStart = clock();
			sort(c.begin(), c.end());//排序
			cout << "sort(), milli-seconds : " << (clock() - timeStart) << endl;//打印排序时间

			timeStart = clock();
			string* pItem = (string*)::bsearch(&target, (c.data()),//二分查找
				c.size(), sizeof(string), compareStrings);
			cout << "bsearch(), milli-seconds : " << (clock() - timeStart) << endl;//打印二分查找时间

			if (pItem != NULL)
				cout << "found, " << *pItem << endl << endl;
			else
				cout << "not found! " << endl << endl;
		}

	}
}

int main(void)
{
	jj02::test_vector(jj02::ASIZE);
	return 0;
}