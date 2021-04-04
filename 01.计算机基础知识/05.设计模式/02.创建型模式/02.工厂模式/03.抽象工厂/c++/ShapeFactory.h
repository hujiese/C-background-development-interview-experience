#pragma once
#include "AbstractFactory.h"
#include "Circle.h"
#include "Square.h"
#include "Rectangle.h"

class ShapeFactory :
	public AbstractFactory
{
public:
	ShapeFactory();
	virtual Shape* getShape(string shape);
	virtual ~ShapeFactory();
};

