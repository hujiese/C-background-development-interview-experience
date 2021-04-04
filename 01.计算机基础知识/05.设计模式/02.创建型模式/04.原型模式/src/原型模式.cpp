#define _CRT_SECURE_NO_WARNINGS 
#include <iostream>
#include <string>
using namespace std;


class Person
{
public:
	virtual Person* clone() = 0;
	virtual void printT() = 0;
};

class  CPlusPlusProgrammer : public Person
{
public:
	CPlusPlusProgrammer() :m_name(""), m_age(0), m_resume(NULL)
	{
		setResume("aaaa");
	}

	CPlusPlusProgrammer(string name, int age) :m_name(name), m_age(age), m_resume(NULL)
	{
		setResume("aaaa");
	}

	void setResume(char *p)
	{
		if (m_resume != NULL)
		{
			delete m_resume;
		}
		m_resume = new char[strlen(p) + 1];
		strcpy(m_resume, p);
	}


	virtual void printT()
	{
		cout << "m_name" << m_name << " m_age" << m_age << "m_resume:" << m_resume <<  endl;
	}
	virtual Person* clone()
	{
		CPlusPlusProgrammer *tmp = new CPlusPlusProgrammer;
		//tmp->m_name = this->m_name;
		*tmp = *this; // =  Ç³¿½±´
		return tmp;
	}
private:
	string	m_name;
	int		m_age ;
	char	*m_resume;
		
};

int main(void)
{
	Person *c1 = new CPlusPlusProgrammer("ÕÅÈý", 32);
	c1->printT();


	Person *c2 = c1->clone();
	c2->printT();
	return 0;
}