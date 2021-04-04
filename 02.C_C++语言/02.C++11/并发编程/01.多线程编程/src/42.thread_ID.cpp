#include <iostream>
#include <thread>

void thread_function()
{
	std::cout << "thread function id is " << std::this_thread::get_id() << std::endl;;
}
int main()
{
	std::thread t(&thread_function);
	std::cout << "child thread id = " << t.get_id() << std::endl;

	t.join();
	std::cout << "main thread id = " << std::this_thread::get_id() << std::endl;
	return 0;
}
