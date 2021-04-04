#pragma once
#include <list>
#include "Memento.h"
using namespace std;

class CareTaker
{
public:
	CareTaker();
	void add(Memento* state);
	Memento* get(int index) const;
	virtual ~CareTaker();

private:
	list<Memento *>* mementoList;
};

