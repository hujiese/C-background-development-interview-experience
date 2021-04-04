#include "ShapeMaker.h"


ShapeMaker::ShapeMaker() :circle(new Circle()), rectangle(new Rectangle()), square(new Square())
{

}

void ShapeMaker::drawCircle()
{
	circle->draw();
}

void ShapeMaker::drawRectangle()
{
	rectangle->draw();
}

void ShapeMaker::drawSquare()
{
	square->draw();
}

ShapeMaker::~ShapeMaker()
{
}
