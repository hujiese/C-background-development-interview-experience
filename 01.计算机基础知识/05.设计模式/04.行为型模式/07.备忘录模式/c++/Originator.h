#pragma once
#include <string>
#include "Memento.h"
using namespace std;
class Originator
{
public:
	Originator();
	void setState(string state);
	string getState() const;
	Memento* saveStateToMemento();
	void getStateFromMemento(Memento* memento);
	virtual ~Originator();

private:
	string state;
};

