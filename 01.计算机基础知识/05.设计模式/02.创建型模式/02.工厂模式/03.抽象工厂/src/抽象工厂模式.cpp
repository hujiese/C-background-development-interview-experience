
#include <iostream>
using namespace std;

class Fruit
{
public:
	virtual void SayName() = 0;
};

class AbstractFactory
{
public:
	virtual Fruit* CreateBanana() = 0;
	virtual Fruit* CreateApple() = 0;
};

class NorthBanana : public Fruit
{
public:
	virtual void SayName()
	{
		cout << "���Ǳ����㽶" << endl;
	}
};

class NorthApple : public Fruit
{
public:
	virtual void SayName()
	{
		cout << "���Ǳ���ƻ��" << endl;
	}
};


class SourthBanana : public Fruit
{
public:
	virtual void SayName()
	{
		cout << "�����Ϸ��㽶" << endl;
	}
};


class SourthApple : public Fruit
{
public:
	virtual void SayName()
	{
		cout << "�����Ϸ�ƻ��" << endl;
	}
};

class NorthFacorty : public AbstractFactory
{
	virtual Fruit * CreateBanana()
	{
		return new NorthBanana();
	}
	virtual Fruit * CreateApple()
	{
		return new NorthApple();
	}
};

class SourthFacorty : public AbstractFactory
{
	virtual Fruit * CreateBanana()
	{
		return new SourthBanana();
	}
	virtual Fruit * CreateApple()
	{
		return new SourthApple();
	}
};


int main(void)
{
	Fruit			*fruit = NULL;
	AbstractFactory *af = NULL;

	af = new SourthFacorty;
	fruit = af->CreateApple();
	fruit->SayName();
	delete fruit;
	fruit = af->CreateBanana();
	fruit->SayName();
	delete fruit;

	af = new NorthFacorty;
	fruit = af->CreateApple();
	fruit->SayName();
	delete fruit;
	fruit = af->CreateBanana();
	fruit->SayName();
	delete fruit;

	return 0;
}