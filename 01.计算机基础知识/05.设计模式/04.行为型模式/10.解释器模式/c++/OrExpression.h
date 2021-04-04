#pragma once
#include "Expression.h"
#include <string>
using namespace std;

class OrExpression :
	public Expression
{
public:
	OrExpression();
	OrExpression(Expression* expr1, Expression* expr2);
	bool interpret(string context);
	virtual ~OrExpression();

private:
	Expression* expr1;
	Expression* expr2;
};

