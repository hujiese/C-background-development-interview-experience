#pragma once
#include <memory>
#include "Strategy.h"

class Context
{
public:
	Context();

	Context(std::shared_ptr<Strategy>  strategy);

	int executeStrategy(int num1, int num2);

	virtual ~Context();

private:
	std::shared_ptr<Strategy> strategy;
};

