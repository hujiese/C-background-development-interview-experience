#include <iostream>
#include <thread>
#include <string>

void thread_function(std::string &s)
{
	std::cout << "thread function ";
	std::cout << "message is = " << s << std::endl;
	s = "Justin Beaver";
}

int main()
{
	std::string s = "Kathy Perry";
	/*std::thread t(&thread_function, s);*/
	std::thread t(&thread_function, std::ref(s));
	t.join();
	std::cout << "main thread message = " << s << std::endl;
	return 0;
}