#pragma once
#include "Shape.h"
#include <iostream>
#include <memory>
using namespace std;
class ShapeDecorator :
	public Shape
{
public:
	ShapeDecorator();
	ShapeDecorator(shared_ptr<Shape> decoratedShape);
	virtual void draw();
	virtual ~ShapeDecorator();

protected:
	shared_ptr<Shape> decoratedShape;
};

