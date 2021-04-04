#include <iostream>
#include <thread>
#include <list>
#include <algorithm>
#include <mutex>
using namespace std;

std::list<int> myList;
std::mutex mu;

void addToList(int max, int interval)
{
	
	for (int i = 0; i < max; i++) 
	{
		mu.lock();
		if ((i % interval) == 0)
		{
			myList.push_back(i);
		}
		mu.unlock();
	}
}

void printList()
{
	mu.lock();
	for (auto itr = myList.begin(), end_itr = myList.end(); itr != end_itr; ++itr) {
		cout << *itr << ",";
	}
	mu.unlock();
}

int main()
{
	int max = 100;

	std::thread t1(addToList, max, 1);
	std::thread t2(addToList, max, 10);
	std::thread t3(printList);

	t1.join();
	t2.join();
	t3.join();

	return 0;
}