#pragma once
#include "State.h"
#include <string>
#include <iostream>
using namespace std;

class StartState :
	public State
{
public:
	StartState();
	virtual void doAction(Context* context);
	string toString();
	virtual ~StartState();
};

