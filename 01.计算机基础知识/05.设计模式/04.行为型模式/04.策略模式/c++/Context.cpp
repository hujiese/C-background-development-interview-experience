#include "Context.h"


Context::Context()
{

}

Context::Context(std::shared_ptr<Strategy> strategy){
	this->strategy = strategy;
}

int Context::executeStrategy(int num1, int num2){
	return this->strategy->doOperation(num1, num2);
}

Context::~Context()
{
}
