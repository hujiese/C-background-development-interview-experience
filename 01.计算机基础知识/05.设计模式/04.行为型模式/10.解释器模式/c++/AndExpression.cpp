#include "AndExpression.h"


AndExpression::AndExpression()
{
}

AndExpression::AndExpression(Expression* expr1, Expression* expr2) {
	this->expr1 = expr1;
	this->expr2 = expr2;
}


bool AndExpression::interpret(string context) {
	return expr1->interpret(context) && expr2->interpret(context);
}

AndExpression::~AndExpression()
{
}
