#include "Context.h"


Context::Context()
{
	state = NULL;
}

void Context::setState(State* state)
{
	this->state = state;
}

State* Context::getState()
{
	return state;
}

Context::~Context()
{
}
