#pragma once
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include "Shape.h"
#include "Circle.h"
using namespace std;
class ShapeFactory
{
public:
	ShapeFactory();
	static shared_ptr<Shape> getCircle(const string color);
	virtual ~ShapeFactory();

private :
	static map<string, shared_ptr<Shape>> circleMap;
};

