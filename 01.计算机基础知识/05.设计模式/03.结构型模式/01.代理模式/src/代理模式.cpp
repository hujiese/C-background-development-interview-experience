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
		cout << "����" << endl;
	}
};

//a�а���b�ࣻa��b��ʵ��Э����protocol 
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
		cout << "˫ʮһ����" << endl;
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