#include "Context.h"
#include "StartState.h"
#include "State.h"
#include "StopState.h"
using namespace std;

int main(void)
{
	Context* context = new Context();

	StartState* startState = new StartState();
	startState->doAction(context);

	cout << context->getState()->toString() << endl;

	StopState* stopState = new StopState();
	stopState->doAction(context);

	cout << context->getState()->toString() << endl;

	delete context;
	delete startState;
	delete stopState;
	return 0;
}