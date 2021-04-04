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
	//std::thread t2 = t;// 错误	1	error C2280: “std::thread::thread(const std::thread &)”: 尝试引用已删除的函数
	std::thread t2 = move(t);
	t2.join();
	return 0;
}