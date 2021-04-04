#include "AbstractFactory.h"


AbstractFactory::AbstractFactory()
{
}

Color* AbstractFactory::getColor(string color)
{
	return NULL;
}
Shape* AbstractFactory::getShape(string shape)
{
	return NULL;
}

AbstractFactory::~AbstractFactory()
{
}
