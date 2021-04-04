#pragma once
#include <string>
using namespace std;
class Context;
class State
{
public:
	State();
	virtual void doAction(Context* context) = 0;
	virtual string toString() = 0;
	virtual ~State();
};

