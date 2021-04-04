#include <iostream>
using namespace std;


class Strategy
{
public:
	virtual void crypt() = 0;
};

//�ԳƼ���  �ٶȿ� ���ܴ����ݿ��ļ� �ص�:������Կ�ͽ�����Կ��һ����.
//�ǶԳƼ��� �����ٶ��� ����ǿ�ȸ� �߰�ȫ�Ը� ;�ص�: ������Կ�ͽ�����Կ��һ��  ��Կ��(��Կ �� ˽Կ)

class AES :  public Strategy
{
public:
	virtual void crypt()
	{
		cout << "AES�����㷨" << endl;
 	}
};

class DES :  public Strategy
{
public:
	virtual void crypt()
	{
		cout << "DES �����㷨" << endl;
	}
};


class Context
{
public:
	void setStrategy(Strategy *strategy)
	{
		this->strategy = strategy;
	}
	void myoperator()
	{
		strategy->crypt();
	}
private:
	Strategy *strategy;
};


int main(void)
{
	Strategy *strategy = NULL;

	//strategy = new DES();
	strategy = new AES();
	Context *context = new Context();
	context->setStrategy(strategy);
	context->myoperator();
	
	delete  strategy;
	delete  context;
		 
	return 0;
}