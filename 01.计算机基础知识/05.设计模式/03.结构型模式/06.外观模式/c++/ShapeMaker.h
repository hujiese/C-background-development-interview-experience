#pragma once
#include <memory>
#include "Shape.h"
#include "Square.h"
#include "Rectangle.h"
#include "Circle.h"
using namespace std;

class ShapeMaker
{
public:
	ShapeMaker();
	void drawCircle();
	void drawRectangle();
	void drawSquare();
	virtual ~ShapeMaker();

private:
	shared_ptr<Shape> circle;
	shared_ptr<Shape> rectangle;
	shared_ptr<Shape> square;
};

