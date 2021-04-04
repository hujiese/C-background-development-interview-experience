#include<iostream>

class Animal {
public:
	void cry()
	{
		std::cout << "Animal cry" << std::endl;
	}
};

class Book {
public:
	void look()
	{
		std::cout << "Book look " << std::endl;
	}
};

int main()
{
	Animal* a = new Animal();
	a->cry();
	Book* b = reinterpret_cast<Book*>(a);  //强制类型的转换
	b->look();
	system("pause");
	return 0;
}