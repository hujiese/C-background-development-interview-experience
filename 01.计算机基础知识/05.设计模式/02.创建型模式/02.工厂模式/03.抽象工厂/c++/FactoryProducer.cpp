#include "FactoryProducer.h"


FactoryProducer::FactoryProducer()
{
}

AbstractFactory* FactoryProducer::getFactory(string choice)
{
	if (choice == "SHAPE"){
		return new ShapeFactory();
	}
	else if (choice == "COLOR"){
		return new ColorFactory();
	}
	return NULL;
}

FactoryProducer::~FactoryProducer()
{
}
