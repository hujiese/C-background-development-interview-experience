#include "SellStock.h"


SellStock::SellStock()
{
}

SellStock::SellStock(shared_ptr<Stock> abcStock) :abcStock(abcStock)
{
}

void SellStock::execute()
{
	abcStock->sell();
}

SellStock::~SellStock()
{
}
