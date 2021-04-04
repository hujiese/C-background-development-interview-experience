#include <iostream>
#include <memory>

class Child;
class Parent;

class Parent 
{
private:
	Child* myChild;
public:
	void setChild(Child* ch) 
	{
		this->myChild = ch;
	}

	void doSomething() 
	{
		if (this->myChild) 
		{

		}
	}

	~Parent() 
	{
		delete myChild;
		std::cout << "Parent's dtor" << std::endl;
	}
};

class Child 
{
private:
	Parent* myParent;
public:
	void setPartent(Parent* p) 
	{
		this->myParent = p;
	}
	void doSomething() 
	{
		if (this->myParent) 
		{

		}
	}
	~Child() 
	{
		delete myParent;
		std::cout << "Child's dtor" << std::endl;
	}
};

int main() 
{
	{
		Parent* p = new Parent;
		Child* c = new Child;
		p->setChild(c);
		c->setPartent(p);
		delete c;  //only delete one
	}
	return 0;
}