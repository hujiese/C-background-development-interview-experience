#include <iostream>
#include <thread>

void thread_function()
{
	std::cout << "thread function\n";
}

int main()
{
	std::thread t(&thread_function);   // t starts running
	//std::thread t(thread_function);
	std::cout << "main thread\n";
	t.join();   // main thread waits for the thread t to finish
	return 0;
}