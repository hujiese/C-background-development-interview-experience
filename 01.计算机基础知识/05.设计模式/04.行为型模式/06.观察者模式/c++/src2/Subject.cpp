#include "Subject.h"


Subject::Subject()
{
}

void Subject::setMsg(const string msg)
{
	notifyAll(msg);
}

void Subject::addAttach(weak_ptr<Observer> observer)
{
	observers.push_back(observer);
}

void Subject::notifyAll(const string msg)
{
	for (weak_ptr<Observer> observer : observers) 
	{
		if (!observer.expired())
		{
			shared_ptr<Observer> obj = observer.lock();
			obj->update(msg);
		}
	}
}

Subject::~Subject()
{
}
