#define _CRT_SECURE_NO_WARNINGS
#include <unordered_set>
#include <stdexcept>
#include <string>
#include <cstdlib> //abort()
#include <cstdio>  //snprintf()
#include <iostream>
#include <ctime> 
using namespace std;

namespace jj08
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

	void test_unordered_multiset(const long& value)
	{
		cout << "\ntest_unordered_multiset().......... \n";

		unordered_multiset<string> c;
		char buf[10];

		clock_t timeStart = clock();
		for (long i = 0; i< value; ++i)
		{
			try {
				sprintf(buf, "%d", rand());
				c.insert(string(buf));
			}
			catch (exception& p) {
				cout << "i=" << i << " " << p.what() << endl;
				abort();
			}
		}
		cout << "milli-seconds : " << (clock() - timeStart) << endl;
		cout << "unordered_multiset.size()= " << c.size() << endl;
		cout << "unordered_multiset.max_size()= " << c.max_size() << endl;	//357913941
		cout << "unordered_multiset.bucket_count()= " << c.bucket_count() << endl;
		cout << "unordered_multiset.load_factor()= " << c.load_factor() << endl;
		cout << "unordered_multiset.max_load_factor()= " << c.max_load_factor() << endl;
		cout << "unordered_multiset.max_bucket_count()= " << c.max_bucket_count() << endl;
		for (unsigned i = 0; i< 20; ++i) {
			cout << "bucket #" << i << " has " << c.bucket_size(i) << " elements.\n";
		}

		string target = get_a_target_string();
		{
			timeStart = clock();
			auto pItem = find(c.begin(), c.end(), target);	//比 c.find(...) 慢很多	
			cout << "std::find(), milli-seconds : " << (clock() - timeStart) << endl;
			if (pItem != c.end())
				cout << "found, " << *pItem << endl;
			else
				cout << "not found! " << endl;
		}

		{
			timeStart = clock();
			auto pItem = c.find(target);		//比 std::find(...) 快很多							
			cout << "c.find(), milli-seconds : " << (clock() - timeStart) << endl;
			if (pItem != c.end())
				cout << "found, " << *pItem << endl;
			else
				cout << "not found! " << endl;
		}

		c.clear();
	}
}

int main(void)
{
	jj08::test_unordered_multiset(jj08::ASIZE);
	return 0;
}