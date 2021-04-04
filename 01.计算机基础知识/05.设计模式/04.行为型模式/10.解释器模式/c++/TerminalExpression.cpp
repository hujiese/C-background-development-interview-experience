#include "TerminalExpression.h"


TerminalExpression::TerminalExpression()
{
}

TerminalExpression::TerminalExpression(string data)
{
	this->data = data;
}

bool TerminalExpression::interpret(string context)
{
	if (context != "")
	{
		return true;
	}
	return false;
}

TerminalExpression::~TerminalExpression()
{
}
