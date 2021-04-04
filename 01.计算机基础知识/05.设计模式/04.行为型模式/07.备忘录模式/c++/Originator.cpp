#include "Originator.h"


Originator::Originator()
{
}

void Originator::setState(string state)
{
	this->state = state;
}

string Originator::getState() const
{
	return state;
}

Memento* Originator::saveStateToMemento()
{
	return new Memento(state);
}

void Originator::getStateFromMemento(Memento* memento)
{
	state = memento->getState();
}

Originator::~Originator()
{
}
