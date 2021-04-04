#include <iostream>     // std::cout
#include <utility>      // std::move
#include <future>       // std::packaged_task, std::future
#include <thread>       // std::thread

int main()
{
	std::packaged_task<int(int)> foo; // Ĭ�Ϲ��캯��.

	// ʹ�� lambda ���ʽ��ʼ��һ�� packaged_task ����.
	std::packaged_task<int(int)> bar([](int x){return x * 2; });

	foo = std::move(bar); // move-��ֵ������Ҳ�� C++11 �е�������.

	// ��ȡ�� packaged_task ����״̬������� future ����.
	std::future<int> ret = foo.get_future();

	std::thread(std::move(foo), 10).detach(); // �����̣߳����ñ���װ������.

	int value = ret.get(); // �ȴ�������ɲ���ȡ���.
	std::cout << "The double of 10 is " << value << ".\n";

	return 0;
}