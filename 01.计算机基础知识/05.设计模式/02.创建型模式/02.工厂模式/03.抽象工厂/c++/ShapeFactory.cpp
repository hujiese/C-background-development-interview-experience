#include "ShapeFactory.h"


ShapeFactory::ShapeFactory()
{
}

Shape* ShapeFactory::getShape(string shape)
{
	if (shape == ""){
		return NULL;
	}
	if (shape == "CIRCLE"){
		return new Circle();
	}
	else if (shape == "RECTANGLE"){
		return new Rectangle();
	}
	else if (shape == "SQUARE"){
		return new Square();
	}
	return NULL;
}

ShapeFactory::~ShapeFactory()
{
}
