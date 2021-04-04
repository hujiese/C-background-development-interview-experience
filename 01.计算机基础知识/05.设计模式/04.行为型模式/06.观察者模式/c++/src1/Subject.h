#ifndef __SUBJECT_H__
#define __SUBJECT_H__
#pragma once
#include <list>
#include <vector>
#include "Observer.h"
using namespace std;
class Observer;
class Subject
{
public:
	Subject();
	int getState();
	void setState(int const state);
	void attach(weak_ptr<Observer> observer);
	void notifyAllObservers();
	virtual ~Subject();

private:
	list<weak_ptr<Observer>> observers;
	int state;
};

#endif