#include <iostream>
#include <memory>

int main()
{
	std::unique_ptr<int> pInt;
	pInt.reset(new int());
	int *p = pInt.release(); //释放所有权
	//由于unique_ptr有std::unique_ptr<T[]>的重载函数，所以它可以用来管理数组资源
	std::unique_ptr<int[]> pArray(new int[3]{1, 3, 3});

	std::cout << "-------------------------------" << std::endl;

	std::unique_ptr<int> uptr(new int(10));  //绑定动态对象
	//std::unique_ptr<int> uptr2 = uptr;  //不能赋值
	//std::unique_ptr<int> uptr2(uptr);  //不能拷贝
	std::unique_ptr<int> uptr2 = std::move(uptr); //转换所有权
	std::cout << *(uptr2.get()) << std::endl;
	uptr2.release(); //释放所有权
}