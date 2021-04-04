#pragma once
#include "State.h"
#include <iostream>
using namespace std;
class Context
{
public:
	Context();
	void setState(State* state);
	State* getState();
	virtual ~Context();

private:
	State* state;
};

