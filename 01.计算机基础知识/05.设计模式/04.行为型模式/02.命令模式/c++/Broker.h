#pragma once
#include "Order.h"
#include <list>
#include <memory>
using namespace std;
class Broker
{
public:
	Broker();
	void takeOrder(shared_ptr<Order> order);
	void placeOrders();
	virtual ~Broker();

private:
	list<shared_ptr<Order>> orderList;
};

