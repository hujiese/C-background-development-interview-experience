#include <iostream>
#include "SingleLazyObject.h"

using namespace std;
shared_ptr<SingleLazyObject> SingleLazyObject::instance = NULL;

int main(void)
{
	shared_ptr<SingleLazyObject> object = SingleLazyObject::getInstance();
	shared_ptr<SingleLazyObject> object2 = SingleLazyObject::getInstance();
	shared_ptr<SingleLazyObject> object3 = SingleLazyObject::getInstance();
	//ÏÔÊ¾ÏûÏ¢
	object->showMessage();
	object2->showMessage();
	object3->showMessage();

	return 0;
}