#define _CRT_SECURE_NO_WARNINGS
#include <unordered_map>
#include <stdexcept>
#include <string>
#include <cstdlib> //abort()
#include <cstdio>  //snprintf()
#include <iostream>
#include <ctime> 
using namespace std;

namespace jj09
{
	const long ASIZE = 50000L;//随机生成字符数个数

	long get_a_target_long()
	{
		long target = 0;

		cout << "target (0~" << RAND_MAX << "): ";
		cin >> target;
		return target;
	}

	void test_unordered_multimap(const long& value)
	{
		cout << "\ntest_unordered_multimap().......... \n";

		unordered_multimap<long, string> c;
		char buf[10];

		clock_t timeStart = clock();
		for (long i = 0; i< value; ++i)
		{
			try {
				sprintf(buf,"%d", rand());
				//multimap 不可使用 [] 進行 insertion 
				c.insert(pair<long, string>(i, buf));
			}
			catch (exception& p) {
				cout << "i=" << i << " " << p.what() << endl;
				abort();
			}
		}
		cout << "milli-seconds : " << (clock() - timeStart) << endl;
		cout << "unordered_multimap.size()= " << c.size() << endl;
		cout << "unordered_multimap.max_size()= " << c.max_size() << endl;	//357913941	

		long target = get_a_target_long();
		timeStart = clock();
		auto pItem = c.find(target);
		cout << "c.find(), milli-seconds : " << (clock() - timeStart) << endl;
		if (pItem != c.end())
			cout << "found, value=" << (*pItem).second << endl;
		else
			cout << "not found! " << endl;
	}
}

int main(void)
{
	jj09::test_unordered_multimap(jj09::ASIZE);
	return 0;
}