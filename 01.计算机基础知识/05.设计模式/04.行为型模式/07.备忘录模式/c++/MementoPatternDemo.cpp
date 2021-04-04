#include "CareTaker.h"
#include "Memento.h"
#include "Originator.h"
#include <iostream>
using namespace std;

int main(void)
{
	Originator* originator = new Originator();
	CareTaker* careTaker = new CareTaker();
	originator->setState("State #1");
	originator->setState("State #2");
	careTaker->add(originator->saveStateToMemento());
	originator->setState("State #3");
	careTaker->add(originator->saveStateToMemento());
	originator->setState("State #4");

	cout << "Current State: " << originator->getState() << endl;
	originator->getStateFromMemento(careTaker->get(0));
	cout << "First saved State: " << originator->getState() << endl;
	originator->getStateFromMemento(careTaker->get(1));
	cout << "Second saved State: " << originator->getState() << endl;

	delete originator;
	delete careTaker;

	return 0;
}