#include "Stock.h"


Stock::Stock() :name("ABC"), quantity(10)
{
}

void Stock::buy()
{
	cout << "Stock [ Name: " << name << ", Quantity: " << quantity << " ] bought" << endl;
}

void Stock::sell()
{
	cout << "Stock [ Name: " << name << ", Quantity: " << quantity << " ] sold" << endl;
}

Stock::~Stock()
{
}
