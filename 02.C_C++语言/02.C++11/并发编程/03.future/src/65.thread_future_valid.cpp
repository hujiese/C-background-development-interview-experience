#include <iostream>       // std::cout
#include <future>         // std::async, std::future
#include <utility>        // std::move

int do_get_value() { return 11; }

int main()
{
	// ��Ĭ�Ϲ��캯�������� std::future ����,
	// ��ʼ��ʱ�� std::future ������Ϊ invalid ״̬.
	std::future<int> foo, bar;
	foo = std::async(do_get_value); // move ��ֵ, foo ��Ϊ valid.
	bar = std::move(foo); // move ��ֵ, bar ��Ϊ valid, �� move ��ֵ�Ժ� foo ��Ϊ invalid.

	if (foo.valid())
		std::cout << "foo's value: " << foo.get() << '\n';
	else
		std::cout << "foo is not valid\n";

	if (bar.valid())
		std::cout << "bar's value: " << bar.get() << '\n';
	else
		std::cout << "bar is not valid\n";

	return 0;
}