#pragma once
#include "Expression.h"
class TerminalExpression :
	public Expression
{
public:
	TerminalExpression();
	TerminalExpression(string data);
	bool interpret(string context);
	virtual ~TerminalExpression();

private:
	string data;
};

