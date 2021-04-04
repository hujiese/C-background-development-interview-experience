#include <iostream>       // std::cout
#include <functional>     // std::ref
#include <thread>         // std::thread
#include <future>         // std::promise, std::future
//#include <Windows.h>
void print_int(std::future<int>& fut) {
	int x = fut.get(); // 获取共享状态的值.
	std::cout << "value: " << x << '\n'; // 打印 value: 10.
}

int main()
{
	std::promise<int> prom; // 生成一个 std::promise<int> 对象.
	std::future<int> fut = prom.get_future(); // 和 future 关联.
	std::thread t(print_int, std::ref(fut)); // 将 future 交给另外一个线程t.
	/*Sleep(1000);*/
	prom.set_value(10); // 设置共享状态的值, 此处和线程t保持同步.
	t.join();
	return 0;
}