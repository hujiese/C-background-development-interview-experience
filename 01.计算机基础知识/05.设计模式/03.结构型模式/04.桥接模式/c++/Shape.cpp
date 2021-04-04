#include "Shape.h"


Shape::Shape()
{
}

Shape::Shape(shared_ptr<DrawAPI> drawAPI) :drawAPI(drawAPI)
{
}

void Shape::draw()
{

}

Shape::~Shape()
{
}
