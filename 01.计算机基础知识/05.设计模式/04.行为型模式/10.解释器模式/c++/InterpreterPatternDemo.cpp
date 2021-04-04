#include <string>
#include <iostream>
#include "AndExpression.h"
#include "Expression.h"
#include "OrExpression.h"
#include "TerminalExpression.h"
using namespace std;

int main(void)
{
	Expression* robert = new TerminalExpression("Robert");
	Expression* john = new TerminalExpression("John");
	Expression* isMale = new OrExpression(robert, john);
	cout << "John is male? " << isMale->interpret("John") << endl;

	Expression* julie = new TerminalExpression("Julie");
	Expression* married = new TerminalExpression("Married");
	Expression* isMarriedWoman = new AndExpression(julie, married);
	cout << "Julie is a married women? " << isMarriedWoman->interpret("Married Julie") << endl;

	delete robert;
	delete john;
	delete isMale;

	delete julie;
	delete married;
	delete isMarriedWoman;
	return 0;
}