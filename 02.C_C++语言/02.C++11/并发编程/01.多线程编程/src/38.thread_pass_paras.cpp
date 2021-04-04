#include <iostream>
#include <thread>
#include <string>

void thread_function(std::string s)
{
	std::cout << "thread function ";
	std::cout << "message is = " << s << std::endl;
}

int main()
{
	std::string s = "Kathy Perry";
	std::thread t(&thread_function, s);
	std::cout << "main thread message = " << s << std::endl;
	t.join();
	return 0;
}