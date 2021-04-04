#include <iostream>
#include <thread>

void thread_function()
{
	std::cout << "thread function\n";
}

int main()
{
	std::thread t(&thread_function);
	std::cout << "main thread\n";
	t.detach();
	if (t.joinable())
	{
		std::cout << "joinable" << std::endl;
	}
	else
	{
		std::cout << "no join" << std::endl;
	}
	return 0;
}