#pragma once
#include "Expression.h"
#include <string>
using namespace std;

class AndExpression :
	public Expression
{
public:
	AndExpression();
	AndExpression(Expression* expr1, Expression* expr2);
	bool interpret(string context);
	virtual ~AndExpression();

private:
	Expression* expr1;
	Expression* expr2;
};

