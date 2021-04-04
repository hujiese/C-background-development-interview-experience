#include "SingleLazyObject.h"


SingleLazyObject::SingleLazyObject()
{
}

shared_ptr<SingleLazyObject> SingleLazyObject::getInstance()
{
	if (instance == NULL)
	{
		instance.reset(new SingleLazyObject());
	}
	return instance;
}

void SingleLazyObject::showMessage()
{
	cout << "Hello World!" << endl;
}

SingleLazyObject::~SingleLazyObject()
{
	cout << "dtor ...." << endl;
}
