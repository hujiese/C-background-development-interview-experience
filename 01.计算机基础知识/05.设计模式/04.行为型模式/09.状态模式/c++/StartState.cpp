#include "StartState.h"
#include "Context.h"

StartState::StartState()
{
}

void StartState::doAction(Context* context)
{
	cout << "Player is in start state" << endl;
	context->setState(this);
}

string StartState::toString()
{
	return "Start State";
}

StartState::~StartState()
{
}
