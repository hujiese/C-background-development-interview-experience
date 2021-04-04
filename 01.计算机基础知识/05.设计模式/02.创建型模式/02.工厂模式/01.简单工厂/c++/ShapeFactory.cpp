#include "ShapeFactory.h"


ShapeFactory::ShapeFactory()
{
}

Shape* ShapeFactory::getShape(string shapeType)
{
	if (shapeType == ""){
		return NULL;
	}
	if (shapeType == "CIRCLE"){
		return new Circle();
	}
	else if (shapeType == "RECTANGLE"){
		return new Rectangle();
	}
	else if (shapeType == "SQUARE"){
		return new Square();
	}
	return NULL;
}

ShapeFactory::~ShapeFactory()
{
}
