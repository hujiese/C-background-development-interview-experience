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
		cout << "造 车头" << endl;
	}
};

class BodyCarHandle : public CarHandle
{
public:
	virtual void HandleCar()
	{
		cout << "造 车身" << endl;
	}
};

class TailCarHandle : public CarHandle
{
public:
	virtual void HandleCar()
	{
		cout << "造 车尾" << endl;
	}
};


int main(void)
{
	CarHandle *headHandle = new HeadCarHandle();
	CarHandle *bodyHandle = new BodyCarHandle();
	CarHandle *tailHandle = new TailCarHandle();

	//业务逻辑 写死在客户端了..
	headHandle->HandleCar();
	bodyHandle->HandleCar();
	tailHandle->HandleCar();

	delete headHandle;
	delete bodyHandle;
	delete tailHandle;

	return 0;
}