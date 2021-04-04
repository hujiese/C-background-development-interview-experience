#pragma once
#include <string>
#include "AbstractFactory.h"
#include "ShapeFactory.h"
#include "ColorFactory.h"

using namespace std;

class FactoryProducer
{
public:
	FactoryProducer();
	static AbstractFactory* getFactory(string choice);
	virtual ~FactoryProducer();
};

