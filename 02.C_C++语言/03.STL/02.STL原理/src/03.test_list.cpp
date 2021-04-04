#define _CRT_SECURE_NO_WARNINGS
#include <list>
#include <stdexcept>
#include <string>
#include <cstdlib> //abort()
#include <cstdio>  //snprintf()
#include <algorithm> //find()
#include <iostream>
#include <ctime> 
using namespace std;

namespace jj03
{
	const long ASIZE = 50000L;//随机生成字符数个数

	string get_a_target_string()
	{
		long target = 0;
		char buf[10];

		cout << "target (0~" << RAND_MAX << "): ";
		cin >> target;
		sprintf(buf, "%d", target);
		return string(buf);
	}

	void test_list(const long& value)
	{
		cout << "\ntest_list().......... \n";

		list<string> c;
		char buf[10];

		clock_t timeStart = clock();
		for (long i = 0; i< value; ++i)
		{
			try {
				sprintf(buf, "%d", rand());
				c.push_back(string(buf));
			}
			catch (exception& p) {
				cout << "i=" << i << " " << p.what() << endl;
				abort();
			}
		}
		cout << "milli-seconds : " << (clock() - timeStart) << endl;
		cout << "list.size()= " << c.size() << endl;
		cout << "list.max_size()= " << c.max_size() << endl;
		cout << "list.front()= " << c.front() << endl;
		cout << "list.back()= " << c.back() << endl;

		string target = get_a_target_string();
		timeStart = clock();
		auto pItem = find(c.begin(), c.end(), target);//调用stl标准库算法find()
		cout << "std::find(), milli-seconds : " << (clock() - timeStart) << endl;

		if (pItem != c.end())
			cout << "found, " << *pItem << endl;
		else
			cout << "not found! " << endl;

		timeStart = clock();
		c.sort();//调用list内置sort()算法
		cout << "c.sort(), milli-seconds : " << (clock() - timeStart) << endl;

		c.clear();
	}
}

int main(void)
{
	jj03::test_list(jj03::ASIZE);
	return 0;
}