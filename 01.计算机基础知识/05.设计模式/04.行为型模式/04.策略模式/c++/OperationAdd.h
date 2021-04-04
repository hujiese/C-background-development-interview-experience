#pragma once
#include "Strategy.h"
class OperationAdd :
	public Strategy
{
public:
	OperationAdd();
	int doOperation(int num1, int num2);
	virtual ~OperationAdd();
};

