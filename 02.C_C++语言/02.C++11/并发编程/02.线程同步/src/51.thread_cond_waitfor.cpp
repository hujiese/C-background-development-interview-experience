#include <iostream>           // std::cout
#include <thread>             // std::thread
#include <chrono>             // std::chrono::seconds
#include <mutex>              // std::mutex, std::unique_lock
#include <condition_variable> // std::condition_variable, std::cv_status

std::condition_variable cv;

int value;

void do_read_value()
{
	std::cin >> value;
	cv.notify_one();
}

int main()
{
	std::cout << "Please, enter an integer (I'll be printing dots): \n";
	std::thread th(do_read_value);

	std::mutex mtx;
	std::unique_lock<std::mutex> lck(mtx);
	while (cv.wait_for(lck, std::chrono::seconds(1)) == std::cv_status::timeout) {
		std::cout << '.';
		std::cout.flush();
	}

	std::cout << "You entered: " << value << '\n';

	th.join();
	return 0;
}