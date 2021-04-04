#pragma once
class Strategy
{
public:
	Strategy();
	virtual int doOperation(int num1, int num2) = 0;
	virtual ~Strategy();
};

