#include "ShapeFactory.h"


ShapeFactory::ShapeFactory()
{
}

shared_ptr<Shape> ShapeFactory::getCircle(const string color)
{
	shared_ptr<Shape> circle = NULL;
	map<string, shared_ptr<Shape>>::iterator it;
	it = circleMap.find(color);
	if (it == circleMap.end())
	{
		circle.reset(new Circle(color));
		circleMap.insert(pair<string, shared_ptr<Shape>>(color, circle));

		cout << "Creating circle of color : " << color << endl;
	}
	else
	{
		circle = it->second;
	}
	return circle;
}

ShapeFactory::~ShapeFactory()
{
}
