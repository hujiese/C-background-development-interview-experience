#include <iostream>
#include <mutex>
#include <memory>
using namespace std;

//#define MUTEX
#define ONCE

#ifdef MUTEX
class Singleton
{
private:
	Singleton(){ cout << "created....." << endl; };
	static Singleton* sign;
	static mutex mut;
public:
	static Singleton* getInstance()
	{
		if (sign == nullptr)
		{
			mut.lock();
			if (sign == nullptr)
				sign = new Singleton;
			mut.unlock();
		}
		return sign;
	}
};
Singleton* Singleton::sign = nullptr;
mutex Singleton::mut;

int main(void)
{
	Singleton* s1 = Singleton::getInstance();
	Singleton* s2 = Singleton::getInstance();

	if (s1 == s2)
		cout << "the same ..." << endl;
	else
		cout << "the different ..." << endl;
}
#endif

#ifdef ONCE
class Singleton
{
private:
	Singleton(){ cout << "created....." << endl; };
	static unique_ptr<Singleton> ptr;
public:
	static Singleton& getInstance()
	{
		static once_flag flag;
		call_once(flag, [&]{
			ptr.reset(new Singleton);
		});
		return *ptr;
	}
};
unique_ptr<Singleton> Singleton::ptr;

int main(void)
{
	Singleton s1 = Singleton::getInstance();
	Singleton s2 = Singleton::getInstance();

	if (&s1 == &s2)
		cout << "the same ..." << endl;
	else
		cout << "the different ..." << endl;
}
#endif