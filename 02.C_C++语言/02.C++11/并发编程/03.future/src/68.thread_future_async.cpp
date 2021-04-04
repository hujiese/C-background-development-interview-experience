#include <stdio.h>
#include <stdlib.h>

#include <cmath>
#include <chrono>
#include <future>
#include <iostream>

double ThreadTask(int n) {
	std::cout << std::this_thread::get_id()
		<< " start computing..." << std::endl;

	double ret = 0;
	for (int i = 0; i <= n; i++) {
		ret += std::sin(i);
	}

	std::cout << std::this_thread::get_id()
		<< " finished computing..." << std::endl;
	return ret;
}

int main(int argc, const char *argv[])
{
	std::future<double> f(std::async(std::launch::async, ThreadTask, 100000000));

#if 0
	while (f.wait_until(std::chrono::system_clock::now() + std::chrono::seconds(1))
		!= std::future_status::ready) {
		std::cout << "task is running...\n";
	}
#else
	while (f.wait_for(std::chrono::seconds(1))
		!= std::future_status::ready) {
		std::cout << "task is running...\n";
	}
#endif

	std::cout << f.get() << std::endl;

	return EXIT_SUCCESS;
}