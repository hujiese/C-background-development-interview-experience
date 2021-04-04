#include <iostream>
#include "Stock.h"
#include "Broker.h"
#include "BuyStock.h"
#include "SellStock.h"
using namespace std;

int main(void)
{
	shared_ptr<Stock> abcStock(new Stock());

	shared_ptr<BuyStock> buyStockOrder(new BuyStock(abcStock));
	shared_ptr<SellStock> sellStockOrder(new SellStock(abcStock));

	shared_ptr<Broker> broker(new Broker());
	broker->takeOrder(buyStockOrder);
	broker->takeOrder(sellStockOrder);

	broker->placeOrders();

	return 0;
}