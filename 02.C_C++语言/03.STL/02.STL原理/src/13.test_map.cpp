#define _CRT_SECURE_NO_WARNINGS
#include <map>
#include <stdexcept>
#include <string>
#include <cstdlib> //abort()
#include <cstdio>  //snprintf()
#include <iostream>
#include <ctime> 
using namespace std;

namespace jj14
{
	const long ASIZE = 50000L;//随机生成字符数个数

	long get_a_target_long()
	{
		long target = 0;

		cout << "target (0~" << RAND_MAX << "): ";
		cin >> target;
		return target;
	}

	void test_map(const long& value)
	{
		cout << "\ntest_map().......... \n";

		map<long, string> c;
		char buf[10];

		clock_t timeStart = clock();
		for (long i = 0; i< value; ++i)
		{
			try {
				sprintf(buf, "%d", rand());
				c[i] = string(buf);
			}
			catch (exception& p) {
				cout << "i=" << i << " " << p.what() << endl;
				abort();
			}
		}
		cout << "milli-seconds : " << (clock() - timeStart) << endl;
		cout << "map.size()= " << c.size() << endl;
		cout << "map.max_size()= " << c.max_size() << endl;		//178956970

		long target = get_a_target_long();
		timeStart = clock();
		auto pItem = c.find(target);
		cout << "c.find(), milli-seconds : " << (clock() - timeStart) << endl;
		if (pItem != c.end())
			cout << "found, value=" << (*pItem).second << endl;
		else
			cout << "not found! " << endl;

		c.clear();
	}
}

int main(void)
{
	jj14::test_map(jj14::ASIZE);
	return 0;
}