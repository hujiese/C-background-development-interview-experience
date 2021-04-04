#include <iostream>                // std::cout
#include <future>                // std::async, std::future
#include <chrono>                // std::chrono::milliseconds

// a non-optimized way of checking for prime numbers:
bool do_check_prime(int x) // 为了体现效果, 该函数故意没有优化.
{
	for (int i = 2; i < x; ++i)
	if (x % i == 0)
		return false;
	return true;
}

int main()
{
	// call function asynchronously:
	std::future < bool > fut = std::async(do_check_prime, 194232491);

	std::cout << "Checking...\n";
	std::chrono::milliseconds span(500); // 设置超时间隔.

	// 如果超时，则输出"."，继续等待
	while (fut.wait_for(span) == std::future_status::timeout)
		std::cout << '.';

	std::cout << "\n194232491 ";
	if (fut.get()) // guaranteed to be ready (and not block) after wait returns
		std::cout << "is prime.\n";
	else
		std::cout << "is not prime.\n";

	return 0;
}