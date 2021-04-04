#pragma once
#include "State.h"
#include <string>
#include <iostream>
using namespace std;
class StopState :
	public State
{
public:
	StopState();
	virtual void doAction(Context* context);
	string toString();
	virtual ~StopState();
};

