#include "Circle.h"


Circle::Circle()
{
}

Circle::Circle(int x, int y, int radius, shared_ptr<DrawAPI> drawAPI) :x(x), y(y), radius(radius), Shape(drawAPI)
{
}

void Circle::draw()
{
	drawAPI->drawCircle(radius, x, y);

}
Circle::~Circle()
{
}
