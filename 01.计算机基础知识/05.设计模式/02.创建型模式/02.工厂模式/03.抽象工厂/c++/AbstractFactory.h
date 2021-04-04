#pragma once
#include "Color.h"
#include "Shape.h"
#include <string>
using namespace std;

class AbstractFactory
{
public:
	AbstractFactory();
	virtual Color* getColor(string color);
	virtual Shape* getShape(string shape);
	virtual ~AbstractFactory();
};

