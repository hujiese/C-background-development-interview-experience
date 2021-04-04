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
		cout << "我是香蕉" << endl;
	}
};

class  Apple : public Fruit
{
public:
	void  sayname()
	{
		cout << "我是 Apple" << endl;
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
//添加新的产品

class Pear : public Fruit
{
public:
	virtual void sayname()
	{
		cout << "我是 pear" << endl;
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

	//吃 香蕉
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