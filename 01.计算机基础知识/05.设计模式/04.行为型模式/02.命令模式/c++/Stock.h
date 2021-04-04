#pragma once
#include <iostream>
#include <string>
using namespace std;

class Stock
{
public:
	Stock();
	void buy();
	void sell();
	virtual ~Stock();

protected:
	string name;
	int quantity;
};

