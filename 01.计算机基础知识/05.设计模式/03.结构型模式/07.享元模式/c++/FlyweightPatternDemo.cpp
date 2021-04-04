#include <iostream>
#include <map>
#include <string>
#include "Shape.h"
#include "Circle.h"
#include "ShapeFactory.h"
using namespace std;

map<string, shared_ptr<Shape>> ShapeFactory::circleMap;

int main(void)
{
	string colors[] = { "Red", "Green", "Blue", "White", "Black" };
	for (int i = 0; i < 20; ++i) {
		shared_ptr<Circle> circle((Circle *)(ShapeFactory::getCircle(colors[i % 5]).get()));
		circle->setX(i);
		circle->setY(i + 5);
		circle->setRadius(100);
		circle->draw();
	}

	return 0;
}