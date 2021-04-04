#include <iostream>
#include <thread>

int main()
{
	std::cout << "A Number of threads = " << std::thread::hardware_concurrency() << std::endl;
	return 0;
}
