#pragma once
#include "ShapeDecorator.h"
#include <iostream>
using namespace std;

class RedShapeDecorator :
	public ShapeDecorator
{
public:
	RedShapeDecorator();
	RedShapeDecorator(shared_ptr<Shape> decoratedShape);
	virtual void draw();
	virtual ~RedShapeDecorator();

private:
	void setRedBorder(shared_ptr<Shape> decoratedShape);
};

