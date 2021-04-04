#include "Memento.h"


Memento::Memento()
{
}

Memento::Memento(string state) :state(state)
{}

string Memento::getState() const
{
	return this->state;
}

Memento::~Memento()
{
}
