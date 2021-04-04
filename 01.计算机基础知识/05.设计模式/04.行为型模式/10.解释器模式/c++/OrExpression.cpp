#include "OrExpression.h"


OrExpression::OrExpression()
{
}

OrExpression::OrExpression(Expression* expr1, Expression* expr2)
{
	this->expr1 = expr1;
	this->expr2 = expr2;
}

bool OrExpression::interpret(string context)
{
	return expr1->interpret(context) || expr2->interpret(context);
}

OrExpression::~OrExpression()
{
}
