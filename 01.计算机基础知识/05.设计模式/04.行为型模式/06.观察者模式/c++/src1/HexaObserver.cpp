#include "HexaObserver.h"


HexaObserver::HexaObserver(shared_ptr<Subject> subject)
{
	shared_ptr<Observer> temp(this);
	this->subject = subject;
	this->subject->attach(temp);
}

void HexaObserver::update()
{
	printf("Hexa String: %x\n", subject->getState());
}

HexaObserver::~HexaObserver()
{
}
