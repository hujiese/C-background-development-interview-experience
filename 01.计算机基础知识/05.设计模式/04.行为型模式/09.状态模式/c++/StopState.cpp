#include "StopState.h"
#include "Context.h"

StopState::StopState()
{
}

void StopState::doAction(Context* context)
{
	cout << "Player is in start state" << endl;
	context->setState(this);
}

string StopState::toString()
{
	return "Start State";
}

StopState::~StopState()
{
}
