#include <iostream>
using namespace std;

class Fruit
{
public:
	virtual void  sayname()  = 0;
};

class  Banana : public Fruit
{
public:
	void  sayname()
	{
		cout << "�����㽶" << endl;
	}
};

class  Apple : public Fruit
{
public:
	void  sayname()
	{
		cout << "���� Apple" << endl;
	} 
};

class  AbFactory 
{
public:
	virtual Fruit *CreateProduct() = 0;
};

class BananaFactory :public AbFactory
{
public:
	virtual Fruit *CreateProduct()
	{
		return new Banana();
	}
};

class AppleFactory :public AbFactory
{
public:
	virtual Fruit *CreateProduct()
	{
		return new Apple();
	}
};

//////////////////////////////////////////////////////////////////////////
//����µĲ�Ʒ

class Pear : public Fruit
{
public:
	virtual void sayname()
	{
		cout << "���� pear" << endl;
	}
};

class PearFactory : public AbFactory
{
public:
	virtual Fruit *CreateProduct()
	{
		return new Pear();
	}
};

int main(void)
{
	AbFactory		*factory = NULL;
	Fruit			*fruit = NULL;

	//�� �㽶
	factory = new BananaFactory;
	fruit = factory->CreateProduct();
	fruit->sayname();

	delete fruit;
	delete factory;


	//Pear 
	factory = new PearFactory;
	fruit = factory->CreateProduct();
	fruit->sayname();

	delete fruit;
	delete factory;

	return 0;
}