#include "Circle.h"


Circle::Circle()
{
}

Circle::Circle(const string color) :color(color)
{
}

void Circle::setX(const int x)
{
	this->x = x;
}

void Circle::setY(const int y)
{
	this->y = y;
}

void Circle::setRadius(const int radius)
{
	this->radius = radius;
}

void Circle::draw()
{
	cout << "Circle: Draw() [Color : " << color << ", x : " << x << ", y :" << y << ", radius :" << radius << endl;
}

Circle::~Circle()
{
}
