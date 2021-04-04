#include "OctalObserver.h"


OctalObserver::OctalObserver(shared_ptr<Subject> subject)
{
	shared_ptr<Observer> temp(this);
	this->subject = subject;
	this->subject->attach(temp);
}

void OctalObserver::update()
{
	printf("Octal String: %o\n", subject->getState());
}

OctalObserver::~OctalObserver()
{
}
