#include "SingleHungryObject.h"


SingleHungryObject::SingleHungryObject()
{
}

shared_ptr<SingleHungryObject> SingleHungryObject::getInstance()
{
	return instance;
}

void SingleHungryObject::showMessage()
{
	cout << "Hello World!" << endl;
}
SingleHungryObject::~SingleHungryObject()
{
	cout << "dotr ....." << endl;
}
