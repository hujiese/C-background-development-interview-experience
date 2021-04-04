#pragma once
#include <memory>
#include "Order.h"
#include "Stock.h"
class BuyStock :
	public Order
{
public:
	explicit BuyStock();
	BuyStock(shared_ptr<Stock> abcStock);
	void execute();
	virtual ~BuyStock();

private:
	shared_ptr<Stock> abcStock;
};

