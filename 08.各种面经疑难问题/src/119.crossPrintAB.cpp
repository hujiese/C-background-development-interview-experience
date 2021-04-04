#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

bool flag = false;
mutex mut;
condition_variable cond;

void printAA(void)
{
	while (true)
	{
		unique_lock<mutex> lock(mut);
		cout << "AA";
		flag = true;
		cond.notify_all();
	}
}

void printBB(void)
{
	while (true)
	{
		unique_lock<mutex> lock(mut);
		while (!flag)
			cond.wait(lock);
		cout << "BB";
		flag = false;
	}
}

int main(void)
{
	thread t1(printAA);
	thread t2(printBB);

	t1.join();
	t2.join();

	return 0;
}