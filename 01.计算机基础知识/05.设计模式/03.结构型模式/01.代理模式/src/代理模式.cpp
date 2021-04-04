#include <iostream>
using namespace std;

class Subject
{
public:
	virtual void salebook() = 0;
};

class RealSubjectBook : public Subject
{
public:
	virtual void salebook()
	{
		cout << "卖书" << endl;
	}
};

//a中包含b类；a、b类实现协议类protocol 
class dangdangProxy : public Subject
{
public:
	virtual void salebook()
	{
		RealSubjectBook *rsb = new RealSubjectBook();
		rsb->salebook();
		discount();
	}
public:
	void discount()
	{
		cout << "双十一打折" << endl;
	}
private:
	Subject *m_subject;
};

int main(void)
{
	Subject *s = new dangdangProxy();
	s->salebook();
	delete s;

	return 0;
}