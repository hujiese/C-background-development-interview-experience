#include <iostream>
using namespace std;

//造完车 以后,需要把任务 传递下去
class CarHandle
{
public:
	virtual void HandleCar() = 0;
	CarHandle *setNextHandle(CarHandle *handle)
	{
		m_handle = handle;
		return m_handle;
	}
	
protected:
	CarHandle *m_handle;  //下一个处理单元
};

class HeadCarHandle : public CarHandle
{
public:
	virtual void HandleCar()
	{
		cout << "造车头" << endl;
		//造 玩车头 以后 把任务递交给 下一个处理者
		if (m_handle != NULL) 
		{
			m_handle->HandleCar(); //
		}
	}
};

class BodyCarHandle : public CarHandle
{
public:
	virtual void HandleCar()
	{
		cout << "造 车身" << endl;
		//造 造 车身 以后 把任务递交给 下一个处理者
		if (m_handle != NULL) 
		{
			m_handle->HandleCar(); //
		}

	}
};

class TailCarHandle : public CarHandle
{
public:
	virtual void HandleCar()
	{
		cout << "造车尾" << endl;
		//造 造车尾 以后 把任务递交给 下一个处理者
		if (m_handle != NULL) 
		{
			m_handle->HandleCar(); //
		}
	}
};


int main(void)
{
	CarHandle *headHandle = new HeadCarHandle();
	CarHandle *bodyHandle = new BodyCarHandle();
	CarHandle *tailHandle = new TailCarHandle();
		
	headHandle->setNextHandle(tailHandle);
	tailHandle->setNextHandle(bodyHandle);
	bodyHandle->setNextHandle(NULL);
	
	headHandle->HandleCar();

	delete headHandle;
	delete bodyHandle;
	delete tailHandle;

	return 0;
}