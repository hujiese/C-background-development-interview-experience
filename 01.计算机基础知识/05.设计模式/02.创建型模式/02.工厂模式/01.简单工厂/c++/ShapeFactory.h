#pragma once
#include "Shape.h"
#include "Circle.h"
#include "Square.h"
#include "Rectangle.h"
#include <iostream>
#include <string>
using namespace std;

class ShapeFactory
{
public:
	ShapeFactory();
	Shape* getShape(string shapeType);
	virtual ~ShapeFactory();
};

