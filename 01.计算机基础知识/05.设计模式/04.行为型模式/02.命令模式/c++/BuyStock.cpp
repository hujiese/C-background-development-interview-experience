#include "BuyStock.h"


BuyStock::BuyStock()
{
}

BuyStock::BuyStock(shared_ptr<Stock> abcStock) :abcStock(abcStock)
{
}

void BuyStock::execute()
{
	abcStock->buy();
}

BuyStock::~BuyStock()
{
}
