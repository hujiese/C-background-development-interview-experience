#include "CareTaker.h"


CareTaker::CareTaker()
{
	mementoList = new list<Memento *>();
}

void CareTaker::add(Memento* state)
{
	mementoList->push_back(state);
}

Memento* CareTaker::get(int index) const
{
	list<Memento *>::iterator it = mementoList->begin();
	for (int i = 0; i < index; i++)
	{
		it++;
	}
	return *it;
}

CareTaker::~CareTaker()
{
	if (mementoList != NULL)
	{
		mementoList = NULL;
	}
}
