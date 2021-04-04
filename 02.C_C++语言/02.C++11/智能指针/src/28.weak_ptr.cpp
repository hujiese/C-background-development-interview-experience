#include <iostream>
#include <memory>

int main() {
	{
		std::shared_ptr<int> sh_ptr = std::make_shared<int>(10);
		std::cout << sh_ptr.use_count() << std::endl; // 1

		std::weak_ptr<int> wp(sh_ptr);
		std::cout << wp.use_count() << std::endl; // 1

		if (!wp.expired()){
			std::shared_ptr<int> sh_ptr2 = wp.lock(); //get another shared_ptr
			*sh_ptr = 100;
			std::cout << wp.use_count() << std::endl; // 2
		}
	}
	//release memory
}