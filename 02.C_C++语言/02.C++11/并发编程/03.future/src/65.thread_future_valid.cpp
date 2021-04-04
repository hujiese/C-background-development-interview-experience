#include <iostream>       // std::cout
#include <future>         // std::async, std::future
#include <utility>        // std::move

int do_get_value() { return 11; }

int main()
{
	// 由默认构造函数创建的 std::future 对象,
	// 初始化时该 std::future 对象处于为 invalid 状态.
	std::future<int> foo, bar;
	foo = std::async(do_get_value); // move 赋值, foo 变为 valid.
	bar = std::move(foo); // move 赋值, bar 变为 valid, 而 move 赋值以后 foo 变为 invalid.

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