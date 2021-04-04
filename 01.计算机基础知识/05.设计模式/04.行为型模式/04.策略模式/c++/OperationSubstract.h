#pragma once
#include "Strategy.h"
class OperationSubstract :
	public Strategy
{
public:
	OperationSubstract();
	int doOperation(int num1, int num2);
	virtual ~OperationSubstract();
};

