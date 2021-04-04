#define _CRT_SECURE_NO_WARNINGS
#include <forward_list>
#include <stdexcept>
#include <string>
#include <cstdlib> //abort()
#include <cstdio>  //snprintf()
#include <iostream>
#include <ctime> 
using namespace std;

namespace jj04
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

	void test_forward_list(const long& value)
	{
		cout << "\ntest_forward_list().......... \n";

		forward_list<string> c;
		char buf[10];

		clock_t timeStart = clock();
		for (long i = 0; i< value; ++i)
		{
			try {
				sprintf(buf, "%d", rand());
				c.push_front(string(buf));
			}
			catch (exception& p) {
				cout << "i=" << i << " " << p.what() << endl;
				abort();
			}
		}
		cout << "milli-seconds : " << (clock() - timeStart) << endl;
		cout << "forward_list.max_size()= " << c.max_size() << endl;  
		cout << "forward_list.front()= " << c.front() << endl;


		string target = get_a_target_string();
		timeStart = clock();
		auto pItem = find(c.begin(), c.end(), target);//调用stl标准库算法find()
		cout << "std::find(), milli-seconds : " << (clock() - timeStart) << endl;

		if (pItem != c.end())
			cout << "found, " << *pItem << endl;
		else
			cout << "not found! " << endl;

		timeStart = clock();
		c.sort();//调用forward_list内置sort()算法
		cout << "c.sort(), milli-seconds : " << (clock() - timeStart) << endl;

		c.clear();
	}
}

int main(void)
{
	jj04::test_forward_list(jj04::ASIZE);
	return 0;
}