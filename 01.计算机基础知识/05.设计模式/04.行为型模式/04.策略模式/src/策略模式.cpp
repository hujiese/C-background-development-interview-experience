#include <iostream>
using namespace std;


class Strategy
{
public:
	virtual void crypt() = 0;
};

//对称加密  速度快 加密大数据块文件 特点:加密密钥和解密密钥是一样的.
//非对称加密 加密速度慢 加密强度高 高安全性高 ;特点: 加密密钥和解密密钥不一样  密钥对(公钥 和 私钥)

class AES :  public Strategy
{
public:
	virtual void crypt()
	{
		cout << "AES加密算法" << endl;
 	}
};

class DES :  public Strategy
{
public:
	virtual void crypt()
	{
		cout << "DES 加密算法" << endl;
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