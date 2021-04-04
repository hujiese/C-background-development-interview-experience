#include <iostream>
#include "OperationAdd.h"
#include "OperationMultiply.h"
#include "OperationSubstract.h"
#include "Context.h"
using namespace std;

int main(void)
{
	shared_ptr<Strategy> add(new OperationAdd());
	shared_ptr<Context> context(new Context(add));
	cout << "10 + 5 = " << context->executeStrategy(10, 5) << endl;

	shared_ptr<Strategy> sub(new OperationSubstract());
	context.reset(new Context(sub));
	cout << "10 - 5 = " << context->executeStrategy(10, 5) << endl;

	shared_ptr<Strategy> mul(new OperationMultiply());
	context.reset(new Context(mul));
	cout << "10 * 5 = " << context->executeStrategy(10, 5) << endl;

	return 0;
}