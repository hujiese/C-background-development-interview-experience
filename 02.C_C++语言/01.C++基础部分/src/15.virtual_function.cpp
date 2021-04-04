#include<iostream>
using namespace std;
class Father{
public:
	virtual void print()
	{
		cout << "Father's print ...." << endl;
	}

	void printX()
	{
		cout << "Father's printX ...." << endl;
	}
};
class Son : public Father
{
public:
	virtual void print()
	{
		cout << "Son's print ...." << endl;
	}

	void printX()
	{
		cout << "Son's printX ...." << endl;
	}

	void prntY()
	{
		cout << "Son's printY ...." << endl;
	}
};
int main()
{
	Father father;
	father.print();
	father.printX();
	cout << "------------------------------------" << endl;
	Son son;
	son.print();
	son.printX();
	son.prntY();
	cout << "------------------------------------" << endl;
	Father* fa = &son;
	fa->print();
	fa->printX();
	//fa->printY(); //error !!!
	return 0;
}
