#include <memory>
#include <iostream>
class Test
{
public:
	Test()
	{
		std::cout << "Test()" << std::endl;
	}
	void print(void)
	{
		std::cout << "----------Test::print(void)----------" << std::endl;
	}
	~Test()
	{
		std::cout << "~Test()" << std::endl;
	}
};
int main()
{
	std::shared_ptr<Test> p1 = std::make_shared<Test>();
	std::cout << "1 ref:" << p1.use_count() << std::endl;
	{
		std::shared_ptr<Test> p2 = p1;
		std::cout << "2 ref:" << p1.use_count() << std::endl;
	}
	std::cout << "3 ref:" << p1.use_count() << std::endl;

	std::cout << "--------------------------" << std::endl;

	std::shared_ptr<Test> temp(new Test());
	temp->print();

	Test* temp2 = temp.get();
	temp2->print();

	return 0;
}