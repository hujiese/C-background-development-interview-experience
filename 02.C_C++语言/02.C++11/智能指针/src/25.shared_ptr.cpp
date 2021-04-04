#include <iostream>
#include <memory>

int main() {
	{
		int a = 10;
		std::shared_ptr<int> ptra = std::make_shared<int>(a);
		std::shared_ptr<int> ptra2(ptra); //copy
		std::cout << ptra.use_count() << std::endl;
		
		std::cout << "-----------------------------------" << std::endl;

		int b = 20;
		int *pb = &a;
		//std::shared_ptr<int> ptrb = pb;  //error
		std::shared_ptr<int> ptrb = std::make_shared<int>(b);
		ptra2 = ptrb; //assign
		pb = ptrb.get(); //获取原始指针

		std::cout << ptra.use_count() << std::endl;
		std::cout << ptrb.use_count() << std::endl;

		std::cout << "-----------------------------------" << std::endl;

		std::shared_ptr<int> temp(new int(10));
		std::cout << *(temp.get()) << std::endl;
	}
}