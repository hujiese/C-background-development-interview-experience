
#include <iostream>
using namespace std;

class Fruit 
{
public:
	virtual void getFruit() = 0;
};

class Banana : public Fruit
{
public:
	virtual void getFruit()
	{
		cout << "�����㽶...." << endl;
	}
};

class Apple : public Fruit
{
public:
	virtual void getFruit()
	{
		cout << "����ƻ��...." << endl;
	}
};


class Factory
{
public:
	Fruit *create(char *p)
	{

		if (strcmp(p, "banana") == 0)
		{
			return new Banana;	 
		}
		else if (strcmp(p, "apple") == 0)
		{
			return new Apple;
		}
		else
		{
			printf("��֧��\n" ) ;
			return NULL;
		}
	}
};


int main(void)
{
	Factory *f = new Factory;

	Fruit *fruit = NULL;


	//�������� �㽶
	fruit = f->create("banana");
	fruit->getFruit();
	delete fruit;


	fruit = f->create("apple");
	fruit->getFruit();
	delete fruit;

	delete f;
	return 0;
}