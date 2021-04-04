#include <iostream>
using namespace std;

class CarHandle
{
public:
	virtual void HandleCar() = 0;
};

class HeadCarHandle : public CarHandle
{
public:
	virtual void HandleCar()
	{
		cout << "�� ��ͷ" << endl;
	}
};

class BodyCarHandle : public CarHandle
{
public:
	virtual void HandleCar()
	{
		cout << "�� ����" << endl;
	}
};

class TailCarHandle : public CarHandle
{
public:
	virtual void HandleCar()
	{
		cout << "�� ��β" << endl;
	}
};


int main(void)
{
	CarHandle *headHandle = new HeadCarHandle();
	CarHandle *bodyHandle = new BodyCarHandle();
	CarHandle *tailHandle = new TailCarHandle();

	//ҵ���߼� д���ڿͻ�����..
	headHandle->HandleCar();
	bodyHandle->HandleCar();
	tailHandle->HandleCar();

	delete headHandle;
	delete bodyHandle;
	delete tailHandle;

	return 0;
}