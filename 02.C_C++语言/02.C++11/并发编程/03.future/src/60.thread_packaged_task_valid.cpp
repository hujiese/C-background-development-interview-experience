#include <iostream>     // std::cout
#include <utility>      // std::move
#include <future>       // std::packaged_task, std::future
#include <thread>       // std::thread

// 在新线程中启动一个 int(int) packaged_task.
std::future<int> launcher(std::packaged_task<int(int)>& tsk, int arg)
{
	if (tsk.valid()) {
		std::future<int> ret = tsk.get_future();
		std::thread(std::move(tsk), arg).detach();
		return ret;
	}
	else return std::future<int>();
}

int main()
{
	std::packaged_task<int(int)> tsk([](int x){return x * 2; });

	std::future<int> fut = launcher(tsk, 25);

	std::cout << "The double of 25 is " << fut.get() << ".\n";

	return 0;
}