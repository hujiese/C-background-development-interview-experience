#include<iostream>

class Animal {
public:
	virtual void cry() = 0;
};

class Dog : public Animal
{
public:
	virtual void cry()
	{
		std::cout << "旺旺" << std::endl;
	}
	void dohome()
	{
		std::cout << "看家" << std::endl;
	}
};

class Cat : public Animal
{
public:
	virtual void cry()
	{
		std::cout << "喵喵" << std::endl;
	}
	void dohome()
	{
		std::cout << "抓老鼠" << std::endl;
	}
};

int main()
{
	Animal* base = NULL;
	base = new Cat();
	base->cry();// 喵喵

	std::cout << "--------------------" << std::endl;
	//⽤用于将⽗父类指针转换成⼦子类，
	Dog	*pDog = dynamic_cast<Dog*>(base); //此时转换时失败的，因为父类指针现在指向的对象是猫，所以转换狗是失败的。
	//转换失败返回空（NULL）
	if (pDog != NULL)
	{
		pDog->cry();
		pDog->dohome();
	}
	std::cout << "--------------------" << std::endl;
	Cat* pCat = dynamic_cast<Cat*>(base); //此时转换成功，成功将父类指针转换成子类指针
	if (pCat != NULL)
	{
		pCat->cry();// 喵喵
		pCat->dohome();// 抓老鼠
	}

	system("pause");
	return 0;
}
