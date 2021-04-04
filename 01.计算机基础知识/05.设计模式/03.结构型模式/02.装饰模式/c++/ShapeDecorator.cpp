#include "ShapeDecorator.h"


ShapeDecorator::ShapeDecorator()
{
}

ShapeDecorator::ShapeDecorator(shared_ptr<Shape> decoratedShape) :decoratedShape(decoratedShape)
{
}

void ShapeDecorator::draw()
{
	decoratedShape->draw();
}

ShapeDecorator::~ShapeDecorator()
{
}
