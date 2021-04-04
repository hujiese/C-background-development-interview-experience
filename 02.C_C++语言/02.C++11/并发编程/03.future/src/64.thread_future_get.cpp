#include <iostream>       // std::cin, std::cout, std::ios
#include <functional>     // std::ref
#include <thread>         // std::thread
#include <future>         // std::promise, std::future
#include <exception>      // std::exception, std::current_exception

void get_int(std::promise<int>& prom) {
	int x;
	std::cout << "Please, enter an integer value: ";
	std::cin.exceptions(std::ios::failbit);   // throw on failbit
	try {
		std::cin >> x;                         // sets failbit if input is not int
		prom.set_value(x);
	}
	catch (std::exception&) {
		prom.set_exception(std::current_exception());
	}
}

void print_int(std::future<int>& fut) {
	try {
		int x = fut.get();
		std::cout << "value: " << x << '\n';
	}
	catch (std::exception& e) {
		std::cout << "[exception caught: " << e.what() << "]\n";
	}
}

int main()
{
	std::promise<int> prom;
	std::future<int> fut = prom.get_future();

	std::thread th1(get_int, std::ref(prom));
	std::thread th2(print_int, std::ref(fut));

	th1.join();
	th2.join();
	return 0;
}