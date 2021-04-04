#include "Broker.h"


Broker::Broker()
{
}

void Broker::takeOrder(shared_ptr<Order> order)
{
	orderList.push_back(order);
}

void Broker::placeOrders()
{
	for (shared_ptr<Order> order : orderList) 
	{
		order->execute();
	}
	orderList.empty();
}

Broker::~Broker()
{
}
