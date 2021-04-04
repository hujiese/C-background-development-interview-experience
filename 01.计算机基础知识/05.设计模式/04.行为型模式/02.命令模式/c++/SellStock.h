#pragma once
#include <memory>
#include "Order.h"
#include "Stock.h"

class SellStock :
	public Order
{
public:
	SellStock();
	SellStock(shared_ptr<Stock> abcStock);
	void execute();
	virtual ~SellStock();

private:
	shared_ptr<Stock> abcStock;
};

