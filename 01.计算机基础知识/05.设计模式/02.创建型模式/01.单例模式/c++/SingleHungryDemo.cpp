#include <iostream>
#include "SingleHungryObject.h"

using namespace std;
shared_ptr<SingleHungryObject> SingleHungryObject::instance(new SingleHungryObject);

int main(void)
{
	shared_ptr<SingleHungryObject> object = SingleHungryObject::getInstance();
	shared_ptr<SingleHungryObject> object1 = SingleHungryObject::getInstance();
	shared_ptr<SingleHungryObject> object2 = SingleHungryObject::getInstance();

	//ÏÔÊ¾ÏûÏ¢
	object->showMessage();
	object1->showMessage();
	object2->showMessage();

	return 0;
}