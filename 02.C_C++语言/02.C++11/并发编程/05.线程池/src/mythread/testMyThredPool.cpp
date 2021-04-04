#include <iostream>
#include "MyThreadPool.h"
using namespace std;

void print()
{
	printf("tid=%d\n", std::this_thread::get_id());
}

void printString(const std::string& str)
{
	/*printf("tid=%d, str=%s\n", std::this_thread::get_id(), str.c_str());*/
	printf("tid=%d\t", std::this_thread::get_id());
	printf("str=%s\n", str.c_str());
	
}

int main()
{
	hujie::ThreadPool pool("MainThreadPool");
	pool.start(5);

	pool.run(print);
	for (int i = 0; i < 100; ++i)
	{
		char buf[32];
		_snprintf_s(buf, sizeof buf, "task %d", i);
		pool.run(std::bind(printString, std::string(buf)));
	}
	//pool.stop();
	while (true);
}