#include "BinaryObserver.h"


BinaryObserver::BinaryObserver(shared_ptr<Subject> subject)
{
	shared_ptr<Observer> temp(this);
	this->subject = subject;
	this->subject->attach(temp);
}

void BinaryObserver::update()
{
	printf("Binary String: %d \n", subject->getState());
}

BinaryObserver::~BinaryObserver()
{
}
