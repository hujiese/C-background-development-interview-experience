
#include <iostream>
using namespace std;

// Current18 
// Current220
// Adapter 

class  Current18v
{
public:
	virtual void useCurrent18v() = 0;
};

class  Current220v
{
public:
	void useCurrent220v()
	{
		cout << "ÎÒÊÇ220v »¶Ó­Ê¹ÓÃ" << endl;
	}
};

class Adapter : public Current18v
{
public:
	Adapter(Current220v *current) :m_current(current)
	{
	}

	virtual void useCurrent18v()
	{
		cout << "ÊÊÅäÆ÷ ÊÊÅä 220v " << endl;
		m_current->useCurrent220v();
	}
private:
	Current220v *m_current;
};


int main(void)
{
	Current220v		*current220v = new Current220v();
	Adapter			*adapter = new Adapter(current220v);

	adapter->useCurrent18v();

	delete current220v ;
	delete adapter;

	return 0;
}