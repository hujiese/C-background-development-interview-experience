#define _CRT_SECURE_NO_WARNINGS
#include <stack>
#include <stdexcept>
#include <string>
#include <cstdlib> //abort()
#include <cstdio>  //snprintf()
#include <iostream>
#include <ctime> 
using namespace std;

namespace jj17
{
	const long ASIZE = 50000L;//随机生成字符数个数

	void test_stack(const long& value)
	{
		cout << "\ntest_stack().......... \n";

		stack<string> c;
		char buf[10];

		clock_t timeStart = clock();
		for (long i = 0; i< value; ++i)
		{
			try {
				sprintf(buf,"%d", rand());
				c.push(string(buf));
			}
			catch (exception& p) {
				cout << "i=" << i << " " << p.what() << endl;
				abort();
			}
		}
		cout << "milli-seconds : " << (clock() - timeStart) << endl;
		cout << "stack.size()= " << c.size() << endl;
		cout << "stack.top()= " << c.top() << endl;
		c.pop();
		cout << "stack.size()= " << c.size() << endl;
		cout << "stack.top()= " << c.top() << endl;

	}
}

int main(void)
{
	jj17::test_stack(jj17::ASIZE);
	return 0;
}