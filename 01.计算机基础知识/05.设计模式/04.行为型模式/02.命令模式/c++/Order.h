#pragma once
class Order
{
public:
	Order();
	virtual void execute() = 0;
	virtual ~Order();
};

