#pragma once
#include "Strategy.h"
class OperationMultiply :
	public Strategy
{
public:
	OperationMultiply();
	int doOperation(int num1, int num2);
	virtual~OperationMultiply();
};

