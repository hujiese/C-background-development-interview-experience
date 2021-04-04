#include "Subject.h"


Subject::Subject()
{
}

int Subject::getState() {
	return state;
}

void Subject::setState(int const state) {
	this->state = state;
	notifyAllObservers();
}

void Subject::attach(weak_ptr<Observer> observer){
	observers.push_back(observer);
}

void Subject::notifyAllObservers(){
	for (weak_ptr<Observer> observer : observers) 
	{
		if (!observer.expired())
		{
			shared_ptr<Observer> ob = observer.lock();
			ob->update();
		}
	}
}

Subject::~Subject()
{
}
