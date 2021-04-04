#include <iostream>
#include <memory>

int main()
{
	std::unique_ptr<int> pInt;
	pInt.reset(new int());
	int *p = pInt.release(); //�ͷ�����Ȩ
	//����unique_ptr��std::unique_ptr<T[]>�����غ�����������������������������Դ
	std::unique_ptr<int[]> pArray(new int[3]{1, 3, 3});

	std::cout << "-------------------------------" << std::endl;

	std::unique_ptr<int> uptr(new int(10));  //�󶨶�̬����
	//std::unique_ptr<int> uptr2 = uptr;  //���ܸ�ֵ
	//std::unique_ptr<int> uptr2(uptr);  //���ܿ���
	std::unique_ptr<int> uptr2 = std::move(uptr); //ת������Ȩ
	std::cout << *(uptr2.get()) << std::endl;
	uptr2.release(); //�ͷ�����Ȩ
}