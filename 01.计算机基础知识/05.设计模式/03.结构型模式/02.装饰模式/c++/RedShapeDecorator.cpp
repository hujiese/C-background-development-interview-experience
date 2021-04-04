#include "RedShapeDecorator.h"


RedShapeDecorator::RedShapeDecorator()
{
}

RedShapeDecorator::RedShapeDecorator(shared_ptr<Shape> decoratedShape) :ShapeDecorator(decoratedShape)
{
}

void RedShapeDecorator::draw() {
	decoratedShape->draw();
	setRedBorder(decoratedShape);

}

void RedShapeDecorator::setRedBorder(shared_ptr<Shape> decoratedShape)
{
	cout << "Border Color: Red" << endl;
}

RedShapeDecorator::~RedShapeDecorator()
{
}
